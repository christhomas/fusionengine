#include <network/NetworkCore.h>
#include <network/ClientSocket.h>
#include <network/ServerSocket.h>

NetworkCore::NetworkCore(){}
NetworkCore::~NetworkCore()
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

bool NetworkCore::Initialise(void)
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

IClientSocket * NetworkCore::CreateSocket(void)
{
    return new ClientSocket(this);
}

IServerSocket * NetworkCore::CreateServerSocket(void)
{
    return new ServerSocket(this);
}

unsigned int NetworkCore::ResolveHost(const char *ip)
{
    // for converting dns names to ip's
    char buffer[80];
    memset(buffer, 0, 80);
    hostent *host;

    // for storing the final ip address
    unsigned int address = -1;

    // convert the string to an ip
    address = inet_addr(ip);

    //      if the conversion didnt work, it's cause it has to be resolved first
    //      so this test will succeed if the address needs to be resolved
    if (address == -1) {
		// conver that hostname into a hostent structure
		host = gethostbyname(ip);

		if (host != NULL) {
			// extract the ip address from the hostent structure
			address = (*((in_addr *) host->h_addr)).s_addr;
		}
    }
    //      if the address was a valid ip string, it'll return a correct address
    //      if it's not, it'll attempt to resolve it, if that succeeds, it'll produce a valid ip address
    //      if both of those fail, it'll return -1, to signal error
    return address;
}



void NetworkCore::Send(NetworkPacket *packet)
{
    //	Lock/Unlock the send data stack
    LockSendStack();
    {
		//	Put the data onto the send stack
		m_senddata.push_back(packet);
    }
    UnlockSendStack();
}

NetworkPacket * NetworkCore::getNetworkPacket(void)
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
void NetworkCore::LockSockets(void)
{
    pthread_mutex_lock(&m_sockets_lock);
}

void NetworkCore::UnlockSockets(void)
{
    pthread_mutex_unlock(&m_sockets_lock);
}

void NetworkCore::LockSendStack(void)
{
    pthread_mutex_lock(&m_senddata_lock);
}

void NetworkCore::UnlockSendStack(void)
{
    pthread_mutex_unlock(&m_senddata_lock);
}

void NetworkCore::startThread(void)
{
	//	you call wait when you start the thread, cause it's initially set to a signalled state
	//	therefore it'll drop through the funtion without waiting, but it will reset the signal
	//	state of the event, therefore when you come to kill the thread, it will WAIT properly
	//	as opposed to dropping straight through that too, which would be bad.
	m_TerminateThread->wait();
}

void NetworkCore::killThread(void)
{
	if(m_destroy_threads == true || m_sockets.size() == 0){
		m_TerminateThread->signal();
		pthread_exit(0);
	}
}
