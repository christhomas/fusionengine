#ifndef _BUTTONCODES_H_
	#define _BUTTONCODES_H_

//======================================================================================
//	Platform Independant Button ID codes
//
//	To support another API:
//	
//	Create an InputMapping.h file,	You want two arrays, one to convert these keyboard
//	codes into api specific codes and another array to convert the api specific codes
//	into platform independant codes
//
//	Platform Independant	->	API Specific
//	API Specific					->	Platform Independant
//
//	Once you can do that, you're all set to start using the new api with this library
//======================================================================================

//=========================
//	Mouse Codes
//=========================
enum MouseButton{
	MOUSE_BUTTON0 = 0,	MOUSE_BUTTON1,	MOUSE_BUTTON2,	MOUSE_BUTTON3
};

//=========================
//	Keyboard Codes
//=========================
enum KeyboardCode{
//=======================
//	PRINTING CHARACTERS
//=======================
	KEY_0 = 0,	KEY_1,	KEY_2,	KEY_3,	KEY_4,	KEY_5,	KEY_6,	KEY_7,	KEY_8,	KEY_9,	//	Numbers
	
	KEY_A,		KEY_B,	KEY_C,	KEY_D,	KEY_E,	KEY_F,	KEY_G,	KEY_H,	KEY_I,	KEY_J,	//	Upper case Letters
	KEY_K,		KEY_L,	KEY_M,	KEY_N,	KEY_O,	KEY_P,	KEY_Q,	KEY_R,	KEY_S,	KEY_T,
	KEY_U,		KEY_V,	KEY_W,	KEY_X,	KEY_Y,	KEY_Z,

	KEY_a,		KEY_b,	KEY_c,	KEY_d,	KEY_e,	KEY_f,	KEY_g,	KEY_h,	KEY_i,	KEY_j,	//	Lower case letters
	KEY_k,		KEY_l,	KEY_m,	KEY_n,	KEY_o,	KEY_p,	KEY_q,	KEY_r,	KEY_s,	KEY_t,
	KEY_u,		KEY_v,	KEY_w,	KEY_x,	KEY_y,	KEY_z,

	KEY_SPACE,							KEY_AT,								KEY_HASH,							//	Special keys
	KEY_EQUALS,							KEY_MINUS,						KEY_SQUAREBRACKETRIGHT,
	KEY_SQUAREBRACKETLEFT,	KEY_LBRACE,						KEY_RBRACE,	
	KEY_TILDE,							KEY_LANGLEBRACKET,		KEY_RANGLEBRACKET,	
	KEY_BACKSLASH,					KEY_FORWARDSLASH,			KEY_BAR,
	KEY_COMMA,							KEY_FULLSTOP,					KEY_QUESTIONMARK,	
	KEY_APOSTROPHE,					KEY_INVERTEDCOMMAS,		KEY_POUNDSIGN,	
	KEY_DOLLARSIGN,					KEY_PERCENT,					KEY_HAT,	
	KEY_AMPERSAND,					KEY_MULTIPLY,					KEY_LBRACKET,			
	KEY_RBRACKET,

//===========================
//	NON PRINTING CHARACTERS
//===========================
	
	KEY_LEFT,	KEY_RIGHT,	KEY_UP,	KEY_DOWN,	KEY_ESCAPE,								//	Arrow keys

	KEY_F1,		KEY_F2,		KEY_F3,	KEY_F4,		KEY_F5,		KEY_F6,						//	Function keys (F1 -> F15)
	KEY_F7,		KEY_F8,		KEY_F9,	KEY_F10,	KEY_F11,	KEY_F12,
	KEY_F13,	KEY_F14,	KEY_F15,

	KEY_NUMPAD0,	KEY_NUMPAD1,	KEY_NUMPAD2,	KEY_NUMPAD3,	KEY_NUMPAD4,		//	Numpad keys
	KEY_NUMPAD5,	KEY_NUMPAD6,	KEY_NUMPAD7,	KEY_NUMPAD8,	KEY_NUMPAD9
};

//======================================================
//	Table of ASCII codes
//	So you can accept keyboard input and turn that into
//	a valid ASCII output code say for entering your name
//	into a highscore table or typing a message to send
//	to a friend you're playing online with
//======================================================
const char AsciiKeyMap[128] = {

'0','1','2','3','4','5','6','7','8','9',	//	Numbers

'A','B','C','D','E','F','G','H','I','J',	//	Upper case letters
'K','L','M','N','O','P','Q','R','S','T',
'U','V','W','X','Y','Z',

'a','b','c','d','e','f','g','h','i','j',	//	Lower case letters
'k','l','m','o','p','q','r','s','t','u',
'v','w','x','y','z',

' ',	'@',	'#',						//	Special keys
'=',	'-',	'[',
']',	'{',	'}',
'~',	'<',	'>',
'\\',	'/',	'|',
',',	'.',	'?',
'\'',	'\"',	'£',
'$',	'%',	'^',
'&',	'*',	'(',
')',

//	Blank out the other keys supported, with space

' ',' ',' ',' ',									//	Blank out the Arrow Keys

' ',' ',' ',' ',' ',' ',					//	Blank out the Function Keys (F1 -> F15)
' ',' ',' ',' ',' ',' ',
' ',' ',' ',

' ',' ',' ',' ',' ',							//	Blank out the Numpad keys
' ',' ',' ',' ',' '

};

//================================================
//	The keyboard codes ready to be implemented as 
//	they are deemed to be needed for something at 
//	the moment, they arent when they are, just 
//	implement them in the keyboard enum
//================================================
#define KEY_BACK					0
#define KEY_TAB						0
#define KEY_RETURN				0
#define KEY_LCONTROL			0
#define KEY_SEMICOLON			0
#define KEY_GRAVE					0
#define KEY_LSHIFT				0
#define KEY_RSHIFT				0
#define KEY_LMENU					0
#define KEY_CAPITAL				0
#define KEY_NUMLOCK				0
#define KEY_SCROLL				0
#define KEY_ADD						0
#define KEY_DECIMAL				0
#define KEY_OEM_102				0
#define KEY_KANA					0
#define KEY_ABNT_C1				0
#define KEY_CONVERT				0
#define KEY_NOCONVERT			0
#define KEY_YEN						0
#define KEY_ABNT_C2				0
#define KEY_NUMPADEQUALS	0
#define KEY_PREVTRACK			0
#define KEY_COLON					0
#define KEY_UNDERLINE			0
#define KEY_KANJI					0
#define KEY_AX						0
#define KEY_UNLABELED			0
#define KEY_NUMPADENTER		0
#define KEY_RCONTROL			0
#define KEY_CALCULATOR		0
#define KEY_MEDIASTOP			0
#define KEY_NUMPADCOMMA		0
#define KEY_DIVIDE				0
#define KEY_SYSRQ					0
#define KEY_RMENU					0
#define KEY_PRIOR					0
#define KEY_HOME					0
#define KEY_END						0
#define KEY_NEXT					0
#define KEY_INSERT				0

#define KEY_DELETE				0			
#define KEY_LWIN					0
#define KEY_RWIN					0
#define KEY_POWER					0
#define KEY_SLEEP					0
#define KEY_WAKE					0

// undefined multimedia keys
#define KEY_WEBBACK				0
#define KEY_WEBFORWARD		0
#define KEY_WEBSTOP				0
#define KEY_WEBREFRESH		0
#define KEY_WEBSEARCH			0
#define KEY_WEBFAVORITES	0
#define KEY_WEBHOME				0
#define KEY_MAIL					0
#define KEY_MUTE					0
#define KEY_VOLUMEDOWN		0
#define KEY_VOLUMEUP			0
#define KEY_PLAYPAUSE			0	// dont get this? play/pause
#define KEY_PAUSE					0	// then this, pause? separate key?
#define KEY_STOP					0
#define KEY_NEXTTRACK			0
#define KEY_MEDIASELECT		0
#define KEY_MYCOMPUTER		0
#define KEY_APPS					0

#endif // #ifndef _BUTTONCODES_H_
