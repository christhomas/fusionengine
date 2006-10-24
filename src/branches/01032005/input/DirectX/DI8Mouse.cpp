#include <DI8InputDeviceDB.h>

/** DirectInput8 Mouse Constructor */
DI8Mouse::DI8Mouse(int DeviceID): InputMouse(DeviceID){}

/**	DirectInput8 Mouse Deconstructor
 *
 *	if the device is valid, Unaquire it (tell directinput to stop using it) and then release it
 */
DI8Mouse::~DI8Mouse()
{
	if(device != NULL){
		device->Unacquire();
		device->Release();
	}
}

/**	Initialise the Mouse
 *
 *	@param pd	A pointer to Fusions Platform data object
 *
 *	@returns boolean true or false depending on whether the device created successfully or not
 *
 *	Operation:
 *		-#	Create the mouse device
 *		-#	Set the Mouse cooperation level
 *		-#	Set the Mouse data format
 *		-#	Aquire the mouse
 */
bool DI8Mouse::Initialise(PlatformData *pd)
{
	Win32PlatformData *wpd = (Win32PlatformData *)pd;

	if(di8->CreateDevice(GUID_SysMouse,&device, NULL) == DI_OK )
	{
		// set keyboard co-operative level
		if( device->SetCooperativeLevel(wpd->m_hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE) == DI_OK )
		{
			// set keyboard data format (uses default)
			if( device->SetDataFormat(&c_dfDIMouse2) == DI_OK )
			{
				// "acquire" the keyboard
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
 *		-#	Calls InputMouse::ReadDevice() to handle the platform independant stuff
 */
bool DI8Mouse::ReadDevice(void)
{
	if(device->GetDeviceState( sizeof(DIMOUSESTATE2), &State ) != DI_OK)	return false;

	return InputMouse::ReadDevice();
}

/**	Checks a mouse button
 *
 *	@param button	The button being checked
 *
 *	@returns boolean true or false, depending on whether the button was pressed or not
 */
bool DI8Mouse::CheckButton(int button)
{
	return ( State.rgbButtons[button] ? true : false );
}

/**	Checks the mouse axes
 *
 *	@param axis	The axis being checked
 *
 *	@returns	The value of the axis being checked or 0 if the axis wasnt valid
 */
long DI8Mouse::CheckAxis(int axis)
{
	if(axis == 1)	return State.lX;
	if(axis == 2)	return State.lY;
	if(axis == 3)	return State.lZ;

	return 0;
}
