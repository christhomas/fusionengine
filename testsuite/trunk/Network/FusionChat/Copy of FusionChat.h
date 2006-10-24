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

struct FCState{
	IClientSocket	*client;
	IServerSocket	*server;
	bool			enableClient;
	bool			enableServer;
	unsigned int	port;
	std::string		username;
	std::string		id;
};

struct ChatPacket{
	unsigned int plen;
	char user[256];
	char data[8912];
	unsigned int dlen;
};

struct ChatSocket{
	IClientSocket *socket;
	ChatPacket packet;
};

extern std::vector<ChatSocket> sockets;
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
void removeUser(std::string user);
bool renameUser(std::string oldUser, std::string newUser, std::string data);
bool findUser(std::string user);
bool findUser(std::string user, GtkTreeIter *iter);
void emptyUsers(void);
std::string getUserlist(void);
void updateUsername(void);
void updateUsername(std::string name);
void setConnected(void);
void setDisconnected(void);
std::string getConnected(void);
void closeApp(void);

// Exported glade callbacks
EXPORT void on_connect_clicked(GtkWidget *widget, gpointer data);
EXPORT void on_appMode_clicked(GtkWidget *widget, gpointer data);
EXPORT void on_message_activate(GtkWidget *widget, gpointer data);
EXPORT void on_send_clicked(GtkWidget *widget, gpointer data);
EXPORT void on_username_clicked(GtkWidget *widget, gpointer data);
EXPORT void on_username_entry_focus(GtkWidget *widget, gpointer data);
EXPORT void on_username_entry_activate(GtkWidget *widget, gpointer data);
EXPORT void on_username_update_clicked(GtkWidget *widget, gpointer data);
EXPORT void on_FusionChat_destroy(GtkWidget *widget, gpointer data);
EXPORT void on_quit_clicked(GtkWidget *widget, gpointer data);

//	Fusion Chat functions
ChatPacket * makePacket(std::string text);
ChatPacket * recvPacket(IClientSocket *socket);
void sendData(ChatPacket *packet, IClientSocket *src);
void receiveData(IClientSocket *socket);
std::string idstring(void *socket);
void startServer(void);
void stopServer(void);
void disconnectClients(void);
bool processNetwork(void *);

//	Fusion Chat command functions
void sendMessage(std::string text);
void broadcastMessage(std::string text, IClientSocket *socket);
void remoteMessage(std::string text, IClientSocket *socket=NULL, bool wait=false);
void commandMessage(std::string command);
void commandMessage(std::string command, IClientSocket *socket);
void clientMessage(std::string text);
void clientMessage(std::string command, std::string option, std::string data, IClientSocket *socket);
void serverMessage(std::string text);
void serverMessage(std::string command, std::string option, std::string data, IClientSocket *socket);

#endif // #ifndef _FUSIONCHAT_H_