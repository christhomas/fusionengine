#ifndef _ALIEN_H_
	#define _ALIEN_H_

#include <Zargadia.h>

class AlienWeapon{
public:
	Entity	*sprite[3];
	float	velocity[3];
	int		GroupID;

	void	Initialise		(void);
	void	Update			(void);
	bool	Damage			(Entity *e);
	void	Fire			(void);
	void	Deactivate		(void);
};

class Alien{
protected:
	Overlay	*overlays[5];		//	List of all the aliens overlay frames
	int		anim_timer;			//	Timer to count ms's between frame changes
	float	velocity;			//	Velocity for each level
	float	xinc,yinc;			//	xinc/yinc for each animation frame
	int		detectside;			//	Which side is current being detected against collision
	int		current_frame;		//	The current animation frame
	int		timer;				//	motion timer, used to calculate the xinc for each frame

public:
	Entity *sprite[72];
	int		GroupID;

			Alien			();
			~Alien			();
	void	Initialise		(void);
	void	ResetPositions	(void);
	void	ResetStates		(void);
	void	ChangeLevel		(void);
	void	Landed			(void);
	void	Update			(void);
	bool	Death			(Entity *e);
	bool	Damage			(Entity *e);
};

extern	Alien			alien;
extern	AlienWeapon		alienweapon;

#endif // #ifndef _ALIEN_H_