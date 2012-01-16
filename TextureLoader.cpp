#include "commons.h"
#include "system.h"
#include <GL/gl.h>
#include <GL/glu.h>
#ifdef WIN32
#include <olectl.h>
#else
#include "SDLUtils.h"
#endif
#include <math.h>

#include "TextureLoader.h"

using namespace game_utils;

namespace loaders
{
	CTextureLoader::CTextureLoader()
	{
	}

	CTextureLoader::~CTextureLoader()
	{
	}

	CTextureLoader::sLoaderResult CTextureLoader::buildTexture(const char *file, GLuint &texture, bool trans, GLint texture_filter, bool auto_transparent, GLubyte R, GLubyte G, GLubyte B, bool full_path)
	{
		sLoaderResult lr;

		if (!file)
		{
			return lr;
		}

#ifdef WIN32
		HDC			hdcTemp;
		HBITMAP		hbmpTemp;
		IPicture	*pPicture;
		OLECHAR		wszPath[MAX_PATH+1];
#else
		SDL_Surface *image;
#endif
		char		szPath[MAX_PATH+1];
		long		lWidth;
		long		lHeight;
		long		lWidthPixels;
		long		lHeightPixels;
		GLint		glMaxTexDim ;
		szPath[0]='\0';

		if (strstr(file, "http://"))
		{
			strcpy(szPath, file);
		}
		else
		{
			if (strstr(file,":")==NULL)
			{
#ifdef WIN32
				GetCurrentDirectory(MAX_PATH, szPath);
				strcat(szPath, PATH_SEP);
#else
				if (file[0]=='/')
					;
				else if (file[0]=='.'&&file[1]=='.'&&file[2]=='/')
				{
					GetCurrentDirectory(MAX_PATH, szPath);
					strcat(szPath,(PATH_SEP+CV_RESOURCES_DIRECTORY).c_str());
				}
				else
				{
					GetCurrentDirectory(MAX_PATH, szPath);
					strcat(szPath, PATH_SEP);
				}
#endif
				strcat(szPath, file);
			}
			else
			{
				strcpy(szPath,file);
			}
		}

#ifdef WIN32
		MultiByteToWideChar(CP_ACP, 0, szPath, -1, wszPath, MAX_PATH);
		HRESULT hr = OleLoadPicturePath(wszPath, 0, 0, 0, IID_IPicture, (void**)&pPicture);

		if(FAILED(hr))
		{
			return lr;
		}

		hdcTemp = CreateCompatibleDC(GetDC(0));
		if(!hdcTemp)
		{
			pPicture->Release();
			return lr;
		}
#else
//printf("\"%s\", // l. 100, TextureLoader.cpp\n",szPath);
		image = IMG_Load(szPath);
		if ( image == NULL ) {
			fprintf(stderr, "Unable to load %s with %s: %s\n", file, szPath, SDL_GetError());
			return lr;
		}
#endif

		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);
#ifdef WIN32
		pPicture->get_Width(&lWidth);
		lWidthPixels = MulDiv(lWidth, GetDeviceCaps(hdcTemp, LOGPIXELSX), 2540);
		pPicture->get_Height(&lHeight);
		lHeightPixels= MulDiv(lHeight, GetDeviceCaps(hdcTemp, LOGPIXELSY), 2540);

		lr.texture_dimensions.x=lWidthPixels;
		lr.texture_dimensions.y=lHeightPixels;

		// Resize Image To Closest Power Of Two
		if (lWidthPixels <= glMaxTexDim)
		{
			lWidthPixels = 1 << (int)floor((log((double)lWidthPixels)/log(2.0f)) + 0.5f);
		}
		else
		{
			lWidthPixels = glMaxTexDim;
		}

		if (lHeightPixels <= glMaxTexDim)
		{
			lHeightPixels = 1 << (int)floor((log((double)lHeightPixels)/log(2.0f)) + 0.5f);
		}
		else
		{
			lHeightPixels = glMaxTexDim;
		}
		//	Create A Temporary Bitmap
		BITMAPINFO	bi = {0};
		DWORD		*pBits = 0;

		bi.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
		bi.bmiHeader.biBitCount		= 32;
		bi.bmiHeader.biWidth		= lWidthPixels;
		bi.bmiHeader.biHeight		= lHeightPixels;
		bi.bmiHeader.biCompression	= BI_RGB;
		bi.bmiHeader.biPlanes		= 1;

		hbmpTemp = CreateDIBSection(hdcTemp, &bi, DIB_RGB_COLORS, (void**)&pBits, 0, 0);

		if(!hbmpTemp)
		{
			DeleteDC(hdcTemp);
			pPicture->Release();
			return lr;
		}

		SelectObject(hdcTemp, hbmpTemp);

		pPicture->Render(hdcTemp, 0, 0, lWidthPixels, lHeightPixels, 0, lHeight, lWidth, -lHeight, 0);

#else
		lWidth = image->w;
		lHeight = image->h;
		lWidthPixels = powerOfTwo( lWidth );
		lHeightPixels = powerOfTwo( lHeight );

		SDL_Surface *tmpbuf = SDL_DisplayFormatAlpha(image);
		if ( tmpbuf == NULL ) {
			fprintf(stderr, "Failed to prepare texture buffer for %s: %s\n", file, SDL_GetError());
			return lr;
		}
#if 0
		SDL_Surface *texbuf = SDL_CreateRGBSurfaceFrom(tmpbuf->pixels, lWidth, lHeight,
			tmpbuf->format->BitsPerPixel,tmpbuf->pitch,
			tmpbuf->format->Rmask,tmpbuf->format->Gmask,
			tmpbuf->format->Bmask,tmpbuf->format->Amask);
#else
//printf("%ix%i/%ix%i", texrect->x, texrect->y, texrect->w, texrect->h);

SDL_Surface *texbuf = tmpbuf;
if (lWidthPixels!=lWidth || lHeightPixels!=lHeight || trans)
{
	// Convert image buffer for OpenGL or simply use tmpbuf
		texbuf = SDL_CreateRGBSurface(0, lWidthPixels, lHeightPixels,
			tmpbuf->format->BitsPerPixel,
			tmpbuf->format->Rmask,
			tmpbuf->format->Gmask,
			tmpbuf->format->Bmask,
			tmpbuf->format->Amask
			);
		SDL_SetAlpha(tmpbuf, 0, 255);
		//SDL_SetAlpha(texbuf, 0, 255);
		if ( texbuf == NULL ) {
			fprintf(stderr, "Failed to get full GL buffer for %s texture: %s\n", file, SDL_GetError());
			return lr;
		}
		if (trans)
		{
			SDL_SetColorKey(tmpbuf,SDL_SRCCOLORKEY,((Uint32 *)tmpbuf->pixels)[0]);
			//SDL_SetColorKey(texbuf,SDL_SRCCOLORKEY,((Uint32 *)tmpbuf->pixels)[0]);
		}
		SDL_Rect texrect;
		SDL_GetClipRect(tmpbuf, &texrect);
//printf(" %ix%i/%ix%i\n", texrect->x, texrect->y, texrect->w, texrect->h);
		SDL_BlitSurface(tmpbuf, &texrect, texbuf, &texrect);
}
#endif
		Uint32 *pBits = (Uint32 *)texbuf->pixels;
#endif

#ifdef WIN32
		bool first=false;
		GLuint r,g,b;
		for(long i = 0; i < lWidthPixels * lHeightPixels; i++)
		{
			BYTE* pPixel	= (BYTE*)(&pBits[i]);
			BYTE  temp		= pPixel[0];
			pPixel[0]		= pPixel[2];
			pPixel[2]		= temp;

			if (!first)
			{
				r=pPixel[0];
				g=pPixel[1];
				b=pPixel[2];
				first=true;
			}

			if (trans)
			{
				if (auto_transparent)
				{
					if ((pPixel[0]==r) && (pPixel[1]==g) && (pPixel[2]==b))
					{
						pPixel[3]=0;
					}
					else
					{
						pPixel[3]=255;
					}
				}
				else
				{
					if ((pPixel[0]==R) && (pPixel[1]==G) && (pPixel[2]==B))
					{
						pPixel[3]=0;
					}
					else
					{
						pPixel[3]=255;
					}
				}
			}
			else
			{
				pPixel[3]=255;
			}
		}
#else
#endif

		glGenTextures(1, &texture);
//printf("Texture %03i: %lix%li -> %lix%li, %s\n", texture, lWidth, lHeight, lWidthPixels, lHeightPixels, szPath);

		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,texture_filter);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,texture_filter);
#ifdef WIN32
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lWidthPixels, lHeightPixels, 0, GL_RGBA, GL_UNSIGNED_BYTE, pBits);
#else
		glMatrixMode( GL_TEXTURE );
		glLoadIdentity();
		glScalef( 1, -1, 1 );
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lWidthPixels, lHeightPixels, 0, GL_BGRA, GL_UNSIGNED_BYTE, pBits); // or GL_BGRA
		//SDL_FreeSurface(texbuf);
		SDL_FreeSurface(image);
		SDL_FreeSurface(tmpbuf);
#endif

		/*glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, lWidthPixels, lHeightPixels, GL_RGBA, GL_UNSIGNED_BYTE, pBits);*/

#ifdef WIN32
		DeleteObject(hbmpTemp);
		DeleteDC(hdcTemp);

		pPicture->Release();
#else
#endif

		lr.done=true;
		return lr;
	}

	CTextureLoader::sRgbResult CTextureLoader::readPixels(char *file)
	{
		sRgbResult rgbResult;

		if (!file)
		{
			return rgbResult;
		}

#ifdef WIN32
		HDC			hdcTemp;
		HBITMAP		hbmpTemp;
		IPicture	*pPicture;
		OLECHAR		wszPath[MAX_PATH+1];
#else
		SDL_Surface *image;
#endif
		long		lWidth;
		long		lHeight;
		char		szPath[MAX_PATH+1];
		long		lWidthPixels;
		long		lHeightPixels;
		GLint		glMaxTexDim ;

		if (strstr(file, "http://"))
		{
			strcpy(szPath, file);
		}
		else
		{
			if (strstr(file,":")==NULL)
			{
				GetCurrentDirectory(MAX_PATH, szPath);
				strcat(szPath, PATH_SEP);
				strcat(szPath, file);
			}
			else
			{
				strcpy(szPath,file);
			}
		}

#ifdef WIN32
		MultiByteToWideChar(CP_ACP, 0, szPath, -1, wszPath, MAX_PATH);
		HRESULT hr = OleLoadPicturePath(wszPath, 0, 0, 0, IID_IPicture, (void**)&pPicture);

		if(FAILED(hr))
		{
			return rgbResult;
		}

		hdcTemp = CreateCompatibleDC(GetDC(0));
		if(!hdcTemp)
		{
			pPicture->Release();
			return rgbResult;
		}
#else
printf("\"%s\", // l. 313, TextureLoader.cpp\n",szPath);
		image = IMG_Load(szPath);
		if ( image == NULL ) {
			fprintf(stderr, "Unable to load %s with %s: %s\n", file, szPath, SDL_GetError());
			return rgbResult;
		}
#endif

		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);

#ifdef WIN32
		pPicture->get_Width(&lWidth);
		lWidthPixels = MulDiv(lWidth, GetDeviceCaps(hdcTemp, LOGPIXELSX), 2540);
		pPicture->get_Height(&lHeight);
		lHeightPixels= MulDiv(lHeight, GetDeviceCaps(hdcTemp, LOGPIXELSY), 2540);
#else
		lWidth = image->w;
		lHeight = image->h;
		lWidthPixels = lWidth;
		lHeightPixels = lHeight;
#endif

		// Resize Image To Closest Power Of Two
		if (lWidthPixels <= glMaxTexDim)
		{
			lWidthPixels = 1 << (int)floor((log((double)lWidthPixels)/log(2.0f)) + 0.5f);
		}
		else
		{
			lWidthPixels = glMaxTexDim;
		}

		if (lHeightPixels <= glMaxTexDim)
		{
			lHeightPixels = 1 << (int)floor((log((double)lHeightPixels)/log(2.0f)) + 0.5f);
		}
		else
		{
			lHeightPixels = glMaxTexDim;
		}

#ifdef WIN32
		//	Create A Temporary Bitmap
		BITMAPINFO	bi = {0};
		DWORD		*pBits = 0;

		bi.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
		bi.bmiHeader.biBitCount		= 32;
		bi.bmiHeader.biWidth		= lWidthPixels;
		bi.bmiHeader.biHeight		= lHeightPixels;
		bi.bmiHeader.biCompression	= BI_RGB;
		bi.bmiHeader.biPlanes		= 1;

		hbmpTemp = CreateDIBSection(hdcTemp, &bi, DIB_RGB_COLORS, (void**)&pBits, 0, 0);

		if(!hbmpTemp)
		{
			DeleteDC(hdcTemp);
			pPicture->Release();
			return rgbResult;
		}

		SelectObject(hdcTemp, hbmpTemp);

		pPicture->Render(hdcTemp, 0, 0, lWidthPixels, lHeightPixels, 0, lHeight, lWidth, -lHeight, 0);
#else
		BYTE* pBits = (BYTE*)image->pixels;
#endif

		rgbResult.resetSize(lWidthPixels);

		for(long i = 0; i < lWidthPixels * lHeightPixels; i++)
		{
			BYTE* pPixel	= (BYTE*)(&pBits[i]);
			BYTE  temp		= pPixel[0];
			pPixel[0]		= pPixel[2];
			pPixel[2]		= temp;

			rgbResult.rgb[i]=sRgbPixel(pPixel);
		}

		return rgbResult;
	}
};
