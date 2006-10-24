/***************************************************************************
                          Win32ModuleDB.h  -  description
                             -------------------
    begin                : Sat Mar 22 2003
    copyright            : (C) 2003 by ChrisThomas
    email                : chris_alex_thomas@yahoo.co.uk
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef _WIN32MODULEDB_H_
	#define _WIN32MODULEDB_H_

#include <IModuleDB.h>

/** @ingroup	Win32_ModuleDB_Group
 *	@brief		Derived IModuleDB class for the Win32 Platform
 */
class Win32ModuleDB: public IModuleDB{
protected:
	virtual IModule *	GetModule		(std::string name);
	virtual void		AddModule		(IModule *module);
public:
						Win32ModuleDB	();
	virtual				~Win32ModuleDB	();
	virtual void		AddPath			(std::string path);
	virtual IModule *	LoadModule		(std::string name);
	virtual bool		UnloadModule	(std::string name);
	virtual void		UnloadAll		(void);
	virtual void *		GetFunction		(std::string name,std::string func);
};

#endif // #ifndef _WIN32MODULEDB_H_

