#ifndef OGL_UTILS_H
#define OGL_UTILS_H

#include "system.h"
#include <GL/gl.h>
#include <string>
#include <cml/cml.h>

#ifdef WIN32
typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)( int );
#endif

namespace utils
{
	class COGLUtils
	{
	public:
		COGLUtils();
		~COGLUtils();

		static cml::vector3ub getColor(cml::vector2i mousePos);
		static GLvoid		toGLOrtho(GLint screenWidth, GLint screenHeight, bool invertY = false);
		static GLvoid		restoreProjection();
		static std::string	errorToString(GLint errorCode);
		static GLvoid		setVSync(bool val = true);
		static GLvoid		clear();
#ifdef WIN32
		static GLvoid		swapDC(HDC dc);
#else
		static GLvoid		swapDC();
#endif
	};
};

#endif // OGL_UTILS_H