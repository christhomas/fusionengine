
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LIBQUAKE3_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LIBBSP_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef LIBQUAKE3_EXPORTS
#define LIBQUAKE3_API extern "C" __declspec(dllexport)
#else
#define LIBQUAKE3_API extern "C" __declspec(dllimport)
#endif

#include <VirtualFS/VFSPlugin_Q3BSP.h>
#include <VirtualFS/VFSPlugin_TGA.h>
#include <VirtualFS/VFSPlugin_JPEG.h>

LIBQUAKE3_API VFSPlugin * CreatePlugin(Fusion *f);
