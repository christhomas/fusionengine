#include <DI8InputDeviceDB.h>

DI8Joystick *joystick;

/**	Enumerate the joysticks attached
 *	@ingroup	DI8_Input_Group
 *
 *	@param instance	Unknown
 *	@param Context	Unknown
 *
 *	@returns boolean true or false, depending on whether the device created successfully or not
 */
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *instance, void *Context)
{
	if(di8->CreateDevice(instance->guidInstance, &joystick->device, NULL ) == DI_OK) return true;

	return false;
}

/**	DirectInput8 Joystick Constructor */
DI8Joystick::DI8Joystick(int DeviceID): InputJoystick(DeviceID){}

/**	DirectInput8 Joystick Deconstructor
 *
 *	if the device is valid, Unaquire it (tell directinput to stop using it) and then release it
 */
DI8Joystick::~DI8Joystick()
{
	if(device != NULL){
		device->Unacquire();
		device->Release();
	}
}

/**	Initialise the Joystick
 *
 *	@param pd	A pointer to Fusions Platform data object
 *
 *	@returns boolean true or false depending on whether the device created successfully or not
 *
 *	Operation:
 *		-#	Enumerate the devices
 *		-#	Set the joysticks cooperation level
 *		-#	Set the joysticks data format
 *		-#	Aquire the joystick
 */
bool DI8Joystick::Initialise(PlatformData *pd)
{
	Win32PlatformData *wpd = (Win32PlatformData *)pd;
	joystick = this;

	if(di8->EnumDevices(DI8DEVCLASS_GAMECTRL,EnumJoysticksCallback,NULL, DIEDFL_ATTACHEDONLY ) == DI_OK )
	{
		// set joystick co-operative level
		if( device->SetCooperativeLevel(wpd->m_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE) == DI_OK )
		{
			// set joystick data format (uses default)
			if( device->SetDataFormat(&c_dfDIJoystick2) == DI_OK )
			{
				// "acquire" the joystick
				if( device->Acquire() == DI_OK )
				{
					return true;
				}
			}
		}
	}

	device = NULL;
	return false;
}

/**	Reads the device
 *
 *	@returns boolean true or false, depending on whether the device read successfully
 *
 *	Operation:
 *		-#	Retrieves the state of the device
 *		-#	Calls InputJoystick::ReadDevice() to handle the platform independant stuff
 */
bool DI8Joystick::ReadDevice(void)
{
	if(device->GetDeviceState( sizeof(DIJOYSTATE2), &State ) != DI_OK)	return false;

	return InputJoystick::ReadDevice();
}

/**	Checks a joystick button
 *
 *	@param button	The button being checked
 *
 *	@returns boolean true or false, depending on whether the button was pressed or not
 */
bool DI8Joystick::CheckButton(int button)
{
	return ( State.rgbButtons[button] ? true : false );
}

/**	Checks the joysticks axes
 *
 *	@param axis	The axis being checked
 *
 *	@returns	The value of the axis being checked or 0 if the axis wasnt valid
 */
long DI8Joystick::CheckAxis(int axis)
{
	if(axis == 1)	return State.lX;
	if(axis == 2)	return State.lY;
	if(axis == 3)	return State.lZ;

	return 0;
}
