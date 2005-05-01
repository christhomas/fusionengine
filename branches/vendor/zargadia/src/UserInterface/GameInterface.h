#ifndef _GAMEINTERFACE_H_
	#define _GAMEINTERFACE_H_

#include <Zargadia.h>

class GameInterface{
private:
	Textbox *score,*highscore,*level,*lives;
public:
	int Level;
	int NumberAliens;
	int PlayerLives;
	int PlayerScore;

	void	Initialise	(void);
	void	Update		(void);
	void	Reset		(void);
};

extern GameInterface gamedisplay;

#endif // #ifndef _GAMEINTERFACE_H_