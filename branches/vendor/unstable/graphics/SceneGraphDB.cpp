#include <graphics/SceneGraphDB.h>
#include <Fusion.h>

SceneGraphDB::SceneGraphDB()
{
	//	By default, no scene will render
	m_scene_active	= NULL;
}

SceneGraphDB::~SceneGraphDB()
{
	//	Upon exit, release all scenes
	ReleaseAllScenes();
}

bool SceneGraphDB::Initialise(void)
{
	return true;
}

SceneGraph * SceneGraphDB::AddScene(void)
{
	//	Create a new scene
	SceneGraph *s = new SceneGraph();

	//	Store a ptr to that scene
	m_scenes.push_back(s);

	//	If this is the first scene, 
	//	the active scene ptr will be null
	//	If so, assign it to the newly created scene
	if(m_scene_active == NULL) m_scene_active = s;

	//	Return a helpful ptr to the new scene
	return s;
}

SceneGraph * SceneGraphDB::operator[](unsigned int index)
{
	SceneGraph *s = NULL;

	//	Find the scene ptr, if it exists at all
	if(index < m_scenes.size()) s = m_scenes[index];

	//	Return what you found, if you didnt find the scene, return NULL
	return s;
}

void SceneGraphDB::ReleaseScene(unsigned int index)
{
	if(index < m_scenes.size()){
		
		//	If this scene is the current active scene
		if(m_scene_active == m_scenes[index]){
			//	Find the number of scenes available, 
			if(m_scenes.size() > 0){
				//	if there is at least one, set the active 
				//	scene to the first scene available
				m_scene_active = m_scenes[0];
			}else{
				//	If there are no scenes available, set to null, so you wont render anything
				m_scene_active = NULL;
			}
		}

		//	Delete the scene
		delete m_scenes[index];
		//	Remove it's ptr from the vector
		m_scenes.erase(m_scenes.begin()+index);
	}
}

void SceneGraphDB::ReleaseAllScenes(void)
{
	//	Loop through the available scenes, deleting them
	for(unsigned int a=0;a<m_scenes.size();a++) delete m_scenes[a];
	
	//	Clear out the vector
	m_scenes.clear();
}

void SceneGraphDB::ActivateScene(unsigned int index)
{
	//	Activate a scene based upon an index value
	if(index < m_scenes.size())	m_scene_active = m_scenes[index];
}

void SceneGraphDB::ActivateScene(SceneGraph *scene)
{
	//	Activate a scene by it's ptr (more common)
	m_scene_active = scene;
}

bool SceneGraphDB::RenderScene(void)
{
	//	If there is an active scene, render it, then return the scene renders return value (whether it rendered ok or not)
	if(m_scene_active!=NULL)	return m_scene_active->Render();

	//	otherwise, just return (i got a return value, 
	//	but it's not being used for error checking or 
	//	anything, so just use it to return, theres a good boy
	return true;
}
