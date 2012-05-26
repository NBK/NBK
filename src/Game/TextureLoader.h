#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include "utils.h"

namespace loaders
{
	class CTextureLoader  
	{
	public:
		CTextureLoader();
		~CTextureLoader();

		struct sLoaderResult
		{
			POINT texture_dimensions;
			bool done;

			sLoaderResult()
			{
				done=false;
			}
		};

		struct sRgbPixel
		{
			BYTE r,g,b;

			sRgbPixel()
			{
				r=g=b=0;
			}

			sRgbPixel(BYTE *bytes)
			{
				this->r=bytes[0];
				this->g=bytes[1];
				this->b=bytes[2];
			}

			sRgbPixel(BYTE r, BYTE g, BYTE b)
			{
				this->r=r;
				this->g=g;
				this->b=b;
			}
		};	

		struct sRgbResult
		{
			sRgbPixel *rgb;
			GLint size;

			GLvoid resetSize(GLint size)
			{
				this->size=size;

				SAFE_DELETE(rgb);

				rgb = new sRgbPixel[size*size];
			}

			GLvoid clean()
			{
				SAFE_DELETE_ARRAY(rgb);
			}

			sRgbResult()
			{
				size=0;
				rgb=NULL;
			}
		};

		static sLoaderResult buildTexture(const char *file, GLuint &texture, bool trans=false, GLint texture_filter=GL_LINEAR, bool auto_transparent=true, GLubyte R=0, GLubyte G=0, GLubyte B=0, bool full_path=false);

		// memory leaks will remain if not deleted properly!
		// texture must be NxN in size
		static sRgbResult readPixels(char *file);
	};
};

#endif // TEXTURE_LOADER_H
