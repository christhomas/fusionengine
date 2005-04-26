#ifndef _DG8STATICVB_H_
	#define _DG8STATICVB_H_

#include "DG8DynamicVB.h"

#define NULL 0

/**	@ingroup	DG8_VertexBuffer_Group
 *	@brief		Derived IVertexBuffer class for Statically assigned mesh data
 */
class DG8StaticVB: public DG8DynamicVB{
public:
											DG8StaticVB				();
	virtual							~DG8StaticVB			();
	
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

#endif // #ifndef _DG8STATICVB_H_