/***************************************************************************
	                      IModuleDB.h  -  description
	                         -------------------
	begin                : Thursday 6th April 2006
	copyright            : (C) 2006 by Chris Thomas
	email                : chris.alex.thomas@gmail.com
	
	description:
	
	Base class for the Module DB system, which is a series of objects which
	load dynamic shared objects (DLL/SO files) from the disk into memory
	and allows access to functions and methods that they export

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

#ifndef _IMODULEDB_H_
	#define _IMODULEDB_H_

#include <IModule.h>
#include <vector>
#include <string>

/** @ingroup	ModuleDB_Helper_Group
 *	@brief		The base class of the Module Database
 */
class IModuleDB{
protected:
	/**	@var		IModule **m_module
	 *	@brief	An array of DLL Modules
	 */
	std::vector<IModule *> m_module;

	/**	@var		char **m_searchpath
	 *	@brief	An array of search paths for loading DLL Modules
	 */
	std::vector<std::string> m_searchpath;

	/**	Retrieves a ptr to a particular DLL module 
	 *	
	 *	@param	name	The filename of the module
	 *
	 *	@returns A pointer to a module, or NULL, if module is not found
	 */
	virtual IModule * GetModule(std::string name) = 0;

	/**	Stores a DLL module
	 *
	 *	@param module	The module to store
	 */
	virtual void AddModule(IModule *module) = 0;
public:

	/**	IModuleDB Constructor */
	IModuleDB(){};

	/**	IModuleDB Deconstructor */
	virtual ~IModuleDB(){};

	/**	Adds a search path to the DB
	 *
	 *	@param	path	The path to add to the database
	 */
	virtual void AddPath(std::string path) = 0;

	/**	Loads a module
	 *
	 *	@param name	The filename of the DLL Module to load
	 *
	 *	@returns A pointer to the loaded DLL module
	 */
	virtual IModule * LoadModule(std::string name) = 0;

	/**	Unloads a module
	 *
	 *	@param name	The filename of the DLL module to unload
	 *
	 *	@returns Boolean true or false, depending on whether the module was successfully unloaded
	 */
	virtual bool UnloadModule(std::string name) = 0;

	/**	Unloads all the modules from the database
	 *
	 *	This is essentially used when cleaning up what modules fusion has open
	 */
	virtual void UnloadAll(void) = 0;

	/**	Retrieves a function pointer from the DLL Module
	 *
	 *	@param	name	The name of the DLL module to retrieve the function from
	 *	@param	func	The name of the function to retrieve
	 *	
	 *	@returns	A Function pointer, or NULL, if the module/function was not found
	 */
	virtual void * GetFunction(std::string name,std::string func) = 0;
};

#endif	//	#ifndef _IMODULEDB_H_

