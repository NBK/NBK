#ifndef OGL_UTILS_H
#define OGL_UTILS_H

#include <windows.h>
#include <gl/gl.h>
#include <string>
#include <cml/cml.h>

typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)( int );

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
		static GLvoid		swapDC(HDC dc);
	};
};

#endif // OGL_UTILS_H