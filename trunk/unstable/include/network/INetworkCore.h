#ifndef _INETWORKCORE_H_
	#define _INETWORKCORE_H_

#include <vector>
#include <FusionSubsystem.h>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#define INFINITE (~0)
#endif // ifdef _WIN32

// This indicates how much data one packet can contain, maximum (32KB)
#define MAX_SEND	32768
#define MAX_RECV	32768

class INetworkCore;

class ISocket{
public:
	bool m_Connected;
	unsigned int m_socket;
	sockaddr_in m_socket_info;
	INetworkCore *m_network;
	
	ISocket(){}
	virtual ~ISocket(){}
};

typedef std::vector<ISocket *> socketlist_t;

class NetworkPacket{
public:
	ISocket			*socketobj;		//	The object who sent the data (if it's a send packet, otherwise NULL)
	unsigned int	length;			//	the length of the data segment in this packet
	unsigned int	socket;			//	the socket this data is to be sent from, or received on
	char			data[MAX_RECV];	//	the data that is being sent, or has been received
};

typedef std::vector<NetworkPacket *> datastack_t;

class IClientSocket: public ISocket{
public:
	IClientSocket(){}
	
	virtual ~IClientSocket(){}
	
	//	Connects this computer to the remote host
	virtual bool Connect(const char *ip, int port) = 0;
	
	//	Disconnects the socket from the server, if it's connected
	virtual void Disconnect(void) = 0;
	
	//	Overrides the connected status
	virtual void SetConnected(bool status) = 0;
	
	//	Sends data to the remote host
	virtual void Send(const char *data, int length, bool wait=false) = 0;
	
	//	Receives data from the remote host
	virtual NetworkPacket * Receive(unsigned int milliseconds = INFINITE) = 0;
};

class IServerSocket: public ISocket{
protected:
	socketlist_t m_Connections;
public:
	IServerSocket(){}
	
	virtual ~IServerSocket(){}
	
	// Tells the socket what port to listen on (no parameter passed, server decides port)
	virtual void Listen(int port = 0, int backlog = 1) = 0;
	
	//	Tells the server to close the listening socket and disconnect all clients connected through it
	virtual void Disconnect(void) = 0;
	
	// Returns a reference to the children this socket has accepted
	virtual socketlist_t & GetConnections(void) = 0;
	
	// Waits x number of milliseconds for a connection to be made (wait stated)
	virtual bool WaitForConnections(unsigned int milliseconds = INFINITE) = 0;

	// Adds a child socket to the server
	virtual void AddConnection(ISocket *child) = 0;
};

class INetworkCore: public FusionSubsystem{
protected:
	socketlist_t	m_sockets;	
	datastack_t		m_senddata;
public:
							INetworkCore		(){};
	virtual					~INetworkCore		(){};
	virtual bool			Initialise			(void)						=	0;
	virtual unsigned int	ResolveHost			(const char *ip)			=	0;
	virtual IClientSocket *	CreateSocket		(void)						=	0;
	virtual IServerSocket *	CreateServerSocket	(void)						=	0;
	
	virtual void			AddSocket			(ISocket *socket,int events)=	0;
	virtual bool			RemoveSocket		(ISocket *socket)			=	0;
	virtual void			Send				(NetworkPacket *packet)		=	0;	
	
	virtual void			error				(void)						=	0;
};

#endif //	#ifndef	_INETWORKCORE_H_
