#ifndef _VFSPLUGIN_TGA_H_
	#define _VFSPLUGIN_TGA_H_

#include <vfs/VirtualFS.h>

/**	@ingroup	VFSPlugin_TGA_Group
 *	@brief		File format plugin to read/write targa images
 */
class VFSPlugin_TGA: public VFSPlugin{
protected:
	/**	@var		ImageFileInfo *m_fileinfo
	 *	@brief	Structure to store the image data
	 */
	ImageFileInfo	*m_fileinfo;
	
	virtual void		InvertScanlines	(void);
	virtual void		InvertRGB		(void);
	virtual void		Convert24Bit	(FileInfo *data);
public:
						VFSPlugin_TGA	();
	virtual				~VFSPlugin_TGA	();
	
	virtual FileInfo *	Read			(unsigned char *buffer, unsigned int length);
	virtual char *		Write			(FileInfo *data, unsigned int &length);

	virtual std::string	Type			(void);
};

#endif // #ifndef _VFSPLUGIN_TGA_H_

