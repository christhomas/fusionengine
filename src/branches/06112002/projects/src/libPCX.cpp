// libPCX.cpp : Defines the entry point for the DLL application.
//

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "libPCX.h"

Fusion *fusion;

BOOL APIENTRY DllMain(HANDLE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
	return true;
}

LIBPCX_API VFSPlugin * CreatePlugin(Fusion *f)
{
	static int count = 0;

	fusion = f;
	
	if(count == 0){
		count++;
		return new VFSPlugin_PCX();
	}

	return NULL;
}
