
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LIB3DS_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LIB3DS_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef LIB3DS_EXPORTS
#define LIB3DS_API extern "C" __declspec(dllexport)
#else
#define LIB3DS_API extern "C" __declspec(dllimport)
#endif

#include <VirtualFS/VFSPlugin_3DS.h>

LIB3DS_API VFSPlugin * CreatePlugin(Fusion *f);
