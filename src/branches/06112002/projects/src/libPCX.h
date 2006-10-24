
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LIBPCX_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LIBPCX_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef LIBPCX_EXPORTS
#define LIBPCX_API extern "C" __declspec(dllexport)
#else
#define LIBPCX_API extern "C" __declspec(dllimport)
#endif

#include <VirtualFS/VFSPlugin_PCX.h>

LIBPCX_API VFSPlugin * CreatePlugin(Fusion *f);
