#include <VFSPlugin_3DS.h>

/**	3DS Mesh plugin Constructor
 *
 *	Set all the internal data to zero or NULL
 *	Set the extension to "3ds"
 */
VFSPlugin_3DS::VFSPlugin_3DS()
{
	m_type			=	"3ds;";

	m_offset			=	0;
	m_numvertex		=	0;
	m_numpolygons	=	0;
	m_vertexctr		=	0;
	m_length			=	0;
	chunkid			=	0;
	chunklen			=	0;

	m_fileinfo			=	NULL;
	m_polygons		=	NULL;
	m_vertex			=	NULL;	
	m_fileinfo			=	NULL;
	m_buffer			=	NULL;
}

/**	3DS Mesh plugin Deconstructor */
VFSPlugin_3DS::~VFSPlugin_3DS(){}

/**	Retrieves the type of file the plugin handles
 *
 *	@returns	The plugin identifier string
 */
char * VFSPlugin_3DS::Type(void)
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
 *		-#	Reset the plugin ready to read a file
 *		-#	Create a new MeshFileInfo structure to store the mesh data
 *		-#	Read a chunk from the bytestream, if the chunkid is MAIN3DS, this is a valid 3ds file
 *		-#	If valid, Call VFSPlugin_3DS::ReadMain()
 *		-#	Process the Surface data after reading all the file contents
 *		-#	If not valid, delete the mesh data assigned and the FileInfo structure
 *		-#	delete the buffer containing the original file data
 *		-#	Return the fileinfo structure
 */
FileInfo * VFSPlugin_3DS::Read(unsigned char *buffer, unsigned int length)
{
	m_offset		= 0;
	m_buffer		=	buffer;
	m_numvertex	=	0;
	m_fileinfo	=	NULL;
	m_vertexctr	=	0;

	//	Read a chunk, hope it's MAIN3DS
	ReadChunk();

	if(chunkid == MAIN3DS){
		m_fileinfo				=	new MeshFileInfo();
		m_fileinfo->mesh		=	fusion->Mesh->CreateMesh();
		m_fileinfo->filelength	=	length;

		//	YAY! Now read the main chunk
		ReadMain();

		ProcessSurface();
	}

	return m_fileinfo;
}

/**	Writes data to a bytestream
 *
 *	@param data		A FileInfo structure containing the data to write to the bytestream
 *	@param length	The length of the resulting bytestream
 *
 *	@returns	A Bytestream containing the contents of the file
 */
char * VFSPlugin_3DS::Write(FileInfo *data, unsigned int &length)
{
	//	Not implemented
	return NULL;
}

/**	Reads the main 3ds chunk
 *
 *	Read chunks and decode them until you reach the end of the Main chunk
 *	
 *	If Chunk ID is VERSION_3DS:	Read the version of 3ds this file is
 *
 *	If Chunk ID is EDIT_3DS:		Call VFSPlugin_3DS::ReadEditor() to read the editor chunk
 *
 *	If Chunk ID is KEYF_3DS:		Call VFSPlugin_3DS::ReadKeyframe() to read the keyframe data
 *
 *	If Chunk ID is unrecognised:	return from the Main chunk, hence the file is finished reading
 */
void VFSPlugin_3DS::ReadMain(void)
{
	//	Read this chunk until length is zero
	while(true)
	{
		//	Read the next chunkid/chunklen
		ReadChunk();
		switch(chunkid)
		{
			case VERSION_3DS:{
				unsigned long version = ReadLong();
			}break;

			case EDIT_3DS:{
				ReadEditor();
			}break;

			case KEYF_3DS:{
				ReadKeyframe();
			}break;

			default:{
				//	If it's unknown, no one can help you :P
				return;
			}break;
		}
	}
}

/**	Read the Editor chunk
 *
 *	Read chunks and decode them until a chunk is unrecognised then pass it back to the upper level
 *
 *	If Chunk ID is EDIT_CONFIG1:	Call VFSPlugin_3DS::ReadEditorConfig1()
 *
 *	If Chunk ID is EDIT_CONFIG2:	Call VFSPlugin_3DS::ReadEditorConfig2()
 *
 *	If Chunk ID is EDIT_MESHVER:	read the version of the mesh, doesnt seem much 
 *																point to this chunk, since you already have VERSION_3DS in the Main chunk
 *
 *	If Chunk ID is EDIT_OBJECT:		Read the name of the object, call VFSPlugin_3DS::ReadObject()
 *
 *	If Chunk ID is EDIT_MATERIAL:	Call VFSPlugin_3DS::ReadMaterial()
 *
 *	If Chunk is unrecognised:			pass it back to the level above
 */
void VFSPlugin_3DS::ReadEditor(void)
{
	while(true)
	{
		ReadChunk();
		switch(chunkid)
		{
			case EDIT_CONFIG1:
			{
				ReadEditorConfig1();
			}break;

			case EDIT_CONFIG2:
			{
				ReadEditorConfig2();
			}break;

			case EDIT_MESHVER:
			{
				long meshversion = ReadLong();
			}break;

			case EDIT_OBJECT:
			{
				char *objectname = ReadString();

				ReadObject();

				delete objectname;
			}break;

			case EDIT_MATERIAL:
			{
				ReadMaterial();
			}break;

			default:
			{
				m_offset-=6;
				return;
			}break;
		}
	}
}

/**	Reads the keyframe data
 *
 *	NOTE: Not supported yet
 */
void VFSPlugin_3DS::ReadKeyframe(void)
{
	SkipChunk();
}

/**	Read the 1st set of configuration data
 *
 *	read the master scale
 *
 *	NOTE: Not sure what this chunk does exactly, documentation of 3ds is sketchy at best
 */
void VFSPlugin_3DS::ReadEditorConfig1(void)
{
	float masterscale = ReadFloat();
}

/**	Read the 2nd set of configuration data
 *
 *	NOTE: Not supported yet.  Not sure what this chunk does exactly, 
 *				documentation of 3ds is sketchy at best, so skip this for now
 */
void VFSPlugin_3DS::ReadEditorConfig2(void)
{
	SkipChunk();
}

/**	Read a material
 *
 *	Read chunks and decode them until a chunk is reached where it's unrecognised, then pass that back up a level
 *
 *	Create a Surface if the pointer is NULL, this is how the method recognises a new surface or a new component of a current surface
 *
 *	If Chunk ID is MAT_NAME:				Read the material name and set the Surface's name to that
 *
 *	If Chunk ID is MAT_AMBIENT:			Read the colour and gamma settings
 *
 *	If Chunk ID is MAT_DIFFUSE:			Read the colour and gamma, the colour is used as the base colour of the mesh
 *
 *	If Chunk ID is MAT_SPEC:				Read the colour and gamma
 *
 *	If Chunk ID is MAT_SHINY:				Read the percentage glossiness
 *
 *	If Chunk ID is MAT_STRENGTH:		Read how strong the shininess component is
 *
 *	If Chunk ID is MAT_TRANS:				Read how transparent the material is
 *
 *	If Chunk ID is MAT_TRANSFALL:		Read the transparency edge falloff
 *
 *	If Chunk ID is MAT_REFBLUR:			Read the reflective blur component
 *
 *	If Chunk ID is MAT_2SIDED:			Read whether the surface is 2 sided or not
 *
 *	If Chunk ID is MAT_ADDTRANS:		Read the Additive transparency component
 *
 *	If Chunk ID is MAT_SELFILLUM:		Read whether the material is self illuminating
 *
 *	If Chunk ID is MAT_WIRE:				Read whether wireframe is to be shown or not
 *
 *	If Chunk ID is MAT_WIRETHICK:		Read the thickness of the wireframe
 *
 *	If Chunk ID is MAT_FACEMAP:			Read the facemap data
 *
 *	If Chunk ID is MAT_INTRAC:			Carter:	HUH? you speaka de english? DO YOU!!! UNDERSTAND!! THE WORDS!!! that are coming outta my mouth???
 *
 *	If Chunk ID is MAT_SOFTEN:			Jackie:	Pfft, no one understand the words coming outta your mouth anyway
 *
 *	If Chunk ID is MAT_WIRESIZE:		Unknown: SKip chunk
 *
 *	If Chunk ID is MAT_RENDERTYPE:	Read the shading type	
 *
 *	If Chunk ID is MAT_ENDMARKER:		Finish Surface and time to either create another one, or return
 *
 *	If Chunk ID is unknown					No more material properties to add, finish surface and return this chunk to the upper level
 */
void VFSPlugin_3DS::ReadMaterial(void)
{
	IVertexBuffer *vb = NULL;
	Material			*m	= NULL;

	float			colour[3];
	float			gamma[3];
	float			percentage;

	bool			SurfaceNew	= true;

	while(true)
	{
		//	Creates a new surface when a new surface is detected
		if(vb==NULL){
			vb	= m_fileinfo->mesh->AddVertexBuffer();
			m		= vb->GetMaterial();	
		}

		ReadChunk();
		switch(chunkid)
		{
			case MAT_NAME:
			{
				char *name = ReadString();
				vb->SetName(name);
				delete name;
			}break;

			case MAT_AMBIENT:
			{
				ReadColour(colour);
				ReadColour(gamma);
			}break;

			case MAT_DIFFUSE:
			{
				ReadColour((float *)&m->colour);
				ReadColour(gamma);
			}break;

			case MAT_SPEC:
			{
				ReadColour(colour);
				ReadColour(gamma);
			}break;

			case MAT_SHINY:
			{
				ReadPercentage((float *)&m->glossiness);
			}break;

			case MAT_STRENGTH:
			{
				ReadPercentage(&percentage);
			}break;

			case MAT_TRANS:
			{
				ReadPercentage(&percentage);
			}break;

			case MAT_TRANSFALL:
			{
				ReadPercentage(&percentage);
			}break;

			case MAT_REFBLUR:
			{
				ReadPercentage(&percentage);
			}break;

			case MAT_2SIDED:
			{
				SkipChunk();
			}break;

			case MAT_ADDTRANS:
			{
				SkipChunk();
			}break;

			case MAT_SELFILLUM:
			{
				ReadPercentage(&percentage);
			}break;

			case MAT_WIRE:
			{
				SkipChunk();
			}break;
			
			case MAT_WIRETHICK:
			{
				float linethickness = ReadFloat();
			}break;

			case MAT_FACEMAP:
			{
				SkipChunk();
			}break;

			case MAT_INTRAC:
			{
				SkipChunk();
			}break;

			case MAT_SOFTEN:
			{
				SkipChunk();
			}break;

			case MAT_WIRESIZE:
			{
				float wiresize = ReadFloat();
			}break;

			case MAT_RENDERTYPE:
			{
				unsigned short shadingtype = ReadShort();
			}break;

			case MAT_ENDMARKER:
			{
				vb->SetMaterial(m);
				vb = NULL;
			}break;

			default:
			{
				//	Nothing more to add, finish surface and return the chunk to the upper level
				vb->SetMaterial(m);
				m_offset-=6;
				return;
			}break;
		};
	}
}

/**	Reads Object Data
 *
 *	Reads chunks and decode them all until a chunk is unrecognised, then pass it to an upper level
 *
 *	If Chunk ID is OBJ_HIDDEN:		Whether the object is hidden or not.  Skip this chunk
 *
 *	If Chunk ID is OBJ_TRIMESH:		Call VFSPlugin_3DS::ReadMesh() to read the mesh data
 *
 *	If Chunk ID is unrecognised:	Return the chunk to the upper level
 */
void VFSPlugin_3DS::ReadObject(void)
{
	while(true)
	{
		ReadChunk();

		switch(chunkid)
		{
			case OBJ_HIDDEN:
			{
				SkipChunk();
			}break;

			case OBJ_TRIMESH:
			{
				ReadMesh();
			}break;

			default:
			{
				m_offset-=6;
				return;
			}break;
		};
	}
}

/**	Reads Mesh Data
 *
 *	Reads chunks and decodes them all until a chunk is unrecognised, then pass it to an upper level
 *
 *	If Chunk ID is TRI_VERTEXL:		Call VFSPlugin_3DS::ReadVertexData() to read the vertex data
 *
 *	If Chunk ID Is TRI_FACEL1:		Call VFSPlugin_3DS::ReadPolyIndexData() to read the vertex index data for the polygon mesh
 *
 *	If Chunk ID is TRI_FACEL2:		Skip the chunk because I dont know what to do with it
 *
 *	If Chunk ID is TRI_MATL:			Read what polygons are assigned to what surface (build the surfaces vertex/polygon index lists)
 *
 *	Operation:
 *		-#	Read the name of the material
 *		-#	Loop through the surfaces until you find the surface with the same name
 *		-#	call VFSPlugin_3DS::AssignVertexData(Surface *s) to assign the vertex data to the surface
 *
 *	If Chunk ID is TRI_SMOOTH:		Read the smoothing group block, skip it
 *
 *	If Chunk ID is TRI_LOCAL:			Read the Local Axis (could be useful, but skip it for now)
 *
 *	If Chunk ID is unrecognised:	Pass it back to the upper level
 */
void VFSPlugin_3DS::ReadMesh(void)
{
	while(true)
	{
		ReadChunk();
		switch(chunkid)
		{
			case TRI_VERTEXL:
			{
				ReadVertexData();
			}break;

			case TRI_FACEL1:
			{
				ReadPolyIndexData();
			}break;

			case TRI_FACEL2:
			{
				SkipChunk();
			}break;

			case TRI_MATL:
			{
				Mesh	*m		= m_fileinfo->mesh;
				char	*name = ReadString();

//				for(int a=0;a<m->NumSurface();a++){
				for(int a=0;a<m->NumVertexBuffer();a++){
					IVertexBuffer *v = m->GetVertexBuffer(a);

					if(strcmp(v->GetName(),name) == 0){
						AssignVertexData(v);
					}
				}
				delete name;
			}break;

			case TRI_SMOOTH:
			{
				SkipChunk();
			}break;

			case TRI_LOCAL:
			{
				SkipChunk();
			}break;

			default:
			{
				m_offset-=6;
				return;
			}break;
		};
	}
}

/**	Reads the Vertex data
 *
 *	Operation:
 *		-#	Read the number of vertices in this vertex chunk
 *		-#	Assign a new vertex array to hold the extra data
 *		-#	Copy the old vertex data to the new vertex array
 *		-#	delete the old vertex array and assign it's pointer to the new array
 *		-#	Read a new vertex and add it to the vertex array, loop until you've read all the vertex data
 *		-#	Update the total number of vertices
 */
void VFSPlugin_3DS::ReadVertexData(void)
{
	int temp = ReadShort();
	Vertex3f *v = new Vertex3f[m_numvertex+temp];
	
	if(m_numvertex > 0)	memcpy(v,m_vertex,m_numvertex*sizeof(Vertex3f));

	delete[] m_vertex;
	m_vertex = v;
	v = &m_vertex[m_numvertex];
	
	for(int a=0;a<temp;a++){
		Vertex3f *tv = ReadVertex();
		memcpy(v,tv,sizeof(Vertex3f));
		v++;

		delete tv;
	}

	m_numvertex += temp;
}

/**	Read the polygon vertex index information 
 *
 *	@todo	Find a better short title for this method =]
 *
 *	Operation:
 *		-#	Delete the old polygon array
 *		-#	Read the number of polygons in this polygon chunk
 *		-#	Make a new polygon array of that size
 *		-#	Loop for the number of polygons
 *		-#	Read 4 shorts, first three are vertex indices, last one is for knowing how to calculate the normals
 *		-#	Assign the information to the polygon structure
 */
void VFSPlugin_3DS::ReadPolyIndexData(void)
{
	unsigned short ind[4];
	int a;

	if(m_polygons != NULL)	for(a=0;a<m_numpolygons;a++)  delete m_polygons[a].index;
	delete[] m_polygons;

	m_numpolygons	=	ReadShort();
	m_polygons		=	new TempPolygon[m_numpolygons];

	for(a=0;a<m_numpolygons;a++){
		ind[0] = ReadShort();
		ind[1] = ReadShort();
		ind[2] = ReadShort();
		ind[3] = ReadShort();
		
		m_polygons[a].numvertex = 3;
		m_polygons[a].index = new int[3];
		
		if(ind[0] < m_numvertex) m_polygons[a].index[0] = ind[0];
		if(ind[1] < m_numvertex) m_polygons[a].index[1] = ind[1];
		if(ind[2] < m_numvertex) m_polygons[a].index[2] = ind[2];
	}
}

/**	Assign the correct index information to a surface
 *
 *	@param	s	The surface to assign the index data to
 *
 *	Operation:
 *		-#	Read the number of polygons in this surface
 *		-#	Initialise the surface
 *		-#	Retrieve a pointer to the index information in the surface
 *		-#	Loop for that many polygons
 *		-#	Read the polygon id
 *		-#	Read that polygon's index information into the surface index array
 */
void VFSPlugin_3DS::AssignVertexData(IVertexBuffer *v)
{
	int a;

	Mesh *m = m_fileinfo->mesh;
		
	int numindex = ReadShort()*3;

	v->Initialise(m->GetNumVertex(),numindex,3,2);
	int *i = v->GetIndex();

	for(a=0;a<numindex/3;a++){		
		unsigned short polyid = ReadShort();

		*i++ = m_vertexctr + m_polygons[polyid].index[0];
		*i++ = m_vertexctr + m_polygons[polyid].index[1];
		*i++ = m_vertexctr + m_polygons[polyid].index[2];
	}

	m_vertexctr = m_numvertex;

	if(m_polygons != NULL)	for(a=0;a<m_numpolygons;a++)  delete m_polygons[a].index;
	delete[] m_polygons;
	m_polygons = NULL;
}

/**	Process a Surfaces information
 *
 *	Operation:
 *		-#	Initialise a mesh object with the correct number of vertices
 *		-#	Retrieve a pointer to the vertex information in the mesh object
 *		-#	Copy the current vertex chunk into the mesh object
 *		-#	Clean up any temporary polygon and vertex data stored
 *		-#	Automatically generate normals for all the polygons
 */
void VFSPlugin_3DS::ProcessSurface(void)
{
	Mesh *m = m_fileinfo->mesh;

	m->Initialise(m_numvertex);
	m->SetPosition(m_vertex);
	m->Finalise();
	m->GenerateNormals();
}

/**	Read a 3ds chunk */
void VFSPlugin_3DS::ReadChunk()
{
	chunkid		= (unsigned short)	ReadShort();
	chunklen	= (unsigned long)		ReadLong();
}

/**	Skip a 3ds chunk */
void VFSPlugin_3DS::SkipChunk(void)
{
	m_offset+=(chunklen-6);
}

/**	Read a string (unk bytes) */
char * VFSPlugin_3DS::ReadString(void)
{
	char c,a=0,*string;
	
	do{	c = ReadChar();	a++; }while(c!=0);

	m_offset-=a;
	
	if(a>0){
		string = new char[a];
		memcpy(string,&m_buffer[m_offset],a);
		m_offset+=a;
	}

	return string;
}

/**	Read a long (4 bytes) */
long VFSPlugin_3DS::ReadLong(void)
{
	long l;
	memcpy(&l,&m_buffer[m_offset],sizeof(long));
	m_offset+=sizeof(long);

	return l;
}

/**	Read a short (2 bytes) */
short VFSPlugin_3DS::ReadShort(void)
{
	short s;
	memcpy(&s,&m_buffer[m_offset],sizeof(short));
	m_offset+=sizeof(short);

	return s;
}

/**	Read a float (4 bytes) */
float VFSPlugin_3DS::ReadFloat(void)
{
	float f;
	memcpy(&f,&m_buffer[m_offset],sizeof(float));
	m_offset+=sizeof(float);

	return f;
}

/**	Read a char (1 byte) */
char VFSPlugin_3DS::ReadChar(void)
{
	char c;
	memcpy(&c,&m_buffer[m_offset],sizeof(char));
	m_offset+=sizeof(char);

	return c;
}

/**	Read a Colour (12 bytes) */
void VFSPlugin_3DS::ReadColour(float *rgb)
{
	ReadChunk();
	switch(chunkid)
	{
		case RGB_BYTE:
		case RGB_BYTE_GAMMA:
		{
			rgb[0] = (float)((unsigned short)ReadChar())/255;
			rgb[1] = (float)((unsigned short)ReadChar())/255;
			rgb[2] = (float)((unsigned short)ReadChar())/255;
		}break;

		case RGB_FLOAT:
		case RGB_FLOAT_GAMMA:
		{
			rgb[0] = ReadFloat();
			rgb[1] = ReadFloat();
			rgb[2] = ReadFloat();
		}break;
	}
}

/**	Read a percentage (2bytes/short or 4 bytes/float) */
void VFSPlugin_3DS::ReadPercentage(float *percentage)
{
	ReadChunk();
	switch(chunkid)
	{
		case PERCENT_INT:		*percentage = (float)	ReadShort();	break;
		case PERCENT_FLOAT:	*percentage =					ReadFloat();	break;
	};	
}

/**	Read a Vertex (12bytes) */
Vertex3f * VFSPlugin_3DS::ReadVertex(int pad)
{
	Vertex3f *v = new Vertex3f;
	memcpy(v,&m_buffer[m_offset],sizeof(Vertex3f));
	m_offset+=sizeof(Vertex3f) + pad;

	return v;
}
