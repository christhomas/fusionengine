#include <Fusion.h>
#include <FusionPlugin.h>
#include <DI8InputDeviceDB.h>

Fusion *fusion;

FUSIONPLUGIN void DestroyInstance(void);

//====================================
//	Creates the DInputDeviceDB object
//====================================
FUSIONPLUGIN void GetInstance(Fusion &f)
{
	fusion = &f;

	if(fusion->Input == NULL){
		fusion->Input = new DI8InputDeviceDB();

		if(fusion->Input->Initialise() == false){
			DestroyInstance();
		}
	}
}

//====================================
//	Destroys the DInputDeviceDB object
//====================================
FUSIONPLUGIN void DestroyInstance(void)
{
	if(fusion->Input != NULL){
		delete fusion->Input;
		fusion->Input = NULL;
	}
}
