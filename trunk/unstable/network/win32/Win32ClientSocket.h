#ifndef _WIN32CLIENTSOCKET_H_
#define _WIN32CLIENTSOCKET_H_

#include <network/ClientSocket.h>

class Win32ClientSocket: public ClientSocket{
protected:
	friend DWORD WINAPI NetworkCoreThread(void *);

	//	Locks/Unlocks the data stack so it can be safely manipulated
	inline virtual void LockDataStack(void);
	inline virtual void UnlockDataStack(void);
public:
	CRITICAL_SECTION m_datastack_lock;
	HANDLE m_DataEvent,m_SendEvent;

	Win32ClientSocket(INetworkCore *network);
	virtual ~Win32ClientSocket();
	
	//	Adds a packet to this sockets data stack
	virtual void AddDataPacket(NetworkPacket * packet);	
	
	void Win32ClientSocket::Send(char *data, int length, bool wait);
	
	//	Signals the current send has completed (if blocked, will unblock)
	virtual void SendComplete(void);
	
	//	Receives data from the remote host
	virtual NetworkPacket *Receive(int milliseconds = INFINITE);
};

#endif // #ifndef _WIN32CLIENTSOCKET_H_