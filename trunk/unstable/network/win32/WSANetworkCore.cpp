#include <WSANetworkCore.h>

// The thread function the win32 layer uses
void * PTNetworkThread(void *data);

//==================================
//	Socket Container class methods
//==================================
WSANetworkCore::WSANetworkCore()
{
}

WSANetworkCore::~WSANetworkCore()
{
	//	Remove all the sockets
	while(m_sockets.size() > 0) RemoveSocket(m_sockets[0]);
	
	//	Trigger the quit event (if the thread is still running, it'll unblock)
	m_network_events.SetQuitEvent();
	
    //===================================
    //	Close the WSA Networking system
    //===================================
    WSACleanup();	
}

bool WSANetworkCore::Initialise(void)
{
    if (WSAStartup(WINSOCK_VERSION, &m_WSAData) == 0)
    {	
		return PTNetworkCore::Initialise();
    }
    // failure
    return false;
}

void WSANetworkCore::AddSocket(ISocket *socket, int events)
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
		m_ThreadID = pthread_create(&m_Thread, NULL, PTNetworkThread, this);
    }
}

bool WSANetworkCore::RemoveSocket(ISocket *socket)
{
    bool success = false;

    //	Lock/Unlock the client sockets
    LockSockets();
    {
		//	Loop through the list of available sockets until you can match the ptr
		for (unsigned int a = 0; a < m_sockets.size(); a++){
			//	If you find the ptr
			if (m_sockets[a] == socket){
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

void WSANetworkCore::Send(NetworkPacket *packet)
{
	PTNetworkCore::Send(packet);
	
	m_network_events.SetSendEvent();
}

WSASocketEvents * WSANetworkCore::getSocketEvents(void)
{
	return &m_network_events;
}
