#ifndef IDirectDrawSurface3Hooked_H
#define IDirectDrawSurface3Hooked_H

HRESULT WINAPI IDirectDrawSurface3Hooked_QueryInterface(IDirectDrawSurface3 *idds3, REFIID riid, LPVOID FAR * ppvObj)
{
	HRESULT fnResult = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_QueryInterface, GUID = "+uintToStr(riid.Data1));

	bool found = (originalIDirectDrawSurface3Vtbls.find(idds3) != originalIDirectDrawSurface3Vtbls.end());

	if (found)
	{
		fnResult = originalIDirectDrawSurface3Vtbls[idds3].QueryInterface(idds3,riid,ppvObj);
	}
	else
	{
		CLogger::addEntry("'Vtbl (IDirectDrawSurface3Hooked_QueryInterface)' not found for surface = "+intToStr((int)idds3));
	}	

	return fnResult;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_Flip(IDirectDrawSurface3 *idds, LPDIRECTDRAWSURFACE3 dds, DWORD p)
{
	HRESULT fnResult = E_FAIL;

	bool found = (originalIDirectDrawSurface3Vtbls.find(idds) != originalIDirectDrawSurface3Vtbls.end());

	if (idds)
	{
		DDSURFACEDESC sDescSrc;

		sDescSrc.dwSize = sizeof(DDSURFACEDESC);

		idds->lpVtbl->GetSurfaceDesc(idds,&sDescSrc);

		CLogger::addEntry("Calling IDirectDrawSurface3Hooked_Flip with src = "
			+intToStr(sDescSrc.dwWidth)+", "+intToStr(sDescSrc.dwHeight));
	}

	if (dds)
	{
		DDSURFACEDESC sDescDest;

		sDescDest.dwSize = sizeof(DDSURFACEDESC);

		dds->lpVtbl->GetSurfaceDesc(dds,&sDescDest);

		CLogger::addEntry(" dest = "
			+intToStr(sDescDest.dwWidth)+", "+intToStr(sDescDest.dwHeight));
	}

	if (found)
	{
		fnResult = originalIDirectDrawSurface3Vtbls[idds].Flip(idds,dds,p);
	}
	else
	{
		if (originalIDirectDrawSurface3Vtbls.size() > 0)
		{
			fnResult = originalIDirectDrawSurface3Vtbls.begin()->second.Flip(idds,dds,p);	
		}
		CLogger::addEntry("'Vtbl (Flip)' not found for surface = "+intToStr((int)idds));
	}

	CLogger::addEntry("Called IDirectDrawSurface3Hooked_Flip");

	return fnResult;
}

// auto generated

HRESULT WINAPI IDirectDrawSurface3Hooked_AddAttachedSurface(IDirectDrawSurface3* p0, LPDIRECTDRAWSURFACE3 p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_AddAttachedSurface = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].AddAttachedSurface(p0,p1);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.AddAttachedSurface(p0,p1);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	if(p1)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p1->lpVtbl->GetSurfaceDesc(p1,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_AddOverlayDirtyRect(IDirectDrawSurface3* p0, LPRECT p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_AddOverlayDirtyRect = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].AddOverlayDirtyRect(p0,p1);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.AddOverlayDirtyRect(p0,p1);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_Blt(IDirectDrawSurface3* p0, LPRECT p1, LPDIRECTDRAWSURFACE3 p2, LPRECT p3, DWORD p4, LPDDBLTFX p5)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_Blt = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].Blt(p0,p1,p2,p3,p4,p5);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.Blt(p0,p1,p2,p3,p4,p5);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	if(p2)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p2->lpVtbl->GetSurfaceDesc(p2,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_BltBatch(IDirectDrawSurface3* p0, LPDDBLTBATCH p1, DWORD p2, DWORD p3)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_BltBatch = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].BltBatch(p0,p1,p2,p3);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.BltBatch(p0,p1,p2,p3);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_BltFast(IDirectDrawSurface3* p0, DWORD p1, DWORD p2, LPDIRECTDRAWSURFACE3 p3, LPRECT p4, DWORD p5)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_BltFast = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].BltFast(p0,p1,p2,p3,p4,p5);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.BltFast(p0,p1,p2,p3,p4,p5);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	if(p3)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p3->lpVtbl->GetSurfaceDesc(p3,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_DeleteAttachedSurface(IDirectDrawSurface3* p0, DWORD p1, LPDIRECTDRAWSURFACE3 p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_DeleteAttachedSurface = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].DeleteAttachedSurface(p0,p1,p2);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.DeleteAttachedSurface(p0,p1,p2);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	if(p2)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p2->lpVtbl->GetSurfaceDesc(p2,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_EnumAttachedSurfaces(IDirectDrawSurface3* p0, LPVOID p1, LPDDENUMSURFACESCALLBACK p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_EnumAttachedSurfaces = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].EnumAttachedSurfaces(p0,p1,p2);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.EnumAttachedSurfaces(p0,p1,p2);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_EnumOverlayZOrders(IDirectDrawSurface3* p0, DWORD p1, LPVOID p2, LPDDENUMSURFACESCALLBACK p3)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_EnumOverlayZOrders = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].EnumOverlayZOrders(p0,p1,p2,p3);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.EnumOverlayZOrders(p0,p1,p2,p3);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_GetAttachedSurface(IDirectDrawSurface3* p0, LPDDSCAPS p1, LPDIRECTDRAWSURFACE3 FAR * p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_GetAttachedSurface = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].GetAttachedSurface(p0,p1,p2);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.GetAttachedSurface(p0,p1,p2);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
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

HRESULT WINAPI IDirectDrawSurface3Hooked_GetBltStatus(IDirectDrawSurface3* p0, DWORD p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_GetBltStatus = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].GetBltStatus(p0,p1);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.GetBltStatus(p0,p1);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_GetCaps(IDirectDrawSurface3* p0, LPDDSCAPS p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_GetCaps = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].GetCaps(p0,p1);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.GetCaps(p0,p1);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_GetClipper(IDirectDrawSurface3* p0, LPDIRECTDRAWCLIPPER FAR* p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_GetClipper = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].GetClipper(p0,p1);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.GetClipper(p0,p1);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_GetColorKey(IDirectDrawSurface3* p0, DWORD p1, LPDDCOLORKEY p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_GetColorKey = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].GetColorKey(p0,p1,p2);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.GetColorKey(p0,p1,p2);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_GetDC(IDirectDrawSurface3* p0, HDC FAR * p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_GetDC = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].GetDC(p0,p1);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.GetDC(p0,p1);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_GetFlipStatus(IDirectDrawSurface3* p0, DWORD p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_GetFlipStatus = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].GetFlipStatus(p0,p1);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.GetFlipStatus(p0,p1);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_GetOverlayPosition(IDirectDrawSurface3* p0, LPLONG p1, LPLONG p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_GetOverlayPosition = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].GetOverlayPosition(p0,p1,p2);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.GetOverlayPosition(p0,p1,p2);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_GetPalette(IDirectDrawSurface3* p0, LPDIRECTDRAWPALETTE FAR* p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_GetPalette = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].GetPalette(p0,p1);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.GetPalette(p0,p1);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_GetPixelFormat(IDirectDrawSurface3* p0, LPDDPIXELFORMAT p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_GetPixelFormat = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].GetPixelFormat(p0,p1);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.GetPixelFormat(p0,p1);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_GetSurfaceDesc(IDirectDrawSurface3* p0, LPDDSURFACEDESC p1)
{
	HRESULT res = E_FAIL;

	//CLogger::addEntry("Calling IDirectDrawSurface3Hooked_GetSurfaceDesc = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].GetSurfaceDesc(p0,p1);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.GetSurfaceDesc(p0,p1);

	/*if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}*/

	if(p1)
		CLogger::addEntry("LPDDSURFACEDESC = ("+intToStr(p1->dwWidth)+", "+intToStr(p1->dwHeight));

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_Initialize(IDirectDrawSurface3* p0, LPDIRECTDRAW p1, LPDDSURFACEDESC p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_Initialize = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].Initialize(p0,p1,p2);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.Initialize(p0,p1,p2);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	if(p2)
		CLogger::addEntry("LPDDSURFACEDESC = ("+intToStr(p2->dwWidth)+", "+intToStr(p2->dwHeight));

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_IsLost(IDirectDrawSurface3* p0)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_IsLost = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].IsLost(p0);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.IsLost(p0);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_Lock(IDirectDrawSurface3* p0, LPRECT p1, LPDDSURFACEDESC p2, DWORD p3, HANDLE p4)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_Lock = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].Lock(p0,p1,p2,p3,p4);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.Lock(p0,p1,p2,p3,p4);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	if(p2)
		CLogger::addEntry("LPDDSURFACEDESC = ("+intToStr(p2->dwWidth)+", "+intToStr(p2->dwHeight));

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_ReleaseDC(IDirectDrawSurface3* p0, HDC p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_ReleaseDC = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].ReleaseDC(p0,p1);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.ReleaseDC(p0,p1);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_Restore(IDirectDrawSurface3* p0)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_Restore = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].Restore(p0);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.Restore(p0);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_SetClipper(IDirectDrawSurface3* p0, LPDIRECTDRAWCLIPPER p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_SetClipper = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].SetClipper(p0,p1);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.SetClipper(p0,p1);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_SetColorKey(IDirectDrawSurface3* p0, DWORD p1, LPDDCOLORKEY p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_SetColorKey = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].SetColorKey(p0,p1,p2);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.SetColorKey(p0,p1,p2);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_SetOverlayPosition(IDirectDrawSurface3* p0, LONG p1, LONG p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_SetOverlayPosition = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].SetOverlayPosition(p0,p1,p2);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.SetOverlayPosition(p0,p1,p2);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_SetPalette(IDirectDrawSurface3* p0, LPDIRECTDRAWPALETTE p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_SetPalette = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].SetPalette(p0,p1);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.SetPalette(p0,p1);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_Unlock(IDirectDrawSurface3* p0, LPVOID p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_Unlock = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].Unlock(p0,p1);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.Unlock(p0,p1);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_UpdateOverlay(IDirectDrawSurface3* p0, LPRECT p1, LPDIRECTDRAWSURFACE3 p2, LPRECT p3, DWORD p4, LPDDOVERLAYFX p5)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_UpdateOverlay = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].UpdateOverlay(p0,p1,p2,p3,p4,p5);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.UpdateOverlay(p0,p1,p2,p3,p4,p5);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	if(p2)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p2->lpVtbl->GetSurfaceDesc(p2,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_UpdateOverlayDisplay(IDirectDrawSurface3* p0, DWORD p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_UpdateOverlayDisplay = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].UpdateOverlayDisplay(p0,p1);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.UpdateOverlayDisplay(p0,p1);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_UpdateOverlayZOrder(IDirectDrawSurface3* p0, DWORD p1, LPDIRECTDRAWSURFACE3 p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_UpdateOverlayZOrder = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].UpdateOverlayZOrder(p0,p1,p2);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.UpdateOverlayZOrder(p0,p1,p2);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	if(p2)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p2->lpVtbl->GetSurfaceDesc(p2,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_GetDDInterface(IDirectDrawSurface3* p0, LPVOID FAR * p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_GetDDInterface = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].GetDDInterface(p0,p1);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.GetDDInterface(p0,p1);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_PageLock(IDirectDrawSurface3* p0, DWORD p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_PageLock = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].PageLock(p0,p1);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.PageLock(p0,p1);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_PageUnlock(IDirectDrawSurface3* p0, DWORD p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_PageUnlock = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].PageUnlock(p0,p1);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.PageUnlock(p0,p1);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurface3Hooked_SetSurfaceDesc(IDirectDrawSurface3* p0, LPDDSURFACEDESC p1, DWORD p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurface3Hooked_SetSurfaceDesc = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurface3Vtbls.find(p0) != originalIDirectDrawSurface3Vtbls.end());
	if (found)
		res = originalIDirectDrawSurface3Vtbls[p0].SetSurfaceDesc(p0,p1,p2);
	else
		res = originalIDirectDrawSurface3Vtbls.begin()->second.SetSurfaceDesc(p0,p1,p2);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	if(p1)
		CLogger::addEntry("LPDDSURFACEDESC = ("+intToStr(p1->dwWidth)+", "+intToStr(p1->dwHeight));

	return res;
}

#endif // IDirectDrawSurface3_H