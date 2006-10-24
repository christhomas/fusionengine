#include <VirtualFS/VFSPlugin_BIN.h>

/**	Function to build all the appropiate plugins associated with this file type
 *	@ingroup VFSPlugin_BIN_Group
 *
 *	@param	f	A pointer to the Fusion Core object, so if required, access to Fusion resources is possible
 *
 *	@returns A pointer to the plugin created by calling this method
 *
 *	It's worth noting that some File formats will require multiple filters to be applied to the data it holds.
 *	A format could be a simple text file, but it could be encrypted, or compressed, or both
 *	So filters to decrypt, decompress might be required to read the contents of the file correctly.
 */
VFSPlugin * CreateBinaryPlugin(Fusion *f)
{
	static int counter=0;

	if(counter == 0){ counter++; return new VFSPlugin_BIN();	}

	return NULL;
}

/**	Binary file format Plugin Constructor 
 *
 *	Sets the default extension to bin, although this 
 *	format will happily accomodate ANY file format as a pure bytestream
 *
 *	@todo	Should a plugin to handle binary files, which are "unformatted" 
 *				or not decoded, for example, reading a file from an archive and 
 *				writing to the disk, you want to treat as anonymous binary data, 
 *				should a file of this type, have an extension? sounds silly to me
 *				surely the data could have any extension and there shouldnt be an
 *				extension set aside for it
 */
VFSPlugin_BIN::VFSPlugin_BIN()
{
	m_type			=	"binary";
	m_offset		=	0;
	m_length		=	0;
	m_fileinfo	=	NULL;
	m_buffer		=	NULL;
}

/**	Binary file format plugin Deconstructor */
VFSPlugin_BIN::~VFSPlugin_BIN(){}

/**	Adds a Data filter
 *
 *	@param filter	The filter to add to the plugin
 *
 */
void VFSPlugin_BIN::AddFilter(VFSFilter *filter)
{
	if(filter != NULL){
		m_filters.push_back(filter);
	}
}

/**	Retrieves a plugin Identifier
 *
 *	@returns The plugin Identifier string
 */
char * VFSPlugin_BIN::Type(void)
{
	return m_type;
}

/**	Decodes the file contents into a structured format
 *
 *	@param buffer		A bytestream containing the files contents
 *	@param length		The length of the bytestream
 *
 *	@returns A FileInfo structure containing the file's contents
 */
FileInfo * VFSPlugin_BIN::Read(unsigned char *buffer, unsigned int length)
{
	for(int a=0;a<m_filters.size();a++){
		buffer = m_filters[a]->Decode(buffer,length);
	}

	m_fileinfo							=	new BinaryFileInfo();
	m_fileinfo->filelength	=	length;
	m_fileinfo->data				=	new unsigned char[length];

	memcpy(m_fileinfo->data,buffer,length);
	
	return m_fileinfo;
}

/**	Writes the contents of a FileInfo structure to a bytestream
 *
 *	@param data		A FileInfo structure containing the data to write to the bytestream
 *	@param length	The length of the bytestream created
 *
 *	@returns	A Bytestream containing the data to write to the file
 */
char * VFSPlugin_BIN::Write(FileInfo *data, unsigned int &length)
{
	//	Not Implemented
	length = 0;
	return NULL;
}