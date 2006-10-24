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
#include "UnixModule.h"

#include <cstring>
#include <dlfcn.h>

/**	Initialises the module
 *
 *	@param	fn	The filename of the DSO Module
 *	@param	p	The path of the DSO Module
 *	@param	ab	The about string to set this module with
 */
UnixModule::UnixModule( char *fn, char *p, char *ab )
{
	filename = NULL;
	path = NULL;
	about = NULL;

	if ( fn != NULL )
	{
		filename = new char[ strlen( fn ) + 1 ];
		strcpy( filename, fn );
	}

	if ( p != NULL )
	{
		path = new char[ strlen( p ) + 1 ];
		strcpy( path, p );
	}

	if ( ab != NULL )
	{


		about = new char[ strlen( ab ) + 1 ];
		strcpy( about, ab );
		delete ab;
	}

	handle = NULL;
}

/**	Unix Module Destructor
 *
 *	Destroy any memory allocated and Free the handle to the DSO module
 */
UnixModule::~UnixModule()
{
	delete[] filename;
	delete[] path;
	delete[] about;

	if(handle != NULL) dlclose(handle);
}

/**	Loads a DSO Module
 *
 *	@param	filename	The filename of the DSO Module
 *
 *	@returns	The platform dependant handle to the DSO Module
 */
void * UnixModule::Load( char *filename )
{
	handle = dlopen(filename, RTLD_LAZY);

	return handle;
}

/**	Retrieves the handle of the DSO Module
 *
 *	@returns	A platform dependant handle for the DSO MOdule
 */
void * UnixModule::GetHandle( void )
{
	return handle;
}

/**	Retrieves the filename of the DSO Module
 *
 *	@returns	A String containing filename for the DSO Module
 */
char * UnixModule::GetFilename( void )
{
	return filename;
}

/**	Retrieves the filename of the DSO Module
 *
 *	@returns	A String containing the path for the DSO Module
 */
char * UnixModule::GetPath( void )
{
	return path;
}

/**	Retrieves the about information for the DSO Module
 *
 *	@returns	A String containing the about information for the DSO Module
 */
char * UnixModule::GetAbout( void )
{
	return about;
}
