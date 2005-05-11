#ifndef _INETWORKCORE_H_
	#define _INETWORKCORE_H_

#ifndef _WINSOCKAPI_
#include <winsock2.h>
#endif

#include <vector>
#include <FusionSubsystem.h>

// This indicates how much data one packet can contain, maximum (32KB)
#define MAX_SEND	32768
#define MAX_RECV	32768

class NetworkPacket{
public:
	unsigned int	length;			//	the length of the data segment in this packet
	unsigned int	socket;			//	the socket this data is to be sent from, or received on
	char			data[MAX_RECV];	//	the data that is being sent, or has been received
};

class INetworkCore;
class ISocket;

typedef std::vector<NetworkPacket *> datastack_t;
typedef std::vector<ISocket *> socketlist_t;

class ISocket{
public:
	bool m_Connected;
	unsigned int m_socket;
	sockaddr_in m_socket_info;
	
	ISocket(){}
	virtual ~ISocket(){}
};

class IClientSocket: public ISocket{
public:
	IClientSocket(){}
	virtual ~IClientSocket(){}
	
	//      Connects this computer to the remote host
	virtual bool Connect(char *ip, int port) = 0;
	
	// Overrides the connected status
	virtual void SetConnected(bool status) = 0;
	
	//      Adds a packet to this sockets data stack
	virtual void AddDataPacket(NetworkPacket *packet) = 0;
	
	//      Sends data to the remote host
	virtual void Send(char *data, int length) = 0;
	
	//      Receives data from the remote host
	virtual NetworkPacket *Receive(int milliseconds = INFINITE) = 0;	
};

class IServerSocket: public ISocket{
public:
	INetworkCore *m_network;
	HANDLE m_ConnectionEvent;
	
	IServerSocket(){}
	virtual ~IServerSocket(){}
	
	// Tells the socket what port to listen on (no parameter passed, server decides port)
	virtual void Listen(int port = 0, int backlog = 1) = 0;
	
	// Returns a reference to the children this socket has accepted
	virtual socketlist_t & GetConnections(void) = 0;
	
	// Waits x number of milliseconds for a connection to be made (wait stated)
	virtual bool WaitForConnections(int milliseconds = INFINITE) = 0;

	// Adds a child socket to the server
	virtual void AddConnection(ISocket * child) = 0;
};

class INetworkCore: public FusionSubsystem{
public:
							INetworkCore		(){};
	virtual					~INetworkCore		(){};
	virtual bool			Initialise			(void)						=	0;
	virtual unsigned int	ResolveHost			(char *ip)					=	0;
	virtual IClientSocket *	CreateSocket		(void)						=	0;
	virtual IServerSocket *	CreateServerSocket	(void)						=	0;
	
	virtual void			AddSocket			(ISocket *socket,int events)=	0;
	virtual bool			RemoveSocket		(ISocket *socket)			=	0;
	virtual void			Send				(NetworkPacket *packet)		=	0;	
};

#endif //	#ifndef	_INETWORKCORE_H_
