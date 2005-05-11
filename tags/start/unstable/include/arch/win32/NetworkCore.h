#ifndef _NETWORKCORE_H_
	#define _NETWORKCORE_H_

#include <INetworkCore.h>
#include <winsock2.h>
#include <Fusion.h>

typedef std::vector<NetworkPacket *>	DATASTACK;
typedef std::vector<Socket *>					SOCKETLIST;

class Socket{
protected:
//====================================
//	friendly Thread functions
//====================================
	friend DWORD WINAPI NetworkCoreThread		(void *);

	inline virtual void		LockDataStack			(void);										//	Locks the data stack so it can be safely manipulated
	inline virtual void		UnlockDataStack		(void);										//	Unlocks the data stack so someone else can lock it

	virtual void					AddDataPacket			(NetworkPacket *packet);	//	Adds a packet to this sockets data stack

public:	
	INetworkCore			*m_network;
	NetworkPacket			*m_send_packet;
	sockaddr_in				m_socket_info;
	unsigned int			m_socket;
	unsigned int			m_timeout;
	bool							m_Connected;
	unsigned int			m_NumberPackets;

	CRITICAL_SECTION	m_datastack_lock;
	DATASTACK					m_datastack;
	NetworkPacket			*m_recv_packet;
	HANDLE						m_DataEvent;

													Socket				();
	virtual									~Socket				();
	virtual bool						Connect				(char *ip, int port);					//	Connects this computer to the remote host
	virtual void						Disconnect		(void);												//	Disconnects the socket from the remote host

	virtual void						SetConnected	(bool status);								//	Overrides the connected status

	virtual bool						Connected			(void);

	virtual void						Send					(char *data,int length);			//	Sends data to the remote host
	virtual NetworkPacket *	Receive				(int milliseconds=INFINITE);	//	Receives data from the remote host

	virtual unsigned int		GetIP					(void);												//	Retrieve the ip of this connection
	virtual unsigned int		GetPort				(void);												//	Retrieve the port of this connection
};

class ServerSocket: public Socket{
protected:
	SOCKETLIST				m_Connections;
	HANDLE						m_ConnectionEvent;
	CRITICAL_SECTION	m_Connections_lock;

//====================================
//	friendly Thread functions
//====================================
	friend DWORD WINAPI NetworkCoreThread		(void *);

	void					LockConnections		(void);
	void					UnlockConnections	(void);
public:
												ServerSocket				();
	virtual								~ServerSocket				();
	virtual void					Listen							(int port=0,int backlog=1);		//	Tells the socket what port to listen on (no parameter passed, server decides port)
	virtual void					Disconnect					(void);												//	Terminates the listening socket
	virtual SOCKETLIST &	GetConnections			(void);												//	Returns a reference to the children this socket has accepted
	virtual void					AddConnection				(Socket *child);							//	Adds a child socket to the server
	virtual void					RemoveConnection		(Socket *child);							//	Removes and disconnects a child socket to this server
	virtual unsigned int	NumberConnections		(void);												//	Requests the number of children
	virtual bool					WaitForConnections	(int milliseconds=INFINITE);	//	Waits x number of milliseconds for a connection to be made (wait stated)
	virtual unsigned int	GetIP								(void);												//	Retrieve the ip of this server socket (returns -1)
};

class SocketEvents{
private:
	unsigned int	m_number_events;
	unsigned int	m_free_places;
	unsigned int	m_cachesize;
	unsigned int	m_triggersize;

public:
	
	WSAEVENT		*m_events;
	Socket			**m_sockets;


	inline SocketEvents(){
		m_cachesize			=	10;
		m_triggersize		=	5;
		m_number_events	=	0;
		m_free_places		=	10;

		m_events		=	(WSAEVENT *)	malloc(m_cachesize+1*sizeof(WSAEVENT));
		m_sockets		=	(Socket **)		malloc(m_cachesize+1*sizeof(Socket *));

		//	This is your send trigger event, when you want to send data across the network
		//	call WSASetEvent(m_events[0]) and the NetworkCore thread will pick it up and interpret
		//	it as a send event
		m_events[0]		=	WSACreateEvent();
		m_sockets[0]	=	NULL;
	}

	inline ~SocketEvents(){
		free(m_events);
		free(m_sockets);
	}

	inline void SetCacheSize(int cachesize=10, int triggersize=5)
	{
		m_cachesize		=	cachesize;
		m_triggersize	=	triggersize;
	}

	inline void	AddEvent	(Socket *socket, unsigned int event){
		if(m_free_places <= m_triggersize){
			m_events		=	(WSAEVENT *)	realloc(m_events,		(m_cachesize + m_number_events) * sizeof(WSAEVENT));
			m_sockets		=	(Socket **)		realloc(m_sockets,	(m_cachesize + m_number_events) * sizeof(Socket *));
			m_free_places	+=	m_cachesize;
		}

		m_number_events++;
		m_free_places--;

		m_events[m_number_events]	=	WSACreateEvent();
		m_sockets[m_number_events]	=	socket;

		WSAEventSelect(socket->m_socket,m_events[m_number_events],event);
	}

	inline void	RemoveEvent	(Socket *socket){
		for(int a=0;a<m_number_events;a++){
			if(m_sockets[a] == socket){
				WSACloseEvent(m_events[a]);
				while(a < m_number_events){
					m_events[a]		= m_events[a+1];
					m_sockets[a]	= m_sockets[a+1];

					a++;
				}

				m_number_events--;
				m_free_places++;

				return;
			}
		}
	}
};

class NetworkCore: public INetworkCore{
protected:
	WSADATA						m_WSAData;
	SOCKETLIST				m_sockets;
	SocketEvents			m_network_events;
	DATASTACK					m_senddata;
	HANDLE						m_Thread, m_SendEvent;
	unsigned long			m_ThreadID;
	CRITICAL_SECTION	m_sockets_lock,	m_senddata_lock;
	bool							m_destroy_threads;
	HANDLE						m_TerminateThread;

//====================================
//	Critical Section Locks/Unlocks
//====================================

	//	Socket Locks
	inline virtual void		LockSockets				(void);
	inline virtual void		UnlockSockets			(void);
	//	Send Data stack locks
	inline virtual void		LockSendStack			(void);
	inline virtual void		UnlockSendStack		(void);

//=============================
//	friends
//=============================
	friend DWORD WINAPI	NetworkCoreThread		(void *);

//=============================
//	Protected methods only 
//	available to sockets
//=============================
	virtual void						AddSocket				(Socket *socket,int events);
	virtual bool						RemoveSocket		(Socket *socket);
	virtual void						Send						(NetworkPacket *packet);

public:
													NetworkCore					();
	virtual									~NetworkCore				();		
	virtual bool						Initialise					(void);
	virtual unsigned int		ResolveHost					(char *ip);
	virtual Socket *				CreateSocket				(void);
	virtual ServerSocket *	CreateServerSocket	(void);
};

#endif // #ifndef _NETWORKCORE_H_