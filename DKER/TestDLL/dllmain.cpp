//#define WIN32_LEAN_AND_MEAN
#define CINTERFACE
#include <windows.h>
#include <shlwapi.h>
#include <ddraw.h>
#include "testdll.h"
#include "..\apihijack.h"
#include <stdio.h>
#include <ddraw.h>
//#include <ddrawex.h>
#include <d3d.h>
#include <string>
#include <map>
#include <sstream>
#include "Logger.h"

#include "commons.h"
#include "setup.h"
#include "IDirect3DDevice2Hooked.h"
#include "IDirect3DViewport2Hooked.h"
#include "IDirectDrawSurface3Hooked.h"
#include "IDirectDrawSurfaceHooked.h"
#include "IDirectDrawSurface4Hooked.h"
#include "IDirect3DHooked.h"
#include "IDirectDraw4Hooked.h"
#include "IDirectDrawHooked.h"
#include "IDirectDrawClipper.h"

char			work[256];
HINSTANCE		hDLL;

// Function pointer types.
typedef HRESULT (WINAPI *DirectDrawCreate_Type)(GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter);
typedef HWND    (WINAPI *CreateWindowExA_Type)(	DWORD dwExStyle,
												LPCTSTR lpClassName,
												LPCTSTR lpWindowName,
												DWORD dwStyle,
												int x,
												int y,
												int nWidth,
												int nHeight,
												HWND hWndParent,
												HMENU hMenu,
												HINSTANCE hInstance,
												LPVOID lpParam);
typedef BOOL (WINAPI *ShowWindow_Type)(HWND hWnd, int nCmdShow);
typedef HMODULE (WINAPI *LoadLibraryA_Type)(LPCTSTR lpFileName);
typedef FARPROC (WINAPI *GetProcAddressType)(HMODULE hModule, LPCSTR lpProcName);


// Function prototypes.
HRESULT WINAPI MyDirectDrawCreate(GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter);
HWND    WINAPI MyCreateWindowExA(	DWORD dwExStyle,
								LPCTSTR lpClassName,
								LPCTSTR lpWindowName,
								DWORD dwStyle,
								int x,
								int y,
								int nWidth,
								int nHeight,
								HWND hWndParent,
								HMENU hMenu,
								HINSTANCE hInstance,
								LPVOID lpParam);
BOOL WINAPI MyShowWindow(HWND hWnd, int nCmdShow);
HMODULE WINAPI MyLoadLibraryA(LPCTSTR lpFileName);
FARPROC WINAPI MyGetProcAddress(HMODULE hModule, LPCSTR lpProcName);

// Hook structure.
enum
{
	D3DFN_DirectDrawCreate
};

SDLLHook D3DHook_DDRAW = 
{
    "DDRAW.DLL",
    false, NULL,
    {
		{ "DirectDrawCreate", MyDirectDrawCreate },
        { NULL, NULL }
    }
};

SDLLHook D3DHook_USER32 = 
{
	"User32.dll",
		false, NULL,
	{
		{ "CreateWindowExA", MyCreateWindowExA},
		{ "CreateWindowEx", MyCreateWindowExA},
		{ "CreateWindow", MyCreateWindowExA},
		{ "ShowWindow", MyShowWindow},
		{ NULL, NULL }
	}
};

SDLLHook D3DHook_KERNEL32 = 
{
	"Kernel32.dll",
		false, NULL,
	{
		//{ "LoadLibraryA", MyLoadLibraryA},
		{ "GetProcAddress", MyGetProcAddress},
		{ NULL, NULL }
	}
};

/****************** D3DHook_DDRAW ******************/

HRESULT WINAPI MyDirectDrawCreate(GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter)
{
	CLogger::addEntry("Calling MyDirectDrawCreate, GUID = "+(lpGUID?uintToStr(lpGUID->Data1):"NULL"));

	DirectDrawCreate_Type OldFn = (DirectDrawCreate_Type)D3DHook_DDRAW.Functions[D3DFN_DirectDrawCreate].OrigFn;

	HRESULT fnResult = OldFn(lpGUID,lplpDD,pUnkOuter);

	if (fnResult == S_OK && lplpDD)
	{
		IDirectDraw_setupHook((IDirectDraw*)*lplpDD);		
	}

	return fnResult;
}


/****************** D3DHook_DDRAW ******************/

BOOL WINAPI MyShowWindow(HWND hWnd, int nCmdShow)
{
	//DebugBreak();
	ShowWindow_Type OldFn = (ShowWindow_Type)D3DHook_USER32.Functions[3].OrigFn;		

	//long r0 = SetWindowLong(hWnd,GWL_STYLE,WS_OVERLAPPEDWINDOW);
	long r0 = SetWindowLong(hWnd,GWL_EXSTYLE,WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);
	long r1 = SetWindowLong(hWnd,GWL_STYLE,WS_OVERLAPPEDWINDOW);
	//long r2 = SetWindowPos(hWnd,HWND_BOTTOM,0,0,800,600,SWP_SHOWWINDOW);

	BOOL res = OldFn(hWnd,nCmdShow);

	return res;
}


/****************** D3DHook_USER32 ******************/

HWND WINAPI MyCreateWindowExA(	DWORD dwExStyle,
							  LPCTSTR lpClassName,
							  LPCTSTR lpWindowName,
							  DWORD dwStyle,
							  int x,
							  int y,
							  int nWidth,
							  int nHeight,
							  HWND hWndParent,
							  HMENU hMenu,
							  HINSTANCE hInstance,
							  LPVOID lpParam
							  )
{
	DebugBreak();
	CreateWindowExA_Type OldFn = (CreateWindowExA_Type)D3DHook_USER32.Functions[0].OrigFn;	

	return OldFn(dwExStyle,lpClassName,lpWindowName,dwStyle,x,y,nWidth,nHeight,hWndParent,hMenu,hInstance,lpParam);
}


/****************** D3DHook_KERNEL32 ******************/

HMODULE WINAPI MyLoadLibraryA(LPCTSTR lpFileName)
{
	DebugBreak();
	//MessageBox(NULL,lpFileName,"",MB_OK);

	LoadLibraryA_Type OldFn = (LoadLibraryA_Type)D3DHook_KERNEL32.Functions[0].OrigFn;		

	HMODULE res = OldFn(lpFileName);

	return res;
}

FARPROC WINAPI MyGetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
	//DebugBreak();

	//MessageBox(NULL,lpProcName,"",MB_OK);

	CLogger::addEntry("GetProcAddress: "+std::string(lpProcName));

	std::string procName = std::string(lpProcName);

	bool contains = false;

	contains |= (hookProcNewAddr.find(procName) != hookProcNewAddr.end());

	if (!contains)
	{
		// we do not want to hook this proccess
		return GetProcAddress(hModule,lpProcName);
	}

	// do we already have the priginal process address?
	contains &= (originalProcAddresses.find(procName) != originalProcAddresses.end());

	if (!contains)
	{
		// if not, store it
		FARPROC fp = GetProcAddress(hModule,lpProcName);
		originalProcAddresses[procName] = fp;
	}	
	
	// and return our rerouted function    
	return hookProcNewAddr[procName];
}


// other hooked functions

typedef BOOL (WINAPI* EnumDisplayDevicesA_Type)(PVOID,DWORD,PVOID,DWORD);

BOOL WINAPI MyEnumDisplayDevicesA(PVOID p0, DWORD p1, PVOID p2, DWORD p3)
{
	//DebugBreak();

	EnumDisplayDevicesA_Type oldFunc = (EnumDisplayDevicesA_Type)originalProcAddresses["EnumDisplayDevicesA"];

	BOOL edd = oldFunc(p0,p1,p2,p3);

	return edd;
}


/****************** CBT Hook-style injection ******************/

BOOL APIENTRY DllMain( HINSTANCE hModule, DWORD fdwReason, LPVOID lpReserved )
{
    if ( fdwReason == DLL_PROCESS_ATTACH )  // When initializing....
    {
        hDLL = hModule;

        // We don't need thread notifications for what we're doing.  Thus, get
        // rid of them, thereby eliminating some of the overhead of this DLL
        DisableThreadLibraryCalls( hModule );

        // Only hook the APIs if this is the Everquest proess.
        GetModuleFileName( GetModuleHandle( NULL ), work, sizeof(work) );
        PathStripPath( work );

        OutputDebugString( "TESTDLL checking process: " );
        OutputDebugString( work );
        OutputDebugString( "\n" );

		//GetProcAddress(

        //if ( stricmp( work, "bend.exe" ) == 0 )
		//if ( stricmp( work, "particle.exe" ) == 0 )
		//if ( stricmp( work, "particle.exe" ) == 0 )
		if (strstr(work,"keepd3d") != NULL)
		{
			MessageBeep(MB_ICONWARNING);

			CLogger::init("hook.log");
            if (HookAPICalls(&D3DHook_DDRAW))
			{				
				// now the ddraw.dll should be loaded, get the hmodule
				HMODULE ddrw_mod = GetModuleHandle("ddraw.dll");

				if (!ddrw_mod)
				{
					// load it manualy if by some chance it didn't get loaded
					ddrw_mod = LoadLibraryA("ddraw.dll");
				}

				// insert redirections			
				hookProcNewAddr.clear();
				//hookProcNewAddr["EnumDisplayDevicesA"] = (FARPROC)&MyEnumDisplayDevicesA;

				// now hook its kernel32 dependancy
				HookAPICalls(&D3DHook_KERNEL32,&ddrw_mod);
			}
		}
    }

    return TRUE;
}

// This segment must be defined as SHARED in the .DEF
#pragma data_seg (".HookSection")		
// Shared instance for all processes.
HHOOK hHook = NULL;	
#pragma data_seg ()

TESTDLL_API LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam) 
{
    return CallNextHookEx( hHook, nCode, wParam, lParam); 
}

TESTDLL_API void InstallHook()
{
    OutputDebugString( "TESTDLL hook installed.\n" );
    hHook = SetWindowsHookEx( WH_CBT, HookProc, hDLL, 0 ); 
}

TESTDLL_API void RemoveHook()
{
    OutputDebugString( "TESTDLL hook removed.\n" );
    UnhookWindowsHookEx( hHook );
}
