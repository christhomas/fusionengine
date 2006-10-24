#include <Win32NetworkCore.h>
#include <Win32ClientSocket.h>
#include <Win32ServerSocket.h>

DWORD WINAPI NetworkCoreThread(void *data)
{
    //	Generic Thread variables
	Win32NetworkCore	*network = (Win32NetworkCore *)data;
	Win32SocketEvents	*socket_events = network->getSocketEvents();
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
											
			if (event_id != Win32SocketEvents::SEND_TRIGGER){
				if(event_id == Win32SocketEvents::QUIT_TRIGGER) continue;
				
				ISocket *socket = socket_events->m_sockets[event_id];
				
				WSAEnumNetworkEvents(socket->m_socket, socket_events->m_events[event_id], &network_event);
				if(network_event.lNetworkEvents & FD_ACCEPT){
					//      Process server sockets
					Win32ServerSocket *server = (Win32ServerSocket *)socket;

					IClientSocket *client = new Win32ClientSocket(network);
					
					client->Connect(server->m_socket);

					network->AddSocket(client, FD_READ);
					server->AddConnection(client);
					server->SignalConnect();					
				}else if(network_event.lNetworkEvents & FD_READ){
					//	Process client sockets
					Win32ClientSocket *client = (Win32ClientSocket *)socket;

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
					int bytes_sent =0;
					int offset = 0;
					
					while(packet->length > 0){
						bytes_sent = send(packet->socket, &packet->data[offset],packet->length, 0);
						
						if(bytes_sent > 0){
							offset += bytes_sent;
							packet->length -= bytes_sent;
						}
					}
					
					if(packet->socketobj != NULL){
						Win32ClientSocket *client = (Win32ClientSocket *)packet->socketobj;
						client->SignalSend();
					}
					
					delete packet;
				}
				
				socket_events->ResetSendEvent();

				/*
				//      Send data across network
				while (network->m_senddata.size() > 0) {
					delete send_packet;

					//      lock senddata stack
					network->LockSendStack();
					{
						send_packet = network->m_senddata[0];
						network->m_senddata.erase(network->m_senddata.begin());
					}
					network->UnlockSendStack();
					//      unlock senddata stack

					int bytes_sent = 0;
					int offset = 0;

					while (send_packet->length > 0) {
						bytes_sent = send(send_packet->socket,&send_packet->data[offset],send_packet->length, 0);

						if (bytes_sent > 0) {
							offset += bytes_sent;
							send_packet->length -= bytes_sent;
						}
					}
					
					if(send_packet->socketobj != NULL){
						Win32ClientSocket *client = (Win32ClientSocket *)send_packet->socketobj;
						client->SignalSend();
					}
				}
				
				WSAResetEvent(network->m_network_events.m_events[0]);
				*/
			}	//     if(event_id!=0)'s else statement
		}		//     WSAWaitForMultipleEvents
    }			//     While loop
}
