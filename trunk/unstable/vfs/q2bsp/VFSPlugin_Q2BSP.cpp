#include <VFSPlugin_Q2BSP.h>

#include <fstream>
using namespace std;
ofstream logfile("bsp.log");

//=========================
//	VFSPlugin_Q2BSP Methods
//=========================
VFSPlugin_Q2BSP::VFSPlugin_Q2BSP()
{
	m_type			=	"bsp;";
	m_offset			=	0;
	m_length			=	0;
	
	m_numvertex		=	0;
	m_numfaces		=	0;
	m_numedges		=	0;
	m_numfaceedges	=	0;
	m_numtexinfo		=	0;

	m_face			=	0;
	m_edge			=	0;
	m_position		=	0;
	m_faceedge		=	0;
	m_texinfo			=	0;

	m_fileinfo			=	NULL;
	m_buffer			= NULL;
	m_fileinfo			= NULL;
	m_header		= NULL;
}

VFSPlugin_Q2BSP::~VFSPlugin_Q2BSP()
{
	delete	m_header;
	delete[]	m_face;
	delete[]	m_position;
	delete[]	m_edge;
	delete[]	m_faceedge;
	delete[]	m_texinfo;
}

char * VFSPlugin_Q2BSP::Type(void)
{
	return m_type;
}

FileInfo * VFSPlugin_Q2BSP::Read(unsigned char *buffer, unsigned int length)
{
	m_offset				= 0;
	m_buffer				=	buffer;
	m_fileinfo				=	new MeshFileInfo();
	m_fileinfo->filelength	=	length;
	m_fileinfo->mesh		=	fusion->Mesh->CreateMesh();

	Read_Header();
	Read_Entities();
	Read_Planes();
	Read_Vertices();
	Read_Visibility();
	Read_Nodes();
	Read_Texture_Information();
	Read_Faces();
	Read_Lightmaps();
	Read_Leaves();
	Read_Leaf_Face_Table();
	Read_Leaf_Brush_Table();
	Read_Edges();
	Read_Face_Edge_Table();
	Read_Models();
	Read_Brushes();
	Read_Brush_Sides();
	Read_Pop();
	Read_Areas();
	Read_Area_Portals();

	ProcessMesh();

	return m_fileinfo;
}

char * VFSPlugin_Q2BSP::Write(FileInfo *data, unsigned int &length)
{
	//	Not implemented
	return NULL;
}

//=====================
//	BSP File methods
//=====================
char * VFSPlugin_Q2BSP::ReadData(int length)
{
	char *data = new char[length];

	memcpy(data,&m_buffer[m_offset],length);

	m_offset+=length;

	return data;
}

void VFSPlugin_Q2BSP::Read_Header(void)
{
	m_header = reinterpret_cast<BSPHeader *>(ReadData(sizeof(BSPHeader)));

//	for(int a=0;a<NUMBERLUMPS;a++){
//		logfile << "Lump[" << a << "] = {" << endl;
//		logfile << "offset = " << m_header->lump[a].offset << endl;
//		logfile << "length = " << m_header->lump[a].length << endl;
//		logfile << "};" << endl << endl;
//	}
}

void VFSPlugin_Q2BSP::Read_Entities(void){}
void VFSPlugin_Q2BSP::Read_Planes(void)
{
	BSPLump *lump = &m_header->lump[BSP_PLANES];
//	logfile << "\n/\n*\n*\tNew Lump: Plane Lump[offset=" << lump->offset << ", length=" << lump->length << "]\n*\n*/" << endl;

	m_offset = lump->offset;

	int num_planes = lump->length/sizeof(BSPPlane);
	BSPPlane *plane = reinterpret_cast<BSPPlane *>(ReadData(lump->length));

	delete plane;

//	for(int a=0;a<num_planes;a++){
//		logfile << "Plane[" << a << "] = { " << endl;
//		logfile << "normal = { " << plane[a].normal.x << ", " << plane[a].normal.y << ", " << plane[a].normal.z << " };" << endl;
//		logfile << "distance = " << plane[a].distance << endl;
//		logfile << "type = " << plane[a].type << endl;
//		logfile << "};" << endl;
//	}
}

void VFSPlugin_Q2BSP::Read_Vertices(void)
{
	BSPLump *lump = &m_header->lump[BSP_VERTEX];
//	logfile << "\n/\n*\n*\tNew Lump: Vertex Lump[offset=" << lump->offset << ", length=" << lump->length << "]\n*\n*/" << endl;

	m_offset		=	lump->offset;
	m_numvertex	=	lump->length/sizeof(Vertex3f);
	m_position	=	reinterpret_cast<Vertex3f *>(ReadData(lump->length));
	
	for(int a=0;a<m_numvertex;a++)	std::swap(m_position[a].y,m_position[a].z);

	//	Assign all the position data
	Mesh *m = m_fileinfo->mesh;
	
	m->Initialise(m_numvertex);
	
	Vertex3f *p = m->GetPosition();
	
	memcpy(p,m_position,m_numvertex*sizeof(Vertex3f));
}

void VFSPlugin_Q2BSP::Read_Visibility(void)
{
	BSPLump *lump = &m_header->lump[BSP_VIS];
//	logfile << "\n/\n*\n*\tNew Lump: Visibility Lump[offset=" << lump->offset << ", length=" << lump->length << "]\n*\n*/" << endl;
	
	m_offset = lump->offset;

	int num_clusters = *((int *)ReadData(sizeof(int)));

	BSPVisibilityLump *vislump = reinterpret_cast<BSPVisibilityLump *>(ReadData(sizeof(BSPVisibilityLump)*num_clusters));

	delete vislump;

//	logfile << "num_clusters = " << num_clusters << endl;
//	for(int a=0;a<num_clusters;a++)
//	{
//		logfile << "VisibilityLump[" << a << "] = {" << endl;
//		logfile << "pvs = " << vislump[a].pvs << endl;
//		logfile << "phs = " << vislump[a].phs << endl;
//		logfile << "};" << endl << endl;
//	}

/*
 *	TODO:	This section is not complete, not enough information to write the function
 *	TODO:	Find the "Quake specifications document, or email amcguire@andrew.cmu.edu
 *	TODO:	for more information
 */
}

void VFSPlugin_Q2BSP::Read_Nodes(void)
{
	BSPLump *lump = &m_header->lump[BSP_NODES];
//	logfile << "\n/\n*\n*\tNew Lump: Node Lump[offset=" << lump->offset << ", length=" << lump->length << "]\n*\n*/" << endl;

	m_offset = lump->offset;

	int num_nodes = lump->length/sizeof(BSPNode);
	BSPNode *node = reinterpret_cast<BSPNode *>(ReadData(lump->length));

	delete node;

//	logfile << "num_nodes = " << num_nodes << endl;
//	for(int a=0;a<num_nodes;a++)
//	{
//		logfile << "Node[" << a << "] = {" << endl;
//		logfile << "plane = " << node[a].plane << endl;
//		logfile << "front_child = " << node[a].front_child << endl;
//		logfile << "back_child = " << node[a].back_child << endl;
//		logfile << "bbox_min = { " << node[a].bbox_min.x << ", " << node[a].bbox_min.y << ", " << node[a].bbox_min.z << " };" << endl;
//		logfile << "bbox_max = { " << node[a].bbox_max.x << ", " << node[a].bbox_max.y << ", " << node[a].bbox_max.z << " };" << endl;
//		logfile << "first_face = " << node[a].first_face << endl;
//		logfile << "num_faces = " << node[a].num_faces << endl;
//		logfile << "};" << endl;
//	}
}

void VFSPlugin_Q2BSP::Read_Texture_Information(void)
{
	BSPLump *lump = &m_header->lump[BSP_TEXINFO];
	logfile << "\n/\n*\n*\tNew Lump: Texture Information Lump[offset=" << lump->offset << ", length=" << lump->length << "]\n*\n*/" << endl;

	m_offset			=	lump->offset;
	m_numtexinfo	= lump->length/sizeof(BSPTexInfo);
	m_texinfo			=	reinterpret_cast<BSPTexInfo *>(ReadData(lump->length));

/*
	int num_texinfo;
	BSPTexInfo *ti;

	num_texinfo = lump->length/sizeof(BSPTexInfo);

	logfile << "num_texinfo = " << num_texinfo << endl;
	for(int a=0;a<num_texinfo;a++){
		ti = reinterpret_cast<BSPTexInfo *>(ReadData(sizeof(BSPTexInfo)));
		textureinfo.push_back(ti);

		logfile << "TextureInfo[" << a << "] = {" << endl;
		logfile << "u_axis = " << ti->u_axis.x << ", " << ti->u_axis.y << ", " << ti->u_axis.z << " };" << endl;
		logfile << "u_offset = " << ti->u_offset << endl;
		logfile << "v_axis = " << ti->v_axis.x << ", " << ti->v_axis.y << ", " << ti->v_axis.z << " };" << endl;
		logfile << "v_offset = " << ti->v_offset << endl;
		logfile << "flags = " << ti->flags << endl;
		logfile << "value = " << ti->value << endl;
		logfile << "texture_name = " << ti->name << endl;
		logfile << "next_texinfo = " << ti->next_texinfo << endl;
		logfile << "};" << endl << endl;
	}
*/
}

void VFSPlugin_Q2BSP::Read_Faces(void)
{
	BSPLump *lump = &m_header->lump[BSP_FACES];
//	logfile << "\n/\n*\n*\tNew Lump: Face Lump[offset=" << lump->offset << ", length=" << lump->length << "]\n*\n*/" << endl;

	m_offset		= lump->offset;
	m_numfaces	=	lump->length/sizeof(BSPFace);
	m_face			=	reinterpret_cast<BSPFace *>(ReadData(lump->length));

//	logfile << "num_faces = " << num_faces << endl;
//	for(int a=0;a<num_faces;a++)
//	{
//		logfile << "Face[" << a << "] = {" << endl;
//		logfile << "Plane = " << faces[a].plane << endl;
//		logfile << "Plane Side = " << faces[a].plane_side << endl;
//		logfile << "first edge = " << faces[a].first_edge << endl;
//		logfile << "texture info = " << faces[a].texture_info << endl;
//		logfile << "lightmap styles = { " << (int)faces[a].lightmap_syles[0] << ", " << (int)faces[a].lightmap_syles[1] << ", " << (int)faces[a].lightmap_syles[2] << ", " << (int)faces[a].lightmap_syles[3] << " };" << endl;
//		logfile << "lightmap offset = " << faces[a].lightmap_offset << endl;
//		logfile << "};" << endl << endl;
//	}
}

void VFSPlugin_Q2BSP::Read_Lightmaps(void){}
void VFSPlugin_Q2BSP::Read_Leaves(void)
{
	BSPLump *lump = &m_header->lump[BSP_LEAVES];
//	logfile << "\n/\n*\n*\tNew Lump: Leaf Lump[offset=" << lump->offset << ", length=" << lump->length << "]\n*\n*/" << endl;

	m_offset = lump->offset;
	
	int num_leaves = lump->length/sizeof(BSPLeaf);
	BSPLeaf *leaves = reinterpret_cast<BSPLeaf *>(ReadData(lump->length));

	delete leaves;

//	logfile << "num_leaves = " << num_leaves << endl;
//	for(int a=0;a<num_leaves;a++)
//	{
//		logfile << "Leaf[" << a << "] = {" << endl;
//		logfile << "brush_or = "			<< leaves[a].brush_or	<< endl;
//		logfile << "cluster = "				<< leaves[a].cluster	<< endl;
//		logfile << "area = "				<< leaves[a].area		<< endl;
//		logfile << "bbox_min = { "			<< leaves[a].bbox_min.x << ", " << leaves[a].bbox_min.y << ", " << leaves[a].bbox_min.z << " };" << endl;
//		logfile << "bbox_max = { "			<< leaves[a].bbox_max.x << ", " << leaves[a].bbox_max.y << ", " << leaves[a].bbox_max.z << " };" << endl;
//		logfile << "first_leaf_face = "		<< leaves[a].first_leaf_face	<< endl;
//		logfile << "num_leaf_faces = "		<< leaves[a].num_leaf_faces		<< endl;
//		logfile << "first_leaf_brush = "	<< leaves[a].first_leaf_brush	<< endl;
//		logfile << "num_leaf_brushes = "	<< leaves[a].num_leaf_brushes	<< endl;
//		logfile << "};" << endl;
//	}
}

void VFSPlugin_Q2BSP::Read_Leaf_Face_Table(void)
{
	BSPLump *lump = &m_header->lump[BSP_LEAFFACETABLE];
//	logfile << "\n/\n*\n*\tNew Lump: Leaf Face Table Lump[offset=" << lump->offset << ", length=" << lump->length << "]\n*\n*/" << endl;

	m_offset = lump->offset;

	int num_leaffaces = lump->length/2;
	int *leaffaces = reinterpret_cast<int *>(ReadData(sizeof(int)*num_leaffaces));
	
	delete leaffaces;

//	logfile << "num_leaffaces = " << num_leaffaces << endl;
//	for(int a=0;a<num_leaffaces;a++)	logfile << "leafface[" << a << "] = " << leaffaces[a] << endl;
}

void VFSPlugin_Q2BSP::Read_Leaf_Brush_Table(void){}
void VFSPlugin_Q2BSP::Read_Edges(void)
{
	BSPLump *lump = &m_header->lump[BSP_EDGES];
//	logfile << "\n/\n*\n*\tNew Lump: Edge Lump[offset=" << lump->offset << ", length=" << lump->length << "]\n*\n*/" << endl;

	m_offset		=	lump->offset;
	m_numedges	= lump->length/4;
	m_edge			=	reinterpret_cast<BSPEdge *>(ReadData(lump->length));
}

void VFSPlugin_Q2BSP::Read_Face_Edge_Table(void)
{
	BSPLump *lump = &m_header->lump[BSP_FACEEDGETABLE];
//	logfile << "\n/\n*\n*\tNew Lump: Face Edge Table Lump[offset=" << lump->offset << ", length=" << lump->length << "]\n*\n*/" << endl;

	m_offset				=	lump->offset;
	m_numfaceedges	=	lump->length/4;
	m_faceedge			=	reinterpret_cast<int *>(ReadData(lump->length));
}

void VFSPlugin_Q2BSP::Read_Models(void){}
void VFSPlugin_Q2BSP::Read_Brushes(void){}
void VFSPlugin_Q2BSP::Read_Brush_Sides(void){}
void VFSPlugin_Q2BSP::Read_Pop(void){}
void VFSPlugin_Q2BSP::Read_Areas(void){}
void VFSPlugin_Q2BSP::Read_Area_Portals(void){}

//=============================
//	Methods to process the 
//	mesh into useable form
//=============================
void VFSPlugin_Q2BSP::ProcessMesh(void)
{
	Mesh *m = m_fileinfo->mesh;

	CentreMesh();
	CreateSurfaces();

	for(int a=0;a<m->NumVertexBuffer();a++)	BuildPolygonLists(m->GetVertexBuffer(a));

	m->Finalise();
	m->GenerateNormals(2,1,0);

	//	Build up a mesh tools system rather than clutter up the mesh objects with tool type functionality
	//MeshTools::Finalise(m);
	//MeshTools::GenerateNormals(m,2,1,0);

	//	Textures dont work, so disable calculation of the texture coordinates
	//for(a=0;a<m->NumSurface();a++)	AssignTextureCoords(m->GetSurface(a));
}

void VFSPlugin_Q2BSP::CentreMesh(void)
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
	centre	= (max - min)/2;

	for(a=0;a<m_numvertex;a++){
		//	Move the Surface to centre around (0,0,0)
		vertex[a].x -= centre.x;
		vertex[a].y -= centre.y;
		vertex[a].z -= centre.z;
		
		//	Invert the x axis, so the Surface displays the correct way
		vertex[a].x = -vertex[a].x;
	}
}

void VFSPlugin_Q2BSP::CreateSurfaces(void)
{
	std::vector<int>	TextureID;
	BSPTexInfo			*t;
	bool				NewTexture	= true;
	Material			*m			= NULL;
	IVertexBuffer		*vb			= NULL;

	//	Build a list of Texture id codes to reference objects in the BSPTexInfo array
	for(int a=0;a<m_numfaces;a++){
		if(m_face[a].num_edges > 4){
			for(int b=1;b<(m_face[a].num_edges-1);b++){
				TextureID.push_back(m_face[a].texture_info);
			}
		}else{
			TextureID.push_back(m_face[a].texture_info);
		}
	}

	//	Now build the surfaces required to hold those textures..
	for(unsigned int a=0;a<TextureID.size();a++){
		NewTexture = true;

		t = &m_texinfo[TextureID[a]];
		
		for(int a=0;a<m_fileinfo->mesh->NumVertexBuffer();a++){
			vb = m_fileinfo->mesh->GetVertexBuffer(a);
			
			if(strcmp(t->name,vb->GetName()) == 0) NewTexture = false;
		}

		if(NewTexture == true){
			char *name = new char[strlen("file://")+strlen(t->name)+strlen(".wal")+1];
			sprintf(name,"file://%s.wal",t->name);

			vb	= m_fileinfo->mesh->AddVertexBuffer();

			m	= vb->GetMaterial();
			m->colour.r	=	1;
			m->colour.g	=	1;
			m->colour.b	=	1;
			m->colour.a	= 1;

			vb->SetName(t->name);
			//	Textures dont work, so dont bother loading the texture
			//s->VertexBuffer->SetTexture(fusion->Graphics->CreateTexture(name));
			vb->SetTextureLayer(0,vb->GetTexcoord(),fusion->Graphics->CreateTexture(name));
			vb->SetMaterial(m);

			delete name;
		}
	}

	TextureID.clear();
}

void VFSPlugin_Q2BSP::AssignTextureCoords(IVertexBuffer *vb)
{
	int					a,b;
	Mesh				*m	=	m_fileinfo->mesh;
	Vertex3f		*v	=	m->GetPosition();
	Vertex2f		*tc	=	NULL;
	BSPTexInfo	*ti	=	NULL;
	int					*i	= vb->GetIndex();

	if((tc = (Vertex2f *)m->GetTexcoord()) == NULL){
		tc = new Vertex2f[m->GetNumVertex()];
		memset(tc,0,m->GetNumVertex()*sizeof(Vertex2f));
		m->SetTexcoord(tc);
	}

	for(a=0;a<m_numtexinfo;a++){
		ti = &m_texinfo[a];

		if(strcmp(vb->GetName(),ti->name) == 0){
			Maths::Vector tu_vec(ti->u_axis);
			Maths::Vector tv_vec(ti->v_axis);

			ITexture *texture = vb->GetTexture();

			for(b=0;b<vb->GetNumIndex();b++){
				Maths::Vector vec(v[i[b]]);
				
				tc[i[b]].x	=	( vec.DotProduct(tu_vec) + ti->u_offset) / texture->GetWidth();
				tc[i[b]].y	=	( vec.DotProduct(tv_vec) + ti->v_offset) / texture->GetHeight();
			}
		}
	}

	//for(a=0;a<m->GetNumVertex();a++)	logfile << "texcoord["<<a<<"] = { " << tc[a].x << ", " << tc[b].y << " };" << std::endl;

//	vb->SetTexcoord((float *)tc);
	vb->SetTextureLayer(0,(float *)tc,vb->GetTexture());
}

void VFSPlugin_Q2BSP::BuildPolygonLists(IVertexBuffer *vb)
{
	int a,b,c,v1,v2,*i,ctr=0;

	CountPolygons(vb);

	i = vb->GetIndex();

	//====================================
	//	Build the polygon vertex indices
	//====================================
	//	Loop through the faces
	for(a=0;a<m_numfaces;a++){
		
		//	Compare this face to the texture, if match found, this is a face from the surface/texture we're constructing
		if(strcmp(vb->GetName(), m_texinfo[m_face[a].texture_info].name) == 0){

			TempPolygon *p = new TempPolygon;
			//	The number of vertex == number of edges in polygon
			p->numvertex = m_face[a].num_edges;
			p->index = new int[p->numvertex];

			//	Loop through the edges in this polygon, 
			//	assigning the first vertex of each edge to the vertex index
			for(b=0;b<p->numvertex;b++){
				v1 = m_edge[abs(m_faceedge[m_face[a].first_edge + b])].v1;
				v2 = m_edge[abs(m_faceedge[m_face[a].first_edge + b])].v2;

				//	If the edge index is negative, the second vertex is the first vertex in the edge, so swap these around
				//	so the first vertex of the edge, is always the correct vertex to start the edge
				if(m_faceedge[m_face[a].first_edge + b] < 0)	std::swap(v1,v2);

				//	Store the first vertex of the edge
				p->index[b] = v1;
			}

			//	Process the list of vertices into triangles
			for(c=1;c<(p->numvertex-1);c++){
				i[ctr++] = p->index[0];
				i[ctr++] = p->index[c+0];
				i[ctr++] = p->index[c+1];
			}
			
			//	Delete the old polygon
			delete p->index;
			delete p;
		}
	}

	vb->SetPosition((float *)m_fileinfo->mesh->GetPosition());
}

void VFSPlugin_Q2BSP::CountPolygons(IVertexBuffer *vb)
{
	int a,ctr = 0;

	for(a=0;a<m_numfaces;a++){
		if(strcmp(vb->GetName(), m_texinfo[m_face[a].texture_info].name) == 0){
			ctr += (m_face[a].num_edges-2);
		}
	}

	vb->Initialise(m_fileinfo->mesh->GetNumVertex(),ctr*3,3,2);
}
