// libDI8.cpp : Defines the entry point for the DLL application.
//

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Fusion.h>
#include "libDI8.h"

static	int		refcount	=	0;
static	bool	first			=	true;
Fusion	*fusion					=	NULL;

BOOL APIENTRY DllMain(HANDLE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
	return true;
}

//===============================
//	Creates the DInputDeviceDB object
//===============================
LIBDI8_API void GetInstance(Fusion &f)
{
	fusion = &f;

	if(refcount++ == 0 && first == true)
	{
		f.Input = new DI8InputDeviceDB();

		if(f.Input->Initialise() == false) delete f.Input;
		first = false;
	}
}

//===============================
//	Destroys the DInputDeviceDB object
//===============================
LIBDI8_API void DestroyInstance(void)
{
	if((--refcount) == 0){
		delete fusion->Input;

		fusion->Input = NULL;
	}
}
