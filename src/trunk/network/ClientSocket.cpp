#include <network/ClientSocket.h>
#include <network/ISocketEvents.h>

ClientSocket::ClientSocket(NetworkCore *network)
{
    m_network = network;
    m_events = m_network->getSocketEvents();
    
    m_smap = NULL;
    
    m_Connected = false;
    m_ready = false;
    
	m_recvEvent = new Event();
	m_sendEvent = new Event(); 
	m_connectEvent = new Event();
}

ClientSocket::~ClientSocket()
{
	disconnect();
	
	m_network->removeSocket(this);
	
	delete m_sendManager;
	delete m_recvManager;
	    
    delete m_recvEvent;
    delete m_sendEvent;	
    delete m_connectEvent;
}

void ClientSocket::setPacketManager(unsigned int type)
{
	IPacketManager *pm;
	
	pm = new BasicPacketManager(m_sendManager);
	delete m_sendManager;
	m_sendManager = pm;
	
	pm = new BasicPacketManager(m_recvManager);
	delete m_recvManager;
	m_recvManager = pm;
}

bool ClientSocket::connect(std::string ip, int port, unsigned int timeout)
{
	return connect(ip.c_str(),port, timeout);
}

bool ClientSocket::connect(const char *ip, int port, unsigned int timeout)
{    
    if(connected() == false){
		if( (m_rsocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET){
			return false;
		}

		//	Setup a sockaddr with some basic information	
		sockaddr_in socket_info;		
		socket_info.sin_family = AF_INET;
		socket_info.sin_port = htons(port);
		memset(&socket_info.sin_zero, 0, 8);
		
		//	Setup the address you want to connect to
		socket_info.sin_addr.s_addr = resolveHost(ip);
		//	Store all relevant information here, so you can use it later
		m_address = ip;
		m_ipaddress = (unsigned int) socket_info.sin_addr.s_addr;
		m_port = port;
		
		//	Connect to the remote machine
		if(::connect(m_rsocket,(sockaddr *)&socket_info,sizeof(sockaddr)) == SOCKET_ERROR){
			m_network->error();
			disconnect();
			return false;
		}
		
		if(m_ready == false){
			//	Add the socket to the network core
			m_smap = m_network->addSocket(this, m_rsocket);
			
			//	If no packet manager is available, create a BasicPacketManager
			if(m_sendManager == NULL || m_recvManager == NULL){
				setPacketManager(BASIC_PACKET_MANAGER);
			}
			
			m_ready = true;
		}
		
		//	Wait for the connection to accept and connect fully
		//	if this fails, the connection fails, regardless of whether
		//	it connects AFTER the timeout, the whole point is you 
		//	are giving a maximum time to reply, or fuck off
		if(m_connectEvent->timedwait(timeout) == ETIMEDOUT){
			disconnect();
			return false;
		}
		
		return true;
    }

    return false;
}

bool ClientSocket::disconnect(void)
{
    if (m_Connected == true) {
    	if (closesocket(m_rsocket) == SOCKET_ERROR) return false;
	
		setConnected(false);
		return true;
    }
    
    return false;
}

bool ClientSocket::disconnect(bool deleteSocket)
{
	//	Signals the server socket wants to delete this object
	//	after it's disconnected itself
	m_smap->sock = INVALID_SOCKET;

	return disconnect();
}

bool ClientSocket::connected(void)
{
    return m_Connected;
}

void ClientSocket::setConnected(bool status)
{
    m_Connected = status;
}

bool ClientSocket::send(const char *data, int length, bool wait)
{
	/*	FIXME:	Should a socket, have a sync/async property, 
				so you can set a socket to always wait, or 
				never wait for completion of sending data?
	*/
	
	if(connected() == false) return false;
	
	unsigned int offset = 0;	
	
    while (length > 0) {
		NetworkPacket *packet = new NetworkPacket;
		packet->socketobj = NULL;

		if (length > MAX_SEND) {
			packet->length = MAX_SEND;
		} else {
			packet->length = length;
			//	this is the last packet in the send block
			//	so set the socketobj, to notify the last packet has
			//	sent
			packet->socketobj = this;
		}

		length -= packet->length;

		memset(packet->data,0,MAX_SEND);
		memcpy(packet->data, &data[offset], packet->length);

		pthread_mutex_lock(&m_sendStack_lock);
		{
			m_sendStack.push_back(packet);
		}
		pthread_mutex_unlock(&m_sendStack_lock);
				
		offset+=length;
    }
    
	//	Signal the thread data is waiting
	m_events->setSendEvent(m_smap);    
    
    //	wait for thread to send all data and release the event
    if(wait == true) m_sendEvent->wait();
    //	Reset the signal
    //	FIXME: Is this needed anymore? or will it cause trouble?
    m_sendEvent->signal();
    
    return true;
}

NetworkPacket * ClientSocket::receive(unsigned int milliseconds)
{
    NetworkPacket *packet = NULL;
    
	if(m_recvEvent->timedwait(milliseconds) != ETIMEDOUT){		
		m_recvManager->lock();
		packet = m_recvManager->requestHead();
		m_recvManager->unlock();
		
		pthread_mutex_lock(&m_recvStack_lock);
		{
			if (m_recvStack.size() > 0){
				packet = m_recvStack[0];
				m_recvStack.erase(m_recvStack.begin());
			}
		}
		pthread_mutex_unlock(&m_recvStack_lock);
    }

    return packet;
}

std::string ClientSocket::getAddress(void)
{
	return m_address;
}

unsigned int ClientSocket::getIP(void)
{
	return m_ipaddress;
}

unsigned int ClientSocket::getPort(void)
{
	return m_port;
}

unsigned int ClientSocket::resolveHost(const char *ip)
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

void ClientSocket::threadConnect(void)
{
	setConnected(true);
	
	//	Signal ClientSocket::connect() to unblock
	m_connectEvent->signal();
}

bool ClientSocket::threadConnect(SOCKET s, unsigned int port)
{
	if(m_Connected == false){
		m_rsocket = s;
		
		m_smap = m_network->addSocket(this, m_rsocket);

		setConnected(true);
		
		return true;
	}
	
	return false;
}

void ClientSocket::threadDisconnect(void)
{
	m_network->removeSocket(this);
	disconnect();
	m_ready = false;
	
	//	Clear out the send/recv data lists
	m_sendManager->clear();
	m_recvManager->clear();
}

void ClientSocket::threadReceive(void)
{
	m_recvManager->lock();
	NetworkPacket *packet = m_recvManager->requestPacket();
	memset(packet->data, 0, MAX_RECV);
	m_recvManager->unlock();
	
	packet->length = recv(m_rsocket, packet->data, MAX_RECV, 0);
	
	if(packet->length > 0){
		m_recvEvent->signal();
	}else{
		if(packet->length == 0){
			//	remote disconnect
			fusion->errlog << "Remote disconnect occured" << std::endl;
			disconnect();
		}else{
			//	length < 0, error
			m_network->error();
		}
	}
}

void ClientSocket::threadSend(void)
{								
	NetworkPacket *packet;
	
	//	If not connected anymore, clear out the send stack
	//	FIXME:	Slow as hell, where is my PacketManager code
	//			I wanted last week?
	if(m_Connected == false){
		while(m_sendStack.size() > 0){
			delete m_sendStack[0];
			m_sendStack.erase(m_sendStack.begin());
		}
	}
	
	//	FIXME:	Is it safe to call .size() outside of the mutex?
	while(m_sendStack.size() > 0){
		pthread_mutex_lock(&m_sendStack_lock);
		{
			packet = m_sendStack[0];
			m_sendStack.erase(m_sendStack.begin());
		}
		pthread_mutex_unlock(&m_sendStack_lock);
		
		int bytes_sent = 0;
		int offset = 0;
		
		while(packet->length > 0){
			bytes_sent = ::send(m_rsocket, &packet->data[offset],packet->length, 0);
			
			if(bytes_sent > 0){
				offset += bytes_sent;
				packet->length -= bytes_sent;
			}else{
				//	send returned 0, this means an error
				
				//	You had a problem sending this packet, put it back
				pthread_mutex_lock(&m_sendStack_lock);
				{
					m_sendStack.insert(m_sendStack.begin(), packet);
				}
				pthread_mutex_unlock(&m_sendStack_lock);				
				
				if(m_network->error() == true){
					//	Holy shit! get back to the chopper!!!
					disconnect();
				}else{
					//	Non-fatal error, attempt to send again
					m_events->setSendEvent(m_smap);
				}
				
				return;				
			}
		}
		
		if(packet->socketobj != NULL) m_sendEvent->signal();
		
		delete packet;
	}
}

void ClientSocket::threadProcess(SOCKET s)
{
//	FIXME: to fix this, make a BSDClientSocket derived class and put all this into there
/*	if(s == m_rsocket){
		//	Network recv'd data
		
		threadReceive();		
	}else if(s == m_lsocket){
		//	You requested a send
		
		threadSend();		
	}*/
}
