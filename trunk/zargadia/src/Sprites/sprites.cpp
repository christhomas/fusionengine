#include <Zargadia.h>

void SetupGameScene(void)
{
	int a;

	player.Initialise();
	playerweapon.Initialise();

	alien.Initialise();
	alienweapon.Initialise();

	defence.Initialise();
	explosion.Initialise();

	//	Setup the collision system
	GameScene->CollisionDetection(SceneGraph::COLLISION_AABB2D);

	//	Create all the collision groups necessary
						GameScene->CollisionSystem->AddEntity(player.GroupID,player.sprite);
	for(a=0;a<72;a++)	GameScene->CollisionSystem->AddEntity(alien.GroupID,alien.sprite[a]);
						GameScene->CollisionSystem->AddEntity(playerweapon.GroupID,playerweapon.sprite);
	for(a=0;a<3;a++)	GameScene->CollisionSystem->AddEntity(alienweapon.GroupID,alienweapon.sprite[a]);
	for(a=0;a<3;a++)	GameScene->CollisionSystem->AddEntity(defence.GroupID,defence.sprite[Defence::NORMAL][a]);

	//	Create all the collision group associations
	unsigned int collisiongroup[3];

	//	Setup the collisions ALIEN -> PLAYERWEAPON
	collisiongroup[0] = 2;

	GameScene->CollisionSystem->AddGroupCollide(1,collisiongroup,1);

	//	Setup the collisions	PLAYER -> ALIEN,
	//							PLAYER -> ALIEN_WEAPON
	collisiongroup[0] = 1;
	collisiongroup[1] = 3;

	GameScene->CollisionSystem->AddGroupCollide(0,collisiongroup,2);

	//	Setup the collisions	DEFENCE -> ALIEN,
	//							DEFENCE -> PLAYER_WEAPON,
	//							DEFENCE -> ALIEN_WEAPON
	collisiongroup[0] = 1;
	collisiongroup[1] = 2;
	collisiongroup[2] = 3;

	GameScene->CollisionSystem->AddGroupCollide(4,collisiongroup,3);

	GameScene->CollisionSystem->ResizeCollisionArea(-10,-10);
}

void ProcessCollisions(void)
{
	for(collisionpair_t::iterator cp=GameScene->CollisionSystem->m_collisions.begin();cp!=GameScene->CollisionSystem->m_collisions.end();cp++)
	{
		if((*cp)->GroupID[0] == player.GroupID){
			if((*cp)->GroupID[1] == alien.GroupID){
				//	Player collided with an alien
				alien.Damage((*cp)->entity[1]);
				player.Damage();

			}else if((*cp)->GroupID[1] == alienweapon.GroupID){
				//	Player collided with an alien weapon
				player.Damage();
				(*cp)->entity[1]->SetActive(false);
			}
		}else if((*cp)->GroupID[0] == alien.GroupID){
			if((*cp)->GroupID[1] == playerweapon.GroupID){
				//	Alien collided with player weapon
				alien.Damage((*cp)->entity[0]);
				(*cp)->entity[1]->SetActive(false);

				gamedisplay.PlayerScore+=250;
				gamedisplay.Update();
			}		
		}else if((*cp)->GroupID[0] == defence.GroupID){
			if((*cp)->GroupID[1] == playerweapon.GroupID){
				//	Defence collided with player weapon
				(*cp)->entity[1]->SetActive(false);

			}else if((*cp)->GroupID[1] == alienweapon.GroupID){
				//	Defence collided with alien weapon
				defence.Damage((*cp)->entity[0],(*cp)->entity[1]);
				(*cp)->entity[1]->SetActive(false);

			}else if((*cp)->GroupID[1] == alien.GroupID){
				//	Defence collided with alien
				defence.Damage((*cp)->entity[0],(*cp)->entity[1]);
				alien.Damage((*cp)->entity[1]);

				gamedisplay.PlayerScore-=100;
				gamedisplay.Update();
			}
		}
	}
}
