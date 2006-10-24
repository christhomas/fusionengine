#ifndef _WIN32MODULE_H_
	#define _WIN32MODULE_H_

#include <Windows.h>
#include <IModule.h>

/** @ingroup	Win32_ModuleDB_Group
 *	@brief		Derived IModule class for the Win32 Platform
 */
class Win32Module: public IModule{
protected:
	/**	@var		HINSTANCE	handle
	 *	@brief	The Win32 handle to the DLL
	 */
	HINSTANCE handle;
public:
									Win32Module		(char *fn,char *p,char *ab);
	virtual					~Win32Module	();
	virtual void *	Load					(char *filename);
	virtual void *	GetHandle			(void);
	virtual char *	GetFilename		(void);
	virtual char *	GetPath				(void);
	virtual char *	GetAbout			(void);
};

#endif // #ifndef _WIN32MODULE_H_

