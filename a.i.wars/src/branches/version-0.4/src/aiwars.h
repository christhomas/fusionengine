#ifndef _AIWARS_H_
	#define _AIWARS_H_

#include <Fusion.h>
#include <Timer.h>
#include <vector>

//	Game Specific includes
#include <GameServer.h>
#include <Bot.h>

extern bool (*aiwars)(void);

extern	unsigned int				SCREENWIDTH;	//	Width of the screen
extern	unsigned int				SCREENHEIGHT;	//	Height of the screen
extern	std::vector<ServerBot *>	serverbot;		//	ServerBot array
extern	GameServer					gameserver;		//	The Gameserver object

//	from UserInterface.cpp
extern SceneGraph		*LoadScene;		//	Loading scenegraph
extern SceneGraph		*TitleScene;	//	Titlescreen scenegraph
extern SceneGraph		*GameScene;		//	In game scenegraph
extern UserInterface	*LoadUI;		//	Loading interface
extern UserInterface	*GameUI;		//	Game interface

//	from main.cpp
int		main			(int argc, char **argv);
bool	SystemInit		(void);
bool	SystemRun		(void);
bool	SystemQuit		(void);
void	ActivateWindow	(bool activate);
void	DestroyWindow	(void);
bool	ToggleGraphics	(void);

//	from aiwars.cpp
bool	Load			(void);
bool	Run				(void);
void	LoadBot			(char *bot);
void	BotAnimation	(Entity *e);

//	from Collision.cpp
void	ProcessCollisions(void);

//	UserInterface.cpp
void	UI_Loading		(void);
void	UI_Titlescreen	(void);
void	UI_Game			(void);

#endif // #ifndef _AIWARS_H_

