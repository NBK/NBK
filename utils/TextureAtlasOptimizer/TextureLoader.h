#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <windows.h>
#include <gl/gl.h>
#include <atlimage.h>

namespace loaders
{
	class CTextureLoader  
	{
	public:
		CTextureLoader();
		~CTextureLoader();

		static CImage loadImage(char *file);
	};
};

#endif // TEXTURE_LOADER_H
