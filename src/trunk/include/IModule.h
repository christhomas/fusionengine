/***************************************************************************
	                      IModule.h  -  description
	                         -------------------
	begin                : Thursday 6th April 2006
	copyright            : (C) 2006 by Chris Thomas
	email                : chris.alex.thomas@gmail.com
	
	description:
	
	A Module class which loads a particular dll/so from the disk into memory

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

#ifndef _IMODULE_H_
	#define _IMODULE_H_
	
#include <string>

/** @ingroup	ModuleDB_Helper_Group
 *	@brief		The base class for all DLL Modules
 */
class IModule{
protected:
	/**	@var		char *filename
	 *	@brief	The name of the DLL module
	 */
	std::string filename;

	/**	@var		char *path
	 *	@brief	The path to the DLL Module 
	 */
	std::string path;

	/**	@var		char *about
	 *	@brief	To hold any details about the DLL module, it's version or author, etc
	 */
	std::string about;

public:
	/**	IModule Constructor */
	IModule(){};

	/**	IModule Deconstructor */
	virtual ~IModule(){};
	
	/**	Loads a DLL module
	 *
	 *	@param filename	The name of the file containing the DLL Module
	 *	
	 *	@returns	A pointer to the platform specific handle
	 */
	virtual void * Load(std::string filename) = 0;

	/**	Retrieves the platform specific handle of the DLL module 
	 *
	 *	@returns	A pointer to the platform specific handle
	 */
	virtual void * GetHandle(void) = 0;

	/**	Retrieves the name of the DLL modules file 
	 *
	 *	@returns	A string containing the filename of the DLL Module
	 */
	virtual std::string GetFilename(void) = 0;

	/**	Retrieves the path of the DLL module 
	 *
	 *	@returns	A string containing the path of the DLL module
	 */
	virtual std::string GetPath(void) = 0;

	/**	Retrieves the about information for the DLL module 
	 *
	 *	@returns	A string containing the about information
	 */
	virtual std::string GetAbout(void) = 0;	
};

#endif // #ifndef _IMODULE_H_

