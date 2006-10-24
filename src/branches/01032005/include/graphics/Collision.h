#ifndef _COLLISION_H_
	#define _COLLISION_H_

#include <mesh/Entity.h>
#include <vector>
#include <Rect.h>

class CollisionPair{
public:
	int		GroupID[2];
	Entity	*entity[2];
};
	
typedef std::vector<Entity *>			entitylist_t;		//	Defines a list of entitys which defines a group
typedef std::vector<entitylist_t>		grouplist_t;		//	Defines a list of groups the system handles
typedef std::vector<int>				complist_t;			//	Defines a list of pair of groups to cd each other
typedef std::vector<CollisionPair *>	collisionpair_t;	//	Defines a list of entitys which has collided

//	Base collision system
class Collision{
protected:
	grouplist_t		m_groups;
	complist_t		m_comparegroups;

	int				m_reduce_x,
					m_reduce_y,
					m_reduce_z;

	enum{SRC=0,DST};

	void (Collision::*CorrectVectors)(Entity *e1,Entity *e2);

	friend class	SceneGraph;
	virtual void	Update					(void);
	virtual void	CalculateBox			(Entity *e,int id)	= 0;
	virtual bool	Intersect				(void)				= 0;
	
	// this method is used when no vector correction is requested (it's a dummy)
	virtual void	dum_VectorCorrection	(Entity *e1,Entity *e2){}

	virtual void	def_VectorCorrection	(Entity *e1,Entity *e2) = 0;
public:
	collisionpair_t	m_collisions;

					Collision				();
	virtual			~Collision				();
	virtual void	AddEntity				(unsigned int GroupID, Entity *e);
	virtual void	AddGroupCollide			(unsigned int GroupID, unsigned int *CollideGroupID, int num_groups); 
	virtual void	AddGroupCollide			(unsigned int GroupID, unsigned int CollideGroupID); 
	virtual void	ResizeCollisionArea		(int percentx,int percenty, int percentz=0);
	virtual void	EnableVectorCorrection	(void);
	virtual void	DisableVectorCorrection	(void);
};

//	Derived dummy collision object
//	NOTE:	I figured calling this functions update() method and doing a empty function call
//			would be better than having a test against null on the ptr each scenegraph update
//			(regardless of whether there is a collision object allocated or not) this way,
//			there is no testing, you just execute the blank function and thats it
class Collision_Dummy: public Collision{
protected:
	virtual void	Update					(void){}
	virtual void	CalculateBox			(Entity *e, int id){}
	virtual bool	Intersect				(){return false;}
	virtual void	def_VectorCorrection	(Entity *e1,Entity *e2){};
public:
					Collision_Dummy			(){}
	virtual			~Collision_Dummy		(){}
};

//	Derived Axis Aligned Bounding Box collision system
class Collision_AABB2D: public Collision{
private:
	Rect rect[2];
protected:
	virtual void	CalculateBox			(Entity *e, int id);
	virtual bool	Intersect				();
	virtual void	def_VectorCorrection	(Entity *e1,Entity *e2);
public:
					Collision_AABB2D		();
	virtual			~Collision_AABB2D		();
};

class Collision_OBB2D: public Collision_AABB2D{
private:
	//	some data more applicable to OBB's
	Maths::Vector	rect[2][4];
	bool			intersect[2][4];
protected:
	virtual void	CalculateBox			(Entity *e, int id);
	virtual bool	Intersect				(void);
	virtual void	def_VectorCorrection	(Entity *e1,Entity *e2);
public:
					Collision_OBB2D			();
	virtual			~Collision_OBB2D		();	
};

class Collision_Sphere2D: public Collision{
private:
	//	Some private data more applicable to this implementation
	Maths::Vector	m_centre[2][2];
	int				m_rad[2];
	double			m_time;
protected:
	virtual void	CalculateBox			(Entity *e, int id);
	virtual bool	Intersect				();
	virtual void	def_VectorCorrection	(Entity *e1,Entity *e2);
public:
					Collision_Sphere2D		();
	virtual			~Collision_Sphere2D		();
};

#endif // #ifndef _COLLISION_H_
