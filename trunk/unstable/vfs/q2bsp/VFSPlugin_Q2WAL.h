#ifndef _VFSPLUGIN_Q2WAL_H_
	#define _VFSPLUGIN_Q2WAL_H_

#include <vfs/VirtualFS.h>
#include <Fusion.h>

/**	@ingroup	VFSPlugin_Q2BSP_Group
 *	@brief		Header structure of the WAL File format
 */
struct WALHeader{
	char	filename[32];		/**<	name of the file */	
	int	width;			/**<	width of the texture */
	int	height;			/**<	height of the texture */
	int	offset[4];			/**<	file image offsets for image data */
	char	animation[32];		/**<	name of next frame in animation chain */
	int	flags;			/**<	texture flags */
	int	contents;			/**<	i dont know */
	int	value;			/**<	i dont know */
};

/**	@ingroup	VFSPlugin_Q2BSP_Group
 *	@brief		File format plugin to read/write Quake2 WAL Texture images
 */
class VFSPlugin_Q2WAL: public VFSPlugin{
protected:
	ImageFileInfo	*m_fileinfo;

	WALHeader	m_header;
	char			*m_palette;
public:
					VFSPlugin_Q2WAL	();
	virtual			~VFSPlugin_Q2WAL	();

	virtual FileInfo *	Read			(unsigned char *buffer, unsigned int length);
	virtual char *		Write			(FileInfo *data, unsigned int &length);

	virtual char *		Type				(void);

	virtual void		ReadPalette		(void);
	virtual void		ReadHeader		(void);
	virtual void		ReadImage		(void);
};

#endif // #ifndef _VFSPLUGIN_Q2WAL_H_

