#ifndef IDirectDrawHooked_H
#define IDirectDrawHooked_H

HRESULT WINAPI IDirectDrawHooked_SetCooperativeLevel(IDirectDraw *iDD, HWND hwnd, DWORD dwFlag)
{	
	dwFlag = DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE | DDSCL_ALLOWMODEX;

	CMNS_AdjustWindowFrame(hwnd, SCR_WIDTH, SCR_HEIGHT);	

	HRESULT fnResult = originalIDirectDrawVtbls[iDD].SetCooperativeLevel(iDD,hwnd,dwFlag);	

	hWnd = hwnd;

	return fnResult;
}

HRESULT WINAPI IDirectDrawHooked_SetDisplayMode(IDirectDraw *iDD, DWORD dwWidth, DWORD dwHeight, DWORD dwBpp)
{
	HRESULT res = 0;

	//if (dwWidth == 640)
	{
		CLogger::addEntry("IDirectDrawHooked_SetDisplayMode ("+intToStr(dwWidth)+","+intToStr(dwHeight)+")"
			+", forced to ("+intToStr(SCR_WIDTH)+","+intToStr(SCR_HEIGHT)+")");

		dwWidth = SCR_WIDTH;
		dwHeight = SCR_HEIGHT;
	}

	if (hWnd) 
	{
		CMNS_AdjustWindowFrame(hWnd, dwWidth, dwHeight);
	}

	//res = originalIDirectDrawVtbls[iDD].SetDisplayMode(iDD, dwWidth, dwHeight, 16);	
	res = originalIDirectDrawVtbls[iDD].SetDisplayMode(iDD, dwWidth, dwHeight, dwBpp);	

	return 0;
}

#define fExists(x,m) ((x)&(m)) 

HRESULT WINAPI IDirectDrawHooked_CreateSurface(IDirectDraw *iDD, LPDDSURFACEDESC lpSurfDesc, LPDIRECTDRAWSURFACE FAR *surf, IUnknown FAR *pUnkOuter)
{	
	if (lpSurfDesc && lpSurfDesc->dwWidth == 640)
	{
		lpSurfDesc->dwWidth = SCR_WIDTH;
		lpSurfDesc->dwHeight = SCR_HEIGHT;
	}		
	else if (lpSurfDesc && lpSurfDesc->dwWidth == 256 && lpSurfDesc->dwHeight == 256)
	{
		// force HIRES textures, set this to the new texture size, also update texture creation in IDirectDrawSurfaceHooked_QueryInterface
		lpSurfDesc->dwWidth = 1024;
		lpSurfDesc->dwHeight = 1024;
	}	
	else if (lpSurfDesc && lpSurfDesc->dwWidth == 32 && lpSurfDesc->dwHeight == 32)
	{
		// force HIRES textures, set this to the new texture size, also update texture creation in IDirectDrawSurfaceHooked_QueryInterface
		lpSurfDesc->dwWidth = 64;
		lpSurfDesc->dwHeight = 64;
	}
	else if (lpSurfDesc)
	{
		CLogger::addEntry("??? IDirectDrawHooked_CreateSurface ("+intToStr(lpSurfDesc->dwWidth)+","+intToStr(lpSurfDesc->dwHeight)+")");
	}
	else
	{
		CLogger::addEntry("??? IDirectDrawHooked_CreateSurface NO SURFACE DESC!");
	}

	if (lpSurfDesc && fExists(lpSurfDesc->ddsCaps.dwCaps,DDSCAPS_PRIMARYSURFACE))
	{
		///CLogger::addEntry("PRIMARY SURFACE "+intToStr((int)*surf));

		//lpSurfDesc->dwFlags = DDSD_CAPS;
		//lpSurfDesc->ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE; TODO
	}

	HRESULT fnResult = originalIDirectDrawVtbls[iDD].CreateSurface(iDD,lpSurfDesc,surf,pUnkOuter);	

	IDirectDrawSurface *surface = *surf;
	IDirectDrawSurface_setupHook(surface);

	return fnResult;
}

HRESULT WINAPI IDirectDrawHooked_QueryInterface(IDirectDraw *iDD, const _GUID &guid, void **obp)
{
	HRESULT fnResult = E_FAIL;

	fnResult = originalIDirectDrawVtbls[iDD].QueryInterface(iDD,guid,obp);		

	if (guid == IID_IDirect3D2)
	{		
		IDirect3D2 *lpiD3D = (IDirect3D2*)*obp;
		IDirect3D2_setupHook(lpiD3D,obp);
	}
	else if (guid == IID_IDirectDraw)
	{
		IDirectDraw *lpDD = (IDirectDraw*)*obp;
		IDirectDraw_setupHook(lpDD);
	}
	else if (guid == IID_IDirectDraw4)
	{		
		IDirectDraw4 *lpDD = (IDirectDraw4*)*obp;
		IDirectDraw4_setupHook(lpDD);
	}
	else
	{
		CLogger::addEntry("??? Calling IDirectDrawHooked_QueryInterface");
	}

	return fnResult;
}

// auto generated

HRESULT WINAPI IDirectDrawHooked_Compact(IDirectDraw* p0)
{
	HRESULT res = E_FAIL;
	CLogger::addEntry("Calling IDirectDrawHooked_Compact = "+intToStr((int)p0));
	bool found = (originalIDirectDrawVtbls.find(p0) != originalIDirectDrawVtbls.end());
	if (found)
	{
		res = originalIDirectDrawVtbls[p0].Compact(p0);
	}
	else
	{
		res = originalIDirectDrawVtbls.begin()->second.Compact(p0);

	}
	return res;
}

HRESULT WINAPI IDirectDrawHooked_CreateClipper(IDirectDraw* p0, DWORD p1, LPDIRECTDRAWCLIPPER FAR* p2, IUnknown FAR * p3)
{
	HRESULT res = E_FAIL;
	CLogger::addEntry("Calling IDirectDrawHooked_CreateClipper = "+intToStr((int)p0));
	bool found = (originalIDirectDrawVtbls.find(p0) != originalIDirectDrawVtbls.end());
	if (found)
	{
		res = originalIDirectDrawVtbls[p0].CreateClipper(p0,p1,p2,p3);
	}
	else
	{
		res = originalIDirectDrawVtbls.begin()->second.CreateClipper(p0,p1,p2,p3);

	}

	IDirectDrawClipper *iddc = (IDirectDrawClipper*)*p2;
	IDirectDrawClipper_setupHook(iddc);

	return res;
}

HRESULT WINAPI IDirectDrawHooked_CreatePalette(IDirectDraw* p0, DWORD p1, LPPALETTEENTRY p2, LPDIRECTDRAWPALETTE FAR* p3, IUnknown FAR * p4)
{
	HRESULT res = E_FAIL;
	CLogger::addEntry("Calling IDirectDrawHooked_CreatePalette = "+intToStr((int)p0));
	bool found = (originalIDirectDrawVtbls.find(p0) != originalIDirectDrawVtbls.end());
	if (found)
	{
		res = originalIDirectDrawVtbls[p0].CreatePalette(p0,p1,p2,p3,p4);
	}
	else
	{
		res = originalIDirectDrawVtbls.begin()->second.CreatePalette(p0,p1,p2,p3,p4);

	}
	return res;
}

HRESULT WINAPI IDirectDrawHooked_DuplicateSurface(IDirectDraw* p0, LPDIRECTDRAWSURFACE p1, LPDIRECTDRAWSURFACE FAR * p2)
{
	HRESULT res = E_FAIL;
	CLogger::addEntry("Calling IDirectDrawHooked_DuplicateSurface = "+intToStr((int)p0));
	bool found = (originalIDirectDrawVtbls.find(p0) != originalIDirectDrawVtbls.end());
	if (found)
	{
		res = originalIDirectDrawVtbls[p0].DuplicateSurface(p0,p1,p2);
	}
	else
	{
		res = originalIDirectDrawVtbls.begin()->second.DuplicateSurface(p0,p1,p2);

	}
	if(p1)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p1->lpVtbl->GetSurfaceDesc(p1,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}
	if(p2 && *p2)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		(*p2)->lpVtbl->GetSurfaceDesc(*p2,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}
	return res;
}

HRESULT WINAPI IDirectDrawHooked_EnumDisplayModes(IDirectDraw* p0, DWORD p1, LPDDSURFACEDESC p2, LPVOID p3, LPDDENUMMODESCALLBACK p4)
{
	HRESULT res = E_FAIL;
	CLogger::addEntry("Calling IDirectDrawHooked_EnumDisplayModes = "+intToStr((int)p0));
	bool found = (originalIDirectDrawVtbls.find(p0) != originalIDirectDrawVtbls.end());
	if (found)
	{
		res = originalIDirectDrawVtbls[p0].EnumDisplayModes(p0,p1,p2,p3,p4);
	}
	else
	{
		res = originalIDirectDrawVtbls.begin()->second.EnumDisplayModes(p0,p1,p2,p3,p4);

	}
	return res;
}

HRESULT WINAPI IDirectDrawHooked_EnumSurfaces(IDirectDraw* p0, DWORD p1, LPDDSURFACEDESC p2, LPVOID p3, LPDDENUMSURFACESCALLBACK p4)
{
	HRESULT res = E_FAIL;
	CLogger::addEntry("Calling IDirectDrawHooked_EnumSurfaces = "+intToStr((int)p0));
	bool found = (originalIDirectDrawVtbls.find(p0) != originalIDirectDrawVtbls.end());
	if (found)
	{
		res = originalIDirectDrawVtbls[p0].EnumSurfaces(p0,p1,p2,p3,p4);
	}
	else
	{
		res = originalIDirectDrawVtbls.begin()->second.EnumSurfaces(p0,p1,p2,p3,p4);

	}
	return res;
}

HRESULT WINAPI IDirectDrawHooked_FlipToGDISurface(IDirectDraw* p0)
{
	HRESULT res = E_FAIL;
	CLogger::addEntry("Calling IDirectDrawHooked_FlipToGDISurface = "+intToStr((int)p0));
	bool found = (originalIDirectDrawVtbls.find(p0) != originalIDirectDrawVtbls.end());
	if (found)
	{
		res = originalIDirectDrawVtbls[p0].FlipToGDISurface(p0);
	}
	else
	{
		res = originalIDirectDrawVtbls.begin()->second.FlipToGDISurface(p0);

	}
	return res;
}

HRESULT WINAPI IDirectDrawHooked_GetCaps(IDirectDraw* p0, LPDDCAPS p1, LPDDCAPS p2)
{
	HRESULT res = E_FAIL;
	CLogger::addEntry("Calling IDirectDrawHooked_GetCaps = "+intToStr((int)p0));
	bool found = (originalIDirectDrawVtbls.find(p0) != originalIDirectDrawVtbls.end());
	if (found)
	{
		res = originalIDirectDrawVtbls[p0].GetCaps(p0,p1,p2);
	}
	else
	{
		res = originalIDirectDrawVtbls.begin()->second.GetCaps(p0,p1,p2);

	}
	return res;
}

HRESULT WINAPI IDirectDrawHooked_GetDisplayMode(IDirectDraw* p0, LPDDSURFACEDESC p1)
{
	HRESULT res = E_FAIL;
	CLogger::addEntry("Calling IDirectDrawHooked_GetDisplayMode = "+intToStr((int)p0));
	bool found = (originalIDirectDrawVtbls.find(p0) != originalIDirectDrawVtbls.end());
	if (found)
	{
		res = originalIDirectDrawVtbls[p0].GetDisplayMode(p0,p1);
	}
	else
	{
		res = originalIDirectDrawVtbls.begin()->second.GetDisplayMode(p0,p1);

	}

	if (p1)
	{
		CLogger::addEntry("IDirectDrawHooked_GetDisplayMode -> LPDDSURFACEDESC("+intToStr(p1->dwWidth)+", "+intToStr(p1->dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawHooked_GetFourCCCodes(IDirectDraw* p0, LPDWORD p1, LPDWORD p2)
{
	HRESULT res = E_FAIL;
	CLogger::addEntry("Calling IDirectDrawHooked_GetFourCCCodes = "+intToStr((int)p0));
	bool found = (originalIDirectDrawVtbls.find(p0) != originalIDirectDrawVtbls.end());
	if (found)
	{
		res = originalIDirectDrawVtbls[p0].GetFourCCCodes(p0,p1,p2);
	}
	else
	{
		res = originalIDirectDrawVtbls.begin()->second.GetFourCCCodes(p0,p1,p2);

	}
	return res;
}

HRESULT WINAPI IDirectDrawHooked_GetGDISurface(IDirectDraw* p0, LPDIRECTDRAWSURFACE FAR * p1)
{
	HRESULT res = E_FAIL;
	CLogger::addEntry("Calling IDirectDrawHooked_GetGDISurface = "+intToStr((int)p0));
	bool found = (originalIDirectDrawVtbls.find(p0) != originalIDirectDrawVtbls.end());
	if (found)
	{
		res = originalIDirectDrawVtbls[p0].GetGDISurface(p0,p1);
	}
	else
	{
		res = originalIDirectDrawVtbls.begin()->second.GetGDISurface(p0,p1);

	}
	if(p1 && *p1)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		(*p1)->lpVtbl->GetSurfaceDesc(*p1,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}
	return res;
}

HRESULT WINAPI IDirectDrawHooked_GetMonitorFrequency(IDirectDraw* p0, LPDWORD p1)
{
	HRESULT res = E_FAIL;
	CLogger::addEntry("Calling IDirectDrawHooked_GetMonitorFrequency = "+intToStr((int)p0));
	bool found = (originalIDirectDrawVtbls.find(p0) != originalIDirectDrawVtbls.end());
	if (found)
	{
		res = originalIDirectDrawVtbls[p0].GetMonitorFrequency(p0,p1);
	}
	else
	{
		res = originalIDirectDrawVtbls.begin()->second.GetMonitorFrequency(p0,p1);

	}
	return res;
}

HRESULT WINAPI IDirectDrawHooked_GetScanLine(IDirectDraw* p0, LPDWORD p1)
{
	HRESULT res = E_FAIL;
	CLogger::addEntry("Calling IDirectDrawHooked_GetScanLine = "+intToStr((int)p0));
	bool found = (originalIDirectDrawVtbls.find(p0) != originalIDirectDrawVtbls.end());
	if (found)
	{
		res = originalIDirectDrawVtbls[p0].GetScanLine(p0,p1);
	}
	else
	{
		res = originalIDirectDrawVtbls.begin()->second.GetScanLine(p0,p1);

	}
	return res;
}

HRESULT WINAPI IDirectDrawHooked_GetVerticalBlankStatus(IDirectDraw* p0, LPBOOL p1)
{
	HRESULT res = E_FAIL;
	CLogger::addEntry("Calling IDirectDrawHooked_GetVerticalBlankStatus = "+intToStr((int)p0));
	bool found = (originalIDirectDrawVtbls.find(p0) != originalIDirectDrawVtbls.end());
	if (found)
	{
		res = originalIDirectDrawVtbls[p0].GetVerticalBlankStatus(p0,p1);
	}
	else
	{
		res = originalIDirectDrawVtbls.begin()->second.GetVerticalBlankStatus(p0,p1);

	}
	return res;
}

HRESULT WINAPI IDirectDrawHooked_Initialize(IDirectDraw* p0, GUID FAR * p1)
{
	HRESULT res = E_FAIL;
	CLogger::addEntry("Calling IDirectDrawHooked_Initialize = "+intToStr((int)p0));
	bool found = (originalIDirectDrawVtbls.find(p0) != originalIDirectDrawVtbls.end());
	if (found)
	{
		res = originalIDirectDrawVtbls[p0].Initialize(p0,p1);
	}
	else
	{
		res = originalIDirectDrawVtbls.begin()->second.Initialize(p0,p1);

	}
	return res;
}

HRESULT WINAPI IDirectDrawHooked_RestoreDisplayMode(IDirectDraw* p0)
{
	HRESULT res = E_FAIL;
	CLogger::addEntry("Calling IDirectDrawHooked_RestoreDisplayMode = "+intToStr((int)p0));
	bool found = (originalIDirectDrawVtbls.find(p0) != originalIDirectDrawVtbls.end());
	if (found)
	{
		res = originalIDirectDrawVtbls[p0].RestoreDisplayMode(p0);
	}
	else
	{
		res = originalIDirectDrawVtbls.begin()->second.RestoreDisplayMode(p0);

	}
	return res;
}

HRESULT WINAPI IDirectDrawHooked_WaitForVerticalBlank(IDirectDraw* p0, DWORD p1, HANDLE p2)
{
	HRESULT res = E_FAIL;
	CLogger::addEntry("Calling IDirectDrawHooked_WaitForVerticalBlank = "+intToStr((int)p0));
	bool found = (originalIDirectDrawVtbls.find(p0) != originalIDirectDrawVtbls.end());
	if (found)
	{
		res = originalIDirectDrawVtbls[p0].WaitForVerticalBlank(p0,p1,p2);
	}
	else
	{
		res = originalIDirectDrawVtbls.begin()->second.WaitForVerticalBlank(p0,p1,p2);

	}
	return res;
}

#endif // IDirectDrawHooked