/***************************************************************************
 *   Copyright (C) 2003 by Christopher Thomas                              *
 *   chris_alex_thomas@yahoo.co.uk                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include <VFSHandle_file.h>

class VFSHandle_file_win32: public VFSHandle_file{
public:
			VFSHandle_file_win32	(VFSTransport *t);
	virtual	~VFSHandle_file_win32	();
	
//=========================================================
//=========================================================
//	public File/Directory manipulation methods 
//=========================================================
//=========================================================
	virtual bool		IsFile				(char *filename);
	virtual bool		IsDirectory			(char *directory);

	virtual FileInfo *	GetFileInfo			(char *filename);

	virtual bool		Createfile			(char *filename, bool recurse=true);
	virtual bool		Deletefile			(char *filename);

	virtual bool		Copyfile			(char *src, char *dest, bool createpath);
	virtual bool		Movefile			(char *src, char *dest, bool createpath);

	virtual bool		CreateDir			(char *directory);
	virtual bool		DeleteDir			(char *directory, bool recurse);	
};