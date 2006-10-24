#ifndef _FUSIONCHAT_H_
#define _FUSIONCHAT_H_

#include <Fusion.h>

//	GTK+ headers
#include <gtk/gtk.h> 
#include <gdk/gdk.h>
#include <glib.h>
#include <glade/glade.h>

//	 STL Headers
#include <string>
#include <sstream>
#include <iostream>
#include <map>

//	EXPORT define, for win32 and unix platforms
#ifdef _WIN32
	#define EXPORT extern "C" __declspec(dllexport)
#else
	#define EXPORT extern "C"
#endif

const unsigned int msgMaxLen = 8912;

struct ChatPacket{
	//	The length of the packet, when fully downloaded
	unsigned int length;
	
	//	The length of data currently downloaded (always < length)
	unsigned int curlen;
	
	//	The buffer to be downloaded into
	char buffer[msgMaxLen];
};

struct ChatSocket{
	//	Socket used to communicate with client/server
	IClientSocket *socket;
	
	//	The send packet
	ChatPacket send;
	
	//	The recv packet
	ChatPacket recv;
	
	//	The username associated with this socket
	std::string userName;
};
typedef std::vector<ChatSocket> chatsocket_t;

struct FCState{
	IServerSocket	*server;
	ChatSocket		client;
	chatsocket_t	remote;
	bool			enableClient;
	bool			enableServer;
	bool			connected;
	unsigned int	port;
	std::string		username;
	std::string		id;
	unsigned int	dconnect;
	bool			quit;
};

extern FCState state;
extern unsigned int netLatency;

//	User Interface functions
void initGTK(int argc, char **argv);
void runGTK(GSourceFunc func);
void setupApp(void);
void setupOutput(void);
void setupUsers(void);
void insertMessage(std::string user, std::string message);
bool addUser(std::string user, std::string id);
bool removeUser(std::string user);
bool renameUser(std::string oldUser, std::string newUser, std::string data);
bool findUser(std::string user);
bool findUser(std::string user, GtkTreeIter *iter);
void emptyUsers(void);
std::string getUserlist(void);
void updateUsername(void);
void updateUsername(std::string name);
void setConnected(void);
void setDisconnected(void);
void requestConnect(void);
void requestDisconnect(void);

// Exported glade callbacks
EXPORT void on_connect_clicked(GtkWidget *widget, gpointer data);
EXPORT void on_appMode_clicked(GtkWidget *widget, gpointer data);
EXPORT void on_message_activate(GtkWidget *widget, gpointer data);
EXPORT void on_send_clicked(GtkWidget *widget, gpointer data);
EXPORT void on_username_clicked(GtkWidget *widget, gpointer data);
EXPORT void on_username_entry_focus(GtkWidget *widget, gpointer data);
EXPORT void on_username_entry_activate(GtkWidget *widget, gpointer data);
EXPORT void on_username_update_clicked(GtkWidget *widget, gpointer data);
EXPORT void on_quit_clicked(GtkWidget *widget, gpointer data);

//	Network functions
ChatPacket * makePacket(std::string text);
void sendData(ChatPacket *packet, ChatSocket *dst, bool wait=false);
void castData(ChatPacket *packet, ChatSocket *ignore);
void receiveData(void);
void receiveData(ChatSocket &chatSocket);
std::string idstring(void *socket);
void startServer(void);
void stopServer(void);
void addClient(IClientSocket *socket);
void disconnectClient(void);
void disconnectClient(ChatSocket *chatSocket);
bool processNetwork(void *);

//	Command Functions
//	Breaks a string of text into components
void decodeMessage(std::string text, std::string &command, std::string &option, std::string &data);
//	acts upon a command message sent either locally or remotely
void commandMessage(std::string text, ChatSocket *socket = NULL);
//	sends a remote computer a command message
void remoteMessage(std::string text, ChatSocket *socket = NULL, bool wait=false);
//	broadcasts a message to remote computers
void broadcastMessage(std::string text, ChatSocket *ignore = NULL);

#endif // #ifndef _FUSIONCHAT_H_