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
	f.vfs = new VirtualFS();
	fusion = &f;
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

	m_tempdir		=	NULL;

	//	Add all the built in formats, pure text and pure binary are defaults
	AddPlugin(CreateTextPlugin);
	AddPlugin(CreateBinaryPlugin);
	
	//	Setup all the built in transports
	AddTransport(CreateFileTransport);
}

/**	Virtual FS Deconstructor
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

void VirtualFS::SetTempDirectory(char *directory)
{
	VFSHandle *h = OpenLocation("file://");

	if(m_tempdir != NULL){
		h->DeleteDir(m_tempdir,true);
		delete[] m_tempdir;
	}

	if(directory != NULL){
		m_tempdir = new char[strlen(directory)+1];
		strcpy(m_tempdir,directory);

		h->CreateDir(m_tempdir);
	}
}

char * VirtualFS::GetTempDirectory(void)
{
	return m_tempdir;
}

/**	Loads a configuration file
 *
 *	@param configfile The filename of the configuration file
 *
 *	This method will search through the config file, if it parses [VFS]
 *	it'll happily start extracting data from that section, until the next 
 *	section is detected (which, incidentally starts with "[").  Upon reading 
 *	a valid command, the data section will be removed (part of the string 
 *	after the '=' in each valid command line) and compared against what 
 *	VirtualFS is programmed to cope with, upon getting a correct comparison 
 *	from the command, the data section will be tagged into the appropiate 
 *	place in the internal configuration data, to be used later
 */
void VirtualFS::LoadConfig(char *configfile)
{
	bool							begin				=	false;
	const int					buffersize	=	2048;
	char							buffer[buffersize];
	std::ifstream			config(configfile);

	//	Read the config file and look for vfs plugins
	while(config.eof()==0){
		config.getline(buffer,buffersize);

		//	Start reading the config file only when you reach the VFS section
		if(strncmp(buffer,"[VFS]",5) == 0){
			begin = true;
			continue;
		}

		//	End reading when you read the next section (each section starts like [SECTION], [ANOTHERSECTION]
		if( (begin == true) && (strncmp(buffer,"[",1) == 0) ){
			break;
		}

		//	Ignore any blank lines
		if(strcmp(buffer,"\0") == 0){
			continue;
		}
		
		//	If begun, process config data
		if(begin==true){
			char *type		= strtok(buffer,"=");
			char *module	= strtok(&buffer[strlen(type)+1],"\n");

			//	Get the function ptr to create the filters
			VFSTransport::transport_t	t = (VFSTransport::transport_t)	m_moduledb->GetFunction(module,"CreateTransport");
			VFSPlugin::plugin_t			p = (VFSPlugin::plugin_t)		m_moduledb->GetFunction(module,"CreatePlugin");
			VFSFilter::filter_t			f = (VFSFilter::filter_t)		m_moduledb->GetFunction(module,"CreateFilter");

			LoadPlugin(t,p,f);
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
		do{	
			if((p = CreatePlugin(fusion))!=NULL){
				for(unsigned int a=0;a<m_plugin.size();a++){
					if(strcmp(m_plugin[a]->Type(),p->Type()) == 0){
						delete p;
						p = NULL;
						a = (unsigned int)m_plugin.size();
					}
				}
			}

			if(p!=NULL) m_plugin.push_back(p);
		}while(p!=NULL);
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
		do{
			if((f = CreateFilter(fusion))!=NULL){
				for(unsigned int a=0;a<m_filter.size();a++){
					if(strcmp(m_filter[a]->Type(),f->Type()) == 0){
						delete f;
						f = NULL;
						a = (unsigned int)m_filter.size();
					}
				}

				if(f!=NULL) m_filter.push_back(f);
			}
		}while(f!=NULL);
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
		do{
			if((t = CreateTransport(fusion))!=NULL){

				for(unsigned int a=0;a<m_transport.size();a++){
					if(strcmp(m_transport[a]->GetIdentifier(),t->GetIdentifier()) == 0){
						delete t;
						t = NULL;
						a = (unsigned int)m_transport.size();
					}
				}

				if(t!=NULL) m_transport.push_back(t);
			}
		}while(t!=NULL);
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
char * VirtualFS::FindExtension(char *filename)
{
	char *token = strrchr(filename,'.');
	char *ext		=	NULL;

	if(token != NULL){
		token++;
		ext =	new char[strlen(token)+1];
		strcpy(ext,token);
		return ext;
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
VFSTransport * VirtualFS::FindTransport(char *filename)
{
	VFSTransport *t = NULL;
	unsigned int a,length;

	//	Look for a network transport
	for(a=0;a<m_transport.size();a++){
		t = m_transport[a];
		length = (unsigned int)strlen(t->GetIdentifier());

		if(t->GetCategory() == VFSTransport::NETWORK){
			if(strncmp(filename,t->GetIdentifier(),length) == 0){
				return t;
			}
		}
	}

	//	look for a archive transport
	char *ext		=	filename;
	char *copy	=	NULL;

	if(strchr(ext,'#') != NULL){
		copy = new char[strlen(filename)+1];
		strcpy(copy,filename);

		ext = strtok(copy,"#");
	}
	
	ext = strrchr(ext,'.');

	if(ext != NULL){
		ext++;
		for(a=0;a<m_transport.size();a++){
			t = m_transport[a];
			length = (unsigned int)strlen(t->GetIdentifier());

			if(t->GetCategory() == VFSTransport::ARCHIVE){
				if(strncmp(ext,t->GetIdentifier(),length) == 0){
					delete copy;
					return t;
				}
			}
		}
		delete copy;
	}

	//	localfs transport
	for(a=0;a<m_transport.size();a++){
		t = m_transport[a];
		length = (unsigned int)strlen(t->GetIdentifier());

		if(t->GetCategory() == VFSTransport::LOCALFS){
			if(strncmp(filename,t->GetIdentifier(),length) == 0){
				return t;
			}
		}
	}

	//	you can't find a transport, lets go for broke and try file:// if it doesnt work, then it doesnt work.
	for(a=0;a<m_transport.size();a++){
		t = m_transport[a];
		if(strcmp(t->GetIdentifier(),"file://") == 0){
			return t;
		}
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
VFSPlugin * VirtualFS::FindPlugin(char *extension)
{
	//	Loop through all the plugins
	for(unsigned int a=0;a<m_plugin.size();a++){
		char *type = new char[strlen(m_plugin[a]->Type())+1];
		strcpy(type,m_plugin[a]->Type());

		char *token = strtok(type,";");

		while(token != NULL){
			if(strcmp(extension,token) == 0){
				delete[] type;
				return m_plugin[a];
			}

			token = strtok(NULL,";");
		}

		delete[] type;
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
VFSFilter * VirtualFS::FindFilter(char *type)
{
	for(unsigned int a=0;a<m_filter.size();a++){
		if(strcmp(type,m_filter[a]->Type()) == 0){
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
VFSHandle * VirtualFS::Open(char *filename, bool create)
{
	char *ext = FindExtension(filename);

	VFSHandle *h = Open(filename,ext,create);

	delete[] ext;

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
VFSHandle * VirtualFS::Open(char *filename, char *ext, bool create)
{
	VFSTransport	*t = FindTransport(filename);
	VFSPlugin			*p = FindPlugin(ext);
	VFSHandle			*h = NULL;

	if(t != NULL && p != NULL)	h = t->Open(filename,p,create);

	return h;
}

VFSHandle * VirtualFS::OpenLocation(char *loc, bool create)
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


