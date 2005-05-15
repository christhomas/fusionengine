#include <Win32ClientSocket.h>
#include <Win32NetworkCore.h>

Win32ClientSocket::Win32ClientSocket(INetworkCore *network)
{
    m_network = network;
    m_send_packet = NULL;
    m_Connected = false;

    m_recv_packet = NULL;

    m_DataEvent = CreateEvent(NULL, false, false, NULL);
    m_SendEvent = CreateEvent(NULL, false, false, NULL);
    InitializeCriticalSection(&m_datastack_lock);
}

Win32ClientSocket::~Win32ClientSocket()
{
    delete m_send_packet;
    Disconnect();

    CloseHandle(m_DataEvent);
    CloseHandle(m_SendEvent);
    DeleteCriticalSection(&m_datastack_lock);
}

void Win32ClientSocket::Send(char *data, int length, bool wait)
{
    ClientSocket::Send(data,length,wait);
    
    if(wait == true){
		unsigned int result = WaitForSingleObject(m_SendEvent, INFINITE);
		ResetEvent(m_SendEvent);
    }
}

void Win32ClientSocket::SignalSend(void)
{
	SetEvent(m_SendEvent);
}

NetworkPacket * Win32ClientSocket::Receive(unsigned int milliseconds)
{
    delete m_recv_packet;
    m_recv_packet = NULL;

    unsigned int result;

    if (m_datastack.size() == 0){
		result = WaitForSingleObject(m_DataEvent, milliseconds);
    } else {
		result = WAIT_OBJECT_0;
    }

    if (result != WAIT_TIMEOUT) {
		LockDataStack();
		{
			m_recv_packet = m_datastack[0];
			m_datastack.erase(m_datastack.begin());
		}
		UnlockDataStack();
    }

    ResetEvent(m_DataEvent);

    return m_recv_packet;
}

void Win32ClientSocket::socketReceive(void)
{
	//	Receive data from network
	NetworkPacket *packet = new NetworkPacket;
	packet->socket = m_socket;
	memset(packet->data, 0, MAX_RECV);

	packet->length = recv(m_socket, packet->data, MAX_RECV, 0);

	if (packet->length > 0) {
		//	FIXME:	Do I need to lock sockets here? or around the recv() call?
		//			or perhaps neither?
		LockDataStack();
		{
			m_datastack.push_back(packet);
			SetEvent(m_DataEvent);
		}
		UnlockDataStack();
	}
}

/*
*	CRITICAL SECTION METHODS
*/
void Win32ClientSocket::LockDataStack(void)
{
    EnterCriticalSection(&m_datastack_lock);
}

void Win32ClientSocket::UnlockDataStack(void)
{
    LeaveCriticalSection(&m_datastack_lock);
}
