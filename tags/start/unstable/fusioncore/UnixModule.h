/***************************************************************************
                         UnixModule.cpp  -  description
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
#ifndef _UNIXMODULE_H_
	#define _UNIXMODULE_H_

#include <IModule.h>

/** @ingroup	Unix_ModuleDB_Group
 *	@brief		Derived IModule class for the Unix Platform
 */
class UnixModule: public IModule
{
protected:
	/**	@var		void *handle
	 *	@brief	The Unix handle to the DSO
	 */
	void	*handle;
public:
	UnixModule	( char *fn, char *p, char *ab );
	virtual	~UnixModule	();
	virtual void *	Load	( char *filename );
	virtual void *	GetHandle	( void );
	virtual char *	GetFilename	( void );
	virtual char *	GetPath	( void );
	virtual char *	GetAbout	( void );
};

#endif // #ifndef _UNIXMODULE_H_

