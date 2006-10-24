#include "TemplateBot.h"
#include <GameServer.h>

enum{
	START=0,
	RUN,
	COLLISION
};

TemplateBot::TemplateBot(char *name, GameServer *server, int key): ClientBot(name,server,key)
{
	SetupTextures();
}

TemplateBot::~TemplateBot()
{
	delete[]	m_bot;
}

bool TemplateBot::Initialise(void)
{
	m_size.Set(32,32,1);
	m_axis.Set(16,16,1);

	m_angle		=	0;
	m_forward	=	3;
	m_rotate	=	0;
	m_state		=	START;
		
	//	Register the bot's key,sprite,size and axis with the server
	m_server->RegisterBot(GetKey(),this);

	return true;
}

void TemplateBot::SetupTextures(void)
{
	//	in most cases, the strings are strlen(xyz)+1, but because there are "/" separators 
	//	in the path, you add a couple of empty spaces to accomodate any that might be needed

	//	Create a string for it's textures to load from
	m_bot = new char[strlen(GetAIDir())+strlen(m_name)+strlen("sprites/tank.tga")+3];
	sprintf(m_bot,"%s/%s/sprites/tank.tga",GetAIDir(),m_name);

	m_weapon = new char[strlen(GetAIDir()) + strlen(m_name) + strlen("sprites/weapon.tga")+3];
	sprintf(m_weapon,"%s/%s/sprites/weapon.tga",GetAIDir(),m_name);
}

char * TemplateBot::GetBotTexture(void)
{
	return m_bot;
}

char * TemplateBot::GetWeaponTexture(void)
{
	return m_weapon;
}

Maths::Vector TemplateBot::GetBotSize(void)
{
	return m_size;
}

Maths::Vector TemplateBot::GetBotAxis(void)
{
	return m_axis;
}

bool TemplateBot::Update()
{	
	if(m_collision==true){
		m_state = COLLISION;
		m_collision=false;
	}

	switch(m_state){
		case START:{
			//	insert some startup code here
			m_server->RotateBody(GetKey(),rand()%360);
			m_state = RUN;
		}break;
		
		case RUN:{
			if(m_forward>0)	m_server->MoveForward(GetKey(),m_forward);
		}break;

		case COLLISION:{
			m_angle+=5;

			m_server->RotateBody(GetKey(),5);

			if(m_angle>15){
				m_server->FireOffence(GetKey(),1);
				
				m_angle=0;
				m_state = RUN;
			}
		}break;
	};

	return true;
}
