/***************************************************************************
                         UnixModuleDB.cpp  -  description
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
#include "UnixModuleDB.h"
#include "UnixModule.h"

#include <cstring>
#include <cstdio>
#include <dlfcn.h>

/**	Unix ModuleDB Constructor
 *
 *	Sets all internal data to zero/NULL
 */
UnixModuleDB::UnixModuleDB()
{
	m_nummodules	= 0;
	m_numsearchpaths	= 0;

	m_module	= NULL;
	m_searchpath	= NULL;
}

/**	Unix ModuleDB Deconstructor
 *
 *	Operation:
 *		-#	Deletes all DSO Modules and unloads them
 *		-#	Deletes all search paths
 */
UnixModuleDB::~UnixModuleDB()
{
	int a;

	for ( a = 0;a < m_nummodules;a++ ) delete m_module[ a ];
	for ( a = 0;a < m_numsearchpaths;a++ ) delete m_searchpath[ a ];

	delete[] m_module;
	delete[] m_searchpath;
}

/**	Retrieves a DSO Module
 *
 *	@param name	The filename of the DSO Module being searched for
 *
 *	@returns	A IModule pointer or NULL is the module is not found
 */
IModule * UnixModuleDB::GetModule( char *name )
{
	for ( int a = 0;a < m_nummodules;a++ )
	{
		if ( strcmp( m_module[ a ] ->GetFilename(), name ) == 0 ) return m_module[ a ];
	}

	return NULL;
}

/**	Adds a searchpath to the ModuleDB
 *
 *	@param path	The path to add to the search list
 *
 *	Operation:
 *		-#	Allocate a new array of searchpath string pointers, increasing the size by one
 *		-#	Copy all the old searchpath pointers to the new array
 *		-#	Delete the old searchpath array
 *		-#	Reset the searchpath array pointer to the new array
 *		-#	Allocate a new searchpath string
 *		-#	Copy the new searchpath to the array
 *		-#	Increase the number of searchpaths by one
 */
void UnixModuleDB::AddPath( char *path )
{
	char **temp = new char * [ m_numsearchpaths + 1 ];

	for ( int a = 0;a < m_numsearchpaths;a++ ) temp[ a ] = m_searchpath[ a ];

	delete[] m_searchpath;

	m_searchpath = temp;

	m_searchpath[ m_numsearchpaths ] = new char[ strlen( path ) + 1 ];
	strcpy( m_searchpath[ m_numsearchpaths ], path );

	m_numsearchpaths++;
}

/**	Adds a module to the database
 *
 *	@param module	An IModule	pointer
 *
 *	Operation:
 *		-#	Allocate a new array of module pointers, increasing the size by one
 *		-#	Copy all the old module pointers to the new array
 *		-#	Delete the old module array
 *		-#	Reset the module array pointer to the new array
 *		-#	Assign the extra pointer to the module paramter passed into the method
 *		-#	Increase the number of modules by one
 */
void UnixModuleDB::AddModule( IModule *module )
{
	IModule **temp = new IModule * [ m_nummodules + 1 ];

	for ( int a = 0;a < m_nummodules;a++ ) temp[ a ] = m_module[ a ];

	delete[] m_module;

	m_module = temp;

	m_module[ m_nummodules ] = module;

	m_nummodules++;
}

/**	Loads a DSO Module
 *
 *	@param	name	The filename of the module to load
 *
 *	@returns	A IModule pointer or NULL if not found
 *
 *	Operation:
 *		-#	Test whether the filename is valid
 *		-#	Attempt to retrieve a pointer to that module, test whether it's already loaded or not
 *		-#	If not loaded, loop through all the searchpaths, attempting to load the DSO Module at each path
 *		-#	Allocate a new UnixModule object
 *		-#	Create a absolute path to the DSO Module based on the current searchpath and the DSO Module filename
 *		-#	Attempt to load the DSO Module
 *		-#	If the attempt was successful, add the module to the Database and clear up, then return
 *		-#	clear up all temporary memory and try the next searchpath
 */
IModule * UnixModuleDB::LoadModule( char *name )
{
	UnixModule * m = NULL;

	if ( name != NULL )
	{
		m = ( UnixModule * ) GetModule( name );

		if ( m == NULL )
		{
			for ( int a = 0;a < m_numsearchpaths;a++ )
			{
				char *path = m_searchpath[ a ];

				m = new UnixModule( name, path, NULL );

				char *filename	= new char[ strlen( path ) + strlen( "/" ) + strlen( name ) + 1 ];

				sprintf( filename, "%s/%s", path, name );

				if ( m->Load( filename ) != NULL )
				{
					AddModule( m );

					delete[] filename;
					return m;
				}

				delete[] filename;
				delete m;
			}
		}
		else
		{
			return m;
		}
	}

	return NULL;
}

/**	Unloads a DSO module
 *
 *	@param name	The filename of the DSO Module to unload
 *
 *	@returns	Boolean true or false, depending on whether the DSO Module successfully unloaded or not
 *
 *	Operation:
 *		-#	Loop through the loaded modules, testing the filename against the one being searched for
 *		-#	If match is found, attempt to remove that module from the database
 *		-#	Create a new temporary module array, with one less element
 *		-#	Loop through all the loaded DSO modules, if the module being copied is the one being deleted, skip it
 *		-#	Copy each module pointer for all those that are not being removed
 *		-#	Delete the module being unloaded
 *		-#	Delete the module array
 *		-#	Reassign the module array pointer to the temporary array
 *		-#	Reduce the number of modules in the database
 */
bool UnixModuleDB::UnloadModule( char *name )
{
	int a, b, c;

	for ( a = 0;a < m_nummodules;a++ )
	{
		if ( strcmp( name, m_module[ a ] ->GetFilename() ) == 0 )
		{
			IModule **temp = new IModule * [ m_nummodules - 1 ];

			for ( b = 0, c = 0;b < m_nummodules;b++ )
			{
				if ( b == a ) continue;

				temp[ c++ ] = m_module[ b ];
			}

			delete m_module[ a ];

			delete[] m_module;

			m_module = temp;

			m_nummodules--;

			return true;
		}
	}

	return false;
}

/**	Unloads all the DSO modules from the database
 *
 *	Operation:
 *		-#	Constantly requests to unload the first DSO module in the array
 *		-#	When the array is empty, then all the DSO Modules are released
 */
void UnixModuleDB::UnloadAll( void )
{
	while ( m_nummodules > 0 )
	{
		UnloadModule( m_module[ 0 ] ->GetFilename() );
	}
}

/**	Retrieve a function pointer from a DSO Module
 *
 *	@param	name	The filename of the DSO Module
 *	@param	func	The name of the function requested
 *
 *	@returns	A Function pointer to the correct DSO function or NULL if not found
 *
 *	Operation:
 *		-#	Attempt to load the DSO Module
 *		-#	If successful, Attempt to retrieve a function pointer from that module
 */
void * UnixModuleDB::GetFunction( char *name, char *func )
{
	UnixModule * m;

	if ( ( m = ( UnixModule * ) LoadModule( name ) ) == NULL ) return NULL;

	return dlsym(m->GetHandle(),func);
}
