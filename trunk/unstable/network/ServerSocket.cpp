#include <network/ServerSocket.h>

ServerSocket::ServerSocket(NetworkCore *network)
{
	m_network = network;
	m_events = m_network->getSocketEvents();
	m_Connected = false;
	m_ready = false;
    
    m_connectEvent = new Event();
    
    pthread_mutex_init(&m_socketLock, NULL);  
}

ServerSocket::~ServerSocket()
{
	disconnect();
	
	m_connectEvent->wait();
	
	delete m_connectEvent;
	pthread_mutex_destroy(&m_socketLock);   	
}

void ServerSocket::listen(int port, int backlog)
{
    if (m_Connected == false) {
		if ((m_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
			return;
		}

		memset(&m_socket_info, 0, sizeof(sockaddr));
		m_socket_info.sin_family = AF_INET;
		m_socket_info.sin_addr.s_addr = INADDR_ANY;
		m_socket_info.sin_port = htons(port);
		memset(&m_socket_info.sin_zero, 0, 8);

		m_port = port;
		
		unsigned int error;

		//	FIXME:	This code is horrible, it needs cleaning up
		//			no error checking, basically hacked together and I hate it
		char yes = 1;
		if ((error = setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &yes,sizeof(char))) < 0) {
			return;
		}
		if ((error = bind(m_socket, (sockaddr *) & m_socket_info,sizeof(sockaddr))) < 0) {
			return;
		}
		
		int len = sizeof(sockaddr);
		if ((error = getsockname(m_socket, (sockaddr *) & m_socket_info,&len)) < 0) {
			return;
		}
		
		if ((error = ::listen(m_socket, backlog)) < 0) {
			return;
		}
		
		if(m_ready == false){
			m_smap = m_network->addSocket(this, m_socket);
			m_ready = true;
		}
		
		m_Connected = true;
    }
}

void ServerSocket::disconnect(void)
{
    if (m_Connected == true) {
		if (closesocket(m_socket) == SOCKET_ERROR) {
			return;
		}
		
		m_Connected = false;
    }
}

socketlist_t & ServerSocket::getConnections(void)
{
	/*	FIXME:		might be nice to know what sockets are new and what are old?
		SOLUTION:	could pass an integer pointer and make a null terminated array of ints
					representing the new connections?
		PROBLEM:	how do you identify "new connections?"
	*/
    return m_Connections;
}

void ServerSocket::addConnection(ISocket *socket)
{
    if (socket != NULL) {
		pthread_mutex_lock(&m_socketLock);
		{
			m_Connections.push_back(socket);
		}
		pthread_mutex_unlock(&m_socketLock);
    }
}

SOCKET ServerSocket::threadAccept(void)
{
	int sockLen = sizeof(sockaddr_in);
	return ::accept(m_socket, NULL, &sockLen);
}

void ServerSocket::threadDisconnect(void)
{
	pthread_mutex_lock(&m_socketLock);
	{
		for (socketlist_t::iterator s = m_Connections.begin();s != m_Connections.end(); s++) {
			IClientSocket *client = (IClientSocket *)(*s);
			client->disconnect(true);
			m_Connections.erase(s);
		}
		
		m_network->removeSocket(this);
		m_ready = false;
	}
	pthread_mutex_unlock(&m_socketLock);	
}

void ServerSocket::emitSignal(void)
{
	m_connectEvent->signal();
}

bool ServerSocket::waitForConnections(unsigned int milliseconds)
{
	/*	FIXME:	If 100 connections attempt at once, the event will trigger 100 times
				but the timedwait, will only decrease it once for each timed wait call
				which means if 100 connections succeed, the next 99 attempts to find a connection
				will succeed, even if those connections have existed since the first event
				was signalled.  Which could cause problems.  Some of those connections
				might even disconnect in the time it takes to process them all like this
				
		SOLUTION:	to zero out the count each time you wait? in some situations I can imagine
					it being useful to have a count of how many triggers have happened
					but I can't imagine this being one of them
					
		FIXED ?
	*/
	
	//	Wait for "milliseconds" to expire waiting for an incoming connection
	//	To prevent waiting on a disconnected socket, m_ready is used, if this is false
	//	it means there is no chance this will succeed and returns false
	if(m_ready == false || m_connectEvent->timedwait(milliseconds) == ETIMEDOUT) return false;
    
    return true;
}

unsigned int ServerSocket::numConnections(void)
{
    return (unsigned int)m_Connections.size();
}

unsigned int ServerSocket::getPort(void)
{
	return m_port;
}
