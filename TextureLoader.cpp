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

	CTextureLoader::sLoaderResult CTextureLoader::buildTexture(const char *file, GLuint &texture, bool trans, GLint texture_filter, bool auto_transparent, GLubyte R, GLubyte G, GLubyte B, bool full_path)
	{
		sLoaderResult lr;

		if (!file)
		{
			return lr;
		}
		
		HDC			hdcTemp;
		HBITMAP		hbmpTemp;
		IPicture	*pPicture;
		OLECHAR		wszPath[MAX_PATH+1];
		char		szPath[MAX_PATH+1];
		long		lWidth;
		long		lHeight;
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
				strcat(szPath, "\\");
				strcat(szPath, file);
			}
			else
			{
				strcpy(szPath,file);
			}
		}

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

		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);
		
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

		glGenTextures(1, &texture);

		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,texture_filter);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,texture_filter);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lWidthPixels, lHeightPixels, 0, GL_RGBA, GL_UNSIGNED_BYTE, pBits);

		/*glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, lWidthPixels, lHeightPixels, GL_RGBA, GL_UNSIGNED_BYTE, pBits);*/

		DeleteObject(hbmpTemp);
		DeleteDC(hdcTemp);

		pPicture->Release();

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
		
		HDC			hdcTemp;
		HBITMAP		hbmpTemp;
		IPicture	*pPicture;
		OLECHAR		wszPath[MAX_PATH+1];
		char		szPath[MAX_PATH+1];
		long		lWidth;
		long		lHeight;
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
				strcat(szPath, "\\");
				strcat(szPath, file);
			}
			else
			{
				strcpy(szPath,file);
			}
		}

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

		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);
		
		pPicture->get_Width(&lWidth);
		lWidthPixels = MulDiv(lWidth, GetDeviceCaps(hdcTemp, LOGPIXELSX), 2540);
		pPicture->get_Height(&lHeight);
		lHeightPixels= MulDiv(lHeight, GetDeviceCaps(hdcTemp, LOGPIXELSY), 2540);	

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
			return rgbResult;
		}

		SelectObject(hdcTemp, hbmpTemp);

		pPicture->Render(hdcTemp, 0, 0, lWidthPixels, lHeightPixels, 0, lHeight, lWidth, -lHeight, 0);

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