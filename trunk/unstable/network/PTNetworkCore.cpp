#include <PTNetworkCore.h>
#include <PTClientSocket.h>
#include <PTServerSocket.h>

PTNetworkCore::PTNetworkCore(){}
PTNetworkCore::~PTNetworkCore()
{
	//	trigger the threads to quit
    m_destroy_threads = true;

    //===================================
    //      Clear the send data stack
    //===================================
    m_senddata.clear();

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
}

bool PTNetworkCore::Initialise(void)
{
	m_Thread = NULL;
	m_ThreadID = 0;
	
	m_SendEvent = new Event();

	//	Trigger + Events to terminate the threads when object dtor is called
	m_destroy_threads = false;
	m_TerminateThread = new Event(1);

	//	Initialise all Critical sections
	pthread_mutex_init(&m_sockets_lock, NULL);
	pthread_mutex_init(&m_senddata_lock, NULL);

	return true;
}

IClientSocket * PTNetworkCore::CreateSocket(void)
{
    return new PTClientSocket(this);
}

IServerSocket * PTNetworkCore::CreateServerSocket(void)
{
    return new PTServerSocket(this);
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
