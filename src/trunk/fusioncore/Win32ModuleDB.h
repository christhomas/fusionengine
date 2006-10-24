/***************************************************************************
	                      Win32ModuleDB.h  -  description
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

#ifndef _WIN32MODULEDB_H_
	#define _WIN32MODULEDB_H_

#include <IModuleDB.h>

/** @ingroup	Win32_ModuleDB_Group
 *	@brief		Derived IModuleDB class for the Win32 Platform
 */
class Win32ModuleDB: public IModuleDB{
protected:
	virtual IModule *	GetModule		(std::string name);
	virtual void		AddModule		(IModule *module);
public:
						Win32ModuleDB	();
	virtual				~Win32ModuleDB	();
	virtual void		AddPath			(std::string path);
	virtual IModule *	LoadModule		(std::string name);
	virtual bool		UnloadModule	(std::string name);
	virtual void		UnloadAll		(void);
	virtual void *		GetFunction		(std::string name,std::string func);
};

#endif // #ifndef _WIN32MODULEDB_H_

