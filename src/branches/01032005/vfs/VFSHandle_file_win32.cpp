/***************************************************************************
 *   Copyright (C) 2003 by Christopher Thomas                              *
 *   chris_alex_thomas@yahoo.co.uk                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <VFSHandle_file_win32.h>
#include <io.h>
#include <sys/stat.h>
#include <direct.h>

VFSHandle_file_win32::VFSHandle_file_win32(VFSTransport *t): VFSHandle_file(t){}

VFSHandle_file_win32::~VFSHandle_file_win32(){}
 
//=========================================================
//=========================================================
//	public File/Directory manipulation methods
//=========================================================
//=========================================================
bool VFSHandle_file_win32::IsFile( std::string filename )
{
	_finddata_t find;

	intptr_t Filehandle = _findfirst( filename.c_str(), &find );

	if ( Filehandle == -1 )
	{
		if ( errno == ENOENT ) {}
		if ( errno == EINVAL ) {}

		_findclose( Filehandle );
		return false;
	}

	_findclose( Filehandle );

	return true;
}

bool VFSHandle_file_win32::IsDirectory( std::string directory )
{
	if (directory.empty() == false)
	{
		_finddata_t find;

		intptr_t Filehandle = _findfirst( directory.c_str(), &find );

		if ( Filehandle == -1 )
		{
			if ( errno == ENOENT ) {}
			if ( errno == EINVAL ) {}

			_findclose( Filehandle );
			return false;
		}

		_findclose( Filehandle );
	}

	return true;
}

//	dummy for the moment, since I dont actually need to extract this kind of information yet
FileInfo * VFSHandle_file_win32::GetFileInfo( std::string filename )
{
	struct stat s;

	if ( stat( filename.c_str(), &s ) == -1 )
	{
		if ( errno == ENOENT ) {};

		return NULL;
	}

	return NULL;
}

bool VFSHandle_file_win32::Createfile( std::string filename, bool recurse )
{
	//	TODO:	make sure the file is created, make all the directories required too
	if ( recurse == true ) CreateDir( filename );

	m_stream.clear();
	m_stream.open( filename.c_str(), std::ios::out );

	bool r = ( bool ) m_stream.is_open();

	m_stream.close();

	return r;
}

bool VFSHandle_file_win32::Deletefile( std::string filename )
{
	if ( remove( filename.c_str() ) == -1 )
	{
		if ( errno == EACCES ) return false; // read only file
		if ( errno == ENOENT ) return false;	// file does not exist
	}

	return true;
}

bool VFSHandle_file_win32::Copyfile( std::string src, std::string dest, bool createpath )
{
	return false;
}

bool VFSHandle_file_win32::Movefile( std::string src, std::string dest, bool createpath )
{
	return false;
}

bool VFSHandle_file_win32::CreateDir( std::string directory )
{
	return false;
}

bool VFSHandle_file_win32::DeleteDir( std::string directory, bool recurse )
{
	return false;

/*	char path[ 256 ];

	sprintf( path, "%s/*.*", directory );

	if ( recurse == true ){
		_finddata_t find;

		intptr_t Filehandle = _findfirst( path, &find );

		if ( Filehandle == -1 ){
			if ( errno == ENOENT ){
				_findclose( Filehandle );
				return DeleteDir( directory, false );
			}

			if ( errno == EINVAL ){
				_findclose( Filehandle );
				return false; // should this happen ever with *.* ? I dont think so....
			}
		}else{
			do{
				if ( strcmp( find.name, "." ) != 0 && strcmp( find.name, ".." ) != 0 ){
					memset( path, 0, 256 );
					sprintf( path, "%s/%s", directory, find.name );
					Deletefile( path );
				}
			}while ( _findnext( Filehandle, &find ) == 0 );

			_findclose( Filehandle );
			return DeleteDir( directory, false );
		}

		//	have to delete all the files in the folder, then remove it, all the way back to the root directory requested to delete

		return false;
	}

	if ( rmdir( directory ) == -1 ){
		if ( errno == ENOTEMPTY ) return false;
		if ( errno == ENOENT ) return false;
	}

	return true;
*/
}
