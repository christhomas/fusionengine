#ifndef _FUSION_H_
	#define _FUSION_H_

//	Subsystem include files
#include <Font/IFont.h>
#include <GUI/gui.h>
#include <Mesh/MeshDB.h>
#include <Input/InputDeviceDB.h>
#include <Graphics/IGraphics.h>
#include <Graphics/SceneGraphDB.h>
#include <VirtualFS/VirtualFS.h>
#include <Sound/ISound.h>
#include <INetworkCore.h>
#include <FusionSubsystem.h>
//	Fusion helper classes + data
#include <PlatformData.h>
#include <Rect.h>
#include <IModuleDB.h>
//	STL file stream to read the config file
#include <fstream>

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
class Fusion{
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

	/** @def	numsystems
	 *	A constant which defines how many subsystems there are in Fusion 
	 */
	#define numsystems 5

	/**	@var		create_t create[numsystems]
	 *	@brief	An array of ptrs which are used to hold the 
	 *					functions used to create the various Fusion subsystems.
	 */
	create_t create[numsystems];

	/**	@var		destroy_t destroy[numsystems]
	 *	@brief	An array of ptrs which are used to hold the 
	 *					functions used to destroy the various Fusion subsystems.
	 */
	destroy_t destroy[numsystems];

	/**	@var		char *m_libfilename[numsystems]
	 *	@brief	Stores dll module filenames.
	 *
	 *					When the configuration file is loaded, the filenames 
	 *					of the dll modules that contain the various Fusion 
	 *					objects is stored, so if requested later, they can 
	 *					be used to load the dll module and access the subsystem
	 */
	char *m_libfilename[numsystems];
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
	IGraphics				*Graphics;	/**< Takes care of handling the screen and graphics commands																		*/
	InputDeviceDB		*Input;			/**< Takes care of input devices and event structures generated by	them												*/
	MeshDB					*Mesh;			/**< Takes care of loading/storing mesh data for use in the application													*/
	SceneGraphDB		*Scene;			/**< Manages a set of scenegraph's which are responsible for rendering entities(also collision)	*/
	UserInterfaceDB	*Interface;	/**< Takes care of a set of windows, each containing their set of window components							*/
	FontDB					*Font;			/**< Looks after all the fonts which are loaded and stored for use within the UserInterface			*/
	ISound					*Sound;			/**< Loads Sounds, Music and Effects, handles playing them and other effects										*/
	INetworkCore		*Network;		/**< Handles communication between two machines across a LAN/Modem connection										*/
	VirtualFS				*vfs;				/**< Loads files and handles their formats invisibly through the data held in the config file		*/
	/**@}*/

	/**
	 *	@enum Subsystem
	 *	Enumerated identifiers for each supported subsystem
	 */
	enum Subsystem{
		GRAPHICS=0,	/**<	(id = 0) Graphics subsystem		*/
		INPUT,			/**<	(id = 1) Input Subsystem			*/
		SOUND,			/**<	(id = 2) Sound Subsystem			*/
		NETWORK,		/**<	(id = 3) Network Subsystem		*/
		VFS					/**<	(id = 4) VirtualFS Subsystem	*/
	};

									Fusion					();
									~Fusion					();
	bool						Update					(void);
	void						Pause						(bool active);
	void						LoadConfig			(char *configfile="fusion.cfg");
	void						InitSystem			(Fusion::Subsystem id, create_t c=NULL);
	void						UnloadModules		(void);
	PlatformData *	GetPlatformData	(void);
	IModuleDB *			GetModuleDB			(void);
};

extern Fusion *fusion;

#endif // #ifndef _FUSION_H_
