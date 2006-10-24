#ifndef _PLAYER_H_
	#define _PLAYER_H_

#include <Zargadia.h>

class PlayerWeapon{
public:
	Entity *sprite;
	int		GroupID;

	void	Initialise	(void);
	void	Fire		(Maths::Vector *v);
	void	Update		(void);
	void	Deactivate	(void);
};

class Player{
public:
	Entity *sprite;
	int		GroupID;

	void	Initialise		(void);
	bool	Damage			(void);
	void	Death			(void);
	void	Reset			(void);
	void	ResetPosition	(void);
};

extern	Player			player;
extern	PlayerWeapon	playerweapon;

#endif // #ifndef _PLAYER_H_