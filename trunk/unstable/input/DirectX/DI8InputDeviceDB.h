#ifndef _DI8INPUTDEVICEDB_H_
	#define _DI8INPUTDEVICEDB_H_

#include <Input/InputDeviceDB.h>
#include <PlatformData.h>
#include <dinput.h>

#include <DI8Joystick.h>
#include <DI8Mouse.h>
#include <DI8Keyboard.h>

/**	@ingroup	DI8_Input_Group
 *	@brief		Derived InputDeviceDB class for the DirectInput8 Platform
 */
class DI8InputDeviceDB: public InputDeviceDB{
protected:
	/**	@var		Win32PlatformData *m_platform
	 *	@brief	A pointer to Fusions platform information
	 */
	Win32PlatformData *m_platform;
public:
													DI8InputDeviceDB	();
	virtual									~DI8InputDeviceDB	();
	virtual bool						CreateDirectInput	(void);
	virtual IInputDevice *	AddDevice					(IInputDevice::DeviceType type);
};

extern IDirectInput8 *di8;

#endif // #ifndef _DI8INPUTDEVICEDB_H_
