#include <VFSPlugin_TGA.h>
#include <maths/Maths.h>
#include <graphics/Pixel.h>

/**	Targa file format plugin Constructor
 *
 *	Sets the default file extension for this format
 */
VFSPlugin_TGA::VFSPlugin_TGA()
{
	m_type		=	"tga;";
	m_fileinfo	=	NULL;
}

/**	Targa file format plugin deconstructor */
VFSPlugin_TGA::~VFSPlugin_TGA(){}

/**	Retrieves the type of file the plugin handles
 *
 *	@returns	The plugin identifier string
 */
std::string VFSPlugin_TGA::Type(void)
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
 *		-#	Read the header from the bytestream
 *		-#	Compare the header from the bytestream with the stored header
 *		-#	If the header matches, the file is okay
 *		-#	Read six bytes from the bytestream, these represent the width/height/bits per pixel of the image
 *		-#	If the width and height are greater than zero, image is ok
 *		-#	Calculate the number of bytes the image would take to store
 *		-#	Allocate a buffer than size and copy the image data across
 *		-#	Return this FileInfo structure
 *		-#	If the header or image dimensions were not valid, the FileInfo structure will be deleted and NULL Returned
 */
FileInfo * VFSPlugin_TGA::Read(unsigned char *buffer, unsigned int length)
{
	m_buffer = buffer;
	m_length = length;
	m_offset = 0;

	m_fileinfo							=	new ImageFileInfo();
	m_fileinfo->filelength	=	m_length;
	m_fileinfo->palette			=	NULL;

	unsigned char		TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header
	unsigned char		header[6];
	unsigned int		imagesize;									// Used To Store The Image Size When Setting Aside Ram

	//	Compare the first 12 bytes with the basic TGA Header
	if(memcmp(TGAheader,&m_buffer[m_offset],sizeof(TGAheader))==0)
	{
		m_offset+=sizeof(TGAheader);

		memcpy(header,&m_buffer[m_offset],sizeof(header));
		m_offset+=sizeof(header);

		// Determine The TGA Width + height
		m_fileinfo->width  = ((short *)header)[0];
		m_fileinfo->height = ((short *)header)[1];

		// Is The Width Less Than Or Equal To Zero
		// Is The Height Less Than Or Equal To Zero
		if(m_fileinfo->width > 0 || m_fileinfo->height > 0)
		{
			// Grab The TGA's Bits Per Pixel (24 or 32)
			m_fileinfo->bpp = header[4];

			// Is The TGA 24 or 32 Bit?
			if(m_fileinfo->bpp==24 || m_fileinfo->bpp==32)
			{
				// Calculate The Memory Required For The TGA Data
				imagesize = m_fileinfo->width * m_fileinfo->height * (m_fileinfo->bpp>>3);

				m_fileinfo->data = new unsigned char[imagesize];

				// read the image data
				memcpy(m_fileinfo->data,&m_buffer[m_offset],imagesize);

				InvertScanlines();
				InvertRGB();

				return m_fileinfo;
			}
		}
	}

	delete m_fileinfo;

	// error occured
	return NULL;
}

/**	Writes data to a bytestream
 *
 *	@param data		A FileInfo structure containing the data to write to the bytestream
 *	@param length	The length of the resulting bytestream
 *
 *	@returns	A Bytestream containing the contents of the file
 */
char * VFSPlugin_TGA::Write(FileInfo *data, unsigned int &length)
{
	m_fileinfo = new ImageFileInfo;

	Convert24Bit(data);
	InvertScanlines();
	InvertRGB();

	int offset = 0;
	unsigned char TGAHeader[12] = {0,0,2,0,0,0,0,0,0,0,0,0};
	unsigned short header[3];

	header[0] = (short)m_fileinfo->width;
	header[1] = (short)m_fileinfo->height;
	header[2] = (short)m_fileinfo->bpp;

	int datalength = m_fileinfo->width * m_fileinfo->height * (m_fileinfo->bpp>>3);

	char *filebuffer = new char[12+6+datalength];

	memcpy(&filebuffer[offset],TGAHeader,12);
	offset+=sizeof(TGAHeader);

	memcpy(&filebuffer[offset],header,6);
	offset+=sizeof(header);

	memcpy(&filebuffer[offset],m_fileinfo->data,datalength);
	
	delete[]	m_fileinfo->data;
	delete		m_fileinfo;

	length = sizeof(TGAHeader) + sizeof(header) + datalength;

	return filebuffer;
}

/**	Inverts all the scanlines in the tga image data
 *
 *	TGA stores it's images upside down, like BMP.
 *	so an inversion of all the scanlines must be done
 *	to "flip" the image the correct way up
 */
void VFSPlugin_TGA::InvertScanlines(void)
{
	int bytes = m_fileinfo->width * (m_fileinfo->bpp>>3);
	unsigned char *scanline = new unsigned char[bytes];

	int a,b;

	for(a=0,b=m_fileinfo->height-1;a<m_fileinfo->height/2;a++,b--){

		unsigned char *s	=	&m_fileinfo->data[bytes * a];
		unsigned char *d	=	&m_fileinfo->data[bytes * b];
		
		memcpy(scanline,s,bytes);		
		memcpy(s,d,bytes);		
		memcpy(d,scanline,bytes);
	}

	delete[] scanline;
}

/**	Inverts the BGR colour data to RGB colour data
 *
 *	TGA files store the colour data as BGR or BGRA, 
 *	which must be swapped to RGB or RGBA
 */
void VFSPlugin_TGA::InvertRGB(void)
{
	int numcomp = m_fileinfo->bpp>>3;

	int bytes = m_fileinfo->width * m_fileinfo->height * numcomp;
		
	for(int a=0;a<bytes;a+=numcomp)	std::swap(m_fileinfo->data[a+0],m_fileinfo->data[a+2]);
}

void VFSPlugin_TGA::Convert24Bit(FileInfo *data)
{
	ImageFileInfo *fi = (ImageFileInfo *)data;

	int numpixels = fi->width*fi->height;

	m_fileinfo->width		=	fi->width;
	m_fileinfo->height	=	fi->height;
	m_fileinfo->bpp			=	fi->bpp;

	if(fi->bpp < 24){
		
		m_fileinfo->bpp			=	24;
		m_fileinfo->data		=	new unsigned char[numpixels*(m_fileinfo->bpp>>3)];

		Pixel *s = NULL;
		Pixel *d = new Pixel24Bit(m_fileinfo->data);

		switch(m_fileinfo->bpp){
			case 8:	{	s	=	new Pixel8Bit(fi->data,fi->palette);	}break;
			case 16:{	s	=	new Pixel16Bit(fi->data,5,6,5);				}break;
		};

		d->SetInput(s->GetOutput());

		for(int offset=0;offset<numpixels;offset++){
			s->ReadPixel(offset);
			d->WritePixel(offset);
		}

		delete	s;
		delete	d;		
	}else{
		m_fileinfo->data = new unsigned char[numpixels*(m_fileinfo->bpp>>3)];
		memcpy(m_fileinfo->data,fi->data,numpixels*(m_fileinfo->bpp>>3));
	}
}

