#ifndef IDirectDraw4Hooked_H
#define IDirectDraw4Hooked_H

HRESULT WINAPI IDirectDraw4Hooked_SetCooperativeLevel(IDirectDraw4 *iDD, HWND hwnd, DWORD dwFlag)
{	
	HRESULT fnResult = E_FAIL;

	CMNS_AdjustWindowFrame(hwnd, SCR_WIDTH, SCR_HEIGHT);

	dwFlag = DDSCL_ALLOWMODEX | DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT | DDSCL_NORMAL ;
		
	bool found = (originalIDirectDraw4Vtbls.find(iDD) != originalIDirectDraw4Vtbls.end());

	if (found)
	{
		fnResult = originalIDirectDraw4Vtbls[iDD].SetCooperativeLevel(iDD,hwnd,dwFlag);	
	}

	hWnd = hwnd;

	return fnResult;
}

HRESULT WINAPI IDirectDraw4Hooked_SetDisplayMode(IDirectDraw4 *iDD, DWORD dwWidth, DWORD dwHeight, DWORD dwBpp, DWORD dwFreq, DWORD dwFlag)
{
	CLogger::addEntry("IDirectDraw4Hooked_SetDisplayMode ("+intToStr(dwWidth)+","+intToStr(dwHeight)+")"
		+", forced to ("+intToStr(SCR_WIDTH)+","+intToStr(SCR_HEIGHT)+")");

	HRESULT res = 0;

	bool found = (originalIDirectDraw4Vtbls.find(iDD) != originalIDirectDraw4Vtbls.end());

	if (found)
	{
		res = originalIDirectDraw4Vtbls[iDD].SetDisplayMode(iDD, dwWidth, dwHeight, dwBpp, dwFreq, dwFlag);	
	}

	return res;
}

HRESULT WINAPI IDirectDraw4Hooked_CreateSurface(IDirectDraw4 *iDD, LPDDSURFACEDESC2 lpSurfDesc, LPDIRECTDRAWSURFACE4 FAR *surf, IUnknown FAR *pUnkOuter)
{
	if (iDD && lpSurfDesc)
	{
		CLogger::addEntry("Creating surface4 ( DDRAW = "+intToStr((int)iDD)+", SURF = "+intToStr((int)surf)+") w = "+intToStr(lpSurfDesc->dwWidth)+", h = "+intToStr(lpSurfDesc->dwHeight));
	}

	HRESULT fnResult = E_FAIL;

	bool found = (originalIDirectDraw4Vtbls.find(iDD) != originalIDirectDraw4Vtbls.end());

	if (found)
	{
		fnResult = originalIDirectDraw4Vtbls[iDD].CreateSurface(iDD,lpSurfDesc,surf,pUnkOuter);	
	}
	else
	{
		// fallback to some other stored IDD4
		if (originalIDirectDraw4Vtbls.size() > 0)
		{
			if (surf && iDD && lpSurfDesc)
			{
				CLogger::addEntry("Creating surface4 (fallback)( DDRAW = "+intToStr((int)iDD)+", SURF = "+intToStr((int)surf)+") w = "+intToStr(lpSurfDesc->dwWidth)+", h = "+intToStr(lpSurfDesc->dwHeight));
			}
			fnResult = originalIDirectDraw4Vtbls.begin()->second.CreateSurface(iDD,lpSurfDesc,surf,pUnkOuter);	

			if (fnResult == E_FAIL)
			{
				CLogger::addEntry("ERR >> Creating surface4 failed (fallback)( DDRAW = "+intToStr((int)iDD)+"!");
			}
		}
		else
		{
			CLogger::addEntry("ERR >> Creating surface4 failed ( DDRAW = "+intToStr((int)iDD)+"!");
		}
	}

	// we can hook it
	if (surf && *surf)
	{
		IDirectDrawSurface4 *surface = (IDirectDrawSurface4*)*surf;
		IDirectDraw4Surface_setupHook(surface);
	}

	return fnResult;
}

HRESULT WINAPI IDirectDraw4Hooked_QueryInterface(IDirectDraw4 *iDD, const _GUID &guid, void **obp)
{
	//DebugBreak();	

	CLogger::addEntry("Calling IDirectDraw4Hooked_QueryInterface, GUID = "+uintToStr(guid.Data1));

	HRESULT fnResult = E_FAIL;	

	bool found = (originalIDirectDraw4Vtbls.find(iDD) != originalIDirectDraw4Vtbls.end());

	if (found)
	{
		fnResult = originalIDirectDraw4Vtbls[iDD].QueryInterface(iDD,guid,obp);	
	}

	return fnResult;
}

// auto generated

HRESULT WINAPI IDirectDraw4Hooked_Compact(IDirectDraw4* p0)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDraw4Hooked_Compact = "+intToStr((int)p0));

	bool found = (originalIDirectDraw4Vtbls.find(p0) != originalIDirectDraw4Vtbls.end());
	if (found)
		res = originalIDirectDraw4Vtbls[p0].Compact(p0);
	else
		res = originalIDirectDraw4Vtbls.begin()->second.Compact(p0);

	return res;
}

HRESULT WINAPI IDirectDraw4Hooked_CreateClipper(IDirectDraw4* p0, DWORD p1, LPDIRECTDRAWCLIPPER FAR* p2, IUnknown FAR * p3)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDraw4Hooked_CreateClipper = "+intToStr((int)p0));

	bool found = (originalIDirectDraw4Vtbls.find(p0) != originalIDirectDraw4Vtbls.end());
	if (found)
		res = originalIDirectDraw4Vtbls[p0].CreateClipper(p0,p1,p2,p3);
	else
		res = originalIDirectDraw4Vtbls.begin()->second.CreateClipper(p0,p1,p2,p3);

	return res;
}

HRESULT WINAPI IDirectDraw4Hooked_CreatePalette(IDirectDraw4* p0, DWORD p1, LPPALETTEENTRY p2, LPDIRECTDRAWPALETTE FAR* p3, IUnknown FAR * p4)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDraw4Hooked_CreatePalette = "+intToStr((int)p0));

	bool found = (originalIDirectDraw4Vtbls.find(p0) != originalIDirectDraw4Vtbls.end());
	if (found)
		res = originalIDirectDraw4Vtbls[p0].CreatePalette(p0,p1,p2,p3,p4);
	else
		res = originalIDirectDraw4Vtbls.begin()->second.CreatePalette(p0,p1,p2,p3,p4);

	return res;
}

HRESULT WINAPI IDirectDraw4Hooked_DuplicateSurface(IDirectDraw4* p0, LPDIRECTDRAWSURFACE4 p1, LPDIRECTDRAWSURFACE4 FAR * p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDraw4Hooked_DuplicateSurface = "+intToStr((int)p0));

	bool found = (originalIDirectDraw4Vtbls.find(p0) != originalIDirectDraw4Vtbls.end());
	if (found)
		res = originalIDirectDraw4Vtbls[p0].DuplicateSurface(p0,p1,p2);
	else
		res = originalIDirectDraw4Vtbls.begin()->second.DuplicateSurface(p0,p1,p2);

	if(p1)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p1->lpVtbl->GetSurfaceDesc(p1,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	if(p2 && *p2)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		(*p2)->lpVtbl->GetSurfaceDesc(*p2,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDraw4Hooked_EnumDisplayModes(IDirectDraw4* p0, DWORD p1, LPDDSURFACEDESC2 p2, LPVOID p3, LPDDENUMMODESCALLBACK2 p4)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDraw4Hooked_EnumDisplayModes = "+intToStr((int)p0));

	bool found = (originalIDirectDraw4Vtbls.find(p0) != originalIDirectDraw4Vtbls.end());
	if (found)
		res = originalIDirectDraw4Vtbls[p0].EnumDisplayModes(p0,p1,p2,p3,p4);
	else
		res = originalIDirectDraw4Vtbls.begin()->second.EnumDisplayModes(p0,p1,p2,p3,p4);

	return res;
}

HRESULT WINAPI IDirectDraw4Hooked_EnumSurfaces(IDirectDraw4* p0, DWORD p1, LPDDSURFACEDESC2 p2, LPVOID p3, LPDDENUMSURFACESCALLBACK2 p4)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDraw4Hooked_EnumSurfaces = "+intToStr((int)p0));

	bool found = (originalIDirectDraw4Vtbls.find(p0) != originalIDirectDraw4Vtbls.end());
	if (found)
		res = originalIDirectDraw4Vtbls[p0].EnumSurfaces(p0,p1,p2,p3,p4);
	else
		res = originalIDirectDraw4Vtbls.begin()->second.EnumSurfaces(p0,p1,p2,p3,p4);

	return res;
}

HRESULT WINAPI IDirectDraw4Hooked_FlipToGDISurface(IDirectDraw4* p0)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDraw4Hooked_FlipToGDISurface = "+intToStr((int)p0));

	bool found = (originalIDirectDraw4Vtbls.find(p0) != originalIDirectDraw4Vtbls.end());
	if (found)
		res = originalIDirectDraw4Vtbls[p0].FlipToGDISurface(p0);
	else
		res = originalIDirectDraw4Vtbls.begin()->second.FlipToGDISurface(p0);

	return res;
}

HRESULT WINAPI IDirectDraw4Hooked_GetCaps(IDirectDraw4* p0, LPDDCAPS p1, LPDDCAPS p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDraw4Hooked_GetCaps = "+intToStr((int)p0));

	bool found = (originalIDirectDraw4Vtbls.find(p0) != originalIDirectDraw4Vtbls.end());
	if (found)
		res = originalIDirectDraw4Vtbls[p0].GetCaps(p0,p1,p2);
	else
		res = originalIDirectDraw4Vtbls.begin()->second.GetCaps(p0,p1,p2);

	return res;
}

HRESULT WINAPI IDirectDraw4Hooked_GetDisplayMode(IDirectDraw4* p0, LPDDSURFACEDESC2 p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDraw4Hooked_GetDisplayMode = "+intToStr((int)p0));

	bool found = (originalIDirectDraw4Vtbls.find(p0) != originalIDirectDraw4Vtbls.end());
	if (found)
		res = originalIDirectDraw4Vtbls[p0].GetDisplayMode(p0,p1);
	else
		res = originalIDirectDraw4Vtbls.begin()->second.GetDisplayMode(p0,p1);

	return res;
}

HRESULT WINAPI IDirectDraw4Hooked_GetFourCCCodes(IDirectDraw4* p0, LPDWORD p1, LPDWORD p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDraw4Hooked_GetFourCCCodes = "+intToStr((int)p0));

	bool found = (originalIDirectDraw4Vtbls.find(p0) != originalIDirectDraw4Vtbls.end());
	if (found)
		res = originalIDirectDraw4Vtbls[p0].GetFourCCCodes(p0,p1,p2);
	else
		res = originalIDirectDraw4Vtbls.begin()->second.GetFourCCCodes(p0,p1,p2);

	return res;
}

HRESULT WINAPI IDirectDraw4Hooked_GetGDISurface(IDirectDraw4* p0, LPDIRECTDRAWSURFACE4 FAR * p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDraw4Hooked_GetGDISurface = "+intToStr((int)p0));

	bool found = (originalIDirectDraw4Vtbls.find(p0) != originalIDirectDraw4Vtbls.end());
	if (found)
		res = originalIDirectDraw4Vtbls[p0].GetGDISurface(p0,p1);
	else
		res = originalIDirectDraw4Vtbls.begin()->second.GetGDISurface(p0,p1);

	if(p1 && *p1)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		(*p1)->lpVtbl->GetSurfaceDesc(*p1,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDraw4Hooked_GetMonitorFrequency(IDirectDraw4* p0, LPDWORD p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDraw4Hooked_GetMonitorFrequency = "+intToStr((int)p0));

	bool found = (originalIDirectDraw4Vtbls.find(p0) != originalIDirectDraw4Vtbls.end());
	if (found)
		res = originalIDirectDraw4Vtbls[p0].GetMonitorFrequency(p0,p1);
	else
		res = originalIDirectDraw4Vtbls.begin()->second.GetMonitorFrequency(p0,p1);

	return res;
}

HRESULT WINAPI IDirectDraw4Hooked_GetScanLine(IDirectDraw4* p0, LPDWORD p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDraw4Hooked_GetScanLine = "+intToStr((int)p0));

	bool found = (originalIDirectDraw4Vtbls.find(p0) != originalIDirectDraw4Vtbls.end());
	if (found)
		res = originalIDirectDraw4Vtbls[p0].GetScanLine(p0,p1);
	else
		res = originalIDirectDraw4Vtbls.begin()->second.GetScanLine(p0,p1);

	return res;
}

HRESULT WINAPI IDirectDraw4Hooked_GetVerticalBlankStatus(IDirectDraw4* p0, LPBOOL p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDraw4Hooked_GetVerticalBlankStatus = "+intToStr((int)p0));

	bool found = (originalIDirectDraw4Vtbls.find(p0) != originalIDirectDraw4Vtbls.end());
	if (found)
		res = originalIDirectDraw4Vtbls[p0].GetVerticalBlankStatus(p0,p1);
	else
		res = originalIDirectDraw4Vtbls.begin()->second.GetVerticalBlankStatus(p0,p1);

	return res;
}

HRESULT WINAPI IDirectDraw4Hooked_Initialize(IDirectDraw4* p0, GUID FAR * p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDraw4Hooked_Initialize = "+intToStr((int)p0));

	bool found = (originalIDirectDraw4Vtbls.find(p0) != originalIDirectDraw4Vtbls.end());
	if (found)
		res = originalIDirectDraw4Vtbls[p0].Initialize(p0,p1);
	else
		res = originalIDirectDraw4Vtbls.begin()->second.Initialize(p0,p1);

	return res;
}

HRESULT WINAPI IDirectDraw4Hooked_RestoreDisplayMode(IDirectDraw4* p0)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDraw4Hooked_RestoreDisplayMode = "+intToStr((int)p0));

	bool found = (originalIDirectDraw4Vtbls.find(p0) != originalIDirectDraw4Vtbls.end());
	if (found)
		res = originalIDirectDraw4Vtbls[p0].RestoreDisplayMode(p0);
	else
		res = originalIDirectDraw4Vtbls.begin()->second.RestoreDisplayMode(p0);

	return res;
}

HRESULT WINAPI IDirectDraw4Hooked_WaitForVerticalBlank(IDirectDraw4* p0, DWORD p1, HANDLE p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDraw4Hooked_WaitForVerticalBlank = "+intToStr((int)p0));

	bool found = (originalIDirectDraw4Vtbls.find(p0) != originalIDirectDraw4Vtbls.end());
	if (found)
		res = originalIDirectDraw4Vtbls[p0].WaitForVerticalBlank(p0,p1,p2);
	else
		res = originalIDirectDraw4Vtbls.begin()->second.WaitForVerticalBlank(p0,p1,p2);

	return res;
}

HRESULT WINAPI IDirectDraw4Hooked_GetAvailableVidMem(IDirectDraw4* p0, LPDDSCAPS2 p1, LPDWORD p2, LPDWORD p3)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDraw4Hooked_GetAvailableVidMem = "+intToStr((int)p0));

	bool found = (originalIDirectDraw4Vtbls.find(p0) != originalIDirectDraw4Vtbls.end());
	if (found)
		res = originalIDirectDraw4Vtbls[p0].GetAvailableVidMem(p0,p1,p2,p3);
	else
		res = originalIDirectDraw4Vtbls.begin()->second.GetAvailableVidMem(p0,p1,p2,p3);

	return res;
}

HRESULT WINAPI IDirectDraw4Hooked_GetSurfaceFromDC(IDirectDraw4* p0, HDC p1, LPDIRECTDRAWSURFACE4 * p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDraw4Hooked_GetSurfaceFromDC = "+intToStr((int)p0));

	bool found = (originalIDirectDraw4Vtbls.find(p0) != originalIDirectDraw4Vtbls.end());
	if (found)
		res = originalIDirectDraw4Vtbls[p0].GetSurfaceFromDC(p0,p1,p2);
	else
		res = originalIDirectDraw4Vtbls.begin()->second.GetSurfaceFromDC(p0,p1,p2);

	if(p2 && *p2)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		(*p2)->lpVtbl->GetSurfaceDesc(*p2,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDraw4Hooked_RestoreAllSurfaces(IDirectDraw4* p0)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDraw4Hooked_RestoreAllSurfaces = "+intToStr((int)p0));

	bool found = (originalIDirectDraw4Vtbls.find(p0) != originalIDirectDraw4Vtbls.end());
	if (found)
		res = originalIDirectDraw4Vtbls[p0].RestoreAllSurfaces(p0);
	else
		res = originalIDirectDraw4Vtbls.begin()->second.RestoreAllSurfaces(p0);

	return res;
}

HRESULT WINAPI IDirectDraw4Hooked_TestCooperativeLevel(IDirectDraw4* p0)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDraw4Hooked_TestCooperativeLevel = "+intToStr((int)p0));

	bool found = (originalIDirectDraw4Vtbls.find(p0) != originalIDirectDraw4Vtbls.end());
	if (found)
		res = originalIDirectDraw4Vtbls[p0].TestCooperativeLevel(p0);
	else
		res = originalIDirectDraw4Vtbls.begin()->second.TestCooperativeLevel(p0);

	return res;
}

HRESULT WINAPI IDirectDraw4Hooked_GetDeviceIdentifier(IDirectDraw4* p0, LPDDDEVICEIDENTIFIER p1, DWORD p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDraw4Hooked_GetDeviceIdentifier = "+intToStr((int)p0));

	bool found = (originalIDirectDraw4Vtbls.find(p0) != originalIDirectDraw4Vtbls.end());
	if (found)
		res = originalIDirectDraw4Vtbls[p0].GetDeviceIdentifier(p0,p1,p2);
	else
		res = originalIDirectDraw4Vtbls.begin()->second.GetDeviceIdentifier(p0,p1,p2);

	return res;
}

#endif // IDirectDraw4Hooked_H