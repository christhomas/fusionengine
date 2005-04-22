/***************************************************************************
 *   Copyright (C) 2003 by Christopher Thomas                              *
 *   chris_alex_thomas@yahoo.co.uk                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <VFSHandle_file_unix.h>
//	TODO: PORTING WORK REQUIRED

VFSHandle_file_unix::VFSHandle_file_unix(VFSTransport *t): VFSHandle_file(t)
{

}

VFSHandle_file_unix::~VFSHandle_file_unix()
{

}
 
//=========================================================
//=========================================================
//	public File/Directory manipulation methods
//=========================================================
//=========================================================
bool VFSHandle_file_unix::IsFile( char *filename )
{
/*	_finddata_t find;

	long Filehandle = _findfirst( filename, &find );

	if ( Filehandle == -1 )
	{
		if ( errno == ENOENT ) {}
		if ( errno == EINVAL ) {}

		_findclose( Filehandle );
		return false;
	}

	_findclose( Filehandle );
*/
	return true;
}

bool VFSHandle_file_unix::IsDirectory( char *directory )
{
/*
	if ( strcmp( directory, "" ) != 0 )
	{
		_finddata_t find;

		long Filehandle = _findfirst( directory, &find );

		if ( Filehandle == -1 )
		{
			if ( errno == ENOENT ) {}
			if ( errno == EINVAL ) {}

			_findclose( Filehandle );
			return false;
		}

		_findclose( Filehandle );
	}
*/
	return true;
}

//	dummy for the moment, since I dont actually need to extract this kind of information yet
FileInfo * VFSHandle_file_unix::GetFileInfo( char *filename )
{
/*
	struct stat s;

	if ( stat( filename, &s ) == -1 )
	{
		if ( errno == ENOENT ) {};

		return NULL;
	}
*/
	return NULL;
}

bool VFSHandle_file_unix::Createfile( char *filename, bool recurse )
{
/*	
	//	TODO:	make sure the file is created, make all the directories required too
	if ( recurse == true ) CreateDir( filename );

	m_stream.clear();
	m_stream.open( filename, std::ios::out );

	bool r = ( bool ) m_stream.is_open();

	m_stream.close();

	return r;
*/
	return false;
}

bool VFSHandle_file_unix::Deletefile( char *filename )
{
/*
	if ( remove( filename ) == -1 )
	{
		if ( errno == EACCES ) return false; // read only file
		if ( errno == ENOENT ) return false;	// file does not exist
	}
*/
	return true;
}

bool VFSHandle_file_unix::Copyfile( char *src, char *dest, bool createpath )
{
	return false;
}

bool VFSHandle_file_unix::Movefile( char *src, char *dest, bool createpath )
{
	return false;
}

bool VFSHandle_file_unix::CreateDir( char *directory )
{
/*
	char * dir = directory;

	for ( int a = 0;a < strlen( directory );a++ ) if ( dir[ a ] == '\\' ) dir[ a ] = '/';

	if ( dir[ 0 ] == '/' ) dir++;

	char *token = dir;

	while ( token != NULL )
	{
		token = strchr( token, '/' );

		if ( token != NULL )
		{
			int bytes = token - directory;

			char *temp = new char[ bytes + 1 ];

			memset( temp, 0, bytes + 1 );

			strncpy( temp, directory, bytes );

			if ( mkdir( temp ) == -1 ) if ( errno == ENOENT ) return false;

			delete[] temp;

			token++;
		}
	}
*/
	return true;
}

bool VFSHandle_file_unix::DeleteDir( char *directory, bool recurse )
{
/*
	char path[ 256 ];

	sprintf( path, "%s/*.*", directory );

	if ( recurse == true )
	{
		_finddata_t find;

		long Filehandle = _findfirst( path, &find );

		if ( Filehandle == -1 )
		{
			if ( errno == ENOENT )
			{
				_findclose( Filehandle );
				return DeleteDir( directory, false );
			}

			if ( errno == EINVAL )
			{
				_findclose( Filehandle );
				return false; // should this happen ever with *.* ? I dont think so....
			}
		}
		else
		{
			do
			{
				if ( strcmp( find.name, "." ) != 0 && strcmp( find.name, ".." ) != 0 )
				{
					memset( path, 0, 256 );
					sprintf( path, "%s/%s", directory, find.name );
					Deletefile( path );
				}
			}
			while ( _findnext( Filehandle, &find ) == 0 );

			_findclose( Filehandle );
			return DeleteDir( directory, false );
		}

		//	have to delete all the files in the folder, then remove it, all the way back to the root directory requested to delete

		return false;
	}

	if ( rmdir( directory ) == -1 )
	{
		if ( errno == ENOTEMPTY ) return false;
		if ( errno == ENOENT ) return false;
	}
*/
	return true;
}
