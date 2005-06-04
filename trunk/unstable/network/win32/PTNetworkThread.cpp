#include <WSANetworkCore.h>
#include <network/ClientSocket.h>
#include <PTServerSocket.h>

void * PTNetworkThread(void *data)
{
	//      Generic Thread variables
	WSANetworkCore		*network = (WSANetworkCore *)data;
	WSASocketEvents		*socket_events = network->getSocketEvents();
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
				
				/*	FIXME:	WSAWaitForMutipleEvents should be avoided, or at least, "handled" :)
				
					The problem I can see is that if multiple network events happen, only one of them is
					processed at a time, the rest, have to wait until the next call to WSAWaitFor...
					
					therefore, I propose a loop here, which loops through all the sockets looking for
					signalled states and processing them all in one go (although this might cause a drop
					in performance, if so, forget the idea, otherwise, guns blazing!)
					
					that way, all events are processed in one loop, next call to WSAWaitFor... is called
					and there are no events waiting, so the next event to occur, will genuinely be a new
					event, instead of an old event, still waiting to be processed
				*/
				
				/*	FIXME: an event for remote disconnection should be added here
					
					Currently, if a socket disconnects on you, you dont know it's done so
					you could continue to try to send data to that socket and you wouldnt
					know that it's actually not listening to you anymore.  An extra event here
					would mean that any socket that disconnects, would be signalled and handled
					appropriately.
				
				*/
				
				WSAEnumNetworkEvents(socket->m_socket, socket_events->m_events[event_id], &network_event);
				if(network_event.lNetworkEvents & FD_ACCEPT){
					//	Process server sockets
					ServerSocket *server = (ServerSocket *)socket;

					ClientSocket *client = new ClientSocket(network);
				
					client->threadConnect(server->m_socket);
					
					server->AddConnection(client);					
					server->SignalConnect();
				}else if(network_event.lNetworkEvents & FD_READ){
					/*	FIXME: exposing more network core internals here
						possible fix?
					
						network->socketReceive(client);
						
						void NetworkCore::socketReceive(IClientSocket *client)
						{
							LockSockets();
							{
								client->socketReceive();	
							}
							UnlockSockets();
						}
						
						move LockSockets/UnlockSockets to protected members of NetworkCore
						
						SIDE EFFECTS
						
						a loss in performance can be found here, since I have to lock the sockets
						in order to recv data into the socket, which, similarly means I also have to do 
						this on send data too, which means send/recv will contend for the socket lock
						meaning a lot of unnecessary waiting.
						
						all I care about here, is whether a the network core will remove a socket, that could be
						in the process of being used.  So perhaps I should remove the need to lock the sockets
						at all, but this might be problematic
					*/						
					network->LockSockets();
					{
						//	Process client sockets
						ClientSocket *client = (ClientSocket *)socket;
						client->threadReceive();
					}
					network->UnlockSockets();
				}
			}else{
				/*	FIXME: exposing client socket internals
					possible fix?
					
					PTClientSocket *client = (PTClientSocket *)socket;
					client->socketSend();
					
					void ClientSocket::socketSend(void)
					{
						//	insert below code
						//	except socket_events->ResetSendEvent() call :)
					}
					
					SIDE EFFECTS
					
					this code would actually allow me to put the datastack from the NetworkCore object
					into each socket, so they each have their own private datastack for sending data
					which also means, I dont have a global lock on each socket sending data, this thread
					could process socket A's send datastack, whilst the other thread, is pushing socket B's
					data to be sent across the network, improving performance
					
					also means less exposure of internal data structures				
				*/
				
				/*	FIXME: A remote socket disconnect can leave it impossible to send final data
					
					the problem comes is the remote computer disconnects, send() returns -1
					with no error checking here, it continues to return -1 on each subsequent
					call with no fallout.  So continues to infinity
					
					SOLUTION:	a NetworkCore::setError(variable) could be used to detect errors
								and notify fusion of the error.  a fallout could be setup so if the
								code returns an error code more than 10 times, it's a failure (for laggy
								links which may require more than one attempt before safely saying it's
								dead)
				
				*/
				
				/*	FIXME:	Attempting to send from a socket, which is simultaniously being deleted (or has become)
					
					This is a socket locking problem, you should lock the sockets, before send/recv any data
					which would stop the main thread, from deleting a socket, until it's completed the current
					cycle, or block the socket from attempting to send, whilst it's being deleted.
					
					SOLUTION:	a per socket mutex which is required before being able to delete fully
								it could be locked on send/recv of any data through the socket and
								locked upon delete too, so either delete will wait until it's complete
								then obtain, stopping any further send/recv to happen, or delete will wait
								for the current send/recv to finish, before locking and deleting the socket
								
					PROBLEM:	the only problem is that network core currently owns all the send
								data, this is fixable by moving this into the socket itself, as noted
								above, upon deletion
								
					PROBLEM:	if the socket is sending and the delete code waits, it'll acquire the lock
								when the send finishes.  But if another send is queued, it'll automatically
								fall through the WaitForMultiple...function call and attempt to send again
								but block on the mutex,waiting for the delete to complete.  Upon completion
								it'll unblock and execute the send, here is the problem, the socket was
								deleted, but the thread state is at a point where it can't possibly know this
								so I have to find a way to signal the thread, to NOT process the event, after
								the mutex was acquired.
				*/
								
				NetworkPacket *packet;

				for(packet = network->getNetworkPacket();packet!=NULL;packet=network->getNetworkPacket())
				{
					int bytes_sent = 0;
					int offset = 0;
					
					while(packet->length > 0){
						bytes_sent = send(packet->socket, &packet->data[offset],packet->length, 0);
						
						if(bytes_sent >= 0){
							offset += bytes_sent;
							packet->length -= bytes_sent;
						}else{
							//	FIXME: Temporary fix for the remote disconnect problem noted above
							break;
						}
					}
					
					if(packet->socketobj != NULL){
						ClientSocket *client = (ClientSocket *)packet->socketobj;
						client->SignalSend();
					}
					
					delete packet;
				}
				
				socket_events->ResetSendEvent();
				
			}	//     if(event_id!=0)'s else statement
		}		//     WSAWaitForMultipleEvents
    }			//     While loop
}

