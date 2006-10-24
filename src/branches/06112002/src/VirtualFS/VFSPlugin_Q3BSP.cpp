#include <VirtualFS/VFSPlugin_Q3BSP.h>
#include <Fusion.h>

#include <fstream>
using namespace std;
ofstream logfile("bsp.log");

VFSPlugin_Q3BSP::VFSPlugin_Q3BSP()
{
	m_type			=	"bsp;";
	m_offset		=	0;
	m_fileinfo	=	NULL;
}

VFSPlugin_Q3BSP::~VFSPlugin_Q3BSP()
{

}

FileInfo * VFSPlugin_Q3BSP::Read(unsigned char *buffer, unsigned int length)
{
	m_offset								= 0;
	m_buffer								=	buffer;
	m_fileinfo							=	new MeshFileInfo();
	m_fileinfo->filelength	=	length;
	m_fileinfo->mesh				=	fusion->Mesh->CreateMesh();
	
	Read_Header();
	Read_Entities();
	Read_Shaders();
	Read_Planes();
	Read_Nodes();
	Read_Leaves();
	Read_Leaf_Face();
	Read_Leaf_Brushes();
	Read_Models();
	Read_Brushes();
	Read_Brush_Sides();
	Read_Vertex();
	Read_MeshVerts();
	Read_Effects();
	Read_Faces();
	Read_Lightmaps();
	Read_LightVols();
	Read_Visibility();
	
	ProcessMesh();

	return m_fileinfo;
}

char * VFSPlugin_Q3BSP::Write(FileInfo *data, unsigned int &length)
{
	//	Not Implemented
	return NULL;
}

char * VFSPlugin_Q3BSP::Type(void)
{
	return m_type;
}

char * VFSPlugin_Q3BSP::ReadData(int length)
{
	char *data = new char[length];

	memcpy(data,&m_buffer[m_offset],length);

	m_offset+=length;

	return data;
}

void VFSPlugin_Q3BSP::Read_Header(void)
{
	m_header = reinterpret_cast<BSPHeader *>(ReadData(sizeof(BSPHeader)));
}

void VFSPlugin_Q3BSP::Read_Entities(void)
{
	BSPLump *lump	=	&m_header->lump[BSP_ENTITIES];
	m_offset			=	lump->offset;
}

void VFSPlugin_Q3BSP::Read_Shaders(void)
{
  BSPLump *lump	=	&m_header->lump[BSP_SHADERS];
	m_offset			=	lump->offset;
	m_shader			= reinterpret_cast<BSPShader *>(ReadData(lump->length));
	m_numshaders	=	lump->length/sizeof(BSPShader);

	for(int a=0;a<m_numshaders;a++){
		logfile << "Shader["<<a<<"] = {" << std::endl;
		logfile << "\t name = " << m_shader[a].name << std::endl;
		logfile << "};" << std::endl << std::endl;
	}
}

void VFSPlugin_Q3BSP::Read_Planes(void)
{
  BSPLump *lump	=	&m_header->lump[BSP_PLANES];
	m_offset			=	lump->offset;
}

void VFSPlugin_Q3BSP::Read_Nodes(void)
{
	BSPLump *lump	=	&m_header->lump[BSP_NODES];
	m_offset			=	lump->offset;
}

void VFSPlugin_Q3BSP::Read_Leaves(void)
{
	BSPLump *lump	=	&m_header->lump[BSP_LEAVES];
	m_offset			=	lump->offset;
}

void VFSPlugin_Q3BSP::Read_Leaf_Face(void)
{
	BSPLump *lump	=	&m_header->lump[BSP_LEAF_FACES];
	m_offset			=	lump->offset;
}

void VFSPlugin_Q3BSP::Read_Leaf_Brushes(void)
{
	BSPLump *lump = &m_header->lump[BSP_LEAF_BRUSHES];
	m_offset			=	lump->offset;
}

void VFSPlugin_Q3BSP::Read_Models(void)
{
	BSPLump *lump	=	&m_header->lump[7];
	m_offset			=	lump->offset;
}

void VFSPlugin_Q3BSP::Read_Brushes(void)
{
	BSPLump *lump = &m_header->lump[BSP_BRUSH];
	m_offset			=	lump->offset;
}

void VFSPlugin_Q3BSP::Read_Brush_Sides(void)
{
	BSPLump *lump = &m_header->lump[BSP_BRUSHSIDES];
	m_offset			=	lump->offset;
}

void VFSPlugin_Q3BSP::Read_Vertex(void)
{
  BSPLump *lump	=	&m_header->lump[BSP_VERTEX];
	m_offset			=	lump->offset;
	m_numvertex		= lump->length/sizeof(BSPVertex);

	BSPVertex *v = reinterpret_cast<BSPVertex *>(ReadData(lump->length));

	Vertex3f *p	= new Vertex3f[m_numvertex];
	Vertex3f *n	= new Vertex3f[m_numvertex];
	Vertex2f *t	= new Vertex2f[m_numvertex];

	logfile << "Position Data: " << std::endl;
	for(int a=0;a<m_numvertex;a++){
		std::swap(v[a].point.y,v[a].point.z);

		memcpy(&p[a],&v[a].point,		sizeof(Vertex3f));
		memcpy(&n[a],&v[a].normal,	sizeof(Vertex3f));
		memcpy(&t[a],&v[a].texture,	sizeof(Vertex2f));

		//logfile << "Position["<<a<<"] = {" << p[a].x << ", " << p[a].y << ", " << p[a].z << "};"<< std::endl;
	}

	delete[] v;

	m_fileinfo->mesh->Initialise(m_numvertex);
	m_fileinfo->mesh->SetPosition(p);
	m_fileinfo->mesh->SetNormal(n);
	m_fileinfo->mesh->SetTexcoord(t);
}

void VFSPlugin_Q3BSP::Read_MeshVerts(void)
{
	BSPLump *lump		=	&m_header->lump[BSP_MESHVERTS];
	m_offset				=	lump->offset;
	m_nummeshverts	=	lump->length/sizeof(int);
	m_meshverts			=	reinterpret_cast<int *>(ReadData(lump->length));
}

void VFSPlugin_Q3BSP::Read_Effects(void)
{
	BSPLump *lump = &m_header->lump[BSP_EFFECTS];
	m_offset			=	lump->offset;
}

void VFSPlugin_Q3BSP::Read_Faces(void)
{
	BSPLump *lump	=	&m_header->lump[BSP_FACES];
	m_offset			=	lump->offset;
	m_numfaces		=	lump->length/sizeof(BSPFace);
	m_face				=	reinterpret_cast<BSPFace *>(ReadData(lump->length));
}
		
void VFSPlugin_Q3BSP::Read_Lightmaps(void)
{
	BSPLump *lump	=	&m_header->lump[BSP_LIGHTMAPS];
	m_offset			=	lump->offset;
}

void VFSPlugin_Q3BSP::Read_LightVols(void)
{
	BSPLump *lump = &m_header->lump[BSP_LIGHTVOLS];
	m_offset			=	lump->offset;
}

void VFSPlugin_Q3BSP::Read_Visibility(void)
{
	BSPLump *lump	=	&m_header->lump[BSP_VIS];
	m_offset			=	lump->offset;
}

void VFSPlugin_Q3BSP::ProcessMesh(void)
{
	Mesh *m = m_fileinfo->mesh;

	CentreMesh();

	for(int a=0;a<m_numshaders;a++){
		CreateVertexBuffer(a);
		BuildVertexBuffer(a);
	}
}

void VFSPlugin_Q3BSP::CentreMesh(void)
{
	int a;
	Maths::Vector centre,min,max;
	Mesh *m = m_fileinfo->mesh;
	Vertex3f *vertex = m->GetPosition();

	//	Sets up the bounding box with extreme values
	min.Set(16000,16000,16000);
	max.Set(-16000,-16000,-16000);

	//	Calculate the bounding box of the Surface
	for(a=0;a<m_numvertex;a++){
		if(vertex[a].x < min.x) min.x = vertex[a].x;
		if(vertex[a].y < min.y) min.y = vertex[a].y;
		if(vertex[a].z < min.z) min.z = vertex[a].z;
		
		if(vertex[a].x > max.x) max.x = vertex[a].x;
		if(vertex[a].y > max.y) max.y = vertex[a].y;
		if(vertex[a].z > max.z) max.z = vertex[a].z;
	}

	//	Calculate the centre of the box
	centre = (max - min)/2;

	for(a=0;a<m_numvertex;a++){
		//	Move the Surface to centre around (0,0,0)
		vertex[a].x -= centre.x;
		vertex[a].y -= centre.y;
		vertex[a].z -= centre.z;
		
		//	Invert the x axis, so the Surface displays the correct way
		vertex[a].x = -vertex[a].x;
	}
}

void VFSPlugin_Q3BSP::CreateVertexBuffer(int shader)
{
	IVertexBuffer	*vb	=	m_fileinfo->mesh->AddVertexBuffer();
	Material			*m	=	vb->GetMaterial();

	m->colour.r	= 1;
	m->colour.g	=	1;
	m->colour.b	=	1;
	m->colour.a	=	1;

	//	Load texture
	vb->SetTextureLayer(0,vb->GetTexcoord(),FindTexture(m_shader[shader].name));
}

ITexture * VFSPlugin_Q3BSP::FindTexture(char *name)
{
	VFSHandle	*h				= NULL;
	char			*filename	= new char[256];
	ITexture	*t				= NULL;

	//	look for tga
	if(t == NULL){
		memset(filename,0,256);
		sprintf(filename,"file://%s.tga",name);

		t = fusion->Graphics->CreateTexture(filename);
	}

	//	look for jpg
	if(t == NULL){
		memset(filename,0,256);
		sprintf(filename,"file://%s.jpg",name);

		t = fusion->Graphics->CreateTexture(filename);
	}

	//	look for jpeg
	if(t == NULL){
		memset(filename,0,256);
		sprintf(filename,"file://%s.jpeg",name);

		t = fusion->Graphics->CreateTexture(filename);
	}

	//	look for shader (return null)
	if(t == NULL){
		memset(filename,0,256);
		sprintf(filename,"file://%s.shader",name);

		t = NULL;
	}

	if(t == NULL){
		logfile << "FindTexture: Couldnt find texture \"" << name << "\"" << std::endl;
	}

	return t;
}

void VFSPlugin_Q3BSP::BuildVertexBuffer(int shader)
{
	CountPolygons(shader);

	Mesh					*m	=	m_fileinfo->mesh;
	IVertexBuffer *vb	=	m->GetVertexBuffer(shader);
	int						ctr	=	0;
	int						*i	=	vb->GetIndex();

	for(int a=0;a<m_numfaces;a++){
		if(m_face[a].shader == shader){
			if(m_face[a].type == 1){
				TempPolygon *p = new TempPolygon;

				p->numvertex	= m_face[a].vert_count;
				p->index			= new int[p->numvertex];

				for(int b=0;b<p->numvertex;b++)	p->index[b] = m_face[a].vert_start+b;

				//	Process the list of vertices into triangles
				for(int c=1;c<p->numvertex-1;c++){
					i[ctr++] = p->index[0];
					i[ctr++] = p->index[c+0];
					i[ctr++] = p->index[c+1];
				}
			}

			if(m_face[a].type == 3){
				TempPolygon *p = new TempPolygon;

				p->numvertex	= m_face[a].vert_count;
				p->index			= new int[p->numvertex];

				for(int b=0;b<p->numvertex;b++)	p->index[b] = m_face[a].vert_start+b;

				//	Process the list of vertices into triangles
				for(int c=1;c<p->numvertex-1;c++){
					i[ctr++] = p->index[0];
					i[ctr++] = p->index[c+0];
					i[ctr++] = p->index[c+1];
				}
			}
		}
	}

	vb->SetPosition	((float *)m->GetPosition());
	vb->SetNormal		((float *)m->GetNormal());
	vb->SetTextureLayer(0,(float *)m->GetTexcoord(),vb->GetTexture());
}

void VFSPlugin_Q3BSP::CountPolygons(int shader)
{
	int a,ctr = 0;

	for(a=0;a<m_numfaces;a++){
		if(m_face[a].shader == shader){
			if(m_face[a].type == 1){
				ctr += (m_face[a].vert_count-2);
			}

			if(m_face[a].type == 3){
				ctr += (m_face[a].vert_count-2);
			}
		}
	}

	IVertexBuffer *vb = m_fileinfo->mesh->GetVertexBuffer(shader);
	vb->Initialise(m_fileinfo->mesh->GetNumVertex(),ctr*3,3,2);
}
