#ifndef _WSANETWORKCORE_H_
#define _WSANETWORKCORE_H_

#include <network/NetworkCore.h>
#include <network/WSASocketEvents.h>

class WSANetworkCore: public NetworkCore{
public:
	WSANetworkCore();
	
	virtual ~WSANetworkCore();
	
	virtual void initSocketEvents(void);
	
	virtual SMap * addSocket(IClientSocket *socket, SOCKET s);
	
	virtual SMap * addSocket(IServerSocket *socket, SOCKET s);
	
	//	EXPLANATION:
	//	Cause you overload addSocket in this object, if you attempt to 
	//	use a further overloaded version of a method from a base class, 
	//	the compiler complains it can't find it, so you tell it to manually 
	//	use the base class version of the method too, then the compiler 
	//	finds the methods it's looking for and shuts up
	using NetworkCore::addSocket;
	
	virtual bool startThread(void);
	
	virtual bool error(void);
};

#endif // #ifndef _WSANETWORKCORE_H_

