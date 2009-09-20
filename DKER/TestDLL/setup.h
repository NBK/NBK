#ifndef SETUP_HOOKS_H
#define SETUP_HOOKS_H

HRESULT WINAPI IDirectDrawHooked_SetCooperativeLevel(IDirectDraw *iDD, HWND hwnd, DWORD dwFlag);
HRESULT WINAPI IDirectDrawHooked_SetDisplayMode(IDirectDraw *iDD, DWORD dwWidth, DWORD dwHeight, DWORD dwBpp);
HRESULT WINAPI IDirectDrawHooked_CreateSurface(IDirectDraw *iDD, LPDDSURFACEDESC lpSurfDesc, LPDIRECTDRAWSURFACE FAR *surf, IUnknown FAR *pUnkOuter);
HRESULT WINAPI IDirectDrawHooked_QueryInterface(IDirectDraw *iDD, const _GUID &guid, void **obp);

// auto generated
HRESULT WINAPI IDirectDrawHooked_Compact(IDirectDraw* p0);
HRESULT WINAPI IDirectDrawHooked_CreateClipper(IDirectDraw* p0, DWORD p1, LPDIRECTDRAWCLIPPER FAR* p2, IUnknown FAR * p3);
HRESULT WINAPI IDirectDrawHooked_CreatePalette(IDirectDraw* p0, DWORD p1, LPPALETTEENTRY p2, LPDIRECTDRAWPALETTE FAR* p3, IUnknown FAR * p4);
HRESULT WINAPI IDirectDrawHooked_DuplicateSurface(IDirectDraw* p0, LPDIRECTDRAWSURFACE p1, LPDIRECTDRAWSURFACE FAR * p2);
HRESULT WINAPI IDirectDrawHooked_EnumDisplayModes(IDirectDraw* p0, DWORD p1, LPDDSURFACEDESC p2, LPVOID p3, LPDDENUMMODESCALLBACK p4);
HRESULT WINAPI IDirectDrawHooked_EnumSurfaces(IDirectDraw* p0, DWORD p1, LPDDSURFACEDESC p2, LPVOID p3, LPDDENUMSURFACESCALLBACK p4);
HRESULT WINAPI IDirectDrawHooked_FlipToGDISurface(IDirectDraw* p0);
HRESULT WINAPI IDirectDrawHooked_GetCaps(IDirectDraw* p0, LPDDCAPS p1, LPDDCAPS p2);
HRESULT WINAPI IDirectDrawHooked_GetDisplayMode(IDirectDraw* p0, LPDDSURFACEDESC p1);
HRESULT WINAPI IDirectDrawHooked_GetFourCCCodes(IDirectDraw* p0, LPDWORD p1, LPDWORD p2);
HRESULT WINAPI IDirectDrawHooked_GetGDISurface(IDirectDraw* p0, LPDIRECTDRAWSURFACE FAR * p1);
HRESULT WINAPI IDirectDrawHooked_GetMonitorFrequency(IDirectDraw* p0, LPDWORD p1);
HRESULT WINAPI IDirectDrawHooked_GetScanLine(IDirectDraw* p0, LPDWORD p1);
HRESULT WINAPI IDirectDrawHooked_GetVerticalBlankStatus(IDirectDraw* p0, LPBOOL p1);
HRESULT WINAPI IDirectDrawHooked_Initialize(IDirectDraw* p0, GUID FAR * p1);
HRESULT WINAPI IDirectDrawHooked_RestoreDisplayMode(IDirectDraw* p0);
HRESULT WINAPI IDirectDrawHooked_WaitForVerticalBlank(IDirectDraw* p0, DWORD p1, HANDLE p2);

/************************************************/

HRESULT WINAPI IDirectDraw4Hooked_SetCooperativeLevel(IDirectDraw4 *iDD, HWND hwnd, DWORD dwFlag);
HRESULT WINAPI IDirectDraw4Hooked_SetDisplayMode(IDirectDraw4 *iDD, DWORD dwWidth, DWORD dwHeight, DWORD dwBpp, DWORD dwFreq, DWORD dwFlag);
HRESULT WINAPI IDirectDraw4Hooked_CreateSurface(IDirectDraw4 *iDD, LPDDSURFACEDESC2 lpSurfDesc, LPDIRECTDRAWSURFACE4 FAR *surf, IUnknown FAR *pUnkOuter);
HRESULT WINAPI IDirectDraw4Hooked_QueryInterface(IDirectDraw4 *iDD, const _GUID &guid, void **obp);

// auto generated
HRESULT WINAPI IDirectDraw4Hooked_Compact(IDirectDraw4* p0);
HRESULT WINAPI IDirectDraw4Hooked_CreateClipper(IDirectDraw4* p0, DWORD p1, LPDIRECTDRAWCLIPPER FAR* p2, IUnknown FAR * p3);
HRESULT WINAPI IDirectDraw4Hooked_CreatePalette(IDirectDraw4* p0, DWORD p1, LPPALETTEENTRY p2, LPDIRECTDRAWPALETTE FAR* p3, IUnknown FAR * p4);
HRESULT WINAPI IDirectDraw4Hooked_DuplicateSurface(IDirectDraw4* p0, LPDIRECTDRAWSURFACE4 p1, LPDIRECTDRAWSURFACE4 FAR * p2);
HRESULT WINAPI IDirectDraw4Hooked_EnumDisplayModes(IDirectDraw4* p0, DWORD p1, LPDDSURFACEDESC2 p2, LPVOID p3, LPDDENUMMODESCALLBACK2 p4);
HRESULT WINAPI IDirectDraw4Hooked_EnumSurfaces(IDirectDraw4* p0, DWORD p1, LPDDSURFACEDESC2 p2, LPVOID p3, LPDDENUMSURFACESCALLBACK2 p4);
HRESULT WINAPI IDirectDraw4Hooked_FlipToGDISurface(IDirectDraw4* p0);
HRESULT WINAPI IDirectDraw4Hooked_GetCaps(IDirectDraw4* p0, LPDDCAPS p1, LPDDCAPS p2);
HRESULT WINAPI IDirectDraw4Hooked_GetDisplayMode(IDirectDraw4* p0, LPDDSURFACEDESC2 p1);
HRESULT WINAPI IDirectDraw4Hooked_GetFourCCCodes(IDirectDraw4* p0, LPDWORD p1, LPDWORD p2);
HRESULT WINAPI IDirectDraw4Hooked_GetGDISurface(IDirectDraw4* p0, LPDIRECTDRAWSURFACE4 FAR * p1);
HRESULT WINAPI IDirectDraw4Hooked_GetMonitorFrequency(IDirectDraw4* p0, LPDWORD p1);
HRESULT WINAPI IDirectDraw4Hooked_GetScanLine(IDirectDraw4* p0, LPDWORD p1);
HRESULT WINAPI IDirectDraw4Hooked_GetVerticalBlankStatus(IDirectDraw4* p0, LPBOOL p1);
HRESULT WINAPI IDirectDraw4Hooked_Initialize(IDirectDraw4* p0, GUID FAR * p1);
HRESULT WINAPI IDirectDraw4Hooked_RestoreDisplayMode(IDirectDraw4* p0);
HRESULT WINAPI IDirectDraw4Hooked_WaitForVerticalBlank(IDirectDraw4* p0, DWORD p1, HANDLE p2);
HRESULT WINAPI IDirectDraw4Hooked_GetAvailableVidMem(IDirectDraw4* p0, LPDDSCAPS2 p1, LPDWORD p2, LPDWORD p3);
HRESULT WINAPI IDirectDraw4Hooked_GetSurfaceFromDC(IDirectDraw4* p0, HDC p1, LPDIRECTDRAWSURFACE4 * p2);
HRESULT WINAPI IDirectDraw4Hooked_RestoreAllSurfaces(IDirectDraw4* p0);
HRESULT WINAPI IDirectDraw4Hooked_TestCooperativeLevel(IDirectDraw4* p0);
HRESULT WINAPI IDirectDraw4Hooked_GetDeviceIdentifier(IDirectDraw4* p0, LPDDDEVICEIDENTIFIER p1, DWORD p2);

/************************************************/

HRESULT WINAPI IDirect3DHooked_QueryInterface(IDirect3D2 *iD3D, const _GUID &guid, void **obp);
HRESULT WINAPI IDirect3DHooked_CreateMaterial(IDirect3D2 *iD3D, LPDIRECT3DMATERIAL2 *id3dm, IUnknown *pUnkOuter);
HRESULT WINAPI IDirect3DHooked_CreateDevice(IDirect3D2 *iD3D, const IID &guid, LPDIRECTDRAWSURFACE ddsurf, LPDIRECT3DDEVICE2 *d3ddevice);
HRESULT WINAPI IDirect3DHooked_CreateViewport(IDirect3D2 *iD3D, LPDIRECT3DVIEWPORT2 *lpd3dvp2, IUnknown *pUnkOuter);

/************************************************/

HRESULT WINAPI IDirectDrawSurfaceHooked_QueryInterface(IDirectDrawSurface *idds, REFIID riid, LPVOID FAR *ppvObj);
HRESULT WINAPI IDirectDrawSurfaceHooked_Blt(IDirectDrawSurface *idds, LPRECT lpDestRect, LPDIRECTDRAWSURFACE lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwFlags, LPDDBLTFX lpDDBltFx);
HRESULT WINAPI IDirectDrawSurfaceHooked_BltBatch(IDirectDrawSurface *idds, _DDBLTBATCH *prc, unsigned long p0, unsigned long p1);
HRESULT WINAPI IDirectDrawSurfaceHooked_BltFast(IDirectDrawSurface *idds, DWORD dwX, DWORD dwY, LPDIRECTDRAWSURFACE lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwTrans);
HRESULT WINAPI IDirectDrawSurfaceHooked_Flip(IDirectDrawSurface *idds, LPDIRECTDRAWSURFACE dds, DWORD p);
HRESULT WINAPI IDirectDrawSurfaceHooked_SetClipper(IDirectDrawSurface *idds, LPDIRECTDRAWCLIPPER ddc);
HRESULT WINAPI IDirectDrawSurfaceHooked_UpdateOverlay(IDirectDrawSurface *idds, LPRECT r0, LPDIRECTDRAWSURFACE dds, LPRECT r1, DWORD p0, LPDDOVERLAYFX ddofx);

// auto generated
HRESULT WINAPI IDirectDrawSurfaceHooked_AddAttachedSurface(IDirectDrawSurface* p0, LPDIRECTDRAWSURFACE p1);
HRESULT WINAPI IDirectDrawSurfaceHooked_AddOverlayDirtyRect(IDirectDrawSurface* p0, LPRECT p1);
HRESULT WINAPI IDirectDrawSurfaceHooked_DeleteAttachedSurface(IDirectDrawSurface* p0, DWORD p1, LPDIRECTDRAWSURFACE p2);
HRESULT WINAPI IDirectDrawSurfaceHooked_EnumAttachedSurfaces(IDirectDrawSurface* p0, LPVOID p1, LPDDENUMSURFACESCALLBACK p2);
HRESULT WINAPI IDirectDrawSurfaceHooked_EnumOverlayZOrders(IDirectDrawSurface* p0, DWORD p1, LPVOID p2, LPDDENUMSURFACESCALLBACK p3);
HRESULT WINAPI IDirectDrawSurfaceHooked_GetAttachedSurface(IDirectDrawSurface* p0, LPDDSCAPS p1, LPDIRECTDRAWSURFACE FAR * p2);
HRESULT WINAPI IDirectDrawSurfaceHooked_GetBltStatus(IDirectDrawSurface* p0, DWORD p1);
HRESULT WINAPI IDirectDrawSurfaceHooked_GetCaps(IDirectDrawSurface* p0, LPDDSCAPS p1);
HRESULT WINAPI IDirectDrawSurfaceHooked_GetClipper(IDirectDrawSurface* p0, LPDIRECTDRAWCLIPPER FAR* p1);
HRESULT WINAPI IDirectDrawSurfaceHooked_GetColorKey(IDirectDrawSurface* p0, DWORD p1, LPDDCOLORKEY p2);
HRESULT WINAPI IDirectDrawSurfaceHooked_GetDC(IDirectDrawSurface* p0, HDC FAR * p1);
HRESULT WINAPI IDirectDrawSurfaceHooked_GetFlipStatus(IDirectDrawSurface* p0, DWORD p1);
HRESULT WINAPI IDirectDrawSurfaceHooked_GetOverlayPosition(IDirectDrawSurface* p0, LPLONG p1, LPLONG p2);
HRESULT WINAPI IDirectDrawSurfaceHooked_GetPalette(IDirectDrawSurface* p0, LPDIRECTDRAWPALETTE FAR* p1);
HRESULT WINAPI IDirectDrawSurfaceHooked_GetPixelFormat(IDirectDrawSurface* p0, LPDDPIXELFORMAT p1);
HRESULT WINAPI IDirectDrawSurfaceHooked_GetSurfaceDesc(IDirectDrawSurface* p0, LPDDSURFACEDESC p1);
HRESULT WINAPI IDirectDrawSurfaceHooked_Initialize(IDirectDrawSurface* p0, LPDIRECTDRAW p1, LPDDSURFACEDESC p2);
HRESULT WINAPI IDirectDrawSurfaceHooked_IsLost(IDirectDrawSurface* p0);
HRESULT WINAPI IDirectDrawSurfaceHooked_Lock(IDirectDrawSurface* p0, LPRECT p1, LPDDSURFACEDESC p2, DWORD p3, HANDLE p4);
HRESULT WINAPI IDirectDrawSurfaceHooked_ReleaseDC(IDirectDrawSurface* p0, HDC p1);
HRESULT WINAPI IDirectDrawSurfaceHooked_Restore(IDirectDrawSurface* p0);
HRESULT WINAPI IDirectDrawSurfaceHooked_SetColorKey(IDirectDrawSurface* p0, DWORD p1, LPDDCOLORKEY p2);
HRESULT WINAPI IDirectDrawSurfaceHooked_SetOverlayPosition(IDirectDrawSurface* p0, LONG p1, LONG p2);
HRESULT WINAPI IDirectDrawSurfaceHooked_SetPalette(IDirectDrawSurface* p0, LPDIRECTDRAWPALETTE p1);
HRESULT WINAPI IDirectDrawSurfaceHooked_Unlock(IDirectDrawSurface* p0, LPVOID p1);
HRESULT WINAPI IDirectDrawSurfaceHooked_UpdateOverlayDisplay(IDirectDrawSurface* p0, DWORD p1);
HRESULT WINAPI IDirectDrawSurfaceHooked_UpdateOverlayZOrder(IDirectDrawSurface* p0, DWORD p1, LPDIRECTDRAWSURFACE p2);

/************************************************/

HRESULT WINAPI IDirectDrawSurface3Hooked_QueryInterface(IDirectDrawSurface3 *idds3, REFIID riid, LPVOID FAR * ppvObj);
HRESULT WINAPI IDirectDrawSurface3Hooked_Flip(IDirectDrawSurface3 *idds, LPDIRECTDRAWSURFACE3 dds, DWORD p);

// auto generated
HRESULT WINAPI IDirectDrawSurface3Hooked_AddAttachedSurface(IDirectDrawSurface3* p0, LPDIRECTDRAWSURFACE3 p1);
HRESULT WINAPI IDirectDrawSurface3Hooked_AddOverlayDirtyRect(IDirectDrawSurface3* p0, LPRECT p1);
HRESULT WINAPI IDirectDrawSurface3Hooked_Blt(IDirectDrawSurface3* p0, LPRECT p1, LPDIRECTDRAWSURFACE3 p2, LPRECT p3, DWORD p4, LPDDBLTFX p5);
HRESULT WINAPI IDirectDrawSurface3Hooked_BltBatch(IDirectDrawSurface3* p0, LPDDBLTBATCH p1, DWORD p2, DWORD p3);
HRESULT WINAPI IDirectDrawSurface3Hooked_BltFast(IDirectDrawSurface3* p0, DWORD p1, DWORD p2, LPDIRECTDRAWSURFACE3 p3, LPRECT p4, DWORD p5);
HRESULT WINAPI IDirectDrawSurface3Hooked_DeleteAttachedSurface(IDirectDrawSurface3* p0, DWORD p1, LPDIRECTDRAWSURFACE3 p2);
HRESULT WINAPI IDirectDrawSurface3Hooked_EnumAttachedSurfaces(IDirectDrawSurface3* p0, LPVOID p1, LPDDENUMSURFACESCALLBACK p2);
HRESULT WINAPI IDirectDrawSurface3Hooked_EnumOverlayZOrders(IDirectDrawSurface3* p0, DWORD p1, LPVOID p2, LPDDENUMSURFACESCALLBACK p3);
HRESULT WINAPI IDirectDrawSurface3Hooked_GetAttachedSurface(IDirectDrawSurface3* p0, LPDDSCAPS p1, LPDIRECTDRAWSURFACE3 FAR * p2);
HRESULT WINAPI IDirectDrawSurface3Hooked_GetBltStatus(IDirectDrawSurface3* p0, DWORD p1);
HRESULT WINAPI IDirectDrawSurface3Hooked_GetCaps(IDirectDrawSurface3* p0, LPDDSCAPS p1);
HRESULT WINAPI IDirectDrawSurface3Hooked_GetClipper(IDirectDrawSurface3* p0, LPDIRECTDRAWCLIPPER FAR* p1);
HRESULT WINAPI IDirectDrawSurface3Hooked_GetColorKey(IDirectDrawSurface3* p0, DWORD p1, LPDDCOLORKEY p2);
HRESULT WINAPI IDirectDrawSurface3Hooked_GetDC(IDirectDrawSurface3* p0, HDC FAR * p1);
HRESULT WINAPI IDirectDrawSurface3Hooked_GetFlipStatus(IDirectDrawSurface3* p0, DWORD p1);
HRESULT WINAPI IDirectDrawSurface3Hooked_GetOverlayPosition(IDirectDrawSurface3* p0, LPLONG p1, LPLONG p2);
HRESULT WINAPI IDirectDrawSurface3Hooked_GetPalette(IDirectDrawSurface3* p0, LPDIRECTDRAWPALETTE FAR* p1);
HRESULT WINAPI IDirectDrawSurface3Hooked_GetPixelFormat(IDirectDrawSurface3* p0, LPDDPIXELFORMAT p1);
HRESULT WINAPI IDirectDrawSurface3Hooked_GetSurfaceDesc(IDirectDrawSurface3* p0, LPDDSURFACEDESC p1);
HRESULT WINAPI IDirectDrawSurface3Hooked_Initialize(IDirectDrawSurface3* p0, LPDIRECTDRAW p1, LPDDSURFACEDESC p2);
HRESULT WINAPI IDirectDrawSurface3Hooked_IsLost(IDirectDrawSurface3* p0);
HRESULT WINAPI IDirectDrawSurface3Hooked_Lock(IDirectDrawSurface3* p0, LPRECT p1, LPDDSURFACEDESC p2, DWORD p3, HANDLE p4);
HRESULT WINAPI IDirectDrawSurface3Hooked_ReleaseDC(IDirectDrawSurface3* p0, HDC p1);
HRESULT WINAPI IDirectDrawSurface3Hooked_Restore(IDirectDrawSurface3* p0);
HRESULT WINAPI IDirectDrawSurface3Hooked_SetClipper(IDirectDrawSurface3* p0, LPDIRECTDRAWCLIPPER p1);
HRESULT WINAPI IDirectDrawSurface3Hooked_SetColorKey(IDirectDrawSurface3* p0, DWORD p1, LPDDCOLORKEY p2);
HRESULT WINAPI IDirectDrawSurface3Hooked_SetOverlayPosition(IDirectDrawSurface3* p0, LONG p1, LONG p2);
HRESULT WINAPI IDirectDrawSurface3Hooked_SetPalette(IDirectDrawSurface3* p0, LPDIRECTDRAWPALETTE p1);
HRESULT WINAPI IDirectDrawSurface3Hooked_Unlock(IDirectDrawSurface3* p0, LPVOID p1);
HRESULT WINAPI IDirectDrawSurface3Hooked_UpdateOverlay(IDirectDrawSurface3* p0, LPRECT p1, LPDIRECTDRAWSURFACE3 p2, LPRECT p3, DWORD p4, LPDDOVERLAYFX p5);
HRESULT WINAPI IDirectDrawSurface3Hooked_UpdateOverlayDisplay(IDirectDrawSurface3* p0, DWORD p1);
HRESULT WINAPI IDirectDrawSurface3Hooked_UpdateOverlayZOrder(IDirectDrawSurface3* p0, DWORD p1, LPDIRECTDRAWSURFACE3 p2);
HRESULT WINAPI IDirectDrawSurface3Hooked_GetDDInterface(IDirectDrawSurface3* p0, LPVOID FAR * p1);
HRESULT WINAPI IDirectDrawSurface3Hooked_PageLock(IDirectDrawSurface3* p0, DWORD p1);
HRESULT WINAPI IDirectDrawSurface3Hooked_PageUnlock(IDirectDrawSurface3* p0, DWORD p1);
HRESULT WINAPI IDirectDrawSurface3Hooked_SetSurfaceDesc(IDirectDrawSurface3* p0, LPDDSURFACEDESC p1, DWORD p2);

/************************************************/

HRESULT WINAPI IDirectDrawSurface4Hooked_QueryInterface(IDirectDrawSurface4 *idds4, REFIID riid, LPVOID FAR * ppvObj);
HRESULT WINAPI IDirectDrawSurface4Hooked_Blt(IDirectDrawSurface4 *idds4, LPRECT rc0,LPDIRECTDRAWSURFACE4 dds4, LPRECT rc1, DWORD p0, LPDDBLTFX ddbltfx);
HRESULT WINAPI IDirectDrawSurface4Hooked_Flip(IDirectDrawSurface4 *idds4, LPDIRECTDRAWSURFACE4 dds4, DWORD p0);
HRESULT WINAPI IDirectDrawSurface4Hooked_SetClipper(IDirectDrawSurface4 *idds4, LPDIRECTDRAWCLIPPER ddc);
HRESULT WINAPI IDirectDrawSurface4Hooked_AddAttachedSurface(IDirectDrawSurface4 *idds4, LPDIRECTDRAWSURFACE4 dds4);

// auto generated
HRESULT WINAPI IDirectDrawSurface4Hooked_AddOverlayDirtyRect(IDirectDrawSurface4* p0, LPRECT p1);
HRESULT WINAPI IDirectDrawSurface4Hooked_BltBatch(IDirectDrawSurface4* p0, LPDDBLTBATCH p1, DWORD p2, DWORD p3);
HRESULT WINAPI IDirectDrawSurface4Hooked_BltFast(IDirectDrawSurface4* p0, DWORD p1, DWORD p2, LPDIRECTDRAWSURFACE4 p3, LPRECT p4, DWORD p5);
HRESULT WINAPI IDirectDrawSurface4Hooked_DeleteAttachedSurface(IDirectDrawSurface4* p0, DWORD p1, LPDIRECTDRAWSURFACE4 p2);
HRESULT WINAPI IDirectDrawSurface4Hooked_EnumAttachedSurfaces(IDirectDrawSurface4* p0, LPVOID p1, LPDDENUMSURFACESCALLBACK2 p2);
HRESULT WINAPI IDirectDrawSurface4Hooked_EnumOverlayZOrders(IDirectDrawSurface4* p0, DWORD p1, LPVOID p2, LPDDENUMSURFACESCALLBACK2 p3);
HRESULT WINAPI IDirectDrawSurface4Hooked_GetAttachedSurface(IDirectDrawSurface4* p0, LPDDSCAPS2 p1, LPDIRECTDRAWSURFACE4 FAR * p2);
HRESULT WINAPI IDirectDrawSurface4Hooked_GetBltStatus(IDirectDrawSurface4* p0, DWORD p1);
HRESULT WINAPI IDirectDrawSurface4Hooked_GetCaps(IDirectDrawSurface4* p0, LPDDSCAPS2 p1);
HRESULT WINAPI IDirectDrawSurface4Hooked_GetClipper(IDirectDrawSurface4* p0, LPDIRECTDRAWCLIPPER FAR* p1);
HRESULT WINAPI IDirectDrawSurface4Hooked_GetColorKey(IDirectDrawSurface4* p0, DWORD p1, LPDDCOLORKEY p2);
HRESULT WINAPI IDirectDrawSurface4Hooked_GetDC(IDirectDrawSurface4* p0, HDC FAR * p1);
HRESULT WINAPI IDirectDrawSurface4Hooked_GetFlipStatus(IDirectDrawSurface4* p0, DWORD p1);
HRESULT WINAPI IDirectDrawSurface4Hooked_GetOverlayPosition(IDirectDrawSurface4* p0, LPLONG p1, LPLONG p2);
HRESULT WINAPI IDirectDrawSurface4Hooked_GetPalette(IDirectDrawSurface4* p0, LPDIRECTDRAWPALETTE FAR* p1);
HRESULT WINAPI IDirectDrawSurface4Hooked_GetPixelFormat(IDirectDrawSurface4* p0, LPDDPIXELFORMAT p1);
HRESULT WINAPI IDirectDrawSurface4Hooked_GetSurfaceDesc(IDirectDrawSurface4* p0, LPDDSURFACEDESC2 p1);
HRESULT WINAPI IDirectDrawSurface4Hooked_Initialize(IDirectDrawSurface4* p0, LPDIRECTDRAW p1, LPDDSURFACEDESC2 p2);
HRESULT WINAPI IDirectDrawSurface4Hooked_IsLost(IDirectDrawSurface4* p0);
HRESULT WINAPI IDirectDrawSurface4Hooked_Lock(IDirectDrawSurface4* p0, LPRECT p1, LPDDSURFACEDESC2 p2, DWORD p3, HANDLE p4);
HRESULT WINAPI IDirectDrawSurface4Hooked_ReleaseDC(IDirectDrawSurface4* p0, HDC p1);
HRESULT WINAPI IDirectDrawSurface4Hooked_Restore(IDirectDrawSurface4* p0);
HRESULT WINAPI IDirectDrawSurface4Hooked_SetColorKey(IDirectDrawSurface4* p0, DWORD p1, LPDDCOLORKEY p2);
HRESULT WINAPI IDirectDrawSurface4Hooked_SetOverlayPosition(IDirectDrawSurface4* p0, LONG p1, LONG p2);
HRESULT WINAPI IDirectDrawSurface4Hooked_SetPalette(IDirectDrawSurface4* p0, LPDIRECTDRAWPALETTE p1);
HRESULT WINAPI IDirectDrawSurface4Hooked_Unlock(IDirectDrawSurface4* p0, LPRECT p1);
HRESULT WINAPI IDirectDrawSurface4Hooked_UpdateOverlay(IDirectDrawSurface4* p0, LPRECT p1, LPDIRECTDRAWSURFACE4 p2, LPRECT p3, DWORD p4, LPDDOVERLAYFX p5);
HRESULT WINAPI IDirectDrawSurface4Hooked_UpdateOverlayDisplay(IDirectDrawSurface4* p0, DWORD p1);
HRESULT WINAPI IDirectDrawSurface4Hooked_UpdateOverlayZOrder(IDirectDrawSurface4* p0, DWORD p1, LPDIRECTDRAWSURFACE4 p2);
HRESULT WINAPI IDirectDrawSurface4Hooked_GetDDInterface(IDirectDrawSurface4* p0, LPVOID FAR * p1);
HRESULT WINAPI IDirectDrawSurface4Hooked_PageLock(IDirectDrawSurface4* p0, DWORD p1);
HRESULT WINAPI IDirectDrawSurface4Hooked_PageUnlock(IDirectDrawSurface4* p0, DWORD p1);
HRESULT WINAPI IDirectDrawSurface4Hooked_SetSurfaceDesc(IDirectDrawSurface4* p0, LPDDSURFACEDESC2 p1, DWORD p2);
HRESULT WINAPI IDirectDrawSurface4Hooked_SetPrivateData(IDirectDrawSurface4* p0, REFGUID p1, LPVOID p2, DWORD p3, DWORD p4);
HRESULT WINAPI IDirectDrawSurface4Hooked_GetPrivateData(IDirectDrawSurface4* p0, REFGUID p1, LPVOID p2, LPDWORD p3);
HRESULT WINAPI IDirectDrawSurface4Hooked_FreePrivateData(IDirectDrawSurface4* p0, REFGUID p1);
HRESULT WINAPI IDirectDrawSurface4Hooked_GetUniquenessValue(IDirectDrawSurface4* p0, LPDWORD p1);
HRESULT WINAPI IDirectDrawSurface4Hooked_ChangeUniquenessValue(IDirectDrawSurface4* p0);

/************************************************/

HRESULT WINAPI IDirectDrawClipperHooked_QueryInterface(IDirectDrawClipper* p0, REFIID p1, LPVOID FAR * p2);
HRESULT WINAPI IDirectDrawClipperHooked_GetClipList(IDirectDrawClipper* p0, LPRECT p1, LPRGNDATA p2, LPDWORD p3);
HRESULT WINAPI IDirectDrawClipperHooked_GetHWnd(IDirectDrawClipper* p0, HWND FAR * p1);
HRESULT WINAPI IDirectDrawClipperHooked_Initialize(IDirectDrawClipper* p0, LPDIRECTDRAW p1, DWORD p2);
HRESULT WINAPI IDirectDrawClipperHooked_IsClipListChanged(IDirectDrawClipper* p0, BOOL FAR * p1);
HRESULT WINAPI IDirectDrawClipperHooked_SetClipList(IDirectDrawClipper* p0, LPRGNDATA p1, DWORD p2);
HRESULT WINAPI IDirectDrawClipperHooked_SetHWnd(IDirectDrawClipper* p0, DWORD p1, HWND p2);

HRESULT IDirectDraw_setupHook(IDirectDraw *lpDD)
{
	if (!lpDD)
	{
		CLogger::addEntry("Failed creating a new IDirectDraw ("+intToStr((int)lpDD)+")");
		return E_FAIL;
	}

	CLogger::addEntry("Created a new IDirectDraw ("+intToStr((int)lpDD)+")");

	bool IDirectDrawHooked = false;

	// test if we have already registered this interface
	IDirectDrawHooked |= (originalIDirectDrawVtbls.find(lpDD) != originalIDirectDrawVtbls.end());

	if (!IDirectDrawHooked)
	{
		if (oldOne_IDD == NULL)
		{
			memcpy(&originalIDirectDrawVtbls[lpDD],lpDD->lpVtbl,sizeof(IDirectDrawVtbl));
			oldOne_IDD = lpDD;
		}
		else
		{
			memcpy(&originalIDirectDrawVtbls[lpDD],&originalIDirectDrawVtbls[oldOne_IDD],sizeof(IDirectDrawVtbl));
			oldOne_IDD = lpDD;
		}
	}

	// test if the interface address is different but its vtbl already contains our methods
	IDirectDrawHooked |= abs((int)lpDD->lpVtbl->SetDisplayMode - (int)lpDD->lpVtbl->AddRef) > 0x900000f;

	if ( !IDirectDrawHooked )
	{			
		//Replace member functions in the real lpVtbl with ours								
		lpDD->lpVtbl->QueryInterface = IDirectDrawHooked_QueryInterface;
		lpDD->lpVtbl->SetCooperativeLevel = IDirectDrawHooked_SetCooperativeLevel;
		lpDD->lpVtbl->SetDisplayMode = IDirectDrawHooked_SetDisplayMode;
		lpDD->lpVtbl->CreateSurface = IDirectDrawHooked_CreateSurface;

		// auto generated
		lpDD->lpVtbl->Compact = IDirectDrawHooked_Compact;
		lpDD->lpVtbl->CreateClipper = IDirectDrawHooked_CreateClipper;
		lpDD->lpVtbl->CreatePalette = IDirectDrawHooked_CreatePalette;
		lpDD->lpVtbl->DuplicateSurface = IDirectDrawHooked_DuplicateSurface;
		lpDD->lpVtbl->EnumDisplayModes = IDirectDrawHooked_EnumDisplayModes;
		lpDD->lpVtbl->EnumSurfaces = IDirectDrawHooked_EnumSurfaces;
		lpDD->lpVtbl->FlipToGDISurface = IDirectDrawHooked_FlipToGDISurface;
		lpDD->lpVtbl->GetCaps = IDirectDrawHooked_GetCaps;
		lpDD->lpVtbl->GetDisplayMode = IDirectDrawHooked_GetDisplayMode;
		lpDD->lpVtbl->GetFourCCCodes = IDirectDrawHooked_GetFourCCCodes;
		lpDD->lpVtbl->GetGDISurface = IDirectDrawHooked_GetGDISurface;
		lpDD->lpVtbl->GetMonitorFrequency = IDirectDrawHooked_GetMonitorFrequency;
		lpDD->lpVtbl->GetScanLine = IDirectDrawHooked_GetScanLine;
		lpDD->lpVtbl->GetVerticalBlankStatus = IDirectDrawHooked_GetVerticalBlankStatus;
		lpDD->lpVtbl->Initialize = IDirectDrawHooked_Initialize;
		lpDD->lpVtbl->RestoreDisplayMode = IDirectDrawHooked_RestoreDisplayMode;
		lpDD->lpVtbl->WaitForVerticalBlank = IDirectDrawHooked_WaitForVerticalBlank;
	}

	return S_OK;
}

HRESULT IDirectDrawSurface_setupHook(IDirectDrawSurface *surface)
{
	if (!surface)
	{
		CLogger::addEntry("Failed creating a new IDirectDrawSurface!");
		return E_FAIL;
	}

	DDSURFACEDESC sDesc;
	sDesc.dwSize = sizeof(DDSURFACEDESC);
	surface->lpVtbl->GetSurfaceDesc(surface,&sDesc);

	CLogger::addEntry("Created IDirectDrawSurface ("+intToStr((int)surface)+") "+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight));

	bool contains = false;

	contains |= (originalIDirectDrawSurfaceVtbls.find(surface) != originalIDirectDrawSurfaceVtbls.end());

	if (!contains)
	{	
		if (oldOne_IDDS == NULL)
		{
			memcpy(&originalIDirectDrawSurfaceVtbls[surface],surface->lpVtbl,sizeof(IDirectDrawSurfaceVtbl));
			oldOne_IDDS = surface;
		}
		else
		{
			memcpy(&originalIDirectDrawSurfaceVtbls[surface],&originalIDirectDrawSurfaceVtbls[oldOne_IDDS],sizeof(IDirectDrawSurfaceVtbl));
			oldOne_IDDS = surface;
		}
	}

	contains |= abs((int)surface->lpVtbl->QueryInterface - (int)surface->lpVtbl->AddRef) > 0x900000f;

	if (!contains)
	{
		surface->lpVtbl->QueryInterface = IDirectDrawSurfaceHooked_QueryInterface;
		surface->lpVtbl->Blt = IDirectDrawSurfaceHooked_Blt;
		surface->lpVtbl->BltBatch = IDirectDrawSurfaceHooked_BltBatch;
		surface->lpVtbl->BltFast = IDirectDrawSurfaceHooked_BltFast;
		surface->lpVtbl->Flip = IDirectDrawSurfaceHooked_Flip;
		surface->lpVtbl->SetClipper = IDirectDrawSurfaceHooked_SetClipper;
		surface->lpVtbl->UpdateOverlay = IDirectDrawSurfaceHooked_UpdateOverlay;

		// auto generated
		surface->lpVtbl->AddAttachedSurface = IDirectDrawSurfaceHooked_AddAttachedSurface;
		surface->lpVtbl->AddOverlayDirtyRect = IDirectDrawSurfaceHooked_AddOverlayDirtyRect;
		surface->lpVtbl->DeleteAttachedSurface = IDirectDrawSurfaceHooked_DeleteAttachedSurface;
		surface->lpVtbl->EnumAttachedSurfaces = IDirectDrawSurfaceHooked_EnumAttachedSurfaces;
		surface->lpVtbl->EnumOverlayZOrders = IDirectDrawSurfaceHooked_EnumOverlayZOrders;
		surface->lpVtbl->GetAttachedSurface = IDirectDrawSurfaceHooked_GetAttachedSurface;
		surface->lpVtbl->GetBltStatus = IDirectDrawSurfaceHooked_GetBltStatus;
		surface->lpVtbl->GetCaps = IDirectDrawSurfaceHooked_GetCaps;
		surface->lpVtbl->GetClipper = IDirectDrawSurfaceHooked_GetClipper;
		surface->lpVtbl->GetColorKey = IDirectDrawSurfaceHooked_GetColorKey;
		surface->lpVtbl->GetDC = IDirectDrawSurfaceHooked_GetDC;
		surface->lpVtbl->GetFlipStatus = IDirectDrawSurfaceHooked_GetFlipStatus;
		surface->lpVtbl->GetOverlayPosition = IDirectDrawSurfaceHooked_GetOverlayPosition;
		surface->lpVtbl->GetPalette = IDirectDrawSurfaceHooked_GetPalette;
		surface->lpVtbl->GetPixelFormat = IDirectDrawSurfaceHooked_GetPixelFormat;
		surface->lpVtbl->GetSurfaceDesc = IDirectDrawSurfaceHooked_GetSurfaceDesc;
		surface->lpVtbl->Initialize = IDirectDrawSurfaceHooked_Initialize;
		surface->lpVtbl->IsLost = IDirectDrawSurfaceHooked_IsLost;
		surface->lpVtbl->Lock = IDirectDrawSurfaceHooked_Lock;
		surface->lpVtbl->ReleaseDC = IDirectDrawSurfaceHooked_ReleaseDC;
		surface->lpVtbl->Restore = IDirectDrawSurfaceHooked_Restore;
		surface->lpVtbl->SetColorKey = IDirectDrawSurfaceHooked_SetColorKey;
		surface->lpVtbl->SetOverlayPosition = IDirectDrawSurfaceHooked_SetOverlayPosition;
		surface->lpVtbl->SetPalette = IDirectDrawSurfaceHooked_SetPalette;
		surface->lpVtbl->Unlock = IDirectDrawSurfaceHooked_Unlock;
		surface->lpVtbl->UpdateOverlayDisplay = IDirectDrawSurfaceHooked_UpdateOverlayDisplay;
		surface->lpVtbl->UpdateOverlayZOrder = IDirectDrawSurfaceHooked_UpdateOverlayZOrder;
	}

	return S_OK;
}

HRESULT IDirectDraw4_setupHook(IDirectDraw4 *lpDD)
{
	if (!lpDD)
	{
		CLogger::addEntry("Failed creating a new IDirectDraw4 ("+intToStr((int)lpDD)+")");
		return E_FAIL;
	}

	CLogger::addEntry("Created a new IDirectDraw4 ("+intToStr((int)lpDD)+")");

	bool IDirectDraw4Hooked = false;

	// test if we have already registered this interface
	IDirectDraw4Hooked |= (originalIDirectDraw4Vtbls.find(lpDD) != originalIDirectDraw4Vtbls.end());

	if (!IDirectDraw4Hooked)
	{
		if (oldOne_IDD4 == NULL)
		{
			memcpy(&originalIDirectDraw4Vtbls[lpDD],lpDD->lpVtbl,sizeof(IDirectDraw4Vtbl));
			oldOne_IDD4 = lpDD;
		}
		else
		{
			memcpy(&originalIDirectDraw4Vtbls[lpDD],&originalIDirectDraw4Vtbls[oldOne_IDD4],sizeof(IDirectDraw4Vtbl));
			oldOne_IDD4 = lpDD;
		}
	}

	// test if the interface address is different but its vtbl already contains our methods
	IDirectDraw4Hooked |= abs((int)lpDD->lpVtbl->QueryInterface - (int)lpDD->lpVtbl->AddRef) > 0x900000f;

	if ( !IDirectDraw4Hooked )
	{			
		//Replace member functions in the real lpVtbl with ours								
		lpDD->lpVtbl->QueryInterface = IDirectDraw4Hooked_QueryInterface;
		lpDD->lpVtbl->SetCooperativeLevel = IDirectDraw4Hooked_SetCooperativeLevel;
		lpDD->lpVtbl->SetDisplayMode = IDirectDraw4Hooked_SetDisplayMode;
		lpDD->lpVtbl->CreateSurface = IDirectDraw4Hooked_CreateSurface; 

		// auto generated
		lpDD->lpVtbl->Compact = IDirectDraw4Hooked_Compact;
		lpDD->lpVtbl->CreateClipper = IDirectDraw4Hooked_CreateClipper;
		lpDD->lpVtbl->CreatePalette = IDirectDraw4Hooked_CreatePalette;
		lpDD->lpVtbl->DuplicateSurface = IDirectDraw4Hooked_DuplicateSurface;
		lpDD->lpVtbl->EnumDisplayModes = IDirectDraw4Hooked_EnumDisplayModes;
		lpDD->lpVtbl->EnumSurfaces = IDirectDraw4Hooked_EnumSurfaces;
		lpDD->lpVtbl->FlipToGDISurface = IDirectDraw4Hooked_FlipToGDISurface;
		lpDD->lpVtbl->GetCaps = IDirectDraw4Hooked_GetCaps;
		lpDD->lpVtbl->GetDisplayMode = IDirectDraw4Hooked_GetDisplayMode;
		lpDD->lpVtbl->GetFourCCCodes = IDirectDraw4Hooked_GetFourCCCodes;
		lpDD->lpVtbl->GetGDISurface = IDirectDraw4Hooked_GetGDISurface;
		lpDD->lpVtbl->GetMonitorFrequency = IDirectDraw4Hooked_GetMonitorFrequency;
		lpDD->lpVtbl->GetScanLine = IDirectDraw4Hooked_GetScanLine;
		lpDD->lpVtbl->GetVerticalBlankStatus = IDirectDraw4Hooked_GetVerticalBlankStatus;
		lpDD->lpVtbl->Initialize = IDirectDraw4Hooked_Initialize;
		lpDD->lpVtbl->RestoreDisplayMode = IDirectDraw4Hooked_RestoreDisplayMode;
		lpDD->lpVtbl->WaitForVerticalBlank = IDirectDraw4Hooked_WaitForVerticalBlank;
		lpDD->lpVtbl->GetAvailableVidMem = IDirectDraw4Hooked_GetAvailableVidMem;
		lpDD->lpVtbl->GetSurfaceFromDC = IDirectDraw4Hooked_GetSurfaceFromDC;
		lpDD->lpVtbl->RestoreAllSurfaces = IDirectDraw4Hooked_RestoreAllSurfaces;
		lpDD->lpVtbl->TestCooperativeLevel = IDirectDraw4Hooked_TestCooperativeLevel;
		lpDD->lpVtbl->GetDeviceIdentifier = IDirectDraw4Hooked_GetDeviceIdentifier;
	}
	return S_OK;
}

HRESULT IDirectDraw3Surface_setupHook(IDirectDrawSurface3 *surface)
{
	if (!surface)
	{
		CLogger::addEntry("Failed creating a new IDirectDrawSurface3!");
		return E_FAIL;
	}

	DDSURFACEDESC sDesc;

	sDesc.dwSize = sizeof(DDSURFACEDESC);

	surface->lpVtbl->GetSurfaceDesc(surface,&sDesc);

	CLogger::addEntry("Called 'IDirectDrawSurfaceHooked_QueryInterface' and created IDirectDrawSurface3 surface ("+intToStr(sDesc.dwWidth)+", "+intToStr(sDesc.dwHeight)+")");

	bool contains = false;

	contains |= (originalIDirectDrawSurface3Vtbls.find(surface) != originalIDirectDrawSurface3Vtbls.end());

	if (!contains)
	{	
		if (oldOne_IDDS3 == NULL)
		{
			memcpy(&originalIDirectDrawSurface3Vtbls[surface],surface->lpVtbl,sizeof(IDirectDrawSurface3Vtbl));
			oldOne_IDDS3 = surface;
		}
		else
		{
			memcpy(&originalIDirectDrawSurface3Vtbls[surface],&originalIDirectDrawSurface3Vtbls[oldOne_IDDS3],sizeof(IDirectDrawSurface3Vtbl));
			oldOne_IDDS3 = surface;
		}
	}

	contains |= abs((int)surface->lpVtbl->QueryInterface - (int)surface->lpVtbl->AddRef) > 0x900000f;

	if (!contains)
	{
		IDirectDrawSurface3Vtbl *tmp = new IDirectDrawSurface3Vtbl;

		memcpy(tmp,&originalIDirectDrawSurface3Vtbls[oldOne_IDDS3],sizeof(IDirectDrawSurface3Vtbl));

		surface->lpVtbl = tmp;					

		tmp->QueryInterface = IDirectDrawSurface3Hooked_QueryInterface;
		tmp->Flip = IDirectDrawSurface3Hooked_Flip;

		// auto generated
		tmp->AddAttachedSurface = IDirectDrawSurface3Hooked_AddAttachedSurface;
		tmp->AddOverlayDirtyRect = IDirectDrawSurface3Hooked_AddOverlayDirtyRect;
		tmp->Blt = IDirectDrawSurface3Hooked_Blt;
		tmp->BltBatch = IDirectDrawSurface3Hooked_BltBatch;
		tmp->BltFast = IDirectDrawSurface3Hooked_BltFast;
		tmp->DeleteAttachedSurface = IDirectDrawSurface3Hooked_DeleteAttachedSurface;
		tmp->EnumAttachedSurfaces = IDirectDrawSurface3Hooked_EnumAttachedSurfaces;
		tmp->EnumOverlayZOrders = IDirectDrawSurface3Hooked_EnumOverlayZOrders;
		tmp->GetAttachedSurface = IDirectDrawSurface3Hooked_GetAttachedSurface;
		tmp->GetBltStatus = IDirectDrawSurface3Hooked_GetBltStatus;
		tmp->GetCaps = IDirectDrawSurface3Hooked_GetCaps;
		tmp->GetClipper = IDirectDrawSurface3Hooked_GetClipper;
		tmp->GetColorKey = IDirectDrawSurface3Hooked_GetColorKey;
		tmp->GetDC = IDirectDrawSurface3Hooked_GetDC;
		tmp->GetFlipStatus = IDirectDrawSurface3Hooked_GetFlipStatus;
		tmp->GetOverlayPosition = IDirectDrawSurface3Hooked_GetOverlayPosition;
		tmp->GetPalette = IDirectDrawSurface3Hooked_GetPalette;
		tmp->GetPixelFormat = IDirectDrawSurface3Hooked_GetPixelFormat;
		tmp->GetSurfaceDesc = IDirectDrawSurface3Hooked_GetSurfaceDesc;
		tmp->Initialize = IDirectDrawSurface3Hooked_Initialize;
		tmp->IsLost = IDirectDrawSurface3Hooked_IsLost;
		tmp->Lock = IDirectDrawSurface3Hooked_Lock;
		tmp->ReleaseDC = IDirectDrawSurface3Hooked_ReleaseDC;
		tmp->Restore = IDirectDrawSurface3Hooked_Restore;
		tmp->SetClipper = IDirectDrawSurface3Hooked_SetClipper;
		tmp->SetColorKey = IDirectDrawSurface3Hooked_SetColorKey;
		tmp->SetOverlayPosition = IDirectDrawSurface3Hooked_SetOverlayPosition;
		tmp->SetPalette = IDirectDrawSurface3Hooked_SetPalette;
		tmp->Unlock = IDirectDrawSurface3Hooked_Unlock;
		tmp->UpdateOverlay = IDirectDrawSurface3Hooked_UpdateOverlay;
		tmp->UpdateOverlayDisplay = IDirectDrawSurface3Hooked_UpdateOverlayDisplay;
		tmp->UpdateOverlayZOrder = IDirectDrawSurface3Hooked_UpdateOverlayZOrder;
		tmp->GetDDInterface = IDirectDrawSurface3Hooked_GetDDInterface;
		tmp->PageLock = IDirectDrawSurface3Hooked_PageLock;
		tmp->PageUnlock = IDirectDrawSurface3Hooked_PageUnlock;
		tmp->SetSurfaceDesc = IDirectDrawSurface3Hooked_SetSurfaceDesc;
	}

	return S_OK;
}

HRESULT IDirectDraw4Surface_setupHook(IDirectDrawSurface4 *surface)
{
	if (!surface)
	{
		CLogger::addEntry("Failed creating a new IDirectDrawSurface4!");
		return E_FAIL;
	}

	DDSURFACEDESC2 sDesc2;

	sDesc2.dwSize = sizeof(DDSURFACEDESC2);

	surface->lpVtbl->GetSurfaceDesc(surface,&sDesc2);

	CLogger::addEntry("Created IDirectDrawSurface4 surface ("+intToStr(sDesc2.dwWidth)+", "+intToStr(sDesc2.dwHeight)+")");			

	bool contains = false;

	contains |= (originalIDirectDrawSurface4Vtbls.find(surface) != originalIDirectDrawSurface4Vtbls.end());

	if (!contains)
	{	
		if (oldOne_IDDS4 == NULL)
		{
			memcpy(&originalIDirectDrawSurface4Vtbls[surface],surface->lpVtbl,sizeof(IDirectDrawSurface4Vtbl));
			oldOne_IDDS4 = surface;
		}
		else
		{
			memcpy(&originalIDirectDrawSurface4Vtbls[surface],&originalIDirectDrawSurface4Vtbls[oldOne_IDDS4],sizeof(IDirectDrawSurface4Vtbl));
			oldOne_IDDS4 = surface;
		}
	}

	contains |= abs((int)surface->lpVtbl->QueryInterface - (int)surface->lpVtbl->AddRef) > 0x900000f;

	if (!contains)
	{
		IDirectDrawSurface4Vtbl *tmp = new IDirectDrawSurface4Vtbl;

		memcpy(tmp,&originalIDirectDrawSurface4Vtbls[oldOne_IDDS4],sizeof(IDirectDrawSurface4Vtbl));

		surface->lpVtbl = tmp;					

		tmp->QueryInterface = IDirectDrawSurface4Hooked_QueryInterface;
		tmp->Blt = IDirectDrawSurface4Hooked_Blt;
		tmp->SetClipper = IDirectDrawSurface4Hooked_SetClipper;
		tmp->Flip = IDirectDrawSurface4Hooked_Flip;
		tmp->AddAttachedSurface = IDirectDrawSurface4Hooked_AddAttachedSurface;

		// auto generated
		tmp->AddOverlayDirtyRect = IDirectDrawSurface4Hooked_AddOverlayDirtyRect;
		tmp->BltBatch = IDirectDrawSurface4Hooked_BltBatch;
		tmp->BltFast = IDirectDrawSurface4Hooked_BltFast;
		tmp->DeleteAttachedSurface = IDirectDrawSurface4Hooked_DeleteAttachedSurface;
		tmp->EnumAttachedSurfaces = IDirectDrawSurface4Hooked_EnumAttachedSurfaces;
		tmp->EnumOverlayZOrders = IDirectDrawSurface4Hooked_EnumOverlayZOrders;
		tmp->GetAttachedSurface = IDirectDrawSurface4Hooked_GetAttachedSurface;
		tmp->GetBltStatus = IDirectDrawSurface4Hooked_GetBltStatus;
		tmp->GetCaps = IDirectDrawSurface4Hooked_GetCaps;
		tmp->GetClipper = IDirectDrawSurface4Hooked_GetClipper;
		tmp->GetColorKey = IDirectDrawSurface4Hooked_GetColorKey;
		tmp->GetDC = IDirectDrawSurface4Hooked_GetDC;
		tmp->GetFlipStatus = IDirectDrawSurface4Hooked_GetFlipStatus;
		tmp->GetOverlayPosition = IDirectDrawSurface4Hooked_GetOverlayPosition;
		tmp->GetPalette = IDirectDrawSurface4Hooked_GetPalette;
		tmp->GetPixelFormat = IDirectDrawSurface4Hooked_GetPixelFormat;
		tmp->GetSurfaceDesc = IDirectDrawSurface4Hooked_GetSurfaceDesc;
		tmp->Initialize = IDirectDrawSurface4Hooked_Initialize;
		tmp->IsLost = IDirectDrawSurface4Hooked_IsLost;
		tmp->Lock = IDirectDrawSurface4Hooked_Lock;
		tmp->ReleaseDC = IDirectDrawSurface4Hooked_ReleaseDC;
		tmp->Restore = IDirectDrawSurface4Hooked_Restore;
		tmp->SetColorKey = IDirectDrawSurface4Hooked_SetColorKey;
		tmp->SetOverlayPosition = IDirectDrawSurface4Hooked_SetOverlayPosition;
		tmp->SetPalette = IDirectDrawSurface4Hooked_SetPalette;
		tmp->Unlock = IDirectDrawSurface4Hooked_Unlock;
		tmp->UpdateOverlay = IDirectDrawSurface4Hooked_UpdateOverlay;
		tmp->UpdateOverlayDisplay = IDirectDrawSurface4Hooked_UpdateOverlayDisplay;
		tmp->UpdateOverlayZOrder = IDirectDrawSurface4Hooked_UpdateOverlayZOrder;
		tmp->GetDDInterface = IDirectDrawSurface4Hooked_GetDDInterface;
		tmp->PageLock = IDirectDrawSurface4Hooked_PageLock;
		tmp->PageUnlock = IDirectDrawSurface4Hooked_PageUnlock;
		tmp->SetSurfaceDesc = IDirectDrawSurface4Hooked_SetSurfaceDesc;
		tmp->SetPrivateData = IDirectDrawSurface4Hooked_SetPrivateData;
		tmp->GetPrivateData = IDirectDrawSurface4Hooked_GetPrivateData;
		tmp->FreePrivateData = IDirectDrawSurface4Hooked_FreePrivateData;
		tmp->GetUniquenessValue = IDirectDrawSurface4Hooked_GetUniquenessValue;
		tmp->ChangeUniquenessValue = IDirectDrawSurface4Hooked_ChangeUniquenessValue;
	}

	return S_OK;
}

HRESULT IDirect3D2_setupHook(IDirect3D2 *lpiD3D, void **obp)
{
	if (!lpiD3D)
	{
		CLogger::addEntry("Failed creating a new IID_IDirect3D2");
		return E_FAIL;
	}

	CLogger::addEntry("Created IID_IDirect3D2 ("+intToStr((int)lpiD3D)+")");

	static bool IDirect3D2Hooked = false;
	if ( !IDirect3D2Hooked )
	{
		originalDirect3D2Vtbl = new IDirect3D2Vtbl;
		replacedDirect3D2Vtbl = new IDirect3D2Vtbl;

		memcpy(originalDirect3D2Vtbl,lpiD3D->lpVtbl,sizeof(IDirect3D2Vtbl));
		memcpy(replacedDirect3D2Vtbl,lpiD3D->lpVtbl,sizeof(IDirect3D2Vtbl));			

		IDirect3D3Vtbl *testVtbl = new IDirect3D3Vtbl;

		IDirect3D3 *lpiD3D3 = (IDirect3D3*)*obp;
		memcpy(testVtbl,lpiD3D3->lpVtbl,sizeof(IDirect3D3Vtbl));

		lpiD3D3->lpVtbl = testVtbl;

		lpiD3D->lpVtbl->QueryInterface = IDirect3DHooked_QueryInterface;
		lpiD3D->lpVtbl->CreateMaterial = IDirect3DHooked_CreateMaterial;
		lpiD3D->lpVtbl->CreateDevice = IDirect3DHooked_CreateDevice;
		lpiD3D->lpVtbl->CreateViewport = IDirect3DHooked_CreateViewport;

		IDirect3D2Hooked = true;
	}
	return S_OK;
}

HRESULT IDirectDrawClipper_setupHook(IDirectDrawClipper *lpDD)
{
	if (!lpDD)
	{
		CLogger::addEntry("Failed creating a new IDirectDrawClipper ("+intToStr((int)lpDD)+")");
		return E_FAIL;
	}

	CLogger::addEntry("Created a new IDirectDrawClipper ("+intToStr((int)lpDD)+")");

	bool IDirectDrawHooked = false;

	// test if we have already registered this interface
	IDirectDrawHooked |= (originalIDirectDrawClipperVtbls.find(lpDD) != originalIDirectDrawClipperVtbls.end());

	if (!IDirectDrawHooked)
	{
		if (oldOne_IDDC == NULL)
		{
			memcpy(&originalIDirectDrawClipperVtbls[lpDD],lpDD->lpVtbl,sizeof(IDirectDrawClipperVtbl));
			oldOne_IDDC = lpDD;
		}
		else
		{
			memcpy(&originalIDirectDrawClipperVtbls[lpDD],&originalIDirectDrawClipperVtbls[oldOne_IDDC],sizeof(IDirectDrawClipperVtbl));
			oldOne_IDDC = lpDD;
		}
	}

	// test if the interface address is different but its vtbl already contains our methods
	IDirectDrawHooked |= abs((int)lpDD->lpVtbl->QueryInterface - (int)lpDD->lpVtbl->AddRef) > 0x900000f;

	if ( !IDirectDrawHooked )
	{		
		CLogger::addEntry("IDirectDrawClipper HOOKED! ("+intToStr((int)lpDD)+")");
		lpDD->lpVtbl->QueryInterface = IDirectDrawClipperHooked_QueryInterface;
		lpDD->lpVtbl->GetClipList = IDirectDrawClipperHooked_GetClipList;
		lpDD->lpVtbl->GetHWnd = IDirectDrawClipperHooked_GetHWnd;
		lpDD->lpVtbl->Initialize = IDirectDrawClipperHooked_Initialize;
		lpDD->lpVtbl->IsClipListChanged = IDirectDrawClipperHooked_IsClipListChanged;
		lpDD->lpVtbl->SetClipList = IDirectDrawClipperHooked_SetClipList;
		lpDD->lpVtbl->SetHWnd = IDirectDrawClipperHooked_SetHWnd;
	}

	return S_OK;
}

#endif // SETUP_HOOKS_H