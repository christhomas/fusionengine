#include <VFSPlugin_JPEG.h>
#include <fstream>

static bool jpeg_error;

static void error_exit(j_common_ptr pcinfo)
{
	// Create the message string
	char sz[256];
	(pcinfo->err->format_message) (pcinfo, sz);
	static std::ofstream o("jerr.log");
	o << sz << std::endl;

	jpeg_error = true;
}

static void init_source(j_decompress_ptr cinfo){}

static boolean fill_input_buffer(j_decompress_ptr cinfo)
{
	jpeg_error = true;
	return TRUE;
}

static void skip_input_data(j_decompress_ptr cinfo, long count) 
{
	jpeg_source_mgr * src = cinfo->src;

	if(count > 0) {
		src->bytes_in_buffer -= count;
		src->next_input_byte += count;
	}
}

static void term_source(j_decompress_ptr cinfo){}

VFSPlugin_JPEG::VFSPlugin_JPEG()
{
	m_type	=	"jpg;jpeg;";
	m_offset	=	0;
	m_length	=	0;
	m_fileinfo	=	NULL;
	m_buffer	=	NULL;
}

/**	JPEG Plugin constructor */
VFSPlugin_JPEG::~VFSPlugin_JPEG()
{

}

std::string VFSPlugin_JPEG::Type(void)
{
	return m_type;
}

FileInfo * VFSPlugin_JPEG::Read(unsigned char *buffer, unsigned int length)
{
	m_offset	= 0;
	m_buffer	= buffer;
	m_length	= length;

	m_fileinfo							=	new ImageFileInfo();
	m_fileinfo->filelength	=	m_length;
	m_fileinfo->palette			=	NULL;

	jpeg_decompress_struct	cinfo;
	jpeg_error_mgr					jerr;

	cinfo.err = jpeg_std_error(&jerr);

	jpeg_create_decompress(&cinfo);

	jpeg_source_mgr jsrc;

	jsrc.bytes_in_buffer = m_length;
	jsrc.next_input_byte = (const unsigned char *)m_buffer;

	jsrc.init_source				=	init_source;
	jsrc.fill_input_buffer	=	fill_input_buffer;
	jsrc.skip_input_data		=	skip_input_data;
	jsrc.resync_to_restart	=	jpeg_resync_to_restart;	// use default method
	jsrc.term_source				=	term_source;

	cinfo.src = &jsrc;

	jpeg_read_header(&cinfo,TRUE);

	jpeg_start_decompress(&cinfo);

	m_fileinfo->width		= cinfo.output_width;
	m_fileinfo->height	= cinfo.output_height;
	m_fileinfo->bpp			= cinfo.num_components<<3;

	int bytes = m_fileinfo->width * m_fileinfo->height * (m_fileinfo->bpp>>3);
	m_fileinfo->data = new unsigned char[bytes];

	// read scanlines
	int i;

	unsigned char *p = m_fileinfo->data;

	while(cinfo.output_scanline < cinfo.output_height) {
		i = jpeg_read_scanlines(&cinfo, &p, 1);
		p += i * (m_fileinfo->bpp>>3) * cinfo.output_width;
	}

	jpeg_finish_decompress(&cinfo);

	jpeg_destroy_decompress(&cinfo);

	return m_fileinfo;
}

char * VFSPlugin_JPEG::Write(FileInfo *data, unsigned int &length)
{
	return NULL;
}

void VFSPlugin_JPEG::Setup(void)
{

}

bool VFSPlugin_JPEG::Decode(void)
{
	return false;
}
