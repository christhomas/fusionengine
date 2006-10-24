
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LIBOPENGL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LIBOPENGL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef LIBOPENGL_EXPORTS
#define LIBOPENGL_API extern "C" __declspec(dllexport)
#else
#define LIBOPENGL_API extern "C" __declspec(dllimport)
#endif

#include <Win32/OpenGL/WGLGraphics.h>
#include <Mesh/MeshDB.h>
#include <Graphics/SceneGraph.h>
#include <GUI/gui.h>
#include <Font/IFont.h>

LIBOPENGL_API void	GetInstance			(Fusion &f);
LIBOPENGL_API void	DestroyInstance	(void);
