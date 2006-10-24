#include "Bot.h"
#include "GameServer.h"
#include <cstdlib>
#include <cstring>

ClientBot::ClientBot(char *name,GameServer *server,int key)
{
	//	Setup the ClientBot's name
	m_name = NULL;
	SetName(name);

	m_server		=	server;
	m_key				=	key;
	m_collision	=	false;
	m_aidir			=	m_server->GetAIDir();
}

ClientBot::~ClientBot()
{
	delete[] m_name;
}

void ClientBot::SetName(char *name)
{
	delete[] m_name;

	m_name = new char[strlen(name)+1];

	strcpy(m_name,name);
}

char * ClientBot::GetName(void)
{
	return m_name;
}

int ClientBot::GetKey(void)
{
	return m_key;
}

char * ClientBot::GetAIDir(void)
{
	return m_aidir;
}
