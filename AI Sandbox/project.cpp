#define _CRTDBG_MAP_ALLOC

#include "../commons.h"
#include <windows.h>
#include <windowsx.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <crtdbg.h>
#include <stdio.h>
#include <time.h>
#include "Resource.h"

HDC			hDC=NULL;
HGLRC		hRC=NULL;
HWND		hWnd=NULL;
HINSTANCE	hInstance;

bool	keys[256];
bool	active=true;
bool	fullscreen=false;

/********************************************************************************************/

#define TNG_CREATURE				5

#define TNG_CREATURE_SPIDER			26
#define TNG_CREATURE_SKELETON		15
#define TNG_CREATURE_HORNY			14
#define TNG_CREATURE_IMP			23
#define TNG_CREATURE_BEETLE			24
#define TNG_CREATURE_BILE_DEMON		22
#define TNG_CREATURE_DEMON_SPAWN	18
#define TNG_CREATURE_HELL_HOUND		27
#define TNG_CREATURE_VAMPIRE		25
#define TNG_CREATURE_DRAGON			17

#define WINDOW_TITLE "AI_SANDBOX"

#define SCREEN_WIDTH	1024
#define SCREEN_HEIGHT	768

#include <cml/cml.h>
#include "../TextureLoader.h"
#include "../OGLUtils.h"
#include <map>
#include <string>
#include "BlockData.h"
#include "AIHelper.h"
#include "SandboxAIHelper.h"

using namespace loaders;
using namespace std;
using namespace utils;
using namespace game_utils;
using namespace AI;
using namespace cml;

map<int, GLuint>	textureMap;
typedef CBlockData	tdLevelBlocks[85][85];
vector3f			camPos(0.0f,0.0f,-120.0f),
					initCamPos(0.0f,0.0f,-120.0f);

bool				zoomed = false;

// loads block types, ownership, things
class CLevelDescriptor
{
public:

	static void describe(string fileName, tdLevelBlocks blockData)
	{
		loadSLB(fileName+".slb",blockData);
		loadOWN(fileName+".own",blockData);
		loadTNG(fileName+".tng",blockData);
	}

private:

    static void loadSLB(string fileName, tdLevelBlocks blockData)
	{
		struct sSLB
		{
			GLbyte typeID;	// ID of the block
			GLbyte empty;	// empty not used reserved value
		};

		// open file and read data
		FILE *inSLB;

		if (!(inSLB=fopen(fileName.c_str(),"rb")))
		{
			return;
		}

		sSLB slb[CV_LEVEL_MAP_SIZE][CV_LEVEL_MAP_SIZE];

		for (GLuint i=0; i<CV_LEVEL_MAP_SIZE; i++)
		{
			if (fread(slb[i],CV_LEVEL_MAP_SIZE,sizeof(sSLB),inSLB)==0)
			{
				fclose(inSLB);
				return;
			}
		}

		fclose(inSLB);			

		// process the data
		for (GLint y=0; y<CV_LEVEL_MAP_SIZE; y++)
		{
			for (GLint x=0; x<CV_LEVEL_MAP_SIZE; x++)
			{
				blockData[y][x].type = (int)slb[y][x].typeID;
			}
		}
	}

	static void loadOWN(string fileName, tdLevelBlocks blockData)
	{
		FILE *inOWN=NULL;
		if (!(inOWN=fopen(fileName.c_str(),"rb")))
		{
			return;
		}

		GLubyte line[256][256];

		for (GLint i=0; i<256; i++)
		{
			fread(line[i],1,256,inOWN);
		}

		fclose(inOWN);

		for (GLint y=0; y<CV_LEVEL_MAP_SIZE; y++)
		{
			for (GLint x=0; x<CV_LEVEL_MAP_SIZE; x++)
			{
				blockData[y][x].owner = line[y*3][x*3];
			}
		}
	}

	static void loadTNG(string fileName, tdLevelBlocks blockData)
	{
		FILE *f_tng = NULL;

		if (!(f_tng=fopen(fileName.c_str(),"rb")))
		{
			return;
		}

		GLshort things_count=0;
		GLubyte thing_data[21];

		GLubyte thing_type;
		GLubyte thing_subtype;
		GLubyte owner;

		GLubyte subtile_x,subtile_y;

		GLint map_x,map_y;

		GLfloat thing_x,thing_y;

		srand(time(NULL));

		// read the number of thing in the dk_map
		fread(&things_count,1,2,f_tng);			

		for (GLshort i=0; i<things_count; i++)
		{
			fread(thing_data,1,21,f_tng);

			thing_type=thing_data[6];
			thing_subtype=thing_data[7];
			owner=thing_data[8];

			subtile_x=thing_data[1];
			subtile_y=thing_data[3];

			map_x=(GLint)subtile_x/3;
			map_y=(GLint)subtile_y/3;

			thing_x=(GLfloat)map_x*CV_BLOCK_WIDTH+(GLfloat)(subtile_x%3)*(CV_BLOCK_WIDTH/3.0f)+(CV_BLOCK_WIDTH/6.0f);
			thing_y=(GLfloat)map_y*CV_BLOCK_DEPTH+(GLfloat)(subtile_y%3)*(CV_BLOCK_DEPTH/3.0f)+(CV_BLOCK_DEPTH/6.0f);

			switch (thing_type)
			{
				/*case TNG_ITEM_DECORATION:
				{
				}*/
				// creature
				case TNG_CREATURE:
				{
					if (thing_subtype==TNG_CREATURE_IMP)
					{		
						int a=3;
						//CV_GAME_MANAGER->getConsole()->writeLine("CREATED IMP");
					}
					break;
				}
				// room effect
				/*case 7:
				{
					break;
				}
				// trap
				case TNG_TRAP:
				{		
				}
				// door
				case 9:
				{
					break;
				}*/
			}
		}

		fclose(f_tng);
	}
};

tdLevelBlocks levelBlocks;

/********************************************************************************************/

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)
{
	if (height==0)
	{
		height=1;
	}

	glViewport(0,0,width,height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,1000.0f);

	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();

	//COGLUtils::toGLOrtho(width,height,true);
}

void DetectMemoryLeaks() 
{ 
   _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF|_CRTDBG_ALLOC_MEM_DF); 
   _CrtSetReportMode(_CRT_ASSERT,_CRTDBG_MODE_FILE); 
   _CrtSetReportFile(_CRT_ASSERT,_CRTDBG_FILE_STDERR); 
} 

int InitGL(GLvoid)
{
	COGLUtils::setVSync(1);

	DetectMemoryLeaks();

	glShadeModel(GL_SMOOTH);
	glClearColor(0.4f, 0.4f, 0.8f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glAlphaFunc(GL_GREATER,0.1f);
	glEnable(GL_ALPHA_TEST);

	// load textures
	GLuint texture = 0;

	CTextureLoader::buildTexture("textures/earth.gif",texture);
	textureMap[CV_BLOCK_TYPE_EARTH_ID] = texture;

	CTextureLoader::buildTexture("textures/gem.gif",texture);
	textureMap[CV_BLOCK_TYPE_GEM_ID] = texture;

	CTextureLoader::buildTexture("textures/gold.gif",texture);
	textureMap[CV_BLOCK_TYPE_GOLD_ID] = texture;

	CTextureLoader::buildTexture("textures/lava.gif",texture);
	textureMap[CV_BLOCK_TYPE_LAVA_ID] = texture;

	CTextureLoader::buildTexture("textures/rock.gif",texture);
	textureMap[CV_BLOCK_TYPE_ROCK_ID] = texture;

	CTextureLoader::buildTexture("textures/water.gif",texture);
	textureMap[CV_BLOCK_TYPE_WATER_ID] = texture;

	CTextureLoader::buildTexture("textures/wall.gif",texture);
	textureMap[CV_BLOCK_TYPE_WALL_ID] = texture;

	CTextureLoader::buildTexture("textures/unclaimed.gif",texture);
	textureMap[CV_BLOCK_TYPE_UNCLAIMED_LAND_ID] = texture;

	CTextureLoader::buildTexture("textures/claimed_player0.gif",texture);
	textureMap[CV_BLOCK_TYPE_CLAIMED_LAND_ID] = texture;

	// prepare block data
	CLevelDescriptor::describe("../data/resources/levels/MAP00005",levelBlocks);

	return TRUE;
}

void drawDungeon(int tileSize)
{	
	int startY = (CV_LEVEL_MAP_SIZE*tileSize)/2;
	int startX = -(CV_LEVEL_MAP_SIZE*tileSize)/2;

	for (GLint y=0; y<CV_LEVEL_MAP_SIZE; y++)
	{
		for (GLint x=0; x<CV_LEVEL_MAP_SIZE; x++)
		{
			bool contains = textureMap.find(levelBlocks[y][x].type) != textureMap.end();

			if (contains)
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,textureMap[levelBlocks[y][x].type]);
			}

			/*int posx = x * tileSize;
			int posy = y * tileSize;*/

			int posx = startX + x * tileSize;
			int posy = startY - y * tileSize;

			glBegin(GL_QUADS);
			{
				glTexCoord2f(0.0f,1.0f);
				glVertex2i(posx,posy);

				glTexCoord2f(1.0f,1.0f);
				glVertex2i(posx+tileSize,posy);

				glTexCoord2f(1.0f,0.0f);
				glVertex2i(posx+tileSize,posy+tileSize);

				glTexCoord2f(0.0f,0.0f);
				glVertex2i(posx,posy+tileSize);
			}
			glEnd();

			if (contains)
			{
				glDisable(GL_TEXTURE_2D);
			}
		}
	}
}

int DrawGLScene(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(camPos[0],camPos[1],camPos[2]);
	drawDungeon(1);
	//glTranslated(0.0f,0.0f,-20.0f);
	
	return TRUE;
}

GLvoid KillGLWindow(GLvoid)
{
	if (fullscreen)
	{
		ChangeDisplaySettings(NULL,0);
		ShowCursor(TRUE);								
	}

	if (hRC)											
	{
		if (!wglMakeCurrent(NULL,NULL))					
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										
	}

	if (hWnd && !DestroyWindow(hWnd))					
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										
	}

	if (!UnregisterClass("OpenGL",hInstance))			
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									
	}
}
 
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			
	WNDCLASS	wc;						
	DWORD		dwExStyle;				
	DWORD		dwStyle;				
	RECT		WindowRect;				
	WindowRect.left=(long)0;			
	WindowRect.right=(long)width;		
	WindowRect.top=(long)0;				
	WindowRect.bottom=(long)height;		

	fullscreen=fullscreenflag;			

	hInstance			= GetModuleHandle(NULL);		
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	
	wc.lpfnWndProc		= (WNDPROC) WndProc;					
	wc.cbClsExtra		= 0;									
	wc.cbWndExtra		= 0;									
	wc.hInstance		= hInstance;							
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			
	wc.hbrBackground	= NULL;									
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= "OpenGL";								

	if (!RegisterClass(&wc))									
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											
	}
	
	if (fullscreen)												
	{
		DEVMODE dmScreenSettings;								
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		
		dmScreenSettings.dmPelsWidth	= width;				
		dmScreenSettings.dmPelsHeight	= height;				
		dmScreenSettings.dmBitsPerPel	= bits;					
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
		
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{			
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;
			}
			else
			{
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;
			}
		}
	}

	if (fullscreen)	
	{
		dwExStyle=WS_EX_APPWINDOW;								
		dwStyle=WS_POPUP;										
		ShowCursor(FALSE);										
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			
		dwStyle=WS_OVERLAPPEDWINDOW;							
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		

	if (!(hWnd=CreateWindowEx(	dwExStyle,							
								"OpenGL",							
								title,								
								dwStyle |							
								WS_CLIPSIBLINGS |					
								WS_CLIPCHILDREN,					
								0, 0,								
								WindowRect.right-WindowRect.left,	
								WindowRect.bottom-WindowRect.top,	
								NULL,								
								NULL,								
								hInstance,							
								NULL)))								
	{
		KillGLWindow();								
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	static	PIXELFORMATDESCRIPTOR pfd=				
	{
		sizeof(PIXELFORMATDESCRIPTOR),				
		1,											
		PFD_DRAW_TO_WINDOW |						
		PFD_SUPPORT_OPENGL |						
		PFD_DOUBLEBUFFER,							
		PFD_TYPE_RGBA,								
		bits,										
		0, 0, 0, 0, 0, 0,							
		0,											
		0,											
		0,											
		0, 0, 0, 0,									
		16,											
		0,											
		0,											
		PFD_MAIN_PLANE,								
		0,											
		0, 0, 0										
	};
	
	if (!(hDC=GetDC(hWnd)))							
	{
		KillGLWindow();								
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	
	{
		KillGLWindow();								
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		
	{
		KillGLWindow();								
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if (!(hRC=wglCreateContext(hDC)))				
	{
		KillGLWindow();								
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if(!wglMakeCurrent(hDC,hRC))					
	{
		KillGLWindow();								
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	ShowWindow(hWnd,SW_SHOW);						
	SetForegroundWindow(hWnd);						
	SetFocus(hWnd);									
	ReSizeGLScene(width, height);					

	if (!InitGL())									
	{
		KillGLWindow();								
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	return TRUE;									
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)			
{
	switch (uMsg)									
	{
		case WM_ACTIVATE:							
		{
			if ((LOWORD(wParam) != WA_INACTIVE) && !((BOOL)HIWORD(wParam)))
				active=TRUE;						
			else
				active=FALSE;

			return 0;								
		}

		case WM_SYSCOMMAND:							
		{
			switch (wParam)							
			{
				case SC_SCREENSAVE:					
				case SC_MONITORPOWER:				
				return 0;							
			}
			break;									
		}

		case WM_CLOSE:								
		{
			PostQuitMessage(0);						
			return 0;								
		}

		case WM_KEYDOWN:							
		{
			keys[wParam] = TRUE;					
			return 0;								
		}

		case WM_KEYUP:								
		{
			keys[wParam] = FALSE;					
			return 0;								
		}

		case WM_SIZE:								
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  
			return 0;								
		}		

		case WM_LBUTTONDOWN:
		{
			return 0;
		}

		case WM_RBUTTONDOWN:
		{			
			if (!zoomed)
			{
				// pick clicked block and zoom to it
			}
			else
			{
				camPos = initCamPos;
			}

			zoomed = !zoomed;
			return 0;
		}

		case WM_RBUTTONUP:
		{
			return 0;
		}

		case WM_LBUTTONUP:
		{
			return 0;
		}

		case WM_MOUSEMOVE:
		{
			return 0;
		}
	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,			
					HINSTANCE	hPrevInstance,		
					LPSTR		lpCmdLine,			
					int			nCmdShow)			
{
	MSG		msg;									
	BOOL	done=FALSE;								
	
	/*if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{	
		fullscreen=FALSE;
	}*/

	if (!CreateGLWindow(WINDOW_TITLE,SCREEN_WIDTH,SCREEN_HEIGHT,32,fullscreen))
	{
		return 0;									
	}

	while(!done)									
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	
		{
			if (msg.message==WM_QUIT)				
			{
				done=TRUE;							
			}
			else									
			{
				TranslateMessage(&msg);				
				DispatchMessage(&msg);				
			}
		}
		else										
		{
			
			if ((active && !DrawGLScene()) || keys[VK_ESCAPE])	
			{
				done=TRUE;							
			}
			else									
			{
				SwapBuffers(hDC);					
			}

			if (keys[VK_F1])						
			{
				keys[VK_F1]=FALSE;					
				KillGLWindow();						
				fullscreen=!fullscreen;				
				
				if (!CreateGLWindow(WINDOW_TITLE,SCREEN_WIDTH,SCREEN_HEIGHT,32,fullscreen))
				{
					return 0;						
				}
			}
		}
	}

	KillGLWindow();									
	return (msg.wParam);							
}