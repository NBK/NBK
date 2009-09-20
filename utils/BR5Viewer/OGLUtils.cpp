#include "OGLUtils.h"
#include <gl/glu.h>

using namespace std;

namespace utils
{
	COGLUtils::COGLUtils()
	{
	}

	COGLUtils::~COGLUtils()
	{
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
	}

	GLvoid COGLUtils::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	GLvoid COGLUtils::swapDC(HDC dc)
	{
		SwapBuffers(dc);
	}
};