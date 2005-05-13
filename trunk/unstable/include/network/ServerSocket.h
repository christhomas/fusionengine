#ifndef _SERVERSOCKET_H_
#define _SERVERSOCKET_H_

#include <network/INetworkCore.h>

class ServerSocket: public IServerSocket{
public:
	ServerSocket();
	
	virtual ~ServerSocket();
	
	virtual void Listen(int port, int backlog);
	
	virtual void Disconnect(void);
	
	virtual socketlist_t & GetConnections(void);
	
	virtual unsigned int NumberConnections(void);
	
	virtual unsigned int GetIP(void);
	
};

#endif // #ifndef _SERVERSOCKET_H_