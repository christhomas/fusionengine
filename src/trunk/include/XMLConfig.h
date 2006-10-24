/***************************************************************************
	                      XMLConfig.h  -  description
	                         -------------------
	begin                : Thursday 6th April 2006
	copyright            : (C) 2006 by Chris Thomas
	email                : chris.alex.thomas@gmail.com
	
	description:
	
	A XML Configuration class which loads a XML formatted configuration file
	from disk and extracts information from it

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

#ifndef _XMLCONFIG_H_
#define _XMLCONFIG_H_

#include <string>

class XMLConfig{
private:
	void *doc, *root;
public:
				XMLConfig		();
	virtual		~XMLConfig		();
	
	virtual bool		xmlLoadConfig		(std::string xml);
	virtual void		xmlCloseConfig	(void);
	virtual bool		xmlIsOpen		(void);
	
	virtual void *	xmlGetRootNode	(void);
	virtual unsigned int	xmlCountNodes		(void *node);
	virtual void *	xmlFindNode		(void *node, std::string search);
	virtual void *	xmlGetChild		(void *node, unsigned int num);
	virtual std::string	xmlGetNodeName	(void *node);
	virtual std::string	xmlGetNodeProperty	(void *node, std::string prop);
	
	virtual std::string	xmlGetError		(void);
};

#endif // #ifndef _XMLCONFIG_H_
