#ifndef _SIMPLETANK2_H_
	#define _SIMPLETANK2_H_

#include <Bot.h>

class SimpleTank2: public ClientBot{
protected:
	int		m_angle;
	int		m_forward;
	int		m_rotate;
	int		m_state;
	
	char	*m_bot;
	char	*m_weapon;
public:
												SimpleTank2				(char *name,GameServer *server,int key);
	virtual								~SimpleTank2			();
	virtual bool					Initialise				(void);
	virtual void					SetupTextures			(void);
	virtual bool					Update						(void);
	virtual char *				GetBotTexture			(void);
	virtual char *				GetWeaponTexture	(void);
	virtual Maths::Vector	GetBotSize				(void);
	virtual Maths::Vector	GetBotAxis				(void);
};

#endif // #ifndef _SIMPLETANK2_H_
