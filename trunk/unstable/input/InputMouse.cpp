// InputMouse.cpp: implementation of the InputMouse class.
//
//////////////////////////////////////////////////////////////////////

#include <Input/InputMouse.h>
#include <Input/InputEvent.h>
#include <Mesh/Entity.h>
#include <Rect.h>

#if defined (WIN32)
#pragma warning (disable:4244)	// disable conversion warnings (dbl -> fl)
#endif

/**	InputMouse constructor
 *
 *	@param DeviceID	The id of this device within the database
 *
 *	Initialises all the mouse data to either zero or NULL and the DeviceID to whatever is passed in
 *
 *	Sets all the button/key states to false (released)
 */
InputMouse::InputMouse(int DeviceID)
{	
	m_DeviceType	=	MOUSE;
	m_DeviceID		=	DeviceID;
	m_event			=	NULL;
	m_listener		=	NULL;
	m_Active		=	true;
	m_AxisArea		=	NULL;
	m_x				=	0;
	m_y				=	0;
	m_devicemode	=	MODE_NORMAL;

	m_cursor		=	NULL;

	m_buttonstate = new bool[3];
	for(int a=0;a<3;a++) m_buttonstate[a] = false;
}

/**	InputMouse Deconstructor
 *
 *	Operation:
 *		-#	Flush the event queue for this device
 *		-#	Remove all the listeners
 *		-#	Delete all the device memory
 */
InputMouse::~InputMouse()
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
bool InputMouse::GetActive(void)
{
	return m_Active;
}

/**	Sets the active state of the device
 *
 *	@param Active	The active state to set the device to
 */
void InputMouse::SetActive(bool Active)
{
	m_Active = Active;
}

/**	Sets the cursor this device will use
 *
 *	@param cursor	The cursor to use
 */
void InputMouse::SetCursor(Entity *cursor)
{
	m_cursor = cursor;
}

/**	Sets the position of the cursor
 *
 *	@param x	The x position
 *	@param y	The y position
 */
void InputMouse::SetCursorPosition(int x, int y)
{
	m_x = x;
	m_y = y;
}

/**	Retrieves the position of the cursor
 *
 *	@param x The x position
 *	@param y The y position
 */
void InputMouse::GetCursorPosition(int &x, int &y)
{
	x = m_x;
	y = m_y;
}

/**	Adds a listener to this device
 *
 *	@param listener	The listener to add
 */
void InputMouse::AddListener(InputEvent **listener)
{
	DeviceListener *dl	= new DeviceListener;
	dl->queue			= listener;
	dl->prev			= m_listener;
	m_listener			= dl;
}

/**	Removes a listener from this device
 *
 *	@param listener	The listener to remove
 */
void InputMouse::RemoveListener(InputEvent **listener)
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
void InputMouse::AddEvent(InputEvent *e)
{
	e->prev = m_event;
	m_event = e;
}

/**	Flushes the input event queue */
void InputMouse::FlushEventQueue(void)
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
void InputMouse::SendEvents(void)
{
	for(DeviceListener *dl=m_listener;dl!=NULL;dl=dl->prev){
		*dl->queue = m_event;
	}
}

/**	Retrieves the ID of the device
 *
 *	@returns	The ID of the device
 */
int InputMouse::GetDeviceID(void)
{
	return m_DeviceID;
}

/**	Retrieves the type of device
 *
 *	@returns	The type of the device
 */
int InputMouse::GetDeviceType(void)
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
void InputMouse::SetAxisArea(int sx, int sy, int ex, int ey)
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
void InputMouse::SetAxisArea(class Rect *r)
{
	delete m_AxisArea;

	m_AxisArea = r;
}

/**	Confines the cursors position to the defined rectangle
 *
 *	This uses AxisArea rectangle to confine the cursor
 */
void InputMouse::ConfinePositionToArea(void)
{
	if(m_AxisArea != NULL)
	{
		if(m_x < m_AxisArea->left)		m_x = m_AxisArea->left;
		if(m_x > m_AxisArea->right)		m_x = m_AxisArea->right;
		if(m_y < m_AxisArea->top)		m_y = m_AxisArea->top;
		if(m_y > m_AxisArea->bottom)	m_y = m_AxisArea->bottom;
	}
}

/**	Sets the device mode
 *
 *	@param mode	The mode to set the device with
 */
void InputMouse::SetDeviceMode(int mode)
{
	m_devicemode = mode;
}

/**	Updates the device
 *
 *	@returns boolean true of false, depending on whether the device updated successfully or not
 *
 *	Operation:
 *		-#	call InputMouse::ReadDevice() and if returns successful, continue
 *		-#	If the cursor is valid, updates it's position with the mouse cursor position
 */
bool InputMouse::Update(void)
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

/**	Reads the device
 *
 *	returns boolean true of false, depending on whether the device read successfully or not
 *
 *	Operation:
 *		-#	Check the x/y axes
 *		-#	Check the device state, if MODE_NORMAL, add the axis values to the cursor position, otherwise, 
 *				if MODE_RELPOS, make the cursor position equal to the axis values
 *		-#	Confine the position to the Axis Rectangle
 *		-#	If there are listeners to this device create events for all the input events
 *		-#	Flush the input event queue
 *		-#	If the cursor position has moved, create an XY event and add it to the queue
 *		-#	Loop through all the device buttons and create BUTTON_UP, BUTTON_DOWN events for each button action
 *		-#	Send the events to all the device listeners
 */
bool InputMouse::ReadDevice(void)
{
	static int last_x=0,last_y=0;
	int tx,ty;

	tx = CheckAxis(1);
	ty = CheckAxis(2);

	if(m_devicemode & MODE_NORMAL){
		m_x += tx;
		m_y += ty;
	}else if(m_devicemode & MODE_RELPOS){
		m_x = tx;
		m_y = ty;
	}

	ConfinePositionToArea();

	if(m_listener != NULL){
		//	flush input events
		FlushEventQueue();

		if((last_x != m_x) && (last_y != m_y)){
			AddEvent(CreateXYEvent(m_hotspot.x+m_x,m_hotspot.y+m_y));
		}

		for(int a=0;a<3;a++)
		{
			if(CheckButton(a) == true){
				if(m_buttonstate[a] == false){
					AddEvent(CreateButtonEvent(MOUSE,BUTTON_DOWN,a));
					m_buttonstate[a] = true;
				}
			}else{
				if(m_buttonstate[a] == true){
					AddEvent(CreateButtonEvent(MOUSE,BUTTON_UP,a));
					m_buttonstate[a] = false;
				}
			}
		}
		
		//	Send input events
		SendEvents();
	}


	last_x = m_x;
	last_y = m_y;

	return true;
}

/**	Set the hotspot for the cursor
 *
 *	@param x	The x position in the cursor
 *	@param y	The y position in the cursor
 */
void InputMouse::SetHotspot(int x, int y)
{
	m_hotspot.Set(x,y,0);
}