#ifndef IDirectDrawSurfaceHooked_H
#define IDirectDrawSurfaceHooked_H

static int replaceTextureIndex = 0;
static int remaps[] = {1,7,7,7,7,7,1,1};

HRESULT WINAPI IDirectDrawSurfaceHooked_QueryInterface(IDirectDrawSurface *idds, REFIID riid, LPVOID FAR *ppvObj)
{
	//DebugBreak();
	HRESULT fnResult = originalIDirectDrawSurfaceVtbls[idds].QueryInterface(idds,riid,ppvObj);

	DDSURFACEDESC sDesc;

	sDesc.dwSize = sizeof(DDSURFACEDESC);

	idds->lpVtbl->GetSurfaceDesc(idds,&sDesc);

	if (riid == IID_IDirect3DTexture2)
	{
		// we created a texture
		IDirect3DTexture2 *texture = (IDirect3DTexture2*)*ppvObj;

		//TODO: http://www.gamedev.net/reference/articles/article787.asp, LOAD THE IMAGE

		if (sDesc.dwWidth == 32 && sDesc.dwHeight == 32)
		{
			//*idds = *CMNS_CreateBitmapSurface("d:\\test.bmp",true);
			//*idds = CreateBitmapSurface

			//TODO: create a new surface adn return texture to its content CreateBitmapSurface
			//CMNS_uploadTexture(idds,"d:\\test.bmp");
			//CLogger::addEntry("REPLACED A 32x32 TEXTURE!");
		}
		/*else if (sDesc.dwWidth == 256 && sDesc.dwHeight == 256)		
		{
			// just to se what texture is in which surface
			//DebugBreak();
			std::string name = "d:\\img_dmb"+intToStr((int)texture)+".bmp";
			CopySurface(idds,name);
		}*/
		else if (sDesc.dwWidth == 1024 && sDesc.dwHeight == 1024)		
		{
			//CLogger::addEntry("Using replace index: "+intToStr(replaceTextureIndex));
			char texName[128];
			sprintf(texName,"DKER/tile%d.bmp",(replaceTextureIndex>8?remaps[replaceTextureIndex-9]:replaceTextureIndex));

			replaceTextureIndex++;

			if (CMNS_uploadTexture(idds,texName))
			{
				CLogger::addEntry("REPLACED A 1024x1024 TEXTURE! ("+std::string(texName)+")");
			}
			else
			{
				CLogger::addEntry("FAILED REPLACING A 1024x1024 TEXTURE! ("+std::string(texName)+")");
			}
		}

		bool contains = false;

		contains |= (originalIDirect3DTexture2Vtbls.find(texture) != originalIDirect3DTexture2Vtbls.end());

		if (!contains)
		{
			memcpy(&originalIDirect3DTexture2Vtbls[texture],texture->lpVtbl,sizeof(IDirect3DTexture2Vtbl));
		}

		CLogger::addEntry("Called 'IDirectDrawSurfaceHooked_QueryInterface' for surface("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight)+") = "+intToStr((int)idds)+", RIID[0] = "+uintToStr(riid.Data1)+" <-- TEXTURE");
	}
	else
	{
		CLogger::addEntry("Called 'IDirectDrawSurfaceHooked_QueryInterface' for surface("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight)+") = "+intToStr((int)idds)+", RIID[0] = "+uintToStr(riid.Data1));

		if (riid == IID_IDirectDrawSurface4)
		{			
			IDirectDrawSurface4 *surface = (IDirectDrawSurface4*)*ppvObj;
			IDirectDraw4Surface_setupHook(surface);
		}
		else if (riid == IID_IDirectDrawSurface3)
		{
			IDirectDrawSurface3 *surface = (IDirectDrawSurface3*)*ppvObj;
			IDirectDraw3Surface_setupHook(surface);
		}
		else
		{
			CLogger::addEntry("Called 'IDirectDrawSurfaceHooked_QueryInterface' and created ??????, RIID[0] = "+uintToStr(riid.Data1));
		}
	}

	return fnResult;
}

HRESULT WINAPI IDirectDrawSurfaceHooked_Blt(IDirectDrawSurface *idds, LPRECT lpDestRect, LPDIRECTDRAWSURFACE lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwFlags, LPDDBLTFX lpDDBltFx)
{
	//DebugBreak();
	HRESULT fnResult = E_FAIL;

	bool found = (originalIDirectDrawSurfaceVtbls.find(idds) != originalIDirectDrawSurfaceVtbls.end());

	if (found)
	{
		if (lpDestRect)
		{
			DDSURFACEDESC sDesc;
			sDesc.dwSize = sizeof(DDSURFACEDESC);
			idds->lpVtbl->GetSurfaceDesc(idds,&sDesc);

			// simple textures get bliped here!!
			/*if (CMNS_getWidth(lpDestRect)*2 < sDesc.dwWidth)
			{
				lpDestRect->right *= 2;
				CLogger::addEntry("Called IDirectDrawSurfaceHooked_Blt MULTIPLY SURFACE");
			}
			else
			{
				CLogger::addEntry("Called IDirectDrawSurfaceHooked_Blt RESIZE NOT POSSIBLE ("+intToStr(CMNS_getWidth(lpDestRect))+", "+intToStr(sDesc.dwWidth)+")");
			}*/
		}	

		if (lpDestRect && lpDestRect->right == 32 && lpDestRect->bottom == 32)
		{
			lpDestRect->right = 64;
			lpDestRect->bottom = 64;
		}

		if (lpSrcRect && lpSrcRect->right == 32 && lpSrcRect->bottom == 32)
		{
			lpSrcRect->right = 64;
			lpSrcRect->bottom = 64;
		}

		if ((lpDestRect && CMNS_getWidth(lpDestRect) == 64) || (lpSrcRect && CMNS_getWidth(lpSrcRect) == 64))
		{
			//CMNS_CopySurface(idds,"D:/tmp/slika"+intToStr((int)idds)+".bmp");
			CMNS_uploadTexture(lpDDSrcSurface,"d:/tmp/slika15148232.bmp");
			//CMNS_uploadTexture(lpDDSrcSurface,"d:\\test.bmp");	
			CLogger::addEntry("Replaced 64x64 texture!!!!!. (IDirectDrawSurfaceHooked_Blt)");			
		}
		
		fnResult = originalIDirectDrawSurfaceVtbls[idds].Blt(idds,lpDestRect,lpDDSrcSurface,lpSrcRect,dwFlags,lpDDBltFx);

		/*if ((lpDestRect && CMNS_getWidth(lpDestRect) == 32) || (lpSrcRect && CMNS_getWidth(lpSrcRect) == 32))
		{
			CMNS_uploadTexture(idds,"d:/tmp/slika15148232.bmp");
			CLogger::addEntry("Replaced 32x32 texture!!!!!. (IDirectDrawSurfaceHooked_Blt)");
		}*/
		
		/*if ((lpDestRect && CMNS_getWidth(lpDestRect) == 32) || (lpSrcRect && CMNS_getWidth(lpSrcRect) == 32))
		{
			CMNS_uploadTexture(idds,"d:\\test.bmp");
			CLogger::addEntry("Replaced 32x32 texture!!!!!. (IDirectDrawSurfaceHooked_Blt)");
		}*/

		if (lpSrcRect && lpDestRect)
		{
			CLogger::addEntry("Called IDirectDrawSurfaceHooked_Blt  source = "
				+intToStr(CMNS_getWidth(lpSrcRect))+", "+intToStr(CMNS_getHeight(lpSrcRect))
				+" | dest = "
				+intToStr(CMNS_getWidth(lpDestRect))+", "+intToStr(CMNS_getHeight(lpDestRect)));
		}
	}
	else
	{
		CLogger::addEntry("'Vtbl (Blt)' not found for surface = "+intToStr((int)idds));
	}

	//CLogger::addEntry("Called IDirectDrawSurfaceHooked_Blt");

	return fnResult;
}

HRESULT WINAPI IDirectDrawSurfaceHooked_BltBatch(IDirectDrawSurface *idds, _DDBLTBATCH *prc, unsigned long p0, unsigned long p1)
{
	//DebugBreak();
	HRESULT fnResult = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurfaceHooked_BltBatch.");

	bool found = (originalIDirectDrawSurfaceVtbls.find(idds) != originalIDirectDrawSurfaceVtbls.end());

	if (found)
	{
		fnResult = originalIDirectDrawSurfaceVtbls[idds].BltBatch(idds,prc,p0,p1);
	}
	else
	{
		CLogger::addEntry("'Vtbl (BltBatch)' not found for surface = "+intToStr((int)idds));
	}

	//CLogger::addEntry("Called IDirectDrawSurfaceHooked_BltBatch");

	return fnResult;
}

HRESULT WINAPI IDirectDrawSurfaceHooked_BltFast(IDirectDrawSurface *idds, DWORD dwX, DWORD dwY, LPDIRECTDRAWSURFACE lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwTrans)
{
	//DebugBreak();

	HRESULT fnResult = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawSurfaceHooked_BltFast = "+intToStr((int)idds) );

	bool found = (originalIDirectDrawSurfaceVtbls.find(idds) != originalIDirectDrawSurfaceVtbls.end());

	if (found)
	{
		fnResult = originalIDirectDrawSurfaceVtbls[idds].BltFast(idds,dwX,dwY,lpDDSrcSurface,lpSrcRect,dwTrans);
	}
	else
	{
		CLogger::addEntry("'Vtbl (BltFast)' not found for surface = "+intToStr((int)idds));
	}

	if (lpSrcRect)
	{
		CLogger::addEntry("Called IDirectDrawSurfaceHooked_BltFast  source = "
			+intToStr(CMNS_getWidth(lpSrcRect))+", "+intToStr(CMNS_getHeight(lpSrcRect)));
	}

	if (lpDDSrcSurface)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		lpDDSrcSurface->lpVtbl->GetSurfaceDesc(lpDDSrcSurface,&sDesc);

		CLogger::addEntry("src surface = ("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight)+")");
	}

	if (idds)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		idds->lpVtbl->GetSurfaceDesc(idds,&sDesc);

		CLogger::addEntry("dest (callee) surface = ("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight)+")");
	}

	return fnResult;
}

HRESULT WINAPI IDirectDrawSurfaceHooked_Flip(IDirectDrawSurface *idds, LPDIRECTDRAWSURFACE dds, DWORD p)
{
	HRESULT fnResult = E_FAIL;

	bool found = (originalIDirectDrawSurfaceVtbls.find(idds) != originalIDirectDrawSurfaceVtbls.end());

	if (idds)
	{
		DDSURFACEDESC sDescSrc;

		sDescSrc.dwSize = sizeof(DDSURFACEDESC);

		idds->lpVtbl->GetSurfaceDesc(idds,&sDescSrc);

		CLogger::addEntry("Calling IDirectDrawSurfaceHooked_Flip with src = "
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
		fnResult = originalIDirectDrawSurfaceVtbls[idds].Flip(idds,dds,p);
	}
	else
	{
		if (originalIDirectDrawSurfaceVtbls.size() > 0)
		{
			fnResult = originalIDirectDrawSurfaceVtbls.begin()->second.Flip(idds,dds,p);	
		}
		CLogger::addEntry("'Vtbl (Flip)' not found for surface = "+intToStr((int)idds));
	}

	CLogger::addEntry("Called IDirectDrawSurfaceHooked_Flip");

	return fnResult;
}

HRESULT WINAPI IDirectDrawSurfaceHooked_SetClipper(IDirectDrawSurface *idds, LPDIRECTDRAWCLIPPER ddc)
{
	CLogger::addEntry("Called IDirectDrawSurfaceHooked_SetClipper");

	HRESULT fnResult = E_FAIL;

	bool found = (originalIDirectDrawSurfaceVtbls.find(idds) != originalIDirectDrawSurfaceVtbls.end());

	if (found)
	{
		fnResult = originalIDirectDrawSurfaceVtbls[idds].SetClipper(idds,ddc);
	}
	else
	{
		if (originalIDirectDrawSurfaceVtbls.size() > 0)
		{
			fnResult = originalIDirectDrawSurfaceVtbls.begin()->second.SetClipper(idds,ddc);
		}
		CLogger::addEntry("'Vtbl (SetClipper)' not found for surface = "+intToStr((int)idds));
	}

	return fnResult;
}

HRESULT WINAPI IDirectDrawSurfaceHooked_UpdateOverlay(IDirectDrawSurface *idds, LPRECT r0, LPDIRECTDRAWSURFACE dds, LPRECT r1, DWORD p0, LPDDOVERLAYFX ddofx)
{
	CLogger::addEntry("Called IDirectDrawSurfaceHooked_UpdateOverlay");

	HRESULT fnResult = E_FAIL;

	bool found = (originalIDirectDrawSurfaceVtbls.find(idds) != originalIDirectDrawSurfaceVtbls.end());

	if (found)
	{
		fnResult = originalIDirectDrawSurfaceVtbls[idds].UpdateOverlay(idds,r0,dds,r1,p0,ddofx);
	}
	else
	{
		if (originalIDirectDrawSurfaceVtbls.size() > 0)
		{
			fnResult = originalIDirectDrawSurfaceVtbls.begin()->second.UpdateOverlay(idds,r0,dds,r1,p0,ddofx);
		}
		CLogger::addEntry("'Vtbl (IDirectDrawSurfaceHooked_UpdateOverlay)' not found for surface = "+intToStr((int)idds));
	}

	return fnResult;
}

// auto generated

HRESULT WINAPI IDirectDrawSurfaceHooked_AddAttachedSurface(IDirectDrawSurface* p0, LPDIRECTDRAWSURFACE p1)
{
	HRESULT res = E_FAIL;

	if(p0)
		CLogger::addEntry("Calling IDirectDrawSurfaceHooked_AddAttachedSurface = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurfaceVtbls.find(p0) != originalIDirectDrawSurfaceVtbls.end());
	if (found)
		res = originalIDirectDrawSurfaceVtbls[p0].AddAttachedSurface(p0,p1);
	else
		res = originalIDirectDrawSurfaceVtbls.begin()->second.AddAttachedSurface(p0,p1);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc (*PRIMARY)("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	if(p1)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p1->lpVtbl->GetSurfaceDesc(p1,&sDesc);
		CLogger::addEntry(intToStr((int)p1)+" surface desc (*BACKGROUND)("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurfaceHooked_AddOverlayDirtyRect(IDirectDrawSurface* p0, LPRECT p1)
{
	HRESULT res = E_FAIL;

	if(p0)
		CLogger::addEntry("Calling IDirectDrawSurfaceHooked_AddOverlayDirtyRect = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurfaceVtbls.find(p0) != originalIDirectDrawSurfaceVtbls.end());
	if (found)
		res = originalIDirectDrawSurfaceVtbls[p0].AddOverlayDirtyRect(p0,p1);
	else
		res = originalIDirectDrawSurfaceVtbls.begin()->second.AddOverlayDirtyRect(p0,p1);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurfaceHooked_DeleteAttachedSurface(IDirectDrawSurface* p0, DWORD p1, LPDIRECTDRAWSURFACE p2)
{
	HRESULT res = E_FAIL;

	if(p0)
		CLogger::addEntry("Calling IDirectDrawSurfaceHooked_DeleteAttachedSurface = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurfaceVtbls.find(p0) != originalIDirectDrawSurfaceVtbls.end());
	if (found)
		res = originalIDirectDrawSurfaceVtbls[p0].DeleteAttachedSurface(p0,p1,p2);
	else
		res = originalIDirectDrawSurfaceVtbls.begin()->second.DeleteAttachedSurface(p0,p1,p2);

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

HRESULT WINAPI IDirectDrawSurfaceHooked_EnumAttachedSurfaces(IDirectDrawSurface* p0, LPVOID p1, LPDDENUMSURFACESCALLBACK p2)
{
	HRESULT res = E_FAIL;

	if(p0)
		CLogger::addEntry("Calling IDirectDrawSurfaceHooked_EnumAttachedSurfaces = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurfaceVtbls.find(p0) != originalIDirectDrawSurfaceVtbls.end());
	if (found)
		res = originalIDirectDrawSurfaceVtbls[p0].EnumAttachedSurfaces(p0,p1,p2);
	else
		res = originalIDirectDrawSurfaceVtbls.begin()->second.EnumAttachedSurfaces(p0,p1,p2);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurfaceHooked_EnumOverlayZOrders(IDirectDrawSurface* p0, DWORD p1, LPVOID p2, LPDDENUMSURFACESCALLBACK p3)
{
	HRESULT res = E_FAIL;

	if(p0)
		CLogger::addEntry("Calling IDirectDrawSurfaceHooked_EnumOverlayZOrders = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurfaceVtbls.find(p0) != originalIDirectDrawSurfaceVtbls.end());
	if (found)
		res = originalIDirectDrawSurfaceVtbls[p0].EnumOverlayZOrders(p0,p1,p2,p3);
	else
		res = originalIDirectDrawSurfaceVtbls.begin()->second.EnumOverlayZOrders(p0,p1,p2,p3);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurfaceHooked_GetAttachedSurface(IDirectDrawSurface* p0, LPDDSCAPS p1, LPDIRECTDRAWSURFACE FAR * p2)
{
	HRESULT res = E_FAIL;

	if(p0)
		CLogger::addEntry("Calling IDirectDrawSurfaceHooked_GetAttachedSurface = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurfaceVtbls.find(p0) != originalIDirectDrawSurfaceVtbls.end());
	if (found)
		res = originalIDirectDrawSurfaceVtbls[p0].GetAttachedSurface(p0,p1,p2);
	else
		res = originalIDirectDrawSurfaceVtbls.begin()->second.GetAttachedSurface(p0,p1,p2);

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

HRESULT WINAPI IDirectDrawSurfaceHooked_GetBltStatus(IDirectDrawSurface* p0, DWORD p1)
{
	HRESULT res = E_FAIL;

	if(p0)
		CLogger::addEntry("Calling IDirectDrawSurfaceHooked_GetBltStatus = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurfaceVtbls.find(p0) != originalIDirectDrawSurfaceVtbls.end());
	if (found)
		res = originalIDirectDrawSurfaceVtbls[p0].GetBltStatus(p0,p1);
	else
		res = originalIDirectDrawSurfaceVtbls.begin()->second.GetBltStatus(p0,p1);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurfaceHooked_GetCaps(IDirectDrawSurface* p0, LPDDSCAPS p1)
{
	HRESULT res = E_FAIL;

	if(p0)
		CLogger::addEntry("Calling IDirectDrawSurfaceHooked_GetCaps = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurfaceVtbls.find(p0) != originalIDirectDrawSurfaceVtbls.end());
	if (found)
		res = originalIDirectDrawSurfaceVtbls[p0].GetCaps(p0,p1);
	else
		res = originalIDirectDrawSurfaceVtbls.begin()->second.GetCaps(p0,p1);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurfaceHooked_GetClipper(IDirectDrawSurface* p0, LPDIRECTDRAWCLIPPER FAR* p1)
{
	HRESULT res = E_FAIL;

	if(p0)
		CLogger::addEntry("Calling IDirectDrawSurfaceHooked_GetClipper = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurfaceVtbls.find(p0) != originalIDirectDrawSurfaceVtbls.end());
	if (found)
		res = originalIDirectDrawSurfaceVtbls[p0].GetClipper(p0,p1);
	else
		res = originalIDirectDrawSurfaceVtbls.begin()->second.GetClipper(p0,p1);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurfaceHooked_GetColorKey(IDirectDrawSurface* p0, DWORD p1, LPDDCOLORKEY p2)
{
	HRESULT res = E_FAIL;

	if(p0)
		CLogger::addEntry("Calling IDirectDrawSurfaceHooked_GetColorKey = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurfaceVtbls.find(p0) != originalIDirectDrawSurfaceVtbls.end());
	if (found)
		res = originalIDirectDrawSurfaceVtbls[p0].GetColorKey(p0,p1,p2);
	else
		res = originalIDirectDrawSurfaceVtbls.begin()->second.GetColorKey(p0,p1,p2);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurfaceHooked_GetDC(IDirectDrawSurface* p0, HDC FAR * p1)
{
	HRESULT res = E_FAIL;

	if(p0)
		CLogger::addEntry("Calling IDirectDrawSurfaceHooked_GetDC = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurfaceVtbls.find(p0) != originalIDirectDrawSurfaceVtbls.end());
	if (found)
		res = originalIDirectDrawSurfaceVtbls[p0].GetDC(p0,p1);
	else
		res = originalIDirectDrawSurfaceVtbls.begin()->second.GetDC(p0,p1);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurfaceHooked_GetFlipStatus(IDirectDrawSurface* p0, DWORD p1)
{
	HRESULT res = E_FAIL;

	if(p0)
		CLogger::addEntry("Calling IDirectDrawSurfaceHooked_GetFlipStatus = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurfaceVtbls.find(p0) != originalIDirectDrawSurfaceVtbls.end());
	if (found)
		res = originalIDirectDrawSurfaceVtbls[p0].GetFlipStatus(p0,p1);
	else
		res = originalIDirectDrawSurfaceVtbls.begin()->second.GetFlipStatus(p0,p1);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurfaceHooked_GetOverlayPosition(IDirectDrawSurface* p0, LPLONG p1, LPLONG p2)
{
	HRESULT res = E_FAIL;

	if(p0)
		CLogger::addEntry("Calling IDirectDrawSurfaceHooked_GetOverlayPosition = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurfaceVtbls.find(p0) != originalIDirectDrawSurfaceVtbls.end());
	if (found)
		res = originalIDirectDrawSurfaceVtbls[p0].GetOverlayPosition(p0,p1,p2);
	else
		res = originalIDirectDrawSurfaceVtbls.begin()->second.GetOverlayPosition(p0,p1,p2);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurfaceHooked_GetPalette(IDirectDrawSurface* p0, LPDIRECTDRAWPALETTE FAR* p1)
{
	HRESULT res = E_FAIL;

	if(p0)
		CLogger::addEntry("Calling IDirectDrawSurfaceHooked_GetPalette = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurfaceVtbls.find(p0) != originalIDirectDrawSurfaceVtbls.end());
	if (found)
		res = originalIDirectDrawSurfaceVtbls[p0].GetPalette(p0,p1);
	else
		res = originalIDirectDrawSurfaceVtbls.begin()->second.GetPalette(p0,p1);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurfaceHooked_GetPixelFormat(IDirectDrawSurface* p0, LPDDPIXELFORMAT p1)
{
	HRESULT res = E_FAIL;

	if(p0)
		CLogger::addEntry("Calling IDirectDrawSurfaceHooked_GetPixelFormat = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurfaceVtbls.find(p0) != originalIDirectDrawSurfaceVtbls.end());
	if (found)
		res = originalIDirectDrawSurfaceVtbls[p0].GetPixelFormat(p0,p1);
	else
		res = originalIDirectDrawSurfaceVtbls.begin()->second.GetPixelFormat(p0,p1);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurfaceHooked_GetSurfaceDesc(IDirectDrawSurface* p0, LPDDSURFACEDESC p1)
{
	HRESULT res = E_FAIL;

	/*if(p0)
		CLogger::addEntry("Calling IDirectDrawSurfaceHooked_GetSurfaceDesc = "+intToStr((int)p0));*/

	bool found = (originalIDirectDrawSurfaceVtbls.find(p0) != originalIDirectDrawSurfaceVtbls.end());
	if (found)
		res = originalIDirectDrawSurfaceVtbls[p0].GetSurfaceDesc(p0,p1);
	else
		res = originalIDirectDrawSurfaceVtbls.begin()->second.GetSurfaceDesc(p0,p1);

	/*if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	if(p1)
		CLogger::addEntry("LPDDSURFACEDESC = ("+intToStr(p1->dwWidth)+", "+intToStr(p1->dwHeight));*/

	return res;
}

HRESULT WINAPI IDirectDrawSurfaceHooked_Initialize(IDirectDrawSurface* p0, LPDIRECTDRAW p1, LPDDSURFACEDESC p2)
{
	HRESULT res = E_FAIL;

	if(p0)
		CLogger::addEntry("Calling IDirectDrawSurfaceHooked_Initialize = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurfaceVtbls.find(p0) != originalIDirectDrawSurfaceVtbls.end());
	if (found)
		res = originalIDirectDrawSurfaceVtbls[p0].Initialize(p0,p1,p2);
	else
		res = originalIDirectDrawSurfaceVtbls.begin()->second.Initialize(p0,p1,p2);

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

HRESULT WINAPI IDirectDrawSurfaceHooked_IsLost(IDirectDrawSurface* p0)
{
	HRESULT res = E_FAIL;

	if(p0)
		CLogger::addEntry("Calling IDirectDrawSurfaceHooked_IsLost = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurfaceVtbls.find(p0) != originalIDirectDrawSurfaceVtbls.end());
	if (found)
		res = originalIDirectDrawSurfaceVtbls[p0].IsLost(p0);
	else
		res = originalIDirectDrawSurfaceVtbls.begin()->second.IsLost(p0);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurfaceHooked_Lock(IDirectDrawSurface* p0, LPRECT p1, LPDDSURFACEDESC p2, DWORD p3, HANDLE p4)
{
	HRESULT res = E_FAIL;

	if(p0)
		CLogger::addEntry("Calling IDirectDrawSurfaceHooked_Lock = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurfaceVtbls.find(p0) != originalIDirectDrawSurfaceVtbls.end());
	if (found)
		res = originalIDirectDrawSurfaceVtbls[p0].Lock(p0,p1,p2,p3,p4);
	else
		res = originalIDirectDrawSurfaceVtbls.begin()->second.Lock(p0,p1,p2,p3,p4);

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

HRESULT WINAPI IDirectDrawSurfaceHooked_ReleaseDC(IDirectDrawSurface* p0, HDC p1)
{
	HRESULT res = E_FAIL;

	if(p0)
		CLogger::addEntry("Calling IDirectDrawSurfaceHooked_ReleaseDC = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurfaceVtbls.find(p0) != originalIDirectDrawSurfaceVtbls.end());
	if (found)
		res = originalIDirectDrawSurfaceVtbls[p0].ReleaseDC(p0,p1);
	else
		res = originalIDirectDrawSurfaceVtbls.begin()->second.ReleaseDC(p0,p1);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurfaceHooked_Restore(IDirectDrawSurface* p0)
{
	HRESULT res = E_FAIL;

	if(p0)
		CLogger::addEntry("Calling IDirectDrawSurfaceHooked_Restore = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurfaceVtbls.find(p0) != originalIDirectDrawSurfaceVtbls.end());
	if (found)
		res = originalIDirectDrawSurfaceVtbls[p0].Restore(p0);
	else
		res = originalIDirectDrawSurfaceVtbls.begin()->second.Restore(p0);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurfaceHooked_SetColorKey(IDirectDrawSurface* p0, DWORD p1, LPDDCOLORKEY p2)
{
	HRESULT res = E_FAIL;

	if(p0)
		CLogger::addEntry("Calling IDirectDrawSurfaceHooked_SetColorKey = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurfaceVtbls.find(p0) != originalIDirectDrawSurfaceVtbls.end());
	if (found)
		res = originalIDirectDrawSurfaceVtbls[p0].SetColorKey(p0,p1,p2);
	else
		res = originalIDirectDrawSurfaceVtbls.begin()->second.SetColorKey(p0,p1,p2);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurfaceHooked_SetOverlayPosition(IDirectDrawSurface* p0, LONG p1, LONG p2)
{
	HRESULT res = E_FAIL;

	if(p0)
		CLogger::addEntry("Calling IDirectDrawSurfaceHooked_SetOverlayPosition = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurfaceVtbls.find(p0) != originalIDirectDrawSurfaceVtbls.end());
	if (found)
		res = originalIDirectDrawSurfaceVtbls[p0].SetOverlayPosition(p0,p1,p2);
	else
		res = originalIDirectDrawSurfaceVtbls.begin()->second.SetOverlayPosition(p0,p1,p2);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurfaceHooked_SetPalette(IDirectDrawSurface* p0, LPDIRECTDRAWPALETTE p1)
{
	HRESULT res = E_FAIL;

	if(p0)
		CLogger::addEntry("Calling IDirectDrawSurfaceHooked_SetPalette = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurfaceVtbls.find(p0) != originalIDirectDrawSurfaceVtbls.end());
	if (found)
		res = originalIDirectDrawSurfaceVtbls[p0].SetPalette(p0,p1);
	else
		res = originalIDirectDrawSurfaceVtbls.begin()->second.SetPalette(p0,p1);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurfaceHooked_Unlock(IDirectDrawSurface* p0, LPVOID p1)
{
	HRESULT res = E_FAIL;

	if(p0)
		CLogger::addEntry("Calling IDirectDrawSurfaceHooked_Unlock = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurfaceVtbls.find(p0) != originalIDirectDrawSurfaceVtbls.end());
	if (found)
		res = originalIDirectDrawSurfaceVtbls[p0].Unlock(p0,p1);
	else
		res = originalIDirectDrawSurfaceVtbls.begin()->second.Unlock(p0,p1);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurfaceHooked_UpdateOverlayDisplay(IDirectDrawSurface* p0, DWORD p1)
{
	HRESULT res = E_FAIL;

	if(p0)
		CLogger::addEntry("Calling IDirectDrawSurfaceHooked_UpdateOverlayDisplay = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurfaceVtbls.find(p0) != originalIDirectDrawSurfaceVtbls.end());
	if (found)
		res = originalIDirectDrawSurfaceVtbls[p0].UpdateOverlayDisplay(p0,p1);
	else
		res = originalIDirectDrawSurfaceVtbls.begin()->second.UpdateOverlayDisplay(p0,p1);

	if(p0)
	{
		DDSURFACEDESC sDesc;
		sDesc.dwSize = sizeof(DDSURFACEDESC);
		p0->lpVtbl->GetSurfaceDesc(p0,&sDesc);
		CLogger::addEntry(intToStr((int)p0)+" surface desc("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));
	}

	return res;
}

HRESULT WINAPI IDirectDrawSurfaceHooked_UpdateOverlayZOrder(IDirectDrawSurface* p0, DWORD p1, LPDIRECTDRAWSURFACE p2)
{
	HRESULT res = E_FAIL;

	if(p0)
		CLogger::addEntry("Calling IDirectDrawSurfaceHooked_UpdateOverlayZOrder = "+intToStr((int)p0));

	bool found = (originalIDirectDrawSurfaceVtbls.find(p0) != originalIDirectDrawSurfaceVtbls.end());
	if (found)
		res = originalIDirectDrawSurfaceVtbls[p0].UpdateOverlayZOrder(p0,p1,p2);
	else
		res = originalIDirectDrawSurfaceVtbls.begin()->second.UpdateOverlayZOrder(p0,p1,p2);

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

#endif // IDirectDrawSurfaceHooked_H