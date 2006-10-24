// libVFS.cpp : Defines the entry point for the DLL application.
//

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "libVFS.h"
#include <Fusion.h>

static	int		refcount	=	0;
static	bool	first			=	true;
Fusion	*fusion					=	NULL;

BOOL APIENTRY DllMain(HANDLE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
	return true;
}

LIBVFS_API void GetInstance(Fusion &f)
{
	fusion = &f;

	if(refcount++ == 0 && first == true){
		CreateVFS(f);

		first = false;
	}
}

LIBVFS_API void DestroyInstance(void)
{
	if((--refcount) == 0)	DestroyVFS();
}
