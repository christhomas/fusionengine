#include <network/ClientSocket.h>

ClientSocket::ClientSocket(){}

ClientSocket::~ClientSocket()
{
	Disconnect();	
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

		if (connect(m_socket, (sockaddr *) & m_socket_info,sizeof(sockaddr)) < 0) {
			Disconnect();
			return m_Connected;
		}

		m_network->AddSocket(this, FD_READ);
		m_Connected = true;
    }

    return m_Connected;
}

void ClientSocket::Connect(unsigned int socket)
{
	int socklen = sizeof(sockaddr_in);
	m_socket = accept(socket, (sockaddr *)&m_socket_info,&socklen);
	SetConnected(true);	
}

void ClientSocket::Disconnect(void)
{
    if (m_Connected == true) {
		if (closesocket(m_socket) == SOCKET_ERROR) {
			return;
		}
		
		m_network->RemoveSocket(this);
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

void ClientSocket::Send(const char *data, int length, bool wait)
{
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
}

unsigned int ClientSocket::GetIP(void)
{
    return (unsigned int) m_socket_info.sin_addr.s_addr;
}

unsigned int ClientSocket::GetPort(void)
{
    return m_socket_info.sin_port;
}