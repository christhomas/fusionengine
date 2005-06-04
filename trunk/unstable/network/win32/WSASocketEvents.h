#ifndef _WIN32SOCKETEVENTS_H_
#define _WIN32SOCKETEVENTS_H_

#include <winsock2.h>

class ISocket;

class WSASocketEvents {
private:
	unsigned int m_numevents;
	unsigned int m_numtrigger;
	unsigned int m_free_places;
	unsigned int m_cachesize;
	unsigned int m_triggersize;
public:
	WSAEVENT * m_events;
	ISocket **m_sockets;
	
	enum{ SEND_TRIGGER=0, QUIT_TRIGGER };

	inline WSASocketEvents()
	{
		SetCacheSize();
		m_free_places = 10;
		m_numevents = 2; // start with 2, send and quit triggers

		m_events = (WSAEVENT *) malloc( (m_cachesize + m_numevents) * sizeof(WSAEVENT));
		m_sockets = (ISocket **) malloc( (m_cachesize + m_numevents) * sizeof(ISocket *));

		//	This is your send trigger event, when you want to send data across the network
		//	call WSASetEvent(m_events[0]) and the NetworkCore thread will pick it up and interpret
		//	it as a send event
		m_events[SEND_TRIGGER] = WSACreateEvent();
		m_sockets[SEND_TRIGGER] = NULL;
		
		//	This is your quit trigger event, when you want to quit the network thread
		//	call WSASetEvent(m_events[1]) and the NetworkCore thread will pick it up and interpret
		//	it as a quit event
		m_events[QUIT_TRIGGER] = WSACreateEvent();
		m_sockets[QUIT_TRIGGER] = NULL;
	} 

	inline ~WSASocketEvents()
	{		
		free(m_events);
		free(m_sockets);
	}

	inline void SetCacheSize(int cachesize = 10, int triggersize = 5)
	{
		m_cachesize = cachesize;
		m_triggersize = triggersize;
	}

	inline void AddEvent(ISocket *socket, unsigned int event)
	{
		if (m_free_places <= m_triggersize) {
			m_events = (WSAEVENT *) realloc(m_events,(m_cachesize+m_numevents) * sizeof(WSAEVENT));
			m_sockets =	(ISocket **) realloc(m_sockets,(m_cachesize+m_numevents) * sizeof(ISocket *));
			m_free_places += m_cachesize;
		}

		m_events[m_numevents] = WSACreateEvent();
		m_sockets[m_numevents] = socket;

		WSAEventSelect(socket->m_socket, m_events[m_numevents], event);
		
		m_numevents++;
		m_free_places--;
	}

	inline void RemoveEvent(ISocket *socket)
	{
		//	FIXME: can this be changed to take a reference (which cannot be NULL?)
		
		//	Make sure the socket entered, is NOT null cause it can confuse the triggers, 
		//	which are events with have a NULL socket, so if you pass null into here, it 
		//	might compare it against a trigger and find a false positive
		if(socket == NULL) return;
		
		for (unsigned int a = 0; a < m_numevents; a++) {
			if (m_sockets[a] == socket) {
				//WSAEventSelect(socket->m_socket,m_events[a],0);
				WSACloseEvent(m_events[a]);
				//while (a < m_numevents-1) {
				while(a < m_numevents){
					m_events[a] = m_events[a + 1];
					m_sockets[a] = m_sockets[a + 1];

					a++;
				}
				//m_events[a]		= 0;
				//m_sockets[a]	= NULL;

				m_numevents--;
				m_free_places++;

				return;
			}
		}
	}
	
	inline unsigned int numEvents(void)
	{
		return m_numevents;
	}
	
	inline WSAEVENT * getEvents(void)
	{
		return m_events;
	}
	
	inline void SetSendEvent(void)
	{
	    // Set the sending data event
		WSASetEvent(m_events[SEND_TRIGGER]);
	}
	
	inline void ResetSendEvent(void)
	{
		WSAResetEvent(m_events[SEND_TRIGGER]);
	}
	
	inline void SetQuitEvent(void)
	{
		// Set the quit thread event (to unblock WSAWaitForMultipleEvents() )
		WSASetEvent(m_events[QUIT_TRIGGER]);
	}
};

#endif // #ifndef _WIN32SOCKETEVENTS_H_