#ifndef _SCENEGRAPHDB_H_
	#define _SCENEGRAPHDB_H_

#include <graphics/SceneGraph.h>

class SceneGraphDB: public FusionSubsystem{
protected:
	scenelist_t	m_scenes;
	SceneGraph	*m_scene_active;

public:
							SceneGraphDB		();
	virtual					~SceneGraphDB		();
	virtual bool			Initialise			(void);
	
	virtual SceneGraph *	operator[]			(unsigned int index);
	
	virtual SceneGraph *	AddScene			(void);

	virtual void			ReleaseAllScenes	(void);
	virtual void			ReleaseScene		(unsigned int index);

	virtual void			ActivateScene		(unsigned int index);
	virtual void			ActivateScene		(SceneGraph *scene);	
		
	virtual bool			RenderScene			(void);
};

#endif // #ifndef _SCENEGRAPHDB_H_
