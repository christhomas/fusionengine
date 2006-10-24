#include <Win32NetworkCore.h>
#include <PTNetworkCore.h>
#include <Fusion.h>
#include <FusionPlugin.h>

Fusion *fusion;

FUSIONPLUGIN void DestroyInstance(void);

//====================================
//	Creates the Network object
//====================================
FUSIONPLUGIN void GetInstance(Fusion &f)
{
	fusion = &f;

	if(fusion->Network == NULL){
		fusion->Network = new PTNetworkCore();

		if(fusion->Network->Initialise() == false){
			DestroyInstance();
		}
	}
}

//====================================
//	Destroys the Network object
//====================================
FUSIONPLUGIN void DestroyInstance(void)
{
	if(fusion->Network != NULL){
		delete fusion->Network;
		fusion->Network = NULL;
	}
}
