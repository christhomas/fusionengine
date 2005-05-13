#ifndef _NETWORKCORE_H_
#define _NETWORKCORE_H_

#include <network/INetworkCore.h>

class NetworkCore: public INetworkCore{
public:
	NetworkCore();
			
	virtual ~NetworkCore();
	
	virtual unsigned int ResolveHost(char *ip);
	
	virtual bool RemoveSocket(ISocket *socket) = 0;
};

#endif // #ifndef _NETWORKCORE_H_