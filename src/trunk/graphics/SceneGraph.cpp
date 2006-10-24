// SceneGraph.cpp: implementation of the SceneGraph class.
//
//////////////////////////////////////////////////////////////////////

#include <graphics/SceneGraph.h>
#include <vector>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SceneGraph::SceneGraph()
{
	CollisionSystem = new Collision_Dummy();
}

SceneGraph::~SceneGraph()
{
	delete CollisionSystem;
	RemoveAllEntities();
}

void SceneGraph::CollisionDetection(int type)
{
	delete CollisionSystem;
	switch(type){
		case COLLISION_AABB2D:{	//	2D Axis Aligned Bounding Boxes
			CollisionSystem = new Collision_AABB2D();
		}break;

		case COLLISION_OBB2D:{	//	2D Orientated Bounding Boxes
			//	NOTE:	This doesnt work, it's most likely not going to work
			//CollisionSystem = new Collision_OBB2D();
		}break;

		case COLLISION_SPHERE2D:{
			CollisionSystem = new Collision_Sphere2D();
		}break;

		case COLLISION_AABB3D:{	//	3D Axis Aligned Bounding Boxes
			//	Code me up bitch!
		}break;
	};
}

bool SceneGraph::Render(void)
{
	CollisionSystem->Update();

	for(unsigned int a=0;a<m_displaylist.size();a++){
		m_displaylist[a]->Render();
	}

	return true;
}

//===================================
//	Camera Methods
//===================================
Camera * SceneGraph::AddCamera(std::string name)
{
	Camera *c = new Camera;

	c->SetName(name);

	m_camera.push_back(c);

	return c;
}

void SceneGraph::RemoveCamera(Camera *c)
{
	RemoveCamera(c->GetName());
}

void SceneGraph::RemoveCamera(std::string name)
{
	for(unsigned int a=0;a<m_camera.size();a++){
		if(m_camera[a]->GetName() == name){
			delete m_camera[a];
			m_camera.erase(m_camera.begin()+a);
			return;
		}
	}
}

//===================================
//	Entity Methods
//===================================
bool SceneGraph::AddEntity(Entity *e)
{
	if(e != NULL){
		m_displaylist.push_back(e);
		return true;
	}
	
	return false;
}

bool SceneGraph::RemoveEntity(unsigned int e_id)
{
	if(e_id >= 0 && e_id < m_displaylist.size())
	{
		m_displaylist.erase(m_displaylist.begin() + e_id);
		return true;
	}
	return false;
}

void SceneGraph::RemoveAllEntities(void)
{
	//	Clear the displaylist
	m_displaylist.clear();
}

Entity * SceneGraph::GetEntityPtr(unsigned int e_id)
{
	Entity *e = NULL;

	if(e_id >= 0 && e_id < m_displaylist.size())	e = m_displaylist[e_id];
	
	return e;
}

