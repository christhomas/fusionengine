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
}

//	Processes commands sent to this client 
//	(from either the local client or remote server)
void commandMessage(std::string text, IClientSocket *socket)
{
	std::string command,option,data;
	decodeMessage(text,command,option,data);
	
	//	CLIENT MESSAGE: Attempt to connect to a server
	if(command == "connect"){
		commandMessage("/info,connect:"+data);
		
		if(state.enableClient == false){
			state.client = fusion->Network->CreateSocket();
			if(state.client->Connect(data.c_str(),state.port) == true){
				state.enableClient = true;
				
				state.remote.push_back(state.client);
							
				//	You are connected to the server with a socket
				
				//	Set your nick, to what it is now 
				//	(results not in an update of username, but an 
				//	update of the clients username on the server)
				remoteMessage("/newClient,"+state.username);
			}else{
				commandMessage("/error,connect");
			}
		}else{
			commandMessage("/error,client");
		}		
		
		return;
	}
	
	//	SERVER MESSAGE: a new client has connected and the requested username
	if(command == "newClient"){
		std::string idcode = idstring(socket);
		commandMessage("/info,newClient message");
		
		if(findUser(data) == true){
			remoteMessage("/error,userFound",socket);
		}else{
			//	Inform the client connection is accepted on server
			remoteMessage("/accepted,"+idcode,socket);
			//	Inform all clients to show user as connected
			//broadcastMessage("/userConnect,"+data+":"+idcode, NULL);
			
			//	Now send a list of current users to the client you just accepted
			std::string token,userlist = getUserlist();

			for(size_t pos = userlist.find(";");pos != std::string::npos;pos = userlist.find(";")){
				token = userlist.substr(0,pos);
				userlist = userlist.substr(pos+1);
				
				size_t sep = token.find(",");
				std::string user = token.substr(0,sep);
				std::string id = token.substr(sep+1);
				
				//remoteMessage("/userConnect,"+user+":"+id,socket);
			}
		}
		return;
	}
	
	//	CLIENT MESSAGE:	when a client receives this command, 
	//					it knows it's fully connected
	if(command == "accepted"){
		commandMessage("/info,accepted message");
		
		if(getConnected() == "Connect"){
			//	Inform the client it has connected successfully
			commandMessage("/info,accepted");
			//	Set the identifying code for this client
			state.id = data;
			//	Set the gui to show connected to a server
			setConnected();
		}
		return;
	}
	
	//	CLIENT MESSAGE: when a client connects to the server
	//					all clients get this message to inform them
	if(command == "userConnect"){
		commandMessage("/info,userConnect message, "+option+", "+data);
		
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
	
	//	FIXME: there is no disconnect/cleanup code
	
	if(command == "setUsername")
	{
		if(option.empty() == true){
			//	No ID passed with this command
			
			//	WAITING CLIENT
			if(state.enableClient == true && getConnected() == "Connect"){
				remoteMessage("/newClient,"+data);
			}else{
				std::string name;
				std::string idcode = state.id;

				if(state.enableClient == true){
					if(getConnected() == "Disconnect")	name	= state.username;
					else								socket	= state.client;
				}else if(state.enableServer == true){
					if(socket == NULL)					name	= state.username;
					else								idcode	= idstring(socket);
				}
				
				commandMessage("/info,socket = "+idcode);
				remoteMessage("/setUsername,"+name+";"+data+":"+idcode,socket);
			}
		}else{
			//	ID was passed with this command
			
			size_t pos = option.find(";");
			if(pos > 0){
				std::string oldUser = option.substr(0,pos);
				std::string newUser = option.substr(pos+1);
				
				if(renameUser(oldUser,newUser,data) == true){
					//broadcastMessage("/setUsername,"+option+":"+data, socket);
				}else{
					commandMessage("/error,userFound");
				}
			}else{
				updateUsername(option.substr(1));
			}
		}
	}
	
	//	CLIENT MESSAGE: sends a message to the application window
	if(command == "message"){
		insertMessage(option,data);
		return;
	}
	
	//	Display some information text (uses "message" command)
	if(command == "info"){
		if(option	== "connect")		data = "Connecting to server on address: " + data;
		if(data		== "serverStart")	data = "Server starting...";
		if(data		== "serverStop")	data = "Server stopping...";
		if(data		== "listening")		data = "Server is listening for clients";
		if(data		== "newClient")		data = "A new client has connected to server";
		if(option	== "userConnect")	data = "user: "+data+", has entered the room";
		
		commandMessage("/message,info:"+data);
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
		
		commandMessage("/message,error:"+data);
		return;
	}
}

//	Sends messages between local/remote clients
//	Sends messages between remote clients and the server
void remoteMessage(std::string text, IClientSocket *socket, bool wait)
{
	ChatPacket *packet = makePacket(text);
	if(socket == NULL) socket = state.client;
	if(socket != NULL) sendData(packet,socket, wait);
	delete packet;
}

//	Sends messages to ALL remote clients
void broadcastMessage(std::string text, IClientSocket *socket)
{
	ChatPacket *packet = makePacket(text);
	castData(packet,socket);
	delete packet;
}