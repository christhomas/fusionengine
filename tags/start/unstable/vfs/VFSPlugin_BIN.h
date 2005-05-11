#ifndef _VFSPLUGIN_BIN_H_
	#define _VFSPLUGIN_BIN_H_

#include <vfs/VirtualFS.h>

VFSPlugin * CreateBinaryPlugin( Fusion *f );

/** @ingroup	VFSPlugin_BIN_Group
 *	@brief		File format plugin to read/write unformatted (or preformatted) binary data
 */
class VFSPlugin_BIN: public VFSPlugin
{
protected:
	/**	@var		BinaryFileInfo *m_fileinfo
	 *	@brief	Structure to store the unformatted data
	 */
	BinaryFileInfo *m_fileinfo;
public:
					VFSPlugin_BIN	();
	virtual			~VFSPlugin_BIN	();

	virtual void		AddFilter	( VFSFilter *filter );

	virtual FileInfo *	Read	( unsigned char *buffer, unsigned int length );
	virtual char *		Write	( FileInfo *data, unsigned int &length );

	virtual char *		Type	( void );
};

#endif // #ifndef _VFSPLUGIN_BIN_H_
