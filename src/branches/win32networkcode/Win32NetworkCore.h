#ifndef _WIN32NETWORKCORE_H_
#define _WIN32NETWORKCORE_H_

#include <network/NetworkCore.h>

#include <Win32SocketEvents.h>
#include <winsock2.h>

class Win32NetworkCore: public NetworkCore{
protected:
	WSADATA m_WSAData;
	Win32SocketEvents m_network_events;
	
	HANDLE m_Thread;
	unsigned long m_ThreadID;
	
	CRITICAL_SECTION m_sockets_lock, m_senddata_lock;
	bool m_destroy_threads;
	HANDLE m_SendEvent,m_TerminateThread;

	//=============================
	//	friends
	//=============================
	friend DWORD WINAPI NetworkCoreThread(void *);
	
	//====================================
	//	Critical Section Locks/Unlocks
	//====================================

	//	Socket Locks
	inline virtual void LockSockets(void);
	inline virtual void UnlockSockets(void);
	//	Send Data stack locks
	inline virtual void LockSendStack(void);
	inline virtual void UnlockSendStack(void);
public:
	Win32NetworkCore();
	
	virtual ~Win32NetworkCore();
	
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

#endif // #ifndef _WIN32NETWORKCORE_H_
