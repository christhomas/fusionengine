#ifndef _OGLSTATICVB_H_
	#define _OGLSTATICVB_H_

#include "OGLDynamicVB.h"

/**	@ingroup	OGL_VertexBuffer_Group
 *	@brief		Derived OGLDynamicVB class for Statically assigned mesh data
 *
 *	NOTE:	This class is not implemented.
 */
class OGLStaticVB:public OGLDynamicVB{
protected:
	/**	@var		float *m_texcoord
	 *	@brief	The pointer to the meshes texture coordinate data
	 */
	float	*m_texcoord;

	/**	@var		ITexture *m_texture
	 *	@brief	The texture which to map onto this mesh data
	 */
	ITexture *m_texture;
public:
							OGLStaticVB		();
	virtual					~OGLStaticVB	();
		
	virtual bool			Initialise		(unsigned int nv, unsigned int ni, unsigned int nc_p, unsigned int nc_t);
	virtual void			ReleaseAll		(void);
	virtual void			SetComponents	(unsigned int p, unsigned int t);
	
	virtual void			SetName			(std::string name);
	virtual void			SetPosition		(float *p);
	virtual void			SetNormal		(float *n);
	virtual void			SetTexcoord		(float *t);
	virtual void			SetIndex		(unsigned int *i);
	virtual void			SetTexture		(ITexture *t);
	virtual void			SetColour		(Colour4f *c);
	virtual void			SetColour		(float r, float g, float b, float a);
	virtual void			SetMaterial		(Material *m);

	virtual std::string		GetName			(void);
	virtual float *			GetPosition		(void);
	virtual float *			GetNormal		(void);
	virtual float *			GetTexcoord		(unsigned int layer=0);
	virtual unsigned int *	GetIndex		(void);
	virtual ITexture *		GetTexture		(unsigned int layer=0);
	virtual Colour4f *		GetColour		(void);
	virtual Material *		GetMaterial		(void);

	virtual unsigned int	GetNumIndex		(void);	//	Returns the number of indices
	
	virtual void			Render			(void);
};

#endif // #ifndef _OGLSTATICVB_H_

