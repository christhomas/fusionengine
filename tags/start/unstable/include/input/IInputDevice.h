#ifndef _IINPUTDEVICE_H_
	#define _IINPUTDEVICE_H_

struct	PlatformData;
class		InputEvent;
class		Entity;

#include <cstdio>

/**	@ingroup	Input_Group
 *	@brief		Defines a method for listening objects to access events made by a device
 *
 *	Defines a device listener structure, this is for storing the ptr to the input event array
 *	each one of these structures represents a pair of ears(tm) and you have to pass events onto
 *	every one of these ears, so they all can hear what you have to say, basically, broadcasting the
 *	events to the whole system, if necessary
 */
struct DeviceListener{
	/**	@var		InputEvent **queue
	 *	@brief	This is a pointer to a linked list which represents how a listener "hears" the events the device makes
	 */
	InputEvent **queue;

	/**	@var		DeviceListner *prev
	 *	@brief	This is a linked list pointer, to the next listener in the linked list
	 */
	DeviceListener *prev;
};

/**	@ingroup	Input_Group
 *	@brief		The base class for all Input Devices
 */
class IInputDevice 
{
protected:
	/**	@var		bool m_Active
	 *	@brief	Whether the device is active or not
	 */
	bool m_Active;

	/**	@var		int m_DeviceType
	 *	@brief	What device it is, either MOUSE/KEYBOARD or JOYSTICK
	 */
	int m_DeviceType;

	/**	@var		int m_DeviceID
	 *	@brief	The ID Code of this device
	 */
	int m_DeviceID;
	
	/**	@var		DeviceListener *m_listener
	 *	@brief	Linked list of listeners
	 *
	 *	A Device listener is something that listens to what the device does, i.e.  The events it produces
	 *	These events are put into the InputEvent member where whoever owns the InputEvent queue can read the events and act upon them
	 */
	DeviceListener *m_listener;

	/**	@var		bool m_eventsys
	 *	@brief	Whether the event system is active or not
	 *
	 *	There are two methods to run the input system, immediate or event modes.
	 *	Immediate modes mean to get the input state, you have to directly ask the 
	 *	device, just like Polling the device for it's state each frame
	 *	Event mode means the device will output the input events to each listener, 
	 *	which can act upon those events as they occur, rather than check for whether the events occured
	 */
	bool m_eventsys;

	/**	@var		InputEvent *m_event
	 *	@brief	Linked list of events which have occured
	 */
	InputEvent *m_event;

	/**	@var		int m_devicemode
	 *	@brief	The mode of the device
	 *
	 *	Supported modes are documented in IInputDevice::DeviceMode
	 */
	int m_devicemode;

	/**	Add an event to the queue
	 *
	 *	@param e	The event to add
	 */
	virtual void AddEvent(InputEvent *e) = 0;

	/**	Sends the events to all the listeners */
	virtual void SendEvents(void) = 0;	
public:

	/**	@enum		DeviceType
	 *	@brief	Supported device types
	 */
	enum DeviceType{	
										MOUSE=0,	/**<	This device is a Mouse		*/
										KEYBOARD,	/**<	This device is a Keyboard */
										JOYSTICK	/**<	This device is a Joystick	*/
									};

	/**	@enum		DeviceEvent
	 *	@brief	Events the devices could create
	 */
	enum DeviceEvent{
										EVENT_XY=0,		/**<	Device Axis event		*/
										EVENT_BUTTON	/**<	Device button event	*/
									};

	/**	@enum		ButtonAction
	 *	@brief	When a button is pressed on the device, one of these codes is used
	 */
	enum ButtonAction{
											BUTTON_DOWN=0,	/**<	Button has been pressed		*/
											BUTTON_UP				/**<	Button has been released	*/
										};

	/**	@enum		DeviceMode
	 *	@brief	The supported device modes
	 */
	enum DeviceMode{
		MODE_NORMAL			= 1,	/**<	Device is normal, no special conditions	*/
		MODE_ASCII			= 2,	/**<	Keyboard has been set to output ascii keyboard codes instead of Fusion codes */
		MODE_RELPOS			= 4,	/**<	Mouse or joystick has been set to output relative position codes */
		MODE_AXISSMOOTH	= 8		/**<	Mouse or Joystick has been set to smooth the output of the axis movement */
	};

	/**	@var		IInputDevice *prev
	 *	@brief	The previous inputdevice in the Databases list of input devices
	 */
	IInputDevice *prev;

	/**	Input Device Constructor */
	IInputDevice(){};

	/**	Input Device Deconstructor */
	virtual ~IInputDevice(){};

	/**	Initialise the device 
	 *
	 *	@param pd	The Fusions Platform data
	 *
	 *	@returns boolean true or false, depending on whether the device initialised successfully or not
	 */
	virtual bool Initialise(PlatformData *pd) = 0;

	/**	Retrieve the device active status
	 *
	 *	@returns boolean true or false, depending on whether the device is active or not
	 */
	virtual bool GetActive(void) = 0;

	/**	Sets a device active status
	 *
	 *	@param Active	Whether to set the device as active or not
	 */
	virtual void SetActive(bool Active) = 0;

	/**	Updates the device
	 *
	 *	@returns boolean true or false, depending on whether the device updated successfully or not
	 *
	 *	This is the main method for updating a device, it will do all the necessary work
	 *	needed to update all of it's functionality, unless some of it was disabled by setting a particular device state
	 *
	 *	@todo	Look at deprecating this method and ReadDevice in favour of one method 
	 *				which does all thats necessary, since this method and ReadDevice seem to do the same thing anyway
	 */
	virtual bool Update(void) = 0;

	/**	Reads the device
	 *
	 *	@returns boolean true or false, depending on whether the device read successfully or not
	 */
	virtual bool ReadDevice(void) = 0;

	/**	Checks a Button on the device (or a key)
	 *
	 *	@param button	The button(or key) to check
	 *
	 *	@returns boolean true or false, depending on whether the button/key was pressed
	 */
	virtual bool CheckButton(int button) = 0;

	/**	Retrieve the Device ID
	 *
	 *	@returns The ID of the device within the Database
	 */
	virtual int GetDeviceID(void) = 0;

	/**	Retrieves the Device Type
	 *
	 *	@returns	The Type of device
	 */
	virtual int GetDeviceType(void) = 0;

	/**	Sets the Device Mode
	 *
	 *	@param mode	The mode to set the device with, if no parameter was passed, MODE_NORMAL is inserted
	 */
	virtual void SetDeviceMode(int mode=MODE_NORMAL) = 0;

	/**	Sets the cursor for this device
	 *
	 *	@param cursor	The entity this device will use as a cursor
	 */
	virtual void SetCursor(Entity *cursor) = 0;

	/**	Sets the position of the cursor
	 *
	 *	@param x	The x position onscreen
	 *	@param y	The y position onscreen
	 */
	virtual void SetCursorPosition(int x, int y) = 0;

	/**	Retrieves the position of the cursor
	 *
	 *	@param x	The x position
	 *	@param y	The y position
	 */
	virtual void GetCursorPosition(int &x, int &y) = 0;

	/**	Sets the area the cursor can move around in
	 *
	 *	@param r	The rectangle the cursor can move within
	 *
	 *	@todo	Rename this method, AxisArea, wtf does that mean anyway, should be SetCursorArea or something
	 */
	virtual void SetAxisArea(class Rect *r) = 0;

	/**	Sets the area the cursor can move around in
	 *
	 *	@param sx	The minimal x position the cursor can take
	 *	@param sy	The minimal y position the cursor can take
	 *	@param ex	The maximum x position the cursor can take
	 *	@param ey	The maximum y position the cursor can take
	 *
	 *	@todo	Rename this method, AxisArea, wtf does that mean anyway, should be SetCursorArea or something
	 */
	virtual void SetAxisArea(int sx,int sy,int ex,int ey) = 0;
	
	/**	Adds a new listener to this device
	 *
	 *	@param listener	A Linked list pointer which will receive all the events
	 *
	 *	This method takes a pointer to a linked list (which is why it needs a 
	 *	double pointer, a linked list is represented by a ptr (to the last 
	 *	data element), hence it's a ptr to a ptr) and when it needs to update 
	 *	that linked list with new events, it'll dereference the first pointer 
	 *	to then be able to add the new events to the linked list
	 */
	virtual void AddListener(InputEvent **listener) = 0;

	/**	Removes a listener from this device
	 *
	 *	@param listener	A ptr to the listener (a linked list) to remove from this device
	 */
	virtual void RemoveListener(InputEvent **listener) = 0;

	/**	Flushes the event queue */
	virtual void FlushEventQueue(void) = 0;
};

#endif // #ifndef _IINPUTDEVICE_H_
