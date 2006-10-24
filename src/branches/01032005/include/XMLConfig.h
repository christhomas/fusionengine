#ifndef _XMLCONFIG_H_
#define _XMLCONFIG_H_

#include <string>

class XMLConfig{
private:
	void *doc, *root;
public:
							XMLConfig			();
	virtual					~XMLConfig			();
	
	virtual	bool			xmlLoadConfig		(std::string xml);
	virtual	void			xmlCloseConfig		(void);
	virtual bool			xmlIsOpen			(void);
	
	virtual void *			xmlGetRootNode		(void);
	virtual unsigned int	xmlCountNodes		(void *node);
	virtual void *			xmlFindNode			(void *node, std::string search);
	virtual void *			xmlGetChild			(void *node, unsigned int num);
	virtual std::string		xmlGetNodeName		(void *node);
	virtual std::string		xmlGetNodeProperty	(void *node, std::string prop);
	
	virtual	std::string		xmlGetError			(void);
};

#endif // #ifndef _XMLCONFIG_H_