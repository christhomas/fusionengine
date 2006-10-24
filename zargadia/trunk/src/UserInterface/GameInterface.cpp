#include <GameInterface.h>

//	Globals
UserInterface	*GameUI;		//	Game UserInterface object
SceneGraph		*GameScene;		//	Game Scene object
GameInterface	gamedisplay;	//	Game Interface wrapper

void GameInterface::Initialise(void)
{
	GameScene	=	fusion->Scene->AddScene();
	GameUI		=	fusion->Interface->AddUI(GameScene);

	Window *w = GameUI->AddWindow(fusion->Input);

	Textbox *tb;
	tb = (Textbox *)w->AddComponent(new TextBoxSetup(10,20,1,font,"Score "));
	tb->SetScale(0.5,0.5);
	tb->SetColour(255,0,0,255);

	tb = (Textbox *)w->AddComponent(new TextBoxSetup(250,20,1,font,"Highscore "));
	tb->SetScale(0.5,0.5);
	tb->SetColour(255,0,0,255);

	tb = (Textbox *)w->AddComponent(new TextBoxSetup(480,20,1,font,"Lives "));
	tb->SetScale(0.5,0.5);
	tb->SetColour(255,0,0,255);

	tb = (Textbox *)w->AddComponent(new TextBoxSetup(650,20,1,font,"Level "));
	tb->SetScale(0.5,0.5);
	tb->SetColour(255,0,0,255);

	//	Create the ingame hud
	score		=	(Textbox *)w->AddComponent(new TextBoxSetup(80, 20,1,font,"TESTING"));
	highscore	=	(Textbox *)w->AddComponent(new TextBoxSetup(350,20,1,font,"TESTING"));
	lives		=	(Textbox *)w->AddComponent(new TextBoxSetup(550,20,1,font,"TESTING"));
	level		=	(Textbox *)w->AddComponent(new TextBoxSetup(720,20,1,font,"TESTING"));
	
	//	Scale the text for the hud
	score->SetScale(0.5,0.5);
	highscore->SetScale(0.5,0.5);
	lives->SetScale(0.5,0.5);
	level->SetScale(0.5,0.5);
}

void GameInterface::Update(void)
{
	static int time = 0;

	//	Make sure the score is valid
	if(PlayerScore < 0) PlayerScore = 0;

	score->UpdateString(PlayerScore);

	time += GameTimer.GetMillisecondsDiff();

	if(time > 3000){
		time = 0;
		int random = rand()%10;
		std::string buffer = Highscore->p[random].tb_name->GetString() +" "+ Highscore->p[random].tb_score->GetString();
		highscore->UpdateString(buffer);
	}

	lives->UpdateString(PlayerLives);
	level->UpdateString(Level);
}

void GameInterface::Reset(void)
{
	PlayerLives		=	3;
	Level			=	1;
	NumberAliens	=	72;
	PlayerScore		=	0;

	Update();
}