#include <PTClientSocket.h>

PTClientSocket::PTClientSocket(INetworkCore *network)
{
    m_network = network;
    m_Connected = false;
    
	pthread_mutex_init(&m_datastack_lock, NULL);
	
	m_DataEvent = new Event();
	m_SendEvent = new Event();    
}

PTClientSocket::~PTClientSocket()
{
    pthread_mutex_destroy(&m_datastack_lock);
    
    delete m_DataEvent;
    delete m_SendEvent;
}

void PTClientSocket::SignalSend(void)
{
	m_SendEvent->signal();
}

void PTClientSocket::Send(const char *data, int length, bool wait)
{
    ClientSocket::Send(data,length,wait);
    
    if(wait == true) m_SendEvent->wait();
    SignalSend();	//	reset the event
}

NetworkPacket * PTClientSocket::Receive(unsigned int milliseconds)
{
    NetworkPacket *packet = NULL;
    
	if(m_DataEvent->timedwait(milliseconds) != ETIMEDOUT){		
		LockDataStack();
		{
		    if (m_datastack.size() > 0){
				packet = m_datastack[0];
				m_datastack.erase(m_datastack.begin());
			}
		}
		UnlockDataStack();
    }

    return packet;
}

void PTClientSocket::socketReceive(void)
{
	NetworkPacket *packet = new NetworkPacket;
	packet->socket = m_socket;
	memset(packet->data, 0, MAX_RECV);

	packet->length = recv(packet->socket, packet->data, MAX_RECV, 0);
	
	if(packet->length > 0){
		LockDataStack();
		{
			m_datastack.push_back(packet);
			m_DataEvent->signal();
		}
		UnlockDataStack();
	}
}

/*
*	CRITICAL SECTION METHODS
*/
void PTClientSocket::LockDataStack(void)
{
	pthread_mutex_lock(&m_datastack_lock);
}

void PTClientSocket::UnlockDataStack(void)
{
	pthread_mutex_unlock(&m_datastack_lock);
}
