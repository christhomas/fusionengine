// InputJoystick.cpp: implementation of the InputJoystick class.
//
//////////////////////////////////////////////////////////////////////

#include <Input/InputJoystick.h>
#include <Input/InputEvent.h>
#include <Maths/Vector.h>
#include <Mesh/Entity.h>
#include <Rect.h>

#if defined (WIN32)
#pragma warning (disable:4244)	// disable conversion warnings (dbl -> fl)
#endif

/**	InputJoystick constructor
 *
 *	@param DeviceID	The id of this device within the database
 *
 *	Initialises all the joystick data to either zero or NULL and the DeviceID to whatever is passed in
 */
InputJoystick::InputJoystick(int DeviceID)
{
	m_DeviceType	=	JOYSTICK;
	m_DeviceID		=	DeviceID;
	m_cursor			=	NULL;
	m_listener		=	NULL;
	m_Active			=	true;
	m_AxisArea		=	NULL;
	m_event				=	NULL;
	m_x						=	0;
	m_y						=	0;
	m_buttonstate	=	NULL;
	m_devicemode	=	MODE_NORMAL;
}

/**	InputJoystick Deconstructor
 *
 *	Operation:
 *		-#	Flush the event queue for this device
 *		-#	Remove all the listeners
 *		-#	Delete all the device memory
 */
InputJoystick::~InputJoystick()
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

/**	Updates the joystick
 *
 *	@returns boolean true of false, depending on whether the joystick updated successfully or not
 *
 *	Operation:
 *		-#	call InputJoystick::ReadDevice() and if returns successful, continue
 *		-#	If the cursor is valid, update it's position with the joysticks cursor position
 */
bool InputJoystick::Update(void)
{
	if(ReadDevice() == true){
		if(m_cursor!=NULL){
			Maths::Vector *v = m_cursor->GetPosition();
			m_cursor->SetTranslate(TRANSLATE_ABS,m_x,m_y,v->z);
		}
		return true;
	}

	return false;
}

/**	Reads the Device
 *
 *	@returns boolean true or false, depending on whether the joystick read successfully or not
 *
 *	NOTE: Not Implemented
 */
bool InputJoystick::ReadDevice(void)
{
	return true;
}

/**	Retrives the active state of the device
 *
 *	@returns boolean true or false, depending on whether the device is active or not
 */
bool InputJoystick::GetActive(void)
{
	return m_Active;
}

/**	Sets the active state of the device
 *
 *	@param Active	The active state to set the device to
 */
void InputJoystick::SetActive(bool Active)
{
	m_Active = Active;
}

/**	Sets the cursor this device will use
 *
 *	@param cursor	The cursor to use
 */
void InputJoystick::SetCursor(Entity *cursor)
{
	m_cursor = cursor;
}

/**	Sets the position of the cursor
 *
 *	@param x	The x position
 *	@param y	The y position
 */
void InputJoystick::SetCursorPosition(int x, int y)
{
	m_x = x;
	m_y = y;
}

/**	Retrieves the position of the cursor
 *
 *	@param x The x position
 *	@param y The y position
 */
void InputJoystick::GetCursorPosition(int &x, int &y)
{
	x = m_x;
	y = m_y;
}

/**	Adds a listener to this device
 *
 *	@param listener	The listener to add
 */
void InputJoystick::AddListener(InputEvent **listener)
{
	DeviceListener *dl	= new DeviceListener;
	dl->queue						= listener;
	dl->prev						= m_listener;
	m_listener					= dl;
}

/**	Removes a listener from this device
 *
 *	@param listener	The listener to remove
 */
void InputJoystick::RemoveListener(InputEvent **listener)
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
}

/**	Adds an event to the queue
 *
 *	@param e	The event to add to the queue
 */
void InputJoystick::AddEvent(InputEvent *e)
{
	e->prev = m_event;
	m_event = e;
}

/**	Flushes the input event queue */
void InputJoystick::FlushEventQueue(void)
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
void InputJoystick::SendEvents(void)
{
	// loop through the list of listeners
	for(DeviceListener *dl=m_listener;dl!=NULL;dl=dl->prev){
		*dl->queue = m_event;
	}
}

/**	Retrieves the ID of the device
 *
 *	@returns	The ID of the device
 */
int InputJoystick::GetDeviceID(void)
{
	return m_DeviceID;
}

/**	Retrieves the type of device
 *
 *	@returns	The type of the device
 */
int InputJoystick::GetDeviceType(void)
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
void InputJoystick::SetAxisArea(int sx, int sy, int ex, int ey)
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
void InputJoystick::SetAxisArea(class Rect *r)
{
	delete m_AxisArea;

	m_AxisArea = r;
}

/**	Confines the cursors position to the defined rectangle
 *
 *	This uses AxisArea rectangle to confine the cursor
 */
void InputJoystick::ConfinePositionToArea(void)
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
void InputJoystick::SetDeviceMode(int mode)
{
	m_devicemode = mode;
}