#ifndef _VFSPLUGIN_TXT_H_
	#define _VFSPLUGIN_TXT_H_

#include <vfs/VirtualFS.h>

VFSPlugin * CreateTextPlugin(Fusion *f);

/** @ingroup	VFSPlugin_TXT_Group
 *	@brief		File format plugin to read/write text data
 */
class VFSPlugin_TXT: public VFSPlugin{
protected:
	/**	@var		TextFileInfo *m_fileinfo
	 *	@brief	Structure to store text data
	 */
	TextFileInfo *m_fileinfo;
public:
					VFSPlugin_TXT		();
	virtual			~VFSPlugin_TXT	();

	virtual void		AddFilter			(VFSFilter *filter);

	virtual FileInfo *	Read			(unsigned char *buffer, unsigned int length);
	virtual char *		Write			(FileInfo *data, unsigned int &length);

	virtual char *		Type				(void);
};

#endif // #ifndef _VFSPLUGIN_TXT_H_

