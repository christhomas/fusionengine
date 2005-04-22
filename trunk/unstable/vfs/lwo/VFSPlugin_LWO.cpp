#include <VFSPlugin_LWO.h>
#include <Fusion.h>

Fusion *fusion;

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	
	#define LIBLWO_API extern "C" __declspec(dllexport)
	
	BOOL APIENTRY DllMain(HANDLE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved){return TRUE;}
#else
	#define LIBLWO_API extern "C"
#endif

/**	Creates the LWO Plugin
 *	@ingroup	VFSPlugin_LWO_Group
 *
 *	@param	f	The Fusion object, if this plugin needs to call fusion for something
 *
 *	@returns	A VFSPlugin object for this plugin type
 *
 *	The plugin function is simple, each time called
 *	create the appropiate plugin, increment the counter
 *	so next time it's called, it'll create the NEXT plugin
 *	then when you have no more plugins to create, return NULL
 */
LIBLWO_API VFSPlugin * CreatePlugin(Fusion *f)
{
	static int count = 0;

	fusion = f;

	if(count == 0){
		count++;
		return new VFSPlugin_LWO();
	}

	return NULL;
}

static void ReverseByteOrder(char *bp, int size, int count)
{
	int a,b,c,baseoffset;
	char temp;

	for(a=0,baseoffset=0;a<count;a++,baseoffset=a*size)	for(b=0,c=size-1;c>=size/2;b++,c--)
	{
		temp							= bp[baseoffset+b];
		bp[baseoffset+b]	= bp[baseoffset+c];
		bp[baseoffset+c]	= temp;
	}
}

//=========================
//	VFSPlugin_LWO Methods
//=========================
VFSPlugin_LWO::VFSPlugin_LWO()
{
	m_type			=	"lwo;";
	m_offset		=	0;
	m_fileinfo	=	NULL;
	m_vb				=	NULL;
}

VFSPlugin_LWO::~VFSPlugin_LWO()
{
}

char * VFSPlugin_LWO::Type(void)
{
	return m_type;
}

FileInfo * VFSPlugin_LWO::Read(unsigned char *buffer, unsigned int length)
{
	m_offset	= 0;
	m_buffer	= buffer;
	m_length	= length;

	m_fileinfo							=	new MeshFileInfo();
	m_fileinfo->mesh				=	fusion->Mesh->CreateMesh();
	m_fileinfo->filelength	=	m_length;

	//	Read the FORM tag
	if(ReadLong() != ID_FORM)	return NULL;

	//	Read the entire data size
	long chunklen = ReadLong()-4;

	//	Read the LWO2 tag
	if(ReadLong() != ID_LWO2)	return NULL;

	//	Read chunks
	while(chunklen > 0)	chunklen -= ReadChunk();

	//	Process Surface (turn it into a useable mesh)
	ProcessSurface();

	//	Return a ptr to the file info
	return m_fileinfo;	
}

char * VFSPlugin_LWO::Write(FileInfo *data, unsigned int &length)
{
	//	Not Implemented
	return NULL;
}

//======================
//	Lightwave Methods
//======================
long VFSPlugin_LWO::ReadChunk(void)
{
	long id		= ReadLong();
	long len	= ReadLong();

	switch(id)
	{
		case ID_TAGS:	{	read_tags(len);			}break;
		case ID_LAYR:	{	read_layr(len);			}break;
		case ID_PNTS:	{	read_pnts(len);			}break;
		case ID_BBOX:	{	read_bbox(len);			}break;
		case ID_POLS:	{	read_pols(len);			}break;
		case ID_PTAG:	{	read_ptag(len);			}break;
		case ID_SURF:	{	read_surf(len);			}break;
		case ID_CLIP:	{	read_clip(len);			}break;
		case ID_ENVL:	{	read_envl(len);			}break;
		case ID_VMAP:	{	read_vmap(len);			}break;
		default:			{	read_unkn(id,len);	}break;
	}

	return len+8;
}

long VFSPlugin_LWO::ReadSubChunk(void)
{
	long	id	= ReadLong();
	short	len	= ReadShort();

	switch(id)
	{
		case ID_STIL:	{	read_unkn(id,len);	}break;
		case ID_COLR:	{	read_colr(len);			}break;
		case ID_DIFF:	{	read_diff(len);			}break;
		case ID_LUMI:	{	read_lumi(len);			}break;
		case ID_SPEC:	{	read_spec(len);			}break;
		case ID_REFL:	{	read_refl(len);			}break;
		case ID_TRAN:	{	read_tran(len);			}break;
		case ID_TRNL:	{	read_trnl(len);			}break;
		case ID_GLOS:	{	read_glos(len);			}break;
		case ID_SHRP:	{	read_shrp(len);			}break;
		case ID_SMAN:	{	read_sman(len);			}break;
		case ID_BUMP:	{	read_bump(len);			}break;
		case ID_SIDE:	{	read_unkn(id,len);	}break;
		case ID_BLOK:	{	read_unkn(id,len);	}break;
		case ID_IMAP:	{	read_unkn(id,len);	}break;
		case ID_TMAP:	{	read_unkn(id,len);	}break;
		case ID_IMAG:	{	read_unkn(id,len);	}break;
		case ID_CHAN:	{	read_unkn(id,len);	}break;
		default:			{	read_unkn(id,len);	}break;

	};

	return len + 6;
}

void VFSPlugin_LWO::read_unkn(long id,long length)
{
	m_offset+=length;
}

//=============================
//	Chunks
//=============================

void VFSPlugin_LWO::read_tags(long length)
{
	//	Find out whether you need to read a pad byte
	bool NeedPad = false;

	if(length & 1)	NeedPad = true;

	char *tagstring;

	do{
		// read list of null terminated strings
		long stringlength = 0;
		tagstring = ReadString(&stringlength);
		length -= stringlength;

		//	Delete the tag's temp memory
		delete tagstring;
	}while(length!=0);
	
	if(NeedPad == true) ReadPad();
}

void VFSPlugin_LWO::read_layr(long length)
{
	//	Read the layer number
	short layernumber = ReadShort();
	length-=sizeof(layernumber);

	//	Read the layer flags
	short layerflags = ReadShort();
	length-=sizeof(layerflags);

	//	Read the layer pivot axis
	Vertex3f *v = ReadVertex();
	length-=sizeof(Vertex3f);

	//	Read the layer name
	long stringlength;
	char *layername = ReadString(&stringlength);
	length-=stringlength;

	//	Read the parent layer number
	short parentid = -1;
	if(length>=sizeof(parentid)) parentid = ReadShort();

	//	Clean up any temporary memory
	delete v;
	delete layername;
}

void VFSPlugin_LWO::read_pnts(long length)
{
	//	Calculate the number of vertex in the mesh
	int numvertex = length/sizeof(Vertex3f);

	Mesh *m = reinterpret_cast<Mesh *>(m_fileinfo->mesh);
	m->Initialise(numvertex);

	Vertex3f *v = new Vertex3f[numvertex];
	Vertex3f *temp;

	//	Read all the vertices from the file data
	for(int a=0;a<numvertex;a++){
		temp = ReadVertex();

		//	Store the vertex data
		v[a].x	= temp->x;
		v[a].y	= temp->y;
		v[a].z	= temp->z;
	
		//	Delete the temporary vertex data
		delete temp;
	}

	m->SetPosition(v);

	delete[] v;
}

void VFSPlugin_LWO::read_bbox(long length)
{
	//	Read the min/max constraints of the mesh
	Vertex3f *min	= ReadVertex();
	Vertex3f *max	= ReadVertex();

	//	Dont use them yet, so delete it's temporary memory
	delete min;
	delete max;
}

void VFSPlugin_LWO::read_pols(long length)
{
	//	Read what type of polygon mesh this is
	long chunkid = ReadLong();
	length-=sizeof(long);

	//	Determine which method to use in reading the polygon data
	switch(chunkid)
	{
		case ID_FACE:	{	read_face(length);					}break;
		case ID_CURV:	{	read_curv(length);					}break;
		case ID_PTCH:	{	read_ptch(length);					}break;
		case ID_MBAL:	{	read_mbal(length);					}break;
		case ID_BONE:	{	read_bone(length);					}break;
		default:			{	read_unkn(chunkid,length);	}break;
	};
}

void VFSPlugin_LWO::read_ptag(long length)
{
	//	Read what type the tags are	(SURF/PART/SMGP)
	long type = ReadLong();
	length-=sizeof(long);

	long polygonid;
	long tag;
	long vxlength;

	//	Read until there are no more tags
	while(length>0)
	{
		//	Read what polygon this tag belongs to and what tag to apply to the polygon
		polygonid	=	ReadVariableLength(&vxlength);
		tag				=	ReadShort();

		//	Store the polygon id and tag data
		polygontags.push_back(tag);
		polygontags.push_back(polygonid);

		length-=(vxlength+sizeof(short));

		//	types:
		//	SURF	Surface
		//	PART	Part
		//	SMGP	Smoothing Group
	}
}

void VFSPlugin_LWO::read_surf(long length)
{
	long stringlength;

	//	Read the surface name
	char *name = ReadString(&stringlength);
	length-=stringlength;

	//	Not sure what the source means but read it anyway
	char *source = ReadString(&stringlength);
	length-=stringlength;

	//	Build the surface
	m_vb = m_fileinfo->mesh->AddVertexBuffer();

	//	Store the surface name
	m_vb->SetName(name);

	//	Read all the subchunks surfaces has
	while(length > 0) length -= ReadSubChunk();

	//	Delete any temporary memory
	delete name;
	delete source;
}

void VFSPlugin_LWO::read_clip(long length)
{
	//	Dont know, dont care, do it later....
	read_unkn(ID_CLIP,length);
}

void VFSPlugin_LWO::read_envl(long length)
{
	//	Skip over the envelope data
	read_unkn(ID_ENVL,length);
}

void VFSPlugin_LWO::read_vmap(long length)
{
	//	Skip over the vmap data
	read_unkn(ID_VMAP,length);
}

//=========================
//	SubChunks
//=========================

void VFSPlugin_LWO::read_colr(long length)
{
	//	Read the base colour of the surface and store it in the material object
	Colour4f *c = ReadColour(3);

	m_vb->SetColour(c);

	//	Read the envelope
	long vxlength;
	long envelope = ReadVariableLength(&vxlength);

	length -= (vxlength + sizeof(float)*3);

	//	Delete all temporary memory
	delete c;
}

void VFSPlugin_LWO::read_diff(long length)
{
	//	Get a ptr to the last surface (hence the current one)
	Material *m = m_vb->GetMaterial();

	//	Read the diffuse value
	m->diffuse = ReadFloat();

	//	Read the envelope
	long vxlength;
	long envelope = ReadVariableLength(&vxlength);
	length -= (vxlength + sizeof(float)*3);
}

void VFSPlugin_LWO::read_lumi(long length)
{
	//	Get a ptr to the last surface (hence the current one)
	Material *m = m_vb->GetMaterial();

	//	Read how luminous the surface is
	m->luminocity = ReadFloat();

	//	Read the envelope
	long vxlength;
	long envelope = ReadVariableLength(&vxlength);
	length -= (vxlength + sizeof(float)*3);
}

void VFSPlugin_LWO::read_spec(long length)
{
	//	Get a ptr to the last surface (hence the current one)
	Material *m = m_vb->GetMaterial();

	//	Read the specular value of the surface
	m->specular = ReadFloat();

	//	Read the envelope
	long vxlength;
	long envelope = ReadVariableLength(&vxlength);
	length -= (vxlength + sizeof(float)*3);
}

void VFSPlugin_LWO::read_refl(long length)
{
	//	Get a ptr to the last surface (hence the current one)
	Material *m = m_vb->GetMaterial();

	//	Read the reflectivity of the surface
	m->reflective = ReadFloat();

	//	Read the envelope
	long vxlength;
	long envelope = ReadVariableLength(&vxlength);
	length -= (vxlength + sizeof(float)*3);
}

void VFSPlugin_LWO::read_tran(long length)
{
	//	Get a ptr to the last surface (hence the current one)
	Material *m = m_vb->GetMaterial();

	//	Read how transparent the surface is
	m->transparency = ReadFloat();

	//	Read the envelope
	long vxlength;
	long envelope = ReadVariableLength(&vxlength);
	length -= (vxlength + sizeof(float)*3);
}

void VFSPlugin_LWO::read_trnl(long length)
{
	//	Get a ptr to the last surface (hence the current one)
	Material *m = m_vb->GetMaterial();
	
	//	Read how translucent the surface is
	m->translucent = ReadFloat();

	//	Read the envelope
	long vxlength;
	long envelope = ReadVariableLength(&vxlength);
	length -= (vxlength + sizeof(float)*3);
}

void VFSPlugin_LWO::read_glos(long length)
{
	//	Get a ptr to the last surface (hence the current one)
	Material *m = m_vb->GetMaterial();

	//	Read how glossy the surface is
	m->glossiness = ReadFloat();

	//	Read the envelope
	long vxlength;
	long envelope = ReadVariableLength(&vxlength);
	length -= (vxlength + sizeof(float)*3);
}

void VFSPlugin_LWO::read_shrp(long length)
{
	//	Get a ptr to the last surface (hence the current one)
	Material *m = m_vb->GetMaterial();

	//	Read how sharp the surface is
	m->sharpness = ReadFloat();

	//	Read the envelope
	long vxlength;
	long envelope = ReadVariableLength(&vxlength);
	length -= (vxlength + sizeof(float)*3);
}

void VFSPlugin_LWO::read_sman(long length)
{
	float angle = ReadFloat();

	m_vb->SetSmoothingAngle(angle);
}

void VFSPlugin_LWO::read_bump(long length)
{
	//	Get a ptr to the last surface (hence the current one)
	Material *m = m_vb->GetMaterial();

	//	Read the bump intensity of the surface
	m->bump = ReadFloat();

	//	Read the envelope
	long vxlength;
	long envelope = ReadVariableLength(&vxlength);
	length -= (vxlength + sizeof(float)*3);
}

void VFSPlugin_LWO::read_curv(long length)
{
	//	Skip over this unknown/unused chunk
	read_unkn(ID_CURV,length);
}
void VFSPlugin_LWO::read_ptch(long length)
{
	//	Skip over this unknown/unused chunk
	read_unkn(ID_PTCH,length);
}
void VFSPlugin_LWO::read_mbal(long length)
{
	//	Skip over this unknown/unused chunk
	read_unkn(ID_MBAL,length);
}
void VFSPlugin_LWO::read_bone(long length)
{
	//	Skip over this unknown/unused chunk
	read_unkn(ID_BONE,length);
}

void VFSPlugin_LWO::read_face(long length)
{
	short		numvertex;
	short		flags;
	short		mask		= 1023;
	TempPolygon	*p	= NULL;
	int			ctr			= 0;

	//	Read all the polygon indices
	while(length != 0){
		//	Read the polygon data
		numvertex			=		ReadShort();
		length				-=	sizeof(numvertex);
		
		//	extract the flags and numvertex from the polygon data
		flags					=		!mask & numvertex;
		numvertex			&=	mask;

		//	Create a temporary polygon, and an array of indices for this polygon
		p							=		new TempPolygon;		
		p->numvertex	=		numvertex;
		p->index			=		new int[p->numvertex];

		long vxlength;

		//	Read all the indices for this polygon
		for(int a=0;a<p->numvertex;a++){
			p->index[a] = (unsigned long)ReadVariableLength(&vxlength);
			length-=vxlength;
		}

		//	Store the polygon indices
		polygons.push_back(p);
	}
}

//=====================================================
//	THESE METHODS DO MISC TASKS REQUIRED BY THE FORMAT
//	TO PRODUCE VALID MESHES
//=====================================================
long VFSPlugin_LWO::ReadLong(void)
{
	long l;
	memcpy(&l,&m_buffer[m_offset],sizeof(long));
	m_offset+=sizeof(long);
	
	ReverseByteOrder((char *)&l,sizeof(long),1);
	
	return l;
}

short VFSPlugin_LWO::ReadShort(void)
{
	short s;
	memcpy(&s,&m_buffer[m_offset],sizeof(short));
	m_offset+=sizeof(short);

	ReverseByteOrder((char *)&s,sizeof(short),1);

	return s;
}

char VFSPlugin_LWO::ReadChar(void)
{
	char c;
	memcpy(&c,&m_buffer[m_offset],sizeof(char));
	m_offset+=sizeof(char);

	return c;
}

Vertex3f * VFSPlugin_LWO::ReadVertex(void)
{
	Vertex3f *v = new Vertex3f;
	
	v->x	= ReadFloat();
	v->y	= ReadFloat();
	v->z	= ReadFloat();

	return v;
}

char * VFSPlugin_LWO::ReadString(long *length)
{
	int a=0;
	char c, *string = NULL;

	do{	c = ReadChar(); a++; }while(c!=0);

	m_offset-=a;

	if(a>0){
		string = new char[a];

		memcpy(string,&m_buffer[m_offset],a);
		m_offset+=a;

		if(a & 1){
			ReadPad();
			a++;
		}
		*length = a;
	}

	return string;
}

float VFSPlugin_LWO::ReadFloat(void)
{
	float f;
	memcpy(&f,&m_buffer[m_offset],sizeof(float));
	m_offset+=sizeof(float);

	ReverseByteOrder((char *)&f,sizeof(float),1);

	return f;	
}

long VFSPlugin_LWO::ReadVariableLength(long *length)
{
	char c = ReadChar();
	long temp;

	m_offset-=1;

	if(c != 0xFF){
		temp = ReadShort();
		*length = sizeof(short);
	}else{
		temp = ReadLong();
		*length = sizeof(long);
	}

	return temp;
}

Colour4f * VFSPlugin_LWO::ReadColour(long number)
{
	Colour4f *c = new Colour4f;

	c->r = ReadFloat();
	c->g = ReadFloat();
	c->b = ReadFloat();
	c->a = 255;
	if(number == 4) c->a = ReadFloat();

	return c;
}

void VFSPlugin_LWO::ReadPad(long padlength)
{
	m_offset+=padlength;
}

//==================================
//	Surface processing methods
//==================================
void VFSPlugin_LWO::ProcessSurface(void)
{
	Mesh *m = m_fileinfo->mesh;

	//	Sort the Polygon Tags into ascending order
	SortPolygonTags();

	unsigned int a=0;
	int ctr=0,numbertags = (int)polygontags.size()/2;

	while(ctr<numbertags)
	{
		IVertexBuffer *vb = m->GetVertexBuffer(a);

		//	Count the number of polygons in this surface
		int numpoly = CountPolygons(a,ctr);

		vb->Initialise(m->GetNumVertex(),numpoly*3,3,2);

		//	Assign the data into the vertex/normal arrays
		AssignBuffers(a);

		vb->SetPosition((float *)m->GetPosition());

		//	Increase to process the next surface
		a++;
	}

	//	Clear up all temporary memory
	for(a=0;a<polygons.size();a++){
		delete polygons[a]->index;
		delete polygons[a];
	}
	
	//	Clear the polygon/polygontags vectors
	polygons.clear();
	polygontags.clear();

	m->Finalise();
	m->GenerateNormals();
}

void VFSPlugin_LWO::SortPolygonTags(void)
{
	int i,j;
	long tmp[2];
	int size = (int)polygontags.size();

	//	Bubble sort all the polygontags in order of surface
	for(i=0;i<size-2;i+=2){
		for(j=0;j<size-2-i;j+=2){
			if(polygontags[j+2] < polygontags[j]){
				tmp[0]							=	polygontags[j+0];
				tmp[1]							=	polygontags[j+1];

				polygontags[j+0]		=	polygontags[j+2+0];
				polygontags[j+1]		=	polygontags[j+2+1];

				polygontags[j+2+0]	=	tmp[0];
				polygontags[j+2+1]	=	tmp[1];
			}
		}
	}
}

unsigned int VFSPlugin_LWO::CountPolygons(int index, int &counter)
{
	int tri=0,quad=0;

	//	Loop through all the polygontags and extract the
	//	number of vertex for each polygon, talling up each type
	for(unsigned int a=0;a<polygontags.size();a+=2){
		if(polygontags[a] == index){
			if(polygons[polygontags[a+1]]->numvertex == 3) tri++;
			if(polygons[polygontags[a+1]]->numvertex == 4) quad++;
		}
	}

	counter += (tri+quad);

	//	Return the number of polygontags that were counted
	return tri+(quad*2);
}

void VFSPlugin_LWO::AssignBuffers(int index)
{
	int						*i	=	m_fileinfo->mesh->GetVertexBuffer(index)->GetIndex();
	TempPolygon		*p	=	NULL;
	int						ctr	=	0;

	for(unsigned int a=0;a<polygontags.size();a+=2){
		if(polygontags[a] == index){
			p = polygons[polygontags[a+1]];

			if(p->numvertex >= 3){
				i[ctr++]	= p->index[0];
				i[ctr++]	= p->index[1];
				i[ctr++]	= p->index[2];

				//	If there are 4 vertices in this polygon, 
				//	there are two polygons, the original 
				//	polygon + this extra polygon, hence making a quad
				if(p->numvertex == 4){
					i[ctr++] = p->index[0];
					i[ctr++] = p->index[2];
					i[ctr++] = p->index[3];
				}
			}
		}
	}
}
