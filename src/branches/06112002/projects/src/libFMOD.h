
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LIBSOUND_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LIBSOUND_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef LIBFMOD_EXPORTS
#define LIBFMOD_API extern "C" __declspec(dllexport)
#else
#define LIBFMOD_API	extern "C" __declspec(dllimport)
#endif

#include <Sound/FMODSound.h>

LIBFMOD_API void	GetInstance			(Fusion &f);
LIBFMOD_API void	DestroyInstance	(void);