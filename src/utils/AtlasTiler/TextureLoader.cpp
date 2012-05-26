#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <olectl.h>
#include <math.h>

#include "TextureLoader.h"

namespace loaders
{
	CTextureLoader::CTextureLoader()
	{
	}

	CTextureLoader::~CTextureLoader()
	{ 
	}

	CImage CTextureLoader::loadImage(char *file)
	{
		CImage img;
		HRESULT result = img.Load(file);

		return CImage(img);
	}
};