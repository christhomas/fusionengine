#ifndef _INPUTKEYBOARD_H_
	#define _INPUTKEYBOARD_H_

#include <input/IInputDevice.h>

/**	@ingroup	Input_Group
 *	@brief		Derived IInputDevice class for all keyboard devices
 */
class InputKeyboard : public IInputDevice{
protected:
	/**	@var		Entity *m_cursor
	 *	@brief	The cursor this device uses
	 */
	Entity *m_cursor;

	/**	@var		int m_x
	 *	@brief	The x position of the cursor
	 */
	int m_x;
	
	/**	@var		int m_y
	 *	@brief	The y position of the cursor
	 */
	int m_y;
	
	/**	@var		bool *m_buttonstate
	 *	@brief	An array of boolean values denoting whether a button/key on the device is pressed or released
	 */
	bool *m_buttonstate;

	/**	@var		Rect *m_AxisArea
	 *	@brief	The area the device can move the cursor within
	 */
	Rect *m_AxisArea;
	
	virtual void	ConfinePositionToArea	(void);
	virtual void	AddEvent				(InputEvent *e);
	virtual void	SendEvents			(void);
public:
				InputKeyboard			(int DeviceID);
	virtual		~InputKeyboard		();

	// Activate/DeActivate a device
	virtual bool	GetActive				(void);
	virtual void	SetActive				(bool Active);

	// Setting/Positioning cursor
	virtual	void	SetCursor				(Entity *cursor);
	virtual void	SetCursorPosition		(int x, int y);
	virtual void	GetCursorPosition		(int &x, int &y);
	virtual void	SetAxisArea			(class Rect *r);
	virtual void	SetAxisArea			(int sx, int sy,int ex, int ey);

	// Listener System
	virtual void	AddListener			(InputEvent **listener);
	virtual void	RemoveListener		(InputEvent **listener);

	virtual int		GetDeviceID			(void);
	virtual int		GetDeviceType			(void);
	virtual void	SetDeviceMode		(int mode);

	// Event System
	virtual void	FlushEventQueue		(void);
};

#endif // #ifndef _INPUTKEYBOARD_H_
