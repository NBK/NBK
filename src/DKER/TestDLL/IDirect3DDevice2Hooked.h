#ifndef IDirect3DDevice2Hooked_H
#define IDirect3DDevice2Hooked_H

HRESULT WINAPI IDirect3DDevice2Hooked_QueryInterface(IDirect3DDevice2 *id3dvp2, REFIID riid, LPVOID *ppvObj)
{
	CLogger::addEntry("Calling IDirect3DDevice2Hooked_QueryInterface, GUID = "+uintToStr(riid.Data1));

	HRESULT fnResult = originalIDirect3DDevice2Vtbl->QueryInterface(id3dvp2,riid,ppvObj);	

	return fnResult;
}

HRESULT WINAPI IDirect3DDevice2Hooked_BeginScene(IDirect3DDevice2 *id3dvp2)
{
	CLogger::addEntry("Calling IDirect3DDevice2Hooked_BeginScene");

	HRESULT fnResult = originalIDirect3DDevice2Vtbl->BeginScene(id3dvp2);	

	return fnResult;
}

HRESULT WINAPI IDirect3DDevice2Hooked_SetClipStatus(IDirect3DDevice2 *id3dvp2, LPD3DCLIPSTATUS d3dstat)
{
	CLogger::addEntry("Calling IDirect3DDevice2Hooked_SetClipStatus, LPD3DCLIPSTATUS:");
	CLogger::addEntry("Do we set 2d extents, 3D extents or status: "+intToStr(d3dstat->dwFlags));
	CLogger::addEntry("Clip status: "+intToStr(d3dstat->dwStatus));
	CLogger::addEntry("X extents: "+floatToStr(d3dstat->minx)+", "+floatToStr(d3dstat->maxx));
	CLogger::addEntry("Y extents: "+floatToStr(d3dstat->miny)+", "+floatToStr(d3dstat->maxy));
	CLogger::addEntry("Z extents: "+floatToStr(d3dstat->minz)+", "+floatToStr(d3dstat->maxz));

	HRESULT fnResult = originalIDirect3DDevice2Vtbl->SetClipStatus(id3dvp2,d3dstat);	

	return fnResult;
}

HRESULT WINAPI IDirect3DDevice2Hooked_SetCurrentViewport(IDirect3DDevice2 *id3dvp2, LPDIRECT3DVIEWPORT2 d3dvp2)
{
	CLogger::addEntry("Calling IDirect3DDevice2Hooked_SetCurrentViewport");

	if (d3dvp2)
	{
		D3DVIEWPORT2 lpvp;
		lpvp.dwSize = sizeof(D3DVIEWPORT2);
		d3dvp2->lpVtbl->GetViewport2(d3dvp2,&lpvp);
		//if (lpvp)
		{
			CLogger::addEntry("dxX = "+uintToStr(lpvp.dwX));
			CLogger::addEntry("dxY = "+uintToStr(lpvp.dwY));
			CLogger::addEntry("dwWidth = "+uintToStr(lpvp.dwWidth));
			CLogger::addEntry("dwHeight = "+uintToStr(lpvp.dwHeight));
			CLogger::addEntry("dvClipX = "+floatToStr(lpvp.dvClipX));
			CLogger::addEntry("dvClipY = "+floatToStr(lpvp.dvClipY));
			CLogger::addEntry("dvClipWidth = "+floatToStr(lpvp.dvClipWidth));
			CLogger::addEntry("dvClipHeight = "+floatToStr(lpvp.dvClipHeight));
			CLogger::addEntry("dvMinZ = "+floatToStr(lpvp.dvMinZ));
			CLogger::addEntry("dvMaxZ = "+floatToStr(lpvp.dvMaxZ));	
		}
	}

	HRESULT fnResult = originalIDirect3DDevice2Vtbl->SetCurrentViewport(id3dvp2,d3dvp2);	

	return fnResult;
}

HRESULT WINAPI IDirect3DDevice2Hooked_SetRenderTarget(IDirect3DDevice2 *id3dvp2, LPDIRECTDRAWSURFACE ddsurf, DWORD p)
{
	CLogger::addEntry("Calling IDirect3DDevice2Hooked_SetRenderTarget with surface:");

	if (ddsurf)
	{
		DDSURFACEDESC sDesc;

		sDesc.dwSize = sizeof(DDSURFACEDESC);

		ddsurf->lpVtbl->GetSurfaceDesc(ddsurf,&sDesc);

		CLogger::addEntry("("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight)+")");
	}

	HRESULT fnResult = originalIDirect3DDevice2Vtbl->SetRenderTarget(id3dvp2,ddsurf,p);	

	return fnResult;
}

HRESULT WINAPI IDirect3DDevice2Hooked_EndScene(IDirect3DDevice2 *id3dvp2)
{
	CLogger::addEntry("Calling IDirect3DDevice2Hooked_EndScene.");

	HRESULT fnResult = originalIDirect3DDevice2Vtbl->EndScene(id3dvp2);	

	return fnResult;
}

HRESULT WINAPI IDirect3DDevice2Hooked_SetRenderState(IDirect3DDevice2 *id3dvp2, D3DRENDERSTATETYPE d3drst, DWORD p)
{
	//CLogger::addEntry("Calling IDirect3DDevice2Hooked_SetRenderState = "+intToStr(d3drst));

	HRESULT fnResult = originalIDirect3DDevice2Vtbl->SetRenderState(id3dvp2,d3drst,p);	

	return fnResult;
}

// auto generated

HRESULT WINAPI IDirect3DDevice2Hooked_GetCaps(IDirect3DDevice2* p0, LPD3DDEVICEDESC p1, LPD3DDEVICEDESC p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirect3DDevice2Hooked_GetCaps = "+intToStr((int)p0));

		res = originalIDirect3DDevice2Vtbl->GetCaps(p0,p1,p2);

	return res;
}

HRESULT WINAPI IDirect3DDevice2Hooked_SwapTextureHandles(IDirect3DDevice2* p0, LPDIRECT3DTEXTURE2 p1, LPDIRECT3DTEXTURE2 p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirect3DDevice2Hooked_SwapTextureHandles = "+intToStr((int)p0));

		res = originalIDirect3DDevice2Vtbl->SwapTextureHandles(p0,p1,p2);

	return res;
}

HRESULT WINAPI IDirect3DDevice2Hooked_GetStats(IDirect3DDevice2* p0, LPD3DSTATS p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirect3DDevice2Hooked_GetStats = "+intToStr((int)p0));

		res = originalIDirect3DDevice2Vtbl->GetStats(p0,p1);

	return res;
}

HRESULT WINAPI IDirect3DDevice2Hooked_AddViewport(IDirect3DDevice2* p0, LPDIRECT3DVIEWPORT2 p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirect3DDevice2Hooked_AddViewport = "+intToStr((int)p0));

		res = originalIDirect3DDevice2Vtbl->AddViewport(p0,p1);

	return res;
}

HRESULT WINAPI IDirect3DDevice2Hooked_DeleteViewport(IDirect3DDevice2* p0, LPDIRECT3DVIEWPORT2 p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirect3DDevice2Hooked_DeleteViewport = "+intToStr((int)p0));

		res = originalIDirect3DDevice2Vtbl->DeleteViewport(p0,p1);

	return res;
}

HRESULT WINAPI IDirect3DDevice2Hooked_NextViewport(IDirect3DDevice2* p0, LPDIRECT3DVIEWPORT2 p1, LPDIRECT3DVIEWPORT2* p2, DWORD p3)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirect3DDevice2Hooked_NextViewport = "+intToStr((int)p0));

		res = originalIDirect3DDevice2Vtbl->NextViewport(p0,p1,p2,p3);

	return res;
}

HRESULT WINAPI IDirect3DDevice2Hooked_EnumTextureFormats(IDirect3DDevice2* p0, LPD3DENUMTEXTUREFORMATSCALLBACK p1, LPVOID p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirect3DDevice2Hooked_EnumTextureFormats = "+intToStr((int)p0));

		res = originalIDirect3DDevice2Vtbl->EnumTextureFormats(p0,p1,p2);

	return res;
}

HRESULT WINAPI IDirect3DDevice2Hooked_GetDirect3D(IDirect3DDevice2* p0, LPDIRECT3D2* p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirect3DDevice2Hooked_GetDirect3D = "+intToStr((int)p0));

		res = originalIDirect3DDevice2Vtbl->GetDirect3D(p0,p1);

	return res;
}

HRESULT WINAPI IDirect3DDevice2Hooked_GetCurrentViewport(IDirect3DDevice2* p0, LPDIRECT3DVIEWPORT2 * p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirect3DDevice2Hooked_GetCurrentViewport = "+intToStr((int)p0));

		res = originalIDirect3DDevice2Vtbl->GetCurrentViewport(p0,p1);

	return res;
}

HRESULT WINAPI IDirect3DDevice2Hooked_GetRenderTarget(IDirect3DDevice2* p0, LPDIRECTDRAWSURFACE * p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirect3DDevice2Hooked_GetRenderTarget = "+intToStr((int)p0));

		res = originalIDirect3DDevice2Vtbl->GetRenderTarget(p0,p1);

	if(p1 && *p1)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		(*p1)->lpVtbl->GetSurfaceDesc(*p1,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirect3DDevice2Hooked_Begin(IDirect3DDevice2* p0, D3DPRIMITIVETYPE p1, D3DVERTEXTYPE p2, DWORD p3)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirect3DDevice2Hooked_Begin = "+intToStr((int)p0));

		res = originalIDirect3DDevice2Vtbl->Begin(p0,p1,p2,p3);

	return res;
}

HRESULT WINAPI IDirect3DDevice2Hooked_BeginIndexed(IDirect3DDevice2* p0, D3DPRIMITIVETYPE p1, D3DVERTEXTYPE p2, LPVOID p3, DWORD p4, DWORD p5)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirect3DDevice2Hooked_BeginIndexed = "+intToStr((int)p0));

		res = originalIDirect3DDevice2Vtbl->BeginIndexed(p0,p1,p2,p3,p4,p5);

	return res;
}

HRESULT WINAPI IDirect3DDevice2Hooked_Vertex(IDirect3DDevice2* p0, LPVOID p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirect3DDevice2Hooked_Vertex = "+intToStr((int)p0));

		res = originalIDirect3DDevice2Vtbl->Vertex(p0,p1);

	return res;
}

HRESULT WINAPI IDirect3DDevice2Hooked_Index(IDirect3DDevice2* p0, WORD p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirect3DDevice2Hooked_Index = "+intToStr((int)p0));

		res = originalIDirect3DDevice2Vtbl->Index(p0,p1);

	return res;
}

HRESULT WINAPI IDirect3DDevice2Hooked_End(IDirect3DDevice2* p0, DWORD p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirect3DDevice2Hooked_End = "+intToStr((int)p0));

		res = originalIDirect3DDevice2Vtbl->End(p0,p1);

	return res;
}

HRESULT WINAPI IDirect3DDevice2Hooked_GetRenderState(IDirect3DDevice2* p0, D3DRENDERSTATETYPE p1, LPDWORD p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirect3DDevice2Hooked_GetRenderState = "+intToStr((int)p0));

		res = originalIDirect3DDevice2Vtbl->GetRenderState(p0,p1,p2);

	return res;
}

HRESULT WINAPI IDirect3DDevice2Hooked_GetLightState(IDirect3DDevice2* p0, D3DLIGHTSTATETYPE p1, LPDWORD p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirect3DDevice2Hooked_GetLightState = "+intToStr((int)p0));

		res = originalIDirect3DDevice2Vtbl->GetLightState(p0,p1,p2);


	return res;
}

HRESULT WINAPI IDirect3DDevice2Hooked_SetLightState(IDirect3DDevice2* p0, D3DLIGHTSTATETYPE p1, DWORD p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirect3DDevice2Hooked_SetLightState = "+intToStr((int)p0));


		res = originalIDirect3DDevice2Vtbl->SetLightState(p0,p1,p2);

	return res;
}

HRESULT WINAPI IDirect3DDevice2Hooked_SetTransform(IDirect3DDevice2* p0, D3DTRANSFORMSTATETYPE p1, LPD3DMATRIX p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirect3DDevice2Hooked_SetTransform = "+intToStr((int)p0));

		res = originalIDirect3DDevice2Vtbl->SetTransform(p0,p1,p2);

	return res;
}

HRESULT WINAPI IDirect3DDevice2Hooked_GetTransform(IDirect3DDevice2* p0, D3DTRANSFORMSTATETYPE p1, LPD3DMATRIX p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirect3DDevice2Hooked_GetTransform = "+intToStr((int)p0));


		res = originalIDirect3DDevice2Vtbl->GetTransform(p0,p1,p2);


	return res;
}

HRESULT WINAPI IDirect3DDevice2Hooked_MultiplyTransform(IDirect3DDevice2* p0, D3DTRANSFORMSTATETYPE p1, LPD3DMATRIX p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirect3DDevice2Hooked_MultiplyTransform = "+intToStr((int)p0));


		res = originalIDirect3DDevice2Vtbl->MultiplyTransform(p0,p1,p2);


	return res;
}

HRESULT WINAPI IDirect3DDevice2Hooked_DrawPrimitive(IDirect3DDevice2* p0, D3DPRIMITIVETYPE p1, D3DVERTEXTYPE p2, LPVOID p3, DWORD p4, DWORD p5)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirect3DDevice2Hooked_DrawPrimitive = "+intToStr((int)p0));


		res = originalIDirect3DDevice2Vtbl->DrawPrimitive(p0,p1,p2,p3,p4,p5);


	return res;
}

HRESULT WINAPI IDirect3DDevice2Hooked_DrawIndexedPrimitive(IDirect3DDevice2* p0, D3DPRIMITIVETYPE p1, D3DVERTEXTYPE p2, LPVOID p3, DWORD p4, LPWORD p5, DWORD p6, DWORD p7)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirect3DDevice2Hooked_DrawIndexedPrimitive = "+intToStr((int)p0));


		res = originalIDirect3DDevice2Vtbl->DrawIndexedPrimitive(p0,p1,p2,p3,p4,p5,p6,p7);

	return res;
}

HRESULT WINAPI IDirect3DDevice2Hooked_GetClipStatus(IDirect3DDevice2* p0, LPD3DCLIPSTATUS p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirect3DDevice2Hooked_GetClipStatus = "+intToStr((int)p0));


		res = originalIDirect3DDevice2Vtbl->GetClipStatus(p0,p1);

	return res;
}

#endif // IDirect3DDevice2Hooked_H