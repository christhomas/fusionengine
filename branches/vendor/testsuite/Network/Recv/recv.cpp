#include <iostream>
#include <Fusion.h>

Fusion f,*fusion = &f;

#ifdef _DEBUG
	std::string fusioncfg = "system/fusion_d.xml";
#else
	std::string fusioncfg = "system/fusion.xml";
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
	
	socketlist_t soclist = server->GetConnections();
	IClientSocket *client = (IClientSocket *)soclist[0];
	
	while(true){
		NetworkPacket *packet;
		packet = client->Receive();
	
		if(packet == NULL) break;
		
		std::cout << "data = " << packet->data << std::endl;
	}
	
	return 0;
}