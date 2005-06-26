#ifndef _BSDSOCKETEVENTS_H_
#define _BSDSOCKETEVENTS_H_

#include <network/INetworkCore.h>

class BSDSocketEvents: public ISocketEvents{
protected:
	sockaddr_in m_socket_info;
	
	socketmap_t m_socketmap;
	fd_set m_events;
public:
	BSDSocketEvents();
	~BSDSocketEvents();
	
	virtual void * getEvents(void);
	virtual void * getSockets(void);
	
	virtual SMap * addEvent(ISocket *socket, SOCKET s, int type);
	virtual bool removeEvent(ISocket *socket);
	
	virtual void clearEvents(void);
	
	virtual unsigned int numEvents(void);
	
	virtual unsigned int numSockets(void);
	
	//	FIXME: Not sure how to do this yet
	virtual void setSendEvent(SMap *smap);
	virtual void resetSendEvent(void);
	virtual void setBreakEvent(void);
};

#endif // #ifndef _BSDSOCKETEVENTS_H_