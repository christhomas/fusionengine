#include <InputInterface.h>

InputInterface	*Input;

//=====================
//	InputConfig class
//=====================
InputConfig::InputConfig()
{
	Read();
}

InputConfig::~InputConfig()
{
	Write();
}

void InputConfig::Read(void)
{
	std::ifstream controls("controls.dat");
	controls.read((char *)keys,6 * sizeof(int));
	controls.close();
}

void InputConfig::Write(void)
{
	std::ofstream writecontrols("controls.dat");
	writecontrols.write((char *)keys,6 * sizeof(int));
	writecontrols.close();
}

void InputConfig::Default(void)
{
	keys[InputInterface::CONTROL_LEFT]			=	KEY_LEFT;
	keys[InputInterface::CONTROL_RIGHT]			=	KEY_RIGHT;
	keys[InputInterface::CONTROL_FIRE]			=	KEY_SPACE;
	keys[InputInterface::CONTROL_QUIT]			=	KEY_ESCAPE;
	keys[InputInterface::CONTROL_PAUSE]			=	KEY_P;
	keys[InputInterface::CONTROL_FULLSCREEN]	=	KEY_F1;
}

//=====================
//	Input class
//=====================
InputInterface::InputInterface()
{
	animation_timer	=	0;
	move			=	0;
	name_position	=	0;
	current_frame	=	10;
	fire			=	false;
	keyboard		=	NULL;
	mouse			=	NULL;
	pos				=	NULL;
	mousecursor		=	NULL;

	state[0]		=	-1;
	state[1]		=	INPUT_TITLE;

	Reset();
}

InputInterface::~InputInterface()
{
	Reset();
}

void InputInterface::Initialise(void)
{
	//	Create mouse
	mouse = fusion->Input->AddDevice(IInputDevice::MOUSE);
	mouse->SetAxisArea(0,0,SCREENWIDTH-20,SCREENHEIGHT-20);
	mouse->SetCursorPosition(SCREENWIDTH/2,SCREENHEIGHT/2);
	mouse->SetActive(false);

	//	Create keyboard
	keyboard = fusion->Input->AddDevice(IInputDevice::KEYBOARD);
	keyboard->AddListener(&inputqueue);
	keyboard->SetActive(false);
}

void InputInterface::Update(void)
{
//====================================
//	Loop through the input and 
//	collect all the events
//====================================
	for(InputEvent *e=inputqueue;e!=NULL;e=e->prev){
		InputButtonEvent *ib = (InputButtonEvent *)e;

		if(state[1] == INPUT_TITLE){
			//	CONTROL_FULLSCREEN
			if(ib->m_ButtonID == cfg.keys[CONTROL_FULLSCREEN]){
				if(ib->m_EventAction == IInputDevice::BUTTON_DOWN){
					ToggleGraphics();
					return;
				}
			}else if(ib->m_ButtonID == KEY_R){
				if(ib->m_EventAction == IInputDevice::BUTTON_DOWN){
					Highscore->Default();
					Highscore->Update();
				}
			}else if(ib->m_ButtonID == cfg.keys[CONTROL_QUIT]){
				System = SystemQuit;
			}

		//=====================================================
		//	Collect all the Game Input Events
		//
		//	CONTROL_LEFT		= left
		//	CONTROL_RIGHT		= right
		//	CONTROL_FIRE		= fires
		//	CONTROL_QUIT		= quits
		//	CONTROL_PAUSE		= pauses
		//	CONTROL_FULLSCREEN	= toggle fullscreen
		//=====================================================
		}else if(state[1] == INPUT_GAME){
			if(player.sprite->GetActive() == true){
				
				//	CONTROL_LEFT
				if(ib->m_ButtonID == cfg.keys[CONTROL_LEFT]){
					if(ib->m_EventAction == IInputDevice::BUTTON_DOWN){
						//	Has the player changed direction, or stopped?
						if(move == 0 || move == 5){
							//	Yes, they have, assign the timer to the current
							//	you're going to animate (change frame) soon
							animation_timer = GameTimer.GetMillisecondsAbs();
						}
						move = -5;
					}
					
					//	The player has stopped pressing the button, so stop
					if(ib->m_EventAction == IInputDevice::BUTTON_UP)	move = 0;
				
				//	CONTROL_RIGHT
				}else if(ib->m_ButtonID == cfg.keys[CONTROL_RIGHT]){
					if(ib->m_EventAction == IInputDevice::BUTTON_DOWN){
						//	Has the player changed direction, or stopped?
						if(move == 0 || move == -5){
							//	Yes, they have, assign the timer to the current
							//	you're going to animate (change frame) soon							
							animation_timer = GameTimer.GetMillisecondsAbs();
						}
						move = 5;
					}
					//	The player has stopped pressing the button, so stop
					if(ib->m_EventAction == IInputDevice::BUTTON_UP)	move = 0;
				
				//	CONTROL_FIRE
				}else if(ib->m_ButtonID == cfg.keys[CONTROL_FIRE]){

					if(ib->m_EventAction == IInputDevice::BUTTON_DOWN)	fire = true;
					if(ib->m_EventAction == IInputDevice::BUTTON_UP)	fire = false;
				
				//	CONTROL_QUIT
				}else if(ib->m_ButtonID == cfg.keys[CONTROL_QUIT]){
					
					//	The player wants to quit
					Zargadia	=	GameOver;
					SetState(INPUT_NONE);
					return;

				//	CONTROL_PAUSE
				}else if(ib->m_ButtonID == cfg.keys[CONTROL_PAUSE]){
					if(ib->m_EventAction == IInputDevice::BUTTON_DOWN){
						SetState(INPUT_PAUSE);
						miscdisplay.ShowPause();
						//	Activate a "PAUSED" Message
					}

				//	CONTROL_FULLSCREEN
				}else if(ib->m_ButtonID == cfg.keys[CONTROL_FULLSCREEN]){
					if(ib->m_EventAction == IInputDevice::BUTTON_DOWN){
						ToggleGraphics();
						return;
					}
				}
			}

		//======================================================
		//	Collect all the Pause events
		//
		//	CONTROL_PAUSE		= player unpauses game
		//	CONTROL_FULLSCREEN	= player wants fullscreen game
		//======================================================
		}else if(state[1] == INPUT_PAUSE){
			
			//	CONTROL_PAUSE
			if(ib->m_ButtonID == cfg.keys[CONTROL_PAUSE]){
				if(ib->m_EventAction == IInputDevice::BUTTON_DOWN){
					int temp = state[0];
					SetState(temp);
					miscdisplay.HidePause();
				}
			}
		//================================================
		//	Collect all the "input name" events
		//
		//	CONTROL_LEFT	=	player moves input left
		//	CONTROL_RIGHT	=	player moves input right
		//	Any Ascii Code	=	player is typing a letter
		//================================================
		}else if(state[1] == INPUT_NAME){
			if(ib->m_EventAction == IInputDevice::BUTTON_DOWN){
				
				//	CONTROL_LEFT
				if(ib->m_ButtonID == cfg.keys[CONTROL_LEFT]){
					name_position--;
				//	CONTROL_RIGHT
				}else if(ib->m_ButtonID == cfg.keys[CONTROL_RIGHT]){
					name_position++;
				//	CONTROL_FIRE
				}else if(ib->m_ButtonID == cfg.keys[CONTROL_FIRE]){
					//	Update the highscore table
					Highscore->Update(PlayerName,gamedisplay.PlayerScore);
					//	Hide the enter name text
					miscdisplay.HideEnterName();
					//	Reset the name position entry point
					name_position = 0;
					//	Execute the titlescreen
					Zargadia = Titlescreen;
				//	Ascii code
				}else{
					PlayerName[name_position] = (char)AsciiKeyMap[ib->m_ButtonID];
					miscdisplay.UpdateEnterName(PlayerName);
				}
				
				//	Bounds check the name entry position
				if(name_position < 0) name_position = 0;
				if(name_position > 2) name_position = 2;
			}
		}
	}

//=======================================
//	Now you've collected all the events
//	if the Input filter is set to INPUT_GAME
//	then you have to process the input
//	which affects the player
//=======================================
	if(state[1] == INPUT_GAME){
		if(player.sprite->GetActive() == true){
			pos = player.sprite->GetPosition();
			
			//	Move the player
			if((pos->x+move >= 0) && (pos->x+move < (800-50)))	pos->x+=move;

			//	You're going to alter the frame currently being used, so grab a mesh ptr
			Overlay *overlay = reinterpret_cast<Overlay *>(player.sprite->GetMesh());

			//	Get the current time and subtract the time since they started
			//	moving to get the number of ms they have pressed the button for
			int time = GameTimer.GetMillisecondsAbs();
			if((time - animation_timer) > 30){
				//	the number of ms has exceeded requirements, so you have to animate

				animation_timer = time;
				
				//	If they are moving left
				if(move == -5){
					//	subtract the current frame, bounds check
					//	it against 0, then set the frame
					if(--current_frame < 0) current_frame = 0;
					overlay->SetFrame(current_frame);

				//	If they are moving right
				}else if(move == 5){
					//	add 1 to the current frame, bounds check
					//	against 20 (max player frame) and then
					//	set the frame
					if(++current_frame > 20) current_frame = 20;
					overlay->SetFrame(current_frame);

				//	If they are stationary
				}else if(move == 0){
					//	find the number of frames diff to the
					//	"stationary" frame
					int diff = 10 - current_frame;

					//	Make sure it's not zero, cause otherwise
					//	no point in changing the frame to the one
					//	already being used
					if(diff != 0){
						//	create a normal to point
						//	in either a +ve or -ve direction
						diff = abs(diff)/diff;
						//	add the normal to the current frame
						//	then use that to change the frame
						current_frame += diff;
						overlay->SetFrame(current_frame);
					}
				}
			}
		}
		//	If they player wants to fire, let them fire dammit!
		if(fire == true) playerweapon.Fire(pos);
	}

//	Now that you've processed all the events
//	just make sure it's clean by clearing the vector
//	NOTE:
//	this is so horrible, I wanna get rid of this vector
//	and replace it with a simple linked list....
	inputqueue = NULL;
}

void InputInterface::Reset(void)
{
	//	reset the objects controlling the input
	inputqueue = NULL;

	PlayerName[0] = 'A';
	PlayerName[1] = 'A';
	PlayerName[2] = 'A';
	PlayerName[3] = '\0';
	
	//	reset the variables controlling the player
	move	= 0;
	fire	= false;
}

void InputInterface::SetState(int newstate)
{
	state[0] = state[1];
	state[1] = newstate;
}

int InputInterface::GetState(void)
{
	return state[1];
}

Entity * InputInterface::InitCursor(void)
{
	if(mousecursor==NULL){
		InputMouse	*m	=	NULL;
		ITexture		*tb	=	fusion->Graphics->CreateTexture("file://sprites/mousecursor.tga");
		Overlay			*o	=	fusion->Mesh->CreateOverlay(tb);
		mousecursor			=	fusion->Mesh->CreateEntity(o);
		
		o->AddFrame(new Rect(0,0,64,64));			

		//	Translate/scale the entity Add the entity to the scene and set
		//	the mouse device to use it as a pointer
		mousecursor->SetTranslate(TRANSLATE_ABS,SCREENWIDTH/2,SCREENHEIGHT/2,1);
		mousecursor->SetScale(50,50,1);
		mouse->SetCursor(mousecursor);
		
		m = reinterpret_cast<InputMouse *>(mouse);
		m->SetHotspot(10,10);
	}

	return mousecursor;
}
