#include <network/BSDSocketEvents.h>

BSDSocketEvents::BSDSocketEvents()
{
	m_socket_info.sin_family = AF_INET;
	m_socket_info.sin_addr.s_addr = inet_addr("127.0.0.1");
	m_socket_info.sin_port = htons(0);
	memset(&m_socket_info.sin_zero, 0, 8);	
	
	FD_ZERO(&m_events);
}

BSDSocketEvents::~BSDSocketEvents()
{
	clearEvents();
}

void * BSDSocketEvents::getEvents(void)
{
	return &m_events;
}

void * BSDSocketEvents::getSockets(void)
{
	return &m_socketmap;
}

SMap * BSDSocketEvents::addEvent(ISocket *socket, SOCKET s, int type)
{
	//	Create a SMap structure and fill it
	SMap *smap = new SMap;
	smap->socketobj = socket;
	smap->type = type;
	smap->sock = s;
	//	add to the std::map and set an event ready
	m_socketmap[s] = smap;
	FD_SET(s,&m_events);
	
	return smap;
}

bool BSDSocketEvents::removeEvent(ISocket *socket)
{
//	Loop through the list of available sockets until you can match the ptr
	for(socketmap_t::iterator i = m_socketmap.begin(); i != m_socketmap.end(); i++){
		//	If you find the ptr
		SMap *smap = (*i).second;
		if (smap->socketobj == socket){
			//	Delete the smap, remove the socket from 
			//	the fd_set and erase the element from the std::map
			delete smap;
			FD_CLR((*i).first,&m_events);
			m_socketmap.erase(i);
			
			return true;
		}
	}
	
	return false;
}

void BSDSocketEvents::clearEvents(void)
{
	for(socketmap_t::iterator i = m_socketmap.begin(); i != m_socketmap.end(); i++){
		SMap *smap = (*i).second;
		
		//	Delete the smap, remote the socket from 
		//	the fd_set and erase the element from the std::map
		delete smap;
		FD_CLR((*i).first,&m_events);
		m_socketmap.erase(i);
	}
}

unsigned int BSDSocketEvents::numEvents(void)
{
	return m_socketmap.size();
}

unsigned int BSDSocketEvents::numSockets(void)
{
	return m_socketmap.size();
}

void BSDSocketEvents::setSendEvent(SMap *smap)
{
	//	Just send the first byte of the variable SOCKET, so the port (enough to trigger it)
	//	REASONING:	Who cares what data you send, you need to send 1 byte, thats it, just use the variable
	//				you are given in this function, then you dont have to create a temp variable to use instead
	send(smap->sock,(char *)&smap,1,0);
	
	//::connect(s,(sockaddr *)&m_socket_info,sizeof(sockaddr));
}

void BSDSocketEvents::resetSendEvent(void)
{
	//	in BSD sockets, you dont reset it, just ignore this
}

void BSDSocketEvents::setBreakEvent(void)
{
	//	do I need to do anything? or will select drop out when all the sockets are closed
	//	Perhaps this object needs to hold a socket in the system too, so it can signal quit through it?
}