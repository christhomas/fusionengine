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