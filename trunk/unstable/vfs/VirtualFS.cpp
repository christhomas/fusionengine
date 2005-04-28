#include <vfs/VirtualFS.h>
#include <Fusion.h>

//	All the built in Transport types
#include <VFSHandle_file.h>

//	All the built in Plugin types
#include <VFSPlugin_BIN.h>
#include <VFSPlugin_TXT.h>

Fusion *fusion;

/**	Method to create the VirtualFS object
 *
 *	This function is required for the application to create a VFS object from a library
 */
void CreateVFS(Fusion &f)
{
	fusion = &f;
	fusion->vfs = new VirtualFS();
}

void DestroyVFS(void)
{
	delete fusion->vfs;
	fusion->vfs = NULL;
}

/**	VirtualFS constructor
 *
 *	Sets the subsystems name and file it was loaded from (The filename of the DLL module)
 *	Obtains a ptr to Fusions Module Database
 */
VirtualFS::VirtualFS()
{
	object_name		= "VirtualFS Version 2.0 <chris_thomas@hotmail.com>";
	object_filename	= "libVFS.dll";

	m_moduledb	=	fusion->GetModuleDB();

	//	Add all the built in formats, pure text and pure binary are defaults
	AddPlugin(CreateTextPlugin);
	AddPlugin(CreateBinaryPlugin);
	
	//	Setup all the built in transports
	AddTransport(CreateFileTransport);
}

/**	VirtualFS Deconstructor
 *
 *	Deletes all the available Transports, plugins and filters
 */
VirtualFS::~VirtualFS()
{
	unsigned int a;

	//	Delete all the m_transport
	for(a=0;a<m_transport.size();a++)	m_transport[a]->CloseAll();
	for(a=0;a<m_transport.size();a++)	delete m_transport[a];
	m_transport.clear();

	//	Delete all the file format plugins
	for(a=0;a<m_plugin.size();a++)	delete m_plugin[a];
	m_plugin.clear();

	//	Delete all the data filters
	for(a=0;a<m_filter.size();a++)	delete m_filter[a];
	m_filter.clear();

	SetTempDirectory();
}

/**	TODO: Look into deprecating this method */
bool VirtualFS::Initialise(void)
{
	return true;
}

bool VirtualFS::UnloadModules(void)
{
/*	
	loop through all the plugins which are dll based
	how do you know which plugins are loaded from dll's
	and which are not?

	I have to store the dll information, because a dll could create more than one object
	if it had dependencies

	information to store

	module filename
	create pointer
	destroy pointer

*/
	return false;	
}

void VirtualFS::SetTempDirectory(std::string directory)
{
	VFSHandle *h = OpenLocation("file://");

	if(m_tempdir.empty() == true){
		h->DeleteDir(m_tempdir,true);
		m_tempdir.clear();
	}

	if(directory.empty() == true){
		m_tempdir = directory;

		h->CreateDir(m_tempdir);
	}
}

std::string VirtualFS::GetTempDirectory(void)
{
	return m_tempdir;
}

/** Loads a configuration file
 *
 *	This method uses the Fusion XMLConfig base object to load the VirtualFS configuration
 *	This means, the VirtualFS config information is combined into the same xml as the rest
 *	of the fusion config, so we just reuse that object
 */
void VirtualFS::LoadConfig(void)
{
	LoadConfig((XMLConfig *)fusion);
}

/**	Loads a configuration file
 *
 *	@param config The XMLConfig object responsible for reading the configuration out of the file, into the program
 *
 *	Uses the XMLConfig object to read properties of the XML config file it has open
 *	using that information, to load transports, plugins and filters from DLL files
 *	stored on the disk
 */
void VirtualFS::LoadConfig(XMLConfig *config)
{
	VFSTransport::transport_t	t;
	VFSPlugin::plugin_t			p;
	VFSFilter::filter_t			f;		
	
	if(config->xmlIsOpen() == true){
		void *root = config->xmlGetRootNode();
		void *vnode = config->xmlFindNode(root,"vfs");
		
		if(vnode != NULL){
			unsigned int numNodes = config->xmlCountNodes(vnode);
			
			for(unsigned int a=0;a<numNodes;a++){
				void *cnode = config->xmlGetChild(vnode,a);
				std::string name,data;
				
				if(config->xmlGetNodeName(cnode) == "plugin"){
					name = config->xmlGetNodeProperty(cnode,"filetype");
					data = config->xmlGetNodeProperty(cnode,"module");
					
					if(name.empty() == false && data.empty() == false){
						//	Get the function ptr to create the filters

						t = (VFSTransport::transport_t)	m_moduledb->GetFunction(data,"CreateTransport");					
						p = (VFSPlugin::plugin_t)		m_moduledb->GetFunction(data,"CreatePlugin");
						f = (VFSFilter::filter_t)		m_moduledb->GetFunction(data,"CreateFilter");

						LoadPlugin(t,p,f);					
					}
				}
			}
		}
	}
}

void VirtualFS::LoadPlugin(VFSTransport::transport_t t, VFSPlugin::plugin_t p, VFSFilter::filter_t f)
{
	AddTransport(t);
	AddPlugin(p);
	AddFilter(f);
}

/**	Adds a File Format plugin to the VFS
 *
 *	@param CreatePlugin	The function which creates the plugins
 *
 *	Each DLL module can contain multiple plugins, so test whether 
 *	the module HAS any plugins, if so, request a plugin and add 
 *	it to the list until the reply is NULL, signifying no more 
 *	plugins to be created
 */
void VirtualFS::AddPlugin(VFSPlugin::plugin_t CreatePlugin)
{
	VFSPlugin *p = NULL;

	//	Add all the plugins
	if(CreatePlugin!=NULL){
		if((p = CreatePlugin(fusion))!=NULL){
			for(unsigned int a=0;a<m_plugin.size();a++){
				if(m_plugin[a]->Type() == p->Type()){
					delete p;
					p = NULL;
					break;
				}
			}
		}

		if(p!=NULL) m_plugin.push_back(p);
	}
}

/**	Adds a Data filter to the VFS
 *
 *	@param CreateFilter	The function which creates the filters
 *
 *	Each DLL module can contain multiple filters, so test whether
 *	the modules HAS any filters, if so, request a filter and add it
 *	to the list until the reply is NULL, signifying no more plugins
 *	to be created
 */
void VirtualFS::AddFilter(VFSFilter::filter_t CreateFilter)
{
	VFSFilter *f = NULL;

	//	Add all the filters
	if(CreateFilter!=NULL){
		if((f = CreateFilter(fusion))!=NULL){
			for(unsigned int a=0;a<m_filter.size();a++){
				if(m_filter[a]->Type() == f->Type()){
					delete f;
					f = NULL;
					break;
				}
			}

			if(f!=NULL) m_filter.push_back(f);
		}
	}
}

/**	Adds a Transport method to the VFS
 *
 *	@param CreateTransport	The function which creates the transport objects
 *
 *	Each DLL module can contain multiple transports, if required, so test whether
 *	the module HAS any transports, if so, request a transport and add it
 *	to the list until the reply is NULL, signifying no more transports
 *	to be created
 */
void VirtualFS::AddTransport(VFSTransport::transport_t CreateTransport)
{
	VFSTransport *t = NULL;

	//	Add all the transports
	if(CreateTransport!=NULL){
		if((t = CreateTransport(fusion))!=NULL){

			for(unsigned int a=0;a<m_transport.size();a++){
				if(m_transport[a]->GetIdentifier() == t->GetIdentifier()){
					delete t;
					t = NULL;
					break;
				}
			}

			if(t!=NULL) m_transport.push_back(t);
		}
	}
}

/**	EXtracts a file extension from a given filename
 *
 *	@param	filename	The filename in which to find the extension
 *
 *	@returns A string containing the extension found in the filename
 *
 *	This method is safe to use with Archival file systems, when you open a file like so:
 *
 *	"zipfs://myArchive.zip#/Sounds/Music/Main.xm"
 *
 *	The Transport identifier is:				"zipfs://"
 *	The filesystem you want to open is:	"myArchive.zip"
 *	The file you want to open is:				"/Sounds/Music/Main.xm"
 *
 *	So: "zipfs://" is the transport to open a filesystem, "myArchive.zip" is the filesystem containing the file "/Sounds/Music/Main.xm"
 *
 *	Operation:
 *		-#	Make a duplicate string to search through
 *		-#	Find the tokens in the string, delimiter "."
 *		-#	Continue to find find new tokens until the string is fully processed
 *		-#	If a delimiter is found, update the extension with the new token
 *		-#	The last token to be found, will be the last extension in the file name
 *		-#	This is the extension of the file you want to open, regardless of the 
 *				extension perhaps assigned to a Archive file system (e.g myArchive.zip) 
 *		-#	Delete the duplicated string
 */
std::string VirtualFS::FindExtension(std::string filename)
{
	std::string ext;
	if(filename.find(".") != std::string::npos){
		ext = filename.substr(filename.find(".")+1);
	}
	
	return ext;
}

/**	Extracts the filesystem identifier from the filename
 *
 *	@param filename A string containing the full filename requested
 *
 *	@returns A Transport object associated with the filesystem requested.  NULL is returned, if not found
 *
 *	Operation:
 *		-#	Loop through each Available transport
 *		-#	Get the transport identifier and compare the transport identifier against the filename
 *		-#	Return the found transport
 *		-#	If no transport is found, file:// is returned, if this works, great, otherwise, it'd have failed anyway
 *		-#	If file:// can't be found either, deep trouble, return NULL, duck and cover!
 */
VFSTransport * VirtualFS::FindTransport(std::string filename)
{
	VFSTransport *t = NULL;

	//	Look for a network transport
	for(unsigned int a=0;a<m_transport.size();a++){
		t = m_transport[a];
		if(t->GetCategory() == VFSTransport::NETWORK){
			if(filename.find(t->GetIdentifier()) != std::string::npos){
				return t;
			}
		}
	}

	//	look for a archive transport
	if(filename.find("#") != std::string::npos){
		std::string ext;
		
		ext = filename.substr(0,filename.find("#"));
	
		if(ext.find(".") != std::string::npos){
			ext = ext.substr(ext.find(".")+1);

			for(unsigned int a=0;a<m_transport.size();a++){
				t = m_transport[a];

				if(t->GetCategory() == VFSTransport::ARCHIVE){
					if(ext.find(t->GetIdentifier()) != std::string::npos){
						return t;
					}
				}
			}
		}
	}
	
	//	localfs transport
	for(unsigned int a=0;a<m_transport.size();a++){
		t = m_transport[a];

		if(t->GetCategory() == VFSTransport::LOCALFS){
			if(filename.find(t->GetIdentifier()) != std::string::npos){
				return t;
			}
		}
	}

	//	you can't find a transport, lets go for broke and try file:// if it doesnt work, then it doesnt work.
	for(a=0;a<m_transport.size();a++){
		t = m_transport[a];
		
		if(t->GetIdentifier() == "file://") return t;
	}

	// holy crap, you can't even find the default transport!!!
	return NULL;
}

/**	Uses the extension of the file, to find an appropiate Plugin to decode the file
 *
 *	@param extension	A String containing the extension for which to find a plugin
 *
 *	@returns A Plugin object to encode/decode the type of file being opened.  NULL is returned, if not found
 *
 *	Operation:
 *		-#	Loop through all the available plugins
 *		-#	Get the plugin identifier
 *		-#	Loop through the plugin identifier string extracting each extension
 *		-#	Compare each extension found to the extension requested
 *		-#	If a match is found, return a pointer to the correct plugin
 *		-#	Otherwise, NULL is returned
 */
VFSPlugin * VirtualFS::FindPlugin(std::string extension)
{
	//	Loop through all the plugins
	for(unsigned int a=0;a<m_plugin.size();a++){
		VFSPlugin *p = m_plugin[a];
		std::string type = p->Type();
		
		std::string token;

		for(size_t pos = type.find(";");pos != std::string::npos;pos = type.find(";")){
			token = type.substr(0,pos);

			if(token == extension)	return m_plugin[a];

			type = type.substr(pos+1);
		}
	}

	return FindPlugin("binary");
}

/**	Finds a filter matching the requested type
 *
 *	@param type	A String containing the type of filter requested
 *
 *	@returns A Filter object to encode/decode the data.  NULL is returned, if not found
 *
 *	Operation:
 *		-#	Loop throgh all the available filters
 *		-#	Compare the filter identifier to the type requested
 *		-#	If a match is found, return a pointer to this filter
 *		-#	Otherwise, NULL is returned
 */
VFSFilter * VirtualFS::FindFilter(std::string type)
{
	for(unsigned int a=0;a<m_filter.size();a++){
		VFSFilter *f = m_filter[a];
		
		if(type == f->Type()){
			return m_filter[a];
		}
	}

	return NULL;
}

/**	Opens a file
 *
 *	@param filename	A String containing the filename requested to open
 *	@param create		Whether the file should be created if not found
 *
 *	@returns A Handle to the file opened.  NULL is returned, if not found
 *
 *	This method requires the filename to contain the extension for the
 *	plugin required to open the file, if the file has no extension, 
 *	use VirtualFS::Open(char *, char *) to open the file, forcing an 
 *	extension to be used
 *
 *	NOTE:	If no extension is appended to the filename, this method will crash upon attempting the read the file
 */
VFSHandle * VirtualFS::Open(std::string filename, bool create)
{
	std::string ext = FindExtension(filename);

	VFSHandle *h = Open(filename,ext,create);

	return h;
}

/**	Opens a file, overriding the filename extension with the one supplied
 *
 *	@param filename	A String containing the file to open
 *	@param ext			A String containing the extension of the file, which overrides the filename extension
 *	@param create		Whether the file should be created if not found
 *
 *	@returns A Handle to the file opened.  NULL is returned if not found
 *
 *	NOTE:	The extension string supplied here will be deleted by this method, 
 *				be sure this will not crash your application, sources of bugs in 
 *				this method are related to this operation
 *
 *	Operation:
 *		-#	Finds a plugin associated with the extension supplied
 *		-#	Finds a transport associated with the filesystem requested in the filename
 *		-#	If the transport was found, attempt to use that transport to open the file, passing the filename and plugin to that transport
 *		-#	delete the extension supplied
 *		-#	Return the handle of the file
 */
VFSHandle * VirtualFS::Open(std::string filename, std::string ext, bool create)
{
	VFSTransport	*t = FindTransport(filename);
	VFSPlugin		*p = FindPlugin(ext);
	VFSHandle		*h = NULL;

	if(t != NULL && p != NULL)	h = t->Open(filename,p,create);

	return h;
}

VFSHandle * VirtualFS::OpenLocation(std::string loc, bool create)
{
	//	identify what transport to use by analysing two things
	//	the location extension, if any
	//	the location prefix, if any
	//	if there is no extension, consider a directory, if there is no prefix, default to file://

	VFSTransport	*t = FindTransport(loc);

	if(t != NULL)	return t->OpenLocation(loc,create);

	return NULL;
}

/**	Closes an open file
 *
 *	@param handle	A Handle to the open file
 *
 *	@returns boolean true or false, depending on whether the file was closed successfully.
 *
 *	NOTE:	if the file wasnt open in the first place, false will be returned to signify such
 *
 *	Operation:
 *		-#	Loops through all the available transports
 *		-#	Loops through each handle the transport has open
 *		-#	Compares each handle in the transport to the handle passed to the method
 *		-#	If a match is found, that transport is asked to close the file
 *		-#	The VFSTransport::Close(VFSHandle *) method is used to return whether the file was closed successfully
 *		-#	If the handle was not found in ANY of the transports, false is returned
 */
bool VirtualFS::Close(VFSHandle *handle)
{
	for(unsigned int a=0;a<m_transport.size();a++){
		for(int b=0;b<m_transport[a]->NumberFiles();b++){
			if(handle == m_transport[a]->GetHandle(b)){
				return m_transport[a]->Close(handle);
			}
		}
	}

	return false;
}


