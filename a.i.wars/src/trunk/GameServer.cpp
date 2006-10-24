#include <vector>
#include "AIWars.h"

GameServer::GameServer(char *ai)
{
	m_aidir = NULL;

	SetAIDir(ai);
}

GameServer::~GameServer()
{
	delete[] m_aidir;
}

void GameServer::SetAIDir(char *ai)
{
	delete[] m_aidir;
	m_aidir = new char[strlen(ai)+1];
	strcpy(m_aidir,ai);
}

char * GameServer::GetAIDir(void)
{
	return m_aidir;
}

void GameServer::Initialise(void)
{
	//	create collision system
	GameScene->CollisionDetection(SceneGraph::COLLISION_SPHERE2D);
	GameScene->CollisionSystem->EnableVectorCorrection();
}

bool GameServer::RegisterBot(int key,ClientBot *client)
{
	ServerBot *sb = new ServerBot(key);

	sb->SetClient(client);
	sb->CreateEntity();
	sb->CreateWeapons();
	sb->Initialise();

	serverbot.push_back(sb);

	return true;
}

void GameServer::InitCD(void)
{
	//	Set tanks to collide with tanks
	GameScene->CollisionSystem->AddGroupCollide(0,0);
	GameScene->CollisionSystem->AddGroupCollide(0,1);
}

void GameServer::ProcessCD(void)
{
	for(COLLISIONPAIR::iterator cp=GameScene->CollisionSystem->m_collisions.begin();cp!=GameScene->CollisionSystem->m_collisions.end();cp++)
	{
		int			g1	=	(*cp)->GroupID[0];
		int			g2	=	(*cp)->GroupID[1];

		Entity	*e1	=	(*cp)->entity[0];
		Entity	*e2	=	(*cp)->entity[1];

		switch(g2){
			case 0:{
				ServerBot *sb1	= ServerBot::Find(e1);
				ServerBot *sb2	=	ServerBot::Find(e2);

				if(sb1!=NULL)	sb1->Collision();
				if(sb2!=NULL)	sb2->Collision();
			}break;

			case 1:{
				ServerBot *sb1	=	ServerBot::Find(e1,e2);

				if(sb1!=NULL){
					sb1->Damage();
					sb1->Collision();
					e2->SetActive(false);
				}
			}break;
		}
	}
}

bool GameServer::RotateBody(int key,int angle)
{
	ServerBot *sb = ServerBot::Find(key);
	sb->RotateBody(angle);
	return true;
}

bool GameServer::RotateLimb(int key, int angle)
{
	ServerBot *sb = ServerBot::Find(key);
	sb->RotateLimb(angle);
	return true;
}

bool GameServer::MoveForward(int key, unsigned int distance)
{
	ServerBot *sb = ServerBot::Find(key);
	sb->MoveForward(distance);

	return true;
}

bool GameServer::MoveBackward(int key, unsigned int distance)
{
	ServerBot *sb = ServerBot::Find(key);
	sb->MoveBackward(distance);

	return true;
}

bool GameServer::FireOffence(int key, int power)
{
	ServerBot *sb = ServerBot::Find(key);

	sb->FireOffence(power);

	return true;
}

void GameServer::Death(ServerBot *sb)
{
	sb->SetActive(false);
}

void GameServer::Update(void)
{
	ProcessCD();

	for(int a=0;a<serverbot.size();a++){
		serverbot[a]->Update();
	}
}