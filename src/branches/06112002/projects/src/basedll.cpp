// dll.cpp : Defines the entry point for the DLL application.
//

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "basedll.h"

Fusion *fusion;

BOOL APIENTRY DllMain(HANDLE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
	return true;
}
