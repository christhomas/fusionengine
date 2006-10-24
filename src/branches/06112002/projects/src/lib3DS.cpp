// lib3DS.cpp : Defines the entry point for the DLL application.
//

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "lib3DS.h"

Fusion *fusion;

BOOL APIENTRY DllMain(HANDLE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
	return TRUE;
}

/**	Creates the 3DS Plugin
 *	@ingroup	VFSPlugin_3DS_Group
 *
 *	@param	f	The Fusion object, if this plugin needs to call fusion for something
 *
 *	@returns	A VFSPlugin object for this plugin type
 *
 *	The plugin function is simple, each time called
 *	create the appropiate plugin, increment the counter
 *	so next time it's called, it'll create the NEXT plugin
 *	then when you have no more plugins to create, return NULL
 */
LIB3DS_API VFSPlugin * CreatePlugin(Fusion *f)
{
	static int count = 0;

	fusion = f;

	if(count == 0){
		count++;
		return new VFSPlugin_3DS();
	}

	return NULL;
}
