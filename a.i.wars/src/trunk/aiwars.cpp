#include "aiwars.h"
#include <vector>

#define LOAD	0
#define TITLE 1
#define	RUN		2

Timer											t;
GameServer								gameserver("ai");
IModuleDB									*mdb;
std::vector<ServerBot *>	serverbot;

typedef ClientBot * (*createbot_t)(char *name,GameServer *server,int key);

bool Load(void)
{
	static int stage = LOAD;

	switch(stage){
		case LOAD:{
			UI_Loading();
			t.Start();
	
			UI_Titlescreen();

			mdb = fusion->GetModuleDB();
			mdb->AddPath("ai");
	
			stage = TITLE;
		}break;

		case TITLE:{
				t.Tick();
			if(t.GetMillisecondsAbs() > 500){
				t.Reset();
				LoadUI->SetActive(false);
				fusion->Scene->ActivateScene(TitleScene);
				aiwars = Run;
			}

		}break;
	};

	return true;
}

bool Run(void)
{
	static int stage = LOAD;

	switch(stage){
		case LOAD:{
			GameScene = fusion->Scene->AddScene();
			gameserver.Initialise();

			int a;
			
			for(a=0;a<5;a++)	LoadBot("TemplateBot");
			for(a=0;a<5;a++)	LoadBot("SimpleTank");
			for(a=0;a<5;a++)	LoadBot("SimpleTank2");
			for(a=0;a<5;a++)	LoadBot("WallBot");

			UI_Game();

			gameserver.InitCD();

			stage = TITLE;
		}break;

		case TITLE:{
			t.Tick();
			if(t.GetMillisecondsAbs() > 500){
				t.Reset();
				GameUI->SetActive(true);
				fusion->Scene->ActivateScene(GameScene);
				stage = RUN;
			}
		}break;

		case RUN:{
			gameserver.Update();
		}break;
	};

	return true;
}

void LoadBot(char *bot)
{
	static int key=0;
	static int a=1,b=1;

	char dllname[2048];

	//	to prevent bots appearing over the world borders

	Maths::Vector pos(a*100,b*100,1);

	if(a++==(SCREENWIDTH/100)){	a=1; b++; }	
	if(b==(SCREENHEIGHT/100)) b=1;

	//	Check for buffer overflows
	if(strlen(bot) > (2048-strlen("tank_d.dll"))) return;

#ifdef _DEBUG	
	sprintf(dllname,"%s/tank_d.dll",bot);
#else
	sprintf(dllname,"%s/tank.dll",bot);
#endif

	//	Load the tank.dll file
	createbot_t create = (createbot_t)mdb->GetFunction(dllname,"CreateBot");

	if(create != NULL){
		ClientBot *b = create(bot,&gameserver,key++);

		if(b != NULL){
			b->Initialise();

			ServerBot *sb = ServerBot::Find(b->GetKey());
			sb->SetPosition(pos);
		}
	}
}