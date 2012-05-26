#ifndef commons_H
#define commons_H

#define SCR_WIDTH 640
#define SCR_HEIGHT 480

static IDirectDraw*			oldOne_IDD = NULL;
static IDirectDraw4*		oldOne_IDD4 = NULL;
static IDirectDrawSurface*	oldOne_IDDS = NULL;
static IDirectDrawSurface3*	oldOne_IDDS3 = NULL;
static IDirectDrawSurface4*	oldOne_IDDS4 = NULL;
static IDirectDrawClipper*	oldOne_IDDC = NULL;

std::map<IDirectDrawSurface*,IDirectDrawSurfaceVtbl>	originalIDirectDrawSurfaceVtbls;
std::map<IDirectDrawSurface4*,IDirectDrawSurface4Vtbl>	originalIDirectDrawSurface4Vtbls;
std::map<IDirectDrawSurface3*,IDirectDrawSurface3Vtbl>	originalIDirectDrawSurface3Vtbls;
std::map<IDirect3DTexture2*,IDirect3DTexture2Vtbl>		originalIDirect3DTexture2Vtbls;

IDirect3DViewport2Vtbl	*originalIDirect3DViewport2Vtbl,
						*replacedIDirect3DViewport2Vtbl;

IDirect3DDevice2Vtbl	*originalIDirect3DDevice2Vtbl,
						*replacedIDirect3DDevice2Vtbl;

std::map<IDirectDrawClipper*,IDirectDrawClipperVtbl>	originalIDirectDrawClipperVtbls;

// <DX5

static std::map<IDirectDraw*,IDirectDrawVtbl>	originalIDirectDrawVtbls;
static std::map<IDirectDraw4*,IDirectDraw4Vtbl>	originalIDirectDraw4Vtbls;

static IDirect3D2Vtbl	*originalDirect3D2Vtbl,
						*replacedDirect3D2Vtbl;

// DX7

static IDirectDraw7Vtbl	originalIDirectDraw7Vtbl;

static IDirect3D7Vtbl	*originalDirect3D7Vtbl,
						*replacedDirect3D7Vtbl;

static IDirect3DDevice7Vtbl	originalIDirect3DDevice7Vtbl;

static std::map<std::string,FARPROC>	originalProcAddresses;


// other variables

std::map<std::string, FARPROC>	hookProcNewAddr;
static HWND						hWnd = 0;

#define IN_GAME_BREAK MessageBox(NULL,"","",MB_OK);DebugBreak();

static std::string intToStr(int val)
{
	std::ostringstream ostr;
	ostr << val;
	return ostr.str();
}

static std::string floatToStr(float val)
{
	std::ostringstream ostr;
	ostr << val;
	return ostr.str();
}

static std::string uintToStr(unsigned int val)
{
	std::ostringstream ostr;
	ostr << val;
	return ostr.str();
}

void CMNS_AdjustWindowFrame(HWND hwnd, DWORD width, DWORD height)
{
	CLogger::addEntry("HWND: "+uintToStr((int)hwnd));

	RECT rect;
	RECT offset;
	POINT p = {0, 0};
	static BOOL check = 0;

	if(check) return;

	ShowCursor(FALSE);

	check = 1;
	rect.top = 0;
	rect.left = 0;
	rect.right = width;
	rect.bottom = height;
	//AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	SetWindowLong(hwnd, GWL_STYLE, WS_POPUP);
	GetWindowRect(hwnd, &offset);

	CLogger::addEntry("Original window rect: "+intToStr(offset.left)+","+intToStr(offset.top)+","+intToStr(offset.right)+","+intToStr(offset.bottom));
	CLogger::addEntry("New window rect: "+intToStr(rect.left)+","+intToStr(rect.top)+","+intToStr(rect.right)+","+intToStr(rect.bottom));

	SetWindowPos(hwnd, 0, offset.left, offset.top, rect.right, rect.bottom, 0);
	//ShowWindow(hwnd, SW_SHOWNORMAL);
	//ShowWindow (hWnd, SW_SHOWDEFAULT);
	ShowWindow(hWnd,SW_SHOWMAXIMIZED);
	UpdateWindow (hWnd);
	/*WNDPROC pWindowProc = (WNDPROC)GetWindowLong(hwnd, GWL_WNDPROC);
	if(!((DWORD)pWindowProc & 0xff000000))
		(WNDPROC)SetWindowLong(hwnd, GWL_WNDPROC, (LONG)extWindowProc);
	ShowCursor(0);*/
	InvalidateRect(0, 0, TRUE);
}

void CMNS_SaveBitmap(std::string filename, HBITMAP hBitmap)

{

	HDC        hdc=NULL;

	FILE*      fp=NULL;

	LPVOID     pBuf=NULL;

	BITMAPINFO bmpInfo;

	BITMAPFILEHEADER  bmpFileHeader; 

	do{ 

		hdc=GetDC(NULL);

		ZeroMemory(&bmpInfo,sizeof(BITMAPINFO));

		bmpInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);

		GetDIBits(hdc,hBitmap,0,0,NULL,&bmpInfo,DIB_RGB_COLORS); 

		if(bmpInfo.bmiHeader.biSizeImage<=0)

			bmpInfo.bmiHeader.biSizeImage=bmpInfo.bmiHeader.biWidth*abs(bmpInfo.bmiHeader.biHeight)*(bmpInfo.bmiHeader.biBitCount+7)/8;

		if((pBuf = malloc(bmpInfo.bmiHeader.biSizeImage))==NULL)

		{

			MessageBox( NULL, "Unable to Allocate Bitmap Memory", "Error", MB_OK|MB_ICONERROR);

			break;

		}           

		bmpInfo.bmiHeader.biCompression=BI_RGB;

		GetDIBits(hdc,hBitmap,0,bmpInfo.bmiHeader.biHeight,pBuf, &bmpInfo, DIB_RGB_COLORS);       

		if((fp = fopen(filename.c_str(),"wb"))==NULL)
		{

			MessageBox( NULL, "Unable to Create Bitmap File", "Error", MB_OK|MB_ICONERROR);

			break;

		} 

		bmpFileHeader.bfReserved1=0;

		bmpFileHeader.bfReserved2=0;

		bmpFileHeader.bfSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+bmpInfo.bmiHeader.biSizeImage;

		bmpFileHeader.bfType='MB';

		bmpFileHeader.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER); 

		fwrite(&bmpFileHeader,sizeof(BITMAPFILEHEADER),1,fp);

		fwrite(&bmpInfo.bmiHeader,sizeof(BITMAPINFOHEADER),1,fp);

		fwrite(pBuf,bmpInfo.bmiHeader.biSizeImage,1,fp); 

	}while(false); 

	if(hdc)     ReleaseDC(NULL,hdc); 

	if(pBuf)    free(pBuf); 

	if(fp)      fclose(fp);

}

void CMNS_CopySurface(IDirectDrawSurface *pDDSurface, std::string filename)
{
	CLogger::addEntry("CMNS_CopySurface: "+filename);

	HDC hdc , hmemdc ;

	HBITMAP hbitmap ,hprevbitmap;

	DDSURFACEDESC ddsd;

	pDDSurface->lpVtbl->GetDC(pDDSurface,&hdc);

	hmemdc=CreateCompatibleDC(hdc);

	ZeroMemory(&ddsd ,sizeof( ddsd )); // better to clear before using

	ddsd.dwSize= sizeof( ddsd ); //initialize with size

	pDDSurface->lpVtbl->GetSurfaceDesc(pDDSurface,&ddsd);

	hbitmap=CreateCompatibleBitmap( hdc ,ddsd.dwWidth ,ddsd.dwHeight);

	hprevbitmap =(HBITMAP) SelectObject( hmemdc, hbitmap );

	BitBlt(hmemdc,0 ,0 ,ddsd.dwWidth ,ddsd.dwHeight ,hdc ,0 ,0,SRCCOPY);

	CMNS_SaveBitmap(filename,hbitmap);

	SelectObject(hmemdc,hprevbitmap); // restore the old bitmap

	DeleteDC(hmemdc);

	pDDSurface->lpVtbl->ReleaseDC(pDDSurface,hdc);

	return ;

}

static bool CMNS_uploadTexture(IDirectDrawSurface *surface, std::string fileName)
{
	CLogger::addEntry("CMNS_uploadTexture: "+fileName);
	HBITMAP hBM;  
	BITMAP BM;  
	HDC hDCImage, hDC;  

	hBM = ( HBITMAP ) LoadImage( NULL, fileName.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION );  	

	if (!hBM)
	{
		return false;
	}

	GetObject( hBM, sizeof( BM ), &BM );  

	hDCImage = CreateCompatibleDC( NULL );  
	SelectObject( hDCImage, hBM );  

	DDSURFACEDESC sDesc;

	sDesc.dwSize = sizeof(DDSURFACEDESC);

	surface->lpVtbl->GetSurfaceDesc(surface,&sDesc);

	//IN_GAME_BREAK

	if( SUCCEEDED( surface->lpVtbl->GetDC(surface,&hDC)))  
	{  
		//IN_GAME_BREAK
		BOOL res = BitBlt( hDC, 0, 0, sDesc.dwWidth, sDesc.dwHeight, hDCImage, 0, 0, SRCCOPY );  
		//BOOL res = StretchBlt(hDC, 0, 0, sDesc.dwWidth, sDesc.dwHeight, hDCImage, 0, 0, 84, 84, SRCCOPY );  
		surface->lpVtbl->ReleaseDC(surface,hDC);  
	}  

	DeleteDC( hDCImage );  
	DeleteObject( hBM );

	return true;

}

static LPDIRECTDRAWSURFACE CMNS_CreateBitmapSurface(std::string fileName, bool hardware)
{
	CLogger::addEntry("CMNS_CreateBitmapSurface");
	LPDIRECTDRAWSURFACE pSurface = NULL;
	DDSURFACEDESC ddsd;

	HBITMAP hBM;  
	BITMAP BM;  
	HDC hDCImage, hDC;  

	hBM = ( HBITMAP ) LoadImage( NULL, fileName.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION );  	

	if (!hBM)
	{
		return false;
	}

	GetObject( hBM, sizeof( BM ), &BM );  

	hDCImage = CreateCompatibleDC( NULL );  
	SelectObject( hDCImage, hBM );  

	// Create a surface for this bitmap
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	ddsd.dwWidth = BM.bmWidth;
	ddsd.dwHeight = BM.bmHeight;
	if (hardware)
		ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_VIDEOMEMORY;
	else
		ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_SYSTEMMEMORY;

	IDirectDraw *m_pDD = originalIDirectDrawVtbls.begin()->first;

	m_pDD->lpVtbl->CreateSurface(m_pDD, &ddsd, &pSurface, NULL);


	if( SUCCEEDED( pSurface->lpVtbl->GetDC(pSurface,&hDC)))  
	{  
		//IN_GAME_BREAK
		BOOL res = BitBlt( hDC, 0, 0, BM.bmWidth, BM.bmHeight, hDCImage, 0, 0, SRCCOPY );  
		//BOOL res = StretchBlt(hDC, 0, 0, sDesc.dwWidth, sDesc.dwHeight, hDCImage, 0, 0, 512, 512, SRCCOPY );  
		pSurface->lpVtbl->ReleaseDC(pSurface,hDC);  
	}  

	DeleteDC( hDCImage );  
	DeleteObject( hBM );

	return pSurface;
}

static int CMNS_getWidth(LPRECT rect)
{
	return abs(rect->right - rect->left);
}

static int CMNS_getHeight(LPRECT rect)
{
	return abs(rect->bottom - rect->top);
}

#endif // commons_H