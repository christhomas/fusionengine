#ifndef _VFSPLUGIN_Q3BSP_H_
	#define _VFSPLUGIN_Q3BSP_H_

#include <vfs/VirtualFS.h>
#include <mesh/Vertex.h>
#include <Fusion.h>

#define BSPMAGIC			"IBSP"
#define BSPVERSION			46
#define NUMBERLUMPS			17

#define BSP_ENTITIES		0
#define BSP_SHADERS			1
#define BSP_PLANES			2
#define BSP_NODES			3
#define BSP_LEAVES			4
#define BSP_LEAF_FACES		5
#define BSP_LEAF_BRUSHES	6
#define BSP_MODELS			7
#define BSP_BRUSH			8
#define BSP_BRUSHSIDES		9
#define BSP_VERTEX			10
#define BSP_MESHVERTS		11
#define BSP_EFFECTS			12
#define BSP_FACES			13
#define BSP_LIGHTMAPS		14
#define BSP_LIGHTVOLS		15
#define BSP_VIS				16

struct BSPLump{
	unsigned int offset;	// offset (in bytes) of the data from the beginning of the file
	unsigned int length;	// length (in bytes) of the data
};

struct BSPHeader{
	unsigned char	magic[4];			// magic number ("IBSP")
	unsigned int	version;			// version of the BSP format (46)
	BSPLump			lump[NUMBERLUMPS];	// directory of the lumps
};

struct BSPFace{
	int	shader;		//	shader ref
	int	unknown;
	int	type;		//	face type
	int	vert_start;	//	Index of first vertex
	int	vert_count;	//	Number of vertices
	int	mv_start;	//	Index of first meshvert
	int	mv_count;		//	Number of meshverts
	int	lm_texture;		//	lightmap
	int	lm_offset[2];
	int	lm_size[2];
	float	org[3];		//	facetype_normal only
	float	bbox[6];		//	facetype_patch only
	float	normal[3];	//	facetype_normal only
	int	mesh_cp[2];	//	patch control point dims
};

struct BSPLeaf{
	int	cluster;		// visibility cluster number
	int	area;
	int	bbox[6];
	int	face_start;
	int	face_count;
	int	brush_start;
	int	brush_count;
};

struct BSPModel{
	float	bbox[6];
	int	face_start;
	int	face_count;
	int	brush_start;
	int	brush_count;
};

struct BSPPlane{
	Vector3f normal;
	float dist;
};

struct BSPShader{
	char	name[64];
	int		surface_flags;
	int		content_flags;
};

struct BSPVis{
	int			cluster_count;
	int			row_size;
	unsigned char	data[1];
};

struct BSPVertex{
	Vertex3f	point;
	Vertex2f	texture;
	Vertex2f	lightmap;
	Vertex3f	normal;
	int		color;
};

struct BSPNode{
	int plane;			// dividing plane
	//int children[2];	// left and right nodes,
					// negative are leaves
	int front;
	int back;
	int bbox[6];
};

/**	@ingroup	VFSPlugin_Q3BSP_Group
 *	@brief		File format plugin to read/write Quake3 BSP files
 *
 *	NOTE: This class is not yet implemented
 */
class VFSPlugin_Q3BSP: public VFSPlugin{
protected:
	MeshFileInfo	*m_fileinfo;

	int m_numshaders;
	int m_numvertex;
	int m_nummeshverts;
	int m_numfaces;

	BSPHeader	*m_header;
	BSPFace		*m_face;
	BSPShader	*m_shader;
	int			*m_meshverts;

	char *				ReadData			(int length);

	void				Read_Header			(void);
	void				Read_Entities		(void);
	void				Read_Shaders		(void);
	void				Read_Planes			(void);
	void				Read_Nodes			(void);
	void				Read_Leaves			(void);
	void				Read_Leaf_Face		(void);
	void				Read_Leaf_Brushes	(void);
	void				Read_Models			(void);
	void				Read_Brushes		(void);
	void				Read_Brush_Sides	(void);
	void				Read_Vertex			(void);
	void				Read_MeshVerts		(void);
	void				Read_Effects		(void);
	void				Read_Faces			(void);
	void				Read_Lightmaps		(void);
	void				Read_LightVols		(void);
	void				Read_Visibility		(void);
	
	void				ProcessMesh			(void);
	void				CentreMesh			(void);
	void				CreateVertexBuffer	(int shader);
	void				BuildVertexBuffer	(int shader);
	void				CountPolygons		(int shader);
	ITexture *			FindTexture			(char *name);
public:
						VFSPlugin_Q3BSP		();
	virtual				~VFSPlugin_Q3BSP	();

	virtual FileInfo *	Read				(unsigned char *buffer, unsigned int length);
	virtual char *		Write				(FileInfo *data, unsigned int &length);

	virtual std::string	Type				(void);
};

#endif // #ifndef _VFSPLUGIN_Q3BSP_H_
