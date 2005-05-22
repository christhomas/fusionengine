#include <network/ServerSocket.h>

ServerSocket::ServerSocket(){}

ServerSocket::~ServerSocket()
{
	Disconnect();
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

		//	 Remove all the child client sockets
		for(unsigned int a=0;a<m_Connections.size();a++){
			delete m_Connections[a];
		}
		m_Connections.clear();
    }
}

socketlist_t & ServerSocket::GetConnections(void)
{
    return m_Connections;
}

unsigned int ServerSocket::NumberConnections(void)
{
    return (unsigned int)m_Connections.size();
}

unsigned int ServerSocket::GetIP(void)
{
    return -1;
}