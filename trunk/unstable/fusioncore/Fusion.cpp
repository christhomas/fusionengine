#include <Fusion.h>

//	For XML Configuration file
#include <libxml/parser.h>

#ifdef _WIN32
	#include <Win32ModuleDB.h>
#else
	#include <UnixModuleDB.h>
#endif

void WindowActivateEvent( bool activate ){}
void WindowDestroyEvent( void ){}

/**
 *	Fusion Engine Constructor.
 *
 *	Creates the m_moduledb and m_platformdata objects, 
 *	assigns all ptrs to null, sets the default active state to true
 */
Fusion::Fusion()
{
	//	Setup the errlog system
	errlog.enableFile("errlog.txt");

#ifdef _WIN32
	m_moduledb	= new Win32ModuleDB;
	m_platform	= new Win32PlatformData;
#else
	//	Create Unix ModuleDB + Platform class here
	m_moduledb	= new UnixModuleDB;
	m_platform	= new UnixPlatformData;
#endif

	m_platform->Initialise();

	m_active	= true;

	Graphics	= NULL;
	Input		= NULL;
	Mesh		= NULL;
	Scene		= NULL;
	Interface	= NULL;
	Font		= NULL;
	Sound		= NULL;
	Network		= NULL;
	vfs			= NULL;
	
	m_libfilename.resize(NUMSUBSYS,"");
	create.resize(NUMSUBSYS,NULL);
	destroy.resize(NUMSUBSYS,NULL);
}

/**
 *	Fusion Engine Deconstructor.
 *
 *	Deletes all the requested subsystems, 
 *	also the IModuleDB and PlatformData objects
 */
Fusion::~Fusion()
{
	//	Unload all the DLL modules
	UnloadModules();

	//	Delete all the platform data
	delete m_platform;
	m_platform = NULL;

	//	Delete all the module data
	delete m_moduledb;
	m_moduledb = NULL;
	
	xmlCloseConfig();
}

/**
 *	Updates the Fusion system and all it's subobjects that are initialised
 *
 *	If Fusion is active it will: 
 *		-#	update the Input, 
 *		-#	update the UserInterface
 *		-#	update the SceneGraphDB
 *		-#	update the Graphics system
 *
 *	@returns boolean true or false, depending on whether the Input, Interface and Scenegraph objects rendered ok
 *
 *	If the Input, Interface or Scenegraph objects dont exist, this object will not return false, it will just skip their update methods.
 *
 *	Regardless of whether the Fusion Engine is active or not, 
 *	it will update the Graphic' Window message loop for 
 *	obvious reasons, if it didnt update, how can the user 
 *	click the Maximise or Close buttons?
 */
bool Fusion::Update( void )
{
	Graphics->Window->MessageLoop();

	if ( m_active == true ){
		if ( Input != NULL ){
			if ( Input->Update() == false ){
				return false;
			}
		}

		if ( Interface->Update() == false )		return false;
		if ( Scene->RenderScene() == false )	return false;

		Graphics->Update();
	}

	return true;
}

/**
 *	Sets whether to pause the Fusion Engine or not
 *
 *	@param active Tells whether to pause Fusion' execution or not
 */
void Fusion::Pause( bool active )
{
	m_active = active;
}

void Fusion::LoadConfig(std::string configfile)
{
	if(xmlLoadConfig(configfile) == true){
		void *root = xmlGetRootNode();
		void *fnode = xmlFindNode(root,"fusion");

		if(fnode != NULL){		
			unsigned int numNodes = xmlCountNodes(fnode);
			std::string name,data;
			
			for(unsigned int a=0;a<numNodes;a++){
				void *cnode = xmlGetChild(fnode,a);
				
				if(xmlGetNodeName(cnode) == "subsystem"){
					name = xmlGetNodeProperty(cnode,"name");
					data = xmlGetNodeProperty(cnode,"module");
					
					if(name.empty() == false && data.empty() == false){
						//	Get the function ptr to create the subsystem objects
						if(name == "graphics")	m_libfilename[GRAPHICS]	= data;
						if(name == "input")		m_libfilename[INPUT]	= data;
						if(name == "sound")		m_libfilename[SOUND]	= data;
						if(name == "vfs")		m_libfilename[VFS]		= data;
						if(name == "network")	m_libfilename[NETWORK]	= data;				
					}
				}else if(xmlGetNodeName(cnode) == "setup"){
					name = xmlGetNodeProperty(cnode,"name");
					data = xmlGetNodeProperty(cnode,"path");
					
					if(name.empty() == false && data.empty() == false){
						if(name == "moduledb") m_moduledb->AddPath(data);
					}
				}
			}
		}		
	}else{
		errlog << "xmlLoadConfig(): " << xmlGetError() << std::endl;
	}
}

/**
 *	Tells Fusion to initialise a supported subsystem
 *
 *	@param id Fusion::Subsystem
 *
 *	Attempts to retrieve a function ptr from the dll module which owns 
 *	the subsystem requested, if successful, it'll call the subsystem's 
 *	create method, and a new Subsystem will be available
 */
void Fusion::InitSystem( Fusion::Subsystem id, create_t c )
{
	if(id < NUMSUBSYS){
		if(c != NULL){
			create[id]	= c;
			destroy[id]	= NULL;
		}else{
			create[id]	= (create_t)	m_moduledb->GetFunction( m_libfilename[id], (char *)"GetInstance" );
			destroy[id]	= (destroy_t)	m_moduledb->GetFunction( m_libfilename[id], (char *)"DestroyInstance");
		}

		if(create[id] != NULL) create[id](*this);

		if(id == GRAPHICS){
			Graphics->ActivateEvent = WindowActivateEvent;
			Graphics->DestroyEvent	= WindowDestroyEvent;
		}
	}
}

/**	Unloads all the DLL based modules from the system
 *
 *	Example of use:
 *	If fusion system is shutting down some applications 
 *	require early release of all DLL modules because their 
 *	files are to be deleted from the disk, DLL modules 
 *	can't be deleted whilst they are opened by the 
 *	application, so they have to be closed first then 
 *	removed from the disk
 */
void Fusion::UnloadModules( void )
{
	for(unsigned int a=0;a<NUMSUBSYS;a++){
		//	Delete all initialised DLL Modules
		if(destroy[a] != NULL)
		{
			destroy[a]();
			destroy[a] = NULL;

			if(m_moduledb->UnloadModule( m_libfilename[ a ] ) == true){
				//	success code
			}else{
				//	failure code
			}
		}
	}

	m_moduledb->UnloadAll();
}

/**
 *	Retrieves a pointer to the Fusion systems Platform specific data
 *
 *	@return Pointer to the Platform Data
 */
PlatformData * Fusion::GetPlatformData( void )
{
	return m_platform;
}

/**
 *	Retrieves a pointer to the Fusion systems DLL Module Database
 *
 *	@return Pointer to the DLL Module Database
 */
IModuleDB * Fusion::GetModuleDB( void )
{
	return m_moduledb;
}
