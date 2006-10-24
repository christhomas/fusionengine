#ifndef _IMODULEDB_H_
	#define _IMODULEDB_H_

#include <IModule.h>

/** @ingroup	ModuleDB_Helper_Group
 *	@brief		The base class of the Module Database
 */
class IModuleDB{
protected:
	/**	@var		IModule **m_module
	 *	@brief	An array of DLL Modules
	 */
	IModule **m_module;

	/**	@var		char **m_searchpath
	 *	@brief	An array of search paths for loading DLL Modules
	 */
	char **m_searchpath;

	/**	@var		int m_nummodules
	 *	@brief	The number of loaded DLL Modules
	 */
	int m_nummodules;

	/**	@var		int m_numsearchpaths
	 *	@brief	The number of available search paths
	 */
	int m_numsearchpaths;

	/**	Retrieves a ptr to a particular DLL module 
	 *	
	 *	@param	name	The filename of the module
	 *
	 *	@returns A pointer to a module, or NULL, if module is not found
	 */
	virtual IModule * GetModule(char * name) = 0;

	/**	Stores a DLL module
	 *
	 *	@param module	The module to store
	 */
	virtual void AddModule(IModule *module) = 0;
public:

	/**	IModuleDB Constructor */
	IModuleDB(){};

	/**	IModuleDB Deconstructor */
	virtual ~IModuleDB(){};

	/**	Adds a search path to the DB
	 *
	 *	@param	path	The path to add to the database
	 */
	virtual void AddPath(char *path) = 0;

	/**	Loads a module
	 *
	 *	@param name	The filename of the DLL Module to load
	 *
	 *	@returns A pointer to the loaded DLL module
	 */
	virtual IModule * LoadModule(char *name) = 0;

	/**	Unloads a module
	 *
	 *	@param name	The filename of the DLL module to unload
	 *
	 *	@returns Boolean true or false, depending on whether the module was successfully unloaded
	 */
	virtual bool UnloadModule(char *name) = 0;

	/**	Unloads all the modules from the database
	 *
	 *	This is essentially used when cleaning up what modules fusion has open
	 */
	virtual void UnloadAll(void) = 0;

	/**	Retrieves a function pointer from the DLL Module
	 *
	 *	@param	name	The name of the DLL module to retrieve the function from
	 *	@param	func	The name of the function to retrieve
	 *	
	 *	@returns	A Function pointer, or NULL, if the module/function was not found
	 */
	virtual void * GetFunction(char *name,char *func) = 0;
};

#endif	//	#ifndef _IMODULEDB_H_

