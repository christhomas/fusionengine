#include <fusion.h>
#include <fusionplugin.h>

/**	Method to create the VirtualFS object
 *
 *	This function is required for the application to create a VFS object from a library
 */
FUSIONPLUGIN void GetInstance(Fusion &f)
{
	if(f.vfs == NULL){
		CreateVFS(f);

		if(f.vfs->Initialise() == false) DestroyVFS();
	}
}

FUSIONPLUGIN void DestroyInstance(void)
{
	DestroyVFS();
}