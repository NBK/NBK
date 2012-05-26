
#ifdef USE_LOCAL_HEADERS
#	include "SDL.h"
#	include "SDL_cpuinfo.h"
#else
#	include <SDL.h>
#	include <SDL_cpuinfo.h>
#endif

#include "system.h"

#include "GameManager.h"
#include "commons.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <time.h>

bool	active=true,
		fullscreen=false;

/********************************************************************************************/

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

#define WINDOW_TITLE "Natural Born Keeper"

managers::CGameManager *game_utils::CV_GAME_MANAGER = NULL;
bool allInited = false;

std::string G_startDir = "";

/********************************************************************************************/

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)
{
	if (height==0)
	{
		height=1;
	}

	glViewport(0,0,width,height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(CV_SETTINGS_FOV,(GLfloat)width/(GLfloat)height,0.01f,100.0f);

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

// extern from commons
GLint game_utils::CV_SETTINGS_WINDOW_WIDTH;
GLint game_utils::CV_SETTINGS_WINDOW_HEIGHT;
GLint game_utils::CV_SETTINGS_WINDOW_WIDTH_HALF;
GLint game_utils::CV_SETTINGS_WINDOW_HEIGHT_HALF;

//TODO: remove LUA, add lighting with fixed functionality

bool initGameSystems()
{
	// toggle input grab state
	// TODO Tequila: Support no input grab state
	//SDL_WM_GrabInput(SDL_GRAB_ON);
	
	// toggle VSYNC
	COGLUtils::setVSync(false);

	// used for particle system init
	char *current_path = get_current_dir_name();
	G_startDir = std::string(current_path);
	SAFE_DELETE(current_path);

	// initialize game manager which initializes all other managers
	CV_GAME_MANAGER = new managers::CGameManager();
	if (!CV_GAME_MANAGER->init())
	{
		printf("[ERROR|GAME] ERROR INITIALIZING GAME SYSTEMS!\n");
		return FALSE;
	}

	allInited = true;

	return TRUE;
}

bool cleanupGameSystems()
{
	CV_GAME_MANAGER->shutdown();
	delete CV_GAME_MANAGER;
	return TRUE;
}

void InitGL()
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glAlphaFunc(GL_GREATER,0.1f);
	glEnable(GL_ALPHA_TEST);
    //glEnable(GL_TEXTURE_2D);			// Enable Texture Mapping
}

bool DrawGLScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	CV_GAME_MANAGER->update();

	// TODO: rooms || lighting system
	// TODO: collision detection

	SDL_GL_SwapBuffers();

	return TRUE;
}

int checkSDLError(int line = -1)
{
	int err = FALSE;
	const char *error = SDL_GetError();
	if (*error != '\0')
	{
		err = TRUE;
		if (line != -1)
			printf("[ERROR|SDL] line %i: %s\n", line, error);
		else
			printf("[ERROR|SDL] %s\n", error);
		SDL_ClearError();
	}
	return err;
}

void OutputSDLError(const char *msg)
{
	printf("[ERROR|SDL] %s: %s\n", msg, SDL_GetError());
}

bool CreateGLWindow(int width, int height, int bits, bool fullscreenflag)
{
	fullscreen=fullscreenflag;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		OutputSDLError("Unable to initialize SDL");
		return FALSE;
	}

	atexit(SDL_Quit);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	if (!SDL_SetVideoMode(width, height, 0, SDL_OPENGL))
	{
		OutputSDLError("Failed To create SDL video mode");
		return FALSE;
	}

	SDL_WM_SetCaption(WINDOW_TITLE, NULL);

	if (fullscreen)
	{
		// TODO Tequila: Support setting fullscreen mode
	}

	InitGL();
	ReSizeGLScene(width, height);
	SDL_GL_SwapBuffers();

	if (!initGameSystems())
	{
		OutputSDLError("Initialization Failed");
		return FALSE;
	}

	return TRUE;
}

/*
=================
main
=================
*/
int main( int argc, char **argv )
{
	bool done=FALSE;

	// TODO Tequila: Don't set screen size as hard-coded values
	CV_SETTINGS_WINDOW_WIDTH = 1024;
	CV_SETTINGS_WINDOW_HEIGHT = 768;
	CV_SETTINGS_WINDOW_WIDTH_HALF = CV_SETTINGS_WINDOW_WIDTH/2;
	CV_SETTINGS_WINDOW_HEIGHT_HALF = CV_SETTINGS_WINDOW_HEIGHT/2;

	if (!CreateGLWindow(CV_SETTINGS_WINDOW_WIDTH,CV_SETTINGS_WINDOW_HEIGHT,32,CV_SETTINGS_WINDOW_FULLSCREEN))
	{
		return 1;
	}

	while(!done)
	{
		SDL_Event event;
		while ( SDL_PollEvent(&event) )
		{
			switch( event.type )
			{
				case SDL_QUIT:
					done = true;
					break;
				case SDL_KEYDOWN:
				case SDL_KEYUP:
				case SDL_MOUSEBUTTONUP:
				case SDL_MOUSEBUTTONDOWN:
					// Update input manager, no KB hook needed here.
					if (allInited)
					{
						CV_GAME_MANAGER->getControlManager()->getInput()->update(event);
						CV_GAME_MANAGER->getGUIManager()->getPlayGUI()->get_active_gui()->get_input_controler()->update(event);
					}
					break;
				case SDL_MOUSEMOTION:
					break;
				case SDL_VIDEORESIZE:
					break;
				default:
					break;
			}
		}
		
		Uint8 *keys = SDL_GetKeyState(NULL);
		if ((active && !DrawGLScene()) || keys[SDLK_ESCAPE])
		{
			done=TRUE;
		}
	}

	// cleanup
	cleanupGameSystems();

	return checkSDLError();
}