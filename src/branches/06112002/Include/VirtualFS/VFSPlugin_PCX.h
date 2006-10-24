#ifndef _VFSPLUGIN_PCX_H_
	#define _VFSPLUGIN_PCX_H_

#include <Graphics/Pixel.h>
#include <VirtualFS/VirtualFS.h>

const int headerlength	= 128;
const int	palettelength	= 768;

/**	@ingroup	VFSPlugin_PCX_Group
 *	@brief		Header structure of the PCX file
 *
 *	This palette structure is 128 bytes long as defined in the spec
 */
struct pcxheader{
	/**	@var		char Manufacturer
	 *	Constant Flag
	 *	10 = ZSoft PCX
	 */
	char Manufacturer;

	/**	@var		char Version
	 *	Version Information:
	 *		0 = v2.5
	 *		2 = v2.8 w/palette
	 *		3 = v2.8 w/o palette
	 *		5 = v3.0 w/palette
	 */
	char Version;

	char Encoding;			/**<	1 = RLE Encoding */
	char BitsPerPixel;	/**<	Number of bits per pixel per plane */
	short minx;					/**<	should be zero */
	short miny;					/**<	should be zero */	
	short maxx;					/**<	width of the image */	
	short maxy;					/**<	height of the image */
	short HRes;					/**<	Horizontal Resolution of creating device */
	short VRes;					/**<	Vertical Resolution of creating device */
	char Colormap[48];	/**<	Colour palette setting */
	char Reserved;			/**<	Reserved */
	char nPlanes;				/**<	Number of colour planes */
	short BytesPerLine;	/**<	Number of bytes per scan line per colour plane (always even for .PCX files) */
	
	/**	@var		short PaletteInfo
	 *	How to interpret palette
	 *	1 = colour/BW
	 *	2 = greyscale
	 */
	short PaletteInfo;

	char Filler[58];		/**<	Blank to fill out 128 byte header */
};

/**	@ingroup	VFSPlugin_PCX_Group
 *	@brief		File format plugin to read/write PCX images
 */
class VFSPlugin_PCX: public VFSPlugin{
protected:
	/**	@var		pcxheader header
	 *	@brief	A structure to read the header into and decode
	 */
	pcxheader header;

	/**	@var		char *raw_image
	 *	@brief	A bytestream pointer to hold a temporary copy of the image data
	 */
	char *raw_image;
	
	/**	@var		ImageFileInfo *m_fileinfo
	 *	@brief	Structure to store the image data
	 */
	ImageFileInfo	*m_fileinfo;

	bool	ReadPCXHeader		(void);
	void	ReadPCXPalette	(void);
	void	ReadPCXImage		(void);
	void	DecodeImage			(void);

public:
											VFSPlugin_PCX		();
	virtual							~VFSPlugin_PCX	();

	virtual FileInfo *	Read						(unsigned char *buffer, unsigned int length);
	virtual char *			Write						(FileInfo *data, unsigned int &length);

	virtual char *			Type						(void);
};

#endif // #ifndef _VFSPLUGIN_PCX_H_