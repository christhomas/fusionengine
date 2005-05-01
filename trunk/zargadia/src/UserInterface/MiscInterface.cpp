#include <MiscInterface.h>

//	Globals
UserInterface	*MiscUI;		//	Misc UserInterface object
MiscInterface	miscdisplay;

MiscInterface::MiscInterface()
{

}

MiscInterface::~MiscInterface()
{
	delete Level;
	delete Complete;
	delete GameOver;
	delete Pause;
	delete EnterName;
}

void MiscInterface::Initialise(void)
{
	//	Create the User Interface and add one window for the textboxes
	MiscUI = fusion->Interface->AddUI(NULL);
	Window *w = MiscUI->AddWindow(fusion->Input);

	//	Allocate all the textboxes
	Level				=	new Textbox *[2];
	Complete		=	new Textbox *[2];
	GameOver		=	new Textbox *[2];
	Pause				=	new Textbox *[2];
	EnterName		=	new Textbox *[2];

	CapabilityData cap;
	cap.FlashingMilliseconds = 500;
	
	//	Setup Level textboxes
	Level[0] = (Textbox *)w->AddComponent(new TextBoxSetup(320,300,1,font,"Level X"));
	Level[0]->SetActive(false);
	Level[0]->SetScale(0.75,0.75);
	Level[0]->SetColour(255,0,0,255);
	
	Level[1] = (Textbox *)w->AddComponent(new TextBoxSetup(300,350,1,font,"Get Ready!!"));
	Level[1]->SetActive(false);
	Level[1]->SetScale(0.75,0.75);
	Level[1]->SetCaps(WNDCOMP_FLASHING,&cap);

	//	Setup Game Complete textboxes
	Complete[0] = (Textbox *)w->AddComponent(new TextBoxSetup(320,300,1,font,"You Win!"));
	Complete[0]->SetActive(false);
	Complete[0]->SetScale(0.75,0.75);
	Complete[0]->SetColour(255,0,0,255);

	Complete[1] = (Textbox *)w->AddComponent(new TextBoxSetup(50,350,1,font,"You killed them all!! DO IT AGAIN! DO IT AGAIN!"));
 	Complete[1]->SetActive(false);
	Complete[1]->SetScale(0.75,0.75);
	Complete[1]->SetCaps(WNDCOMP_FLASHING,&cap);

	//	Setup GameOver textboxes
	GameOver[0] = (Textbox *)w->AddComponent(new TextBoxSetup(320,300,1,font,"You Lose"));
	GameOver[0]->SetActive(false);
	GameOver[0]->SetScale(0.75,0.75);
	GameOver[0]->SetColour(255,0,0,255);

	GameOver[1] = (Textbox *)w->AddComponent(new TextBoxSetup(70,350,1,font,"HAHAHHAAHHA Look how rubbish you are!"));
	GameOver[1]->SetActive(false);
	GameOver[1]->SetScale(0.75,0.75);
	GameOver[1]->SetCaps(WNDCOMP_FLASHING,&cap);

	//	Setup Pause textboxes
	Pause[0] = (Textbox *)w->AddComponent(new TextBoxSetup(320,300,1,font,"Game Paused"));
	Pause[0]->SetActive(false);
	Pause[0]->SetScale(0.75,0.75);
	Pause[0]->SetColour(255,0,0,255);

	Pause[1] = (Textbox *)w->AddComponent(new TextBoxSetup(260,350,1,font,"Press p again to play"));
	Pause[1]->SetActive(false);
	Pause[1]->SetScale(0.75,0.75);
	Pause[1]->SetCaps(WNDCOMP_FLASHING,&cap);

	//	Setup Entername textboxes
	EnterName[0] = (Textbox *)w->AddComponent(new TextBoxSetup(300,250,1,font,"Enter your name"));
	EnterName[0]->SetActive(false);
	EnterName[0]->SetScale(0.75,0.75);
	EnterName[0]->SetColour(255,0,0,255);

	EnterName[1] = (Textbox *)w->AddComponent(new TextBoxSetup(380,300,1,font,"AAA"));
	EnterName[1]->SetActive(false);
	EnterName[1]->SetScale(0.75,0.75);
}

bool MiscInterface::ShowLevel(void)
{
	static int time = 0;
	char buffer[150];

	if(time == 0){
		sprintf(buffer,"Level %i",gamedisplay.Level);
		Level[0]->UpdateString(buffer);

		Level[0]->SetActive(true);
		Level[1]->SetActive(true);
	}

	time += GameTimer.GetMillisecondsDiff();

	if(time < 3000)	return true;

	Level[0]->SetActive(false);
	Level[1]->SetActive(false);
	time = 0;
	
	return false;
}

bool MiscInterface::ShowComplete(void)
{
	static int time = 0;

	if(time == 0){
		Complete[0]->SetActive(true);
		Complete[1]->SetActive(true);
	}

	time += GameTimer.GetMillisecondsDiff();

	if(time < 3000)	return true;

	Complete[0]->SetActive(false);
	Complete[1]->SetActive(false);
	time = 0;

	return false;
}

bool MiscInterface::ShowGameOver(void)
{
	static int time = 0;

	if(time == 0){
		GameOver[0]->SetActive(true);
		GameOver[1]->SetActive(true);
	}

	time += GameTimer.GetMillisecondsDiff();

	if(time < 3000)	return true;

	GameOver[0]->SetActive(false);
	GameOver[1]->SetActive(false);
	time = 0;

	return false;
}

void MiscInterface::ShowPause(void)
{
	Pause[0]->SetActive(true);
	Pause[1]->SetActive(true);
}

void MiscInterface::HidePause(void)
{
	Pause[0]->SetActive(false);
	Pause[1]->SetActive(false);
}

void MiscInterface::ShowEnterName(void)
{
	EnterName[0]->SetActive(true);
	EnterName[1]->SetActive(true);
}

void MiscInterface::HideEnterName(void)
{
	EnterName[0]->SetActive(false);
	EnterName[1]->SetActive(false);
}

void MiscInterface::UpdateEnterName(char *name)
{
	EnterName[1]->UpdateString(name);
}
