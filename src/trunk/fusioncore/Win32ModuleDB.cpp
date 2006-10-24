/***************************************************************************
	                      Win32ModuleDB.cpp  -  description
	                         -------------------
	begin                : Thursday 6th April 2006
	copyright            : (C) 2006 by Chris Thomas
	email                : chris.alex.thomas@gmail.com
	
	description:
	
	A Win32 specific Module database class which constructs module classes 
	to load a particular dll/so from the disk into memory so the module 
	system can operate upon it


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

#include "Win32ModuleDB.h"
#include "Win32Module.h"

#include <windows.h>

/**	Win32 ModuleDB Constructor
 *
 */
Win32ModuleDB::Win32ModuleDB(){}

/**	Win32 ModuleDB Deconstructor
 *
 *	Operation:
 *		-#	Deletes all DLL Modules and unloads them
 *		-#	Deletes all search paths
 */
Win32ModuleDB::~Win32ModuleDB()
{
	unsigned int a;
	for(a=0;a<m_module.size();a++) delete m_module[a];
	
	m_module.clear();
	m_searchpath.clear();
}

/**	Retrieves a DLL Module
 *
 *	@param name	The filename of the DLL Module being searched for
 *	
 *	@returns	A IModule pointer or NULL is the module is not found
 */
IModule * Win32ModuleDB::GetModule(std::string name)
{
	for(unsigned int a=0;a<m_module.size();a++){
		IModule *m = m_module[a];
		if(m->GetFilename() == name) return m;
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
void Win32ModuleDB::AddPath(std::string path)
{
	m_searchpath.push_back(path);
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
void Win32ModuleDB::AddModule(IModule *module)
{
	m_module.push_back(module);
}

/**	Loads a DLL Module
 *
 *	@param	name	The filename of the Module to load
 *
 *	@returns	A IModule pointer or NULL if not found
 *
 *	Operation:
 *		-#	Test whether the filename is valid
 *		-#	Attempt to retrieve a pointer to that module, test whether it's already loaded or not
 *		-#	If not loaded, loop through all the searchpaths, attempting to load the DLL Module at each path
 *		-#	Allocate a new Win32Module object
 *		-#	Create a absolute path to the DLL Module based on the current searchpath and the DLL Module filename
 *		-#	Attempt to load the DLL Module
 *		-#	If the attempt was successful, add the module to the Database and clear up, then return
 *		-#	clear up all temporary memory and try the next searchpath
 */
IModule * Win32ModuleDB::LoadModule(std::string name){

	Win32Module *m = NULL;

	if(name.empty() == false){
		m = (Win32Module *)GetModule(name);
			
		if(m==NULL){
			for(unsigned int a=0;a<m_searchpath.size();a++){
				std::string path = m_searchpath[a];

				m = new Win32Module(name,path,"");
				
				std::string filename = path + "/" + name;

				if(m->Load(filename) != NULL){
					AddModule(m);
					break;
				}
				
				delete m;
				m = NULL;
			}
		}
	}
	
	return m;
}

/**	Unloads a DLL module
 *
 *	@param name	The filename of the DLL Module to unload
 *
 *	@returns	Boolean true or false, depending on whether the DLL Module successfully unloaded or not
 *
 *	Operation:
 *		-#	Loop through the loaded modules, testing the filename against the one being searched for
 *		-#	If match is found, attempt to remove that module from the database
 *		-#	Create a new temporary module array, with one less element
 *		-#	Loop through all the loaded DLL modules, if the module being copied is the one being deleted, skip it
 *		-#	Copy each module pointer for all those that are not being removed
 *		-#	Delete the module being unloaded
 *		-#	Delete the module array
 *		-#	Reassign the module array pointer to the temporary array
 *		-#	Reduce the number of modules in the database
 */
bool Win32ModuleDB::UnloadModule(std::string name)
{
	for(unsigned int a=0;a<m_module.size();a++){
		if(m_module[a]->GetFilename() == name){
			m_module.erase(m_module.begin()+a);

			return true;
		}
	}

	return false;
}

/**	Unloads all the DLL modules from the database
 *	
 *	Operation:
 *		-#	Constantly requests to unload the first DLL module in the array
 *		-#	When the array is empty, then all the DLL Modules are released
 */
void Win32ModuleDB::UnloadAll(void)
{
	while(m_module.size() > 0){
		UnloadModule(m_module[0]->GetFilename());
	}
}

/**	Retrieve a function pointer from a DLL Module 
 *
 *	@param	name	The filename of the DLL Module
 *	@param	func	The name of the function requested
 *
 *	@returns	A Function pointer to the correct DLL function or NULL if not found
 *
 *	Operation:
 *		-#	Attempt to load the DLL Module
 *		-#	If successful, Attempt to retrieve a function pointer from that module
 */
void * Win32ModuleDB::GetFunction(std::string name, std::string func)
{
	Win32Module *m;
	
	if((m = (Win32Module *)LoadModule(name.c_str())) == NULL) return NULL;

	return (void *)GetProcAddress((HINSTANCE)m->GetHandle(),TEXT(func.c_str()));
}
