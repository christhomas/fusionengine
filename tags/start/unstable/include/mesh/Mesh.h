#ifndef _MESH_H_
	#define _MESH_H_

#include <mesh/Vertex.h>
#include <mesh/Colour.h>
#include <graphics/IVertexBuffer.h>
#include <vector>

/**	@ingroup	Mesh_Graphics_Group
 *	@brief		A set of components which affect how a mesh looks
 */
struct Material{
	Colour4f	colour;		/**<	Base colour	*/
	float		diffuse;		/**<	Diffuse		*/
	float		luminocity;	/**<	Luminocity	*/
	float		specular;		/**<	Specular		*/
	float		reflective;		/**<	Reflective		*/
	float		transparency;	/**<	Transparent	*/
	float		translucent;	/**<	Translucent	*/
	float		glossiness;	/**<	Glossiness	*/
	float		sharpness;	/**<	Sharpness	*/
	float		bump;		/**<	Bumpmapping	*/
};

/**	@ingroup	Mesh_Graphics_Group
 *	@brief		Structure to hold temporary polygon data
 */
struct TempPolygon{
	int numvertex;	/**<	The number of vertices in this polygon */
	int *index;			/**<	An array of vertex indices */
};
typedef std::vector<TempPolygon *> polygonlist_t;

/**	@ingroup	Mesh_Graphics_Group
 *	@brief		The base class for all Mesh objects
 */
class Mesh{
protected:
	/**	@var		int m_numvertex
	 *	@brief	The number of vertices in this mesh
	 */
	int m_numvertex;

	/**	@var		int m_numvb
	 *	@brief	The number of VertexBuffers used to represent the mesh object
	 */
	int m_numvb;

	/**	@var		Vertex3f *m_position
	 *	@brief	An array of vertex data
	 */
	Vertex3f *m_position;

	/**	@var		Vertex3f *m_normal
	 *	@brief	An array of normal data
	 */
	Vertex3f *m_normal;

	/**	@var		Vertex2f *m_texcoord
	 *	@brief	An array of texture coordinate data
	 */
	Vertex2f *m_texcoord;

	/**	@var		VertexBuffer **VertexBuffer
	 *	@brief	An array of VertexBuffer objects representing parts of the mesh
	 */
	IVertexBuffer **m_vertexbuffer;

public:
						Mesh			();
	virtual				~Mesh			();
	
	virtual void			Initialise			(int numvertex);
	
	virtual void			SetPosition		(Vertex3f *p);
	virtual void			SetNormal		(Vertex3f *n);
	virtual void			SetTexcoord		(Vertex2f *t);
	
	virtual Vertex3f *		GetPosition		(void);
	virtual Vertex3f *		GetNormal		(void);
	virtual Vertex2f *		GetTexcoord		(void);
	
	virtual int				GetNumVertex		(void);
	
	virtual void			Finalise			(void);
	virtual void			GenerateNormals	(int i=0,int j=1,int k=2);

	virtual IVertexBuffer *	AddVertexBuffer	(void);
	virtual void			AddVertexBuffer	(IVertexBuffer *v);

	virtual int				NumVertexBuffer	(void);

	virtual void			Render			(void);

	virtual IVertexBuffer *	GetVertexBuffer	(unsigned int index);

	virtual void			operator=			(Mesh &m);

	virtual void			Scale			(float x, float y, float z);
};

#endif // #ifndef _MESH_H_

