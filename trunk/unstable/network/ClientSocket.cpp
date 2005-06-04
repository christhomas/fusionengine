#include <network/ClientSocket.h>

ClientSocket::ClientSocket(INetworkCore *network)
{
    m_network = network;
    m_Connected = false;
    
	pthread_mutex_init(&m_recvStack_lock, NULL);
	pthread_mutex_init(&m_sendStack_lock, NULL);
	
	m_recvEvent = new Event();
	m_sendEvent = new Event(); 
}

ClientSocket::~ClientSocket()
{
	Disconnect();
    
    pthread_mutex_destroy(&m_recvStack_lock);
    pthread_mutex_destroy(&m_sendStack_lock);
    
    delete m_recvEvent;
    delete m_sendEvent;	
}

bool ClientSocket::Connect(const char *ip, int port)
{
    if (m_Connected == false) {
		if ((m_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
			return m_Connected;
		}

		m_socket_info.sin_addr.s_addr = m_network->ResolveHost(ip);
		m_socket_info.sin_family = AF_INET;
		m_socket_info.sin_port = htons(port);
		memset(&m_socket_info.sin_zero, 0, 8);

		if (connect(m_socket, (sockaddr *) & m_socket_info,sizeof(sockaddr)) == SOCKET_ERROR) {
			m_network->error();
			Disconnect();
			return m_Connected;
		}

		m_network->AddSocket(this, FD_READ);
		SetConnected(true);
    }

    return m_Connected;
}

void ClientSocket::Disconnect(void)
{
    if (m_Connected == true) {
		m_network->RemoveSocket(this);

		if (closesocket(m_socket) == SOCKET_ERROR) return;
	
		m_Connected = false;
    }
}

bool ClientSocket::Connected(void)
{
    return m_Connected;
}

void ClientSocket::SetConnected(bool status)
{
    m_Connected = status;
}

void ClientSocket::SignalSend(void)
{
	m_sendEvent->signal();
}

void ClientSocket::Send(const char *data, int length, bool wait)
{
	/*	FIXME:	Should a socket, have a sync/async property, 
				so you can set a socket to always wait, or 
				never wait for completion of sending data?
	*/
	unsigned int offset = 0;
	
    while (length > 0) {
		NetworkPacket *packet = new NetworkPacket;
		packet->socketobj = NULL;

		if (length > MAX_SEND) {
			packet->length = MAX_SEND;
		} else {
			packet->length = length;
			//	this is the last packet in the send block
			//	so set the socketobj, to notify the last packet has
			//	sent
			packet->socketobj = this;
		}

		length -= packet->length;

		packet->socket = m_socket;

		memset(packet->data,0,MAX_SEND);
		memcpy(packet->data, &data[offset], packet->length);

		m_network->Send(packet);
		
		offset+=length;
    }
    
    if(wait == true) m_sendEvent->wait();
    SignalSend();	//	reset the event    
}

NetworkPacket * ClientSocket::Receive(unsigned int milliseconds)
{
    NetworkPacket *packet = NULL;
    
	if(m_recvEvent->timedwait(milliseconds) != ETIMEDOUT){		
		pthread_mutex_lock(&m_recvStack_lock);
		{
			if (m_recvStack.size() > 0){
				packet = m_recvStack[0];
				m_recvStack.erase(m_recvStack.begin());
			}
		}
		pthread_mutex_unlock(&m_recvStack_lock);
    }

    return packet;
}

unsigned int ClientSocket::GetIP(void)
{
    return (unsigned int) m_socket_info.sin_addr.s_addr;
}

unsigned int ClientSocket::GetPort(void)
{
    return m_socket_info.sin_port;
}

void ClientSocket::threadConnect(unsigned int socket)
{
	if(m_Connected == false){
		int socklen = sizeof(sockaddr_in);
		m_socket = accept(socket, (sockaddr *)&m_socket_info,&socklen);
		
		m_network->AddSocket(this, FD_READ);
		SetConnected(true);	
	}
}

void ClientSocket::threadReceive(void)
{
	NetworkPacket *packet = new NetworkPacket;
	packet->socket = m_socket;
	memset(packet->data, 0, MAX_RECV);

	packet->length = recv(packet->socket, packet->data, MAX_RECV, 0);
	
	if(packet->length > 0){
		pthread_mutex_lock(&m_recvStack_lock);
		{
			m_recvStack.push_back(packet);
			m_recvEvent->signal();
		}
		pthread_mutex_unlock(&m_recvStack_lock);
	}
}
