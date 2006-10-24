// libQuake3.cpp : Defines the entry point for the DLL application.
//

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "libQuake3.h"

Fusion *fusion;

BOOL APIENTRY DllMain(HANDLE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
	return TRUE;
}

LIBQUAKE3_API VFSPlugin * CreatePlugin(Fusion *f)
{
	static int count = 0;

	fusion = f;

	VFSPlugin *p = NULL;

	switch(count){
		case 0:{
			p = new VFSPlugin_Q3BSP();
		}break;

		case 1:{
			p = new VFSPlugin_TGA();
		}break;

		case 2:{
			p = new VFSPlugin_JPEG();
		}break;
	};

	count++;

	return p;
}
