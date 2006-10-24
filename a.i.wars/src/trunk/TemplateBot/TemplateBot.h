#ifndef _TEMPLATEBOT_H_
	#define _TEMPLATEBOT_H_

#include <Bot.h>

class TemplateBot: public ClientBot{
protected:
	int		m_angle;
	int		m_forward;
	int		m_rotate;
	int		m_state;
	
	char	*m_bot;
	char	*m_weapon;
public:
												TemplateBot				(char *name,GameServer *server,int key);
	virtual								~TemplateBot			();
	virtual bool					Initialise				(void);
	virtual void					SetupTextures			(void);
	virtual bool					Update						(void);
	virtual char *				GetBotTexture			(void);
	virtual char *				GetWeaponTexture	(void);
	virtual Maths::Vector	GetBotSize				(void);
	virtual Maths::Vector	GetBotAxis				(void);
};

#endif // #ifndef _TEMPLATEBOT_H_
