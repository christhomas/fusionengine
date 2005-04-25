#include <Fusion.h>
#include <FusionPlugin.h>
 
Fusion *fusion;

#ifdef _WIN32
	#include <win32/WGLGraphics.h>
#else
	#include <unix/GLXGraphics.h>	
#endif

FUSIONPLUGIN void GetInstance(Fusion &f)
{
	fusion = &f;
	
	if(fusion->Graphics == NULL){
#ifndef _WIN32		
		fusion->Graphics		=	new GLXGraphics();
#else
		fusion->Graphics		=	new WGLGraphics();
#endif

		fusion->Mesh		=	new MeshDB();
		fusion->Scene		=	new SceneGraphDB();
		fusion->Interface	=	new UserInterfaceDB();
		fusion->Font		=	new FontDB();

		if(fusion->Graphics->Initialise()		==	false) delete fusion->Graphics;
		if(fusion->Mesh->Initialise()		==	false) delete fusion->Mesh;
		if(fusion->Scene->Initialise()		==	false) delete fusion->Scene;
		if(fusion->Interface->Initialise()		==	false) delete fusion->Interface;
		if(fusion->Font->Initialise()		==	false) delete fusion->Font;
	}
}

 FUSIONPLUGIN void DestroyInstance(void)
{
	if(fusion->Graphics != NULL){
		delete fusion->Font;
		delete fusion->Interface;
		delete fusion->Scene;
		delete fusion->Mesh;
		delete fusion->Graphics;

		fusion->Font		=	NULL;
		fusion->Interface	=	NULL;
		fusion->Scene		=	NULL;
		fusion->Mesh		=	NULL;
		fusion->Graphics	=	NULL;
	}
}