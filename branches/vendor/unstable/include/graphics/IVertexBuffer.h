#ifndef _IVERTEXBUFFER_H_
	#define _IVERTEXBUFFER_H_

struct	Colour4f;
struct	Material;
class		ITexture;

/**	@ingroup	Graphics_Group
 *	@brief		The base class for all VertexBuffer objects
 */
class IVertexBuffer{
protected:
	/**	@var		int m_state
	 *	@brief	Stores the state of the vertexpool
	 */
	int m_state;
public:

	/**	@enum		VertexState
	 *	Enumerated IVertexBuffer states
	 */
	enum VertexState{
		STATERESET	=	0,	/**<	(id = 0) VertexBuffer has been reset							*/
		TEXTURE			=	1,	/**<	(id = 1) VertexBuffer has texture coords assigned */
		STATIC			=	2,	/**<	(id = 2) VertexBuffer contains static data				*/
		DYNAMIC			=	4		/**<	(id = 4) VertexBuffer contains dynamic data				*/
	};
			
	/**	IVertexBuffer constructor */
	IVertexBuffer(){};

	/**	IVertexBuffer deconstructor */
	virtual ~IVertexBuffer(){};
	
	/**	Initialise the VertexBuffer's data */
	virtual bool Initialise(int nv,int ni,int nc_p,int nc_t) = 0;

	/**	Release all the VertexBuffer's data */
	virtual void ReleaseAll(void) =	0;

	/** Sets the number of components in a vertex and texture coordinate*/
	virtual	void SetComponents(int p, int t) = 0;
	
	/**	Sets the name of the mesh data */
	virtual void SetName(char *n) = 0;

	/** Sets the Position data */
	virtual void SetPosition(float *p) = 0;
	
	/** Sets the Normal data */
	virtual void SetNormal(float *n) = 0;

	/**	Sets the texture layer */
	virtual void SetTextureLayer(int layer, float *tc, ITexture *t) = 0;
	
	/** Sets the index data */
	virtual void SetIndex(int *i) = 0;

	/** Sets the colour */
	virtual void SetColour(Colour4f *c) = 0;
	
	/** Sets the colour */
	virtual void SetColour(float r, float g, float b, float a) = 0;

	/**	Sets the material */
	virtual void SetMaterial(Material *m) = 0;

	/**	Sets the maximum smoothing angle */
	virtual void SetSmoothingAngle(float angle) = 0;

	/**	Retrieves the name */
	virtual char * GetName(void) = 0;

	/** Retrieves the position data*/
	virtual float * GetPosition(void) = 0;
	
	/** Retrieves the normal data*/
	virtual float *	GetNormal(void) = 0;
	
	/** Retrieves the texture coordinate data */
	virtual float *	GetTexcoord(int layer=0) = 0;
	
	/** Retrieves the index data */
	virtual int * GetIndex(void) = 0;
	
	/** Retrieves the texture map */
	virtual ITexture * GetTexture(int layer=0) = 0;
	
	/** Retrieves the colour */
	virtual Colour4f * GetColour(void) = 0;

	/**	Retrieves the material */
	virtual Material * GetMaterial(void) = 0;

	/**	Retrieves the maximum smoothing angle for this mesh chunk */
	virtual float GetSmoothingAngle(void) = 0;
	
	/** Retrieves the number of indices (and because "1 index" == "1 vertex") or vertices*/
	virtual int GetNumIndex(void) = 0;

	/** Renders the VertexBuffer object */
	virtual void Render(void) = 0;
};

#endif // #ifndef _IVERTEXBUFFER_H_
