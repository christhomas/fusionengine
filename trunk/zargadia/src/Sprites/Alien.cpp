#include <Alien.h>

Alien		alien;
AlienWeapon	alienweapon;

//====================================
//	Alien functions
//====================================
Alien::Alien()
{
	velocity	=	0.1f;
	detectside	=	1;
	GroupID		=	1;
}

Alien::~Alien(){}

void Alien::Initialise(void)
{
	ITexture *tb;
	int a,b;

//==========================
//	Setup Alien Sprites
//==========================
	tb = fusion->Graphics->CreateTexture("file://sprites/alien.tga");
	for(a=0;a<5;a++){
		overlays[a] = fusion->Mesh->CreateOverlay(tb);

		for(b=0;b<5;b++){
			overlays[a]->AddFrame(new Rect(b*30,a*40,(b+1)*30,(a+1)*40));
		}
	}

	for(a=0;a<6;a++) for(b=0;b<12;b++)
	{
		int random = rand()%5;
		int alien = ((a*12)+b);
		sprite[alien] = fusion->Mesh->CreateEntity(overlays[random]);
		sprite[alien]->SetScale(30,40,1);
		GameScene->AddEntity(sprite[alien]);
	}
}

void Alien::ResetPositions(void)
{
	int a,b,
		x_spacing = 35,
		y_spacing = 45;

	Maths::Vector *position;

	for(a=0;a<6;a++) for(b=0;b<12;b++)
	{
		position = sprite[(a*12)+b]->GetPosition();
		position->Set((b*x_spacing)+5,50+((a*y_spacing)+10),0);
	}
}

void Alien::ResetStates(void)
{
	gamedisplay.NumberAliens = 72;

	for(int a=0;a<72;a++)	sprite[a]->SetActive(true);
}

void Alien::ChangeLevel(void)
{
	static int levelchange = 0;

	if(++levelchange & 4) velocity += 0.05f;

	xinc		=	velocity;
	yinc		=	10;
	detectside	=	1;
	
	ResetStates();
	ResetPositions();

	alienweapon.Deactivate();
}

void Alien::Landed(void)
{
}

void Alien::Update(void)
{
	int	 a = 0;
	bool hitside = false;
	bool hitbottom = false;
	
	Maths::Vector *position = NULL;

	int ms = GameTimer.GetMillisecondsDiff();

	//======================
	//	Animate the aliens
	//======================
	anim_timer += ms;
	if(anim_timer > 50){
		anim_timer = 0;
		current_frame++;
		if(current_frame == 5) current_frame = 0;
	}
	//}
	for(a=0;a<5;a++) overlays[a]->SetFrame(current_frame);

	//======================
	//	Move the aliens
	//======================
	for(a=0;a<72;a++){
		if(sprite[a]->GetActive() == true){
			position	=	sprite[a]->GetPosition();
			position->x	+=	xinc*ms;
			
			if(detectside==1){
				if(position->x > SCREENWIDTH-35+abs(xinc*ms)){
					hitside		= true;
					detectside	= 0;
				}
			}else{
				if(position->x < abs(xinc*ms)){
					hitside		= true;
					detectside	= 1;
				}
			}
		}
	}
	
	//=======================
	//	If you hit the side
	//=======================
	if(hitside==true){
		hitside	=	false;
		xinc	=-	xinc;

		for(a=0;a<72;a++){
			if(sprite[a]->GetActive() == true){
				position	=	sprite[a]->GetPosition();
				position->y	+=	yinc;

				if(position->y < abs(yinc) || position->y > (SCREENHEIGHT-(40+abs(yinc)))) hitbottom = true;
			}
		}

		if(hitbottom==true) Landed();
	}
}

bool Alien::Death(Entity *e)
{
	explosion.Explode(e->GetPosition(),NULL);

	e->SetActive(false);

	if((--gamedisplay.NumberAliens) <= 0)	Zargadia = LevelComplete;
	
	Sound->GameSFX[SoundInterface::SOUND_ALIEN_EXPLODE]->Play();

	return true;
}

bool Alien::Damage(Entity *e)
{
	return Death(e);
}

//====================================
//	Alien Weapon functions
//====================================

void AlienWeapon::Initialise(void)
{
	ITexture *tb;
	Overlay *o;
	int a,x;

//===============================
//	Setup Alien Weapon Sprites
//===============================
	tb = fusion->Graphics->CreateTexture("file://sprites/AlienWeapon.tga");

	for(a=0;a<3;a++){
		o = fusion->Mesh->CreateOverlay(tb);

		for(x=0;x<5;x++) o->AddFrame(new Rect(x*13,0,(x+1)*13,28));

		sprite[a] = fusion->Mesh->CreateEntity(o);
		sprite[a]->SetScale(10,15,1);
		sprite[a]->SetActive(false);
		
		float temp = rand()%6;
		velocity[a] = 0.1+(temp/10);

		GameScene->AddEntity(sprite[a]);
	}

	GroupID = 3;
}

void AlienWeapon::Update(void)
{
	Maths::Vector *v;

	for(int a=0;a<3;a++) if(sprite[a]->GetActive() == true){
		
		v = sprite[a]->GetPosition();
		
		if(v->y > SCREENHEIGHT)	sprite[a]->SetActive(false);
	
		v->y += velocity[a]*GameTimer.GetMillisecondsDiff(); 
	}
}

void AlienWeapon::Fire(void)
{
	int a,b;
	int random = rand()%gamedisplay.NumberAliens;
	Maths::Vector *v;

	for(a=0;a<3;a++){
		if(sprite[a]->GetActive() == false){

			for(b=0;b<72;b++){
				if(alien.sprite[b]->GetActive() == true){
					if(--random <= 0){
						sprite[a]->SetActive(true);
						v = alien.sprite[b]->GetPosition();
						
						sprite[a]->SetTranslate(TRANSLATE_ABS,v->x,v->y,v->z);

						Sound->GameSFX[SoundInterface::SOUND_ALIEN_FIRE]->Play();
						return;
					}
				}
			}
		}
	}
}

void AlienWeapon::Deactivate(void)
{
	for(int a=0;a<3;a++)	sprite[a]->SetActive(false);
}