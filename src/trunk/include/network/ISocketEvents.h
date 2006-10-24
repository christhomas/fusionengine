#ifndef _ISOCKETEVENTS_H_
#define _ISOCKETEVENTS_H_

class ISocket;
struct SMap;

class ISocketEvents{
public:
	ISocketEvents(){};
	virtual ~ISocketEvents(){};
	
	virtual void * getEvents(void) = 0;
	
	virtual void * getSockets(void) = 0;
	
	virtual SMap * addEvent(ISocket *socket, SOCKET s, int type) = 0;
	
	virtual bool removeEvent(ISocket *socket) = 0;
	
	//	FIXME: Protected method?
	virtual void clearEvents(void) = 0;
	
	virtual unsigned int numEvents(void) = 0;
	
	virtual unsigned int numSockets(void) = 0;
	
	virtual void setSendEvent(SMap *smap) = 0;
	
	virtual void resetSendEvent(void) = 0;
	
	virtual void setBreakEvent(void) = 0;
};

#endif // #ifndef _ISOCKETEVENTS_H_