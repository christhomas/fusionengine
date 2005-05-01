#ifndef _INPUTINTERFACE_H_
	#define _INPUTINTERFACE_H_

#include <Zargadia.h>

class InputConfig{
public:
	int keys[6];

			InputConfig		();
			~InputConfig	();
	void	Read			(void);
	void	Write			(void);
	void	Default			(void);
};

class InputInterface{
private:
	int state[2];
public:
	InputEvent		*inputqueue;
	char			PlayerName[3];
	Entity			*mousecursor;
	IInputDevice	*keyboard;
	IInputDevice	*mouse;
	InputConfig		cfg;

	int				move;				//	player current movement state
	bool			fire;				//	player current firing state
	int				name_position;		//	current enter name position
	int				animation_timer;	//	player animation timer
	int				current_frame;		//	animation frame
	Maths::Vector	*pos;				//	position on screen
	int				type;				//	type of input game is accepting

	//=========================
	//	Input Modes
	//=========================
	enum{
		INPUT_NONE=0,
		INPUT_TITLE,
		INPUT_GAME,
		INPUT_PAUSE,
		INPUT_NAME
	};

	//=========================
	//	Input Control ID Codes
	//=========================
	enum{
		CONTROL_LEFT = 0,
		CONTROL_RIGHT,
		CONTROL_FIRE,
		CONTROL_QUIT,
		CONTROL_PAUSE,
		CONTROL_FULLSCREEN
	};

				InputInterface		();
				~InputInterface		();
	void		Initialise			(void);
	void		Update				(void);
	void		Reset				(void);
	void		SetState			(int newstate);
	int			GetState			(void);
	Entity *	InitCursor			(void);
};

extern	InputInterface	*Input;

#endif // #ifndef _INPUTINTERFACE_H_