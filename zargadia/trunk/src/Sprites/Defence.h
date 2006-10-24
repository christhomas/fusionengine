#ifndef _DEFENCE_H
	#define _DEFENCE_H

#include <Zargadia.h>

class DefenceData{
public:
	int		lives;
	bool	flash;
	int		start_milliseconds;
	int		damage_milliseconds;
	int		anim_ms;
};

class Defence{
protected:
	int			Milliseconds[20];
	Overlay		*defence_overlays[3][2];
	DefenceData	du[3];
	int			current_frame;
public:
	enum{	NORMAL=0,DAMAGE	};

	Entity	*sprite[2][3];
	int		GroupID;

			Defence		();
			~Defence	();
	void	Initialise	(void);
	void	Update		(void);
	bool	Damage		(Entity *defence, Entity *e);
	bool	Death		(int defence);
	void	Reset		(void);
};

extern	Defence	defence;

#endif  // #ifndef _DEFENCE_H