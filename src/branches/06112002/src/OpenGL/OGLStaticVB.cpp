#include <OpenGL/OGLStaticVB.h>
#include <OpenGL/OpenGL12.h>

/**	OpenGL Static VertexBuffer constructor */
OGLStaticVB::OGLStaticVB()
{

}

/**	OpenGL Static VertexBuffer Deconstructor */
OGLStaticVB::~OGLStaticVB()
{

}

/**	Initialises the VertexBuffer
 *
 *	@param nv		The number of Vertices this object has access to
 *	@param ni		The number of indices this object will reference
 *	@param nc_p	The number of components in each position
 *	@param nc_t	The number of components in each texture coordinate
 *
 *	@returns boolean true or false, depending on nothing at this time, you'll get true regardless
 *
 *	NOTE: NOT IMPLEMENTED
 */
bool OGLStaticVB::Initialise(int nv, int ni, int nc_p, int nc_t)
{
	return true;
}

/**	Releases all the stored internal data
 *	
 *	NOTE: NOT IMPLEMENTED
 */
void OGLStaticVB::ReleaseAll(void)
{

}

/**	Sets the number of components for each position and texture coordinate
 *
 *	@param p		The number of components in each position
 *	@param t	The number of components in each texture coordinate
 *
 *	NOTE: NOT IMPLEMENTED
 */
void OGLStaticVB::SetComponents(int p, int t)
{

}

/**	Sets the name of this part of the mesh
 *
 *	@param c	A string containing the name
 */
void OGLStaticVB::SetName(char *n)
{
	delete[] m_name;

	m_name = new char[strlen(n)+1];

	strcpy(m_name,n);
}

/**	Sets the position data
 *
 *	@param p	A pointer to the position data
 *
 *	NOTE: NOT IMPLEMENTED
 */
void OGLStaticVB::SetPosition(float *p)
{
}

/**	Sets the normal data
 *
 *	@param normal	A pointer to the normal data
 *
 *	NOTE: NOT IMPLEMENTED
 */
void OGLStaticVB::SetNormal(float *n)
{
}

/**	Sets the texture coordinate data
 *
 *	@param t	A pointer to the texture coordinate data
 *
 *	NOTE: NOT IMPLEMENTED
 */
void OGLStaticVB::SetTexcoord(float *t)
{
}

/**	Sets the index data for the mesh
 *
 *	@param	i	A pointer to the index data
 *
 *	NOTE: NOT IMPLEMENTED
 */
void OGLStaticVB::SetIndex(int *i)
{
}

/**	Sets the texture to use on this mesh
 *
 *	@param t	The texture to use
 *	NOTE: NOT IMPLEMENTED
 */
void OGLStaticVB::SetTexture(ITexture *t)
{
}

/**	Sets the colour to render the mesh in
 *
 *	@param c	A Colour4f structure containing the colour to render in
 *	NOTE: NOT IMPLEMENTED
 */
void OGLStaticVB::SetColour(Colour4f *c)
{
}

/**	Sets the Material of this mesh chunk
 *
 *	@param m	The material data to copy
 */
void OGLStaticVB::SetMaterial(Material *m)
{
	memcpy(&m_material,m,sizeof(Material));
}

/**	Sets the colour to render the mesh in
 *
 *	@param r	The red component
 *	@param g	The green component
 *	@param b	The blue component
 *	@param a	The Alpha component
 *
 *	NOTE: NOT IMPLEMENTED
 */
void OGLStaticVB::SetColour(float r, float g, float b, float a)
{
}

/**	Retrieves the name of the mesh chunk
 *
 *	@returns	A String containing the name
 */
char * OGLStaticVB::GetName(void)
{
	return m_name;
}

/**	Retrieves a pointer to the position data
 *
 *	@returns A pointer to the position data
 */
float * OGLStaticVB::GetPosition(void)
{
	return m_position;
}

/**	Retrieves a pointer to the normal data
 *
 *	@returns A pointer to the normal data
 */
float *	OGLStaticVB::GetNormal(void)
{
	return m_normal;
}

/**	Retrieves a pointer to the texture coordinate data
 *
 *	@returns	A pointer to texture coordinate data
 */
float * OGLStaticVB::GetTexcoord(int layer)
{
	return m_texcoord;
}

/**	Retrieves a pointer to the index data
 *
 *	@returns A pointer to the index data
 */
int * OGLStaticVB::GetIndex(void)
{
	return m_index;
}

/**	Retrieves a pointer to the texture assigned
 *
 *	@returns A ITexture object if one has been assigned
 */
ITexture * OGLStaticVB::GetTexture(int layer)
{
	return m_texture;
}

/**	Retrieves a pointer to a Colour4f structure
 *
 *	@returns A Colour4f pointer
 */
Colour4f * OGLStaticVB::GetColour(void)
{
	return &m_material.colour;
}

/**	Retrieves the material data of this mesh chunk
 *
 *	@returns	A Material object
 */
Material * OGLStaticVB::GetMaterial(void)
{
	return &m_material;
}

/**	Retrieves The number of indices this object is using
 *
 *	@returns The number of indices allocated
 *
 *	This incidentally is the same as the number of vertices this vertexpool is referencing
 */
int OGLStaticVB::GetNumIndex(void)
{
	return m_num_index;
}

/**	Renders the VertexBuffer
 *
 *	NOTE: NOT IMPLEMENTED
 */
void OGLStaticVB::Render(void)
{
}
