#include <FusionChat.h>

Fusion f,*fusion = &f;

#ifdef _DEBUG
	std::string fusioncfg = "fusion_d.xml";
#else
	std::string fusioncfg = "fusion.xml";
#endif

//	main function, runs the client side
int main(int argc, char **argv)
{
	//	Setup the initial app state	
	state.server = NULL;
	state.enableClient = false;
	state.enableServer = false;
	state.connected	= false;
	state.port = 5678;
	state.username = "fcuser";
	state.dconnect = 0;
	state.quit = false;
	
	//	Init GTK+
	initGTK(argc, argv);
	
	//	Init Fusion
	fusion->LoadConfig(fusioncfg);
	fusion->InitSystem(Fusion::NETWORK);
	
	fusion->errlog.disableFile();
	fusion->errlog.enableConsole();

	//	Run GTK+
	runGTK((GSourceFunc)processNetwork);
	
	//	Finished :)
	return 0;
}