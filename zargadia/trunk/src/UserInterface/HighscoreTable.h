#ifndef _HIGHSCORETABLE_H_
	#define _HIGHSCORETABLE_H_

#include <Zargadia.h>

class Position{
public:
	char name[3];
	int score;

	Textbox *tb_name;
	Textbox *tb_score;
};

class HighscoreTable{
public:
	Position p[10];

			HighscoreTable	();
			~HighscoreTable	();
	void	Initialise		(void);
	void	Read			(void);
	void	Write			(void);
	void	Default			(void);
	void	Update			(void);
	void	Update			(char *name, int score);
};

extern	HighscoreTable	*Highscore;

#endif // #ifndef _HIGHSCORETABLE_H_