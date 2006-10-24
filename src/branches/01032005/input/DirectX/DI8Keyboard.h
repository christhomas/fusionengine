#ifndef _DI8KEYBOARD_H_
	#define _DI8KEYBOARD_H_

#include <Input/InputKeyboard.h>

/**	@ingroup	DI8_Input_Group
 *	@brief		Derived InputKeyboard class for the DirectInput8 Platform
 */
class DI8Keyboard: public InputKeyboard  
{
protected:
	/**	@var		IDirectInputDevice8 *device
	 *	@brief	The DirectInput device for the keyboard
	 */
	IDirectInputDevice8 *device;

	/**	@var		unsigned char keystates[256]
	 *	@brief	An array of values representing the keyboard keys
	 */
	unsigned char keystates[256];
public:
								DI8Keyboard			(int DeviceID);
	virtual				~DI8Keyboard		();
	virtual bool	Initialise			(PlatformData *dd);
	virtual bool	ReadDevice			(void);
	virtual bool	CheckButton			(int button);
	virtual bool	Update					(void);
};

#endif // #ifndef _DI8KEYBOARD_H_