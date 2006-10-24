#ifndef _ISERVERSOCKET_H_
#define _ISERVERSOCKET_H_

class IServerSocket: public ISocket{
public:
	IServerSocket(){}
	
	virtual ~IServerSocket(){}
	
	// Tells the socket what port to listen on (no parameter passed, server decides port)
	virtual void listen(int port = 0, int backlog = 1) = 0;
	
	//	Tells the server to close the listening socket and disconnect all clients connected through it
	virtual void disconnect(void) = 0;
	
	// Returns a reference to the children this socket has accepted
	virtual socketlist_t & getConnections(void) = 0;
	
	// Waits x number of milliseconds for a connection to be made (wait stated)
	virtual bool waitForConnections(unsigned int milliseconds = INFINITE) = 0;

	// Adds a child socket to the server
	virtual void addConnection(ISocket *child) = 0;
};

#endif // #ifndef _ISERVERSOCKET_H_