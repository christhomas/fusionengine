#include <PTServerSocket.h>
#include <PTHelper.h>

PTServerSocket::PTServerSocket(INetworkCore *network)
{
	m_network = network;
	m_Connected = false;
    
    m_ConnectEvent = new Event(false);
    
    pthread_mutex_init(&m_SocketLock, NULL);  
}

PTServerSocket::~PTServerSocket()
{
    Disconnect();

	delete m_ConnectEvent; 
	pthread_mutex_destroy(&m_SocketLock);   
}

void PTServerSocket::AddConnection(ISocket * socket)
{
    if (socket != NULL) {
		LockConnections();
		{
			m_Connections.push_back(socket);
		}
		UnlockConnections();
    }
}

void PTServerSocket::RemoveConnection(ISocket * socket)
{
    if (socket != NULL) {
		LockConnections();
		{
			for (socketlist_t::iterator s = m_Connections.begin();s != m_Connections.end(); s++) {
				if ((*s) == socket) {
					m_Connections.erase(s);
					delete socket;
					break;
				}
			}
		}
		UnlockConnections();
    }
}

void PTServerSocket::SignalConnect(void)
{
	m_ConnectEvent->signal();
}

bool PTServerSocket::WaitForConnections(unsigned int milliseconds)
{
	if(m_ConnectEvent->timedwait(milliseconds) == ETIMEDOUT) return false;
    
    return true;
}

/*
*	CRITICAL SECTION METHODS
*/
void PTServerSocket::LockConnections(void)
{
    pthread_mutex_lock(&m_SocketLock);
}
void PTServerSocket::UnlockConnections(void)
{
	pthread_mutex_unlock(&m_SocketLock);
}
