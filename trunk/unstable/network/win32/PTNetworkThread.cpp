#include <WSANetworkCore.h>
#include <PTClientSocket.h>
#include <PTServerSocket.h>

void * PTNetworkThread(void *data)
{
	//      Generic Thread variables
	WSANetworkCore		*network = (WSANetworkCore *)data;
	WSASocketEvents		*socket_events = network->getSocketEvents();
	NetworkPacket		*send_packet = NULL;
	WSANETWORKEVENTS	network_event;
	int event_id;
	
	network->startThread();

    while (true) {
		//	This will either kill the thread, or return ok
		network->killThread();
		
		if ((event_id = WSAWaitForMultipleEvents(
								socket_events->numEvents(),	//      The number of event to wait on
								socket_events->getEvents(),	//      the array of events you´re waiting on
								false,						//      dont wait until ALL have occured
								INFINITE,					//      timeout value for the wait state
								false))						//      whether I should alert anybody :P (IO Completion ports?)
								!= WSA_WAIT_TIMEOUT)
		{
											
			if (event_id != WSASocketEvents::SEND_TRIGGER){
				if(event_id == WSASocketEvents::QUIT_TRIGGER)	continue;
				
				ISocket *socket = socket_events->m_sockets[event_id];
				
				WSAEnumNetworkEvents(socket->m_socket, socket_events->m_events[event_id], &network_event);
				if(network_event.lNetworkEvents & FD_ACCEPT){
					//	Process server sockets
					PTServerSocket *server = (PTServerSocket *)socket;

					IClientSocket *client = new PTClientSocket(network);
				
					client->Connect(server->m_socket);

					network->AddSocket(client, FD_READ);
					server->AddConnection(client);					
					server->SignalConnect();
				}else if(network_event.lNetworkEvents & FD_READ){
					//	Process client sockets
					PTClientSocket *client = (PTClientSocket *)socket;

					network->LockSockets();
					{
						client->socketReceive();
					}
					network->UnlockSockets();
				}
			}else{
				NetworkPacket *packet;

				for(packet = network->getNetworkPacket();packet!=NULL;packet=network->getNetworkPacket())
				{
					int bytes_sent = 0;
					int offset = 0;
					
					while(packet->length > 0){
						bytes_sent = send(packet->socket, &packet->data[offset],packet->length, 0);
						
						if(bytes_sent > 0){
							offset += bytes_sent;
							packet->length -= bytes_sent;
						}
					}
					
					if(packet->socketobj != NULL){
						PTClientSocket *client = (PTClientSocket *)packet->socketobj;
						client->SignalSend();
					}
					
					delete packet;
				}
				
				socket_events->ResetSendEvent();
				
			}	//     if(event_id!=0)'s else statement
		}		//     WSAWaitForMultipleEvents
    }			//     While loop
}
