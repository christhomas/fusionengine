#ifndef _VFSPLUGIN_H_
	#define _VFSPLUGIN_H_

#include <vector>

#include <vfs/FileInfo.h>
#include <vfs/VFSFilter.h>

/**	@ingroup	VFSPlugin_Group
 *	@brief		The base class for all VFS Plugins
 */
class VFSPlugin{
protected:
	/**	@var		int m_offset
	 *	@brief	The offset into the data buffer
	 */
	int						m_offset;

	/**	@var		unsigned char *m_buffer
	 *	@brief	The data buffer
	 */
	unsigned char *m_buffer;

	/**	@var		int m_length
	 *	@brief	The length of the data buffer
	 */
	unsigned int	m_length;

	/**	@var		char *m_type
	 *	@brief	String identifing the File format plugin
	 */
	char *m_type;

	/**	@var		std::vector<VFSFilter *>	m_filters
	 *	@brief	Array of Filters attached to this plugin to filter the file data through
	 */
	std::vector<VFSFilter *> m_filters;
public:

	/**	@typedef plugin_t
	 *	Function pointer to create a plugin object
	 */
	typedef VFSPlugin *	(*plugin_t)(Fusion *f);
	
	/**	File format plugin Constructor
	 *
	 *	Abstract base class constructor
	 */
	VFSPlugin(){};

	/**	File format plugin Deconstructor
	 *
	 *	Abstract base class Deconstructor
	 */
	virtual	~VFSPlugin()
	{
		for(unsigned int a=0;a<m_filters.size();a++) delete m_filters[a];
		m_filters.clear();
	};

	/**	Adds a filter to the array of filters this plugin uses to decode files
	 *
	 *	@param filter	The Filter to add to the plugin
	 */
	virtual void AddFilter(VFSFilter *filter)
	{
		if(filter != NULL) m_filters.push_back(filter);
	};

	/**	Decodes the contents of a bytestream
	 *
	 *	@param buffer	A bytestream containing the contents of the file
	 *	@param length	The length of the bytestream
	 *
	 *	@returns A FileInfo structure containing the decoded file contents
	 */
	virtual FileInfo * Read(unsigned char *buffer, unsigned int length) = 0;

	/**	Writes a FileInfo structure to the file
	 *
	 *	@param data		A FileInfo structure containing the data I want to write
	 *	@param length	Will contain the length of the encoded bytestream
	 *
	 *	@returns A bytestrem containing the contents of the file to be written
	 */
	virtual char * Write(FileInfo *data, unsigned int &length) = 0;

	/**	Retrieves the type of plugin this is
	 *
	 *	@returns The identifing string of the plugin
	 */
	virtual char * Type(void) = 0;
};

#endif // #ifndef _VFSPLUGIN_H_
