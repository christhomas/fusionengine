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
		return NetworkCore::Initialise();
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
	NetworkCore::Send(packet);
	
	m_network_events.SetSendEvent();
}

WSASocketEvents * WSANetworkCore::getSocketEvents(void)
{
	return &m_network_events;
}

//	 FIXME:	This method means I am tying the network system to fusion, whereas before, 
//			I could use it, without using fusion, future policy decision?
#include <fusion.h>
void WSANetworkCore::error(void)
{
	int error = WSAGetLastError();
	
	switch(error){
		case WSANOTINITIALISED:{ fusion->errlog << "A successful WSAStartup call must occur before using this function." << std::endl; }break;
		case WSAENETDOWN:{ fusion->errlog << "The network subsystem has failed." << std::endl; }break;
		case WSAEADDRINUSE:{ fusion->errlog << "The socket's local address is already in use and the socket was not marked to allow address reuse with SO_REUSEADDR. This error usually occurs when executing bind, but could be delayed until this function if the bind was to a partially wildcard address (involving ADDR_ANY) and if a specific address needs to be committed at the time of this function." << std::endl; }break;
		case WSAEINTR:{ fusion->errlog << "The blocking Windows Socket 1.1 call was canceled through WSACancelBlockingCall." << std::endl; }break;
		case WSAEINPROGRESS:{ fusion->errlog << "A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function." << std::endl; }break;
		case WSAEALREADY:{ fusion->errlog << "A nonblocking connect call is in progress on the specified socket.\nNote  In order to preserve backward compatibility, this error is reported as WSAEINVAL to Windows Sockets 1.1 applications that link to either Winsock.dll or Wsock32.dll." << std::endl; }break;
		case WSAEADDRNOTAVAIL:{ fusion->errlog << "The remote address is not a valid address (such as ADDR_ANY)." << std::endl; }break;
		case WSAEAFNOSUPPORT:{ fusion->errlog << "Addresses in the specified family cannot be used with this socket." << std::endl; }break;
		case WSAECONNREFUSED:{ fusion->errlog << "The attempt to connect was forcefully rejected." << std::endl; }break;
		case WSAEFAULT:{ fusion->errlog << "The name or the namelen parameter is not a valid part of the user address space, the namelen parameter is too small, or the name parameter contains incorrect address format for the associated address family." << std::endl; }break;
		case WSAEINVAL:{ fusion->errlog << "The parameter s is a listening socket." << std::endl; }break;
		case WSAEISCONN:{ fusion->errlog << "The socket is already connected (connection-oriented sockets only)." << std::endl; }break;
		case WSAENETUNREACH:{ fusion->errlog << "The network cannot be reached from this host at this time." << std::endl; }break;
		case WSAENOBUFS:{ fusion->errlog << "No buffer space is available. The socket cannot be connected." << std::endl; }break;
		case WSAENOTSOCK:{ fusion->errlog << "The descriptor is not a socket." << std::endl; }break;
		case WSAETIMEDOUT:{ fusion->errlog << "Attempt to connect timed out without establishing a connection." << std::endl; }break;
		case WSAEWOULDBLOCK:{ fusion->errlog << "The socket is marked as nonblocking and the connection cannot be completed immediately." << std::endl; }break;
		case WSAEACCES:{ fusion->errlog << "Attempt to connect datagram socket to broadcast address failed because setsockopt option SO_BROADCAST is not enabled." << std::endl; }break;		
	};
}
