/***************************************************************************
	                      Win32Module.cpp  -  description
	                         -------------------
	begin                : Thursday 6th April 2006
	copyright            : (C) 2006 by Chris Thomas
	email                : chris.alex.thomas@gmail.com
	
	description:
	
	A Win32 specific Module class which loads a particular dll/so from the 
	disk into memory so the module system can operate upon it

	licence: bsd
	
***************************************************************************/

/***************************************************************************
Christopher Thomas (chris.alex.thomas@gmail.com)
Antimatter Studios
22nd March 2006

In the original BSD license, both occurrences of the phrase 
"COPYRIGHT HOLDERS AND CONTRIBUTORS" in the disclaimer read 
"REGENTS AND CONTRIBUTORS".

Here is the license template:

Copyright (c) 2006, Christopher Thomas
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, 
  	  this list of conditions and the following disclaimer.
    
    * Redistributions in binary form must reproduce the above copyright notice, 
	    this list of conditions and the following disclaimer in the documentation 
    	and/or other materials provided with the distribution.
    
    * Neither the name of the Antimatter Studios nor the names of its contributors 
    	may be used to endorse or promote products derived from this software 
    	without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

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
