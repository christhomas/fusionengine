#ifndef _FUSION_H_
	#define _FUSION_H_

//	Fusion helper classes + data
#include <PlatformData.h>
#include <Rect.h>
#include <IModuleDB.h>
#include <dbstream.h>

//	STL headers
#include <fstream>
#include <vector>
#include <string>

//	Subsystem include files
#include <font/IFont.h>
#include <gui/gui.h>
#include <mesh/MeshDB.h>
#include <input/InputDeviceDB.h>
#include <graphics/IGraphics.h>
#include <graphics/SceneGraphDB.h>
#include <vfs/VirtualFS.h>
#include <sound/ISound.h>
#include <INetworkCore.h>
#include <FusionSubsystem.h>
#include <XMLConfig.h>

/** @ingroup	Fusion_Group
 *	@brief		The core of the Fusion system
 *
 *	This is how developers of fusion powered applications gain access
 *	to functionality provided.  The various sub-objects are not guarenteed
 *	to be valid objects, since whether they are valid or not, depends on whether
 *	a sub-object has been requested, for example.  Applications which dont need sound
 *	won't create a sound object, so therefore the sound object will be NULL and should
 *	be left alone.  Any attempt to access a subsystem which was not requested, 
 *	is met with an application crash, stupidity is not tolerated.
 */
class Fusion: public XMLConfig{
protected:
	/** @name Subsystem Creation/Destruction Data
	 *	@{
	 */

	/** @typedef create_t
	 *	Function pointer used in Fusion subsystem creation
	 */
	typedef void (*create_t)	(Fusion &);

	/** @typedef destroy_t
	 *	Function pointer used in Fusion subsystem destruction
	 */
	typedef void (*destroy_t)	(void);

	/**	@var	create_t create[numsystems]
	 *	@brief	An array of ptrs which are used to hold the 
	 *			functions used to create the various Fusion subsystems.
	 */
	std::vector<create_t> create;

	/**	@var	destroy_t destroy[numsystems]
	 *	@brief	An array of ptrs which are used to hold the 
	 *			functions used to destroy the various Fusion subsystems.
	 */
	std::vector<destroy_t> destroy;

	/**	@var	char *m_libfilename[numsystems]
	 *	@brief	Stores dll module filenames.
	 *
	 *			When the configuration file is loaded, the filenames 
	 *			of the dll modules that contain the various Fusion 
	 *			objects is stored, so if requested later, they can 
	 *			be used to load the dll module and access the subsystem
	 */
	 std::vector<std::string> m_libfilename;
	/**@}*/

	
	/**	@var		bool m_active
	 *	@brief	Sets Fusion to active or inactive status.
	 *
	 *					The control variable for whether the Fusion engine is active, 
	 *					or not, for example.  If the window is not active, Fusion will 
	 *					cease to run, therefore preventing system resources from being 
	 *					gobbled up by an application which is not in use.  This may 
	 *					change to allow people to focus another window and still allow 
	 *					fusion to continue, but at the moment, there are no situations 
	 *					in which this sceanario can be envisaged
	 */
	bool m_active;

	/**	@var		IModuleDB *m_moduledb
	 *	@brief	Frontend for the dll module loader.
	 *
	 *					This object transparently loads dll modules for 
	 *					whatever operating system by hiding *how* the 
	 *					modules are loaded behind an abstract C++ class
	 */
	IModuleDB *m_moduledb;

	/**	@var		PlatformData *m_platform
	 *	@brief	Stores all the platform specific data.
	 *
	 *					The PlatformData object's job is to hide the system specifics 
	 *					behind an abstract C++ class, in the various subsystems, this will 
	 *					be blind casted to the appropiate derived class, hence allowing 
	 *					access to the system specifics, but only when the application 
	 *					or Fusion subsystem implementation needs to
	 */
	PlatformData *m_platform;
	
public:

	/** @name Subsystem pointers
	 *	@{
	 */
	IGraphics		*Graphics;	/**< Takes care of handling the screen and graphics commands																		*/
	InputDeviceDB	*Input;		/**< Takes care of input devices and event structures generated by	them												*/
	MeshDB			*Mesh;		/**< Takes care of loading/storing mesh data for use in the application													*/
	SceneGraphDB	*Scene;		/**< Manages a set of scenegraph's which are responsible for rendering entities(also collision)	*/
	UserInterfaceDB	*Interface;	/**< Takes care of a set of windows, each containing their set of window components							*/
	FontDB			*Font;		/**< Looks after all the fonts which are loaded and stored for use within the UserInterface			*/
	ISound			*Sound;		/**< Loads Sounds, Music and Effects, handles playing them and other effects										*/
	INetworkCore	*Network;	/**< Handles communication between two machines across a LAN/Modem connection										*/
	VirtualFS		*vfs;		/**< Loads files and handles their formats invisibly through the data held in the config file		*/
	/**@}*/
	
	/** @var	Error logging class
	 *	@brief	Uses a std::cout style class to control error output
	 *
	 *	This class provides advanced debugging to the entire fusion system
	 *	in this specific case, it's used to report errors from anywhere
	 *	in the system to a logging file
	 */
	dbg::debugstream errlog; 
	
	/**
	 *	@enum Subsystem
	 *	Enumerated identifiers for each supported subsystem
	 */
	enum Subsystem{
		GRAPHICS=0,	/**<	(id = 0) Graphics subsystem		*/
		INPUT,		/**<	(id = 1) Input Subsystem		*/
		SOUND,		/**<	(id = 2) Sound Subsystem		*/
		NETWORK,	/**<	(id = 3) Network Subsystem		*/
		VFS,		/**<	(id = 4) VirtualFS Subsystem	*/
		NUMSUBSYS	/**<	(id = n) Number of subsystems	*/
	};

							Fusion			();
	virtual					~Fusion			();
	virtual	bool			Update			(void);
	virtual	void			Pause			(bool active);
	virtual	void			LoadConfig		(std::string configfile);
	virtual	void			InitSystem		(Fusion::Subsystem id, create_t c=NULL);
	virtual	void			UnloadModules	(void);
	virtual	PlatformData *	GetPlatformData	(void);
	virtual	IModuleDB *		GetModuleDB		(void);
};

extern Fusion *fusion;

#endif // #ifndef _FUSION_H_
