#include <Zargadia.h>

const int MAX_LEVELS = 100;

#define SETUP		0
#define EXECUTE		1
#define COMPLETE	2
static int state = SETUP;

bool Load(void)
{
	static int loading = 0;

	switch(loading){
		case 0:{
			Sound		=	new SoundInterface();
			Highscore	=	new HighscoreTable();

			Sound->cfg.Read();
			BuildLoadingUI();
		}break;	
		case 1:{	
			Section->UpdateString("UserInterface: Credits");
			BuildTitleUI();				
		}break;	//	Build the Title screen UI
		
		case 2:{	
			Section->UpdateString("UserInterface: Misc");	
			BuildCreditUI();			
		}break;	//	Build the Credits UI
		
		case 3:{	
			Section->UpdateString("UserInterface: Game");	
			miscdisplay.Initialise();	
		}break;	//	Build the Misc Textboxes
		
		case 4:{	
			Section->UpdateString("Highscore Table");		
			gamedisplay.Initialise();	
		}break;	//	Build the main game UI
		
		case 5:{	
			Section->UpdateString("Game Sprites");			
			Highscore->Initialise();	
		}break;	//	Build the highscore table
		
		case 6:{	
			Section->UpdateString("Music: Title");			
			SetupGameScene();			
		}break;	//	Load the game sprites
		
		case 7:{	
			Section->UpdateString("Music: Credits");		
			Sound->LoadTitleMusic();	
		}break;	//	Load the title music
		
		case 8:{	
			Section->UpdateString("Music: Game");			
			Sound->LoadCreditsMusic();	
		}break;	//	Load the credits music
		
		case 9:{	
			Section->UpdateString("SFX: Title");			
			Sound->LoadGameMusic();		
		}break;	//	Load the game music
		
		case 10:{	
			Section->UpdateString("SFX: Game");				
			Sound->LoadTitleSFX();		
		}break;	//	Load the title sfx
		
		case 11:{	
			Section->UpdateString("");						
			Sound->LoadGameSFX();		
		}break;	//	Load the game sfx
		
		case 12:{	
			LoadUI->SetActive(false);
					
			//	Read the highscore table
			Highscore->Read();

			Zargadia = Titlescreen;

			ActivateTitleScreen();
		}break;
	};

	loading++;

	return true;
}

bool Titlescreen(void)
{
	switch(state){
		case SETUP:{
			Sound->TitleMusic->Play();

			//	Enable Title/Misc user interfaces
			TitleUI->SetActive(true);
			MiscUI->SetActive(true);
			//	Disable Loading and Game UserInterfaces
			LoadUI->SetActive(false);
			GameUI->SetActive(false);

			fusion->Scene->ActivateScene(TitleScene);

			Input->SetState(InputInterface::INPUT_TITLE);
			Input->mouse->SetActive(true);
			Input->keyboard->SetActive(true);

			state = EXECUTE;
		}break;
	};

	return true;
}

bool GameStart(void)
{
	//	Reset the hud
	gamedisplay.Reset();	

	//	Deactivate the title screen
	TitleUI->SetActive(false);

	//	Stop the title music
	Sound->TitleMusic->Stop();

	//	Start playing the game music
	Sound->GameMusic->Play();

	//	Set the input devices
	Input->SetState(InputInterface::INPUT_GAME);
	Input->mouse->SetActive(false);

	//	Set to LevelStart mode
	Zargadia = LevelStart;

	state = SETUP;

	return true;
}

bool LevelStart(void)
{
//	static int music = 0;

	switch(state){
		case SETUP:{

			alien.ChangeLevel();
			player.ResetPosition();
			defence.Reset();
			explosion.Reset();

			player.sprite->SetActive(true);

			Input->SetState(InputInterface::INPUT_GAME);
	
			GameUI->SetActive(false);
			fusion->Scene->ActivateScene((SceneGraph *)NULL);

			state = EXECUTE;
		}break;

		case EXECUTE:{
			if(miscdisplay.ShowLevel() == false) state = COMPLETE;
		}break;

		case COMPLETE:{

			GameUI->SetActive(true);
			fusion->Scene->ActivateScene(GameScene);

			//	Lets try playing the music non-stop rather than stopping and starting each level
			//Sound->GameMusic->Play();
			
			//	Next level, swap tracks, I'm so unhappy
			//	about not being able to supply 10 tracks
			//	of pure mod mania
//			music = !music;

			Zargadia = Run;
			state = SETUP;
		}break;
	};
	return true;
}

bool Run(void)
{
	if(Input->GetState() == InputInterface::INPUT_GAME){
		if(player.sprite->GetActive() == true){
			alien.Update();
			alienweapon.Fire();
			alienweapon.Update();
			playerweapon.Update();
			ProcessCollisions();
		}
		explosion.Update();
		defence.Update();
		gamedisplay.Update();
	}

	return true;
}

bool LevelComplete(void)
{
	//	Dont stop playing the game music until your dead
	//Sound->GameMusic->Stop();
	
	//	Decide whether game is complete or another level needs to be played
	if(++gamedisplay.Level == MAX_LEVELS)	Zargadia = GameCompleted;
	else									Zargadia = LevelStart;
	
	return true;
}

bool GameCompleted(void)
{
	Sound->GameMusic->Stop();

	fusion->Scene->ActivateScene((SceneGraph *)NULL);
	MiscUI->SetActive(true);
	GameUI->SetActive(false);

	//	Show Game complete msg
	if(miscdisplay.ShowComplete() == false) Zargadia = EnterName;

	return true;
}

bool GameOver(void)
{
	Sound->GameMusic->Stop();

	fusion->Scene->ActivateScene((SceneGraph *)NULL);
	MiscUI->SetActive(true);
	GameUI->SetActive(false);

	if(miscdisplay.ShowGameOver() == false)	Zargadia = EnterName;

	return true;
}

bool EnterName(void)
{
	bool entername = false;

	//	Decide whether to ask for a name to enter into scoreboard
	for(int a=0;a<10;a++)	if(gamedisplay.PlayerScore > Highscore->p[a].score) entername = true;

	if(entername == true){
		Input->SetState(InputInterface::INPUT_NAME);
		miscdisplay.ShowEnterName();
	}else{	
		//	if not, return to the titlescreen
		Zargadia = Titlescreen;
	}
	return true;
}
