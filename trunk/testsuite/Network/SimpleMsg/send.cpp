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
	
	std::cout << "Create Client Socket" << std::endl;
	IClientSocket *client = fusion->Network->CreateSocket();
	
	std::cout << "Connecting to localhost" << std::endl;
	if(client->Connect("localhost",1234) == true){
	
		char buffer[1000];
		sprintf(buffer,"This is your wake up call");
	
		std::cout << "Sending: " << buffer << std::endl;
		client->Send(buffer,1000);
		
		NetworkPacket *packet = client->Receive(10000);
		if(packet != NULL){
			std::cout << "Server says: " << packet->data << std::endl;
		}
		
		return 0;
	}
	
	std::cout << "Error, failed to connect" << std::endl;
	
	return 1;
}