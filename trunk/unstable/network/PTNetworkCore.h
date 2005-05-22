#ifndef _PTNETWORKCORE_H_
#define _PTNETWORKCORE_H_

#include <network/NetworkCore.h>

#include <fusion_pthread.h>

class PTNetworkCore: public NetworkCore{
protected:
	pthread_t m_Thread;
	unsigned int m_ThreadID;
	
	pthread_mutex_t m_sockets_lock, m_senddata_lock;
	bool m_destroy_threads;
	Event *m_SendEvent, *m_TerminateThread;
public:
	PTNetworkCore();
	
	virtual ~PTNetworkCore();
	
	virtual bool Initialise(void);
	
	virtual IClientSocket * CreateSocket(void);
	
	virtual IServerSocket * CreateServerSocket(void);
	
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

#endif // #ifndef _PTNETWORKCORE_H_