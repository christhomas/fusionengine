#ifndef _WIN32SERVERSOCKET_H_
#define _WIN32SERVERSOCKET_H_

#include <network/ServerSocket.h>

class Win32ServerSocket: public ServerSocket{
protected:
	CRITICAL_SECTION m_Connections_lock;

	//====================================
	//      friendly Thread functions
	//====================================
	friend DWORD WINAPI NetworkCoreThread(void *);

	void LockConnections(void);
	void UnlockConnections(void);
public:
	HANDLE m_ConnectionEvent;
	
	Win32ServerSocket(INetworkCore *network);
	
	virtual ~Win32ServerSocket();
	
	//      Adds a child socket to the server
	virtual void AddConnection(ISocket * child);
	
	//      Removes and disconnects a child socket to this server
	virtual void RemoveConnection(ISocket * child);
	
	virtual void SignalConnect(void);
	
	//      Waits x number of milliseconds for a connection to be made (wait stated)
	virtual bool WaitForConnections(unsigned int milliseconds = INFINITE);
};


#endif // #ifndef _WIN32SERVERSOCKET_H_