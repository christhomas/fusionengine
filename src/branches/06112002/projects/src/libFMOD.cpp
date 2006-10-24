// libFMOD.cpp : Defines the entry point for the DLL application.
//

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "libFMOD.h"

static	int		refcount	=	0;
static	bool	first			=	true;
Fusion	*fusion					=	NULL;

BOOL APIENTRY DllMain(HANDLE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
	return true;
}

LIBFMOD_API void GetInstance(Fusion &f)
{
	fusion = &f;

	if(refcount++ == 0 && first == true){
		f.Sound = new FMODSound();
		
		//	PLEASE NOTE:
		//	You attempt to initialise the sound object here, 
		//	if you want it attached to your window, you'll 
		//	have to call Sound::Initialise(Fusion *f) later
		//	in your app cause it'll fail here, but when you initialise
		//	your window, you can safetly initialise the sound object and
		//	have it attached to your window (as it is, you're sound object IS NOT
		//	attached to the window, whatever benefit that gives you anyway)
		//
		//	Therefore since I know this function will fail, except for setting the sound buffer format
		//	I wont delete the object upon failure.

		f.Sound->Initialise();

		first = false;
	}
}

LIBFMOD_API void DestroyInstance(void)
{
	if((--refcount) == 0){
		delete fusion->Sound;

		fusion->Sound = NULL;
	}
}