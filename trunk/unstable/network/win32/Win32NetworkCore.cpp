#include <Win32NetworkCore.h>

//==================================
//      Socket Container class methods
//==================================
NetworkCore::NetworkCore()
{
    m_Thread = NULL;
    m_ThreadID = 0;
    m_SendEvent = CreateEvent(NULL, false, false, NULL);

    //      Trigger + Events to terminate the threads when object dtor is called
    m_destroy_threads = false;
    m_TerminateThread = CreateEvent(NULL, false, true, NULL);

    //      Initialise all Critical sections
    InitializeCriticalSection(&m_sockets_lock);
    InitializeCriticalSection(&m_senddata_lock);
}

NetworkCore::~NetworkCore()
{
    //      trigger the threads to quit
    m_destroy_threads = true;

    //      Resumes all threads so you can kill them
    ResumeThread(m_Thread);

    //      Wait until all the threads exit
    WaitForSingleObject(m_TerminateThread, INFINITE);

    //==============================
    //      Close all the event handles
    //==============================

    //      Close the send/recv event signals
    CloseHandle(m_SendEvent);

    //      Close the thread termination events
    CloseHandle(m_TerminateThread);

    //      Close the thread handles
    CloseHandle(m_Thread);

    //===================================
    //      Delete all the critical sections
    //===================================
    DeleteCriticalSection(&m_sockets_lock);
    DeleteCriticalSection(&m_senddata_lock);

    //===================================
    //      Clear the send data stack
    //===================================
    m_senddata.clear();

    //===================================
    //      Clear the socket queue
    //===================================
    m_sockets.clear();

    //===================================
    //      Close the WSA Networking system
    //===================================
    WSACleanup();
}

bool NetworkCore::Initialise(void)
{
    if (WSAStartup(WINSOCK_VERSION, &m_WSAData) == 0) {
	return true;
    }
    // failure
    return false;
}

IClientSocket * NetworkCore::CreateSocket(void)
{
    return new Socket(this);
}

IServerSocket * NetworkCore::CreateServerSocket(void)
{
    return new ServerSocket(this);
}

unsigned int NetworkCore::ResolveHost(char *ip)
{
    // for converting dns names to ip's
    char buffer[80];
    memset(buffer, 0, 80);
    hostent *host;

    // for storing the final ip address
    unsigned int address = -1;

    // convert the string to an ip
    address = inet_addr(ip);

    //      if the conversion didnt work, it's cause it has to be resolved first
    //      so this test will succeed if the address needs to be resolved
    if (address == -1) {
	// conver that hostname into a hostent structure
	host = gethostbyname(ip);

	if (host != NULL) {
	    // extract the ip address from the hostent structure
	    address = (*((in_addr *) host->h_addr)).s_addr;
	}
    }
    //      if the address was a valid ip string, it'll return a correct address
    //      if it's not, it'll attempt to resolve it, if that succeeds, it'll produce a valid ip address
    //      if both of those fail, it'll return -1, to signal error
    return address;
}

void NetworkCore::AddSocket(ISocket *socket, int events)
{
    //      Lock/Unlock the server sockets
    LockSockets();
    {
		//      Store the socket and create the events it requested
		m_sockets.push_back(socket);
		m_network_events.AddEvent(socket, events);
    }
    UnlockSockets();

    //      If this is the first socket in the list, then either/or the network core's thread has
    //      a)      not been created yet
    //      b)      been suspended and need resuming
    if (m_sockets.size() == 1) {
		if (m_Thread == NULL) {
			//      Create the network core thread
			m_Thread = CreateThread(NULL, 0, NetworkCoreThread, this, 0,&m_ThreadID);
		} else {
			//      Resume the network core thread
			ResumeThread(m_Thread);
		}
    }
}

bool NetworkCore::RemoveSocket(ISocket * socket)
{
    bool success = false;

    //      Lock/Unlock the client sockets
    LockSockets();
    {
		//      Loop through the list of available sockets until you can match the ptr
		for (unsigned int a = 0; a < m_sockets.size(); a++) {
			//      If you find the ptr
			if (m_sockets[a] == socket) {
				//      erase the socket from the list and remove it's event
				m_network_events.RemoveEvent(m_sockets[a]);
				m_sockets.erase(m_sockets.begin() + a);
				success = true;
				break;
			}
		}
    }				//     Remember you've locked the sockets, so you have to unlock it here, or trouble ensues
    UnlockSockets();

    //      return whether you were successful of not
    return success;
}

void NetworkCore::Send(NetworkPacket * packet)
{
    // Lock/Unlock the send data stack
    LockSendStack();
    {
		// Put the data onto the send stack
		m_senddata.push_back(packet);
    }
    UnlockSendStack();

    // Set the sending data event
    WSASetEvent(m_network_events.m_events[0]);
}

//====================================
//      Critical Section Locks/Unlocks
//====================================
void NetworkCore::LockSockets(void)
{
    EnterCriticalSection(&m_sockets_lock);
};
void NetworkCore::UnlockSockets(void)
{
    LeaveCriticalSection(&m_sockets_lock);
};
void NetworkCore::LockSendStack(void)
{
    EnterCriticalSection(&m_senddata_lock);
};
void NetworkCore::UnlockSendStack(void)
{
    LeaveCriticalSection(&m_senddata_lock);
};

//==================================
//      Network Thread functions
//==================================
DWORD WINAPI NetworkCoreThread(void *data)
{
    //      Generic Thread variables
    NetworkCore *network = (NetworkCore *)data;
    WSANETWORKEVENTS network_event;
    NetworkPacket *send_packet = NULL;

    ResetEvent(network->m_TerminateThread);

    while (true) {
		if (network->m_destroy_threads == true) {
			SetEvent(network->m_TerminateThread);
			ExitThread(0);
		}

		if (network->m_sockets.size() == 0) {
			SuspendThread(network->m_Thread);
		}

		int event_id;
		if ((event_id = WSAWaitForMultipleEvents(network->m_sockets.size() + 1,	//      The number of sockets to wait on
											network->m_network_events.m_events,	//      the events your looking for
											false,								//      dont wait until ALL have occured
											INFINITE,							//      timeout value for the wait state
											false))								//      whether I should alert anybody :P (IO Completion ports?)
											!= WSA_WAIT_TIMEOUT){
			if (event_id != 0) {
				ISocket *socket = network->m_network_events.m_sockets[event_id];
				
				WSAEnumNetworkEvents(socket->m_socket, network->m_network_events. m_events[event_id], &network_event);
				if (network_event.lNetworkEvents & FD_ACCEPT) {
					//      Process server sockets
					IServerSocket *server = (IServerSocket *)socket;

					IClientSocket *client = new Socket(network);
					int sockaddr_len = sizeof(client->m_socket_info);

					client->m_socket = accept(server->m_socket,(sockaddr *) & client->m_socket_info,&sockaddr_len);
					client->SetConnected(true);

					server->AddConnection(client);
					network->AddSocket(client, FD_READ);
					SetEvent(server->m_ConnectionEvent);
				} else if (network_event.lNetworkEvents & FD_READ) {
					//      Process client sockets
					IClientSocket *client = (IClientSocket *)socket;

					//      Receive data from network
					NetworkPacket *packet = new NetworkPacket;
					packet->socket = client->m_socket;
					memset(packet->data, 0, MAX_RECV);

					int bufferlen = recv(client->m_socket, packet->data, MAX_RECV, 0);

					if (bufferlen > 0) {
						packet->length = bufferlen;

						network->LockSockets();
						{
							client->AddDataPacket(packet);
						}
						network->UnlockSockets();
					}
				}
			}else{
				//      Send data across network
				while (network->m_senddata.size() > 0) {
					delete send_packet;

					//      lock senddata stack
					network->LockSendStack();
					{
						send_packet = network->m_senddata[0];
						network->m_senddata.erase(network->m_senddata.begin());
					}
					network->UnlockSendStack();
					//      unlock senddata stack

					int bytes_sent = 0;
					int offset = 0;

					while (send_packet->length > 0) {
						bytes_sent = send(send_packet->socket,&send_packet->data[offset],send_packet->length, 0);

						if (bytes_sent > 0) {
							offset += bytes_sent;
							send_packet->length -= bytes_sent;
						}
					}
				}
				
				WSAResetEvent(network->m_network_events.m_events[0]);
				
			}	//     if(event_id!=0)'s else statement
		}		//     WSAWaitForMultipleEvents
    }			//     While loop
}

//==================================
//      Socket class methods
//==================================

Socket::Socket(INetworkCore *network)
{
    m_network = network;
    m_timeout = INFINITE;
    m_send_packet = NULL;
    m_Connected = false;
    m_NumberPackets = 0;

    m_recv_packet = NULL;

    m_DataEvent = CreateEvent(NULL, false, false, NULL);
    InitializeCriticalSection(&m_datastack_lock);
}

Socket::~Socket()
{
    delete m_send_packet;
    Disconnect();

    CloseHandle(m_DataEvent);
    DeleteCriticalSection(&m_datastack_lock);
}

bool Socket::Connect(char *ip, int port)
{
    if (m_Connected == false) {
		if ((m_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
			return m_Connected;
		}

		m_socket_info.sin_addr.s_addr = m_network->ResolveHost(ip);
		m_socket_info.sin_family = AF_INET;
		m_socket_info.sin_port = htons(port);
		memset(&m_socket_info.sin_zero, 0, 8);

		if (connect(m_socket, (sockaddr *) & m_socket_info,sizeof(sockaddr)) < 0) {
			Disconnect();
			return m_Connected;
		}

		m_network->AddSocket(this, FD_READ);
		m_Connected = true;
    }

    return m_Connected;
}

void Socket::Disconnect(void)
{
    if (m_Connected == true) {
		if (closesocket(m_socket) == SOCKET_ERROR) {
			return;
		}
		m_network->RemoveSocket(this);
		m_Connected = false;
    }
}

bool Socket::Connected(void)
{
    return m_Connected;
}

void Socket::SetConnected(bool status)
{
    m_Connected = status;
}

void Socket::Send(char *data, int length)
{
    while (length > 0) {
		m_send_packet = new NetworkPacket;

		if (length > MAX_SEND) {
			m_send_packet->length = MAX_SEND;
		} else {
			m_send_packet->length = length;
		}

		length -= m_send_packet->length;

		m_send_packet->socket = m_socket;

		memcpy(m_send_packet->data, data, m_send_packet->length);

		m_network->Send(m_send_packet);
    }
}

void Socket::LockDataStack(void)
{
    EnterCriticalSection(&m_datastack_lock);
}
void Socket::UnlockDataStack(void)
{
    LeaveCriticalSection(&m_datastack_lock);
}

void Socket::AddDataPacket(NetworkPacket * packet)
{
    LockDataStack();
    {
		m_datastack.push_back(packet);
		m_NumberPackets++;
		SetEvent(m_DataEvent);
    }
    UnlockDataStack();
}

NetworkPacket *Socket::Receive(int milliseconds)
{
    delete m_recv_packet;

    unsigned int result;

    if (m_NumberPackets == 0) {
		result = WaitForSingleObject(m_DataEvent, milliseconds);
    } else {
		result = WAIT_OBJECT_0;
    }

    if (result != WAIT_TIMEOUT) {
		LockDataStack();
		{
			m_recv_packet = m_datastack[0];
			m_datastack.erase(m_datastack.begin());
			m_NumberPackets--;
		}
		UnlockDataStack();
    }

    ResetEvent(m_DataEvent);

    return m_recv_packet;
}

unsigned int Socket::GetIP(void)
{
    return (unsigned int) m_socket_info.sin_addr.s_addr;
}

unsigned int Socket::GetPort(void)
{
    return m_socket_info.sin_port;
}

//==================================
//      ServerSocket class methods
//==================================
ServerSocket::ServerSocket(INetworkCore *network)
{
	m_network = network;
	m_Connected = false;
    m_ConnectionEvent = CreateEvent(NULL, false, false, NULL);
    InitializeCriticalSection(&m_Connections_lock);
}

ServerSocket::~ServerSocket()
{
    Disconnect();
    CloseHandle(m_ConnectionEvent);
    DeleteCriticalSection(&m_Connections_lock);
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

		m_Connections.clear();
    }
}

socketlist_t & ServerSocket::GetConnections(void)
{
    return m_Connections;
}

void ServerSocket::AddConnection(ISocket * socket)
{
    if (socket != NULL) {
		LockConnections();
		{
			m_Connections.push_back(socket);
		}
		UnlockConnections();
    }
}

void ServerSocket::RemoveConnection(ISocket * socket)
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

unsigned int ServerSocket::NumberConnections(void)
{
    return (unsigned int)m_Connections.size();
}

bool ServerSocket::WaitForConnections(int milliseconds)
{
    if (WaitForSingleObject(m_ConnectionEvent, milliseconds) ==	WAIT_TIMEOUT) {
		return false;
    }
    
    return true;
}

unsigned int ServerSocket::GetIP(void)
{
    return -1;
}

void ServerSocket::LockConnections(void)
{
    EnterCriticalSection(&m_Connections_lock);
}
void ServerSocket::UnlockConnections(void)
{
    LeaveCriticalSection(&m_Connections_lock);
}
