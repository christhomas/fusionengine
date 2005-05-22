#ifndef _CLIENTSOCKET_H_
#define _CLIENTSOCKET_H_

#include <network/INetworkCore.h>

class ClientSocket: public IClientSocket{
public:
	ClientSocket();
	
	virtual ~ClientSocket();	

	//	Connects this computer to the remote host
	virtual bool Connect(const char *ip, int port);
	
	virtual void Connect(unsigned int socket);
	
	//	Disconnects the socket from the remote host
	virtual void Disconnect(void);

	//	Overrides the connected status
	virtual void SetConnected(bool status);

	virtual bool Connected(void);
	
	//	Sends data to the remote host
	virtual void Send(const char *data, int length, bool wait);	
	
	//	Retrieve the ip of this connection
	virtual unsigned int GetIP(void);
	
	//	Retrieve the port of this connection
	virtual unsigned int GetPort(void);	
};

#endif // #ifndef _CLIENTSOCKET_H_