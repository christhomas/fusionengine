#include <Win32NetworkCore.h>
#include <Win32ClientSocket.h>
#include <Win32ServerSocket.h>

DWORD WINAPI NetworkCoreThread(void *data)
{
    //      Generic Thread variables
    Win32NetworkCore	*network = (Win32NetworkCore *)data;
    Win32SocketEvents	*socket_events = network->getSocketEvents();
    WSANETWORKEVENTS	network_event;
    NetworkPacket		*send_packet = NULL;

    ResetEvent(network->m_TerminateThread);

    while (true) {
		if (network->m_destroy_threads == true) {
			SetEvent(network->m_TerminateThread);
			ExitThread(0);
		}

		if (network->m_sockets.size() == 0) {
			SuspendThread(network->m_Thread);
		}

		int event_id;
		if ((event_id = WSAWaitForMultipleEvents(
								socket_events->numEvents(),	//      The number of event to wait on
								socket_events->getEvents(),	//      the array of events you´re waiting on
								false,						//      dont wait until ALL have occured
								INFINITE,					//      timeout value for the wait state
								false))						//      whether I should alert anybody :P (IO Completion ports?)
								!= WSA_WAIT_TIMEOUT)
		{
											
			if (event_id != Win32SocketEvents::SEND_TRIGGER) {
				if(event_id == Win32SocketEvents::QUIT_TRIGGER) continue;
				
				ISocket *socket = socket_events->m_sockets[event_id];
				
				WSAEnumNetworkEvents(socket->m_socket, socket_events->m_events[event_id], &network_event);
				if (network_event.lNetworkEvents & FD_ACCEPT) {
					//      Process server sockets
					IServerSocket *server = (IServerSocket *)socket;

					IClientSocket *client = new Win32ClientSocket(network);
					int sockaddr_len = sizeof(client->m_socket_info);

					client->m_socket = accept(server->m_socket,(sockaddr *) & client->m_socket_info,&sockaddr_len);
					client->SetConnected(true);

					server->AddConnection(client);
					network->AddSocket(client, FD_READ);
					SetEvent(server->m_ConnectionEvent);
				} else if (network_event.lNetworkEvents & FD_READ) {
					//	Process client sockets
					IClientSocket *client = (IClientSocket *)socket;

					//	Receive data from network
					NetworkPacket *packet = new NetworkPacket;
					packet->socket = client->m_socket;
					memset(packet->data, 0, MAX_RECV);

					packet->length = recv(client->m_socket, packet->data, MAX_RECV, 0);

					if (packet->length > 0) {
						//	FIXME:	Do I need to lock sockets here? or around the recv() call?
						//			or perhaps neither?
						network->LockSockets();
						{
							client->AddDataPacket(packet);
						}
						network->UnlockSockets();
					}
				}
			}else{
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
						IClientSocket *client = (IClientSocket *)send_packet->socketobj;
						client->SendComplete();
					}
				}
				
				WSAResetEvent(network->m_network_events.m_events[0]);
				
			}	//     if(event_id!=0)'s else statement
		}		//     WSAWaitForMultipleEvents
    }			//     While loop
}
