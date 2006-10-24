#include "Bot.h"
#include <vector>
#include "aiwars.h"

ServerBot::ServerBot(int key)
{
	m_key				=	key;
	m_entity		=	NULL;
	m_energy		=	100;
	m_tb_energy	=	NULL;
	m_tb_name		=	NULL;
	m_active		=	true;
}

ServerBot::~ServerBot()
{

}

bool ServerBot::GetActive(void)
{
	return m_active;
}

void ServerBot::SetActive(bool active)
{
	m_active = active;
	m_entity->SetActive(active);
	m_tb_name->SetActive(active);
	m_tb_energy->SetActive(active);
}

int ServerBot::GetKey(void)
{
	return m_key;
}

void ServerBot::CreateEntity(void)
{
	char					*tex	=	m_client->GetBotTexture();
	Maths::Vector	s			=	m_client->GetBotSize();
	Maths::Vector a			=	m_client->GetBotAxis();

	ITexture	*t		= fusion->Graphics->CreateTexture(tex);
	Overlay		*o		=	fusion->Mesh->CreateOverlay(t);

	int width,height;

	t->GetDimensions(width,height);

	o->AddFrame(new Rect(0,0,width,height));

	m_entity	=	fusion->Mesh->CreateEntity(o);
	m_entity->SetScale(s);
	m_entity->SetAxis(a);
	m_entity->SetActive(true);

	GameScene->AddEntity(m_entity);
}

void ServerBot::CreateWeapons(void)
{
	char *tex = m_client->GetWeaponTexture();

	m_weaponsystem.Initialise(tex);

	GameScene->AddEntity(m_weaponsystem.GetEntity());
}

void ServerBot::Initialise(void)
{
//	Create a position
//	Create a rotation for the body
	m_bodyangle	=	0;

//	Create a forward vector
//	Create a backward vector
//	NOTE: you create them by "initialising" a zero angle rotation
	RotateBody(0);

//	Create a rotation for the turret
//	Create a bounding box

//	Setup collision system
	GameScene->CollisionSystem->AddEntity(0,m_entity);
}

void ServerBot::SetClient(ClientBot *cb)
{
	m_client = cb;
}

void ServerBot::SetPosition(Maths::Vector position)
{
	m_position = position;

	m_position.z = 0;

	m_entity->InitTranslate(m_position);
}

void ServerBot::MoveForward(int distance)
{
	float *elements = m_bodyvector.GetElements();

	Maths::Vector t(elements[1],-elements[5],0);

	m_position += (t*distance);

	m_distance = distance;

	//	Check position against world boundaries and clip them accordingly
	//	NOTE:	This is such a shit method of hacking the CD to work
	AdjustMovement();

	m_entity->SetTranslate(TRANSLATE_ABS,m_position);
}

void ServerBot::MoveBackward(int distance)
{
	float *elements = m_bodyvector.GetElements();

	Maths::Vector t(-elements[1],elements[5],0);

	m_position += (t*distance);

	m_distance = -distance;

	//	Check position against world boundaries and clip them accordingly
	//	NOTE:	This is such a shit method of hacking the CD to work
	AdjustMovement();

	m_entity->SetTranslate(TRANSLATE_ABS,m_position);
}

void ServerBot::RotateBody(int angle)
{
	//	Determine if you have the energy and clip it if you do not

	//	Manipulate the energy for the rotation

	m_bodyangle += angle;

	m_bodyangle%=360;

	m_bodyquat.LoadIdentity();
	m_bodyquat.Rotate(0,0,m_bodyangle);

	m_bodyvector = m_bodyquat;

	m_lastangle = angle;

	m_entity->SetRotate(m_bodyangle,0,0,1);
}

void ServerBot::RotateLimb(int angle)
{
//	m_turretangle += angle;

//	m_turretquat.LoadIdentity();
//	m_turretquat.Rotate(0,0,m_turretangle);

//	m_turretview = m_turretquat;

//	m_entity->Rotate(angle,0,0,1);
}

bool ServerBot::FireOffence(int power)
{
	float	*e = m_bodyvector.GetElements();
	Maths::Vector	v(e[1],-e[5],0);

	return m_weaponsystem.Fire(WeaponSystem::OFFENCE,power,m_position,v);
}

void ServerBot::Collision(void)
{
	m_client->m_collision = true;
}

void ServerBot::Damage(void)
{
	m_energy-=1;

	if(m_energy < 0){
		SetActive(false);
		gameserver.Death(this);
	}
}

void ServerBot::AdjustMovement(void)
{
	bool collision = false;

	if(m_position.x < 0)
	{
		m_position.x = 0;
		collision = true;
	}

	if(m_position.x > SCREENWIDTH-32)
	{
		m_position.x = SCREENWIDTH-32;
		collision = true;
	}

	if(m_position.y < 0)
	{
		m_position.y = 0;
		collision = true;
	}

	if(m_position.y > SCREENHEIGHT-32)
	{
		m_position.y = SCREENHEIGHT-32;
		collision = true;
	}

	if(collision==true) m_client->m_collision = true;
}

void ServerBot::SetDisplay(Textbox *e, Textbox *n)
{
	m_tb_energy = e;
	m_tb_name		=	n;
}

bool ServerBot::Update(void)
{
	//m_energy += 1;

	//if(m_energy > 100) m_energy = 100;]

	if(GetActive() == false) return false;

	char str[256];

	//	Update onscreen output
	sprintf(str,"%s:",m_client->GetName());
	m_tb_name->UpdateString(str);
	m_tb_name->SetPosition(m_position.x,m_position.y-20,1);
	
	sprintf(str,"%d",m_energy);
	m_tb_energy->UpdateString(str);
	m_tb_energy->SetPosition(m_position.x,m_position.y-10,1);

	m_weaponsystem.Update();

	return m_client->Update();
}

bool ServerBot::OwnWeapon(Entity *e)
{
	if(e == m_weaponsystem.GetEntity()) return true;

	return false;
}

ServerBot * ServerBot::Find(int key)
{
	for(int a=0;a<serverbot.size();a++){
		if(key == serverbot[a]->GetKey()) return serverbot[a];
	}

	return NULL;
}

ServerBot * ServerBot::Find(Entity *e)
{
	for(int a=0;a<serverbot.size();a++){
		if(e == serverbot[a]->m_entity){
			return serverbot[a];
		}
	}

	return NULL;
}

ServerBot * ServerBot::Find(Entity *e1,Entity *e2)
{
	ServerBot *sb = NULL;

	for(int a=0;a<serverbot.size();a++){
		if(e1 == serverbot[a]->m_entity){
			sb = serverbot[a];

			if(sb->OwnWeapon(e2) == true){
				return NULL;
			}
		}
	}

	return sb;
}
