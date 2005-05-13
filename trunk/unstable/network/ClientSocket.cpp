#include <network/ClientSocket.h>

ClientSocket::ClientSocket()
{

}

ClientSocket::~ClientSocket()
{

}

bool ClientSocket::Connect(char *ip, int port)
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

void ClientSocket::Send(char *data, int length, bool wait)
{
    while (length > 0) {
		m_send_packet = new NetworkPacket;
		m_send_packet->socketobj = NULL;

		if (length > MAX_SEND) {
			m_send_packet->length = MAX_SEND;
		} else {
			m_send_packet->length = length;
			//	this is the last packet in the send block
			//	so set the socketobj, to notify the last packet has
			//	sent
			m_send_packet->socketobj = this;
		}

		length -= m_send_packet->length;

		m_send_packet->socket = m_socket;

		memcpy(m_send_packet->data, data, m_send_packet->length);

		m_network->Send(m_send_packet);
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