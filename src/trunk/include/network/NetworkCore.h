#ifndef _NETWORKCORE_H_
#define _NETWORKCORE_H_

#include <network/INetworkCore.h>
#include <fusion_pthread.h>
#include <fusion.h>

class ISocket;
class IClientSocket;
class IServerSocket;
class ISocketEvents;
struct SMap;

class IPacketManager;
class BasicPacketManager;
//class BlockPacketManager;

class NetworkCore: public INetworkCore{
protected:	
	pthread_t m_Thread;
	unsigned int m_ThreadID;
	
	pthread_mutex_t m_sockets_lock;
	bool m_destroy_threads;
	Event *m_TerminateThread;
	
	IPacketManager *m_sendPacketManager;
	IPacketManager *m_recvPacketManager;
	
	ISocketEvents *m_socketEvents;
	
	virtual void initSocketEvents(void);

	virtual SMap * addSocket(ISocket *socket, SOCKET s, int type);

	virtual void clearSockets(void);		
public:
	NetworkCore();
			
	virtual ~NetworkCore();
	
	virtual bool Initialise(void);	
	
	virtual IPacketManager * getPacketManager(unsigned int type);
		
	virtual ISocketEvents * getSocketEvents(void);
	
	virtual IClientSocket * createSocket(void);
	
	virtual IServerSocket * createServerSocket(void);
		
	virtual SMap * addSocket(IClientSocket *socket, SOCKET s);
	
	virtual SMap * addSocket(IServerSocket *socket, SOCKET s);
		
	virtual bool removeSocket(ISocket *socket);
		
	virtual void socketSend(IClientSocket *socket);
	
	virtual void socketReceive(IClientSocket *socket);
	
	virtual void initThread(void);
	
	virtual bool startThread(void);	
	
	virtual bool killThread(void);
	
	virtual bool error(void);
};

#endif // #ifndef _NETWORKCORE_H_