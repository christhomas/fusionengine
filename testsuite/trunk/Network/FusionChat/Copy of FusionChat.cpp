#include <FusionChat.h>

Fusion f,*fusion = &f;

#ifdef _DEBUG
	std::string fusioncfg = "fusion_d.xml";
#else
	std::string fusioncfg = "fusion.xml";
#endif

FCState state;
std::vector<ChatSocket> sockets;

//	Builds a Chat packet to send across the network
ChatPacket * makePacket(std::string text)
{
	//	Allocate and set to zero a ChatPacket
	ChatPacket *packet = new ChatPacket;
	memset(packet,0,sizeof(ChatPacket));
	
	unsigned int len;
	
	//	Obtain, clip and copy a number of bytes from username
	len = (unsigned int)state.username.length();
	if(len > 256) len = 256;
	memcpy(packet->user,state.username.c_str(),len);
	
	//	Obtain, clip and copy a number of bytes from text
	len = (unsigned int)text.length();
	if(len > 8912) len = 8912;
	memcpy(packet->data,text.c_str(),len);
	
	//	Set the length of data in buffer and the packet length (constant)
	packet->dlen = (unsigned int)text.length();
	packet->plen = sizeof(ChatPacket);
	
	return packet;
}

void sendData(ChatPacket *packet, IClientSocket *src)
{
	//	Only send the packet, if it's got data inside it
	if(packet->dlen > 0){
		for(unsigned int a=0;a<sockets.size();a++){
			IClientSocket *socket = sockets[a].socket;
			
			//	Do not send to 
			//	a) the first socket (local client socket, only server broadcasts)
			//	b) the socket which originated the message
			if(a == 0) continue;
			if(src == socket) continue;
			
			socket->Send((char *)packet, packet->plen);
		}
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
void receiveData(IClientSocket *socket)
{
	ChatPacket *packet = NULL;
	
	//	Find the chat socket associated with the socket
	for(unsigned int a=0;a<sockets.size();a++){
		if(sockets[a].socket == socket) packet = &sockets[a].packet;
	}
	
	//	If you can't find it, it's cause it's not added to the array yet
	if(packet == NULL){
		//	Add a new chat socket and recurse (it'll find it next time)
		ChatSocket s;
		s.socket = socket;
		memset(&s.packet,0,sizeof(ChatPacket));
		sockets.push_back(s);
		//	recurse into this method again, to find the socket added to the list
		receiveData(socket);
	}
	
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
			serverMessage("/info,serverStart");
			state.server->Listen(state.port,5);
			serverMessage("/info,listening");
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
			
			serverMessage("/info,serverStop");
		}
	}
}

void disconnectClients(void)
{
	while(sockets.size() > 0){
		IClientSocket *client = sockets[0].socket;
		remoteMessage("/disconnect",client,true);
		client->Disconnect();
		sockets.erase(sockets.begin());
	}
}

bool processNetwork(void *)
{
	//	Receive data from the server
	if(state.enableClient == true){
		receiveData(state.client);
	}

	//	Process the server events
	if(state.enableServer == true){
		socketlist_t remote;
		//	Are there any waiting clients to connect?
		if(state.server->WaitForConnections(netLatency) == true){
			serverMessage("/info,newClient");
			remote = state.server->GetConnections();
		}
		
		//	Receive data from the clients
		for(unsigned int a=0;a<remote.size();a++){
			IClientSocket *s = (IClientSocket *)remote[a];
			receiveData(s);
		}
	}
	
	return true;
}

//	main function, runs the client side
int main(int argc, char **argv)
{
	//	Setup the initial app state	
	state.client = NULL;
	state.server = NULL;
	state.enableClient = false;
	state.enableServer = false;
	state.port = 5678;
	state.username = "fcuser";
	
	//	Init GTK+
	initGTK(argc, argv);
	
	//	Init Fusion
	fusion->LoadConfig(fusioncfg);
	fusion->InitSystem(Fusion::NETWORK);
	
	fusion->errlog.disableFile();
	fusion->errlog.enableConsole();

	//	Run GTK+
	runGTK((GSourceFunc)processNetwork);
	
	//	Finished :)
	return 0;
}