#ifndef _BASICPACKETMANGER_H_
#define _BASICPACKETMANGER_H_

#include <network/IPacketManager.h>

class BasicPacketManager: public IPacketManager{
protected:
	unsigned int m_cache, m_free;
	
	NetworkPacket *m_empty, *m_head, *m_tail;
	
	pthread_mutex_t m_lock;
	
	//	This method is relied upon to clear up all the allocated memory
	virtual void deallocatePackets(void);
	
	//	Allocates a single packet
	virtual void allocatePacket(void);
	
	//	Returns the next available packet
	virtual NetworkPacket * findPacket(void);
	
	virtual IPacketManager & operator=(IPacketManager &pm);
public:
	BasicPacketManager();
	BasicPacketManager(IPacketManager *pm);
	virtual ~BasicPacketManager();
	
	//	Locks access to the packet manager
	virtual void lock(void);
	
	//	unlocks access to the packet manager
	virtual void unlock(void);
	
	//	Sets the amount of NetworkPackets allocated in one chunk
	virtual void setCacheSize(unsigned int cache);
	
	//	Request a send or recv packet to use
	virtual NetworkPacket * requestPacket(void);
	
	//	Requests the head of the linked list
	virtual NetworkPacket * requestHead(void);
	
	//	Requests the tail of the linked list
	virtual NetworkPacket * requestTail(void);
	
	//	Release a packet back to be reused
	virtual void releasePacket(NetworkPacket *packet);

	//	Clears the linked list and resets to a default state (all empty)	
	virtual void clear(void);
};

#endif // #ifndef _BASICPACKETMANGER_H_