#include <Win32ServerSocket.h>

Win32ServerSocket::Win32ServerSocket(INetworkCore *network)
{
	m_network = network;
	m_Connected = false;
    m_ConnectionEvent = CreateEvent(NULL, false, false, NULL);
    InitializeCriticalSection(&m_Connections_lock);
}

Win32ServerSocket::~Win32ServerSocket()
{
    Disconnect();
    CloseHandle(m_ConnectionEvent);
    DeleteCriticalSection(&m_Connections_lock);
}

void Win32ServerSocket::AddConnection(ISocket * socket)
{
    if (socket != NULL) {
		LockConnections();
		{
			m_Connections.push_back(socket);
		}
		UnlockConnections();
    }
}

void Win32ServerSocket::RemoveConnection(ISocket * socket)
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

void Win32ServerSocket::SignalConnect(void)
{
	SetEvent(m_ConnectionEvent);
}

bool Win32ServerSocket::WaitForConnections(unsigned int milliseconds)
{
    if (WaitForSingleObject(m_ConnectionEvent, milliseconds) ==	WAIT_TIMEOUT) {
		return false;
    }
    
    return true;
}

/*
*	CRITICAL SECTION METHODS
*/
void Win32ServerSocket::LockConnections(void)
{
    EnterCriticalSection(&m_Connections_lock);
}
void Win32ServerSocket::UnlockConnections(void)
{
    LeaveCriticalSection(&m_Connections_lock);
}
