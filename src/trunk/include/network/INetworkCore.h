#ifndef _INETWORKCORE_H_
	#define _INETWORKCORE_H_

#include <FusionSubsystem.h>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#define INFINITE (~0)
#endif // ifdef _WIN32

#include <network/INetwork.h>

class ISocket;
class IClientSocket;
class IServerSocket;
struct SMap;

class INetworkCore: public FusionSubsystem{
protected:
	//	Creates the appropriate SocketEvent object
	virtual void initSocketEvents(void) = 0;
	
	virtual SMap * addSocket(ISocket *socket, SOCKET s, int type) = 0;
	
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
