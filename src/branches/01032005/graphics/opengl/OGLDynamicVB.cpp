#include <OGLDynamicVB.h>
#include <graphics/ITexture.h>
#include <Fusion.h>
#include <OpenGL12.h>

/**	OpenGL Dynamic VertexBuffer constructor
 *	Resets all the internal data to zero
 */
OGLDynamicVB::OGLDynamicVB()
{
	m_index				= NULL;
	m_state				= STATERESET;

	m_smoothingangle	= 0;

	ReleaseAll();
}

/**	OpenGL Dynamic VertexBuffer deconstructor
 *	Upon deletion, resets all the internal data to zero
 */
OGLDynamicVB::~OGLDynamicVB()
{
	ReleaseAll();
}

/**	Initialises the Vertexpool
 *
 *	@param nv		The number of Vertices this object has access to
 *	@param ni		The number of indices this object will reference
 *	@param nc_p	The number of components in each position
 *	@param nc_t	The number of components in each texture coordinate
 *
 *	@returns boolean true or false, depending on nothing at this time, you'll get true regardless
 *
 *	NOTE:	Texcoords are the only part of any geometry which is not assigned by default
 *			since you cannot assume every geometry chunk will have need for texture coordinates
 *			perhaps they only want flat shaded? assigning the memory for a chunk of texture 
 *			coordinates	would be a waste of memory in that situation, in any case, it's not 
 *			as if assigning a chunk of memory once every time you create a geometry chunk 
 *			is time consuming
 *			(wouldnt happen mid-game anyway, would happen whilst initialising a level or something)
 *
 *	Operations:
 *		-#	Release all previously allocated data
 *		-#	Store the number of position and indices
 *		-#	Allocate a buffer to store the index dat
 *		-#	Set the number of components for each position and texture coordinate
 *		-#	Set the vertexpool's initial state to IVertexBuffer::STATERESET
 */
bool OGLDynamicVB::Initialise(unsigned int nv, unsigned int ni, unsigned int nc_p, unsigned int nc_t)
{
	ReleaseAll();

	m_num_vertex	= nv;
	m_num_index		= ni;

	m_index = new unsigned int[ni];
	memset(m_index,0,ni*sizeof(unsigned int));

	SetComponents(nc_p, nc_t);

	m_state	= STATERESET;

	return true;
}

/**	Releases all the stored internal data
 *	
 *	Operation:
 *		-#	Zero's out the number of positions and indices
 *		-#	Zero's out the number of components in each position and texture coordinate
 *		-#	NULL's the position and normal pointers
 *		-#	deletes the index array
 */
void OGLDynamicVB::ReleaseAll(void)
{
	m_num_vertex		= 0;
	m_num_index			= 0;

	m_numcomp_position	= 0;
	m_numcomp_texcoord	= 0;

	m_bytes_position	= 0;
	m_bytes_texcoord	= 0;

	m_position			= NULL;
	m_normal			= NULL;

	delete[] m_index;
	m_index = NULL;
}

/**	Sets the number of components for each position and texture coordinate
 *
 *	@param p	The number of components in each position
 *	@param t	The number of components in each texture coordinate
 *
 *	This method also pre-calculates the number of bytes in each position and texture coordinate too
 */
void OGLDynamicVB::SetComponents(unsigned int p, unsigned int t)
{
	m_numcomp_position	=	p;
	m_numcomp_texcoord	=	t;
	m_bytes_position	=	p	* sizeof(float);
	m_bytes_texcoord	=	t	* sizeof(float);
}

/**	Sets this VertexBuffer's identifiable name
 *
 *	@param n	A string containing the name of the vb
 */
void OGLDynamicVB::SetName(std::string name)
{
	m_name = name;
}

/**	Sets the position data
 *
 *	@param p	A pointer to the new position data
 *
 *	This is just a simple pointer to the data, no copying takes place
 */
void OGLDynamicVB::SetPosition(float *p)
{
	m_position = p;
}

/**	Sets the normal data
 *
 *	@param n	A pointer to the normal data
 *
 *	This is just a simple pointer to the data, no copying takes place
 */
void OGLDynamicVB::SetNormal(float *n)
{
	m_normal = n;
}

/**	Sets up a texture layer
 *
 *	@param layer	The layer to setup
 *	@param tc			An array of texcoord data
 *	@param t			A Texture
 *	
 *	Setting up a texture layer doesnt account for any errors 
 *	that might occur, if texture layer 0 + 2 are setup, but 
 *	there is no layer 1, which *may* cause trouble
 */
void OGLDynamicVB::SetTextureLayer(unsigned int layer, float *tc, ITexture *t)
{
	m_shader.AddLayer(layer,tc,t);
}

/**	Sets the index data for the mesh
 *
 *	@param i	A pointer to the index data
 *
 *	The index data is individual to this mesh, so a copy of the data must be made.
 *	The buffer to contain the index data was created when OGLDynamicVB::Initialise() was called
 *	So no buffer is created here, the data is simply copied into the buffer
 */
void OGLDynamicVB::SetIndex(unsigned int *i)
{
	memcpy(m_index,i,m_num_index*sizeof(unsigned int));
}

/**	Sets the colour to render the mesh in
 *
 *	@param rgba	A Colour4f structure containing the colour to render in
 */
void OGLDynamicVB::SetColour(Colour4f *c)
{
	memcpy(&m_material.colour,c,sizeof(Colour4f));
}

/**	Sets the colour to render the mesh in
 *
 *	@param r	The red component
 *	@param g	The green component
 *	@param b	The blue component
 *	@param a	The Alpha component
 */
void OGLDynamicVB::SetColour(float r, float g, float b, float a)
{
	m_material.colour.r	=	r;
	m_material.colour.g	=	g;
	m_material.colour.b	=	b;
	m_material.colour.a	=	a;
}

/**	Sets the material data
 *
 *	@param m	The material data to copy
 */
void OGLDynamicVB::SetMaterial(Material *m)
{
	memcpy(&m_material,m,sizeof(Material));
}

void OGLDynamicVB::SetSmoothingAngle(float angle)
{
	m_smoothingangle = angle;
}

/**	Retrieves the name of the Vertexbuffer
 *
 *	@returns A string containing the name of the vb
 */
std::string OGLDynamicVB::GetName(void)
{
	return m_name;
}

/**	Retrieves a pointer to the position data
 *
 *	@returns A pointer to the position data
 */
float * OGLDynamicVB::GetPosition(void)
{
	return m_position;
}

/**	Retrieves a pointer to the normal data
 *
 *	@returns A pointer to an array of floating point values, which represents the normal data
 */
float *	OGLDynamicVB::GetNormal(void)
{
	return m_normal;
}

/**	Retrieves a pointer to the texture coordinate data
 *
 *	@returns	A pointer to an array of floating point values, which represents the texture coordinate data
 */
float * OGLDynamicVB::GetTexcoord(unsigned int layer)
{
	TexLayer *tl = m_shader.GetLayer(layer);

	if(tl != NULL) return tl->tc;

	return NULL;
}


/**	Retrieves a pointer to the index data
 *
 *	@returns A pointer to an array of integers, which represent the index data
 */
unsigned int * OGLDynamicVB::GetIndex(void)
{
	return m_index;
}

/**	Retrieves a pointer to the texture assigned
 *
 *	@returns A ITexture object if one has been assigned
 */
ITexture * OGLDynamicVB::GetTexture(unsigned int layer)
{
	TexLayer *tl = m_shader.GetLayer(layer);

	if(tl != NULL) return tl->t;

	return NULL;
}


/**	Retrieves a pointer to a Colour4f structure
 *
 *	@returns A Colour4f pointer
 */
Colour4f * OGLDynamicVB::GetColour(void)
{
	return &m_material.colour;
}

/**	Retrieves a pointer to the material data
 *
 *	@returns A Material object
 */
Material * OGLDynamicVB::GetMaterial(void)
{
	return &m_material;
}

/**	Retrieves the maximum smoothing angle for the mesh
 *
 *	@returns	The maximum smoothing angle
 *
 *	@todo	Explain this in a better way, it's too ambiguous
 */
float OGLDynamicVB::GetSmoothingAngle(void)
{
	return m_smoothingangle;
}

/**	Retrieves The number of indices this object is using
 *
 *	@returns The number of indices allocated
 *
 *	This incidentally is the same as the number of vertices this vertexbuffer is referencing
 */
unsigned int OGLDynamicVB::GetNumIndex(void)
{
	return m_num_index;
}

/**	Renders the VertexBuffer
 *
 *	Operation:
 *		-#	Tests whether the texture state was set
 *		-#	If set, calls ITexture::Set() and sets opengl's texture pointer
 *		-#	Sets the colour to render
 *		-#	Sets the position and normal pointers
 *		-#	Draws the mesh data
 */
void OGLDynamicVB::Render(void)
{
	m_shader.Set();

	glColor4fv((float *)&m_material.colour);

	//	Setup the correct position/normal pointers
	glVertexPointer(m_numcomp_position,GL_FLOAT,0,m_position);
	glNormalPointer(GL_FLOAT,0,m_normal);

	glDrawElements(fusion->Graphics->GetRenderMode(), m_num_index, GL_UNSIGNED_INT, m_index);
}

