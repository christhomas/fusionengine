#ifndef _WIN32NETWORKCORE_H_
#define _WIN32NETWORKCORE_H_

#include <INetworkCore.h>
#include <winsock2.h>

class Socket: public IClientSocket{
protected:
//====================================
//      friendly Thread functions
//====================================
	friend DWORD WINAPI NetworkCoreThread(void *);

	//      Locks the data stack so it can be safely manipulated
	inline virtual void LockDataStack(void);
	
	//      Unlocks the data stack so someone else can lock it
	inline virtual void UnlockDataStack(void);
public:
	INetworkCore *m_network;
	NetworkPacket *m_send_packet;
	unsigned int m_timeout;
	unsigned int m_NumberPackets;

	CRITICAL_SECTION m_datastack_lock;
	datastack_t m_datastack;
	NetworkPacket *m_recv_packet;
	HANDLE m_DataEvent;

	Socket(INetworkCore *network);
	virtual ~Socket();
	
	//      Connects this computer to the remote host
	virtual bool Connect(char *ip, int port);
	
	//      Disconnects the socket from the remote host
	virtual void Disconnect(void);

	//      Overrides the connected status
	virtual void SetConnected(bool status);

	virtual bool Connected(void);
	
	//      Adds a packet to this sockets data stack
	virtual void AddDataPacket(NetworkPacket * packet);	

	//      Sends data to the remote host
	virtual void Send(char *data, int length);
	
	//      Receives data from the remote host
	virtual NetworkPacket *Receive(int milliseconds = INFINITE);

	//      Retrieve the ip of this connection
	virtual unsigned int GetIP(void);
	
	//      Retrieve the port of this connection
	virtual unsigned int GetPort(void);
};

class ServerSocket: public IServerSocket{
protected:
	socketlist_t m_Connections;
	CRITICAL_SECTION m_Connections_lock;

	//====================================
	//      friendly Thread functions
	//====================================
	friend DWORD WINAPI NetworkCoreThread(void *);

	void LockConnections(void);
	void UnlockConnections(void);
public:
	ServerSocket(INetworkCore *network);
	virtual ~ServerSocket();
	
	//      Tells the socket what port to listen on (no parameter passed, server decides port)
	virtual void Listen(int port = 0, int backlog = 1);
	
	//      Terminates the listening socket
	virtual void Disconnect(void);
	
	//      Returns a reference to the children this socket has accepted
	virtual socketlist_t & GetConnections(void);
	
	//      Adds a child socket to the server
	virtual void AddConnection(ISocket * child);
	
	//      Removes and disconnects a child socket to this server
	virtual void RemoveConnection(ISocket * child);
	
	//      Requests the number of children
	virtual unsigned int NumberConnections(void);
	
	//      Waits x number of milliseconds for a connection to be made (wait stated)
	virtual bool WaitForConnections(int milliseconds = INFINITE);
	
	//      Retrieve the ip of this server socket (returns -1)
	virtual unsigned int GetIP(void);
};

#include <Win32SocketEvents.h>

class NetworkCore:public INetworkCore {
protected:
	WSADATA m_WSAData;
	socketlist_t m_sockets;
	Win32SocketEvents m_network_events;
	datastack_t m_senddata;
	HANDLE m_Thread, m_SendEvent;
	unsigned long m_ThreadID;
	CRITICAL_SECTION m_sockets_lock, m_senddata_lock;
	bool m_destroy_threads;
	HANDLE m_TerminateThread;

	//====================================
	//      Critical Section Locks/Unlocks
	//====================================

	//      Socket Locks
	inline virtual void LockSockets(void);
	inline virtual void UnlockSockets(void);
	//      Send Data stack locks
	inline virtual void LockSendStack(void);
	inline virtual void UnlockSendStack(void);

	//=============================
	//      friends
	//=============================
	friend DWORD WINAPI NetworkCoreThread(void *);
public:
	NetworkCore();
	
	virtual ~ NetworkCore();
	
	virtual bool Initialise(void);
	
	virtual unsigned int ResolveHost(char *ip);
	
	virtual IClientSocket * CreateSocket(void);
	
	virtual IServerSocket * CreateServerSocket(void);
	
	virtual void AddSocket(ISocket *socket, int events);
	
	virtual bool RemoveSocket(ISocket *socket);
	
	virtual void Send(NetworkPacket *packet);	
};

#endif // #ifndef _WIN32NETWORKCORE_H_
