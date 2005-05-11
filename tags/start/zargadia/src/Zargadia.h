#include <SoundInterface.h>
#include <InputInterface.h>
#include <GameInterface.h>
#include <MiscInterface.h>
#include <HighscoreTable.h>
#include <Defence.h>
#include <Player.h>
#include <Alien.h>
#include <Explosion.h>
#include <Fusion.h>

//	Externs
extern bool (*Zargadia)	(void);
extern bool (*System)	(void);	//	Game flow control

extern	Textbox			*Loading,*Section;
extern	UserInterface	*LoadUI,*TitleUI,*GameUI,*MiscUI;
extern	SceneGraph		*TitleScene,*GameScene;
extern	unsigned int	SCREENWIDTH,SCREENHEIGHT;
extern	Timer			GameTimer;
extern	IFont			*font;
extern	Entity			*fusionlogo;

//	from main.cpp
int		main						(int argc, char **argv);
bool	SystemInit					(void);
bool	SystemRun					(void);
bool	SystemQuit					(void);
void	ActivateWindow				(bool activate);
void	DestroyWindow				(void);
bool	ToggleGraphics				(void);

//	from Zargadia.cpp
bool	Load						(void);
bool	Titlescreen					(void);
bool	GameStart					(void);
bool	LevelStart					(void);
bool	Run							(void);
bool	LevelComplete				(void);
bool	GameCompleted				(void);
bool	GameOver					(void);
bool	EnterName					(void);

//	from Sprites.cpp
void	SetupGameScene				(void);
void	ProcessCollisions			(void);

//	from Loading.cpp
void	BuildLoadingUI				(void);

//	from Titlescreen.cpp
void	BuildTitleUI				(void);
void	ActivateTitleScreen			(void);
bool	StartButtonClick			(void);				//	The function which executes when start button is clicked
bool	ExitButtonClick				(void);				//	The function which executes when exit button is clicked
void	TS_ShipAnimation			(Entity *entity);	//	Animation function for the titlescreen ship
void	TS_ZargadiaTitleAnimation	(Entity *entity);	//	Animation function for the zargadia title
void	TS_ButtonAnimation			(Entity *entity);	//	Animation function for the titlescreen buttons
void	TS_FusionLogoAnimation		(Entity *entity);	//	Animation function for the fusion logo
void	ActivateTitleScreen			(void);				//	Function to activate all the entities on the titlescreen

//	from credits.cpp
void	BuildCreditUI				(void);
bool	ShowCredits					(void);
bool	HideCredits					(void);