#ifndef _INPUTEVENT_H_
	#define _INPUTEVENT_H_

#include <input/IInputDevice.h>

//======================================
//	The base class of every input event
//======================================
class InputEvent{
public:
	virtual ~InputEvent(){};
	InputEvent(int DeviceType,int InputEvent, int EventAction=0){
		m_DeviceType	=	DeviceType;
		m_InputEvent	=	InputEvent;
		m_EventAction	=	EventAction;
	}
	int m_DeviceType;		//	The DeviceType ID of the object sending this event
	int	m_InputEvent;		//	The Input event which occured
	int m_EventAction;		//	What action this event performs

	InputEvent *prev;
};

//======================================
//	Mouse Input Events
//======================================
class MouseXYEvent: public InputEvent{
public:
	inline MouseXYEvent(int DeviceType, int x, int y): InputEvent(DeviceType,IInputDevice::EVENT_XY){
		m_x	=	x;
		m_y	=	y;
	}
	virtual ~MouseXYEvent(){};

	//	The X,Y coordinate of the mouse
	int	m_x,m_y;
};

//======================================
//	Device Buttons/key Input Events
//======================================
class InputButtonEvent: public InputEvent{
public:
	inline InputButtonEvent(	int DeviceType, bool EventAction, int ButtonID):InputEvent(DeviceType,IInputDevice::EVENT_BUTTON)
	{
		m_ButtonID	=	ButtonID;
		m_EventAction	=	EventAction;
	}
	virtual ~InputButtonEvent(){};

	//	What the ButtonID is
	int	m_ButtonID;
};

#define CreateButtonEvent(DeviceType,EventAction,ButtonID)	new InputButtonEvent(DeviceType,EventAction,ButtonID)
#define CreateXYEvent(x,y)							new MouseXYEvent(IInputDevice::MOUSE,x,y)

#endif // #ifndef _INPUTEVENT_H_
