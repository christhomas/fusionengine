#include <mesh/Mesh.h>
#include <Fusion.h>

/**	Mesh Constructor 
 *
 *	Set all the internal data to zero/NULL
 */
Mesh::Mesh()
{
	m_numvertex		=	0;
	m_numvb			=	0;

	m_position		=	NULL;
	m_normal			=	NULL;
	m_texcoord		=	NULL;
	m_vertexbuffer		=	NULL;
}

/**	Mesh Deconstructor
 *
 *	Operation:
 *		-#	Delete all the assigned surfaces
 *		-#	Delete the position data
 *		-#	Delete the normal data
 */
Mesh::~Mesh()
{
	for(int a=0;a<m_numvb;a++) delete m_vertexbuffer[a];
	delete[] m_vertexbuffer;

	delete[] m_position;
	delete[] m_normal;
	delete[] m_texcoord;
}

/**	Initialise the mesh
 *
 *	@param numvertex	The number of vertices this mesh will hold
 *
 *	Operation:
 *		-#	Delete any old position/normal data
 *		-#	Allocate new position/normal arrays to store the new position/normal data
 *		-#	Initialise the arrays to zero
 */
void Mesh::Initialise(int numvertex)
{
	m_numvertex	= numvertex;

	delete[] m_position;
	delete[] m_normal;
	delete[] m_texcoord;

	m_position	=	new Vertex3f[numvertex];
	m_normal		=	new Vertex3f[numvertex];
	m_texcoord	=	new Vertex2f[numvertex];

	memset(m_position,0,numvertex*sizeof(Vertex3f));
	memset(m_normal,	0,numvertex*sizeof(Vertex3f));
	memset(m_texcoord,0,numvertex*sizeof(Vertex2f));
}

/**	Renders the Mesh
 *
 *	Loop through each surface, calling Surface::Render() on each one
 */
void Mesh::Render(void)
{	
	for(int a=0;a<m_numvb;a++) m_vertexbuffer[a]->Render();
}

/**	Adds a VertexBuffer to the mesh object
 *
 *	@returns	A VertexBuffer object
 *
 *	Operation:
 *		-#	Create a new vertexbuffer
 *		-#	Retrieve it's material data
 *		-#	Set the material colour to pure white (default colour)
 *		-#	Calls Mesh::AddVertexBuffer(IVertexBuffer *) to append the vertexbuffer into the mesh object
 */
IVertexBuffer * Mesh::AddVertexBuffer(void)
{
	IVertexBuffer *vb = fusion->Graphics->CreateVertexBuffer(IVertexBuffer::DYNAMIC);

	Material *m = vb->GetMaterial();

	m->colour.r	= 1;
	m->colour.g	= 1;
	m->colour.b	= 1;
	m->colour.a	= 1;

	AddVertexBuffer(vb);

	return vb;
}

/**	Adds a VertexBuffer to the mesh object
 *
 *	@param s	The vertexbuffer to add to the mesh
 *
 *	Operation:
 *		-#	Increase the number of IVertexBuffer pointers
 *		-#	Allocate a larger array of IVertexBuffer pointers
 *		-#	Copy all the existing vertexbuffer pointers to the new array
 *		-#	Delete the old vertexbuffer pointer array
 *		-#	Reset the vertexbuffer pointer to the new array
 *		-#	Assign the next vertexbuffer to the vertexbufferbeing passed into the method
 */
void Mesh::AddVertexBuffer(IVertexBuffer *v)
{
	IVertexBuffer **tvb = new IVertexBuffer *[m_numvb+1];

	memcpy(tvb,m_vertexbuffer,m_numvb*sizeof(IVertexBuffer *));

	delete[] m_vertexbuffer;

	m_vertexbuffer = tvb;

	m_vertexbuffer[m_numvb] = v;

	m_numvb++;
}

/**	Retrieves the number of vertexbuffers in the mesh
 *
 *	@returns	The number of vertexbuffers stored
 */
int Mesh::NumVertexBuffer(void)
{
	return m_numvb;
}

/**	Retrieves a Vertexbuffer object
 *
 *	@param index	The vertexbuffer to retrieve
 *
 *	@returns	A IVertexBuffer object or NULL, depending on whether the vertexbuffer could be found
 */
IVertexBuffer * Mesh::GetVertexBuffer(unsigned int index)
{
	if(index < m_numvb) return m_vertexbuffer[index];

	return NULL;
}

/**	Copies a mesh object
 *
 *	@param m	The mesh object to copy
 *
 *	@todo	Implement this function correctly
 */
void Mesh::operator= (Mesh &m){}

/**	Scales a mesh object
 *
 *	@param x	The x scalar
 *	@param y	The y scalar
 *	@param z	The z scalar
 */
void Mesh::Scale(float x, float y, float z)
{
}

/**	Copies new position data
 *
 *	@param p	The vertex data to copy
 */
void Mesh::SetPosition(Vertex3f *p)
{
	if(m_position != NULL && p != NULL)	memcpy(m_position,p,m_numvertex*sizeof(Vertex3f));
}

/**	Copies new normal data
 *
 *	@param n	The normal data to copy
 */
void Mesh::SetNormal(Vertex3f *n)
{
	if(m_normal != NULL && n != NULL)	memcpy(m_normal,n,m_numvertex*sizeof(Vertex3f));
}

/**	Copies new texture coordinate data
 *
 *	@param t	The texture coordinate data
 */
void Mesh::SetTexcoord(Vertex2f *t)
{
	if(m_texcoord != NULL && t != NULL) memcpy(m_texcoord,t,m_numvertex*sizeof(Vertex2f));
}

/**	Retrieves the position data
 *
 *	@returns	The pointer to the position data
 */
Vertex3f * Mesh::GetPosition(void)
{
	return m_position;
}

/**	Retrieves the normal data
 *
 *	@returns	The pointer to the normal data
 */
Vertex3f * Mesh::GetNormal(void)
{
	return m_normal;
}

/**	Retrieves the texture coordinate data
 *
 *	@returns	A pointer to the texture coordinate data
 */
Vertex2f * Mesh::GetTexcoord(void)
{
	return m_texcoord;
}

/**	Retrieves the number of vertices in the mesh
 *
 *	@returns	The number of vertices this mesh holds or has room for
 */
int Mesh::GetNumVertex(void)
{
	return m_numvertex;
}

/**	Performs some final optimisations and reorganisations on the mesh
 *
 *	Basic Overview:
 *	@todo	Complete an explanation of why this method needs to be called for each mesh
 *
 *	Explanation 1:
 *		Counting the number of positions/normals required for the final mesh
 *		-#	Reset the array of vertex counters
 *		-#	Loop through the surfaces
 *		-#	If the Maximum smoothing angle of a surface is 0, the surface is flat shaded so just add the number of indices in the surface to the total vertex count
 *		-#	Otherwise, loop through the indices setting the vertex counter to 1, this will act as a bitfield so counting the number of shared vertices later is easy
 *		
 *		This results in a total vertex count for all the flat shaded surfaces and a bitfield of 1's for the shared vertices, to finalise the count
 *		simply loop through the vertex counter array and add the number (which will be a 1 or 0) to the totalvertex counter, this will yield the correct
 *		number of positions/normals for the entire mesh
 *
 *	Explanation 2:
 *		Now the vertices have been counted, the position/normal arrays must be enlarged to cope with the extra data
 *		-#	Delete the original vertex counter array
 *		-#	Allocate a larger position/normal/vertex counter arrays
 *		-#	Set all the contents to zero
 *		-#	Copy the m_position data from the old array into the new array
 *		-#	Delete the old m_position array
 *		-#	Reset the position/normal array pointers to the new array pointers
 *
 *		This will mean larger arrays that are required to store all the additional position data that will be generated
 *
 *	Explanation 3:
 *		Now that there is the correct amount of array space to hold all the data, process the flat shaded surfaces.  
 *		To do so, three things must be done
 *		-#	Identify vertices that are shared across surfaces (either flat to smooth shaded or flat to flat shaded surfaces)
 *		-#	Once they are identified, eliminate the sharing by duplication
 *		-#	Then eliminate intersurface vertex sharing
 *
 *		The effect will be each vertex in the flat shaded surface will not be shared with any other polygon 
 *		(hence when m_normal generation begins, everything will turn out ok)
 *		
 *	Explanation 4:
 *		Creating a bitfield to identify vertices shared across surfaces
 *		-#	Increase the count of any vertex to 1
 *		-#	Loop through all the other Surface objects
 *		-#	If a vertex counter is 1 increase it to 2, but leaving 0 alone, giving you a bitfield of 0's, 1's + 2's
 *		
 *		0 = vertex not used
 *		1 = vertex used by surface but not shared
 *		2 = vertex used by surface AND is shared
 *
 *	Explanation 5:
 *		Now the vertices have been identified, duplicate them so they are no longer shared
 *		-#	Loop through the vertex counters
 *		-#	if you can find a 2 duplicate it
 *		-#	Store the vertex id
 *		-#	loop through the surface indices and update all the same vertices to the same vertex id
 *
 *		Now all the cross surface sharing is eliminated, but vertices are still shared on an inter-surface basis
 *
 *	Explanation 6:
 *		Duplicate the vertex data in flat shaded surfaces, so they dont share vertices across polygons
 *		-#	Loop through the index list, incrementing the vertex counter for each vertex as it's used
 *		-#	Now there is a counter for each vertex and how many polygons it's shared between
 *		-#	Loop through the index list
 *		-#	If the counter is greater than 1
 *		-#	Duplicate the vertex, update the index, decrement the vertex counter
 *		-#	Repeat until the index is 1
 *
 *		Therefore no vertices are no longer shared
 */
void Mesh::Finalise(void)
{
	int			a,b,c;
	
	int			totalvertex = 0;					//	Total number of vertices in this Surface object
	
	int			*vc		=	new int[m_numvertex];	//	Array of vertex ctrs

	int			nvb		=	m_numvb;			//	Number of Surface objects

	int			*si		=	NULL;				//	Source Index list
	int			*di		=	NULL;				//	Dest Index list

	int			sni		=	0;					//	Number of Source Indices

	IVertexBuffer	*src		=	NULL,				//	Source vb
				*dest	=	NULL;				//	Destination vb

	/*	See Explanation 1 */
	memset(vc,0,m_numvertex*sizeof(int));

	for(a=0;a<nvb;a++){
		src = m_vertexbuffer[a];
		sni = src->GetNumIndex();
		si	=	src->GetIndex();

		if(src->GetSmoothingAngle() == 0){
			totalvertex += sni;
		}else{
			for(b=0;b<sni;b++) vc[si[b]] = 1;
		}
	}

	for(a=0;a<m_numvertex;a++)	totalvertex += vc[a];

	/*	See Explanation 2 */
	if(totalvertex > m_numvertex)
	{
		Vertex3f *tp = new Vertex3f[totalvertex];
		Vertex3f *tn = new Vertex3f[totalvertex];
		Vertex2f *tt = new Vertex2f[totalvertex];
		
		delete[] vc;
		vc = new int[totalvertex];

		memset(tp,0,totalvertex*sizeof(Vertex3f));
		memset(tn,0,totalvertex*sizeof(Vertex3f));
		memset(tt,0,totalvertex*sizeof(Vertex2f));

		if(m_position	!= NULL)	memcpy(tp,m_position,	m_numvertex	*	sizeof(Vertex3f));
		if(m_normal	!= NULL)	memcpy(tn,m_normal,	m_numvertex	*	sizeof(Vertex3f));
		if(m_texcoord	!= NULL)	memcpy(tt,m_texcoord,	m_numvertex	*	sizeof(Vertex2f));

		delete[] m_position;
		delete[] m_normal;
		delete[] m_texcoord;

		m_position	=	tp;
		m_normal		=	tn;
		m_texcoord	=	tt;
	}else{
		totalvertex = m_numvertex;
	}

	/*	See Explanation 3 */
	for(a=0;a<nvb;a++) if(m_vertexbuffer[a]->GetSmoothingAngle() == 0){
		src	=	m_vertexbuffer[a];
		si	=	src->GetIndex();
		sni	=	src->GetNumIndex();

		memset(vc,0,totalvertex*sizeof(int));

		/*	See Explanation 4 */
		for(b=0;b<sni;b++)	vc[si[b]] = 1;

		for(b=0;b<nvb;b++)	if(a != b){
			dest = m_vertexbuffer[b];
			di = dest->GetIndex();

			for(c=0;c<dest->GetNumIndex();c++)	if(vc[di[c]] == 1)	vc[di[c]] = 2;
		}

		/*	See Explanation 5 */
		for(b=0;b<sni;b++)	if(vc[si[b]] == 2){	//	if ctr == 2, vertex is cross surface shared
			memcpy(&m_position[m_numvertex],	&m_position[si[b]],	sizeof(Vertex3f));
			memcpy(&m_normal[m_numvertex],	&m_normal[si[b]],	sizeof(Vertex3f));
			memcpy(&m_texcoord[m_numvertex],	&m_texcoord[si[b]],	sizeof(Vertex2f));

			for(c=0;c<sni;c++)	if(si[b] == si[c])	si[c] = m_numvertex++;

			vc[si[b]] = 0;
		}

		/*	See Explanation 6 */
		memset(vc,0,totalvertex*sizeof(int));

		for(b=0;b<sni;b++)	vc[si[b]]++;

		for(b=0;b<sni;b++)	if(vc[si[b]] > 1){
			memcpy(&m_position[m_numvertex],	&m_position[si[b]],	sizeof(Vertex3f));
			memcpy(&m_normal[m_numvertex],	&m_normal[si[b]],	sizeof(Vertex3f));
			memcpy(&m_texcoord[m_numvertex],	&m_texcoord[si[b]],	sizeof(Vertex2f));

			vc[si[b]]--;

			si[b] = m_numvertex++;
		}
	}

	delete[] vc;
}

/**	Generates m_normals for the mesh data it contains
 *
 *	@param i	The first m_position index
 *	@param j	The second m_position index
 *	@param k	The third m_position index
 *
 *	Basic Overview:
 *	This method basically allows the generation of m_normals which would suit most needs in most situations
 *	If the application requires customised m_normals which are not what is m_normally expected, then this method
 *	will not generate the m_normals the application requires, the application will have to supply it's own
 *	method of generating m_normals
 *
 *	This method allows you to specify which order each triangles 
 *	vertices are selected so when the m_normal is calcaulated, it's calculated correctly
 *
 *	The method is documented in chunks, each chunk has an explanation, 
 *	so best to open the documentation and code and read it side by side, 
 *	the explanations will make more sense since this is quite a complex 
 *	operation involving a bit of logic
 *
 *	Explanation 1:
 *		-#	Loop through all the surfaces
 *		-#	Retrieve the index pointer for each surface
 *		-#	Each triplet of indices are a polygon
 *		-#	use i/j/k to define which indices are used in what order
 *		-#	Generate two vectors from the three vertices
 *		-#	Calculate the cross product of those two vertices
 *		-#	m_normalise the result to produce a m_normal
 *		-#	Add the m_normal to the appropriate m_normals in the mesh object
 *
 *	Explanation 2:
 *		-#	Loop through all the Surfaces
 *		-#	For each surface, loop through all the indices
 *		-#	Make a Vector3d object for each m_normal using each triple of indices
 *		-#	Calculate the length of the m_normal
 *		-#	Divide the m_normal by the length
 *		-#	Once all is calculated, update the vertex/m_normal array pointers
 *
 *	Now the mesh has perfectly generated m_normals that should suit almost any mesh
 */
void Mesh::GenerateNormals(int i, int j, int k)
{
	int			a,b,c;

	int			nvb	=	m_numvb;	//	Number of Surface objects

	int			*si	=	NULL;		//	Source Index list

	int			sni	=	0;			//	Number of Source Indices

	IVertexBuffer	*src	=	NULL;		//	Source vertexbuffer ptr

	memset(m_normal,0,m_numvertex*sizeof(Vertex3f));

	/*	See Explanation 1 */
	for(a=0;a<nvb;a++)
	{
		src = m_vertexbuffer[a];
		sni	= src->GetNumIndex();
		si	= src->GetIndex();
		Vertex3f			*v[3];
		Maths::Vector	va,vb;

		for(b=0;b<sni;b+=3){
			//	Find three vertices of this polygon
			v[0] = &m_position[si[b+i]];
			v[1] = &m_position[si[b+j]];
			v[2] = &m_position[si[b+k]];

			//	Generate two vectors within the polygon
			va.Set(v[1]->x-v[0]->x, v[1]->y-v[0]->y, v[1]->z-v[0]->z);
			vb.Set(v[2]->x-v[0]->x, v[2]->y-v[0]->y, v[2]->z-v[0]->z);

			//	Cross the two vectors, then m_normalise it
			va = va.CrossProduct(vb);
			va /= va.Length();
			
			for(c=0;c<3;c++){

				m_normal[si[b+c]].x	+=	va.x;
				m_normal[si[b+c]].y	+=	va.y;
				m_normal[si[b+c]].z	+=	va.z;
			}
		}
	}

	/*	See Explanation 2 */
	for(a=0;a<nvb;a++)
	{
		src = m_vertexbuffer[a];
		sni	= src->GetNumIndex();
		si	= src->GetIndex();

		for(b=0;b<sni;b+=3)	for(c=0;c<3;c++){
			Maths::Vector v(m_normal[si[b+c]].x,m_normal[si[b+c]].y,m_normal[si[b+c]].z);

			float length = v.Length();
				
			m_normal[si[b+c]].x	/= length;
			m_normal[si[b+c]].y	/= length;
			m_normal[si[b+c]].z	/= length;
		}

		src->SetPosition((float *)m_position);
		src->SetNormal((float *)m_normal);
	}
}

