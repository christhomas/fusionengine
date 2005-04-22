#include <OGLImageTexture.h>
#include <graphics/Pixel.h>
#include <Fusion.h>

/**	OpenGL Image Texture object Constructor
 *
 *	@param filename	The filename of the texture
 */
OGLImageTexture::OGLImageTexture(char *filename): 
OGLTexture(0,0,0,filename){}

/**	OpenGL Image Texture object Destructor
 *
 *	Deletes any memory allocated
 */
OGLImageTexture::~OGLImageTexture(){}

/**	Updates the texture
 *
 *	@returns	The texture buffer ID of this object
 *
 *	Operation:
 *		-#	Attempts to use the vfs to open the file
 *		-#	Sets the filename to the file requested
 *		-#	If the file handle is valid, continue to load the image
 *		-#	Read the file contents
 *		-#	Close the file handle
 *		-#	Create a 32bit image buffer to store the image in
 *		-#	Create two pixel objects, src/dest, the dest pixel object is 32bit, the src is whatever bpp the source image was
 *		-#	Setup the pixel objects to read pixels from the src image and convert them to 32bit pixels
 *		-#	Loop through the pixels, converting each to 32bit
 *		-#	delete any temporary memory allocated
 *		-#	Return the texture buffer id
 */
int OGLImageTexture::UpdateTexture(void)
{
	DeleteTexture();

	VFSHandle *handle = fusion->vfs->Open(m_filename);

	if(handle != NULL){
		SetFilename(handle->Filename());

		m_fileinfo = reinterpret_cast<ImageFileInfo *>(handle->Read());

		if(m_fileinfo != NULL){
			m_width		= m_fileinfo->width;
			m_height	=	m_fileinfo->height;
			m_numcomp	=	m_fileinfo->bpp>>3;

			unsigned char *buffer = m_fileinfo->data;

			if(m_fileinfo->bpp < 24){
				m_numcomp = 3;

				buffer = new unsigned char[m_width * m_height * m_numcomp];

				Pixel	*s			=	NULL;
				Pixel	*d			=	new Pixel24Bit(buffer);

				switch(m_fileinfo->bpp){
					case 8:	{	s = new Pixel8Bit		(m_fileinfo->data,m_fileinfo->palette);	}break;
					case 16:{	s = new Pixel16Bit	(m_fileinfo->data,5,6,5);								}break;
				};

				d->SetInput(s->GetOutput());

				int numpixels = m_width * m_height;

				for(int offset=0;offset<numpixels;offset++){
					s->ReadPixel(offset);
					d->WritePixel(offset);
				}

				delete		s;
				delete		d;
				delete[]	m_fileinfo->data;
			}

			CreateTexture(buffer);

			delete[]	buffer;
			delete[]	m_fileinfo->palette;
			delete		m_fileinfo;
		}	

		fusion->vfs->Close(handle);
	}

	return m_tbid;
}

