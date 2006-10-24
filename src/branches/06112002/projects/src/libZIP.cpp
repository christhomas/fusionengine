// libZIP.cpp : Defines the entry point for the DLL application.
//

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "libZIP.h"

Fusion *fusion;

BOOL APIENTRY DllMain(HANDLE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
	return true;
}

LIBZIP_API VFSTransport * CreateTransport(Fusion *f)
{
	static int count = 0;

	fusion = f;

	if(count == 0){
		count++;
		return CreateZIP(fusion);
	}

	return NULL;
}
