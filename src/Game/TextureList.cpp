#include "commons.h"
#include "system.h"
#include <GL/gl.h>
#include <stdio.h>

#include "TextureLoader.h"
#include "TextureList.h"
#include "utils.h"

using namespace std;
using namespace loaders;
using namespace game_utils;

namespace lists
{
	CTextureList::CTextureList(): error(false)
	{		
	}

	CTextureList::~CTextureList()
	{
	}

	GLvoid CTextureList::addTexture(GLint index, string fileName, GLint textureFilter, bool transparent, bool autoTransparent, GLubyte R, GLubyte G, GLubyte B)
	{
		textures[textures.find(index)==textures.end()?index:index+CV_NORMAL_MAP_START_INDEX] = sTextureData(fileName,textureFilter,transparent,autoTransparent,R,G,B);
	}

	GLvoid CTextureList::addTexture(GLint index, sTextureData &textureData)
	{
		addTexture(index,textureData.fileName,textureData.textureFilter,textureData.transparent,textureData.autoTransparent,textureData.R,textureData.G,textureData.B);
	}

	GLvoid CTextureList::buildTexture(GLint textureIndex)
	{
		sTextureData *texData = &textures[textureIndex];

		if (texData->fileName=="")
		{
			return;
		}

		CTextureLoader::sLoaderResult lr = CTextureLoader::buildTexture(texData->fileName.c_str(),texData->texture,texData->transparent,texData->textureFilter,texData->autoTransparent,texData->R,texData->G,texData->B);
		
		if (!lr.done)
		{
#ifdef WIN32
			MessageBox(CV_WINDOW_HANDLE, (string("Texture error: ")+texData->fileName).c_str(), "Error", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
#else
			printf("[ERROR|TEXTURE] filename %s\n", string(texData->fileName).c_str());
#endif
		}
		else
		{
			texData->width=lr.texture_dimensions.x;
			texData->height=lr.texture_dimensions.y;
		}
	}

	GLvoid CTextureList::buildTextures()
	{
		for (map<GLint,sTextureData>::iterator iter = textures.begin(); iter != textures.end(); iter++)
		{
			sTextureData *texData = &iter->second;

			if (texData->fileName=="")
			{
				continue;
			}

			CTextureLoader::sLoaderResult lr = CTextureLoader::buildTexture(texData->fileName.c_str(),texData->texture,texData->transparent,texData->textureFilter,texData->autoTransparent,texData->R,texData->G,texData->B);
			
			if (!lr.done)
			{
#ifdef WIN32
				MessageBox(CV_WINDOW_HANDLE, (string("Texture error: ")+texData->fileName).c_str(), "Error", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
#else
				printf("[ERROR|TEXTURE] filename %s\n", string(texData->fileName).c_str());
#endif
			}
			else
			{
				texData->width=lr.texture_dimensions.x;
				texData->height=lr.texture_dimensions.y;
			}
		}
	}

	GLuint CTextureList::getTexture(GLint index)
	{
		if (textures.find(index)==textures.end())
		{
			//MessageBox(CV_WINDOW_HANDLE,"The texture by this name does not exist!","ERROR",MB_OK|MB_ICONERROR | MB_SETFOREGROUND);
			return 0;
		}
		else
		{
			return textures[index].texture;
		}
	}
};