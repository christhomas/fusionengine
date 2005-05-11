#ifndef _WIN32SOCKETEVENTS_H_
#define _WIN32SOCKETEVENTS_H_

#include <winsock2.h>

class ISocket;

class Win32SocketEvents {
private:
	unsigned int m_number_events;
	unsigned int m_free_places;
	unsigned int m_cachesize;
	unsigned int m_triggersize;
public:
	WSAEVENT * m_events;
	ISocket **m_sockets;

	inline Win32SocketEvents(){
		m_cachesize = 10;
		m_triggersize = 5;
		m_number_events = 0;
		m_free_places = 10;

		m_events = (WSAEVENT *) malloc(m_cachesize + 1 * sizeof(WSAEVENT));
		m_sockets = (ISocket **) malloc(m_cachesize + 1 * sizeof(ISocket *));

		//      This is your send trigger event, when you want to send data across the network
		//      call WSASetEvent(m_events[0]) and the NetworkCore thread will pick it up and interpret
		//      it as a send event
		m_events[0] = WSACreateEvent();
		m_sockets[0] = NULL;
	} 

	inline ~Win32SocketEvents(){
		free(m_events);
		free(m_sockets);
	}

	inline void SetCacheSize(int cachesize = 10, int triggersize = 5) {
		m_cachesize = cachesize;
		m_triggersize = triggersize;
	}

	inline void AddEvent(ISocket * socket, unsigned int event) {
		if (m_free_places <= m_triggersize) {
			m_events = (WSAEVENT *) realloc(m_events,(m_cachesize +m_number_events) * sizeof(WSAEVENT));
			m_sockets =	(ISocket **) realloc(m_sockets,(m_cachesize +m_number_events) * sizeof(ISocket *));
			m_free_places += m_cachesize;
		}

		m_number_events++;
		m_free_places--;

		m_events[m_number_events] = WSACreateEvent();
		m_sockets[m_number_events] = socket;

		WSAEventSelect(socket->m_socket, m_events[m_number_events], event);
	}

	inline void RemoveEvent(ISocket * socket) {
		for (unsigned int a = 0; a < m_number_events; a++) {
			if (m_sockets[a] == socket) {
				WSACloseEvent(m_events[a]);
				while (a < m_number_events) {
					m_events[a] = m_events[a + 1];
					m_sockets[a] = m_sockets[a + 1];

					a++;
				}

				m_number_events--;
				m_free_places++;

				return;
			}
		}
	}
};

#endif // #ifndef _WIN32SOCKETEVENTS_H_