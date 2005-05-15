#include <PTClientSocket.h>
#include <PTHelper.h>

PTClientSocket::PTClientSocket(INetworkCore *network)
{
    m_network = network;
    m_send_packet = NULL;
    m_Connected = false;

    m_recv_packet = NULL;
    
	pthread_mutex_init(&m_datastack_lock, NULL);
	
	m_DataEvent = new Event(false);
	m_SendEvent = new Event(false);    
}

PTClientSocket::~PTClientSocket()
{
    delete m_send_packet;
    Disconnect();

    pthread_mutex_destroy(&m_datastack_lock);
    
    delete m_DataEvent;
    delete m_SendEvent;
}

void PTClientSocket::SignalSend(void)
{
	m_SendEvent->signal();
}

void PTClientSocket::Send(char *data, int length, bool wait)
{
    ClientSocket::Send(data,length,wait);
    
    if(wait == true) m_SendEvent->wait();
}

NetworkPacket * PTClientSocket::Receive(unsigned int milliseconds)
{
    delete m_recv_packet;
    m_recv_packet = NULL;

	if(m_DataEvent->timedwait(milliseconds) != ETIMEDOUT){		
	    if (m_datastack.size() > 0){
			LockDataStack();
			{
				m_recv_packet = m_datastack[0];
				m_datastack.erase(m_datastack.begin());
			}
			UnlockDataStack();
		}		
    }

    return m_recv_packet;
}

void PTClientSocket::socketReceive(void)
{
	//	Receive data from network
	NetworkPacket *packet = new NetworkPacket;
	packet->socket = m_socket;
	memset(packet->data, 0, MAX_RECV);

	packet->length = recv(packet->socket, packet->data, MAX_RECV, 0);
	
	if(packet->length > 0){
		LockDataStack();
		{
			m_datastack.push_back(packet);
		}
		UnlockDataStack();
		m_DataEvent->signal();
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
