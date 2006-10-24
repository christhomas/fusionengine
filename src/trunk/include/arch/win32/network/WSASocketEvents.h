#ifndef _WIN32SOCKETEVENTS_H_
#define _WIN32SOCKETEVENTS_H_

#include <network/INetwork.h>
#include <network/ISocketEvents.h>

class WSASocketEvents: public ISocketEvents{
private:
	unsigned int m_numevents;
	unsigned int m_numtrigger;
	unsigned int m_free_places;
	unsigned int m_cachesize;
	unsigned int m_triggersize;
	
	WSAEVENT *m_events;
	socketmap_t m_socketmap;
	
	SMap *m_send,*m_break;
public:
	
	enum{ SEND_TRIGGER=0, BREAK_TRIGGER };

	WSASocketEvents();
	
	virtual ~WSASocketEvents();
	
	virtual void * getEvents(void);
	
	virtual void * getSockets(void);
	
	virtual SMap * addEvent(ISocket *socket, SOCKET s, int type);
	
	virtual bool removeEvent(ISocket *socket);
	
	virtual void clearEvents(void);
	
	virtual unsigned int numEvents(void);
	
	virtual unsigned int numSockets(void);
	
	virtual void output(void);
	
	virtual void setSendEvent(SMap *smap);
	
	virtual void resetSendEvent(void);
	
	virtual void setBreakEvent(void);
};

#endif // #ifndef _WIN32SOCKETEVENTS_H_