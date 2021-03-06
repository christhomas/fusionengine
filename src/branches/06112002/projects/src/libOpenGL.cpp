// libOpenGL.cpp : Defines the entry point for the DLL application.
//

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "libOpenGL.h"

static	int		refcount	=	0;
static	bool	first			=	true;
Fusion	*fusion					=	NULL;

BOOL APIENTRY DllMain(HANDLE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
	return true;
}

LIBOPENGL_API void GetInstance(Fusion &f)
{
	fusion = &f;

	if(refcount++ == 0 && first == true){
		f.Graphics	=	new WGLGraphics();
		f.Mesh			=	new MeshDB();
		f.Scene			=	new SceneGraphDB();
		f.Interface	=	new UserInterfaceDB();
		f.Font			=	new FontDB();

		if(f.Graphics->Initialise()		==	false) delete f.Graphics;
		if(f.Mesh->Initialise()				==	false) delete f.Mesh;
		if(f.Scene->Initialise()			==	false) delete f.Scene;
		if(f.Interface->Initialise()	==	false) delete f.Interface;
		if(f.Font->Initialise()				==	false) delete f.Font;

		first = false;
	}
}

LIBOPENGL_API void DestroyInstance(void)
{
	if((--refcount) == 0){
		delete fusion->Font;
		delete fusion->Interface;
		delete fusion->Scene;
		delete fusion->Mesh;
		delete fusion->Graphics;

		fusion->Font			=	NULL;
		fusion->Interface	=	NULL;
		fusion->Scene			=	NULL;
		fusion->Mesh			=	NULL;
		fusion->Graphics	=	NULL;
	}
}