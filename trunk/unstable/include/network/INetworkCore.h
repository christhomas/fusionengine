#ifndef _INETWORKCORE_H_
	#define _INETWORKCORE_H_

#include <vector>
#include <map>
#include <FusionSubsystem.h>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#define INFINITE (~0)
#endif // ifdef _WIN32

//	FIXME:	This value should probably be variable
//			Also send/recv buffers should probably vary, depending on the loads involved
// This indicates how much data one packet can contain, maximum (32KB)
#define MAX_SEND	32768
#define MAX_RECV	32768

class INetworkCore;
class ISocket;

struct NetworkPacket{
	ISocket			*socketobj;		//	The object who sent the data (if it's a send packet, otherwise NULL)
	unsigned int	length;			//	the length of the data segment in this packet
	char			data[MAX_RECV];	//	the data that is being sent, or has been received
};
typedef std::vector<NetworkPacket *> datastack_t;

class ISocket{
public:
	ISocket(){}
	virtual ~ISocket(){}
};
typedef std::vector<ISocket *> socketlist_t;

/*	Explanation of SMap structure

	SMap and socketmap_t are ways to map pieces of information about a socket
	together so if you have one piece of data, you can find out the other
	
	in BSD sockets and WSA sockets, this works slightly differently.
	
	BSD:
		You have to map the SOCKET to a SMap structure, which in turn, gives you a ISocket
		object and a type variable, type, being whether it's a SMap::CLIENT_SOCKET or SMap::SERVER_SOCKET
		so you can cast ISocket to the type you need, in order to determine what kind of socket this is
		and what to do with it when you have cast it.
		
	WSA:
		You have to map the EVENT HANDLE to a SMap structure, since they are pointers, or 32 bit references
		I use the unsigned int value of the EVENT HANDLE, which is unique, as a key to getting hold of the SMap
		structure, I'm aware this isnt really a very nice method, but it's the only way I can think of doing
		BSD and WSA simply and quickly using the same structures and without customising everything.
		
		Once you have the SMap structure, you can get the ISocket object and the SOCKET, which you can use for
		further processing.
*/
struct SMap{
	enum{ CLIENT_SOCKET=0,SERVER_SOCKET };
	
	ISocket *socketobj;

	int type;		//	Use with BSD sockets
	SOCKET sock;	//	Use with WSA sockets
};
typedef std::map<unsigned int, SMap *> socketmap_t;

class ISocketEvents{
public:
	ISocketEvents(){};
	virtual ~ISocketEvents(){};
	
	virtual void * getEvents(void) = 0;
	
	virtual void * getSockets(void) = 0;
	
	virtual SMap * addEvent(ISocket *socket, SOCKET s, int type) = 0;
	
	virtual bool removeEvent(ISocket *socket) = 0;
	
	//	FIXME: Protected method?
	virtual void clearEvents(void) = 0;
	
	virtual unsigned int numEvents(void) = 0;
	
	virtual unsigned int numSockets(void) = 0;
	
	virtual void setSendEvent(SMap *smap) = 0;
	
	virtual void resetSendEvent(void) = 0;
	
	virtual void setBreakEvent(void) = 0;
};

class IClientSocket: public ISocket{
public:
	IClientSocket(){}
	
	virtual ~IClientSocket(){}
	
	//	Connects this computer to the remote host
	virtual bool connect(std::string ip, int port, unsigned int timeout = INFINITE) = 0;
	virtual bool connect(const char *ip, int port, unsigned int timeout = INFINITE) = 0;
	
	//	Disconnects the socket from the server, if it's connected
	virtual bool disconnect(void) = 0;
	virtual bool disconnect(bool deleteSocket) = 0;
	
	//	Overrides the connected status
	virtual void setConnected(bool status) = 0;
	
	//	Sends data to the remote host
	virtual bool send(const char *data, int length, bool wait=false) = 0;
	
	//	Receives data from the remote host
	virtual NetworkPacket * receive(unsigned int milliseconds = INFINITE) = 0;
};

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

class INetworkCore: public FusionSubsystem{
protected:
	//	FIXME: Another protected method?
	virtual void initSocketEvents(void) = 0;
	//	FIXME: This method should be protected? cause you shouldnt use it externally?
	virtual SMap * addSocket(ISocket *socket, SOCKET s, int type) = 0;
	//	FIXME: Another protected method?
	virtual void clearSockets(void) = 0;
public:
	INetworkCore(){};
	virtual ~INetworkCore(){};
	
	virtual bool Initialise(void) = 0;
		
	virtual IClientSocket * createSocket(void) = 0;
	
	virtual IServerSocket * createServerSocket(void) = 0;
		
	virtual SMap * addSocket(IClientSocket *socket, SOCKET s) = 0;
	
	virtual SMap * addSocket(IServerSocket *socket, SOCKET s) = 0;
		
	virtual bool removeSocket(ISocket *socket) = 0;
			
	//	Method to output details of the error to Fusion's 
	//	logfile and return it's severity 
	//	(true = fatal, false = non-fatal, retry)
	virtual bool error(void) = 0;	
};

#endif //	#ifndef	_INETWORKCORE_H_
