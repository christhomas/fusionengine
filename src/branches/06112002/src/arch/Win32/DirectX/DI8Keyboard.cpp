// DI8Keyboard.cpp: implementation of the DI8Keyboard class.
//
//////////////////////////////////////////////////////////////////////

#include <Win32/DirectX/DI8InputDeviceDB.h>
#include <Win32/DirectX/InputMapping.h>

/**	DirectInput8 Keyboard Constructor */
DI8Keyboard::DI8Keyboard(int DeviceID): InputKeyboard(DeviceID){}

/**	DirectInput8 keyboard Deconstructor
 *
 *	if the device is valid, Unaquire it (tell directinput to stop using it) and then release it
 */
DI8Keyboard::~DI8Keyboard()
{
	if(device != NULL){
		device->Unacquire();
		device->Release();
	}
}

/**	Initialise the keyboard
 *
 *	@param pd	A pointer to Fusions Platform data object
 *
 *	@returns boolean true or false depending on whether the device created successfully or not
 *
 *	Operation:
 *		-#	Create the keyboard device
 *		-#	Set the keyboards cooperation level
 *		-#	Set the keyboards data format
 *		-#	Aquire the keyboard
 */
bool DI8Keyboard::Initialise(PlatformData *pd)
{
	Win32PlatformData *wpd = (Win32PlatformData *)pd;

	// create keyboard object
	if( di8->CreateDevice(GUID_SysKeyboard,&device, NULL) == DI_OK )
	{
		// set keyboard co-operative level
		if( device->SetCooperativeLevel(wpd->m_hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE) == DI_OK )
		{
			// set keyboard data format (uses default)
			if( device->SetDataFormat(&c_dfDIKeyboard) == DI_OK )
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
 */
bool DI8Keyboard::ReadDevice(void)
{
	if( device->GetDeviceState(256, keystates) != DI_OK )	return false;

	return true;
}

/**	Updates the device
 *
 *	@returns boolean true or false, depending on whether the device updated successfully or not
 *
 *	Operation:
 *		-#	Flush the event queue
 *		-#	call DI8Keyboard::ReadDevice() to read the device state
 *		-#	If there are listeners on this device, loop through each key
 *		-#	If MODE_ASCII is enabled, get the ascii mapped value of this key
 *		-#	Check whether the button is pressed or not
 *		-#	If the buttonstate is false, add an event to signal a key is pressed and enable the buttonstate to true
 *		-#	If the button is not pressed and the buttonstate is set to true, Add an event to signal a key released and set the buttonstate to false
 *		-#	Once looped through all the keys, send all the events
 */
bool DI8Keyboard::Update(void)
{
	int buttonid=0;

	FlushEventQueue();
	
	if(ReadDevice() == true){
		if(m_listener != NULL){
			for(int a=0;a<256;a++){
				buttonid = DirectInputMap[a];

				if(m_devicemode & MODE_ASCII)	buttonid = AsciiKeyMap[buttonid];

				if(keystates[a] & 0x80){
					if(m_buttonstate[a] == false){
						// this is keydown event
						AddEvent(CreateButtonEvent(KEYBOARD,BUTTON_DOWN,buttonid));
						m_buttonstate[a] = true;
					}
				}else{
					if(m_buttonstate[a] == true){
						// this is keyup event
						AddEvent(CreateButtonEvent(KEYBOARD,BUTTON_UP,buttonid));
						m_buttonstate[a] = false;
					}
				}
			}
			SendEvents();
		}
		return true;
	}

	return false;
}

/**	Checks a keyboard key
 *
 *	@param button	The key being checked
 *
 *	@returns boolean true or false, depending on whether the key was pressed or not
 */
bool DI8Keyboard::CheckButton(int button)
{
	if(keystates[InputMap[button]] & 0x80)	return true;

	return false;
}