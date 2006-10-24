#include <Explosion.h>

Explosion	explosion;

Explosion::Explosion()
{
	for(int a=0;a<15;a++)
	{
		ed[a].overlay		=	NULL;
		ed[a].Active		=	false;
		ed[a].anim_ms		=	0;
		ed[a].current_frame	=	0;
		ed[a].sprite		=	NULL;
	}
}

Explosion::~Explosion(){}

void Explosion::Initialise(void)
{
	ITexture *tb;
	int a,x,y;

//==============================
//	Explosions sprites
//==============================
	tb = fusion->Graphics->CreateTexture("file://sprites/explosions.tga");

	for(a=0;a<15;a++){
		ed[a].overlay = fusion->Mesh->CreateOverlay(tb);

		for(y=0;y<4;y++) for(x=0;x<3;x++) ed[a].overlay->AddFrame(new Rect(x*75,y*56,(x+1)*75,(y+1)*56));

		sprite[a] = fusion->Mesh->CreateEntity(ed[a].overlay);
		sprite[a]->SetScale(100,75,1);
		sprite[a]->SetActive(false);

		GameScene->AddEntity(sprite[a]);
	}
}

void Explosion::Reset(void)
{
	for(int a=0;a<15;a++){
		sprite[a]->SetActive(false);
		ed[a].Active = false;
	}
}


void Explosion::Explode(Maths::Vector *position, Entity *e)
{
	for(int a=0;a<15;a++){
		if(sprite[a]->GetActive() == false){
			
			sprite[a]->SetActive(true);
			sprite[a]->SetTranslate(TRANSLATE_ABS,position->x-50,position->y-25,position->z);
			
			ed[a].Active		=	true;
			ed[a].anim_ms		=	0;
			ed[a].current_frame	=	0;
			ed[a].sprite		=	e;
			return;
		}
	}
}

void Explosion::Update(void)
{
	for(int a=0;a<15;a++){
		if(sprite[a]->GetActive() == true){
			//	Animate the explosions
			ed[a].overlay->SetFrame(ed[a].current_frame);
			if(ed[a].current_frame == 5){
				ed[a].current_frame = 0;
				sprite[a]->SetActive(false);
				ed[a].Active = false;
				if(ed[a].sprite != NULL) ed[a].sprite->SetActive(true);
			}else{
				ed[a].anim_ms += GameTimer.GetMillisecondsDiff();
				if(ed[a].anim_ms > 20){
					ed[a].anim_ms = 0;
					ed[a].current_frame++;
				}
			}
		}
	}
}
