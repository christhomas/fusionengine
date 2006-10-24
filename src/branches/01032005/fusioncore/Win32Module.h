/***************************************************************************
                          Win32Module.cpp  -  description
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
						Win32Module		(std::string fn,std::string p,std::string ab);
	virtual				~Win32Module	();
	virtual void *		Load			(std::string filename);
	virtual void *		GetHandle		(void);
	virtual	std::string	GetFilename		(void);
	virtual std::string GetPath			(void);
	virtual std::string	GetAbout		(void);
};

#endif // #ifndef _WIN32MODULE_H_

