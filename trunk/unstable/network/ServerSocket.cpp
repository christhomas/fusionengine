#include <network/ServerSocket.h>

ServerSocket::ServerSocket(INetworkCore *network)
{
	m_network = network;
	m_Connected = false;
    
    m_connectEvent = new Event();
    
    pthread_mutex_init(&m_socketLock, NULL);  
}

ServerSocket::~ServerSocket()
{
	delete m_connectEvent; 
	pthread_mutex_destroy(&m_socketLock);   

	Disconnect();
}

void ServerSocket::Listen(int port, int backlog)
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
		
		if ((error = listen(m_socket, backlog)) < 0) {
			return;
		}
		
		m_network->AddSocket(this, FD_ACCEPT);
		m_Connected = true;
    }
}

void ServerSocket::Disconnect(void)
{
    if (m_Connected == true) {
		if (closesocket(m_socket) == SOCKET_ERROR) {
			return;
		}
		m_network->RemoveSocket(this);
		m_Connected = false;

		//	 Remove all the child client sockets
		for(unsigned int a=0;a<m_Connections.size();a++){
			delete m_Connections[a];
		}
		m_Connections.clear();
    }
}

socketlist_t & ServerSocket::GetConnections(void)
{
	/*	FIXME:	might be nice to know what sockets are new and what are old?
		SOLUTION:	could pass an integer pointer and make a null terminated array of ints
					representing the new connections?
		PROBLEM:	how do you identify "new connections?"
	*/
    return m_Connections;
}

void ServerSocket::AddConnection(ISocket * socket)
{
    if (socket != NULL) {
		pthread_mutex_lock(&m_socketLock);
		{
			m_Connections.push_back(socket);
		}
		pthread_mutex_unlock(&m_socketLock);
    }
}

void ServerSocket::RemoveConnection(ISocket * socket)
{
    if (socket != NULL) {
		pthread_mutex_lock(&m_socketLock);
		{
			for (socketlist_t::iterator s = m_Connections.begin();s != m_Connections.end(); s++) {
				if ((*s) == socket) {
					m_Connections.erase(s);
					delete socket;
					break;
				}
			}
		}
		pthread_mutex_unlock(&m_socketLock);
    }
}

void ServerSocket::SignalConnect(void)
{
	m_connectEvent->signal();
}

bool ServerSocket::WaitForConnections(unsigned int milliseconds)
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
	if(m_connectEvent->timedwait(milliseconds) == ETIMEDOUT) return false;
    
    return true;
}

unsigned int ServerSocket::NumberConnections(void)
{
    return (unsigned int)m_Connections.size();
}

unsigned int ServerSocket::GetIP(void)
{
    return -1;
}
