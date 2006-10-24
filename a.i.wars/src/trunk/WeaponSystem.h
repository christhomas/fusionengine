#ifndef _WEAPONSYSTEM_H_
	#define _WEAPONSYSTEM_H_

#include <Maths/Vector.h>

class Entity;

//	Weapon handling subclass
class WeaponSystem{
private:
	Entity				*m_weapon;
	Maths::Vector	m_vector;
public:
	enum WeaponType{ OFFENCE=0,DEFENCE };

										WeaponSystem		();
	virtual						~WeaponSystem		();
	virtual	void			Initialise			(char *texture);
	virtual Entity *	GetEntity				(void);
	virtual bool			WeaponAvailable	(int type);
	virtual bool			Fire						(int type,int power, Maths::Vector &pos, Maths::Vector &v);
	virtual void			Update					(void);
};

#endif // #ifndef _WEAPONSYSTEM_H_