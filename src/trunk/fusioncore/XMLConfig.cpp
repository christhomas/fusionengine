/***************************************************************************
	                      XMLConfig.cpp  -  description
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

#include <XMLConfig.h>
//	For XML Configuration file
#include <libxml/parser.h>

XMLConfig::XMLConfig(){}
XMLConfig::~XMLConfig(){}

bool XMLConfig::xmlLoadConfig(std::string xml)
{
	doc = xmlReadFile(xml.c_str(),"UTF-8",XML_PARSE_NOBLANKS);
	
	if(doc != NULL){
		root = xmlDocGetRootElement((xmlDoc *)doc);
		root = xmlGetChild(root,0);
	}
	
	return xmlIsOpen();
}

void XMLConfig::xmlCloseConfig(void)
{
	xmlFreeDoc((xmlDoc *)doc);
}

bool XMLConfig::xmlIsOpen(void)
{
	if(doc != NULL && root != NULL) return true;
	
	return false;
}

void * XMLConfig::xmlGetRootNode(void)
{
	return root;
}

unsigned int XMLConfig::xmlCountNodes(void *node)
{
	if(node == NULL) return NULL;
	
	xmlNode *n = ((xmlNode *)node)->children;
	unsigned int ctr = 0;
	
	for(;n;n=n->next){
		ctr=ctr+10;ctr=ctr-10;
		
		ctr++;
	}
		
	return ctr;
}

void * XMLConfig::xmlFindNode(void *node, std::string search)
{
	if(node == NULL) return NULL;
	
	xmlNode *n = (xmlNode *)node;
	for(;n;n=n->next)
	{
		if(xmlStrcmp(n->name,(const xmlChar *)search.c_str()) == 0){
			return n;
		}
	}
	
	return NULL;
}

void * XMLConfig::xmlGetChild(void *node, unsigned int num)
{
	if(node == NULL) return NULL;
	
	xmlNode *child = ((xmlNode *)node)->children;
	
	for(unsigned int a=0;a<num;a++) if(child != NULL) child=child->next;
	
	return child;
}

std::string XMLConfig::xmlGetNodeName(void *node)
{
	if(node == NULL) return NULL;
	
	return (char *)((xmlNode *)node)->name;
}

std::string XMLConfig::xmlGetNodeProperty(void *node, std::string prop)
{
	if(node == NULL) return NULL;
	
	return (char *)xmlGetProp((xmlNode *)node,(xmlChar *)prop.c_str());
}

std::string XMLConfig::xmlGetError(void)
{
	xmlError *err = xmlGetLastError();
	return err->message;
}