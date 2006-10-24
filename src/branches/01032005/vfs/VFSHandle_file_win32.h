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
	virtual bool		IsFile				(std::string filename);
	virtual bool		IsDirectory			(std::string directory);

	virtual FileInfo *	GetFileInfo			(std::string filename);

	virtual bool		Createfile			(std::string filename, bool recurse=true);
	virtual bool		Deletefile			(std::string filename);

	virtual bool		Copyfile			(std::string src, std::string dest, bool createpath);
	virtual bool		Movefile			(std::string src, std::string dest, bool createpath);

	virtual bool		CreateDir			(std::string directory);
	virtual bool		DeleteDir			(std::string directory, bool recurse);	
};