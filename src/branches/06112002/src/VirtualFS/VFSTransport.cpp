#include <VirtualFS/VFSTransport.h>

/**	Transport constructor 
 *
 *	@param	ident			The identifier string for the transport
 *	@param	category	The category this transport belongs to
 *	@param	h					The Function pointer that will create the handles
 *
 *	Sets the type of file handle this 
 *	transport will be storing and how to construct them
 */
VFSTransport::VFSTransport(char *ident, int category, VFSHandle::handle_t h)
{
	m_ident = new char[strlen(ident)+1];
	strcpy(m_ident,ident);

	m_category = category;

	CreateHandle = h;
}

/**	Transport deconstructor
 *
 *	Operation:
 *		-#	Cleans up any memory allocated
 *		-#	Removes all the handles contained
 */
VFSTransport::~VFSTransport()
{
	delete[] m_ident;

	m_handles.clear();
}

/**	Opens a file
 *
 *	@param filename	The name of the file being opened
 *	@param plugin		The plugin to be used to decode the file data
 *	@param create		Whether to create a non-existant file, or return a false upon attempting to open
 *
 *	Operation:
 *		-#	Creates a new handle
 *		-#	Move the pointer along the string to remove the transport identifier
 *		-#	Requests the handle open the file
 *		-#	Sets the plugin of the new handle
 *		-#	if opening the file was successful, returns the new file handle
 *		-#	Otherwise, handle is deleted and NULL is returned
 */
VFSHandle * VFSTransport::Open(char *filename, VFSPlugin *plugin, bool create)
{
	VFSHandle *handle = CreateHandle(this);

	handle->SetPlugin(plugin);

	if(handle->Open(filename, create) == true){
		m_handles.push_back(handle);
		return handle;
	}

	delete handle;
	return NULL;
}

VFSHandle * VFSTransport::OpenLocation(char *loc, bool create)
{
	VFSHandle *handle = CreateHandle(this);

	if(handle->OpenLocation(loc,create) == true){
		m_handles.push_back(handle);
		return handle;
	}

	delete[] handle;
	return NULL;
}

/**	Closes a file 
 *
 *	@returns true or false, depending on whether the handle was found to be removed from the vfs
 *
 *	Operation:
 *		-#	Loop through all the available handles in this transport
 *		-#	Compare the handle passed into the method, with each handle
 *		-#	If match is found, delete that handle and remove it from the storage
 *		-#	If a match is not found, return false, to signify the handle was not found
 */
bool VFSTransport::Close(VFSHandle *handle)
{
	for(int a=0;a<m_handles.size();a++){
		if(handle == m_handles[a]){
			delete m_handles[a];
			m_handles.erase(m_handles.begin()+a);

			return true;
		}
	}

	return false;
}

/**	Closes All the files relating to this transport
 */
void VFSTransport::CloseAll(void)
{
	for(int a=0;a<m_handles.size();a++){
		delete m_handles[a];
	}

	m_handles.clear();
}

/**	Retrieves the number of open files	*/
int VFSTransport::NumberFiles(void)
{
	return m_handles.size();
}

/**	Retrieves a file handle	*/
VFSHandle * VFSTransport::GetHandle(unsigned int index)
{
	if(index < m_handles.size()) return m_handles[index];

	return NULL;
}

/**	Retrieves the identifier string of this transport	*/
char * VFSTransport::GetIdentifier(void)
{
	return m_ident;
}

/**	Returns the category this transport belongs to */
int VFSTransport::GetCategory(void)
{
	return m_category;
}
