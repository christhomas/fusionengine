#include <FusionChat.h>

void decodeMessage(std::string text, std::string &command, std::string &option, std::string &data)
{
	/*	Explanation of FusionChats command system:
	
		commands sent to a client to be read follow a simple pattern:
		/command,option:data
	*/
	
	//	If you can't find / to begin the string, it's not a valid command
	//	something went wrong, error detection?
	if(text[0] != '/') return;
	
	//	Remove the / from the start
	text = text.substr(1);
	
	size_t sp,ep;
	
	//	Find command
	sp = 0;
	ep = text.find(",");
	if(sp != std::string::npos && ep != std::string::npos) command = text.substr(sp,ep-sp);
	
	//	Find option
	sp = ep+1;
	ep = text.find(":");
	if(sp != std::string::npos && ep != std::string::npos) option = text.substr(sp,ep-sp);
	
	//	Find data from whats left of the string
	if(ep != std::string::npos)	sp = ep+1;
	data = text.substr(sp);	
	
	//	If command is empty, but data is not, copy data -> command, reason for this
	//	is a single word command like "disconnect" with no options or associated data
	if(command.empty() == true && data.empty() == false){ command = data; data = ""; }
}

//	Processes commands sent to this client 
//	(from either the local client or remote server)
void commandMessage(std::string text, ChatSocket *socket)
{
	std::string command,option,data;
	decodeMessage(text,command,option,data);
	
	//	CLIENT MESSAGE: Attempt to connect to a server
	if(command == "connect"){
		commandMessage("/info,connect:"+data);
		
		if(state.enableClient == false){
			IClientSocket *client = fusion->Network->createSocket();
			if(client->connect(data.c_str(),state.port) == true){
				state.enableClient = true;
				
				state.client.socket = client;
				state.client.recv.length = -1;
				state.client.send.length = -1;
							
				//	You are connected to the server with a socket
				
				//	Set your nick, to what it is now 
				//	(results not in an update of username, but an 
				//	update of the clients username on the server)
				remoteMessage("/newClient,"+state.username);
			}else{
				commandMessage("/error,connect");
				delete client;
			}
		}else{
			commandMessage("/error,client");
		}		
		
		return;
	}
	
	//	CLIENT/SERVER MESSAGE: 
	//	The server gets told a client wants to disconnect, sends command to client
	//	The client gets told, to disconnect by the server, or by force (client force)
	if(command == "disconnect"){
		//	This message came from the local client (force disconnect)
		//	or the server (socket state.client)
		if(socket == NULL || socket == &state.client){
			//	Client side disconnect code
			
			//	Set application to disconnected and send an information message saying so
			setDisconnected();
			
			//	If this disconnect is combined with a request to quit, do so now
			if(state.quit == true && state.enableClient == false){
				stopServer();
				gtk_main_quit();
			}
		}else{
			//	Server side disconnect code
			disconnectClient(socket);
		}
		return;
	}
	
	//	SERVER MESSAGE: a new client has connected and the requested username
	if(command == "newClient"){
		std::string idcode = idstring(socket);
		
		if(findUser(data) == true){
			remoteMessage("/error,userFound",socket);
		}else{
			std::cout << "newClient, name = " << data << std::endl;
			//	Update the userName in this chatSocket (quick was of finding this out if you need to)
			socket->userName = data;
			//	Inform the client connection is accepted on server
			remoteMessage("/accepted,"+idcode,socket);
			//	Inform all clients to show user as connected
			broadcastMessage("/userConnect,"+data+":"+idcode);
			
			//	Now send a list of current users to the client you just accepted
			std::string token,userlist = getUserlist();

			for(size_t pos = userlist.find(";");pos != std::string::npos;pos = userlist.find(";")){
				token = userlist.substr(0,pos);
				userlist = userlist.substr(pos+1);
				
				size_t sep = token.find(",");
				std::string user = token.substr(0,sep);
				std::string id = token.substr(sep+1);
				
				remoteMessage("/userConnect,"+user+":"+id,socket);
			}
		}
		return;
	}
	
	//	CLIENT MESSAGE:	when a client receives this command, 
	//					it knows it's fully connected
	if(command == "accepted"){
		if(state.connected == false){
			//	Set the gui to show connected to a server
			setConnected();

			//	Set the identifying code for this client
			state.id = data;
		}
		return;
	}
	
	//	CLIENT MESSAGE: when a client connects to the server
	//					all clients get this message to inform them
	if(command == "userConnect"){
		std::cout << "userConnect" << std::endl;
		//	If the user being added, is you, then set the state.id
		if(addUser(option,data) == false){
			//	Error adding the user to the client
			commandMessage("/error,invalidUsername:"+option,socket);
		}else{
			//	Should the client do something if addUser succeeded?
			commandMessage("/info,userConnect:"+option);
		}		
		
		return;
	}
	
	//	CLIENT MESSAGE: when a user disconnects, the clients have to remove from the userlist
	if(command == "userDisconnect"){
		if(removeUser(data) == true) commandMessage("/info,userDisconnect:"+data);
	}
	
	if(command == "setUsername")
	{
		if(option.empty() == true){
			//	No ID passed with this command
			//	Only the client part of the app executes this
			
			//	WAITING CLIENT
			if(state.enableClient == true && state.connected == false){
				remoteMessage("/newClient,"+data);
				updateUsername(data);
			}else{
				std::string name;
				std::string idcode = state.id;

				if(state.enableClient == true){
					if(state.connected == true)	name	= state.username;
					else						socket	= &state.client;
				}else if(state.enableServer == true){
					if(socket == NULL)			name	= state.username;
					else						idcode	= idstring(socket);
				}
				
				remoteMessage("/setUsername,"+name+";"+data+":"+idcode,socket);
			}
		}else{
			//	ID was passed with this command
			//	1) the server is attempting to rename a client (local or remote)
			//	2) the client has been ordered to rename itself by the server

			size_t pos = option.find(";");
			if(pos > 0){
				std::string oldUser = option.substr(0,pos);
				std::string newUser = option.substr(pos+1);
				
				if(renameUser(oldUser,newUser,data) == true){
					//	If state.client is the socket that recv'd this message
					//	the client is renaming itself on orders from the server
					//	so dont go broadcasting anything
					if(socket != &state.client){
						broadcastMessage("/setUsername,"+option+":"+data);
					}
				}else{
					//	Dont send a message to the server saying you found the username
					//	IT GAVE YOU to rename yourself too, if you are given a username
					//	it's cause it's ok to rename, but since this is complaining...
					if(socket != &state.client){
						remoteMessage("/error,userFound",socket);
					}
				}
			}else{
				updateUsername(option.substr(1));
			}
		}
	}
	
	//	CLIENT MESSAGE: sends a message to the application window
	if(command == "message"){
		std::cout << "message" << std::endl;
		//	if local client, need to send to server
		//	if server, need to broadcast to all clients, except who sent it
		
		if(socket == NULL || socket == &state.client){
			insertMessage(option,data);
			
			if(socket == NULL) remoteMessage(text);
		}else{
			broadcastMessage(text,socket);
		}
		return;
	}
	
	//	Display some information text (uses "message" command)
	if(command == "info"){
		if(option	== "connect")		data = "Connecting to server on address: " + data;
		if(data		== "accepted")		data = "You successfully connected to the server";
		if(data		== "disconnect")	data = "Disconnection has been requested (3 requests = FORCE DISCONNECT)";
		if(data		== "disconnected")	data = "You are now offline, thanks for using FusionChat";
		if(data		== "serverStart")	data = "Server starting...";
		if(data		== "serverStop")	data = "Server stopping...";
		if(data		== "listening")		data = "Server is listening for clients";
		if(data		== "newClient")		data = "A new client has connected to server";
		if(option	== "userConnect")	data = "user: "+data+", has entered the room";
		if(option	== "userDisconnect")data = "user: "+data+", has left the room";
		
		insertMessage("info",data);
		return;
	}
	
	if(command == "error"){
		if(data		== "connect")			data = "Failed to connect, sorry";
		if(data		== "client")			data = "Client already running, please disconnect and try again";
		if(data		== "invalidAddress")	data = "The address you entered, was not valid";
		if(option	== "invalidUsername"){
			data = "The username: "+data+", is invalid, please choose another username";
			updateUsername();
		}
		if(data		== "userFound"){
			data = "Server reported this username is not available";		
			updateUsername();
		}
	
		insertMessage("error",data);	
		return;
	}
}

//	Sends messages between local/remote clients
//	Sends messages between remote clients and the server
void remoteMessage(std::string text, ChatSocket *socket, bool wait)
{
	ChatPacket *packet = makePacket(text);
	if(socket == NULL) socket = &state.client;
	if(socket != NULL) sendData(packet,socket, wait);
	delete packet;
}

//	Sends messages to ALL remote clients
void broadcastMessage(std::string text, ChatSocket *ignore)
{
	//	You can only broadcast, if you are the server
	if(state.enableServer == true){
		ChatPacket *packet = makePacket(text);
		castData(packet,ignore);
		delete packet;
	}else{
		commandMessage("/info,Attempt to broadcast a message was made, but from a client FusionChat");
	}
}