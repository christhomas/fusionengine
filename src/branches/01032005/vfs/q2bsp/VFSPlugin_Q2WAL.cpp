#include <VFSPlugin_Q2WAL.h>

//=============================
//	FileQ2WAL methods
//=============================

VFSPlugin_Q2WAL::VFSPlugin_Q2WAL()
{
	m_type		=	"wal;";
	m_offset		=	0;
	m_palette		= NULL;
}

VFSPlugin_Q2WAL::~VFSPlugin_Q2WAL()
{
	delete[] m_palette;
}

std::string VFSPlugin_Q2WAL::Type(void)
{
	return m_type;
}

FileInfo * VFSPlugin_Q2WAL::Read(unsigned char *buffer, unsigned int length)
{
	m_offset				=	0;
	m_buffer				=	buffer;
	m_fileinfo				=	new ImageFileInfo();
	m_fileinfo->filelength	=	length;

	ReadPalette();

	ReadHeader();

	ReadImage();

	return m_fileinfo;
}

char * VFSPlugin_Q2WAL::Write(FileInfo *data, unsigned int &length)
{
	//	Not implemented
	return NULL;
}

void VFSPlugin_Q2WAL::ReadPalette(void)
{
	ImageFileInfo *fi = NULL;

	if(m_palette == NULL){
		VFSHandle *h = fusion->vfs->Open("file://colourmap.pcx");
		
		if(h != NULL){
			fi = (ImageFileInfo *)h->Read();
			m_palette = new char[768];
			memcpy(m_palette,fi->palette,768);
			delete fi;
		}

		fusion->vfs->Close(h);
	}
}

void VFSPlugin_Q2WAL::ReadHeader(void)
{
	memcpy(&m_header,&m_buffer[m_offset],sizeof(m_header));
	m_offset+=sizeof(m_header);

	m_fileinfo->width	=	m_header.width;
	m_fileinfo->height	=	m_header.height;
	m_fileinfo->bpp	=	8;
	m_fileinfo->palette	=	new unsigned char[768];
	memcpy(m_fileinfo->palette,m_palette,768);
}

void VFSPlugin_Q2WAL::ReadImage(void)
{
	int bytes = m_header.width * m_header.height;

	m_fileinfo->data = new unsigned char[bytes];

	unsigned char *s = m_fileinfo->data;
	unsigned char *d = &m_buffer[m_offset];

	memcpy(s,d,bytes);

	m_offset+=bytes;
}

