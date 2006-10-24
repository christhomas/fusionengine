#ifndef _DYNAMICVB_H_
	#define _DYNAMICVB_H_

#include <graphics/IVertexBuffer.h>
#include <OGLShader.h>
#include <mesh/Colour.h>
#include <mesh/Mesh.h>

class ITexture;

/**	@ingroup	OGL_VertexBuffer_Group
 *	@brief		Derived IVertexBuffer class for Dynamically assigned mesh data
 *
 *	NOTE: When dealing with the m_position, m_normal, m_texture pointers:
 *
 *	The data pointed at here is not necessarily all used 
 *	by this vertex buffer, the index pointer defines WHAT 
 *	position data to use in this mesh
 *	The dynamic vertex buffer does not own the position data, 
 *	so do not attempt to derive this class into a custom 
 *	version and in that version, delete this data.
 *	Therefore the m_position, m_normal and m_texture pointers
 *	are not to be directly manipulated, cause this will cause
 *	corruption.  If you want to do this, you should derive
 *	from the Static version of this class, where the data
 *	is COPIED into the class and therefore owns the memory
 *	and can be manipulated.
 *
 *	However, the contents of the pointers can be manipulated
 *	in whatever way the application likes, this does not affect
 *	the pointer to the data.
 */
class OGLDynamicVB:public IVertexBuffer{
protected:
	/**	@var	char *m_name
	 *	@brief	A indentifiable name for the chunk of mesh data
	 */
	std::string m_name;

	/**	@var	int m_num_vertex
	 *	@brief	The number of vertices in the mesh data
	 */
	unsigned int	m_num_vertex;

	/**	@var	int m_num_index
	 *	@brief	The number of indices in the mesh data
	 */
	unsigned int	m_num_index;
	
	/**	@var	int m_numcomp_position
	 *	@brief	The number of components (floating point values) in each position.  Typically this is 3
	 */
	unsigned int	m_numcomp_position;

	/**	@var	int m_numcomp_texcoord
	 *	@brief	The number of components (floating point values) in each texture coordinate.  Typically this is 2
	 */
	unsigned int	m_numcomp_texcoord;
	
	/**	@var	int m_bytes_position
	 *	@brief	The number of bytes per position.  This is just a pre-calculated value to make life easier
	 */
	unsigned int	m_bytes_position;

	/**	@var	int m_bytes_texcoord
	 *	@brief	The number of bytes per texcoord.  This is just a pre-calcaulated value to make life easier
	 */
	unsigned int	m_bytes_texcoord;
	
	/**	@var	int *m_index
	 *	@brief	The pointer to the meshes index data
	 */
	unsigned int	*m_index;

	/**	@var	float *m_position
	 *	@brief	The pointer to the meshes position data
	 */
	float	*m_position;

	/**	@var	float *m_normal
	 *	@brief	The pointer to the meshes normal data
	 */
	float	*m_normal;

	/**	@var	Material m_material
	 *	@brief	The material assigned to the mesh
	 */
	Material m_material;

	/**	@var	float m_smoothingangle
	 *	@brief	The maximum smoothing angle between faces before the vertex data is duplicated (flat shaded)
	 *
	 *	@todo	This is a crap explanation
	 */
	float m_smoothingangle;

	/**	@var	IShader *m_shader;
	 *	@brief	The shader used to colour/texture the mesh stored in this VB
	 */
	OGLShader m_shader;

public:
							OGLDynamicVB		();
	virtual					~OGLDynamicVB		();
	
	virtual bool			Initialise			(unsigned int nv, unsigned int ni, unsigned int nc_p, unsigned int nc_t);
	virtual void			ReleaseAll			(void);
	virtual void			SetComponents		(unsigned int p, unsigned int t);
	
	virtual void			SetName				(std::string name);
	virtual void			SetPosition			(float *p);
	virtual void			SetNormal			(float *n);
	virtual void			SetTextureLayer		(unsigned int layer, float *tc, ITexture *t);
	virtual void			SetIndex			(unsigned int *i);
	virtual void			SetColour			(Colour4f *c);
	virtual void			SetColour			(float r, float g, float b, float a);
	virtual void			SetMaterial			(Material *m);
	virtual void			SetSmoothingAngle	(float angle);

	virtual std::string		GetName				(void);
	virtual float *			GetPosition			(void);
	virtual float *			GetNormal			(void);
	virtual float *			GetTexcoord			(unsigned int layer=0);
	virtual unsigned int *	GetIndex			(void);
	virtual ITexture *		GetTexture			(unsigned int layer=0);
	virtual Colour4f *		GetColour			(void);
	virtual Material *		GetMaterial			(void);
	virtual float			GetSmoothingAngle	(void);
	
	virtual unsigned int	GetNumIndex			(void);
	
	virtual void			Render				(void);
};

#endif // #ifndef _DYNAMICVB_H_

