#include <Win32NetworkCore.h>
#include <Win32ClientSocket.h>
#include <Win32ServerSocket.h>

//==================================
//	Socket Container class methods
//==================================
Win32NetworkCore::Win32NetworkCore(){}

Win32NetworkCore::~Win32NetworkCore()
{
    //	trigger the threads to quit
    m_destroy_threads = true;

    //===================================
    //      Clear the send data stack
    //===================================
    m_senddata.clear();
    
    //===================================
    //      Clear the socket queue
    //===================================
    while(m_sockets.size() > 0) RemoveSocket(m_sockets[0]);    
    	
	//	Trigger the quit event (if the thread is still running, it'll unblock)
	m_network_events.SetQuitEvent();

    //	Wait until all the threads exit
    WaitForSingleObject(m_TerminateThread, INFINITE);

    //==============================
    //	Close all the event handles
    //==============================

    //	Close the send/recv event signals
    CloseHandle(m_SendEvent);

    //	Close the thread termination events
    CloseHandle(m_TerminateThread);

    //	Close the thread handles
    CloseHandle(m_Thread);

    //===================================
    //	Delete all the critical sections
    //===================================
    DeleteCriticalSection(&m_sockets_lock);
    DeleteCriticalSection(&m_senddata_lock);

    //===================================
    //	Close the WSA Networking system
    //===================================
    WSACleanup();
}

bool Win32NetworkCore::Initialise(void)
{
    if (WSAStartup(WINSOCK_VERSION, &m_WSAData) == 0)
    {
		m_Thread = NULL;
		m_ThreadID = 0;
		m_SendEvent = CreateEvent(NULL, false, false, NULL);

		//	Trigger + Events to terminate the threads when object dtor is called
		m_destroy_threads = false;
		m_TerminateThread = CreateEvent(NULL, false, true, NULL);

		//	Initialise all Critical sections
		InitializeCriticalSection(&m_sockets_lock);
		InitializeCriticalSection(&m_senddata_lock);    

		return true;
    }
    // failure
    return false;
}

IClientSocket * Win32NetworkCore::CreateSocket(void)
{
    return new Win32ClientSocket(this);
}

IServerSocket * Win32NetworkCore::CreateServerSocket(void)
{
    return new Win32ServerSocket(this);
}

void Win32NetworkCore::AddSocket(ISocket *socket, int events)
{
    //	Lock/Unlock the server sockets
    LockSockets();
    {
		//	Store the socket and create the events it requested
		m_sockets.push_back(socket);
		m_network_events.AddEvent(socket, events);
    }
    UnlockSockets();

    //	If this is the first socket in the list, then either/or the network core's thread has
    //	a)	not been created yet
    //	b)	been suspended and need resuming
    if (m_sockets.size() == 1) {
		//	Create the network core thread
		m_Thread = CreateThread(NULL, 0, NetworkCoreThread, this, 0,&m_ThreadID);
    }
}

bool Win32NetworkCore::RemoveSocket(ISocket *socket)
{
    bool success = false;

    //	Lock/Unlock the client sockets
    LockSockets();
    {
		//	Loop through the list of available sockets until you can match the ptr
		for (unsigned int a = 0; a < m_sockets.size(); a++) {
			//	If you find the ptr
			if (m_sockets[a] == socket) {
				//	erase the socket from the list and remove it's event
				m_network_events.RemoveEvent(m_sockets[a]);
				m_sockets.erase(m_sockets.begin() + a);
				success = true;
				break;
			}
		}
    }	//	Remember you've locked the sockets, so you have to unlock it here, or trouble ensues
    UnlockSockets();

    //	return whether you were successful of not
    return success;
}

void Win32NetworkCore::Send(NetworkPacket *packet)
{
    //	Lock/Unlock the send data stack
    LockSendStack();
    {
		//	Put the data onto the send stack
		m_senddata.push_back(packet);
    }
    UnlockSendStack();
    
    m_network_events.SetSendEvent();
}

Win32SocketEvents * Win32NetworkCore::getSocketEvents(void)
{
	return &m_network_events;
}

NetworkPacket * Win32NetworkCore::getNetworkPacket(void)
{
	NetworkPacket *packet = NULL;
	LockSendStack();
	{
		if(m_senddata.size() > 0){
			packet = m_senddata[0];
			m_senddata.erase(m_senddata.begin());
		}
	}
	UnlockSendStack();
	
	return packet;
}	

/*
*	CRITICAL SECTION METHODS
*/
void Win32NetworkCore::LockSockets(void)
{
    EnterCriticalSection(&m_sockets_lock);
}

void Win32NetworkCore::UnlockSockets(void)
{
    LeaveCriticalSection(&m_sockets_lock);
}

void Win32NetworkCore::LockSendStack(void)
{
    EnterCriticalSection(&m_senddata_lock);
}

void Win32NetworkCore::UnlockSendStack(void)
{
    LeaveCriticalSection(&m_senddata_lock);
}

void Win32NetworkCore::startThread(void)
{
	ResetEvent(m_TerminateThread);
}

void Win32NetworkCore::killThread(void)
{
	if(m_destroy_threads == true || m_sockets.size() == 0){	
		SetEvent(m_TerminateThread);
		ExitThread(0);
	}
}

