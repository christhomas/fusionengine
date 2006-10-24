#ifndef _VFSTRANSPORT_H_
	#define _VFSTRANSPORT_H_

#include <vector>
#include <string>

#include <vfs/VFSHandle.h>

/**	@ingroup	VFS_Group
 *	@brief		Allows file handles to be grouped together by their type
 */
class VFSTransport{
protected:
	/**	@var		std::vector<VFSHandle *> m_handles
	 *	@brief	Array of File handles attached to this transport
	 */
	std::vector<VFSHandle *> m_handles;

	/**	@var		std::string m_ident
	 *	@brief	identifing string for the transport
	 */
	std::string m_ident;

	/**	@var		int m_category
	 *	@brief	The category of transport this belongs to
	 */
	int m_category;

	/**	@var		VFSHandle::handle_t	CreateHandle
	 *	@brief	Function pointer to create a type of file handle
	 */
	VFSHandle::handle_t	CreateHandle;

public:
	/**	@typedef transport_t
	 *	Function pointer to create a transport object
	 */
	typedef VFSTransport * (*transport_t)(Fusion *f);

	enum Category{
		LOCALFS = 1,
		ARCHIVE = 2,
		NETWORK = 3
	};

						VFSTransport	(std::string ident, int category, VFSHandle::handle_t);
	virtual				~VFSTransport	();
	virtual VFSHandle *	Open			(std::string filename, VFSPlugin *plugin, bool create);
	virtual VFSHandle *	OpenLocation	(std::string loc, bool create);
	virtual bool		Close			(VFSHandle *handle);
	virtual void		CloseAll		(void);
	virtual	int			NumberFiles		(void);
	virtual VFSHandle *	GetHandle		(unsigned int index);
	virtual std::string	GetIdentifier	(void);
	virtual int			GetCategory		(void);
};

#endif // #ifndef _VFSTRANSPORT_H_
