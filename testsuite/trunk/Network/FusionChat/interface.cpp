#include <FusionChat.h>

//	Glade xml object
GladeXML		*xml;

//	GTK+ widgets that are used by the GTK+ functions in the app
GtkHBox			*connectBox;
GtkButton		*connectButton;
GtkLabel		*connectLabel;
GtkEntry		*connectEntry;

GtkButton		*appMode;
GtkLabel		*appModeLabel;

GtkCellRenderer	*renderer;
GtkTreeView		*output;
GtkListStore	*ostore;
GtkTreeModel	*omodel;
	
GtkTreeView		*users;
GtkListStore	*ustore;
GtkTreeModel	*umodel;

GtkButton		*userButton;
GtkWidget		*userBox;
GtkEntry		*userEntry;
GtkButton		*userUpdate;

GtkEntry		*msgEntry;
GtkWidget		*msgBox;
GtkButton		*msgSend;

std::string		defServerAddress;
std::string		defUsername;

const int force_dconnect = 3;
unsigned int	networkTimeout = 100;
typedef std::map<std::string, std::string> usermap_t;
usermap_t usermap;
usermap_t::iterator useriter;

void initGTK(int argc, char **argv)
{
	gtk_init(&argc, &argv);
	
	//	load the xml interface
	xml = glade_xml_new("interface.glade",NULL,NULL);
	glade_xml_signal_autoconnect(xml);
	
	//	grab the widgets you want to manipulate
	connectBox = GTK_HBOX(glade_xml_get_widget(xml,"connectBox"));
	connectButton = GTK_BUTTON(glade_xml_get_widget(xml,"connect"));
	connectLabel = GTK_LABEL(glade_xml_get_widget(xml,"connectLabel"));
	connectEntry = GTK_ENTRY(glade_xml_get_widget(xml,"connectEntry"));

	appMode = GTK_BUTTON(glade_xml_get_widget(xml,"appMode"));
	appModeLabel = GTK_LABEL(glade_xml_get_widget(xml,"appModeLabel"));
	
	output = GTK_TREE_VIEW(glade_xml_get_widget(xml,"output"));
	users = GTK_TREE_VIEW(glade_xml_get_widget(xml,"users"));
	
	userButton = GTK_BUTTON(glade_xml_get_widget(xml,"userButton"));
	userBox = glade_xml_get_widget(xml,"userBox");
	userEntry = GTK_ENTRY(glade_xml_get_widget(xml,"userEntry"));
	userUpdate = GTK_BUTTON(glade_xml_get_widget(xml,"userUpdate"));
	
	msgEntry = GTK_ENTRY(glade_xml_get_widget(xml,"msgEntry"));
	msgBox = glade_xml_get_widget(xml,"msgBox");
	msgSend = GTK_BUTTON(glade_xml_get_widget(xml,"msgSend"));
	
	//	Set all the default strings (to see if they are changed)
	updateUsername(state.username);
	defServerAddress = gtk_entry_get_text(connectEntry);
	
	setupApp();
}

void runGTK(GSourceFunc func)
{
	g_timeout_add(networkTimeout,(GSourceFunc)func,NULL);
	gtk_main();
}

void setupApp(void)
{
	setupOutput();
	setupUsers();
}

void setupOutput(void)
{
	//	Setup the treeviews for output
	GtkTreeViewColumn *column;
		
	//	setup the output treeview
	ostore = gtk_list_store_new(2, G_TYPE_STRING,G_TYPE_STRING );
	omodel = GTK_TREE_MODEL(ostore);
	gtk_tree_view_set_model(output,omodel);
	
	renderer = gtk_cell_renderer_text_new();

	//	setup output, username column
	column = gtk_tree_view_column_new_with_attributes("username",renderer,"text",0,NULL);
	gtk_tree_view_append_column(output,column);
	gtk_tree_view_column_set_min_width(column,100);
	//	setup output, message column
	column = gtk_tree_view_column_new_with_attributes("messages",renderer,"text",1,NULL);
	gtk_tree_view_append_column(output,column);
}

void setupUsers(void)
{
	//	Setup the treeviews for user
	GtkTreeViewColumn *column;
		
	//	setup the output treeview
	ustore = gtk_list_store_new(1, G_TYPE_STRING);
	umodel = GTK_TREE_MODEL(ustore);
	gtk_tree_view_set_model(users,umodel);
	
	renderer = gtk_cell_renderer_text_new();

	//	setup output, username column
	column = gtk_tree_view_column_new_with_attributes("username",renderer,"text",0,NULL);
	gtk_tree_view_append_column(users,column);
}

void insertMessage(std::string user, std::string message)
{
	//	FIXME:	There is a crash here, if you close the 
	//			app and insert a message, it can crash with adj = NULL
	
	//	Insert the message into the treeview, along with the user which sent it
	GtkTreeIter iter;
	gtk_list_store_append(ostore,&iter);
	
	gtk_list_store_set(ostore,&iter,0,user.c_str(),1,message.c_str(),-1);
	
	GtkAdjustment *adj = gtk_tree_view_get_vadjustment(output);
	gtk_adjustment_set_value(adj,adj->upper);
}

bool addUser(std::string user,std::string id)
{
	GtkTreeIter iter;
	
	if(findUser(user,&iter) == false){
		usermap[user] = id;
				
		//	If the user being added is the username of the client, then setup the id
		if(user == state.username) state.id = id;
		
		//	update the treeview with a new name
		gtk_list_store_append(ustore,&iter);
		
		gtk_list_store_set(ustore,&iter,0,user.c_str(),-1);
		
		return true;
	}
	
	return false;
}

bool removeUser(std::string user)
{
	GtkTreeIter iter;
	if(findUser(user,&iter) == true){
		usermap.erase(usermap.find(user));
		gtk_list_store_remove(ustore,&iter);
		return true;
	}
	return false;
}

bool renameUser(std::string oldUser, std::string newUser, std::string id)
{
	GtkTreeIter iter;
	
	//	If you can't find the newUser and can find the oldUser, rename :)
	if(findUser(newUser,&iter) == false && findUser(oldUser,&iter) == true){
		std::string test = usermap[oldUser];
		if(id == test){
			usermap.erase(usermap.find(oldUser));
			usermap[newUser] = id;
			gtk_list_store_set(ustore,&iter,0,newUser.c_str(),-1);
			
			//	If this local client, is the owner of the requested username, change it
			if(state.username == oldUser) updateUsername(newUser);
			
			return true;
		}
	}
	return false;

}

bool findUser(std::string user)
{
	GtkTreeIter iter;
	return findUser(user,&iter);
}

bool findUser(std::string user, GtkTreeIter *iter)
{
	char *str;
	if(gtk_tree_model_get_iter_first(umodel,iter) == true){
		do{
			str = NULL;
			gtk_tree_model_get(umodel,iter,0,&str,-1);
			if(user == str) return true;
		}while(gtk_tree_model_iter_next(umodel,iter) == true);
	}
	
	return false;
}

void emptyUsers(void)
{
	//	DO NOT EMPTY THE LIST IF YOU ARE RUNNING A SERVER
	//	reason: new clients, wont see the people existing on the server before they connected
	if(state.enableServer == false){
		//	Remove everyone from your list
		gtk_list_store_clear(ustore);
		usermap.clear();
	}
}

std::string getUserlist(void)
{
	std::string userlist;
	
	GtkTreeIter iter;
	char *str;
	
	if(gtk_tree_model_get_iter_first(umodel,&iter) == true){
		do{
			str = NULL;
			gtk_tree_model_get(umodel,&iter,0,&str,-1);
			std::string user = str;
			userlist+=user+","+usermap[user]+";";
		}while(gtk_tree_model_iter_next(umodel,&iter) == true);
	}
	
	return userlist;
}

void updateUsername(void)
{
	on_username_clicked(GTK_WIDGET(userButton),NULL);
}

void updateUsername(std::string name)
{
	state.username = name;
	gtk_button_set_label(userButton,name.c_str());
	gtk_entry_set_text(userEntry,name.c_str());
}

void setConnected(void)
{
	//	Inform the client it has connected successfully
	commandMessage("/info,accepted");
	
	state.connected = true;
	
	gtk_label_set_text(connectLabel,"Disconnect");
	gtk_widget_set_sensitive(GTK_WIDGET(connectEntry),false);
	gtk_widget_grab_focus(GTK_WIDGET(msgEntry));
}

void setDisconnected(void)
{
	commandMessage("/info,disconnected");
	
	//	Disconnect socket, if it's connected (delete object too)
	if(state.enableClient == true){
		state.client.socket->disconnect();
		delete state.client.socket;
		state.client.socket = NULL;
	}
	
	//	Set the connected/enableClient state back to false (so you can connect again)
	state.connected = false;
	state.enableClient = false;
	//	Reset the request disconnect counter (stop it accumulating)
	state.dconnect = 0;
	//	You should now clear out all the users that WERE there in your user list
	emptyUsers();	
		
	gtk_label_set_text(connectLabel,"Connect");
	gtk_widget_set_sensitive(GTK_WIDGET(connectEntry),true);
	gtk_widget_grab_focus(GTK_WIDGET(connectEntry));
}

void requestConnect(void)
{
	std::string server = gtk_entry_get_text(connectEntry);
	
	if(server != defServerAddress){
		commandMessage("/connect,"+server);
	}else{
		commandMessage("/error,invalidAddress");
	}
}

void requestDisconnect(void)
{
	if(state.connected == true){
		//	Request the server disconnect me, increase counter
		remoteMessage("/disconnect");
		commandMessage("/info,disconnect");
		//	Force the disconnect locally, if the server doesent listen/respond/quit is requested
		if(state.dconnect++ == force_dconnect) commandMessage("/disconnect");
	}else{
		commandMessage("/disconnect");
	}
}

EXPORT void on_connect_clicked(GtkWidget *widget, gpointer data)
{
	//	find the server address from the serverAddress widget
	//	initiate a connection to that server
	//	augment the command code method of connecting
	
	gtk_widget_grab_focus(GTK_WIDGET(connectEntry));
	
	if(state.connected == false)requestConnect();
	else						requestDisconnect();
}

EXPORT void on_connectAddress_activate(GtkWidget *widget, gpointer data)
{
	on_connect_clicked(GTK_WIDGET(connectButton),data);
}

EXPORT void on_appMode_clicked(GtkWidget *widget, gpointer data)
{
//	the appMode button was clicked, invert the current status of client/server
	std::string text = gtk_label_get_text(appModeLabel);
	
	if(text == "Client Mode"){
		gtk_label_set_text(appModeLabel,"Server Mode");
		//	Start server code
		startServer();
		//	Connect a local client to the server 
		//	(so the server can talk to remote clients)
		commandMessage("/connect,localhost");
	}else{
		gtk_label_set_text(appModeLabel,"Client Mode");
		//	Stop server code
		stopServer();
	}	
}

EXPORT void on_message_activate(GtkWidget *widget, gpointer data)
{
//	send the message to the treeview + server
	std::string text = gtk_entry_get_text(GTK_ENTRY(widget));
	if(text.empty() == false){
		commandMessage("/message,"+state.username+":"+text);
		gtk_entry_set_text(GTK_ENTRY(widget),"");
	}
}

EXPORT void on_send_clicked(GtkWidget *widget, gpointer data)
{
	//	Just another way to send a message, augment the message_activate code
	on_message_activate(GTK_WIDGET(msgEntry), data);
}

EXPORT void on_username_clicked(GtkWidget *widget, gpointer data)
{
	//	hide the message_box
	//	show the username_box (contains the widgets to update the username)
	
	gtk_widget_hide(msgBox);
	gtk_widget_show(userBox);
	gtk_widget_set_sensitive(GTK_WIDGET(widget),false);
	
	gtk_widget_grab_focus(GTK_WIDGET(userEntry));
}

EXPORT void on_username_entry_activate(GtkWidget *widget, gpointer data)
{
	//	set the username to this
	//	hide the username_box
	//	show the message_box
	std::string user = gtk_entry_get_text(GTK_ENTRY(widget));
	
	size_t scolon = user.find(";");
	size_t comma = user.find(",");
	
	if(scolon != std::string::npos && comma != std::string::npos){
		commandMessage("/error,invalidUsername");
	}else{
		//	Update username if you didnt find
		//	1) no change was made
		//	2) the string was empty
		if(state.username != user && user.empty() == false){
			gtk_widget_hide(userBox);
			gtk_widget_show(msgBox);
			gtk_widget_set_sensitive(GTK_WIDGET(userButton),true);
			
			commandMessage("/setUsername,"+user);
		}
	}
}

EXPORT void on_username_update_clicked(GtkWidget *widget, gpointer data)
{
	//	Just another way to update the user, augment the username_entry_activate code
	on_username_entry_activate(GTK_WIDGET(userEntry), data);
}

EXPORT void on_quit_clicked(GtkWidget *widget, gpointer data)
{
	//	We are quitting, so you dont want to try more than once
	state.quit = true;
	requestDisconnect();
}