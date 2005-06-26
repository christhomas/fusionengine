#include <network/NetworkCore.h>
#include <network/ClientSocket.h>
#include <network/ServerSocket.h>

#include <iostream>
void * BSDNetworkThread(void *data)
{
	NetworkCore *network = (NetworkCore *)data;
	ISocketEvents *socketEvents = network->getSocketEvents();
	socketmap_t *sockmap = (socketmap_t *)socketEvents->getSockets();
	fd_set *events;
	
	network->initThread();
	
	while(true){
		network->killThread();
		
		events = (fd_set *)socketEvents->getEvents();
		
		std::cout << "select()" << std::endl;
		int num = select(0,events,NULL,NULL,NULL);
		
		if(num > 0){
			std::cout << "Something signalled" << std::endl;
			for(unsigned int a=0;a<events->fd_count;a++){
				if(FD_ISSET(events->fd_array[a],&events)){
					SOCKET socket = events->fd_array[a];
					
					SMap *s = (*sockmap)[socket];
					
					if(s->type == SMap::CLIENT_SOCKET){
						std::cout << "Client socket" << std::endl;
						ClientSocket *client = (ClientSocket *)s->socketobj;
						client->threadProcess(socket);
					}else{
						std::cout << "Server socket" << std::endl;
						ServerSocket *server = (ServerSocket *)s->socketobj;
						
						ClientSocket *client = new ClientSocket(network);
						client->threadConnect(socket,server->getPort());
										
						server->addConnection(client);
						server->emitSignal();
					}					
				}
			}
		}else{
			//	The value of num is SOCKET_ERROR, find out why
			network->error();
		}
		std::cout << "loop" << std::endl;
	}
	
	return NULL;
}
