#ifndef _PTCLIENTSOCKET_H_
#define _PTCLIENTSOCKET_H_

#include <network/ClientSocket.h>

#include <fusion_pthread.h>

class PTClientSocket: public ClientSocket{
protected:
	pthread_mutex_t m_datastack_lock;
	Event *m_DataEvent, *m_SendEvent;
	
	//	Locks/Unlocks the data stack so it can be safely manipulated
	inline virtual void LockDataStack(void);
	inline virtual void UnlockDataStack(void);
public:
	PTClientSocket(INetworkCore *network);
	virtual ~PTClientSocket();
		
	virtual void Send(const char *data, int length, bool wait);
	
	//	Signals the current send has completed (if blocked, will unblock)
	virtual void SignalSend(void);
	
	//	Receives data from the remote host
	virtual NetworkPacket *Receive(unsigned int milliseconds = INFINITE);
	
	virtual void socketReceive(void);	
};

#endif // #ifndef _PTCLIENTSOCKET_H_
