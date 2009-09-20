#ifndef IDirectDrawSurface4_H
#define IDirectDrawSurface4_H

HRESULT WINAPI IDirectDrawSurface4Hooked_QueryInterface(IDirectDrawSurface4 *idds4, REFIID riid, LPVOID FAR * ppvObj)
{
	HRESULT fnResult = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_QueryInterface, GUID = "+uintToStr(riid.Data1));

	bool found = (originalIDirectDrawSurface4Vtbls.find(idds4) != originalIDirectDrawSurface4Vtbls.end());

	if (found)
	{
		fnResult = originalIDirectDrawSurface4Vtbls[idds4].QueryInterface(idds4,riid,ppvObj);
	}
	else
	{
		CLogger::addEntry("'Vtbl (IDirectDrawSurface4Hooked_QueryInterface)' not found for surface = "+intToStr((int)idds4));
	}

	if (riid == IID_IDirectDrawSurface)
	{		
		//HRESULT fnResult = originalIDirectDrawSurface4Vtbls[idds4].QueryInterface(idds4,riid,ppvObj);

		IDirectDrawSurface *surface = (IDirectDrawSurface *)*ppvObj;	
		IDirectDrawSurface_setupHook(surface);		
	}
	else
	{
		CLogger::addEntry("??? Calling IDirectDrawSurface4Hooked_QueryInterface");
	}

	return fnResult;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_Blt(IDirectDrawSurface4 *idds4, LPRECT rc0,LPDIRECTDRAWSURFACE4 dds4, LPRECT rc1, DWORD p0, LPDDBLTFX ddbltfx)
{
	HRESULT fnResult = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_Blt");

	bool found = (originalIDirectDrawSurface4Vtbls.find(idds4) != originalIDirectDrawSurface4Vtbls.end());

	if (found)
	{
		fnResult = originalIDirectDrawSurface4Vtbls[idds4].Blt(idds4,rc0,dds4,rc1,p0,ddbltfx);
	}
	else
	{
		CLogger::addEntry("'Vtbl (IDirectDrawSurface4Hooked_Blt)' not found for surface = "+intToStr((int)idds4));
	}

	return fnResult;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_Flip(IDirectDrawSurface4 *idds4, LPDIRECTDRAWSURFACE4 dds4, DWORD p0)
{
	HRESULT fnResult = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_Flip");

	bool found = (originalIDirectDrawSurface4Vtbls.find(idds4) != originalIDirectDrawSurface4Vtbls.end());

	if (found)
	{
		fnResult = originalIDirectDrawSurface4Vtbls[idds4].Flip(idds4,dds4,p0);
	}
	else
	{
		CLogger::addEntry("'Vtbl (IDirectDrawSurface4Hooked_Flip)' not found for surface = "+intToStr((int)idds4));
	}

	return fnResult;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_SetClipper(IDirectDrawSurface4 *idds4, LPDIRECTDRAWCLIPPER ddc)
{
	HRESULT fnResult = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_SetClipper");

	bool found = (originalIDirectDrawSurface4Vtbls.find(idds4) != originalIDirectDrawSurface4Vtbls.end());

	if (found)
	{
		fnResult = originalIDirectDrawSurface4Vtbls[idds4].SetClipper(idds4,ddc);
	}
	else
	{
		CLogger::addEntry("'Vtbl (IDirectDrawSurface4Hooked_SetClipper)' not found for surface = "+intToStr((int)idds4));
	}

	return fnResult;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_AddAttachedSurface(IDirectDrawSurface4 *idds4, LPDIRECTDRAWSURFACE4 dds4)
{
	HRESULT fnResult = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_AddAttachedSurface");

	if (dds4)
	{
		DDSURFACEDESC2 sDesc2;
		sDesc2.dwSize = sizeof(DDSURFACEDESC2);
		dds4->lpVtbl->GetSurfaceDesc(dds4,&sDesc2);
		CLogger::addEntry("IDirectDrawSurface4Hooked_AddAttachedSurface with surface ("+intToStr(sDesc2.dwWidth)+", "+intToStr(sDesc2.dwHeight)+")");			
	}

	bool found = (originalIDirectDrawSurface4Vtbls.find(idds4) != originalIDirectDrawSurface4Vtbls.end());

	if (found)
	{
		fnResult = originalIDirectDrawSurface4Vtbls[idds4].AddAttachedSurface(idds4,dds4);
	}
	else
	{
		CLogger::addEntry("'Vtbl (IDirectDrawSurface4Hooked_SetClipper)' not found for surface = "+intToStr((int)idds4));
	}

	return fnResult;
}

// auto generated

HRESULT WINAPI IDirectDrawSurface4Hooked_AddOverlayDirtyRect(IDirectDrawSurface4* p0, LPRECT p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_AddOverlayDirtyRect = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].AddOverlayDirtyRect(p0,p1);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.AddOverlayDirtyRect(p0,p1);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_BltBatch(IDirectDrawSurface4* p0, LPDDBLTBATCH p1, DWORD p2, DWORD p3)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_BltBatch = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].BltBatch(p0,p1,p2,p3);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.BltBatch(p0,p1,p2,p3);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_BltFast(IDirectDrawSurface4* p0, DWORD p1, DWORD p2, LPDIRECTDRAWSURFACE4 p3, LPRECT p4, DWORD p5)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_BltFast = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].BltFast(p0,p1,p2,p3,p4,p5);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.BltFast(p0,p1,p2,p3,p4,p5);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	if(p3)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p3->lpVtbl->GetSurfaceDesc(p3,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_DeleteAttachedSurface(IDirectDrawSurface4* p0, DWORD p1, LPDIRECTDRAWSURFACE4 p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_DeleteAttachedSurface = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].DeleteAttachedSurface(p0,p1,p2);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.DeleteAttachedSurface(p0,p1,p2);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	if(p2)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p2->lpVtbl->GetSurfaceDesc(p2,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_EnumAttachedSurfaces(IDirectDrawSurface4* p0, LPVOID p1, LPDDENUMSURFACESCALLBACK2 p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_EnumAttachedSurfaces = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].EnumAttachedSurfaces(p0,p1,p2);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.EnumAttachedSurfaces(p0,p1,p2);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_EnumOverlayZOrders(IDirectDrawSurface4* p0, DWORD p1, LPVOID p2, LPDDENUMSURFACESCALLBACK2 p3)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_EnumOverlayZOrders = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].EnumOverlayZOrders(p0,p1,p2,p3);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.EnumOverlayZOrders(p0,p1,p2,p3);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_GetAttachedSurface(IDirectDrawSurface4* p0, LPDDSCAPS2 p1, LPDIRECTDRAWSURFACE4 FAR * p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_GetAttachedSurface = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].GetAttachedSurface(p0,p1,p2);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.GetAttachedSurface(p0,p1,p2);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
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

HRESULT WINAPI IDirectDrawSurface4Hooked_GetBltStatus(IDirectDrawSurface4* p0, DWORD p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_GetBltStatus = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].GetBltStatus(p0,p1);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.GetBltStatus(p0,p1);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_GetCaps(IDirectDrawSurface4* p0, LPDDSCAPS2 p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_GetCaps = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].GetCaps(p0,p1);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.GetCaps(p0,p1);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_GetClipper(IDirectDrawSurface4* p0, LPDIRECTDRAWCLIPPER FAR* p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_GetClipper = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].GetClipper(p0,p1);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.GetClipper(p0,p1);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_GetColorKey(IDirectDrawSurface4* p0, DWORD p1, LPDDCOLORKEY p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_GetColorKey = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].GetColorKey(p0,p1,p2);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.GetColorKey(p0,p1,p2);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_GetDC(IDirectDrawSurface4* p0, HDC FAR * p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_GetDC = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].GetDC(p0,p1);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.GetDC(p0,p1);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_GetFlipStatus(IDirectDrawSurface4* p0, DWORD p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_GetFlipStatus = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].GetFlipStatus(p0,p1);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.GetFlipStatus(p0,p1);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_GetOverlayPosition(IDirectDrawSurface4* p0, LPLONG p1, LPLONG p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_GetOverlayPosition = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].GetOverlayPosition(p0,p1,p2);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.GetOverlayPosition(p0,p1,p2);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_GetPalette(IDirectDrawSurface4* p0, LPDIRECTDRAWPALETTE FAR* p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_GetPalette = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].GetPalette(p0,p1);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.GetPalette(p0,p1);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_GetPixelFormat(IDirectDrawSurface4* p0, LPDDPIXELFORMAT p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_GetPixelFormat = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].GetPixelFormat(p0,p1);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.GetPixelFormat(p0,p1);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_GetSurfaceDesc(IDirectDrawSurface4* p0, LPDDSURFACEDESC2 p1)
{
	HRESULT res = E_FAIL;

	//CLogger::addEntry("Calling IDirectDrawSurface4Hooked_GetSurfaceDesc = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].GetSurfaceDesc(p0,p1);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.GetSurfaceDesc(p0,p1);

	/*if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}*/

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_Initialize(IDirectDrawSurface4* p0, LPDIRECTDRAW p1, LPDDSURFACEDESC2 p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_Initialize = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].Initialize(p0,p1,p2);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.Initialize(p0,p1,p2);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_IsLost(IDirectDrawSurface4* p0)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_IsLost = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].IsLost(p0);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.IsLost(p0);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_Lock(IDirectDrawSurface4* p0, LPRECT p1, LPDDSURFACEDESC2 p2, DWORD p3, HANDLE p4)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_Lock = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].Lock(p0,p1,p2,p3,p4);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.Lock(p0,p1,p2,p3,p4);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_ReleaseDC(IDirectDrawSurface4* p0, HDC p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_ReleaseDC = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].ReleaseDC(p0,p1);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.ReleaseDC(p0,p1);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_Restore(IDirectDrawSurface4* p0)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_Restore = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].Restore(p0);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.Restore(p0);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_SetColorKey(IDirectDrawSurface4* p0, DWORD p1, LPDDCOLORKEY p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_SetColorKey = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].SetColorKey(p0,p1,p2);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.SetColorKey(p0,p1,p2);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_SetOverlayPosition(IDirectDrawSurface4* p0, LONG p1, LONG p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_SetOverlayPosition = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].SetOverlayPosition(p0,p1,p2);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.SetOverlayPosition(p0,p1,p2);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_SetPalette(IDirectDrawSurface4* p0, LPDIRECTDRAWPALETTE p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_SetPalette = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].SetPalette(p0,p1);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.SetPalette(p0,p1);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_Unlock(IDirectDrawSurface4* p0, LPRECT p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_Unlock = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].Unlock(p0,p1);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.Unlock(p0,p1);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_UpdateOverlay(IDirectDrawSurface4* p0, LPRECT p1, LPDIRECTDRAWSURFACE4 p2, LPRECT p3, DWORD p4, LPDDOVERLAYFX p5)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_UpdateOverlay = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].UpdateOverlay(p0,p1,p2,p3,p4,p5);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.UpdateOverlay(p0,p1,p2,p3,p4,p5);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	if(p2)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p2->lpVtbl->GetSurfaceDesc(p2,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_UpdateOverlayDisplay(IDirectDrawSurface4* p0, DWORD p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_UpdateOverlayDisplay = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].UpdateOverlayDisplay(p0,p1);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.UpdateOverlayDisplay(p0,p1);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_UpdateOverlayZOrder(IDirectDrawSurface4* p0, DWORD p1, LPDIRECTDRAWSURFACE4 p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_UpdateOverlayZOrder = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].UpdateOverlayZOrder(p0,p1,p2);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.UpdateOverlayZOrder(p0,p1,p2);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	if(p2)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p2->lpVtbl->GetSurfaceDesc(p2,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_GetDDInterface(IDirectDrawSurface4* p0, LPVOID FAR * p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_GetDDInterface = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].GetDDInterface(p0,p1);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.GetDDInterface(p0,p1);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_PageLock(IDirectDrawSurface4* p0, DWORD p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_PageLock = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].PageLock(p0,p1);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.PageLock(p0,p1);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_PageUnlock(IDirectDrawSurface4* p0, DWORD p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_PageUnlock = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].PageUnlock(p0,p1);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.PageUnlock(p0,p1);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_SetSurfaceDesc(IDirectDrawSurface4* p0, LPDDSURFACEDESC2 p1, DWORD p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_SetSurfaceDesc = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].SetSurfaceDesc(p0,p1,p2);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.SetSurfaceDesc(p0,p1,p2);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_SetPrivateData(IDirectDrawSurface4* p0, REFGUID p1, LPVOID p2, DWORD p3, DWORD p4)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_SetPrivateData = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].SetPrivateData(p0,p1,p2,p3,p4);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.SetPrivateData(p0,p1,p2,p3,p4);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_GetPrivateData(IDirectDrawSurface4* p0, REFGUID p1, LPVOID p2, LPDWORD p3)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_GetPrivateData = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].GetPrivateData(p0,p1,p2,p3);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.GetPrivateData(p0,p1,p2,p3);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_FreePrivateData(IDirectDrawSurface4* p0, REFGUID p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_FreePrivateData = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].FreePrivateData(p0,p1);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.FreePrivateData(p0,p1);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_GetUniquenessValue(IDirectDrawSurface4* p0, LPDWORD p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_GetUniquenessValue = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].GetUniquenessValue(p0,p1);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.GetUniquenessValue(p0,p1);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface4Hooked_ChangeUniquenessValue(IDirectDrawSurface4* p0)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface4Hooked_ChangeUniquenessValue = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface4Vtbls.find(p0) != originalIDirectDrawSurface4Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface4Vtbls[p0].ChangeUniquenessValue(p0);
	else
		res = originalIDirectDrawSurface4Vtbls.begin()->second.ChangeUniquenessValue(p0);

	if(p0)
	{
		DDSURFACEDESC2 sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC2);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

#endif // IDirectDrawSurface4_H