#ifndef _ENTITY_H_
	#define _ENTITY_H_

#include <Maths/Vector.h>

class Entity;
class Mesh;
class IVertexBuffer;
//class Surface;

// Translation types
#define TRANSLATE_ABS	0
#define TRANSLATE_REL	1

/**	@typedef	meshanim_t
 *	@brief		Function pointer to a mesh manipulation function
 */
typedef void (*meshanim_t)(Entity *e);

/**	@typedef	textureanim_t
 *	@brief		Function pointer to a texture manipulation function
 */
typedef void (*textureanim_t)(IVertexBuffer *v);

/**	@ingroup	Mesh_Graphics_Group
 *	@brief		The base class for all entity objects (instances of mesh objects)
 *
 *	This type of object is used in the scenegraph, 
 *	it allows multiple instances of the same mesh 
 *	to hold different positions/rotations and other 
 *	such individual characteristics, of course, 
 *	manipulating the mesh will alter all the mesh 
 *	objects regardless of whether they are a different 
 *	entity or not.  This will be fixed in a later release
 *
 *	@todo	Fix this class so I can specify a shared mesh or not, so I can safely manipulate the mesh data and not affect other entities sharing the same mesh
 */
class Entity{
protected:
	/**	@var		Mesh *m_mesh
	 *	@brief	A Mesh object this entity object will render
	 */
	Mesh *m_mesh;
	
	/**	@var		Maths::Vector m_position
	 *	@brief	The position of the mesh
	 */
	Maths::Vector m_position;

	/**	@var		Maths::Vector m_vector
	 *	@bried	The vector of movement
	 */
	Maths::Vector	m_vector;

	/**	@var		Maths::Vector m_scale
	 *	@brief	The scale of the mesh
	 */
	Maths::Vector m_scale;

	/**	@var		Maths::Vector m_axis
	 *	@brief	The central axis of the mesh
	 */
	Maths::Vector m_axis;

	/**	@var		Maths::Vector m_angle;
	 *	@brief	The rotation of this entity on x/y/z axes, used by the collision system for example to accurately perform collisions
	 */
	Maths::Vector m_angle;

	/**	@var		float m_rotation[4]
	 *	@brief	The Axis/angle rotation data of the mesh
	*/
	float m_rotation[4];

	/**	@var		bool m_active
	 *	@brief	Whether the entity is active or not
	 */
	bool m_active;

	/**	@var		meshanimation_t mesh_anim
	 *	@brief	The animation function for the mesh
	 */
	meshanim_t mesh_anim;

	/**	@var		textureanimation_t tex_anim
	 *	@brief	The texture animation function for the mesh
	 */
	textureanim_t tex_anim;

public:
													Entity							(Mesh *mesh);
	virtual									~Entity							();

	virtual void						SetActive						(bool active);
	virtual	bool						GetActive						(void);
	
	virtual void						InitTranslate				(float x, float y, float z);
	virtual void						InitTranslate				(Maths::Vector &translate);
	virtual void						SetTranslate				(int type, float x, float y, float z);
	virtual void						SetTranslate				(int type, Maths::Vector &translate);	
	virtual void						Translate						(void);

	virtual void						SetScale						(float x, float y, float z);
	virtual void						SetScale						(Maths::Vector &scale);
	virtual void						Scale								(void);

	virtual void						SetRotate						(float angle, float x, float y, float z);
	virtual void						Rotate							(void);

	virtual void						SetColour						(float r, float g, float b, float a);

	virtual void						SetAxis							(Maths::Vector &axis);
	virtual void						SetAxis							(float x, float y, float z);
	
	virtual void						Render							(void);

	virtual void						SetMeshAnimation		(meshanim_t mesh_anim);
	virtual void						SetTextureAnimation	(textureanim_t tex_anim);

	virtual Mesh *					GetMesh							(void);

	virtual Maths::Vector *	GetPosition					(void);
	virtual Maths::Vector *	GetScale						(void);
	virtual Maths::Vector *	GetRotation					(Maths::Vector &axis);
	virtual Maths::Vector *	GetVector						(void);
};

#endif // #ifndef _ENTITY_H_
