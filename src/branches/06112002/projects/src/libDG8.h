
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LIBDG8_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LIBDG8_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef LIBDG8_EXPORTS
#define LIBDG8_API extern "C" __declspec(dllexport)
#else
#define LIBDG8_API extern "C" __declspec(dllimport)
#endif


#include <Win32/DirectX/DG8Graphics.h>
#include <Mesh/MeshDB.h>
#include <Graphics/SceneGraph.h>
#include <GUI/gui.h>
#include <Font/IFont.h>

LIBDG8_API void	GetInstance			(Fusion &f);
LIBDG8_API void	DestroyInstance	(void);

