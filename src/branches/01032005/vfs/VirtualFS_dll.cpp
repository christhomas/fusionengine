#include <fusion.h>
#include <fusionplugin.h>

FUSIONPLUGIN void DestroyInstance(void);

/**	Method to create the VirtualFS object
 *
 *	This function is required for the application to create a VFS object from a library
 */
FUSIONPLUGIN void GetInstance(Fusion &f)
{
	fusion = &f;
	
	if(fusion->vfs == NULL){
		CreateVFS(f);

		if(fusion->vfs->Initialise() == false) DestroyInstance();
	}
}

FUSIONPLUGIN void DestroyInstance(void)
{
	DestroyVFS();
}