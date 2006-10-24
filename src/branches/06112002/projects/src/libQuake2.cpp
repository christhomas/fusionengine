// libQuake2.cpp : Defines the entry point for the DLL application.
//

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "libQuake2.h"

Fusion *fusion;

BOOL APIENTRY DllMain(HANDLE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
	return TRUE;
}

LIBQUAKE2_API VFSPlugin * CreatePlugin(Fusion *f)
{
	static int count = 0;

	fusion = f;

	VFSPlugin *p = NULL;

	switch(count){
		//	Quake2 Plugins
		case 0:{
			p = new VFSPlugin_Q2BSP();
		}break;

		case 1:{
			p = new VFSPlugin_Q2WAL();
		}break;

		case 2:{
			p = new VFSPlugin_PCX();
		}break;
	};

	count++;

	return p;
}
