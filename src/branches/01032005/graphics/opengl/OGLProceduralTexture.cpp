#include <OGLProceduralTexture.h>

/** OpenGL Procedural texture object Constructor
 *
 *	@param	width		The width of the texture
 *	@param	height	The height of the texture
 *	@param	proc		The procedure that will generate the texture
 */
OGLProceduralTexture::OGLProceduralTexture(int width, int height, int numcomp, textureproc_t proc): 
OGLTexture(width,height,numcomp,"Procedural")
{	
	m_proc = proc;
}

/**	OpenGL Procedural texture object Deconstructor
 *
 *	Deletes any memory allocated
 */
OGLProceduralTexture::~OGLProceduralTexture(){}

/**	Updates the texture
 *
 *	@returns	The texture buffer ID of this object
 *
 *	Operation:
 *		-#	Allocate a buffer the size requested in the constructor
 *		-#	Initialise the buffer with zeros
 *		-#	Call the procedure, passing the buffer, width and height to it
 *		-#	Generate an OpenGL texture object
 *		-#	Enable GL_TEXTURE_2D
 *		-#	Load the buffer containing the 32bit image data into an opengl texture object (This is not the same as a Fusion Texture Object)
 *		-#	Delete any temporary memory allocated
 *		-#	Return the texture buffer id
 */
int OGLProceduralTexture::UpdateTexture()
{
	DeleteTexture();

	unsigned char *buffer = new unsigned char[m_width*m_height*m_numcomp];

	memset(buffer,0,m_width*m_height*m_numcomp);

	buffer = m_proc(buffer,m_width,m_height,m_numcomp);

	CreateTexture(buffer);

	delete[] buffer;

	return m_tbid;
}

