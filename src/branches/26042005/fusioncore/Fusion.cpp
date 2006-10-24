#include <Fusion.h>

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
	Mesh	= NULL;
	Scene	= NULL;
	Interface	= NULL;
	Font		= NULL;
	Sound	= NULL;
	Network	= NULL;
	vfs		= NULL;

	//	Null out the lib filename, create + destroy ptrs for all the supported objects
	for ( int a = 0;a < numsystems;a++ )
	{
		m_libfilename[ a ]	= NULL;
		create[ a ]		= NULL;
		destroy[ a ]		= NULL;
	}
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

	//	Delete all the module data
	delete m_moduledb;
	m_moduledb = NULL;
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

	if ( m_active == true )
	{
		if ( Input != NULL )
		{
			if ( Input->Update() == false )
			{
				return false;
			}
		}

		if ( Interface->Update() == false )
			return false;
		if ( Scene->RenderScene() == false )
			return false;

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

/**
 *	Tells Fusion to load a configuration file
 *
 *	@param configfile char pointer to config filename
 *
 *	This method will search through the config file, if it parses [FUSION]
 *	it'll happily start extracting data from that section, until the next 
 *	section is detected (which, incidentally starts with "[").  Upon reading 
 *	a valid command, the data section will be removed (part of the string 
 *	after the = in each valid command line) and compared against what 
 *	Fusion is programmed to cope with, upon getting a correct comparison 
 *	from the command, the data section will be tagged into the appropiate 
 *	place in the internal configuration data, to be used later
 */
void Fusion::LoadConfig( char *configfile )
{
	//	FIXME:	Should probably move all this to use STL more
	//			considering it's 2-3 years old, I didnt know STL then
	//			should fix it now I know better
	bool begin = false;
	char buffer[ 2048 ];
	std::ifstream config( configfile );
	std::ofstream output( "output.txt" );

	output << "LoadConfig" << std::endl;

	//	Did the file open successfully?
	if ( config.is_open() == false )
		return ;

	//	Parse the config file, pull out all the data in the [Fusion] section
	while ( config.eof() == 0 )
	{
		config.getline( buffer, 2048 );

		//	Start reading the config file only when you reach the Fusion section
		if ( strcmp( buffer, "[FUSION]" ) == 0 )
		{
			begin = true;
			continue;
		}

		//	End reading when you read the next section (each section starts like [SECTION] or [ANOTHERSECTION]
		if ( ( begin == true ) && ( strncmp( buffer, "[", 1 ) == 0 ) )
		{
			break;
		}

		//	Ignore any blank lines
		if ( strcmp( buffer, "\0" ) == 0 )
		{
			continue;
		}

		//	If begun, process config data
		if ( begin == true )
		{
			char * command	= strtok( buffer, "=" );

			if ( command != NULL )
			{
				char * temp = strtok( &buffer[ strlen( command ) + 1 ], "\n" );
				char *param = new char[ strlen( temp ) + 1 ];
				strcpy( param, temp );

				output << "command:param = " << command << ":" << param << std::endl;
				for ( int a = 0;a < numsystems;a++ )
				{
					if ( m_libfilename[ a ] == NULL ){
						output << "m_filename[" << a << "] = " << (int *) m_libfilename[ a ] << std::endl;
					}else{
						output << "m_filename[" << a << "] = " << m_libfilename[ a ] << std::endl;
					}
				}
				output << std::endl;

				if ( strcmp( command, "MODULEDB" ) == 0 )
				{
					m_moduledb->AddPath( param );
					continue;
				}
				if ( strcmp( command, "GRAPHICS" ) == 0 )
				{
					m_libfilename[ GRAPHICS ] = param;
					continue;
				}
				if ( strcmp( command, "INPUT" ) == 0 )
				{
					m_libfilename[ INPUT ] = param;
					continue;
				}
				if ( strcmp( command, "SOUND" ) == 0 )
				{
					m_libfilename[ SOUND ] = param;
					continue;
				}
				if ( strcmp( command, "VFS" ) == 0 )
				{
					m_libfilename[ VFS ] = param;
					continue;
				}
				if ( strcmp( command, "NETWORK" ) == 0 )
				{
					m_libfilename[ NETWORK ] = param;
					continue;
				}

				delete[] param;
			}
		}
	}

	config.close();
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
	if ( id < numsystems )
	{

		if ( c != NULL )
		{
			create[ id ] = c;
			destroy[ id ] = NULL;
		}
		else
		{
			create[ id ] = ( create_t ) m_moduledb->GetFunction( m_libfilename[ id ], ( char * ) "GetInstance" );
			destroy[ id ] = ( destroy_t ) m_moduledb->GetFunction( m_libfilename[ id ], ( char * ) "DestroyInstance" );
		}

		if ( create[ id ] != NULL )
			create[ id ] ( *this );

		if ( id == GRAPHICS )
		{
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
	for ( int a = 0;a < numsystems;a++ )
	{
		//	Delete all initialised DLL Modules
		if ( destroy[ a ] != NULL )
		{
			destroy[ a ] ();
			destroy[ a ] = NULL;

			if ( m_moduledb->UnloadModule( m_libfilename[ a ] ) == true )
			{
				//	success code
			}
			else
			{
				//	failure code
			}

			delete[] m_libfilename[ a ];
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
