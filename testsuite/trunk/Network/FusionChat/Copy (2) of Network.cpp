#include <FusionChat.h>

FCState state;
std::vector<ChatSocket> sockets;
unsigned int netLatency = 50;

//	Builds a Chat packet to send across the network
char * makePacket(std::string text)
{
	//	Allocate and set to zero a ChatPacket
	unsigned int length = sizeof(unsigned int)+text.length()+1;
	char *packet = new char[length];
	memset(packet,0,length);
	unsigned int offset = 0;
	memcpy(&packet[offset],&length,sizeof(unsigned int));
	offset+=sizeof(unsigned int);
	memcpy(&packet[offset],text.c_str(),text.length());
			
	return packet;
}

void sendData(char *packet, IClientSocket *dst, bool wait)
{
	if(packet->dlen > 0 && dst != NULL){
		dst->Send((const char *)packet,packet->plen,wait);
	}
}

void castData(char *packet, IClientSocket *ignore)
{
	for(unsigned int a=0;a<sockets.size();a++){
		IClientSocket *s = sockets[a].socket;
		
		//	ignore the client who sent this message (dont bounce it back to them)
		if(ignore == s) continue;
		if(state.client == s) continue;
		
		//	send to everyone other than them
		sendData(packet,s);
	}
}

/**	Receive Data from network

	This method is required to be built like this, cause it's
	more than likely you can't depend on whole ChatPacket
	structures coming through the network in one piece
	so you have to figure a way to build a ChatPacket out of
	the NetworkPacket structures that are extracted from the network
	
	This means figuring out where a ChatPacket's data begins and ends
	within the NetworkPacket structure and splitting the data accordingly
**/
void receiveData(void)
{
	for(unsigned int a=0;a<sockets.size();a++) receiveData(&sockets[a]);	
}

void receiveData(IClientSocket *socket)
{
	ChatPacket *packet = &chatSocket->packet;
	
	if(packet != NULL){
		NetworkPacket *recv = socket->Receive(netLatency);
		if(recv != NULL){
			//	Buffer control variables
			//	src
			unsigned int slen = recv->length;
			unsigned int soff = 0;
			//	dst
			unsigned int dlen = 0;			
			unsigned int doff = packet->plen;
			unsigned int dmax = sizeof(ChatPacket);
			
			do{
				char *src = recv->data;
				char *dst = (char *)packet;
				
				//
				//	buffer maths here
				//
				
				//	Max data allowed to fill the packet
				dlen = dmax - doff;
								
				//	Limit the max, if less data is available in the NetworkPacket
				if(slen < dlen) dlen = slen;
				
				//	copy the data across
				memcpy(&dst[doff],&src[soff],dlen);
				
				//	Update the buffer control variables
				slen -= dlen;	//	Decrease the amount of data left
				soff += dlen;	//	Increase the src buffer position
				doff += dlen;	//	Increase the dst buffer position
				
				if(doff == dmax){
					//	NOTE: You have a full ChatPacket here, do something with it
					commandMessage(packet->data,socket);
					
					packet->plen = 0;
					doff = 0;
				}
			}while(slen > 0);
			
			delete recv;
		}
	}	
}

//	Used to convert a socket address in memory, to an idstring, 
//	so the client can identify itself and use it to be identified (if necessary)
#include <iomanip>
std::string idstring(void *socket)
{
	std::stringstream conv;
	conv << std::setfill('0') << std::setw(8) << socket;
	return conv.str();
}

void startServer(void)
{
	if(state.enableServer == false && state.enableClient == false){
		//	If the server socket is null, this is the first 
		//	time you started the server so create a new socket
		if(state.server == NULL){
			state.server = fusion->Network->CreateServerSocket();
		}
		
		//	If the socket was created ok, lets start listening for clients
		if(state.server != NULL){
			commandMessage("/info,serverStart");
			state.server->Listen(state.port,5);
			commandMessage("/info,listening");
		}
		
		state.enableServer = true;
	}
}

void stopServer(void)
{
	if(state.enableServer == true){
		state.enableServer = false;
		
		if(state.server != NULL){
			disconnectClients();
			state.server->Disconnect();
			
			commandMessage("/info,serverStop");
		}
	}
}

void addClient(IClientSocket *socket)
{
	state.remote.push_back(socket);
}

void disconnectClients(void)
{
	while(sockets.size() > 0){
		IClientSocket *client = sockets[0].socket;
		remoteMessage("/disconnect",client,true);
		client->Disconnect();
		state.remote.erase(state.remote.begin());
	}	
}

bool processNetwork(void *)
{
	//	Process the server events
	if(state.enableServer == true){
		//	Are there any waiting clients to connect?
		if(state.server->WaitForConnections(netLatency) == true){
			commandMessage("/info,newClient");
			
			socketlist_t remote = state.server->GetConnections();
			
			//	Add any new connections to the sockets array
			for(unsigned int a=0;a<remote.size();a++){
				bool found = false;
				for(unsigned int b=0;b<sockets.size();b++){
					if(remote[a] == sockets[b].socket){
						found = true;
						break;
					}
				}
				
				if(found == false) state.remote.push_back(remote[a]);
			}
		}
	}
	
	receiveData();
	
	return true;
}