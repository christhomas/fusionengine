#include <Player.h>

Player			player;
PlayerWeapon	playerweapon;

void Player::Initialise(void)
{
	ITexture *tb;
	int x,y;
	Overlay *o;

//==========================
//	Setup Player Sprites
//==========================
	tb	=	fusion->Graphics->CreateTexture("file://sprites/PlayerShip.tga");
	o	=	fusion->Mesh->CreateOverlay(tb);

	for(y=0;y<3;y++) for(x=0;x<7;x++) o->AddFrame(new Rect(x*64,y*64,(x+1)*64,(y+1)*64));

	o->SetFrame(11);

	sprite = fusion->Mesh->CreateEntity(o);
	sprite->SetScale(50,50,1);	
	sprite->SetTranslate(TRANSLATE_ABS,(800-50)/2,550,0);

	GameScene->AddEntity(sprite);

	GroupID = 0;
}

bool Player::Damage(void)
{
	explosion.Explode(sprite->GetPosition(),sprite);

	sprite->SetActive(false);

	alien.ResetPositions();
	ResetPosition();
	Input->Reset();
	
	playerweapon.Deactivate();
	alienweapon.Deactivate();

	Sound->GameSFX[SoundInterface::SOUND_PLAYER_EXPLODE]->Play();

	if((gamedisplay.PlayerLives--) == 1)	Death();

	return true;
}

void Player::Death(void)
{
	Zargadia = GameOver;
}

void Player::ResetPosition(void)
{
	Maths::Vector *position = sprite->GetPosition();
	position->Set((800-25)/2,550,0);
}


//==============================
//	Player Weapon Functions
//==============================

void PlayerWeapon::Initialise(void)
{
	ITexture *tb;
	int x;
	Overlay *o;

//===============================
//	Setup Player Weapon sprites
//===============================
	tb = fusion->Graphics->CreateTexture("file://sprites/Playerweapon.tga");

	o = fusion->Mesh->CreateOverlay(tb);
		
	for(x=0;x<5;x++) o->AddFrame(new Rect(x*20,0,(x+1)*20,32));
		
	sprite = fusion->Mesh->CreateEntity(o);
	sprite->SetScale(10,15,1);	
	sprite->SetActive(false);
	GameScene->AddEntity(sprite);

	GroupID = 2;
}

void PlayerWeapon::Fire(Maths::Vector *v)
{
	if(sprite->GetActive() == false){
		sprite->SetActive(true);
		sprite->SetTranslate(TRANSLATE_ABS,v->x+20,v->y,v->z);
		Sound->GameSFX[SoundInterface::SOUND_PLAYER_FIRE]->Play();
	}
}

void PlayerWeapon::Update(void)
{
	Maths::Vector *v;

	if(sprite->GetActive() == true){
		v = sprite->GetPosition();
		
		if(v->y < 0) sprite->SetActive(false);
					
		v->y -= 1.2*GameTimer.GetMillisecondsDiff();
	}
}

void PlayerWeapon::Deactivate(void)
{
	sprite->SetActive(false);
}