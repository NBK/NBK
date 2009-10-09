#define _CRTDBG_MAP_ALLOC

#define NOMINMAX

#include "GameManager.h"
#include "commons.h"
#include <windows.h>
#include <windowsx.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <crtdbg.h>
#include <stdio.h>
#include <time.h>
#include "Resource.h"
#include "BugTrap\BugTrap.h"

HDC			hDC=NULL;
HGLRC		hRC=NULL;
HWND		hWnd=NULL;
HINSTANCE	hInstance;

bool	active=true,
		fullscreen=false,
		esc=false;

/********************************************************************************************/

#include <string>
#include "Conversions.h"
#include "OGLUtils.h"
#include "DeltaTime.h"
#include "LUA/lua_common.h"
#include "utils.h"

using namespace std;
using namespace game_utils;
//using namespace game_utils::managers;
using namespace utils;
using namespace control;

#define WINDOW_TITLE "Natural Born Keeper Extreme"

managers::CGameManager *game_utils::CV_GAME_MANAGER = NULL;
bool allInited = false;

std::string	G_startDir = "";

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

	gluPerspective(CV_SETTINGS_FOV,(GLfloat)width/(GLfloat)height,0.01f,10000.0f);

	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();

	if (allInited)
	{
		CV_GAME_MANAGER->getControlManager()->getViewFrustum()->updateRatio((GLfloat)width/(GLfloat)height);
		//CV_GAME_MANAGER->getConsole()->writeLine("Ratio changed: "+CConversions::floatToStr((GLfloat)width/(GLfloat)height));

		CV_GAME_MANAGER->getGUIManager()->getPlayGUI()->get_active_gui()->updateRatio(width,height);

		game_utils::CV_SETTINGS_WINDOW_WIDTH = width;
		game_utils::CV_SETTINGS_WINDOW_HEIGHT = height;
		game_utils::CV_SETTINGS_WINDOW_WIDTH_HALF = CV_SETTINGS_WINDOW_WIDTH/2;
		game_utils::CV_SETTINGS_WINDOW_HEIGHT_HALF = CV_SETTINGS_WINDOW_HEIGHT/2;
	}
}

void DetectMemoryLeaks() 
{ 
   _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF|_CRTDBG_ALLOC_MEM_DF); 
   _CrtSetReportMode(_CRT_ASSERT,_CRTDBG_MODE_FILE); 
   _CrtSetReportFile(_CRT_ASSERT,_CRTDBG_FILE_STDERR); 
} 

// extern from commons
HWND game_utils::CV_WINDOW_HANDLE;
HDC	game_utils::CV_DEVICE_CONTEXT;

GLint game_utils::CV_SETTINGS_WINDOW_WIDTH;
GLint game_utils::CV_SETTINGS_WINDOW_HEIGHT;
GLint game_utils::CV_SETTINGS_WINDOW_WIDTH_HALF;
GLint game_utils::CV_SETTINGS_WINDOW_HEIGHT_HALF;

//TODO: remove LUA, add lighting with fixed functionality

BOOL initGameSystems()
{
	// toggle VSYNC
	COGLUtils::setVSync(false);

	// set global hwnd
	CV_WINDOW_HANDLE = hWnd;
	CV_DEVICE_CONTEXT = hDC;

	// used for particle system init
	G_startDir = utils::sGeneralUtils::getCurrentDirectory();

	// initialize game manager which initializes all other managers
	CV_GAME_MANAGER = new managers::CGameManager();
	if (!CV_GAME_MANAGER->init())
	{
		MessageBox(hWnd,"ERROR INITIALIZING GAME SYSTEMS!","ERROR",MB_OK|MB_ICONERROR);
		return FALSE;
	}

	allInited = true;

	return TRUE;
}

BOOL cleanupGameSystems()
{
	CV_GAME_MANAGER->shutdown();
	delete CV_GAME_MANAGER;

	return TRUE;
}

int InitGL(GLvoid)
{	
	DetectMemoryLeaks();
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glAlphaFunc(GL_GREATER,0.1f);
	glEnable(GL_ALPHA_TEST);	

	return initGameSystems();
}

int DrawGLScene(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	CV_GAME_MANAGER->update();

	// TODO: rooms || lighting system
	// TODO: collision detection
	
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
		//ShowCursor(FALSE);										
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
	// Update input manager, no KB hook needed.
	if (allInited)
	{
		CV_GAME_MANAGER->getControlManager()->getInput()->update(uMsg,wParam,lParam);
		CV_GAME_MANAGER->getGUIManager()->getPlayGUI()->get_active_gui()->get_input_controler()->update(uMsg,wParam,lParam);
	}

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
			esc = (wParam == VK_ESCAPE);			
			return 0;								
		}

		case WM_KEYUP:								
		{
			return 0;								
		}

		case WM_SIZE:								
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  
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
	// Setup exception handler
	BT_SetAppName("NBKe");
	BT_SetSupportEMail("wtfo-bugs@keeperklan.com");
	BT_SetFlags(BTF_DETAILEDMODE | BTF_DESCRIBEERROR | BTF_INTERCEPTSUEF | BTF_EDITMAIL | BTF_ATTACHREPORT | BTF_SCREENCAPTURE);
	BT_SetSupportURL("http://forum.keeperklan.com");
	BT_InstallSehFilter();

	MSG		msg;									
	BOOL	done=FALSE;

	CV_SETTINGS_WINDOW_WIDTH = 1024;
	CV_SETTINGS_WINDOW_HEIGHT = 768;
	CV_SETTINGS_WINDOW_WIDTH_HALF = CV_SETTINGS_WINDOW_WIDTH/2;
	CV_SETTINGS_WINDOW_HEIGHT_HALF = CV_SETTINGS_WINDOW_HEIGHT/2;

	if (!CreateGLWindow(WINDOW_TITLE,CV_SETTINGS_WINDOW_WIDTH,CV_SETTINGS_WINDOW_HEIGHT,32,CV_SETTINGS_WINDOW_FULLSCREEN))
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
			
			if ((active && !DrawGLScene()) || esc)	
			{				
				done=TRUE;							
			}
			else									
			{
				SwapBuffers(hDC);
			}
		}
	}
	KillGLWindow();	

	// cleanup
	cleanupGameSystems();

	return (msg.wParam);							
}