// InputKeyboard.cpp: implementation of the InputKeyboard class.
//
//////////////////////////////////////////////////////////////////////

#include <Input/InputKeyboard.h>
#include <Input/InputEvent.h>
#include <Rect.h>

/**	InputKeyboard constructor
 *
 *	@param DeviceID	The id of this device within the database
 *
 *	Initialises all the keyboard data to either zero or NULL and the DeviceID to whatever is passed in
 *
 *	Sets all the button/key states to false (released)
 */
InputKeyboard::InputKeyboard(int DeviceID)
{
	m_DeviceType	=	KEYBOARD;
	m_DeviceID		=	DeviceID;	
	m_listener		=	NULL;
	m_event				=	NULL;
	m_eventsys		=	false;	
	m_Active			=	true;
	m_AxisArea		=	NULL;
	m_x						=	0;
	m_y						=	0;
	m_devicemode	=	MODE_NORMAL;

	m_buttonstate = new bool[256];
	for(int a=0;a<256;a++) m_buttonstate[a] = false;
}

/**	InputKeyboard Deconstructor
 *
 *	Operation:
 *		-#	Flush the event queue for this device
 *		-#	Remove all the listeners
 *		-#	Delete all the device memory
 */
InputKeyboard::~InputKeyboard()
{
	FlushEventQueue();

	DeviceListener *dl = m_listener,*temp;
	while(dl!=NULL){
		temp = dl->prev;
		delete dl;
		dl=temp;
	}

	delete m_AxisArea;
	delete m_buttonstate;
}

/**	Retrives the active state of the device
 *
 *	@returns boolean true or false, depending on whether the device is active or not
 */
bool InputKeyboard::GetActive(void)
{
	return m_Active;
}

/**	Sets the active state of the device
 *
 *	@param Active	The active state to set the device to
 */
void InputKeyboard::SetActive(bool Active)
{
	m_Active = Active;
}

/**	Sets the cursor this device will use
 *
 *	@param cursor	The cursor to use
 */
void InputKeyboard::SetCursor(Entity *cursor)
{
	m_cursor = cursor;
}

/**	Sets the position of the cursor
 *
 *	@param x	The x position
 *	@param y	The y position
 */
void InputKeyboard::SetCursorPosition(int x, int y)
{
	m_x = x;
	m_y = y;
}

/**	Retrieves the position of the cursor
 *
 *	@param x The x position
 *	@param y The y position
 */
void InputKeyboard::GetCursorPosition(int &x, int &y)
{
	x = m_x;
	y = m_y;
}

/**	Adds a listener to this device
 *
 *	@param listener	The listener to add
 */
void InputKeyboard::AddListener(InputEvent **listener)
{
	DeviceListener *dl	= new DeviceListener;
	dl->queue						= listener;
	dl->prev						= m_listener;
	m_listener					= dl;
	m_eventsys					= true;
}

/**	Removes a listener from this device
 *
 *	@param listener	The listener to remove
 */
void InputKeyboard::RemoveListener(InputEvent **listener)
{
	for(DeviceListener *dl=m_listener;dl!=NULL;dl=dl->prev){
		if(dl->queue == listener){
			m_listener = dl->prev;
			delete dl;
		}
		if(dl->prev->queue == listener){
			DeviceListener *temp = dl->prev->prev;
			delete dl->prev;
			dl->prev = temp;			
		}	
	}

	if(m_listener == NULL) m_eventsys = false;
}

/**	Adds an event to the queue
 *
 *	@param e	The event to add to the queue
 */
void InputKeyboard::AddEvent(InputEvent *e)
{
	if(m_eventsys == true){
		e->prev = m_event;
		m_event = e;
	}
}

/**	Flushes the input event queue */
void InputKeyboard::FlushEventQueue(void)
{
	InputEvent *temp;
	while(m_event!=NULL){
		temp=m_event->prev;
		delete m_event;
		m_event = temp;
	}
}

/**	Sends the events to all the listeners
 *
 *	Allows each listener to read the input event queue of this object, 
 *	therefore the object which owns the listener pointer must not in 
 *	any way modify the event queue
 */
void InputKeyboard::SendEvents(void)
{
	for(DeviceListener *dl=m_listener;dl!=NULL;dl=dl->prev){
		*dl->queue = m_event;
	}
}

/**	Retrieves the ID of the device
 *
 *	@returns	The ID of the device
 */
int InputKeyboard::GetDeviceID(void)
{
	return m_DeviceID;
}

/**	Retrieves the type of device
 *
 *	@returns	The type of the device
 */
int InputKeyboard::GetDeviceType(void)
{
	return m_DeviceType;
}

/**	Sets the Confining rectangle for the cursor
 *
 *	@param sx	The minimal x position for the cursor
 *	@param sy	The minimal y position for the cursor
 *	@param ex	The maximum x position for the cursor
 *	@param ey The maximum y position for the cursor
 *
 *	@todo	Rename this method SetCursorArea
 */
void InputKeyboard::SetAxisArea(int sx, int sy, int ex, int ey)
{
	class Rect *rect = new Rect(sx,sy,ex,ey);

	SetAxisArea(rect);
}

/**	Sets the Confining rectangle for the cursor
 *
 *	@param r	The rectangle defining the minimum/maximum position for the cursor
 *
 *	@todo	Rename this method SetCursorArea
 */
void InputKeyboard::SetAxisArea(class Rect *r)
{
	delete m_AxisArea;

	m_AxisArea = r;
}

/**	Confines the cursors position to the defined rectangle
 *
 *	This uses AxisArea rectangle to confine the cursor
 */
void InputKeyboard::ConfinePositionToArea(void)
{
	if(m_AxisArea != NULL)
	{
		if(m_x < m_AxisArea->left)		m_x = m_AxisArea->left;
		if(m_x > m_AxisArea->right)		m_x = m_AxisArea->right;
		if(m_y < m_AxisArea->top)			m_y = m_AxisArea->top;
		if(m_y > m_AxisArea->bottom)	m_y = m_AxisArea->bottom;
	}
}

/**	Sets the device mode
 *
 *	@param mode	The mode to set the device with
 */
void InputKeyboard::SetDeviceMode(int mode)
{
	m_devicemode = mode;
}