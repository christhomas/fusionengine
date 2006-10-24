
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LIBDI8_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LIBDI8_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef LIBDI8_EXPORTS
#define LIBDI8_API extern "C" __declspec(dllexport)
#else
#define LIBDI8_API extern "C" __declspec(dllimport)
#endif

#include <Win32/Directx/DI8InputDeviceDB.h>

LIBDI8_API void	GetInstance			(Fusion &f);
LIBDI8_API void	DestroyInstance	(void);
