// libNetwork.cpp : Defines the entry point for the DLL application.
//

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Fusion.h>
#include "libNetwork.h"

static	int		refcount	=	0;
static	bool	first			=	true;
Fusion	*fusion					=	NULL;

BOOL APIENTRY DllMain(HANDLE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
	return true;
}

LIBNETWORK_API void GetInstance(Fusion &f)
{
	fusion = &f;

	if(refcount++ == 0 && first == true){
		f.Network = new NetworkCore();

		if(f.Network->Initialise() == false) delete f.Network;
		first = false;
	}
}

LIBNETWORK_API void DestroyInstance(void)
{
	if((--refcount) == 0){
		delete fusion->Network;

		fusion->Network = NULL;
	}
}