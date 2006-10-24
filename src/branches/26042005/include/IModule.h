#ifndef _IMODULE_H_
	#define _IMODULE_H_

/** @ingroup	ModuleDB_Helper_Group
 *	@brief		The base class for all DLL Modules
 */
class IModule{
protected:
	/**	@var		char *filename
	 *	@brief	The name of the DLL module
	 */
	char *filename;

	/**	@var		char *path
	 *	@brief	The path to the DLL Module 
	 */
	char *path;

	/**	@var		char *about
	 *	@brief	To hold any details about the DLL module, it's version or author, etc
	 */
	char *about;

public:
	/**	IModule Constructor */
	IModule(){};

	/**	IModule Deconstructor */
	virtual ~IModule(){};
	
	/**	Loads a DLL module
	 *
	 *	@param filename	The name of the file containing the DLL Module
	 *	
	 *	@returns	A pointer to the platform specific handle
	 */
	virtual void * Load(char *filename) = 0;

	/**	Retrieves the platform specific handle of the DLL module 
	 *
	 *	@returns	A pointer to the platform specific handle
	 */
	virtual void * GetHandle(void) = 0;

	/**	Retrieves the name of the DLL modules file 
	 *
	 *	@returns	A string containing the filename of the DLL Module
	 */
	virtual char * GetFilename(void) = 0;

	/**	Retrieves the path of the DLL module 
	 *
	 *	@returns	A string containing the path of the DLL module
	 */
	virtual char * GetPath(void) = 0;

	/**	Retrieves the about information for the DLL module 
	 *
	 *	@returns	A string containing the about information
	 */
	virtual char * GetAbout(void) = 0;	
};

#endif // #ifndef _IMODULE_H_

