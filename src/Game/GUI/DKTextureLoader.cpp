#include "../system.h"
#include <GL/gl.h>
#ifdef WIN32
#include <olectl.h>
#else
#include "../SDLUtils.h"
#endif
#include <math.h>

#include "DKTextureLoader.h"


CDKTextureLoader::CDKTextureLoader()
{

}

CDKTextureLoader::~CDKTextureLoader()
{

}

CDKTextureLoader::LOADER_RESULT CDKTextureLoader::build_texture(char *file, GLuint &texture, bool trans, GLint texture_filter, bool auto_transparent, GLubyte R, GLubyte G, GLubyte B)
{
	LOADER_RESULT lr;

	if (!file)
	{
		return lr;
	}

#ifdef WIN32
	HDC			hdcTemp;												// The DC To Hold Our Bitmap
	HBITMAP		hbmpTemp;												// Holds The Bitmap Temporarily
	IPicture	*pPicture;												// IPicture Interface
	OLECHAR		wszPath[MAX_PATH+1];									// Full Path To Picture (WCHAR)
#else
	SDL_Surface	*image = NULL;
#endif
	char		szPath[MAX_PATH+1];										// Full Path To Picture
	long		lWidth;													// Width In Logical Units
	long		lHeight;												// Height In Logical Units
	long		lWidthPixels;											// Width In Pixels
	long		lHeightPixels;											// Height In Pixels
	GLint		glMaxTexDim ;											// Holds Maximum Texture Size

#ifndef NDEBUG
	//debug("About to load: %s\n", file);
#endif
	if (strstr(file, "http://"))									// If PathName Contains http:// Then...
	{
		strcpy(szPath, file);										// Append The PathName To szPath
	}
	else															// Otherwise... We Are Loading From A File
	{
#ifdef WIN32
		GetCurrentDirectory(MAX_PATH, szPath);						// Get Our Working Directory
		strcat(szPath, PATH_SEP);									// Append "\" After The Working Directory
		strcat(szPath, file);										// Append The PathName
#else
		strcpy(szPath, file);
#endif
	}

#ifdef WIN32
	MultiByteToWideChar(CP_ACP, 0, szPath, -1, wszPath, MAX_PATH);		// Convert From ASCII To Unicode
	HRESULT hr = OleLoadPicturePath(wszPath, 0, 0, 0, IID_IPicture, (void**)&pPicture);

	if(FAILED(hr))
	{
		return lr;
	}

	hdcTemp = CreateCompatibleDC(GetDC(0));								// Create The Windows Compatible Device Context
	if(!hdcTemp)														// Did Creation Fail?
	{
		pPicture->Release();											// Decrements IPicture Reference Count
		return lr;													// Return False (Failure)
	}
#else
	image = IMG_Load(szPath);
	if ( image == NULL ) {
		fprintf(stderr, "Unable to load %s: %s\n", szPath, SDL_GetError());
		return lr;
	}
#endif

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);					// Get Maximum Texture Size Supported

#ifdef WIN32
	pPicture->get_Width(&lWidth);										// Get IPicture Width (Convert To Pixels)
	lWidthPixels	= MulDiv(lWidth, GetDeviceCaps(hdcTemp, LOGPIXELSX), 2540);
	pPicture->get_Height(&lHeight);										// Get IPicture Height (Convert To Pixels)
	lHeightPixels	= MulDiv(lHeight, GetDeviceCaps(hdcTemp, LOGPIXELSY), 2540);

	lr.texture_dimensions.x=lWidthPixels;
	lr.texture_dimensions.y=lHeightPixels;

	// Resize Image To Closest Power Of Two
	if (lWidthPixels <= glMaxTexDim) // Is Image Width Less Than Or Equal To Cards Limit
		lWidthPixels = 1 << (int)floor((log((double)lWidthPixels)/log(2.0f)) + 0.5f);
	else  // Otherwise  Set Width To "Max Power Of Two" That The Card Can Handle
		lWidthPixels = glMaxTexDim;

	if (lHeightPixels <= glMaxTexDim) // Is Image Height Greater Than Cards Limit
		lHeightPixels = 1 << (int)floor((log((double)lHeightPixels)/log(2.0f)) + 0.5f);
	else  // Otherwise  Set Height To "Max Power Of Two" That The Card Can Handle
		lHeightPixels = glMaxTexDim;

	//	Create A Temporary Bitmap
	BITMAPINFO	bi = {0};												// The Type Of Bitmap We Request
	DWORD		*pBits = 0;												// Pointer To The Bitmap Bits

	bi.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);				// Set Structure Size
	bi.bmiHeader.biBitCount		= 32;									// 32 Bit
	bi.bmiHeader.biWidth		= lWidthPixels;							// Power Of Two Width
	bi.bmiHeader.biHeight		= lHeightPixels;						// Make Image Top Up (Positive Y-Axis)
	bi.bmiHeader.biCompression	= BI_RGB;								// RGB Encoding
	bi.bmiHeader.biPlanes		= 1;									// 1 Bitplane

	//	Creating A Bitmap This Way Allows Us To Specify Color Depth And Gives Us Imediate Access To The Bits
	hbmpTemp = CreateDIBSection(hdcTemp, &bi, DIB_RGB_COLORS, (void**)&pBits, 0, 0);

	if(!hbmpTemp)														// Did Creation Fail?
	{
		DeleteDC(hdcTemp);												// Delete The Device Context
		pPicture->Release();											// Decrements IPicture Reference Count
		return lr;													// Return False (Failure)
	}

	SelectObject(hdcTemp, hbmpTemp);									// Select Handle To Our Temp DC And Our Temp Bitmap Object

	// Render The IPicture On To The Bitmap
	pPicture->Render(hdcTemp, 0, 0, lWidthPixels, lHeightPixels, 0, lHeight, lWidth, -lHeight, 0);

	// Convert From BGR To RGB Format And Add An Alpha Value Of 255
	bool first=false;
	GLuint r,g,b;
	for(long i = 0; i < lWidthPixels * lHeightPixels; i++)				// Loop Through All Of The Pixels
	{
		BYTE* pPixel	= (BYTE*)(&pBits[i]);							// Grab The Current Pixel
		BYTE  temp		= pPixel[0];									// Store 1st Color In Temp Variable (Blue)
		pPixel[0]		= pPixel[2];									// Move Red Value To Correct Position (1st)
		pPixel[2]		= temp;											// Move Temp Value To Correct Blue Position (3rd)

		if (!first)
		{
			r=pPixel[0];
			g=pPixel[1];
			b=pPixel[2];
			first=true;
		}

		// This Will Make Any Black Pixels, Completely Transparent		(You Can Hardcode The Value If You Wish)
//		if ((pPixel[0]==0) && (pPixel[1]==0) && (pPixel[2]==0))			// Is Pixel Completely Black
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
	lWidth = image->w;
	lHeight = image->h;
	lWidthPixels = powerOfTwo( lWidth );
	lHeightPixels = powerOfTwo( lHeight );

	lr.texture_dimensions.x=lWidthPixels;
	lr.texture_dimensions.y=lHeightPixels;

	SDL_Surface *tmpbuf = SDL_DisplayFormatAlpha(image);
	if ( tmpbuf == NULL ) {
		fprintf(stderr, "Failed to prepare texture buffer for %s: %s\n", file, SDL_GetError());
		return lr;
	}

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
		if ( texbuf == NULL ) {
			fprintf(stderr, "Failed to get full GL buffer for %s texture: %s\n", file, SDL_GetError());
			return lr;
		}
		if (trans)
		{
			SDL_SetColorKey(tmpbuf,SDL_SRCCOLORKEY,((Uint32 *)tmpbuf->pixels)[0]);
		}
		SDL_Rect texrect;
		SDL_GetClipRect(tmpbuf, &texrect);
		SDL_BlitSurface(tmpbuf, &texrect, texbuf, &texrect);
	}
#endif

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,texture_filter);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,texture_filter);
/*	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);*/

#ifdef WIN32
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lWidthPixels, lHeightPixels, 0, GL_RGBA, GL_UNSIGNED_BYTE, pBits);

	DeleteObject(hbmpTemp);
	DeleteDC(hdcTemp);

	pPicture->Release();
#else
	//glMatrixMode( GL_TEXTURE ) ;
	//glLoadIdentity() ;
	//glScalef( 1, -1, 1 ) ;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lWidthPixels, lHeightPixels, 0, GL_BGRA, GL_UNSIGNED_BYTE, texbuf->pixels);

	SDL_FreeSurface(image);
	SDL_FreeSurface(tmpbuf);
#endif

	lr.result=true;
	return lr;
}
