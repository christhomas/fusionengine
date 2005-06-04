#ifndef _NETWORKCORE_H_
#define _NETWORKCORE_H_

#include <network/INetworkCore.h>
#include <fusion_pthread.h>

class NetworkCore: public INetworkCore{
protected:
	pthread_t m_Thread;
	unsigned int m_ThreadID;
	
	pthread_mutex_t m_sockets_lock, m_senddata_lock;
	bool m_destroy_threads;
	Event *m_SendEvent, *m_TerminateThread;
public:
	NetworkCore();
			
	virtual ~NetworkCore();
	
	virtual bool Initialise(void);
	
	virtual IClientSocket * CreateSocket(void);
	
	virtual IServerSocket * CreateServerSocket(void);
	
	virtual unsigned int ResolveHost(const char *ip);
	
	virtual void Send(NetworkPacket *packet);	
	
	virtual NetworkPacket * getNetworkPacket(void);
	
	virtual void startThread(void);
	
	virtual void killThread(void);
	
	//====================================
	//	Mutex Locks/Unlocks
	//====================================

	//	Socket Locks
	inline virtual void LockSockets(void);
	inline virtual void UnlockSockets(void);
	//	Send Data stack locks
	inline virtual void LockSendStack(void);
	inline virtual void UnlockSendStack(void);
};

#endif // #ifndef _NETWORKCORE_H_