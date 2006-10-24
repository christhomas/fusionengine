#ifndef _INETWORK_H_
#define _INETWORK_H_

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#define INFINITE (~0)
#endif // ifdef _WIN32

#include <vector>
#include <map>

//	FIXME:	This value should probably be variable
//			Also send/recv buffers should probably vary, depending on the loads involved
//	NOTE:	Would involve changing NetworkPacket::data[MAX_RECV] to a char * 
//			and dynamically allocating the ram
// This indicates how much data one packet can contain, maximum (32KB)
#define MAX_SEND	32768
#define MAX_RECV	32768

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

#endif // #ifndef _INETWORK_H_