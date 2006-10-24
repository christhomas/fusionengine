/***************************************************************************
                         UnixModuleDB.h  -  description
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
#ifndef _UNIXMODULEDB_H_
	#define _UNIXMODULEDB_H_

#include <IModuleDB.h>

/** @ingroup	Unix_ModuleDB_Group
 *	@brief		Derived IModuleDB class for the Unix Platform
 */
class UnixModuleDB: public IModuleDB
{
protected:
	virtual IModule *	GetModule	( char *name );
	virtual void	AddModule	( IModule *module );
public:
	UnixModuleDB	();
	virtual ~UnixModuleDB	();
	virtual void	AddPath	( char *path );
	virtual IModule *	LoadModule	( char *name );
	virtual bool	UnloadModule	( char *name );
	virtual void	UnloadAll	( void );
	virtual void *	GetFunction	( char *name, char *func );
};

#endif // #ifndef _UNIXMODULEDB_H_
