#ifndef _WALLBOT_H_
	#define _WALLBOT_H_

#include <Bot.h>

class WallBot: public ClientBot{
protected:
	int		m_angle;
	int		m_forward;
	int		m_rotate;
	int		m_state;
	
	char	*m_bot;
	char	*m_weapon;
public:
												WallBot						(char *name,GameServer *server,int key);
	virtual								~WallBot					();
	virtual bool					Initialise				(void);
	virtual void					SetupTextures			(void);
	virtual bool					Update						(void);
	virtual char *				GetBotTexture			(void);
	virtual char *				GetWeaponTexture	(void);
	virtual Maths::Vector	GetBotSize				(void);
	virtual Maths::Vector	GetBotAxis				(void);
};

#endif // #ifndef _WALLBOT_H_
