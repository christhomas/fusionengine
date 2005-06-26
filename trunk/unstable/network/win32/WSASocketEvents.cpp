#include <network/WSASocketEvents.h>

WSASocketEvents::WSASocketEvents()
{
	//	FIXME: There is no error detection here, so if this fails, you can't possibly know
	WSADATA WSAData;	//	Dont actually use this, but have to supply it anyway
	WSAStartup(WINSOCK_VERSION, &WSAData);
	
	m_cachesize = 10;
	m_triggersize = 5;
	m_free_places = 10;
	m_numevents = 0;

	m_events = (WSAEVENT *) malloc( (m_cachesize + m_numevents) * sizeof(WSAEVENT));

	m_send	= addEvent(NULL,0,-1);
	m_break	= addEvent(NULL,0,-1);
}

WSASocketEvents::~WSASocketEvents()
{
	clearEvents();
	
	WSACleanup();	
}

void * WSASocketEvents::getEvents(void)
{
	return m_events;
}

void * WSASocketEvents::getSockets(void)
{
	return &m_socketmap;
}

SMap * WSASocketEvents::addEvent(ISocket *socket, SOCKET s, int type)
{
	if (m_free_places <= m_triggersize) {
		m_events = (WSAEVENT *) realloc(m_events,(m_cachesize+m_numevents) * sizeof(WSAEVENT));
		m_free_places += m_cachesize;
	}

	WSAEVENT e = WSACreateEvent();
	m_events[m_numevents] = e;
	
	SMap *smap = new SMap;
	smap->socketobj = socket;
	smap->sock = s;
	m_socketmap[(unsigned int)e] = smap;

	if(type >= 0) WSAEventSelect(s, e, type);
		
	m_numevents++;
	m_free_places--;
	
	return smap;
}

bool WSASocketEvents::removeEvent(ISocket *socket)
{
	//	Make sure the socket entered, is NOT null cause it can confuse the triggers, 
	//	which are events with have a NULL socket, so if you pass null into here, it 
	//	might compare it against a trigger and find a false positive
	if(socket != NULL){
		unsigned int a=0;
		for(socketmap_t::iterator i = m_socketmap.begin(); i != m_socketmap.end(); i++, a++){
			SMap *smap = (*i).second;
			if(socket == smap->socketobj){
				//	Close the event, delete the SMap, erase the element from the map
				WSACloseEvent(m_events[a]);
				delete smap;
				m_socketmap.erase(i);
				
				while(a < m_numevents){
					m_events[a] = m_events[a+1];
					a++;
				}
				
				m_numevents--;
				m_free_places++;
				
				return true;
			}
		}
	}
		
	return false;
}

void WSASocketEvents::clearEvents(void)
{
	//	FIXME: This doesnt really cleanup very well, do something better
	//	FIXME: Are you responsible for deleting sockets which are removed from the array/map?
	while(m_numevents > 0)
	free(m_events);
	m_socketmap.clear();
}

/*void WSASocketEvents::findEvent(ISocket *socket)
{
	if(socket != NULL){
		unsigned int a=0;
		for
	}
}*/

unsigned int WSASocketEvents::numEvents(void)
{
	//	NOTE:	You subtract 2 from the count, because you have two trigger events
	//			which you shouldnt count, cause it'll make the thread start which
	//			is unnecessary until you have a REAL event to wait upon
	return m_numevents-2;
}

unsigned int WSASocketEvents::numSockets(void)
{
	//	NOTE:	This is the TRUE number of events, including the triggers
	return m_numevents;
}
#include <iostream>
void WSASocketEvents::output(void)
{
	std::cout << "socketmap = {" << std::endl;		
	for(unsigned int a=0;a<m_numevents;a++){
		socketmap_t::iterator i = m_socketmap.find((unsigned int)m_events[a]);
		SMap *smap = (*i).second;
		std::cout << "\t " << a << ": " << m_events[a] << ", socketobj = " << smap->socketobj << ", sock = " << smap->sock << std::endl;
	}
	std::cout << "}" << std::endl;
}

void WSASocketEvents::setSendEvent(SMap *smap)
{
	m_send->socketobj = smap->socketobj;
	m_send->sock = smap->sock;
		
	// Set the sending data event
	WSASetEvent(m_events[SEND_TRIGGER]);
}

void WSASocketEvents::resetSendEvent(void)
{
	m_send->socketobj = NULL;
	m_send->sock = 0;
	
	WSAResetEvent(m_events[SEND_TRIGGER]);
}

void WSASocketEvents::setBreakEvent(void)
{
	std::cout << "Send break event" << std::endl;
	// Set the break event (to unblock WSAWaitForMultipleEvents() )
	WSASetEvent(m_events[BREAK_TRIGGER]);
	//WSAResetEvent(m_events[BREAK_TRIGGER]);
}
