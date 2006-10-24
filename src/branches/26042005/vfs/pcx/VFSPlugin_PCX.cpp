#include <VFSPlugin_PCX.h>

/**	PCX File Format plugin Constructor
 *
 *	Sets the default file extension for this format
 */
VFSPlugin_PCX::VFSPlugin_PCX()
{
	m_type			=	"pcx;";
	m_offset		=	0;
	m_fileinfo	=	NULL;
}

/**	PCX File Format plugin Deconstructor */
VFSPlugin_PCX::~VFSPlugin_PCX(){}

/**	Retrieves the type of file the plugin handles
 *
 *	@returns	The plugin identifier string
 */
char * VFSPlugin_PCX::Type(void)
{
	return m_type;
}

/**	Takes a bytestream and converts it into a structured format
 *
 *	@param buffer		A Bytestream containing the file contents
 *	@param length		The length of the bytestream
 *
 *	@returns	A FileInfo structure containing the files contents
 *
 *	Operation:
 *		-#	Calls VFSPlugin_PCX::ReadPCXHeader()
 *		-#	Calls VFSPlugin_PCX::ReadPCXImage()
 *		-#	Calls VFSPlugin_PCX::ReadPCXPalette()
 *		-#	Calls VFSPlugin_PCX::DecodeImage()
 *		-#	Returns FileInfo structure
 */
FileInfo * VFSPlugin_PCX::Read(unsigned char *buffer, unsigned int length)
{
	m_buffer = buffer;
	m_length = length;
		
	m_fileinfo							=	new ImageFileInfo();
	m_fileinfo->filelength	=	m_length;

	m_offset = 0;

	if(ReadPCXHeader() == false){
		delete m_fileinfo;
		return NULL;
	}
	
	ReadPCXImage();
	ReadPCXPalette();
	DecodeImage();
	
	return m_fileinfo;
}

/**	Writes data to a bytestream
 *
 *	@param data		A FileInfo structure containing the data to write to the bytestream
 *	@param length	The length of the resulting bytestream
 *
 *	@returns	A Bytestream containing the contents of the file
 */
char * VFSPlugin_PCX::Write(FileInfo *data, unsigned int &length)
{
	//	Not Implemented
	return NULL;
}

/**	Reads the PCX Header
 *
 *	@returns boolean true or false, depending on whether the file is Version 5 and 8bit colour
 *
 *	Operation:
 *		-#	Copies the first 128 bytes into the header structure
 *		-#	extracts the width/height/bit depth information from the header
 */
bool VFSPlugin_PCX::ReadPCXHeader(void)
{
	memcpy(&header,&m_buffer[m_offset],headerlength);
	m_offset+=headerlength;
	m_length -= headerlength;

	m_fileinfo->width		= header.maxx+1;
	m_fileinfo->height	= header.maxy+1;
	m_fileinfo->bpp			= header.BitsPerPixel;

	if(header.Version == 5 && header.BitsPerPixel == 8) return true;

	return false;
}

/**	Reads the Palette Data
 *
 *	Operation:
 *		-#	If the PCX Version is 5 and bitdepth is 8bit, file is ok to read
 *		-#	Allocate a buffer for the palette data and copy it from the bytestream
 */
void VFSPlugin_PCX::ReadPCXPalette(void)
{
	m_fileinfo->palette = new unsigned char[palettelength];
	
	memset(m_fileinfo->palette,0,palettelength);

	memcpy(m_fileinfo->palette,&m_buffer[m_offset],palettelength);
	
	m_offset+=palettelength;

	m_length -= palettelength;
}

/**	Reads the Image data
 *
 *	Operation:
 *		-#	Calculate the number of bytes the image is stored in
 *		-#	Create a buffer that size
 *		-#	Copy that many bytes from the bytestream into the buffer
 */
void VFSPlugin_PCX::ReadPCXImage(void)
{
	int read = m_length - palettelength;

	raw_image = new char[read];

	memcpy(raw_image,&m_buffer[m_offset],read);
	m_offset+=read;

	m_length -= read;
}

/**	Decodes the raw image data into the final image
 *
 *	The pcx format compresses the data to the raw image is a compressed version
 *
 *	Operation:
 *		-#	Calculate the number of pixels in the image
 *		-#	Allocate a decodebuffer, the size of the decompressed image
 *		-#	Loop for the number of pixels in the decompressed image
 *		-#	Decode the pixel, copy the number of expanded pixels to the decodebuffer
 *		-#	Once decoded delete the raw image buffer
 *		-#	Assign the buffer into the FileInfo structure
 *	
 *	Decoding:
 *	This is a very simple process, you take a byte, 
 *	compare it against an ID byte, which will tell 
 *	you whether its a data byte or an ID byte, the 
 *	value of an ID byte is 192 (0xC0), if and'ing the byte 
 *	against the databyte produces 192, this is an ID byte.
 *
 *	You then extract from the byte the number of 
 *	pixels the next byte will decompress to, to 
 *	do this "and" the binary "not" of 0xC0, which 
 *	is 0x3F to the databyte.
 *	
 *	0xC0 = 1100 0000
 *	0x3F = 0011 1111
 *
 *	This will produce a value which is the number 
 *	of pixels to apply the next byte to.  Once 
 *	you have this value, you set the number of 
 *	pixels and read the next byte, once read, 
 *	you copy that byte into that many pixels, 
 *	then continue until you've read the 
 *	correct number of pixels.  Easy
 *
 *	@todo	Perhaps I should implement this encoding as a VFSFilter?
 */
void VFSPlugin_PCX::DecodeImage(void)
{
	int pcxlocation = 0,pixels = 0;
	int numberpixels = (header.maxx+1) * (header.maxy+1);
	
	char	databyte			= 0;
	char	*decodebuffer	= new char[numberpixels];
	char	*buffer				= raw_image;
	
	while (pcxlocation < numberpixels)
	{
		databyte = *buffer++;
		pixels = 1;
		if((databyte & 0xC0) == 0xC0)
		{
			pixels = (databyte & 0x3F);
			databyte = *buffer++;
		}
		memset(decodebuffer+pcxlocation,databyte,pixels);
		pcxlocation += pixels;
	}

	if(raw_image) delete[] raw_image;

	m_fileinfo->data = (unsigned char *)decodebuffer;
	decodebuffer = NULL;
}
