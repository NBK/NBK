#ifndef OGL_UTILS_H
#define OGL_UTILS_H

#include <windows.h>
#include <gl/gl.h>
#include <string>

typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)( int );

namespace utils
{
	class COGLUtils
	{
	public:
		COGLUtils();
		~COGLUtils();

		static GLvoid		toGLOrtho(GLint screenWidth, GLint screenHeight, bool invertY = false);
		static GLvoid		restoreProjection();
		static std::string	errorToString(GLint errorCode);
		static GLvoid		setVSync(bool val = true);
		static GLvoid		clear();
		static GLvoid		swapDC(HDC dc);
	};
};

#endif // OGL_UTILS_H