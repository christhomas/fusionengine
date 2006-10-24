#include <network/BasicPacketManager.h>

//	FIXME:	All this code needs to be locked, 
//			cause the thread and main thread 
//			can both access it and cause trouble

BasicPacketManager::BasicPacketManager()
{
	m_head = NULL;
	m_tail = NULL;
	
	pthread_mutex_init(&m_lock, NULL);
}

BasicPacketManager::BasicPacketManager(IPacketManager *pm)
{
	m_head = NULL;
	m_tail = NULL;
	
	//	Copy the contents of pm, into here
	*this = *pm;
}

BasicPacketManager::~BasicPacketManager()
{	
	//	Have to make sure all the memory is deleted
	deallocatePackets();

    pthread_mutex_destroy(&m_lock);	
}

void BasicPacketManager::deallocatePackets(void)
{
	lock();
	
	//	This method has to delete all the allocate memory
	
	//	Cause there are three linked lists, each holding memory
	//	Just go through, deleting all the memory in each list
	//	that'll be enough to clean up all the memory;
	NetworkPacket *packet, *next;
	
	for(packet = m_empty, next = NULL; packet != NULL; packet = next){
		next = packet->next;
		delete packet;
	}
	
	for(packet = m_head, next = NULL; packet != NULL; packet = next){
		next = packet->next;
		delete packet;
	}
	
	unlock();
}

void BasicPacketManager::allocatePacket(void)
{
	//	This basic packet manager simply allocates each packet on request
	m_empty = new NetworkPacket();
	m_empty->prev = NULL;
	m_empty->next = NULL;
	m_empty->manager = this;
}

NetworkPacketLL * BasicPacketManager::findPacket(void)
{
	//	BasicPacketManager allocates packets on demand and has no caching
	//	So just allocate a new packet and use it directly
	allocatePacket();
	
	NetworkPacket *packet = m_empty;
	
	m_empty = NULL;
	
	return packet;
}

NetworkPacket * BasicPacketManager::requestPacket(void)
{
	NetworkPacket *packet = findPacket();

	//	If linked list is empty, this is the first packet, setup the list
	if(m_head == NULL){
		m_head = packet;
	}else{
		m_tail->next = packet;
		packet->prev = m_tail;
	}
	m_tail = packet;

	return m_tail;
}

//	Request the head of the linked list, unlink it and return
void BasicPacketManager::requestHead(void)
{
	NetworkPacket *head = m_head;
	m_head = m_head->next;
	m_head->prev = NULL;
	
	return head;
}

//	Requests the tail of the linked list
NetworkPacket * BasicPacketManager::requestTail(void)
{
	return m_tail;
}

void BasicPacketManager::returnPacket(NetworkPacket *packet)
{
	delete packet;
}

void BasicPacketManager::lock(void)
{
	pthread_mutex_lock(&m_lock);
}

void BasicPacketManager::unlock(void)
{
	pthread_mutex_unlock(&m_lock);
}

void BasicPacketManager::setCacheSize(unsigned int cache){}

IPacketManager & BasicPacketManager::operator=(IPacketManager &pm)
{
	NetworkPacket *pm_head = pm.requestHead();
	NetworkPacket *next = NULL;
	
	//	FIXME: Should I let packet managers steal packets from each other?
	//			would solve all the memory allocation, deallocation which happens
	
	//	Steal all the packets from the packet manager you are copying
	for(;pm_head != NULL; pm_head = next){
		pm_head = pm.requestHead();
		
		NetworkPacket *packet = requestPacket();
		
		packet->length = pm_head->length;
		packet->socketobj = pm_head->socketobj;
		memcpy(packet->data,pm_head->data,pm_head->length);
		
		pm.returnPacket(pm_head);
	}
	
	return *this;
}

virtual void BasicPacketManager::clear(void)
{
	deallocatePackets();
}
