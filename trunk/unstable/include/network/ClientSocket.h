#ifndef _CLIENTSOCKET_H_
#define _CLIENTSOCKET_H_

#include <network/INetworkCore.h>
#include <fusion_pthread.h>

class ClientSocket: public IClientSocket{
protected:
	datastack_t m_recvStack;
	datastack_t m_sendStack;

	pthread_mutex_t m_recvStack_lock;
	pthread_mutex_t m_sendStack_lock;
	Event *m_recvEvent, *m_sendEvent;
public:
	ClientSocket(INetworkCore *network);
	
	virtual ~ClientSocket();	

	//	Connects this computer to the remote host
	virtual bool Connect(const char *ip, int port);
	
	//	Disconnects the socket from the remote host
	virtual void Disconnect(void);

	//	Overrides the connected status
	virtual void SetConnected(bool status);

	//	Retrieves the connected status
	virtual bool Connected(void);
	
	//	Signals the current send has completed (if blocked, will unblock)
	virtual void SignalSend(void);

	//	Sends data to the remote host
	virtual void Send(const char *data, int length, bool wait);	
		
	//	Retrieves a network packet, from the network core, to the application
	virtual NetworkPacket * Receive(unsigned int milliseconds = INFINITE);
		
	//	Retrieve the ip of this connection
	virtual unsigned int GetIP(void);
	
	//	Retrieve the port of this connection
	virtual unsigned int GetPort(void);	
	
	//	FIXME: This can only be used from thread, but it's public, so can be used from anywhere
	//	Connects a remote host, to this socket
	virtual void threadConnect(unsigned int socket);

	//	FIXME: This can only be used from thread, but it's public, so can be used from anywhere
	//	Retrieves a network packet, from the thread, to the network core
	virtual void threadReceive(void);	
};

#endif // #ifndef _CLIENTSOCKET_H_