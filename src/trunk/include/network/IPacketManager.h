#ifndef _IPACKETMANAGER_H_
#define _IPACKETMANAGER_H_

#include <network/INetwork.h>

class ISocket;
class IPacketManager;

//	A callback which can be setup to create a packet manager object
typedef IPacketManager * (*create_packet_manager_t)(void);

//	A structure containing all the information 
//	needed to send data or recv data
struct NetworkPacket{
	ISocket			*socketobj;		//	The object who sent the data (if it's a send packet, otherwise NULL)
	unsigned int	length;			//	the length of the data segment in this packet
	char			data[MAX_RECV];	//	the data that is being sent, or has been received

	IPacketManager	*manager;
	NetworkPacket	*prev, *next;
};


class IPacketManager{
protected:
	//	This method is relied upon to clear up all the allocated memory
	virtual void deallocatePackets(void) = 0;
	
	//	Allocates a new cache of packets
	virtual void allocatePacket(void) = 0;
	
	//	Finds the next empty packet
	virtual NetworkPacket * findPacket(void) = 0;
	
	//	Copies the internal data from a another packet manager object
	virtual IPacketManager & operator=(IPacketManager &pm) = 0;
public:
	IPacketManager();
	virtual ~IPacketManager();
	
	//	Locks access to the packet manager
	virtual void lock(void) = 0;
	
	//	Unlocks access to the packet manager
	virtual void unlock(void) = 0;
	
	//	Sets the amount of NetworkPackets allocated in one chunk
	virtual void setCacheSize(unsigned int cache) = 0;
	
	//	Request a packet to use
	virtual NetworkPacket * requestPacket(void) = 0;
	
	//	Requests the head of the linked list
	virtual NetworkPacket * requestHead(void) = 0;
	
	//	Requests the tail of the linked list
	virtual NetworkPacket * requestTail(void) = 0;
	
	//	Release a packet from the linked list to the app
	virtual void releasePacket(NetworkPacket *packet) = 0;
	
	//	Returns control of a packet to the manager
	virtual void returnPacket(NetworkPacket *packet) = 0;
	
	//	Clears the linked list and resets to a default state (all empty)
	virtual void clear(void) = 0;
};

#endif // #ifndef _IPACKETMANAGER_H_