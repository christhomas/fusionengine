#include <network/NetworkCore.h>
#include <network/ClientSocket.h>
#include <network/ServerSocket.h>
#include <network/BSDSocketEvents.h>

#include <network/BasicPacketManager.h>
//#include <network/BlockPacketManager.h>

void * BSDNetworkThread(void *);

/*	EXPLANATION: m_TerminateThread event

	The terminate thread has to be handled correctly, since there are two possible situations
	the code can be in, You have to make sure that destroying this object, will not block 
	waiting for the event to signal:
	
	1)	where this object is created, but the thread has not been started
		
		so you set the event to be signalled by default, then if you dont create the thread, but destroy the
		object, it'll wait(), but drop straight through, cause it's already signalled.
			
	2)	where the object is created and the thread started.
		
		if you create the thread, it'll wait on the signal but drop straight through, cause it's already signalled
		then if you want to destroy this object, the destructor sets m_destroy_threads to true and call 
		socketEvents->sendQuitSignal() which unblocks the wait state inside the thread and allows it to cycle 
		around, where it'll enter killThread, which in turn, signals the event (unblocking it) and destroys the thread
*/

NetworkCore::NetworkCore(){}

NetworkCore::~NetworkCore()
{
	//	trigger the threads to quit
    m_destroy_threads = true;
    m_socketEvents->setBreakEvent();

    //	Wait until all the threads exit
	m_TerminateThread->wait();
	
	clearSockets();
	delete m_socketEvents;
	
    //	Close the thread termination events
    delete m_TerminateThread;

    //	Close the thread handles
    pthread_join(m_Thread,NULL);

    //	Delete all the mutexes
    pthread_mutex_destroy(&m_sockets_lock);
}

bool NetworkCore::Initialise(void)
{
	m_Thread = NULL;
	m_ThreadID = 0;
	
	//	Trigger + Events to terminate the threads when object dtor is called
	m_destroy_threads = false;
	m_TerminateThread = new Event(1);

	//	Initialise all Critical sections
	pthread_mutex_init(&m_sockets_lock, NULL);
	
	initSocketEvents();
	
	return true;
}

void NetworkCore::initSocketEvents(void)
{
	m_socketEvents = new BSDSocketEvents();
}

ISocketEvents * NetworkCore::getSocketEvents(void)
{
	return m_socketEvents;
}

IClientSocket * NetworkCore::createSocket(void)
{
    return new ClientSocket(this);
}

IServerSocket * NetworkCore::createServerSocket(void)
{
    return new ServerSocket(this);
}

SMap * NetworkCore::addSocket(IClientSocket *socket, SOCKET s)
{
	return addSocket(socket,s,SMap::CLIENT_SOCKET);
}

SMap * NetworkCore::addSocket(IServerSocket *socket, SOCKET s)
{
	return addSocket(socket,s,SMap::SERVER_SOCKET);
}

SMap * NetworkCore::addSocket(ISocket *socket, SOCKET s, int type)
{
	SMap *smap = NULL;
	
    //	Lock/Unlock the server sockets
    pthread_mutex_lock(&m_sockets_lock);
    {
		smap = m_socketEvents->addEvent(socket,s,type);
    }
    pthread_mutex_unlock(&m_sockets_lock);
    
	//	FIXME:	A true/false value is returned, you should probably do something with it
    startThread();
    
    return smap;
}

bool NetworkCore::removeSocket(ISocket *socket)
{
	bool success = false;

    //	Lock/Unlock the client sockets
    pthread_mutex_lock(&m_sockets_lock);
    {
		success = m_socketEvents->removeEvent(socket);
    }
    pthread_mutex_unlock(&m_sockets_lock);

    //	return whether you were successful of not
    return success;
}

void NetworkCore::clearSockets(void)
{
	pthread_mutex_lock(&m_sockets_lock);
	{
		m_socketEvents->clearEvents();
	}
	pthread_mutex_unlock(&m_sockets_lock);
}

void NetworkCore::socketSend(IClientSocket *socket)
{
	pthread_mutex_lock(&m_sockets_lock);
	{
		((ClientSocket *)socket)->threadSend();
	}
	pthread_mutex_unlock(&m_sockets_lock);
}

void NetworkCore::socketReceive(IClientSocket *socket)
{
	pthread_mutex_lock(&m_sockets_lock);
	{
		//	Process client sockets
		((ClientSocket *)socket)->threadReceive();
	}
	pthread_mutex_unlock(&m_sockets_lock);
}

void NetworkCore::initThread(void)
{	
	//	See EXPLANATION: m_TerminateThread event
	m_TerminateThread->wait();
}

bool NetworkCore::startThread(void)
{
    //	If this is the first socket in the list, then either/or the network core's thread has
    //	a)	not been created yet
    //	b)	been suspended and need resuming
    if(m_socketEvents->numEvents() == 1){
		//	Create the network core thread
		int error;
		if((error = pthread_create(&m_Thread, NULL, BSDNetworkThread, this)) != 0){
			if(error == EAGAIN) fusion->errlog << "The process lacks the resources to create another thread, or the total number of threads in a process would exceed  PTHREAD_THREADS_MAX." << std::endl;
			if(error == EINVAL)	fusion->errlog << "A value specified by attr is invalid." << std::endl;
			
			return false;
		}
		
		return true;
    }
    
    return false;
}

bool NetworkCore::killThread(void)
{
	if(m_destroy_threads == true || m_socketEvents->numEvents() == 0){
		m_TerminateThread->signal();
		pthread_exit(0);
		return true;
	}
	
	return false;
}

bool NetworkCore::error(void)
{
	//	Dont know if this is useful from a BSD point of view, perhaps I can use it 
	//	as a central function for outputting error messages to fusion->errlog?
	
	return true;
}
