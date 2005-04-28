#include <Fusion.h>
#include <FusionPlugin.h>
#include <FMODSound.h>

Fusion *fusion;

//====================================
//	Creates the DInputDeviceDB object
//====================================
FUSIONPLUGIN void GetInstance(Fusion &f)
{
	fusion = &f;

	if(fusion->Sound == NULL){
		fusion->Sound = new FMODSound();

		fusion->Sound->Initialise();
	}
}

//====================================
//	Destroys the DInputDeviceDB object
//====================================
FUSIONPLUGIN void DestroyInstance(void)
{
	if(fusion->Sound != NULL){
		delete fusion->Sound;
		fusion->Sound = NULL;
	}
}
