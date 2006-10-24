#include <Win32/Directx/DI8InputDeviceDB.h>
#include <Fusion.h>

IDirectInput8	*di8 = NULL;

/**	DirectInput8 Database Constructor */
DI8InputDeviceDB::DI8InputDeviceDB()
{
	m_platform	= (Win32PlatformData *)fusion->GetPlatformData();
	di8					=	NULL;
}

/**	DirectInput8 Database Deconstructor
 *
 *	Release the directinput8 object
 */
DI8InputDeviceDB::~DI8InputDeviceDB()
{
	if(di8!=NULL) di8->Release();
}

/**	Create the DirectInput object
 *
 *	@returns boolean true or false depending on whether the object created successfully
 */
bool DI8InputDeviceDB::CreateDirectInput(void)
{
	if(di8 == NULL){
		// create the direct input object
		if( DirectInput8Create(m_platform->m_hinst, DIRECTINPUT_VERSION,IID_IDirectInput8, (void **)&di8, NULL) != DI_OK ){
			return false;
		}
	}

	return true;
}

/**	Add a DirectInput Device to the database
 *
 *	@param type	The type of device to add
 *
 *	@returns	An IInputDevice object or NULL if the object was not recognised or failed to create
 */
IInputDevice * DI8InputDeviceDB::AddDevice(IInputDevice::DeviceType type)
{
	IInputDevice *temp = NULL;

	if(CreateDirectInput() == true){

		switch(type){
			case IInputDevice::MOUSE:{		temp = new DI8Mouse(m_num_mouse++);				}break;
			case IInputDevice::KEYBOARD:{	temp = new DI8Keyboard(m_num_keyboard++);	}break;
			case IInputDevice::JOYSTICK:{	temp = new DI8Joystick(m_num_joystick++);	}break;
		};
		
		temp->Initialise(m_platform);

		StoreDevice(temp);
	}

	return temp;
}
