#include <PTNetworkCore.h>
#include <PTClientSocket.h>
#include <PTServerSocket.h>

PTNetworkCore::PTNetworkCore(void){}

#include <iostream>
PTNetworkCore::~PTNetworkCore()
{
	//	trigger the threads to quit
    m_destroy_threads = true;

    //===================================
    //      Clear the send data stack
    //===================================
    m_senddata.clear();
    
    //===================================
    //      Clear the socket queue
    //===================================
    while(m_sockets.size() > 0) RemoveSocket(m_sockets[0]);
    	
	//	Trigger the quit event (if the thread is still running, it'll unblock)
	m_network_events.SetQuitEvent();

    //	Wait until all the threads exit
	m_TerminateThread->wait();

    //==============================
    //	Close all the event handles
    //==============================

    //	Close the send/recv event signals
    delete m_SendEvent;
	
    //	Close the thread termination events
    delete m_TerminateThread;

    //	Close the thread handles
    pthread_join(m_Thread,NULL);

    //===================================
    //	Delete all the mutexes
    //===================================
    pthread_mutex_destroy(&m_sockets_lock);
    pthread_mutex_destroy(&m_senddata_lock);

    //===================================
    //	Close the WSA Networking system
    //===================================
    WSACleanup();
}

bool PTNetworkCore::Initialise(void)
{
    if (WSAStartup(WINSOCK_VERSION, &m_WSAData) == 0){
	    m_Thread = NULL;
		m_ThreadID = 0;
		
		m_SendEvent = new Event(false);

		//	Trigger + Events to terminate the threads when object dtor is called
		m_destroy_threads = false;
		m_TerminateThread = new Event(true);

		//	Initialise all Critical sections
		pthread_mutex_init(&m_sockets_lock, NULL);
		pthread_mutex_init(&m_senddata_lock, NULL);

		return true;
    }
    // failure
    return false;
}

IClientSocket * PTNetworkCore::CreateSocket(void)
{
    return new PTClientSocket(this);
}

IServerSocket * PTNetworkCore::CreateServerSocket(void)
{
    return new PTServerSocket(this);
}


void PTNetworkCore::AddSocket(ISocket *socket, int events)
{
    //	Lock/Unlock the server sockets
    LockSockets();
    {
		//	Store the socket and create the events it requested
		m_sockets.push_back(socket);
		m_network_events.AddEvent(socket, events);
    }
    UnlockSockets();

    //	If this is the first socket in the list, then either/or the network core's thread has
    //	a)	not been created yet
    //	b)	been suspended and need resuming
    if (m_sockets.size() == 1) {
		//	Create the network core thread
		m_ThreadID = pthread_create(&m_Thread, NULL, PTNetworkCoreThread, this);
    }
}

bool PTNetworkCore::RemoveSocket(ISocket *socket)
{
    bool success = false;

    //	Lock/Unlock the client sockets
    LockSockets();
    {
		//	Loop through the list of available sockets until you can match the ptr
		for (unsigned int a = 0; a < m_sockets.size(); a++){
			//	If you find the ptr
			if (m_sockets[a] == socket){
				//	erase the socket from the list and remove it's event
				m_network_events.RemoveEvent(m_sockets[a]);
				m_sockets.erase(m_sockets.begin() + a);
				success = true;
				break;
			}
		}
    }	//	Remember you've locked the sockets, so you have to unlock it here, or trouble ensues
    UnlockSockets();

    //	return whether you were successful of not
    return success;
}

void PTNetworkCore::Send(NetworkPacket *packet)
{
    //	Lock/Unlock the send data stack
    LockSendStack();
    {
		//	Put the data onto the send stack
		m_senddata.push_back(packet);
    }
    UnlockSendStack();
    
    m_network_events.SetSendEvent();
}

Win32SocketEvents * PTNetworkCore::getSocketEvents(void)
{
	return &m_network_events;
}

NetworkPacket * PTNetworkCore::getNetworkPacket(void)
{
	NetworkPacket *packet = NULL;
	LockSendStack();
	{
		if(m_senddata.size() > 0){
			packet = m_senddata[0];
			m_senddata.erase(m_senddata.begin());
		}
	}
	UnlockSendStack();
	
	return packet;
}

/*
*	MUTEX/THREAD METHODS
*/
void PTNetworkCore::LockSockets(void)
{
    pthread_mutex_lock(&m_sockets_lock);
}

void PTNetworkCore::UnlockSockets(void)
{
    pthread_mutex_unlock(&m_sockets_lock);
}

void PTNetworkCore::LockSendStack(void)
{
    pthread_mutex_lock(&m_senddata_lock);
}

void PTNetworkCore::UnlockSendStack(void)
{
    pthread_mutex_unlock(&m_senddata_lock);
}

void PTNetworkCore::startThread(void)
{
	//	you call wait when you start the thread, cause it's initially set to a signalled state
	//	therefore it'll drop through the funtion without waiting, but it will reset the signal
	//	state of the event, therefore when you come to kill the thread, it will WAIT properly
	//	as opposed to dropping straight through that too, which would be bad.
	m_TerminateThread->wait();
}

void PTNetworkCore::killThread(void)
{
	if(m_destroy_threads == true || m_sockets.size() == 0){
		m_TerminateThread->signal();
		pthread_exit(0);
	}
}
