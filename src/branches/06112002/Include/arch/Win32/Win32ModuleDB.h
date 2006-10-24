#ifndef _WIN32MODULEDB_H_
	#define _WIN32MODULEDB_H_

#include <IModuleDB.h>
#include <windows.h>

/** @ingroup	Win32_ModuleDB_Group
 *	@brief		Derived IModuleDB class for the Win32 Platform
 */
class Win32ModuleDB: public IModuleDB{
protected:
	virtual IModule *	GetModule				(char *name);
public:
										Win32ModuleDB		();
	virtual						~Win32ModuleDB	();
	virtual void			AddPath					(char *path);
	virtual void			AddModule				(IModule *module);
	virtual IModule *	LoadModule			(char *name);
	virtual bool			UnloadModule		(char *name);
	virtual void			UnloadAll				(void);
	virtual void *		GetFunction			(char *name,char *func);
};

#endif // #ifndef _WIN32MODULEDB_H_

