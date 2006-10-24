#ifndef _MISCINTERFACE_H_
	#define _MISCINTERFACE_H_

#include <Zargadia.h>

class MiscInterface{
protected:
	Textbox	**Level,
			**Complete,
			**GameOver,
			**Pause,
			**EnterName;

public:
			MiscInterface	();
			~MiscInterface	();

	void	Initialise		(void);

	bool	ShowLevel		(void);
	bool	ShowComplete	(void);
	bool	ShowGameOver	(void);

	void	ShowPause		(void);
	void	HidePause		(void);

	void	ShowEnterName	(void);
	void	HideEnterName	(void);
	void	UpdateEnterName	(char *name);
};

extern MiscInterface miscdisplay;

#endif // #ifndef _MISCINTERFACE_H_