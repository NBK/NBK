
#include "OGLUtils.h"
#ifndef WIN32
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glxext.h>
#endif
#include <GL/glu.h>

using namespace std;
using namespace cml;

#ifndef WIN32
#ifdef USE_LOCAL_HEADERS
#	include "SDL.h"
#	include "SDL_cpuinfo.h"
#else
#	include <SDL.h>
#	include <SDL_cpuinfo.h>
#endif
#endif

namespace utils
{
	COGLUtils::COGLUtils()
	{
	}

	COGLUtils::~COGLUtils()
	{
	}

	vector3ub COGLUtils::getColor(vector2i mousePos)
	{
		/* get color information from frame buffer */
		unsigned char pixel[3];
		int viewport[4];
		glGetIntegerv(GL_VIEWPORT,viewport);
		glReadPixels(mousePos[0],viewport[3]-mousePos[1],1,1,GL_RGB,GL_UNSIGNED_BYTE,pixel);	
		return vector3ub(pixel[0],pixel[1],pixel[2]);
	}

	GLvoid COGLUtils::toGLOrtho(GLint screenWidth, GLint screenHeight, bool invertY)
	{
		glMatrixMode(GL_PROJECTION);				
		glPushMatrix();								
		glLoadIdentity();								
		glOrtho(0,screenWidth,invertY?screenHeight:0,invertY?0:screenHeight,-1,1);		
		glMatrixMode(GL_MODELVIEW);						
		glPushMatrix();									
		glLoadIdentity();	
	}

	GLvoid COGLUtils::restoreProjection()
	{
		glMatrixMode(GL_PROJECTION);	
		glPopMatrix();				
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}

	string COGLUtils::errorToString(GLint errorCode)
	{
		switch(errorCode)
		{		
			case GL_INVALID_ENUM: 
			{
				return "An unacceptable value is specified for an enumerated argument. The offending function is ignored, having no side effect other than to set the error flag.";
			}
			case GL_INVALID_VALUE:
			{
				return "A numeric argument is out of range. The offending function is ignored, having no side effect other than to set the error flag.";
			}
			case GL_INVALID_OPERATION:
			{
				return "The specified operation is not allowed in the current state. The offending function is ignored, having no side effect other than to set the error flag.";
			}
			case GL_STACK_OVERFLOW:
			{
				return "This function would cause a stack overflow. The offending function is ignored, having no side effect other than to set the error flag.";
			}
			case GL_STACK_UNDERFLOW:
			{
				return "This function would cause a stack underflow. The offending function is ignored, having no side effect other than to set the error flag.";
			}
			case GL_OUT_OF_MEMORY:
			{
				return "There is not enough memory left to execute the function. The state of OpenGL is undefined, except for the state of the error flags, after this error is recorded.";
			}
			default:
			{
				return "No error has been recorded. The value of this symbolic constant is guaranteed to be zero.";
			}
		}
	}

	GLvoid COGLUtils::setVSync(bool val)
	{
#ifdef WIN32
		char *extensions = (char*)glGetString(GL_EXTENSIONS);
		if (strstr(extensions,"WGL_EXT_swap_control")==0)
		{
			return;
		}
		else
		{
			PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");

			if(wglSwapIntervalEXT)
			{
				wglSwapIntervalEXT(val?1:0);
			}
		}
#else
#ifdef USE_SDL
	#if SDL_VERSION_ATLEAST(1,3,0)
	SDL_GL_SetSwapInterval(val);
	#else /* SDL_VERSION_ATLEAST(1,3,0) */
	#ifdef SDL_GL_SWAP_CONTROL
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, val);
	#else /* SDL_GL_SWAP_CONTROL */
	printf("VSync unsupported on old SDL versions (before 1.2.10).\n");
	#endif /* SDL_GL_SWAP_CONTROL */
	#endif /* SDL_VERSION_ATLEAST(1,3,0) */
#endif
#endif
	}

	GLvoid COGLUtils::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

#ifdef WIN32
	GLvoid COGLUtils::swapDC(HDC dc)
	{
		SwapBuffers(dc);
	}
#else
	GLvoid COGLUtils::swapDC()
	{
		SDL_GL_SwapBuffers();
	}
#endif
};