#ifndef _EXPLOSION_H_
	#define _EXPLOSION_H_

#include <Zargadia.h>

struct ExplosionData{
	Overlay	*overlay;		//	The explosions overlay ptr	
	bool	Active;			//	Whether the explosion is active or not
	int		anim_ms;		//	the number of ms into the animation cycle
	int		current_frame;	//	the current frame of animation
	Entity	*sprite;		//	the sprite that exploded (if you want to reactivate it later)
};

class Explosion{
protected:
	ExplosionData	ed[15];	//	Data stored for each explosion
public:
	Entity			*sprite[15];

			Explosion	();
			~Explosion	();
	void	Initialise	(void);
	void	Explode		(Maths::Vector *position, Entity *e);
	void	Update		(void);
	void	Reset		(void);
};

extern Explosion explosion;

#endif // #ifndef _EXPLOSION_H_