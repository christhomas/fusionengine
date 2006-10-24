#ifndef _DI8MOUSE_H_
	#define _DI8MOUSE_H_

#include <Input/InputMouse.h>

/**	@ingroup	DI8_Input_Group
 *	@brief		Derived InputMouse class for the DirectInput8 Platform
 */
class DI8Mouse: public InputMouse  
{
protected:
	/**	@var		IDirectInputDevice8 *device
	 *	@brief	The DirectInput device for the mouse
	 */
	IDirectInputDevice8 *device;

	/**	@var		DIMOUSESTATE2 State
	 *	@brief	A Structure representing the state of the mouse
	 */
	DIMOUSESTATE2 State;
public:
								DI8Mouse			(int DeviceID);
	virtual				~DI8Mouse			();
	virtual bool	Initialise		(PlatformData *dd);		//	Initialises the device
	virtual bool	ReadDevice		(void);								//	Reads the device's state
	virtual bool	CheckButton		(int button);					//	To check whether a button is pressed
	virtual long	CheckAxis			(int axis);						//	Checks a particular axis for it's value
};

#endif // #ifndef _DI8MOUSE_H_