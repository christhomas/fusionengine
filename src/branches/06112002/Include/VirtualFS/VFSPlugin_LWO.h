#ifndef _VFSPLUGIN_LWO_H_
	#define _VFSPLUGIN_LWO_H_

#include <VirtualFS/VirtualFS.h>
#include <Mesh/Mesh.h>

//================================
//	Lightwave Chunk ID Tags
//================================
#define MakeID(a,b,c,d)	(((a)<<24)|((b)<<16)|((c)<<8)|(d))

#define ID_FORM	MakeID('F','O','R','M')
#define ID_LWO2	MakeID('L','W','O','2')
#define ID_TAGS	MakeID('T','A','G','S')

/* top-level chunks */
#define ID_LAYR MakeID('L','A','Y','R')
#define ID_TAGS MakeID('T','A','G','S')
#define ID_PNTS MakeID('P','N','T','S')
#define ID_BBOX MakeID('B','B','O','X')
#define ID_VMAP MakeID('V','M','A','P')
#define ID_VMAD MakeID('V','M','A','D')
#define ID_POLS MakeID('P','O','L','S')
#define ID_PTAG MakeID('P','T','A','G')
#define ID_ENVL MakeID('E','N','V','L')
#define ID_CLIP MakeID('C','L','I','P')
#define ID_SURF MakeID('S','U','R','F')
#define ID_DESC MakeID('D','E','S','C')
#define ID_TEXT MakeID('T','E','X','T')
#define ID_ICON MakeID('I','C','O','N')

/* polygon types */
#define ID_FACE	MakeID('F','A','C','E')
#define ID_CURV	MakeID('C','U','R','V')
#define ID_PTCH	MakeID('P','T','C','H')
#define ID_MBAL	MakeID('M','B','A','L')
#define ID_BONE	MakeID('B','O','N','E')

/* polygon tags */
#define ID_SURF	MakeID('S','U','R','F')
#define ID_PART	MakeID('P','A','R','T')
#define ID_SMGP	MakeID('S','M','G','P')

/* envelopes */
#define ID_PRE	MakeID('P','R','E',' ')
#define ID_POST	MakeID('P','O','S','T')
#define ID_KEY	MakeID('K','E','Y',' ')
#define ID_SPAN	MakeID('S','P','A','N')
#define ID_TCB	MakeID('T','C','B',' ')
#define ID_HERM	MakeID('H','E','R','M')
#define ID_BEZI	MakeID('B','E','Z','I')
#define ID_BEZ2	MakeID('B','E','Z','2')
#define ID_LINE	MakeID('L','I','N','E')
#define ID_STEP	MakeID('S','T','E','P')

/* clips */
#define ID_STIL  MakeID('S','T','I','L')
#define ID_ISEQ  MakeID('I','S','E','Q')
#define ID_ANIM  MakeID('A','N','I','M')
#define ID_XREF  MakeID('X','R','E','F')
#define ID_STCC  MakeID('S','T','C','C')
#define ID_TIME  MakeID('T','I','M','E')
#define ID_CONT  MakeID('C','O','N','T')
#define ID_BRIT  MakeID('B','R','I','T')
#define ID_SATR  MakeID('S','A','T','R')
#define ID_HUE   MakeID('H','U','E',' ')
#define ID_GAMM  MakeID('G','A','M','M')
#define ID_NEGA  MakeID('N','E','G','A')
#define ID_IFLT  MakeID('I','F','L','T')
#define ID_PFLT  MakeID('P','F','L','T')

/* surfaces */
#define ID_GLOW  MakeID('G','L','O','W')
#define ID_LCOL  MakeID('L','C','O','L')
#define ID_CMNT  MakeID('C','M','N','T')
#define ID_COLR  MakeID('C','O','L','R')
#define ID_LUMI  MakeID('L','U','M','I')
#define ID_DIFF  MakeID('D','I','F','F')
#define ID_SPEC  MakeID('S','P','E','C')
#define ID_GLOS  MakeID('G','L','O','S')
#define ID_REFL  MakeID('R','E','F','L')
#define ID_RFOP  MakeID('R','F','O','P')
#define ID_RIMG  MakeID('R','I','M','G')
#define ID_RSAN  MakeID('R','S','A','N')
#define ID_TRAN  MakeID('T','R','A','N')
#define ID_TROP  MakeID('T','R','O','P')
#define ID_TIMG  MakeID('T','I','M','G')
#define ID_RIND  MakeID('R','I','N','D')
#define ID_TRNL  MakeID('T','R','N','L')
#define ID_BUMP  MakeID('B','U','M','P')
#define ID_SMAN  MakeID('S','M','A','N')
#define ID_SIDE  MakeID('S','I','D','E')
#define ID_CLRH  MakeID('C','L','R','H')
#define ID_CLRF  MakeID('C','L','R','F')
#define ID_ADTR  MakeID('A','D','T','R')
#define ID_SHRP  MakeID('S','H','R','P')
#define ID_LINE  MakeID('L','I','N','E')
#define ID_LSIZ  MakeID('L','S','I','Z')
#define ID_ALPH  MakeID('A','L','P','H')
#define ID_AVAL  MakeID('A','V','A','L')
#define ID_GVAL  MakeID('G','V','A','L')
#define ID_BLOK  MakeID('B','L','O','K')

/* texture layer */
#define ID_TYPE  MakeID('T','Y','P','E')
#define ID_CHAN  MakeID('C','H','A','N')
#define ID_NAME  MakeID('N','A','M','E')
#define ID_ENAB  MakeID('E','N','A','B')
#define ID_OPAC  MakeID('O','P','A','C')
#define ID_FLAG  MakeID('F','L','A','G')
#define ID_PROJ  MakeID('P','R','O','J')
#define ID_STCK  MakeID('S','T','C','K')
#define ID_TAMP  MakeID('T','A','M','P')

/* texture coordinates */
#define ID_TMAP  MakeID('T','M','A','P')
#define ID_AXIS  MakeID('A','X','I','S')
#define ID_CNTR  MakeID('C','N','T','R')
#define ID_SIZE  MakeID('S','I','Z','E')
#define ID_ROTA  MakeID('R','O','T','A')
#define ID_OREF  MakeID('O','R','E','F')
#define ID_FALL  MakeID('F','A','L','L')
#define ID_CSYS  MakeID('C','S','Y','S')

/* image map */
#define ID_IMAP  MakeID('I','M','A','P')
#define ID_IMAG  MakeID('I','M','A','G')
#define ID_WRAP  MakeID('W','R','A','P')
#define ID_WRPW  MakeID('W','R','P','W')
#define ID_WRPH  MakeID('W','R','P','H')
#define ID_VMAP  MakeID('V','M','A','P')
#define ID_AAST  MakeID('A','A','S','T')
#define ID_PIXB  MakeID('P','I','X','B')

/* procedural */
#define ID_PROC  MakeID('P','R','O','C')
#define ID_COLR  MakeID('C','O','L','R')
#define ID_VALU  MakeID('V','A','L','U')
#define ID_FUNC  MakeID('F','U','N','C')
#define ID_FTPS  MakeID('F','T','P','S')
#define ID_ITPS  MakeID('I','T','P','S')
#define ID_ETPS  MakeID('E','T','P','S')

/* gradient */
#define ID_GRAD  MakeID('G','R','A','D')
#define ID_GRST  MakeID('G','R','S','T')
#define ID_GREN  MakeID('G','R','E','N')
#define ID_PNAM  MakeID('P','N','A','M')
#define ID_INAM  MakeID('I','N','A','M')
#define ID_GRPT  MakeID('G','R','P','T')
#define ID_FKEY  MakeID('F','K','E','Y')
#define ID_IKEY  MakeID('I','K','E','Y')

/* shader */
#define ID_SHDR  MakeID('S','H','D','R')
#define ID_DATA  MakeID('D','A','T','A')

/**	@ingroup	VFSPlugin_LWO_Group
 *	@brief		File format plugin to read/write Newtek Lightwave 6+ Mesh files
 */
class VFSPlugin_LWO: public VFSPlugin{
protected:
	polygonlist_t								polygons;
	std::vector<long>						polygontags;

	IVertexBuffer *m_vb;

	// file info object
	MeshFileInfo	*m_fileinfo;

	virtual	long					ReadChunk							(void);
	virtual	long					ReadSubChunk					(void);
	//================================
	//	Chunks
	//================================
	virtual	void					read_tags							(long length);
	virtual	void					read_layr							(long length);
	virtual	void					read_pnts							(long length);
	virtual	void					read_pols							(long length);
	virtual	void					read_bbox							(long length);
	virtual	void					read_clip							(long length);
	virtual	void					read_envl							(long length);
	virtual	void					read_vmap							(long length);
	virtual	void					read_ptag							(long length);
	virtual	void					read_unkn							(long id,long length);
	//================================
	//	Subchunks
	//================================
	virtual	void					read_colr							(long length);
	virtual	void					read_diff							(long length);
	virtual	void					read_lumi							(long length);
	virtual	void					read_spec							(long length);
	virtual	void					read_refl							(long length);
	virtual	void					read_tran							(long length);
	virtual	void					read_trnl							(long length);
	virtual	void					read_glos							(long length);
	virtual	void					read_shrp							(long length);
	virtual	void					read_sman							(long length);
	virtual	void					read_bump							(long length);
	virtual	void					read_face							(long length);
	virtual	void					read_surf							(long length);
	virtual	void					read_curv							(long length);
	virtual	void					read_ptch							(long length);
	virtual	void					read_mbal							(long length);
	virtual	void					read_bone							(long length);

	//================================
	//	Surface Processing Methods
	//================================
	virtual	void					ProcessSurface				(void);
	virtual	void					SortPolygonTags				(void);
	virtual	unsigned int	CountPolygons					(int index, int &counter);
	virtual void					AssignBuffers					(int index);

	//================================
	//	File read functions
	//================================
	virtual	long					ReadLong							(void);
	virtual	short					ReadShort							(void);
	virtual	char					ReadChar							(void);
	virtual	long					ReadVariableLength		(long *length);
	virtual	Vertex3f *		ReadVertex						(void);
	virtual	char *				ReadString						(long *length);
	virtual	float					ReadFloat							(void);
	virtual	Colour4f *		ReadColour						(long number=3);
	virtual	void					ReadPad								(long padlength=1);

public:
												VFSPlugin_LWO					();
	virtual								~VFSPlugin_LWO				();

	virtual FileInfo *		Read									(unsigned char *buffer, unsigned int length);
	virtual char *				Write									(FileInfo *data, unsigned int &length);

	virtual char *				Type									(void);
};

#endif // #ifndef _VFSPLUGIN_LWO_H_