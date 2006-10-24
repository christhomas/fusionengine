#ifndef _INPUTDEVICEDB_H_
	#define _INPUTDEVICEDB_H_

#include <FusionSubsystem.h>

#include <Input/InputEvent.h>
#include <Input/ButtonCodes.h>
#include <Input/IInputDevice.h>
#include <Input/InputMouse.h>
#include <Input/InputKeyboard.h>
#include <Input/InputJoystick.h>

/**	@ingroup	Input_Group
 *	@brief		The base class for all Input Device Databases
 */
class InputDeviceDB: public FusionSubsystem{
protected:
	/**	@var		IInputDevice *m_mouse
	 *	@brief	Linked list of mouse devices
	 */
	IInputDevice *m_mouse;

	/**	@var		IInputDevice *m_keyboard
	 *	@brief	Linked list of keyboard devices
	 */
	IInputDevice *m_keyboard;

	/**	@var		IInputDevice *m_joystick
	 *	@brief	Lined list of joystick devices
	 */
	IInputDevice *m_joystick;

	/**	@var		int m_num_mouse
	 *	@brief	The number of mice this database holds
	 */
	int	m_num_mouse;

	/**	@var		int m_num_keyboard
	 *	@brief	The number of keyboards this database holds
	 */
	int	m_num_keyboard;

	/**	@var		int m_num_joystick
	 *	@brief	The number of joysticks this database holds
	 */
	int	m_num_joystick;

	virtual	void						StoreDevice			(IInputDevice *device);
public:
													InputDeviceDB		();
	virtual									~InputDeviceDB	();
	virtual bool						Initialise			(void);
	virtual IInputDevice *	GetDevicePtr		(IInputDevice::DeviceType type,unsigned int DeviceID=0);
	virtual void						FlushAll				(void);
	virtual bool						Update					(void);

	/**	Adds an input device to the database
	 *
	 *	@param type	The type of device to add
	 *
	 *	@returns	An IInputDevice object or NULL, depending on whether the device created successfully or not
	 */
	virtual IInputDevice *	AddDevice				(IInputDevice::DeviceType type) = 0;
	virtual void						RemoveDevice		(IInputDevice **id);
};

#endif // #ifndef _INPUTDEVICEDB_H_

