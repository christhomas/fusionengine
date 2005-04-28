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
#include "Win32Module.h"

/**	Initialises the module
 *
 *	@param	fn	The filename of the DLL Module
 *	@param	p		The path of the DLL Module
 *	@param	ab	The about string to set this module with
 */
Win32Module::Win32Module(std::string fn,std::string p,std::string ab)
{
	filename	= fn;
	path		= p;
	about		= ab;
	handle		= NULL;
}

/**	Win32 Module Destructor
 *
 *	Destroy any memory allocated and Free the handle to the DLL module
 */
Win32Module::~Win32Module()
{
	if(handle != NULL)	FreeLibrary(handle);
}

/**	Loads a DLL Module
 *
 *	@param	filename	The filename of the DLL Module
 *
 *	@returns	The platform dependant handle to the DLL Module
 */
void * Win32Module::Load(std::string filename)
{
	handle = LoadLibrary(filename.c_str());

	return handle;
}

/**	Retrieves the handle of the DLL Module
 *
 *	@returns	A platform dependant handle for the DLL MOdule
 */
void * Win32Module::GetHandle(void)
{
	return handle;
}

/**	Retrieves the filename of the DLL Module
 *
 *	@returns	A String containing filename for the DLL Module
 */
std::string Win32Module::GetFilename(void)
{
	return filename;
}

/**	Retrieves the filename of the DLL Module
 *
 *	@returns	A String containing the path for the DLL Module
 */
std::string Win32Module::GetPath(void)
{
	return path;
}

/**	Retrieves the about information for the DLL Module
 *
 *	@returns	A String containing the about information for the DLL Module
 */
std::string Win32Module::GetAbout(void)
{
	return about;
}
