#include "stdafx.h"
#pragma comment(lib, "..\\Debug\\ReplaceDll.lib")
#include "..//ReplaceDll/ReplaceDll.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		Replace("string", "hello");
		break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    default: ;
    }
    return TRUE;
}