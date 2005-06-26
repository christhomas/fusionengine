#ifndef _CLIENTSOCKET_H_
#define _CLIENTSOCKET_H_

#include <network/NetworkCore.h>
#include <fusion_pthread.h>

class ClientSocket: public IClientSocket{
protected:
	NetworkCore *m_network;
	ISocketEvents *m_events;
	
	std::string m_address;
	unsigned int m_ipaddress;
	unsigned int m_port;
	
	SMap *m_smap;
	
	bool m_Connected, m_ready;
	SOCKET m_rsocket;
	
	datastack_t m_recvStack;
	datastack_t m_sendStack;

	pthread_mutex_t m_recvStack_lock;
	pthread_mutex_t m_sendStack_lock;
	Event *m_recvEvent, *m_sendEvent, *m_connectEvent;
	
	//	Resolves a string into an ip address used by the network stack
	virtual unsigned int resolveHost(const char *ip);
	
public:
	ClientSocket(NetworkCore *network);
	
	virtual ~ClientSocket();	

	//	Connects this computer to the remote host
	virtual bool connect(std::string ip, int port, unsigned int timeout);
	virtual bool connect(const char *ip, int port, unsigned int timeout);
	
	//	Disconnects the socket from the remote host
	virtual bool disconnect(void);
	virtual bool disconnect(bool deleteSocket);

	//	Overrides the connected status
	virtual void setConnected(bool status);

	//	Retrieves the connected status
	virtual bool connected(void);

	//	Sends data to the remote host
	virtual bool send(const char *data, int length, bool wait);	
		
	//	Retrieves a network packet, from the network core, to the application
	virtual NetworkPacket * receive(unsigned int milliseconds);
		
	//	Retrieve the address/hostname of this connection
	virtual std::string getAddress(void);
	
	//	Retrieve the ip of this connection
	virtual unsigned int getIP(void);
	
	//	Retrieve the port of this connection
	virtual unsigned int getPort(void);	
		
	//	FIXME: This can only be used from thread, but it's public, so can be used from anywhere
	//	Connects a remote host, to this socket and sets up an event upon it
	virtual void threadConnect(void);
	
	//	FIXME: This can only be used from thread, but it's public, so can be used from anywhere
	//	Connects a remote host, to this socket
	virtual bool threadConnect(SOCKET s, unsigned int port);
	
	//	FIXME: This can only be used from thread, but it's public, so can be used from anywhere
	//	Connects a remote host, to this socket	
	virtual void threadDisconnect(void);

	//	FIXME: This can only be used from thread, but it's public, so can be used from anywhere
	//	Retrieves a network packet, from the thread, to the network core
	virtual void threadReceive(void);
	
	//	FIXME: This can only be used from thread, but it's public, so can be used from anywhere
	//	Sends out all the data the socket is buffering, across the socket
	virtual void threadSend(void);
	
	//	FIXME: This can only be used from thread, but it's public, so can be used from anywhere
	//	Determines from the state of the socket, whether a send/recv needs to execute
	virtual void threadProcess(SOCKET s);
};

#endif // #ifndef _CLIENTSOCKET_H_