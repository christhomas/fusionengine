#ifndef _VFSPLUGIN_3DS_H_
	#define _VFSPLUGIN_3DS_H_

#include <VirtualFS/VirtualFS.h>
#include <Mesh/Mesh.h>
#include <cmath>
#include <Fusion.h>

//
//	The defines are copied from the documentation produced by:
//	Martin van Velsen	email: <vvelsen@ronix.ptf.hro.nl>
//	Robin Feroq				email: <robin@msrwww.fc-net.fr>
//	Jim Pitts					email: <jim@micronetics.com>
//	Albert Szilvasy		email: <szilvasy@almos.vein.hu>
//
//	See I'm not a twat that doesnt recognise other people who help me :P
//
//	I added more definitions to these defines to support chunks which werent
//	known at the time, but are known now
//
//------ Primary chunk

#define MAIN3DS					0x4D4D

//------ Main Chunks

#define VERSION_3DS			0x0002	//	This is the version number of the 3ds modeller
#define EDIT_3DS				0x3D3D  //	this is the start of the editor config
#define KEYF_3DS				0xB000  //	this is the start of the keyframer config

//------ sub defines of EDIT3DS

#define EDIT_MESHVER		0x3D3E
#define EDIT_MATERIAL		0xAFFF
#define EDIT_CONFIG1		0x0100
#define EDIT_CONFIG2		0x3E3D
#define EDIT_VIEW_P1		0x7012
#define EDIT_VIEW_P2		0x7011
#define EDIT_VIEW_P3		0x7020
#define EDIT_VIEW1			0x7001
#define EDIT_BACKGR			0x1200
#define EDIT_AMBIENT		0x2100
#define EDIT_OBJECT			0x4000

#define EDIT_UNKNW01		0x1100
#define EDIT_UNKNW02		0x1201
#define EDIT_UNKNW03		0x1300
#define EDIT_UNKNW04		0x1400
#define EDIT_UNKNW05		0x1420
#define EDIT_UNKNW06		0x1450
#define EDIT_UNKNW07		0x1500
#define EDIT_UNKNW08		0x2200
#define EDIT_UNKNW09		0x2201
#define EDIT_UNKNW10		0x2210
#define EDIT_UNKNW11		0x2300
#define EDIT_UNKNW12		0x2302 
#define EDIT_UNKNW13		0x3000
#define EDIT_UNKNW14		0xAFFF

//------ sub defines of EDIT_MATERIAL
#define MAT_NAME				0xA000
#define MAT_AMBIENT			0xA010
#define MAT_DIFFUSE			0xA020
#define MAT_SPEC				0xA030
#define MAT_SHINY				0xA040
#define MAT_STRENGTH		0xA041
#define MAT_TRANS				0xA050
#define MAT_TRANSFALL		0xA052
#define MAT_REFBLUR			0xA053
#define MAT_2SIDED			0xA081
#define MAT_ADDTRANS		0xA083
#define MAT_SELFILLUM		0xA084
#define MAT_WIRE				0xA085
#define MAT_WIRETHICK		0xA087
#define MAT_FACEMAP			0xA088
#define MAT_INTRAC			0xA08A
#define MAT_SOFTEN			0xA08C
#define MAT_WIRESIZE		0xA08E
#define MAT_RENDERTYPE	0xA100
#define MAT_ENDMARKER		0xAFFF

//------ sub defines of EDIT_OBJECT
#define OBJ_TRIMESH			0x4100
#define OBJ_LIGHT				0x4600
#define OBJ_CAMERA			0x4700

#define OBJ_HIDDEN			0x4010
#define OBJ_UNKNWN02		0x4012 //---- Could be shadow

//------ sub defines of OBJ_CAMERA
#define CAM_UNKNWN01		0x4710 
#define CAM_UNKNWN02		0x4720 

//------ sub defines of OBJ_LIGHT
#define LIT_OFF					0x4620
#define LIT_SPOT				0x4610
#define LIT_UNKNWN01		0x465A

//------ sub defines of OBJ_TRIMESH
#define TRI_VERTEXL			0x4110
#define TRI_FACEL2			0x4111 
#define TRI_FACEL1			0x4120
#define TRI_MATL				0x4130
#define TRI_MAPPING			0x4140
#define TRI_SMOOTH			0x4150
#define TRI_LOCAL				0x4160
#define TRI_VISIBLE			0x4165

//------ sub defs of KEYF3DS

#define KEYF_UNKNWN01		0xB009
#define KEYF_UNKNWN02		0xB00A
#define KEYF_FRAME			0xB008
#define KEYF_OBJDES			0xB002

//------  these define the different color chunk types
#define COL_RGB					0x0010
#define COL_TRU					0x0011
#define COL_UNK					0x0013

#define	RGB_FLOAT				0x0010
#define	RGB_BYTE				0x0011
#define	RGB_BYTE_GAMMA	0x0012
#define	RGB_FLOAT_GAMMA	0x0013

//----- defines for percentage chunks
#define	PERCENT_INT			0x0030
#define	PERCENT_FLOAT		0x0031

//------ defines for viewport chunks

#define TOP							0x0001
#define BOTTOM					0x0002
#define LEFT						0x0003
#define RIGHT						0x0004
#define FRONT						0x0005
#define BACK						0x0006
#define USER						0x0007
#define CAMERA					0x0008 // 0xFFFF is the actual code read from file
#define LIGHT						0x0009
#define DISABLED				0x0010
#define BOGUS						0x0011

/**	@ingroup	VFSPlugin_3DS_Group
 *	@brief		File format plugin to read/write Discreet 3DS Mesh files
 */
class VFSPlugin_3DS: public VFSPlugin{
protected:
	/**	@var		TempPolygon *m_polygons
	 *	@brief	Array of polygon data
	 */
	TempPolygon	*m_polygons;

	/**	@var		Vertex3f *m_vertex
	 *	@brief	Array of vertex data
	 */
	Vertex3f *m_vertex;

	/**	@var		int m_vertexctr
	 *	@brief	This keeps a count of the number of vertices read so far
	 */
	int m_vertexctr;

	/**	@var		int m_numvertex
	 *	@brief	The number of vertices in the mesh segment
	 */
	int	m_numvertex;

	/**	@var		int m_numpolygons
	 *	@brief	The number of polygons in the mesh segment
	 */
	int m_numpolygons;

	/**	@var		MeshFileInfo *m_fileinfo
	 *	@brief	Structure to store the mesh data
	 */
	MeshFileInfo *m_fileinfo;

	/**	@var		unsigned short chunkid
	 *	@brief	The id of the current chunk being read
	 */
	unsigned short chunkid;

	/**	@var		unsigned long chunklen
	 *	@brief	The length of the current chunk
	 */
	unsigned long chunklen;

//	These methods either read or skip a chunk of the format
 	void								SkipChunk					(void);
	void								ReadChunk					(void);

//	These methods control HOW the 3ds format is read, what sections
//	that are supported by this file format
	void								ReadMain					(void);
	void								ReadEditor				(void);
	void								ReadKeyframe			(void);
	void								ReadEditorConfig1	(void);
	void								ReadEditorConfig2	(void);
	void								ReadMaterial			(void);
	void								ReadObject				(void);
	void								ReadMesh					(void);
	void								ReadVertexData		(void);
	void								ReadPolyIndexData	(void);
	void								AssignVertexData	(IVertexBuffer *v);
	void								ProcessSurface		(void);

//	These methods read a specific size of data from the file
	char *							ReadString				(void);
	Vertex3f *					ReadVertex				(int pad=0);
	void								ReadColour				(float *rgb);
	long								ReadLong					(void);
	short								ReadShort					(void);
	float								ReadFloat					(void);
	char								ReadChar					(void);
	void								ReadPercentage		(float *percentage);
public:
											VFSPlugin_3DS			();
	virtual							~VFSPlugin_3DS		();

	virtual FileInfo *	Read							(unsigned char *buffer, unsigned int length);
	virtual char *			Write							(FileInfo *data, unsigned int &length);

	virtual char *			Type							(void);
};

#endif // #ifndef _VFSPLUGIN_3DS_H_