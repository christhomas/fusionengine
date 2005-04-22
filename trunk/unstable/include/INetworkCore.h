#ifndef _INETWORKCORE_H_
	#define _INETWORKCORE_H_

#include <vector>
#include <FusionSubsystem.h>

// This indicates how much data one packet can contain, maximum (32KB)
#define MAX_SEND	32768
#define MAX_RECV	32768

class NetworkPacket{
public:
	unsigned int	length;			//	the length of the data segment in this packet
	unsigned int	socket;			//	the socket this data is to be sent from, or received on
	char			data[MAX_RECV];	//	the data that is being sent, or has been received
};

class INetworkCore: public FusionSubsystem{
protected:
//=============================
//	friends
//=============================
	friend class Socket;
	friend class ServerSocket;

//=============================
//	Protected methods only 
//	available to sockets
//=============================
	virtual void						AddSocket						(Socket *socket,int events)	=	0;
	virtual bool						RemoveSocket				(Socket *socket)						=	0;
	virtual void						Send								(NetworkPacket *packet)			=	0;
public:
													INetworkCore				(){};
	virtual									~INetworkCore				(){};
	virtual bool						Initialise					(void)											=	0;
	virtual unsigned int		ResolveHost					(char *ip)									=	0;
	virtual Socket *				CreateSocket				(void)											=	0;
	virtual ServerSocket *	CreateServerSocket	(void)											=	0;
};

#endif //	#ifndef	_INETWORKCORE_H_
