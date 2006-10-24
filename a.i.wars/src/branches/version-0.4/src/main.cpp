#include <aiwars.h>

bool			fullscreen		=	false;	//	Whether to play in fullscreen or not (set this to inverse of what you actually want, trust me)
unsigned int	SCREENWIDTH		=	800;	//	Width of the screen
unsigned int	SCREENHEIGHT	=	600;	//	Height of the screen
Fusion			f,*fusion			= &f;	//	Fusion object
Timer			GameTimer;					//	Global game timer

#ifdef _DEBUG
	std::string fusioncfg = "system/fusion_d.xml";
#else
	std::string fusioncfg = "system/fusion.xml";
#endif

//	Program Flow control function ptrs
bool (*System)	(void)	=	NULL;
bool (*aiwars)	(void)	=	NULL;

int main(int argc, char **argv)
{
	System = SystemInit;

	while(true)	if(System() == false) return false;
}

bool SystemInit(void)
{
	//	Create and load config for the engine
	fusion->LoadConfig(fusioncfg);
	
	//	Initialise all the systems you need
	fusion->InitSystem(Fusion::GRAPHICS);
	fusion->InitSystem(Fusion::INPUT);
//	fusion->InitSystem(Fusion::SOUND);
	fusion->InitSystem(Fusion::VFS);

	fusion->vfs->LoadConfig();

	if(ToggleGraphics() == true){
		fusion->Graphics->ActivateEvent	= ActivateWindow;
		fusion->Graphics->DestroyEvent	= DestroyWindow;

		fusion->Graphics->Window->SetTitle("AI Wars - 0.3 Alpha");

		aiwars	= Load;
		System	= SystemRun;
		return true;
	}
	
	//	If something goes wrong, quit
	System = SystemQuit;
	return true;
}

bool SystemRun(void)
{
	GameTimer.Tick();

	fusion->Graphics->ClearScreen();

	if(aiwars() == false){
		System = SystemQuit;
	}else{
		fusion->Update();
	}

	return true;
}

bool SystemQuit(void)
{
	return false;
}

void ActivateWindow(bool activate)
{
	// When the window becomes inactive, you should pause execution of the app
	// when the window becomes active again, you should resume the execution
	fusion->Pause(activate);
}

void DestroyWindow(void)
{
	// when the window is destroyed, you want to quit
	System = SystemQuit;
}

bool ToggleGraphics(void)
{
	if(fusion->Graphics->SetMode(SCREENWIDTH,SCREENHEIGHT,fullscreen) == true){
		fusion->Graphics->Enable(IGraphics::MODE2D);
		fusion->Graphics->ReloadTextures();
		
		fullscreen=!fullscreen;
		
		return true;
	}

	return false;
}

