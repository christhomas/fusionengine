#ifndef _BOT_H_
	#define _BOT_H_

#include <Maths/Vector.h>
#include <Maths/Quaternion.h>
#include <Maths/Matrix.h>

#include "WeaponSystem.h"

class GameServer;
class Entity;
class Textbox;

//	A Bot derives itself from this class
class ClientBot{
private:
	int						m_key;
	char					*m_aidir;
protected:
	char					*m_name;
	GameServer		*m_server;
	Maths::Vector	m_size;
	Maths::Vector m_axis;
public:
	bool					m_collision;

												ClientBot					(char *name,GameServer *server,int key);
	virtual								~ClientBot				();
	virtual bool					Initialise				(void)	=	0;
	virtual	bool					Update						(void)	=	0;
	virtual	void					SetName						(char *name);
	
	virtual char *				GetName						(void);
	virtual int						GetKey						(void);
	virtual char *				GetAIDir					(void);
	virtual char *				GetBotTexture			(void)	=	0;
	virtual char *				GetWeaponTexture	(void)	=	0;
	virtual Maths::Vector	GetBotSize				(void)	=	0;
	virtual Maths::Vector	GetBotAxis				(void)	=	0;
};

//	The Gameserver uses this to reference each bot within the gameworld
class ServerBot{
private:
	int			m_key;
protected:
	ClientBot					*m_client;
	Entity						*m_entity;
	Entity						*m_weapon;
	Maths::Vector			m_position;

	Maths::Quaternion	m_bodyquat;
	Maths::Matrix			m_bodyvector;
	int								m_bodyangle;
	int								m_distance;
	int								m_lastangle;

	Textbox						*m_tb_energy;
	Textbox						*m_tb_name;

	int								m_energy;

	bool							m_active;

	WeaponSystem			m_weaponsystem;
public:
										ServerBot			(int key);
	virtual						~ServerBot		();
	
	virtual int				GetKey				(void);
	
	virtual void			Initialise		(void);

	virtual bool			GetActive			(void);
	virtual void			SetActive			(bool active);

	virtual void			CreateEntity	(void);
	virtual void			CreateWeapons	(void);

	virtual void			Collision			(void);
	virtual void			AdjustMovement(void);
	virtual void			SetDisplay		(Textbox *e,Textbox *n);
	
	virtual void			SetClient			(ClientBot *cb);
	
	virtual void			SetPosition		(Maths::Vector position);
	
	virtual void			MoveForward		(int distance);
	virtual void			MoveBackward	(int distance);

	virtual void			RotateBody		(int angle);
	virtual void			RotateLimb		(int angle);

	virtual bool			FireOffence		(int power);

	virtual void			Damage				(void);

	virtual bool			OwnWeapon			(Entity *e);
	
	virtual bool			Update				(void);

	static	ServerBot *	Find	(int key);
	static	ServerBot * Find	(Entity *e);
	static	ServerBot * Find	(Entity *e1,Entity *e2);
};

#endif // #ifndef _BOT_H_

