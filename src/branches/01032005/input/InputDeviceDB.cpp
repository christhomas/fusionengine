// InputDeviceDB.cpp: implementation of the InputDeviceDB class.
//
//////////////////////////////////////////////////////////////////////

#include <Input/InputDeviceDB.h>
#include <Fusion.h>

/**	InputDeviceDB Constructor
 *
 *	Operation:
 *		-#	Set all the linked lists to null and number of devices to 0
 *		-#	Set the name of the subsystem and it's module dll
 */
InputDeviceDB::InputDeviceDB(void)
{
	m_mouse				= NULL;
	m_keyboard			= NULL;
	m_joystick			= NULL;

	m_num_mouse			= 0;
	m_num_keyboard		= 0;
	m_num_joystick		= 0;

	object_name			= "InputDeviceDB Version 1.2 <chris_thomas@hotmail.com>";
	object_filename		= "libInput.dll";
}

/**	InputDeviceDB Deconstructor
 *
 *	Operation:
 *		-#	Remove all the Mice
 *		-#	Remove all the Keyboards
 *		-#	Remove all the joysticks
 */
InputDeviceDB::~InputDeviceDB()
{
	IInputDevice *temp;

	temp = m_mouse;
	while(m_mouse != NULL){
		temp = m_mouse->prev;
		delete m_mouse;
		m_mouse = temp;
	}

	temp = m_keyboard;
	while(m_keyboard != NULL){
		temp = m_keyboard->prev;
		delete m_keyboard;
		m_keyboard = temp;	
	}

	temp = m_joystick;
	while(m_joystick != NULL){
		temp = m_joystick->prev;
		delete m_joystick;
		m_joystick = temp;	
	}
}

/**	Initialise the subsystem
 *
 *	@returns true
 *
 *	@todo Deprecate from the fusion subsystem object and allow each object to decide whether it needs an initialisation method or not
 */
bool InputDeviceDB::Initialise(void)
{
	return true;
}

/**	Retrieves a device pointer
 *
 *	@param type			The type of device to retrieve
 *	@param DeviceID	The id of the device in the database
 *
 *	@returns	An IInputDevice object or NULL, if the object was not found
 */
IInputDevice * InputDeviceDB::GetDevicePtr(IInputDevice::DeviceType type,unsigned int DeviceID)
{
	IInputDevice *temp;

	if(type == IInputDevice::MOUSE)		for(temp=m_mouse;	temp!=NULL;temp=temp->prev)	if(temp->GetDeviceID() == DeviceID) return temp;
	if(type == IInputDevice::KEYBOARD)	for(temp=m_keyboard;temp!=NULL;temp=temp->prev)	if(temp->GetDeviceID() == DeviceID) return temp;
	if(type == IInputDevice::JOYSTICK)	for(temp=m_joystick;temp!=NULL;temp=temp->prev)	if(temp->GetDeviceID() == DeviceID) return temp;

	return NULL;
}

/**	Removes a device from the database
 *
 *	@param id	The input device to remove
 *
 *	NOTE: This method is not yet implemented
 */
void InputDeviceDB::RemoveDevice(IInputDevice **id)
{
	// not implemented yet
}

/**	Stores a device in the database
 *
 *	@param device	The device to store
 *
 *	Operation:
 *		-#	Find out what type of device this is
 *		-#	Add it to the appropriate linked list
 */
void InputDeviceDB::StoreDevice(IInputDevice *device)
{
	switch(device->GetDeviceType()){
		case IInputDevice::MOUSE:		{	device->prev = m_mouse;		m_mouse		= device;	}break;
		case IInputDevice::KEYBOARD:	{	device->prev = m_keyboard;	m_keyboard	= device;	}break;
		case IInputDevice::JOYSTICK:	{	device->prev = m_joystick;	m_joystick	= device;	}break;
	};
}

/**	Flush all the device input event queues */
void InputDeviceDB::FlushAll(void)
{
	IInputDevice *temp;

	for(temp=m_mouse;	temp!=NULL;temp=temp->prev)	temp->FlushEventQueue();
	for(temp=m_keyboard;temp!=NULL;temp=temp->prev)	temp->FlushEventQueue();
	for(temp=m_joystick;temp!=NULL;temp=temp->prev)	temp->FlushEventQueue();
}

/**	Updates all the devices in the database
 *
 *	@returns boolean true or false, but this method ignores any failure and only returns true at this time
 *
 *	Operation:
 *		-#	calls each devices IInputDevice::GetActive() method to find whether the device is active or not
 *		-#	If the device is active, call IInputDevice::Update() method to update the device
 *
 *	Do nothing if the device fails to update properly
 */
bool InputDeviceDB::Update(void)
{
	IInputDevice *temp;

	//	For now, do nothing when a device fails to update 
	//	(havent figured out what is best to do in this situation, just carry on, or do some interrogation)
	for(temp=m_mouse;	temp!=NULL;temp=temp->prev)	if(temp->GetActive() == true)	if(temp->Update() == false){}
	for(temp=m_keyboard;temp!=NULL;temp=temp->prev)	if(temp->GetActive() == true)	if(temp->Update() == false){}
	for(temp=m_joystick;temp!=NULL;temp=temp->prev)	if(temp->GetActive() == true)	if(temp->Update() == false){}

	return true;
}