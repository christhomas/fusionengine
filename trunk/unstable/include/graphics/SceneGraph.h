// SceneGraph.h: interface for the SceneGraph class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _SCENEGRAPH_H_
	#define _SCENEGRAPH_H_

#include <mesh/Entity.h>

#include <graphics/Collision.h>
#include <graphics/Camera.h>
#include <FusionSubsystem.h>

class SceneGraph{
protected:
	//	Internal list of entities to render
	std::vector<Entity *>	m_displaylist;
	std::vector<Camera *>	m_camera;
public:
	//	Collision system	
	Collision	*CollisionSystem;

					SceneGraph		();
	virtual			~SceneGraph		();
	virtual bool		Render			(void);

//	Camera Functions	
	virtual Camera *	AddCamera		(char *name);
	virtual void		RemoveCamera	(Camera *c);
	virtual void		RemoveCamera	(char *name);

//	Entity Functions
	virtual bool		AddEntity			(Entity *e);
	virtual bool		RemoveEntity		(unsigned int e_id);
	virtual void		RemoveAllEntities	(void);
	virtual Entity *		GetEntityPtr		(unsigned int e_id);

//	Collision Detection Functions
	virtual	void		CollisionDetection	(int type);

	enum CollisionSystemType{
		COLLISION_AABB2D=0,
		COLLISION_AABB3D,
		COLLISION_OBB2D,
		COLLISION_SPHERE2D
	};
};

typedef std::vector<SceneGraph *>	SCENELIST;

#endif // #ifndef _SCENEGRAPH_H_

