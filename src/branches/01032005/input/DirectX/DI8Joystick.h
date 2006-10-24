#ifndef _DI8JOYSTICK_H_
	#define _DI8JOYSTICK_H_

#include <Input/InputJoystick.h>

/**	@ingroup	DI8_Input_Group
 *	@brief		Derived InputJoystick class for the DirectInput8 Platform
 */
class DI8Joystick: public InputJoystick 
{
protected:
	/**	@var		IDirectInputDevice8 *device
	 *	@brief	The DirectInput device for the joystick
	 */
	IDirectInputDevice8 *device;

	/**	@var		DIJOYSTATE2 State
	 *	@brief	A structure representing the state of the joystick
	 */
	DIJOYSTATE2 State;

	//	For Creating Joysticks which can't be created directly, you have to enumerate them (sounds awful)
	friend BOOL CALLBACK	EnumJoysticksCallback(const DIDEVICEINSTANCE *instance, void *Context);
public:
								DI8Joystick			(int DeviceID);
	virtual				~DI8Joystick		();
	virtual bool	Initialise			(PlatformData *dd);
	virtual bool	ReadDevice			(void);
	virtual bool	CheckButton			(int button);
	virtual long	CheckAxis				(int axis);
};


#endif // #ifndef _DI8JOYSTICK_H_