#include <FusionChat.h>

void decodeMessage(std::string text, std::string &command, std::string &option, std::string &data)
{
	/* Explanation of FusionChats command system
	
	commands sent to a client to be read follow a simple pattern
	
	/command,option:data
	
	commands, option and data must be one word
	
	valid delimiters are: comma, colon
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
	
	//	Find data
	if(ep != std::string::npos)	sp = ep+1;
	data = text.substr(sp);	
}

//	The user interface commandMessage method
void commandMessage(std::string command)
{
	commandMessage(command,state.client);
}

//	The network commandMessage method
void commandMessage(std::string text, IClientSocket *socket)
{
	std::string command, option, data;
	decodeMessage(text,command,option,data);
	
	fusion->errlog << "command = " << command << ", option = " << option << ", data = " << data << std::endl;
	
	//	If all strings are empty, error occured, a blank
	//	string, or messed up string	cannot be processed
	if(command.empty() == true && option.empty() == true && data.empty() == true) return;
	
	if(socket == state.client)	clientMessage(command,option,data,socket);
	else						serverMessage(command,option,data,socket);
	
	/*
		Client/Server commands go here, where they are not resolved
		in the specific clientMessage/serverMessage methods
	*/	
	
	//	Client/Server command: both can change their userName they are chatting with
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
				
				clientMessage("/info,socket = "+idcode);
				remoteMessage("/setUsername,"+name+";"+data+":"+idcode,socket);
			}
		}else{
			//	ID was passed with this command
			
			size_t pos = option.find(";");
			if(pos > 0){
				std::string oldUser = option.substr(0,pos);
				std::string newUser = option.substr(pos+1);
				
				if(renameUser(oldUser,newUser,data) == true){
					broadcastMessage("/setUsername,"+option+":"+data, socket);
				}else{
					clientMessage("/error,userFound");
				}
			}else{
				updateUsername(option.substr(1));
			}
		}
	}
	
	if(command == "quit") closeApp();
}

//	Client issued commands, 
void clientMessage(std::string text)
{
	std::string command, option, data;
	decodeMessage(text,command,option,data);
	clientMessage(command,option,data,state.client);
}

void clientMessage(std::string command, std::string option, std::string data, IClientSocket *socket)
{
	if(command == "message") insertMessage(option,data);
	
	//	Client command: connect to a server
	if(command == "connect"){
		clientMessage("/info,connect:"+data);
		
		if(state.enableClient == false){
			state.client = fusion->Network->CreateSocket();
			if(state.client->Connect(data.c_str(),state.port) == true){
				state.enableClient = true;
							
				//	You are connected to the server with a socket
				
				//	Set your nick, to what it is now 
				//	(results not in an update of username, but an 
				//	update of the clients username on the server)
				remoteMessage("/newClient,"+state.username);
			}else{
				clientMessage("/error,connect");
			}
		}else{
			clientMessage("/error,connectionPresent");
		}
	}
	
	//	Client command: disconnect from a server
	if(command == "disconnect"){
		state.client->Disconnect();
		state.enableClient = false;
		setDisconnected();
	}	
	
	//	Client command: server accepted your connection
	if(command == "accepted"){
		if(getConnected() == "Connect"){
			//	Inform the client it has connected successfully
			clientMessage("/info,accepted");
			//	Set the identifying code for this client
			state.id = data;
			//	Set the gui to show connected to a server
			setConnected();
		}
	}
	
	//	Client command: informs the client a new user has entered the server
	if(command == "userConnect")
	{
		clientMessage("/info,userConnect:"+option+";"+data);
		//	If the user being added, is you, then set the state.id
		if(addUser(option,data) == false){
			//	Error adding the user to the client
			clientMessage("/error,invalidUsername",socket);
		}else{
			//	Should the client do something if addUser succeeded?
			clientMessage("/info,userConnect");
		}
	}
	
	//	Client command: informs the client a user has disconnected from the server
	if(command == "userDisconnect"){}
	
	//	Client command: show information to the interface
	if(command == "info")
	{
		if(option == "connect")		data = "Lets connect to: " + data;
		if(data == "accepted")		data = "connection accepted!";
		if(data == "userConnect")	data = option + " has entered the room";
		if(data == "userDisconnect")data = option + " has left the room";

		insertMessage("info",data);
	}	
	
	if(command == "error")
	{
		//	The client failed to connect to the server
		if(data == "connect") data = "Failed to connect, sorry";

		//	The connect address was invalid (the network couldnt resolve or understand the address given
		if(data == "invalidAddress") data = "Try to use a correct hostname/address please";

		//	The app is running as a server, or the client is already connected
		//	therefore you cannot make another connection until the server is deactivated
		//	or the client, disconnected.
		if(data == "connectionPresent") data = "Cannot connect, Server running, or client already connected";
		
		//	The username is invalid (you entered a semi colon?)
		if(data == "invalidUsername"){
			data = "the username requested is invalid, please attempt to choose another";
			updateUsername();
		}

		//	This is sent from the server in response to an attempt to use an username which already exists there
		//	it says userFound, so you can choose another username and attempt again
		if(data == "userFound"){
			data = "Server reported this username is not available";
			updateUsername();
		}
		insertMessage("error",data);
	}
}

void serverMessage(std::string text)
{
	std::string command, option, data;
	decodeMessage(text,command,option,data);
	serverMessage(command, option, data, NULL);
}

void serverMessage(std::string command, std::string option, std::string data, IClientSocket *socket)
{
	//	Server command: client attempts a connection
	if(command == "newClient"){
		std::string idcode = idstring(socket);
		
		if(findUser(data) == true){
			remoteMessage("/error,userFound",socket);
		}else{
			//	Inform the client connection is accepted on server
			remoteMessage("/accepted,"+idcode,socket);
			//	Inform all clients to show user as connected
			broadcastMessage("/userConnect,"+data+":"+idcode, socket);
			
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
	}
	
	if(command == "info")
	{
		if(data == "listening")		data = "Listening for clients";
		if(data == "serverStart")	data = "Server started";
		if(data == "serverStop")	data = "Server stopped, all clients disconnected";
		if(data == "newClient")		data = "incoming connection from client";
		
		
		insertMessage("info",data);
	}
}

void remoteMessage(std::string text, IClientSocket *socket, bool wait)
{
	ChatPacket *packet = makePacket(text);
	
	if(packet->dlen > 0)
	{	
		//	No remote socket specified, send message to the server
		if(socket == NULL) socket = state.client;
		
		//	Send message
		socket->Send((char *)packet, packet->plen, wait);
	}
	
	delete packet;
}

void broadcastMessage(std::string text, IClientSocket *socket)
{
	ChatPacket *packet = makePacket(text);
	
	sendData(packet, socket);
	
	delete packet;
}

void sendMessage(std::string text)
{
	ChatPacket *packet = makePacket(text);
	
	//	Send to your server connection
	if(packet->dlen > 0 && state.enableClient == true){
		state.client->Send((char*)packet, packet->plen);
	}
		
	delete packet;
}
