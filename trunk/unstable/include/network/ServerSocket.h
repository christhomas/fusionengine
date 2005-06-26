#ifndef _SERVERSOCKET_H_
#define _SERVERSOCKET_H_

#include <network/NetworkCore.h>
#include <fusion_pthread.h>

class ServerSocket: public IServerSocket{
protected:
	bool m_Connected, m_ready;
	SOCKET m_socket;
	sockaddr_in m_socket_info;
	NetworkCore *m_network;
	ISocketEvents *m_events;
	
	socketlist_t m_Connections;

	pthread_mutex_t m_socketLock;
	Event *m_connectEvent;
	
	unsigned int m_port;
	
	SMap *m_smap;
public:
	ServerSocket(NetworkCore *network);
	
	virtual ~ServerSocket();
	
	virtual void listen(int port, int backlog);
	
	virtual void disconnect(void);
	
	virtual socketlist_t & getConnections(void);
	
	//	Adds a child socket to the server
	virtual void addConnection(ISocket *child);
	
	//	Signals either a connection, or disconnection (completed) event
	virtual void emitSignal(void);
	
	//	Waits x number of milliseconds for a connection to be made (wait stated)
	virtual bool waitForConnections(unsigned int milliseconds = INFINITE);
	
	virtual unsigned int numConnections(void);
	
	virtual unsigned int getPort(void);
	
	virtual SOCKET threadAccept(void);

	virtual void threadDisconnect(void);	
};

#endif // #ifndef _SERVERSOCKET_H_