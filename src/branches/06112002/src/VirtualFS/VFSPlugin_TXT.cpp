#include <VirtualFS/VFSPlugin_TXT.h>

/**	Function to build all the appropiate plugins associated with this file type
 *	@ingroup VFSPlugin_TXT_Group
 *
 *	@param	f	A pointer to the Fusion Core object, so if required, access to Fusion resources is possible
 *
 *	@returns A pointer to the plugin created by calling this method
 *
 *	It's worth noting that some File formats will require multiple filters to be applied to the data it holds.
 *	A format could be a simple text file, but it could be encrypted, or compressed, or both
 *	So filters to decrypt, decompress might be required to read the contents of the file correctly.
 */
VFSPlugin * CreateTextPlugin(Fusion *f)
{
	static int counter = 0;

	if(counter == 0){ counter++; return new VFSPlugin_TXT();	}

	return NULL;
}

/**	Text file format Plugin	Constructor	*/
VFSPlugin_TXT::VFSPlugin_TXT()
{
	m_type			=	"txt;text";
	m_offset		=	0;
	m_length		=	0;
	m_fileinfo	=	NULL;
	m_buffer		=	NULL;
}

/**	Text file format Plugin Deconstructor */
VFSPlugin_TXT::~VFSPlugin_TXT(){}

/**	Adds a Data filter
 *
 *	@param filter	The filter to add to the plugin
 *
 */
void VFSPlugin_TXT::AddFilter(VFSFilter *filter)
{
	if(filter != NULL)	m_filters.push_back(filter);
}

/**	Retrieves a plugin Identifier
 *
 *	@returns The plugin Identifier string
 */
char * VFSPlugin_TXT::Type(void)
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
FileInfo * VFSPlugin_TXT::Read(unsigned char *buffer, unsigned int length)
{
	int		offset	=	0;

	for(int a=0;a<m_filters.size();a++){
		buffer = m_filters[a]->Decode(buffer,length);
	}
	
	while(offset < length){
		//	Chop the data into lines, store them in the file information block
		char *token = strtok((char *)&buffer[offset],"\n\0");
		offset+=strlen(token)+1;

		char *copy = new char[strlen(token)+1];
		strcpy(copy,token);

		m_fileinfo = new TextFileInfo();
		m_fileinfo->lines.push_back(copy);
	}

	delete[] buffer;
	
	return m_fileinfo;
}

/**	Writes the contents of a FileInfo structure to a bytestream
 *
 *	@param data		A FileInfo structure containing the data to write to the bytestream
 *	@param length	The length of the bytestream created
 *
 *	@returns	A Bytestream containing the data to write to the file
 */
char * VFSPlugin_TXT::Write(FileInfo *data, unsigned int &length)
{
	//	Not Implemented
	length = 0;
	return NULL;
}