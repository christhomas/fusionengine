#include <network/NetworkCore.h>
#include <network/ClientSocket.h>
#include <network/ServerSocket.h>
#include <network/WSASocketEvents.h>

#include <iostream>
void * WSANetworkThread(void *data)
{
	//      Generic Thread variables
	NetworkCore *network = (NetworkCore *)data;
	WSASocketEvents *socketEvents = (WSASocketEvents *)network->getSocketEvents();
	socketmap_t *socketmap = (socketmap_t *)socketEvents->getSockets();
	WSANETWORKEVENTS networkEvent;
	WSAEVENT *events;
	unsigned int event_id, numSockets;
	
	network->initThread();

    while (true){
		//	This will either kill the thread, or return ok
		network->killThread();
		
		numSockets = socketEvents->numSockets();
		events = (WSAEVENT *)socketEvents->getEvents();
		//socketEvents->output();
				
		if ((event_id = WSAWaitForMultipleEvents(
								numSockets,	//      The number of event to wait on
								events,		//      the array of events you´re waiting on
								false,		//      dont wait until ALL have occured
								INFINITE,	//      timeout value for the wait state
								false))		//      whether I should alert anybody :P (IO Completion ports?)
								!= WSA_WAIT_TIMEOUT)
		{
			/*	NOTE:	Normally you would check the iterator, but I dont think you can
						get to this point in the code without a valid SMap structure
						I think it's safe to assume the iterator is ok
						
						I will no doubt be proven wrong
			*/
			socketmap_t::iterator i = socketmap->find((unsigned int)events[event_id]);
			SMap *smap = (*i).second;
			ISocket *socket = smap->socketobj;
			
			if (event_id != WSASocketEvents::SEND_TRIGGER){
				if(event_id == WSASocketEvents::BREAK_TRIGGER) continue;				
			
				WSAEnumNetworkEvents(smap->sock, events[event_id], &networkEvent);
				if(networkEvent.lNetworkEvents & FD_ACCEPT){
					//	Event only happens with listening sockets (server sockets)
					
					ServerSocket *server = (ServerSocket *)socket;

					ClientSocket *client = new ClientSocket(network);
				
					client->threadConnect(server->threadAccept(), server->getPort());
					
					server->addConnection(client);					
					server->emitSignal();
				}else if(networkEvent.lNetworkEvents & FD_READ){
					network->socketReceive((IClientSocket *)socket);
				}else if(networkEvent.lNetworkEvents & FD_CONNECT){
					std::cout << "FD_CONNECT" << std::endl;
					ClientSocket *client = (ClientSocket *)socket;
					client->threadConnect();
					
				}else if(networkEvent.lNetworkEvents & FD_CLOSE){
					std::cout << "FD_CLOSE" << std::endl;
					
					if(socket != NULL){
						if(smap->type == SMap::CLIENT_SOCKET){
							ClientSocket *client = (ClientSocket *)socket;
							client->threadDisconnect();
							
							//	This will be set to INVALID_SOCKET if the server socket
							//	owns it and wants it to be deleted
							if(smap->sock == INVALID_SOCKET) delete smap->socketobj;
						}else{
							ServerSocket *server = (ServerSocket *)socket;
							server->threadDisconnect();
						}
					}
				}
			}else{				
				/*	FIXME: A remote socket disconnect can leave it impossible to send final data
					
					the problem comes is the remote computer disconnects, send() returns -1
					with no error checking here, it continues to return -1 on each subsequent
					call with no fallout.  So continues to infinity
					
					SOLUTION:	a NetworkCore::setError(variable) could be used to detect errors
								and notify fusion of the error.  a fallout could be setup so if the
								code returns an error code more than 10 times, it's a failure (for laggy
								links which may require more than one attempt before safely saying it's
								dead)
								
					SOLUTION:	There is a linger/nolinger set of options for setsockopt that might come handy
								here, has something to do with data that is waiting to be read/written to the socket
								when the socket closes								
				
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
				
				/*	FIXME:	The idea about putting the send code/stack into the socket
				
					The problem is, that you call the network object to do it internally it locks 
					the sockets and does it's job the focus of this is, if a socket sends/recv's 
					at the same time it must wait until either the send/recv completes in order to 
					obtain the lock, also, multiple sockets effectively queue for the lock, meaning
					the network still processes network data in a serial fashion
								
					SOLUTION:	The reason for the lock, is because you cannot delete sockets from the arrays
								while this thread still considers them alive and well, so doing any network
								activity must prevent the socket from disconnecting before completion, of course
								the remote end of the socket can do what it likes since there is no control over it.
								
								A solution would be a lock for certain types of actions which preclude sockets from
								disconnecting, but do not preclude them from other types of actions, so they can send/recv
								on any socket as much as they like, but disconnections are prevented until completion and
								the lock is obtained by the code which wishes to disconnect.
								
								I am also fairly sure that remote disconnections mean send/recv bsd functions return 0, meaning
								remote disconnect, so utilising that return code and acting accordingly is a good idea.
								
					PROBLEM:	the disconnect code is now serialised, only one socket at a time can disconnect, yet there
								isnt much of a problem behind ALL the connections suddenly wanting to disconnect.
								
								so a socket might be send/recv'ing data, but that shouldnt stop any other socket from disconnecting
				*/
				
				socketEvents->resetSendEvent();
				network->socketSend((IClientSocket *)socket);
				
			}	//     if(event_id!=0)'s else statement
		}		//     WSAWaitForMultipleEvents
    }			//     While loop
}

