#include <windows.h>
#include "GameManager.h"

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd) 
{
    try {
        GameManager myFramework;
        myFramework.Initialize(hInstance);
        int ret = myFramework.GameLoop();
        myFramework.Release();
        return ret;
    } catch (const com_exception& e) {
        OutputDebugStringA(e.what());
        MessageBoxA(nullptr, e.what(), "Error", MB_OK);
    }

    return 0;
}