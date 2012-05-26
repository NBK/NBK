#ifndef TEXTURE_LIST_H
#define TEXTURE_LIST_H

#include "system.h"
#include <GL/gl.h>
#include <map>
#include <string>

namespace lists
{
	class CTextureList
	{
	public:
		CTextureList();
		~CTextureList();

		struct sTextureData
		{		
			GLuint	texture,
					width,
					height;

			std::string	fileName;

			bool	transparent, 
					autoTransparent;

			GLubyte R,G,B;

			GLint	textureFilter;

			sTextureData()
			{
				texture = width = height = 0;
			}

			sTextureData(std::string fileName, GLint textureFilter=GL_LINEAR, bool transparent=false, bool autoTransparent=true, GLubyte R=0, GLubyte G=0, GLubyte B=0)
			{
				this->fileName=fileName;
				this->textureFilter=textureFilter;
				this->autoTransparent=autoTransparent;
				this->R=R;
				this->G=G;
				this->B=B;
				this->transparent=transparent;

				texture = width = height = 0;
			}
		};

		GLvoid addTexture(GLint index, std::string fileName, GLint textureFilter=GL_LINEAR, bool transparent=false, bool autoTransparent=true, GLubyte R=0, GLubyte G=0, GLubyte B=0);
		GLvoid addTexture(GLint index, sTextureData &textureData);

		GLvoid buildTextures();
		GLvoid buildTexture(GLint textureIndex);

		GLuint getTexture(GLint index);

	private:
		
		std::map<GLint, sTextureData> textures;
		bool error;
	};
};

#endif // TEXTURE_LIST_H
