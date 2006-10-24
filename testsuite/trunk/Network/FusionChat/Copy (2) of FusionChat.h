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

const unsigned int msgMaxLen = 8912;

struct ChatSocket{
	//	Socket used to communicate with client/server
	IClientSocket *socket;
	//	length of the current command being transferred
	unsigned int length;
	//	Block of memory used to send commands from
	char send[msgMaxLen];
	//	Block of memory used to recv commands into
	char recv[msgMaxLen];
};

extern FCState state;
extern unsigned int netLatency;
extern std::vector<ChatSocket> sockets;

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

//	Network functions
char * makePacket(std::string text);
void sendData(char *packet, IClientSocket *dst, bool wait=false);
void castData(char *packet, IClientSocket *ignore);
void receiveData(void);
void receiveData(IClientSocket *socket);
std::string idstring(void *socket);
void startServer(void);
void stopServer(void);
void disconnectClients(void);
bool processNetwork(void *);

//	Command Functions
//	Breaks a string of text into components
void decodeMessage(std::string text, std::string &command, std::string &option, std::string &data);
//	acts upon a command message sent either locally or remotely
void commandMessage(std::string text, IClientSocket *socket = NULL);
//	sends a remote computer a command message
void remoteMessage(std::string text, IClientSocket *socket = NULL, bool wait=false);
//	broadcasts a message to remote computers
void broadcastMessage(std::string text, IClientSocket *socket);

#endif // #ifndef _FUSIONCHAT_H_