#include <Mesh/Mesh.h>
#include <Fusion.h>

/**	Default Mesh Animation function
 *	@ingroup	Mesh_Graphics_Group
 *
 *	Operation:
 *		-#	Translates the mesh
 *		-#	Rotates the mesh
 *		-#	Scales the mesh
 */
void DefaultMeshAnimation(Entity *e)
{
	e->Translate();
	e->Rotate();
	e->Scale();
}

/**	Default Texture/Material Animation
 *	@ingroup	Mesh_Graphics_Group
 *
 *	This function does nothing, since no default texture manipulation can be done (it's all custom stuff)
 */
void DefaultTextureAnimation(IVertexBuffer *v){}

/**	Entity Constructor
 *
 *	@param mesh	The mesh this entity will control
 *
 *	Operation:
 *		-#	Sets the mesh object
 *		-#	Sets the active state (default is true)
 *		-#	Sets the initial mesh/texture animation functions
 *		-#	Sets the default scale (1,1,1)
 *		-#	Sets the starting rotation (default is no rotation)
 */
Entity::Entity(Mesh *mesh)
{
	m_mesh		=	mesh;
	m_active	=	true;
	mesh_anim	=	DefaultMeshAnimation;
	tex_anim	=	DefaultTextureAnimation;
	m_scale.Set(1,1,1);

	SetRotate(0,1,1,1);
}

/**	Entity Deconstructor */
Entity::~Entity(){}

/**	Sets the active state
 *
 *	@param active	Whether the entity is to be activated or not
 */
void Entity::SetActive(bool active)
{
	m_active = active;
}

/**	Retrieves the active state
 *
 *	@returns boolean true or false, depending on whether the entity is active or not
 */
bool Entity::GetActive(void)
{
	return m_active;
}

/**	Sets the Mesh Animation function
 *
 *	@param anim	The Function to use when animating the mesh
 *
 *	This allows the application to specify effectively what 
 *	happens to a mesh when it updates it's position/rotation/scale.
 *	Allowing custom animation functions to be executed as part of the entities update cycle
 *
 *	@todo	Investigate whether vertex programs/shaders would superceed this method of mesh animation
 */
void Entity::SetMeshAnimation(meshanim_t anim)
{
	mesh_anim = anim;
}

/**	Sets the Texture Animation function
 *
 *	@param	anim	The function to use when animating a texture
 *
 *	This allows the application to specify effectively what
 *	happens to a texture when it updates itself, this function is not yet
 *	fully implemented, due to a lack of practical experience on how this is best
 *	done, so for now, it serves as a handy startig point
 *
 *	@todo	Investigate better ways of animating textures
 *	@todo	Investigate whether opengl/pixel shaders are better for doing texture animation
 */
void Entity::SetTextureAnimation(textureanim_t anim)
{
	tex_anim = anim;
}

/**	Retrieves a Mesh object for this entity
 *
 *	@returns	A Mesh object this entity is controlling
 */
Mesh * Entity::GetMesh(void)
{
	return m_mesh;
}

/**	Retrieves the position of this entities mesh
 *
 *	@returns	A Maths::Vector object containing the entities position
 */
Maths::Vector * Entity::GetPosition(void)
{
	return &m_position;
}

/**	Retrieves the scale of this entities mesh
 *
 *	@returns	A Maths::Vector object containing the entities scale
 */
Maths::Vector * Entity::GetScale(void)
{
	return &m_scale;
}

/**	Retrieves the rotation of the entity on all three angles, also providing the axis
 *
 *	@param	axis	The axis of the rotation for the entity
 *	
 *	@returns	A Maths::Vector object containing the rotation on all three axes
 */
Maths::Vector * Entity::GetRotation(Maths::Vector &axis)
{
	axis = m_axis;

	return &m_angle;
}

/**	Retrieves the vector movement of this mesh
 *
 *	@returns	A Maths::Vector object containing the vector of the entity
 */
Maths::Vector * Entity::GetVector(void)
{
	return &m_vector;
}

/**	This initialises the meshes position in the world
 *
 *	@param	x	The starting x position
 *	@param	y	The starting y position
 *	@param	z	The starting z position
 *
 *	NOTE:	This method should be called first before SetTranslate because it means if you want to use the m_vector variable
 *				it wont contain an initial value which could be quite huge, this throws off the collision detection code.
 *				I just can't see how you could set a correct value for m_vector, when the first translation of a mesh, is not
 *				the meshes translation, but merely the game/app placing the mesh in the world, this shouldnt affect m_vector, since
 *				what the game/app is doing is not a movement, but a placement.  Any ideas?
 */
void Entity::InitTranslate(float x, float y, float z)
{
	m_position.Set(x,y,z);
}

/**	This initialises the meshes position in the world
 *
 *	@param	A Maths::Vector object containing the meshes starting position
 *
 *	NOTE:	This method should be called first before SetTranslate because it means if you want to use the m_vector variable
 *				it wont contain an initial value which could be quite huge, this throws off the collision detection code.
 *				I just can't see how you could set a correct value for m_vector, when the first translation of a mesh, is not
 *				the meshes translation, but merely the game/app placing the mesh in the world, this shouldnt affect m_vector, since
 *				what the game/app is doing is not a movement, but a placement.  Any ideas?
 */
void Entity::InitTranslate(Maths::Vector &translate)
{
	m_position = translate;
}

/**	Sets the position of the Mesh
 *
 *	@param	type			Whether the translation is relative or absolute in the world
 *	@param	translate	A vector containing the position to translate
 */
void Entity::SetTranslate(int type, Maths::Vector &translate)
{
	if(type == TRANSLATE_ABS){
		m_vector		=		m_position-translate;
		m_position	=		translate;
	}else if(type == TRANSLATE_REL){
		m_vector		=		translate;
		m_position	+=	translate;
	}
}

/**	Sets the position of the mesh
 *
 *	@param	type	Whether the translation is relative or absolute in the world
 *	@param	x			The x translation
 *	@param	y			The y translation
 *	@param	z			The z translation
 */
void Entity::SetTranslate(int type, float x, float y, float z)
{
	if(type == TRANSLATE_ABS){
		m_vector.Set(m_position.x-x,m_position.y-y,m_position.z-z);
		m_position.Set(x,y,z);
	}else if(type == TRANSLATE_REL){
		m_vector.Set(x,y,z);
		m_position.Set(m_position.x+x,m_position.y+y,m_position.z+z);
	}
}

/**	Translates a mesh
 *
 *	This method performs the actual translation for the mesh, it also clears the vector the mesh
 *	currently has, since next frame, it might not move, so the vector is a per frame value, where it could change
 *	at any time
 */
void Entity::Translate(void)
{
	fusion->Graphics->Translate(m_position);
}

/**	Sets the Meshes Axis of rotation
 *
 *	@param axis A vector containing it's axis of rotation on the x/y/z axes
 */
void Entity::SetAxis(Maths::Vector &axis)
{
	m_axis = axis;
}

/**	Sets the Mesh Axis of rotation
 *
 *	@param	x	The x position
 *	@param	y	The y position
 *	@param	z	The z position
 */
void Entity::SetAxis(float x, float y, float z)
{
	m_axis.Set(x,y,z);
}

/**	Sets the rotation of the mesh
 *
 *	@param angle	The angle to rotate by
 *	@param x			Whether to rotate by the angle on the x axis
 *	@param y			Whether to rotate by the angle on the y axis
 *	@param z			Whether to rotate by the angle on the z axis
 *
 *	This is axis/angle notation for a given rotation
 */
void Entity::SetRotate(float angle, float x, float y, float z)
{
	m_rotation[0] = angle;
	m_rotation[1] = x;
	m_rotation[2] = y;
	m_rotation[3] = z;

	m_angle.x +=	(angle*x);
	m_angle.y +=	(angle*y);
	m_angle.z +=	(angle*z);

	m_angle.x	=	(float)(((int)m_angle.x)%360);
	m_angle.y	=	(float)(((int)m_angle.y)%360);
	m_angle.z	=	(float)(((int)m_angle.z)%360);
}

/**	Rotates a mesh
 *
 *	This method differs from the other two methods in that it is a default rotation for the mesh.  The 
 *	rotation itself is defined in another call, this method simply executes that same rotation again
 */
void Entity::Rotate(void)
{
	//	do the rotation
	fusion->Graphics->Translate(m_axis);
	fusion->Graphics->Rotate(m_rotation);
	fusion->Graphics->Translate(-m_axis.x,-m_axis.y,-m_axis.z);
}

/**	Sets the scale of the Mesh
 *
 *	@param scale	A Vector representing it's x/y/z scalar values
 */
void Entity::SetScale(Maths::Vector &scale)
{
	m_scale = scale;
}

/**	Sets the scale of the mesh
 *
 *	@param x	The x scalar component
 *	@param y	The y scalar component
 *	@param z	The z scalar component
 */
void Entity::SetScale(float x, float y, float z)
{
	m_scale.Set(m_scale.x*x,m_scale.y*y,m_scale.z*z);
}

/**	Sets the scale of the mesh
 *
 *	This method differs from the other two methods in that it is a default scale for the mesh.  The 
 *	scale itself is defined in another call, this method simply executes that same scale again
 */
void Entity::Scale(void)
{
	fusion->Graphics->Scale(m_scale);
}

/**	Sets the colour of the mesh
 *
 *	@param r	The red component
 *	@param g	The green component
 *	@param b	The blue component
 *	@param a	The alpha component
 *
 *	@todo	Investigate whether this method should be deprecated
 */
void Entity::SetColour(float r, float g, float b, float a)
{
	IVertexBuffer *v = m_mesh->GetVertexBuffer(0);
	Material *m = v->GetMaterial();

	m->colour.r = r;
	m->colour.g = g;
	m->colour.b = b;
	m->colour.a = a;
}

/**	Renders an entity
 *
 *	Operation:
 *		-#	Test whether the entity is active or not
 *		-#	Loads an identity matrix for the mesh's translation
 *		-#	Calls the mesh animation function (DefaultMeshAnimation(Entity *) is the default)
 *		-#	Calls Mesh::Render() to render the mesh
 */
void Entity::Render(void)
{
	if(m_active == true){

		fusion->Graphics->LoadIdentity();
		
		//	Animate this entity
		mesh_anim(this);

		m_mesh->Render();
	}
}

