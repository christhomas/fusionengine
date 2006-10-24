#ifndef _VFSPLUGIN_Q2H_
	#define _VFSPLUGIN_Q2H_

#include <VirtualFS/VirtualFS.h>
#include <Mesh/Vertex.h>
#include <Fusion.h>

#define BSPMAGIC						"IBSP"
#define BSPVERSION					38
#define NUMBERLUMPS					19

#define BSP_ENTITIES				0
#define BSP_PLANES					1
#define BSP_VERTEX					2
#define BSP_VIS							3
#define BSP_NODES						4
#define BSP_TEXINFO					5
#define BSP_FACES						6
#define BSP_LIGHTMAPS				7
#define BSP_LEAVES					8
#define BSP_LEAFFACETABLE		9
#define BSP_LEAFBRUSHTABLE	10
#define BSP_EDGES						11
#define BSP_FACEEDGETABLE		12
#define BSP_MODELS					13
#define BSP_BRUSHES					14
#define BSP_BRUSHSIDES			15
#define BSP_POP							16
#define BSP_AREAS						17
#define BSP_AREAPORTALS			18

/**	@ingroup	VFSPlugin_Q2Group */
struct BSPLump{
	unsigned int	offset;     // offset (in bytes) of the data from the beginning of the file
	unsigned int	length;     // length (in bytes) of the data
};

/**	@ingroup	VFSPlugin_Q2Group	*/
struct BSPHeader{
	unsigned int	magic;								// magic number ("IBSP")
	unsigned int	version;							// version of the BSP format (38)
	BSPLump				lump[NUMBERLUMPS];		// directory of the lumps
};

/**	@ingroup	VFSPlugin_Q2Group */
struct BSPFace{
	unsigned short	plane;             // index of the plane the face is parallel to
	unsigned short	plane_side;        // set if the normal is parallel to the plane normal

	unsigned int		first_edge;        // index of the first edge (in the face edge array)
	unsigned short	num_edges;         // number of consecutive edges (in the face edge array)
	
	unsigned short	texture_info;      // index of the texture info structure	
   
	unsigned char		lightmap_syles[4]; // styles (bit flags) for the lightmaps
	unsigned int		lightmap_offset;   // offset of the lightmap (in bytes) in the lightmap lump
};

/**	@ingroup	VFSPlugin_Q2Group	*/
struct BSPPlane{
	Vertex3f			normal;      // A, B, C components of the plane equation
	float					distance;    // D component of the plane equation
	unsigned int	type;        // ?
};

/**	@ingroup	VFSPlugin_Q2Group */
struct BSPNode{
	unsigned int		plane;             // index of the splitting plane (in the plane array)
    
	int							front_child;       // index of the front child node or leaf
	int							back_child;        // index of the back child node or leaf
   
	Vertex3s				bbox_min;          // minimum x, y and z of the bounding box
	Vertex3s				bbox_max;          // maximum x, y and z of the bounding box
	
	unsigned short	first_face;        // index of the first face (in the face array)
	unsigned short	num_faces;         // number of consecutive edges (in the face array)
};

/**	@ingroup	VFSPlugin_Q2Group */
struct BSPLeaf{
	unsigned int		brush_or;						// ?
	
	unsigned short	cluster;						// -1 for cluster indicates no visibility information
	unsigned short	area;								// ?

	Vertex3s				bbox_min;						// bounding box minimums
	Vertex3s				bbox_max;						// bounding box maximums

	unsigned short	first_leaf_face;		// index of the first face (in the face leaf array)
	unsigned short	num_leaf_faces;			// number of consecutive edges (in the face leaf array)

	unsigned short	first_leaf_brush;		// ?
	unsigned short	num_leaf_brushes;		// ?
};

/**	@ingroup	VFSPlugin_Q2Group */
struct BSPTexInfo{
	Vertex3f			u_axis;
	float					u_offset;
   
	Vertex3f			v_axis;
	float					v_offset;

	unsigned int	flags;
	unsigned int	value;

	char					name[32];

	unsigned int	next_texinfo;
};

/**	@ingroup	VFSPlugin_Q2Group */
struct BSPVisibilityLump
{
	unsigned int pvs;   // offset (in bytes) from the beginning of the visibility lump
	unsigned int phs;   // ?
};

/**	@ingroup	VFSPlugin_Q2Group */
struct BSPEdge{
	short	v1,	//	Edge vertex 1
				v2;	//	Edge vertex 2
};

/**	@ingroup	VFSPlugin_Q2Group
 *	@brief		File format plugin to read/write Quake2 BSP files
 */
class VFSPlugin_Q2BSP: public VFSPlugin{
protected:

	// file info object
	MeshFileInfo	*m_fileinfo;

	BSPHeader			*m_header;
	
	int						m_numvertex;
	int						m_numfaces;
	int						m_numedges;
	int						m_numfaceedges;
	int						m_numtexinfo;

	//	Temporary geometry, surface structures
	BSPFace				*m_face;
	BSPEdge				*m_edge;
	Vertex3f			*m_position;
	int						*m_faceedge;
	BSPTexInfo		*m_texinfo;
	
	char *	ReadData									(int length);

	void		Read_Header								(void);
	void		Read_Entities							(void);
	void		Read_Planes								(void);
	void		Read_Vertices							(void);
	void		Read_Visibility						(void);
	void		Read_Nodes								(void);
	void		Read_Texture_Information	(void);
	void		Read_Faces								(void);
	void		Read_Lightmaps						(void);
	void		Read_Leaves								(void);
	void		Read_Leaf_Face_Table			(void);
	void		Read_Leaf_Brush_Table			(void);
	void		Read_Edges								(void);
	void		Read_Face_Edge_Table			(void);
	void		Read_Models								(void);
	void		Read_Brushes							(void);
	void		Read_Brush_Sides					(void);
	void		Read_Pop									(void);
	void		Read_Areas								(void);
	void		Read_Area_Portals					(void);

	void		ProcessMesh								(void);
	void		CentreMesh								(void);
	void		CreateSurfaces						(void);
	void		BuildPolygonLists					(IVertexBuffer *vb);
	void		CountPolygons							(IVertexBuffer *vb);
	void		AssignTextureCoords				(IVertexBuffer *vb);

public:
											VFSPlugin_Q2BSP		();
	virtual							~VFSPlugin_Q2BSP	();

	virtual FileInfo *	Read							(unsigned char *buffer, unsigned int length);
	virtual char *			Write							(FileInfo *data, unsigned int &length);

	virtual char *			Type							(void);
};

#endif // #ifndef _VFSPLUGIN_Q2H_