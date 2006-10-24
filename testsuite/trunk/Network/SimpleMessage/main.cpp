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
	
	//	obtain a command from the user (send/recv are the only valid commands)
	std::string command;
	std::cout << "Enter Command (send recv): ";
	std::cin >> command;
	
	if(command == "send"){
		std::cout << "Create Client Socket" << std::endl;
		IClientSocket *client = fusion->Network->createSocket();
		
		std::cout << "Connecting to localhost" << std::endl;
		if(client->connect("localhost",1234) == true){
		
			char buffer[1000];
			sprintf(buffer,"This is your wake up call");
		
			std::cout << "Sending: " << buffer << std::endl;
			client->send(buffer,1000);
			
			NetworkPacket *packet = client->receive(10000);
			if(packet != NULL){
				std::cout << "Server says: " << packet->data << std::endl;
			}
			
			return 0;
		}
		
		std::cout << "Error, failed to connect" << std::endl;
		
	}else if(command == "recv"){
		std::cout << "Create Server Socket" << std::endl;
		IServerSocket *server = fusion->Network->createServerSocket();

		std::cout << "Listening on port" << std::endl;
		server->listen(1234,5);

		std::cout << "Waiting for connections" << std::endl;
		server->waitForConnections();

		std::cout << "Client connected" << std::endl;
		socketlist_t soclist = server->getConnections();
		IClientSocket *client = (IClientSocket *)soclist[0];

		std::cout << "Receiving data" << std::endl;
		//	Wait for the data to start crossing the network (for an infinite time)
		NetworkPacket *packet = client->receive();

		std::cout << "packet = " << packet << std::endl;
		std::cout << "packet->length = " << packet->length << std::endl;
		std::cout << "packet->data = " << packet->data << std::endl;
			
		while(true){
			if(packet == NULL) break;
			std::cout << "Data received" << std::endl;
			
			std::cout << "data = " << packet->data << std::endl;
			
			//	once data has arrived, cut the time to wait to 1 seconds
			//	so you have a reason to quit (when data has completed)
			packet = client->receive(1000);
		}

		client->send("QUIT",5,true);
		std::cout << "Quitting" << std::endl;		
		
		return 0;
	}
	
	return 1;
}