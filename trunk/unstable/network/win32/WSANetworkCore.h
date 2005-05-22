#ifndef _WSANETWORKCORE_H_
#define _WSANETWORKCORE_H_

#include <PTNetworkCore.h>

#include <WSASocketEvents.h>
#include <winsock2.h>

class WSANetworkCore: public PTNetworkCore{
protected:
	WSADATA m_WSAData;
	WSASocketEvents m_network_events;
public:
	WSANetworkCore();
	
	virtual ~WSANetworkCore();
	
	virtual bool Initialise(void);
	
	virtual void AddSocket(ISocket *socket, int events);
	
	virtual bool RemoveSocket(ISocket *socket);	
	
	virtual void Send(NetworkPacket *packet);
	
	virtual WSASocketEvents * getSocketEvents(void);
};

#endif // #ifndef _WSANETWORKCORE_H_

