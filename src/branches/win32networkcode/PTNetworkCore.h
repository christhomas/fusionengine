#ifndef _PTNETWORKCORE_H_
#define _PTNETWORKCORE_H_

#include <network/NetworkCore.h>

#include <pthread.h>
#include <Win32SocketEvents.h>
#include <winsock2.h>
#include <PTHelper.h>

class PTNetworkCore: public NetworkCore{
protected:
	WSADATA m_WSAData;
	Win32SocketEvents m_network_events;
	
	pthread_t m_Thread;
	unsigned int m_ThreadID;
	
	pthread_mutex_t m_sockets_lock, m_senddata_lock;
	bool m_destroy_threads;
	Event *m_SendEvent, *m_TerminateThread;
	
	//====================================
	//	Critical Section Locks/Unlocks
	//====================================

	//	Socket Locks
	inline virtual void LockSockets(void);
	inline virtual void UnlockSockets(void);
	//	Send Data stack locks
	inline virtual void LockSendStack(void);
	inline virtual void UnlockSendStack(void);	
	
	//=============================
	//	friends
	//=============================
	friend void * PTNetworkCoreThread(void *);	
public:
	PTNetworkCore();
	
	virtual ~PTNetworkCore();
	
	virtual bool Initialise(void);
	
	virtual IClientSocket * CreateSocket(void);
	
	virtual IServerSocket * CreateServerSocket(void);
	
	virtual void AddSocket(ISocket *socket, int events);
	
	virtual bool RemoveSocket(ISocket *socket);
	
	virtual void Send(NetworkPacket *packet);	
	
	virtual Win32SocketEvents * getSocketEvents(void);
	
	virtual NetworkPacket * getNetworkPacket(void);
	
	virtual void startThread(void);
	
	virtual void killThread(void);
};

#endif // #ifndef _PTNETWORKCORE_H_