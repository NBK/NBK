#include <stdio.h>
#include <windows.h>

#include <d3d.h>

#pragma comment(lib, "d3d8.lib") 

typedef HRESULT (WINAPI *DirectDrawCreate_Type)(GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter);

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID reserved)
{
	//Crea
	return TRUE;
}