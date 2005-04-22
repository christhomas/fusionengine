#ifndef _DG8DYNAMICVB_H_
	#define _DG8DYNAMICVB_H_

#include <Graphics/IVertexBuffer.h>
#include <d3d8.h>

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE)

#define DG8COLOUR(r,g,b,a)	(r << 24) | (g << 16) | (b << 8) | a	

/**	@ingroup	DG8_VertexBuffer_Group
 *	@brief		Derived IVertexBuffer class for Dynamically assigned mesh data
 */
class DG8DynamicVB:public IVertexBuffer{
protected:
	IDirect3DVertexBuffer8	*m_vertexbuffer;
	IDirect3DIndexBuffer8		*m_indexbuffer;

	int						m_numvertex;
	int						m_numindex;
	D3DMATERIAL8	m_material;

	struct CUSTOMVERTEX{
		float			px,py,pz;
		float			nx,ny,nz;
		unsigned int	colour;
		//	TODO:	Add texture coordinates to this vertex format
	};

	float m_smoothingangle;

	char *m_name;
	
public:
											DG8DynamicVB			();
	virtual							~DG8DynamicVB			();
	
	virtual bool				Initialise				(int nv, int ni, int nc_p, int nc_t);
	virtual void				ReleaseAll				(void);
	virtual	void				SetComponents			(int p, int t);
	
	virtual void				SetName						(char *n);
	virtual void				SetPosition				(float *p);
	virtual void				SetNormal					(float *n);
	virtual void				SetTexcoord				(float *t);
	virtual void				SetIndex					(int *i);
	virtual void				SetTexture				(ITexture *t);
	virtual void				SetColour					(Colour4f *c);
	virtual void				SetColour					(float r, float g, float b, float a);
	virtual void				SetMaterial				(Material *m);
	virtual void				SetSmoothingAngle	(float angle);

	virtual char *			GetName						(void);
	virtual float *			GetPosition				(void);
	virtual float *			GetNormal					(void);
	virtual float *			GetTexcoord				(void);
	virtual int *				GetIndex					(void);
	virtual ITexture *	GetTexture				(void);
	virtual Colour4f *	GetColour					(void);
	virtual Material *	GetMaterial				(void);
	virtual float				GetSmoothingAngle	(void);

	virtual int					GetNumIndex				(void);													//	Returns the number of indices
	
	virtual void				Render						(void);
};

#endif // #ifndef _DG8DYNAMICVB_H_