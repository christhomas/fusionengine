#ifndef _VFSPLUGIN_JPEG_H_
	#define _VFSPLUGIN_JPEG_H_

#include <vfs/VirtualFS.h>
#include <jpeglib.h>

class VFSPlugin_JPEG: public VFSPlugin{
protected:
	/**	@var		ImageFileInfo *m_fileinfo
	 *	@brief	Structure to store the image data
	 */
	ImageFileInfo	*m_fileinfo;

	jpeg_decompress_struct	m_cinfo;
	
	jpeg_error_mgr	m_jerr;

	virtual void Setup	(void);
	virtual bool Decode	(void);

public:
					VFSPlugin_JPEG	();
	virtual			~VFSPlugin_JPEG	();

	virtual FileInfo *	Read			(unsigned char *buffer, unsigned int length);
	virtual char *		Write			(FileInfo *data, unsigned int &length);

	virtual char *		Type				(void);
};

#endif // #ifndef _VFSPLUGIN_JPEG_H_
