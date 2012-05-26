#ifndef IDirect3DHooked_H
#define IDirect3DHooked_H

HRESULT WINAPI IDirect3DHooked_QueryInterface(IDirect3D2 *iD3D, const _GUID &guid, void **obp)
{
	//DebugBreak();	

	CLogger::addEntry("Calling IDirect3DHooked_QueryInterface, GUID = "+uintToStr(guid.Data1));

	HRESULT fnResult = E_FAIL;

	fnResult = originalDirect3D2Vtbl->QueryInterface(iD3D,guid,obp);

	return fnResult;
}

HRESULT WINAPI IDirect3DHooked_CreateMaterial(IDirect3D2 *iD3D, LPDIRECT3DMATERIAL2 *id3dm, IUnknown *pUnkOuter)
{
	//DebugBreak();	

	CLogger::addEntry("Calling IDirect3DHooked_CreateMaterial");

	HRESULT fnResult = E_FAIL;

	fnResult = originalDirect3D2Vtbl->CreateMaterial(iD3D,id3dm,pUnkOuter);

	return fnResult;
}

HRESULT WINAPI IDirect3DHooked_CreateDevice(IDirect3D2 *iD3D, const IID &guid, LPDIRECTDRAWSURFACE ddsurf, LPDIRECT3DDEVICE2 *d3ddevice)
{
	//DebugBreak();	

	//IN_GAME_BREAK

	CLogger::addEntry("Calling IDirect3DHooked_CreateDevice, REFCLSID = "+uintToStr(guid.Data1));

	HRESULT fnResult = E_FAIL;

	fnResult = originalDirect3D2Vtbl->CreateDevice(iD3D,guid,ddsurf,d3ddevice);

	if (fnResult == S_OK)
	{
		CLogger::addEntry("Device created!, REFCLSID = "+uintToStr(guid.Data1));

		DDSURFACEDESC sDesc;

		sDesc.dwSize = sizeof(DDSURFACEDESC);

		ddsurf->lpVtbl->GetSurfaceDesc(ddsurf,&sDesc);

		CLogger::addEntry("Called 'IDirect3DHooked_CreateDevice' with surface description: ("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight)+")");

        // hook device funcs			
		IDirect3DDevice2 *id3dd = (IDirect3DDevice2*)*d3ddevice;

		originalIDirect3DDevice2Vtbl = new IDirect3DDevice2Vtbl;
		replacedIDirect3DDevice2Vtbl = new IDirect3DDevice2Vtbl;

		memcpy(originalIDirect3DDevice2Vtbl,id3dd->lpVtbl,sizeof(IDirect3DDevice2Vtbl));
		memcpy(replacedIDirect3DDevice2Vtbl,id3dd->lpVtbl,sizeof(IDirect3DDevice2Vtbl));

		replacedIDirect3DDevice2Vtbl->QueryInterface = IDirect3DDevice2Hooked_QueryInterface;	
		replacedIDirect3DDevice2Vtbl->SetCurrentViewport = IDirect3DDevice2Hooked_SetCurrentViewport;	
		replacedIDirect3DDevice2Vtbl->SetRenderTarget = IDirect3DDevice2Hooked_SetRenderTarget;	
		replacedIDirect3DDevice2Vtbl->BeginScene = IDirect3DDevice2Hooked_BeginScene;	
		replacedIDirect3DDevice2Vtbl->EndScene = IDirect3DDevice2Hooked_EndScene;	
		replacedIDirect3DDevice2Vtbl->SetClipStatus = IDirect3DDevice2Hooked_SetClipStatus;	
		replacedIDirect3DDevice2Vtbl->SetRenderState = IDirect3DDevice2Hooked_SetRenderState;	

		// auto generated
		replacedIDirect3DDevice2Vtbl->GetCaps = IDirect3DDevice2Hooked_GetCaps;
		replacedIDirect3DDevice2Vtbl->SwapTextureHandles = IDirect3DDevice2Hooked_SwapTextureHandles;
		replacedIDirect3DDevice2Vtbl->GetStats = IDirect3DDevice2Hooked_GetStats;
		replacedIDirect3DDevice2Vtbl->AddViewport = IDirect3DDevice2Hooked_AddViewport;
		replacedIDirect3DDevice2Vtbl->DeleteViewport = IDirect3DDevice2Hooked_DeleteViewport;
		replacedIDirect3DDevice2Vtbl->NextViewport = IDirect3DDevice2Hooked_NextViewport;
		replacedIDirect3DDevice2Vtbl->EnumTextureFormats = IDirect3DDevice2Hooked_EnumTextureFormats;
		replacedIDirect3DDevice2Vtbl->GetDirect3D = IDirect3DDevice2Hooked_GetDirect3D;
		replacedIDirect3DDevice2Vtbl->GetCurrentViewport = IDirect3DDevice2Hooked_GetCurrentViewport;
		replacedIDirect3DDevice2Vtbl->GetRenderTarget = IDirect3DDevice2Hooked_GetRenderTarget;
		replacedIDirect3DDevice2Vtbl->Begin = IDirect3DDevice2Hooked_Begin;
		replacedIDirect3DDevice2Vtbl->BeginIndexed = IDirect3DDevice2Hooked_BeginIndexed;
		replacedIDirect3DDevice2Vtbl->Vertex = IDirect3DDevice2Hooked_Vertex;
		replacedIDirect3DDevice2Vtbl->Index = IDirect3DDevice2Hooked_Index;
		replacedIDirect3DDevice2Vtbl->End = IDirect3DDevice2Hooked_End;
		replacedIDirect3DDevice2Vtbl->GetRenderState = IDirect3DDevice2Hooked_GetRenderState;
		replacedIDirect3DDevice2Vtbl->GetLightState = IDirect3DDevice2Hooked_GetLightState;
		replacedIDirect3DDevice2Vtbl->SetLightState = IDirect3DDevice2Hooked_SetLightState;
		replacedIDirect3DDevice2Vtbl->SetTransform = IDirect3DDevice2Hooked_SetTransform;
		replacedIDirect3DDevice2Vtbl->GetTransform = IDirect3DDevice2Hooked_GetTransform;
		replacedIDirect3DDevice2Vtbl->MultiplyTransform = IDirect3DDevice2Hooked_MultiplyTransform;
		replacedIDirect3DDevice2Vtbl->DrawPrimitive = IDirect3DDevice2Hooked_DrawPrimitive;
		replacedIDirect3DDevice2Vtbl->DrawIndexedPrimitive = IDirect3DDevice2Hooked_DrawIndexedPrimitive;
		replacedIDirect3DDevice2Vtbl->GetClipStatus = IDirect3DDevice2Hooked_GetClipStatus;

		id3dd->lpVtbl = replacedIDirect3DDevice2Vtbl;
	}

	return fnResult;
}

HRESULT WINAPI IDirect3DHooked_CreateViewport(IDirect3D2 *iD3D, LPDIRECT3DVIEWPORT2 *lpd3dvp2, IUnknown *pUnkOuter)
{
	CLogger::addEntry("Calling IDirect3DHooked_CreateViewport");

	HRESULT fnResult = E_FAIL;

	fnResult = originalDirect3D2Vtbl->CreateViewport(iD3D,lpd3dvp2,pUnkOuter);

	if (fnResult == S_OK)
	{
		IDirect3DViewport2 *vp = (IDirect3DViewport2*)*lpd3dvp2;

		originalIDirect3DViewport2Vtbl = new IDirect3DViewport2Vtbl;
		replacedIDirect3DViewport2Vtbl = new IDirect3DViewport2Vtbl;

		memcpy(originalIDirect3DViewport2Vtbl,vp->lpVtbl,sizeof(IDirect3DViewport2Vtbl));
		memcpy(replacedIDirect3DViewport2Vtbl,vp->lpVtbl,sizeof(IDirect3DViewport2Vtbl));

		replacedIDirect3DViewport2Vtbl->SetViewport = IDirect3DViewport2Hooked_SetViewport;
		replacedIDirect3DViewport2Vtbl->SetViewport2 = IDirect3DViewport2Hooked_SetViewport2;
		replacedIDirect3DViewport2Vtbl->GetViewport2 = IDirect3DViewport2Hooked_GetViewport2;
		replacedIDirect3DViewport2Vtbl->Clear = IDirect3DViewport2Hooked_Clear;
		replacedIDirect3DViewport2Vtbl->QueryInterface = IDirect3DViewport2Hooked_QueryInterface;

		vp->lpVtbl = replacedIDirect3DViewport2Vtbl;
	}

	return fnResult;
}

#endif // IDirect3DHooked