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
	
	std::cout << "Create Client Socket" << std::endl;
	IClientSocket *socket = fusion->Network->CreateSocket();
	
	std::cout << "Connecting to localhost" << std::endl;
	if(socket->Connect("localhost",1234) == true){
	
		char buffer[1000];
		sprintf(buffer,"This is your wake up call");
	
		std::cout << "Sending: " << buffer << std::endl;
		socket->Send(buffer,1000);
		
		while(true){}
		
		return 0;
	}
	
	std::cout << "Error, failed to connect" << std::endl;
	
	return 1;
}