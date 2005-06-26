#include <network/WSANetworkCore.h>
#include <network/WSASocketEvents.h>
#include <fusion.h>

void * WSANetworkThread(void *data);

WSANetworkCore::WSANetworkCore(){}
WSANetworkCore::~WSANetworkCore(){}

void WSANetworkCore::initSocketEvents(void)
{
	m_socketEvents = new WSASocketEvents();
}

SMap * WSANetworkCore::addSocket(IClientSocket *socket, SOCKET s)
{
	//	Register read(recv)/connect/close events
	SMap *smap = addSocket(socket,s,FD_READ | FD_CONNECT | FD_CLOSE);
	smap->type = SMap::CLIENT_SOCKET;
	return smap;
}

SMap * WSANetworkCore::addSocket(IServerSocket *socket, SOCKET s)
{
	//	Register accept and close events
	SMap *smap = addSocket(socket,s,FD_ACCEPT | FD_CLOSE);
	smap->type = SMap::SERVER_SOCKET;
	return smap;
}

bool WSANetworkCore::startThread(void)
{
    //	If this is the first socket in the list, then either/or the network core's thread has
    //	a)	not been created yet
    //	b)	been suspended and need resuming
    if(m_socketEvents->numEvents() == 1){
		//	Create the network core thread
		int error;
		if((error = pthread_create(&m_Thread, NULL, WSANetworkThread, this)) != 0){
			if(error == EAGAIN) fusion->errlog << "The process lacks the resources to create another thread, or the total number of threads in a process would exceed  PTHREAD_THREADS_MAX." << std::endl;
			if(error == EINVAL)	fusion->errlog << "A value specified by attr is invalid." << std::endl;
			
			return false;
		}
		
		return true;
    }
    
    return false;
}

bool WSANetworkCore::error(void)
{	
	unsigned int e = WSAGetLastError();
	
	//	NOTE:	Copied pretty much directly from the FormatMessage example
	LPVOID lpMsgBuf;
	if(FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		e,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL ))
	{
		//	FormatMessage() was ok, lets
		fusion->errlog << "Network error: (" << e << "): " << (char *)lpMsgBuf << std::endl;
		// Free the buffer.
		LocalFree( lpMsgBuf );
	}
	
	switch(e){
		//	SEVERE, FATAL ERROR
		case 0:{
			return true;		
		}break;
		
		//	NON-SEVERE, RETRY ERROR
		case 1:
		case WSAEWOULDBLOCK:{
			return false;
		}break;
	};
	
	//	Unknown error value, return fatal
	return true;
}