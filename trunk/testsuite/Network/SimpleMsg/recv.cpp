#include <iostream>
#include <Fusion.h>

Fusion f,*fusion = &f;

#ifdef _DEBUG
	std::string fusioncfg = "fusion_d.xml";
#else
	std::string fusioncfg = "fusion.xml";
#endif

int main(int argc, char **argv)
{
	fusion->LoadConfig(fusioncfg);
	fusion->InitSystem(Fusion::NETWORK);
	
	std::cout << "Create Server Socket" << std::endl;
	IServerSocket *server = fusion->Network->CreateServerSocket();

	std::cout << "Listening on port" << std::endl;
	server->Listen(1234,5);
	
	std::cout << "Waiting for connections" << std::endl;
	server->WaitForConnections();
	
	std::cout << "Client connected" << std::endl;
	socketlist_t soclist = server->GetConnections();
	IClientSocket *client = (IClientSocket *)soclist[0];
	
	std::cout << "Receiving data" << std::endl;
	//	Wait for the data to start crossing the network (for an infinite time)
	NetworkPacket *packet = client->Receive();
	
	std::cout << "packet = " << packet << std::endl;
	std::cout << "packet->length = " << packet->length << std::endl;
	std::cout << "packet->data = " << packet->data << std::endl;
	std::cout << "packet->socket = " << packet->socket << std::endl;
		
	while(true){
		if(packet == NULL) break;
		std::cout << "Data received" << std::endl;
		
		std::cout << "data = " << packet->data << std::endl;
		
		//	once data has arrived, cut the time to wait to 1 seconds
		//	so you have a reason to quit (when data has completed)
		packet = client->Receive(1000);
	}
	
	client->Send("QUIT",5,true);
	std::cout << "Quitting" << std::endl;
	
	return 0;
}