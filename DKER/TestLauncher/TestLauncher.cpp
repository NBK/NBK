#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "..\testdll\testdll.h"

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
    InstallHook();

    MessageBox( NULL, "Hook installed.  Press OK to remove and exit.", "DKER HOOK", MB_OK );

    RemoveHook();
    return 0;
}
