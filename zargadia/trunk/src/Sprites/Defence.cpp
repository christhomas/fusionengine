#include <Defence.h>

//	Globals
Defence	defence;

//	the times show how many milliseconds to display the damage for in each second

Defence::Defence()
{
	Milliseconds[0]		=	750;
	Milliseconds[1]		=	600;
	Milliseconds[2]		=	600;
	Milliseconds[3]		=	450;
	Milliseconds[4]		=	450;
	Milliseconds[5]		=	450;
	Milliseconds[6]		=	300;
	Milliseconds[7]		=	300;
	Milliseconds[8]		=	300;
	Milliseconds[9]		=	150;
	Milliseconds[10]	=	150;
	Milliseconds[11]	=	150;
	Milliseconds[12]	=	150;
	Milliseconds[13]	=	150;
	Milliseconds[14]	=	0;
	Milliseconds[15]	=	0;
	Milliseconds[16]	=	0;
	Milliseconds[17]	=	0;
	Milliseconds[18]	=	0;
	Milliseconds[19]	=	0;

	GroupID = 4;
}

Defence::~Defence(){}

void Defence::Initialise(void)
{
	ITexture *tb;
	int x,y,a;

//==============================
//	Setup Defence Unit sprites
//==============================
	tb = fusion->Graphics->CreateTexture("file://sprites/Defence.tga");

	for(a=0;a<3;a++){
		defence_overlays[a][0] = fusion->Mesh->CreateOverlay(tb);

		for(y=0;y<3;y++) for(x=0;x<10;x++)	defence_overlays[a][0]->AddFrame(new Rect(x*50,y*85,(x+1)*50,(y+1)*85));
		
		sprite[NORMAL][a] = fusion->Mesh->CreateEntity(defence_overlays[a][0]);
		sprite[NORMAL][a]->SetScale(50,85,1);
		sprite[NORMAL][a]->SetTranslate(TRANSLATE_ABS,(800/4)*(a+1)+(-75+(a*50)),450,0);	

		GameScene->AddEntity(sprite[NORMAL][a]);
	}

//====================================
//	Setup Defence unit damage sprites
//====================================
	tb = fusion->Graphics->CreateTexture("file://sprites/DefenceDamage.tga");

	for(a=0;a<3;a++){
		defence_overlays[a][1] = fusion->Mesh->CreateOverlay(tb);

		for(y=0;y<3;y++) for(x=0;x<10;x++)	defence_overlays[a][1]->AddFrame(new Rect(x*50,y*85,(x+1)*50,(y+1)*85));
		
		sprite[DAMAGE][a] = fusion->Mesh->CreateEntity(defence_overlays[a][1]);
		sprite[DAMAGE][a]->SetScale(50,85,1);
		sprite[DAMAGE][a]->SetTranslate(TRANSLATE_ABS,(800/4)*(a+1)+(-75+(a*50)),450,0);

		GameScene->AddEntity(sprite[DAMAGE][a]);
	}
}

void Defence::Update(void)
{
	int ms = GameTimer.GetMillisecondsDiff();

	for(int a=0;a<3;a++) if(sprite[NORMAL][a]->GetActive() == true || sprite[DAMAGE][a]->GetActive() == true)
	{
		du[a].anim_ms += ms;
		if(du[a].anim_ms > 50){
			du[a].anim_ms = 0;

			if(++current_frame == 30) current_frame = 0;

			//	Set the normal defence frame
			defence_overlays[a][0]->SetFrame(current_frame);
			//	Set the damaged defence frame
			defence_overlays[a][1]->SetFrame(current_frame);
		}
		
		if(du[a].start_milliseconds <= 0){
			
			if(du[a].damage_milliseconds < 0){
				du[a].start_milliseconds	= 1000;
				du[a].damage_milliseconds	= 0;
				sprite[DAMAGE][a]->SetActive(false);
				sprite[NORMAL][a]->SetActive(true);
			}
			
			if(du[a].damage_milliseconds == 0){
				du[a].damage_milliseconds = Milliseconds[du[a].lives-1];

			}else{
				du[a].damage_milliseconds -= ms;
				du[a].flash = !du[a].flash;
				sprite[DAMAGE][a]->SetActive(du[a].flash);
				sprite[NORMAL][a]->SetActive(!du[a].flash);
			}
		}else{
			du[a].start_milliseconds -= ms;
			if(du[a].start_milliseconds <= 0) Sound->GameSFX[SoundInterface::SOUND_DEFENCE_DAMAGE]->Play();
		}
	}
}


bool Defence::Damage(Entity *defence, Entity *e)
{
// if I want to damage this sprite
// add an explosion where the weapon hit it
// thinking...
	Maths::Vector *position = e->GetPosition();

	explosion.Explode(position,NULL);

	for(int a=0;a<3;a++){
		if(defence == sprite[NORMAL][a]){
			if((--du[a].lives) > 0){
				Sound->GameSFX[SoundInterface::SOUND_DEFENCE_HIT]->Play();
			}else{
				Death(a);
			}
		}
	}

	return true;
}

bool Defence::Death(int defence)
{
	sprite[NORMAL][defence]->SetActive(false);
	sprite[DAMAGE][defence]->SetActive(false);

	Sound->GameSFX[SoundInterface::SOUND_DEFENCE_EXPLODE]->Play();

	return true;	
}

/*
 *	void DefenceResetStates(void)
 *
 *	Reset the active states of the defence plus their respective data structures
 */
void Defence::Reset(void)
{
	for(int a=0;a<3;a++){
		sprite[NORMAL][a]->SetActive(true);
		sprite[DAMAGE][a]->SetActive(false);

		du[a].flash					= false;
		du[a].lives					= 20;
		du[a].damage_milliseconds	= 0;
		du[a].start_milliseconds	= 1000;
		du[a].anim_ms				= 0;
	}
}
