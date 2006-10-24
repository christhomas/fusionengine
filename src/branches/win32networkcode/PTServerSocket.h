#ifndef _PTSERVERSOCKET_H_
#define _PTSERVERSOCKET_H_

#include <network/ServerSocket.h>

#include <pthread.h>
#include <PTHelper.h>

class PTServerSocket: public ServerSocket{
protected:
	friend void * PTNetworkCoreThread(void *);

	pthread_mutex_t m_SocketLock;
	Event *m_ConnectEvent;	

	void LockConnections(void);
	void UnlockConnections(void);
public:
	
	PTServerSocket(INetworkCore *network);
	
	virtual ~PTServerSocket();
	
	//      Adds a child socket to the server
	virtual void AddConnection(ISocket * child);
	
	//      Removes and disconnects a child socket to this server
	virtual void RemoveConnection(ISocket * child);
	
	virtual void SignalConnect(void);
	
	//      Waits x number of milliseconds for a connection to be made (wait stated)
	virtual bool WaitForConnections(unsigned int milliseconds = INFINITE);
};


#endif // #ifndef _PTSERVERSOCKET_H_