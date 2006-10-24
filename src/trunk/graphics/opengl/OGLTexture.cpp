#include <OGLTexture.h>
#include <OpenGL12.h>

OGLTexture::OGLTexture(int width, int height, int numcomp, std::string filename)
{
	m_tbid		=	-1;
	m_width		=	width;
	m_height	=	height;
	m_numcomp	=	numcomp;
	
	SetFilename(filename);
}

OGLTexture::~OGLTexture()
{
	DeleteTexture();
}

/**	Sets the filename of the texture
 *
 *	@param	filename	The filename of the texture
 */
void OGLTexture::SetFilename(std::string filename)
{
	m_filename = filename;
}

/**	Creates the opengl texture object
 *
 *	@param buffer	The buffer containing the texture image data
 *
 *	Operation:
 *		-#	Generate an opengl texture
 *		-#	Enable GL_TEXTURE_2D
 *		-#	Load the buffer containing the 32bit image data into an opengl texture object (This is not the same as a Fusion Texture object)
 *		-#	Sets some standard texture parameters, 2D Texture, Min filter is linear, Max filter is linear
 */
void OGLTexture::CreateTexture(unsigned char *buffer)
{
	// Create The Texture
	glGenTextures(1, (GLuint*)&m_tbid);

	glEnable(GL_TEXTURE_2D);

	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, m_tbid);
	
	//	Explanation of:
	//	GL_RGB+(m_numcomp-3)
	//	
	//	since m_numcomp will be 3 or 4, depending on whether you want RGB or RGBA if you subtract 3 from the number of components
	//	you'll get a 0 or 1 value, add this to GL_BGR and you get the required opengl identifier for the texture you are creating
	glTexImage2D(GL_TEXTURE_2D,0,m_numcomp,m_width,m_height,0,GL_RGB+(m_numcomp-3),GL_UNSIGNED_BYTE,buffer);
	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

/**	Deletes the opengl texture object
 *
 *	Operation:
 *		-#	Test whether the texture is valid (there is a texture object to delete)
 *		-#	If so, make the tbid (texture buffer id) into an array, opengl 
 *				requires an array of texture id's, rather than one texture id at a time
 *		-#	Call opengl to delete the texture object
 *		-#	Reset the tbid to -1
 */
void OGLTexture::DeleteTexture(void)
{
	if(m_tbid != -1){
		const unsigned int t[1] = {m_tbid};
		glDeleteTextures(1,t);
		m_tbid = -1;
	}
}

/**	Sets the texture to current rendering source
 *
 *	This tells opengl to use this texture when rendering any texturemaps
 */
void OGLTexture::Set(void)
{
	glBindTexture(GL_TEXTURE_2D, m_tbid);
}

/**	Reload the image
 *
 *	@returns The texture buffer ID of this object
 *
 *	This attempts to reload the image from the file, if for example, the opengl texture object was lost
 */
int	OGLTexture::Reload(void)
{
	return UpdateTexture();
}

/**	Retrieves the dimensions of the file
 *
 *	@param x	The width of the image
 *	@param y	The height of the image	
 */
void OGLTexture::GetDimensions(int &x, int &y)
{
	x = m_width;
	y = m_height;
}

/**	Retrieves the width of the texture
 *
 *	@returns	The width of the texture
 */
int OGLTexture::GetWidth(void)
{
	return m_width;
}

/**	Retrieves the height of the texture
 *
 *	@returns	The height of the texture
 */
int OGLTexture::GetHeight(void)
{
	return m_height;
}
