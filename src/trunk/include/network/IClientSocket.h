#ifndef _ICLIENTSOCKET_H_
#define _ICLIENTSOCKET_H_

#include <string>

#include <network/INetwork.h>

struct NetworkPacket;

class IClientSocket: public ISocket{
public:
	IClientSocket(){}
	
	virtual ~IClientSocket(){}
	
	//	Connects this computer to the remote host
	virtual bool connect(std::string ip, int port, unsigned int timeout = INFINITE) = 0;
	virtual bool connect(const char *ip, int port, unsigned int timeout = INFINITE) = 0;
	
	//	Disconnects the socket from the server, if it's connected
	virtual bool disconnect(void) = 0;
	virtual bool disconnect(bool deleteSocket) = 0;
	
	//	Overrides the connected status
	virtual void setConnected(bool status) = 0;
	
	//	Sends data to the remote host
	virtual bool send(const char *data, int length, bool wait=false) = 0;
	
	//	Receives data from the remote host
	virtual NetworkPacket * receive(unsigned int milliseconds = INFINITE) = 0;
};

#endif // #ifndef _ICLIENTSOCKET_H_