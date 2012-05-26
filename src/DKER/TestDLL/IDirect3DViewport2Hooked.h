#ifndef IDirect3DViewport2Hooked_H
#define IDirect3DViewport2Hooked_H

HRESULT WINAPI IDirect3DViewport2Hooked_QueryInterface(IDirect3DViewport2 *id3dvp2, REFIID riid, LPVOID *ppvObj)
{
	CLogger::addEntry("Calling IDirect3DViewport2Hooked_QueryInterface, GUID = "+uintToStr(riid.Data1));

	HRESULT fnResult = originalIDirect3DViewport2Vtbl->QueryInterface(id3dvp2,riid,ppvObj);	

	return fnResult;
}

HRESULT WINAPI IDirect3DViewport2Hooked_Clear(IDirect3DViewport2 *id3dvp2, DWORD p0, LPD3DRECT lprect, DWORD p1)
{
	CLogger::addEntry("\n\nCalling IDirect3DViewport2Hooked_Clear");

	if (lprect)
	{
		CLogger::addEntry("IDirect3DViewport2 rect: "
			+intToStr(lprect->x1)
			+","+intToStr(lprect->y1)
			+","+intToStr(lprect->x2)
			+","+intToStr(lprect->y2));

		lprect->x2 = SCR_WIDTH;
		lprect->y2 = SCR_HEIGHT;
	}
	else		
	{
		CLogger::addEntry("Calling IDirect3DViewport2Hooked_Clear using default RECT!");
	}

	HRESULT fnResult = originalIDirect3DViewport2Vtbl->Clear(id3dvp2,p0,lprect,p1);	

	return fnResult;
}

HRESULT WINAPI IDirect3DViewport2Hooked_SetViewport(IDirect3DViewport2 *id3dvp2, LPD3DVIEWPORT d3dvp)
{
	CLogger::addEntry("Calling IDirect3DViewport2Hooked_SetViewport");

	HRESULT fnResult = originalIDirect3DViewport2Vtbl->SetViewport(id3dvp2,d3dvp);	

	return fnResult;
}

HRESULT WINAPI IDirect3DViewport2Hooked_SetViewport2(IDirect3DViewport2 *id3dvp2, LPD3DVIEWPORT2 d3dvp2)
{
	CLogger::addEntry("Calling IDirect3DViewport2Hooked_SetViewport2 with params:");

	//if (d3dvp2->dwWidth == 640)
	{
		//d3dvp2 = new D3DVIEWPORT2;

		d3dvp2->dwWidth = SCR_WIDTH;
		d3dvp2->dwHeight = SCR_HEIGHT;
		d3dvp2->dvClipWidth = SCR_WIDTH;
		d3dvp2->dvClipHeight = SCR_HEIGHT;
		CLogger::addEntry("(forced hires)");
	}

	HRESULT fnResult = originalIDirect3DViewport2Vtbl->SetViewport2(id3dvp2,d3dvp2);	

	CLogger::addEntry("dxX = "+uintToStr(d3dvp2->dwX));
	CLogger::addEntry("dxY = "+uintToStr(d3dvp2->dwY));
	CLogger::addEntry("dwWidth = "+uintToStr(d3dvp2->dwWidth));
	CLogger::addEntry("dwHeight = "+uintToStr(d3dvp2->dwHeight));
	CLogger::addEntry("dvClipX = "+floatToStr(d3dvp2->dvClipX));
	CLogger::addEntry("dvClipY = "+floatToStr(d3dvp2->dvClipY));
	CLogger::addEntry("dvClipWidth = "+floatToStr(d3dvp2->dvClipWidth));
	CLogger::addEntry("dvClipHeight = "+floatToStr(d3dvp2->dvClipHeight));
	CLogger::addEntry("dvMinZ = "+floatToStr(d3dvp2->dvMinZ));
	CLogger::addEntry("dvMaxZ = "+floatToStr(d3dvp2->dvMaxZ));

	return fnResult;
}

HRESULT WINAPI IDirect3DViewport2Hooked_GetViewport2(IDirect3DViewport2 *id3dvp2, LPD3DVIEWPORT2 d3dvp2)
{
	CLogger::addEntry("Calling IDirect3DViewport2Hooked_GetViewport2 with params:");

	HRESULT fnResult = originalIDirect3DViewport2Vtbl->GetViewport2(id3dvp2,d3dvp2);	

	CLogger::addEntry("dxX = "+uintToStr(d3dvp2->dwX));
	CLogger::addEntry("dxY = "+uintToStr(d3dvp2->dwY));
	CLogger::addEntry("dwWidth = "+uintToStr(d3dvp2->dwWidth));
	CLogger::addEntry("dwHeight = "+uintToStr(d3dvp2->dwHeight));
	CLogger::addEntry("dvClipX = "+floatToStr(d3dvp2->dvClipX));
	CLogger::addEntry("dvClipY = "+floatToStr(d3dvp2->dvClipY));
	CLogger::addEntry("dvClipWidth = "+floatToStr(d3dvp2->dvClipWidth));
	CLogger::addEntry("dvClipHeight = "+floatToStr(d3dvp2->dvClipHeight));
	CLogger::addEntry("dvMinZ = "+floatToStr(d3dvp2->dvMinZ));
	CLogger::addEntry("dvMaxZ = "+floatToStr(d3dvp2->dvMaxZ));

	return fnResult;
}

#endif // IDirect3DViewport2Hooked