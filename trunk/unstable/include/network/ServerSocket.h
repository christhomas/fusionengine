#ifndef _SERVERSOCKET_H_
#define _SERVERSOCKET_H_

#include <network/INetworkCore.h>
#include <fusion_pthread.h>

class ServerSocket: public IServerSocket{
protected:
	friend void * PTNetworkCoreThread(void *);

	pthread_mutex_t m_socketLock;
	Event *m_connectEvent;	
public:
	ServerSocket(INetworkCore *network);
	
	virtual ~ServerSocket();
	
	virtual void Listen(int port, int backlog);
	
	virtual void Disconnect(void);
	
	virtual socketlist_t & GetConnections(void);
	
	//      Adds a child socket to the server
	virtual void AddConnection(ISocket * child);
	
	//      Removes and disconnects a child socket to this server
	virtual void RemoveConnection(ISocket * child);
	
	virtual void SignalConnect(void);
	
	//      Waits x number of milliseconds for a connection to be made (wait stated)
	virtual bool WaitForConnections(unsigned int milliseconds = INFINITE);
	
	virtual unsigned int NumberConnections(void);
	
	virtual unsigned int GetIP(void);
};

#endif // #ifndef _SERVERSOCKET_H_