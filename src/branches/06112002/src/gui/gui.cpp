// UserInterface.cpp: implementation of the UserInterface class.
//
//////////////////////////////////////////////////////////////////////

#include <GUI/gui.h>

UserInterface::UserInterface(IModuleDB *moduledb,SceneGraph *scene)
{
	m_moduledb		=	moduledb;
	m_scenegraph	=	scene;
	m_active		=	false;
}

UserInterface::~UserInterface()
{
//	empty the window list
	for(WINDOWLIST::iterator w=m_windows.begin();w!=m_windows.end();w++)	delete *w;

	m_windows.clear();
}

Window * UserInterface::AddWindow(InputDeviceDB *inputdevicedb)
{
	Window *w	= new Window(inputdevicedb, m_scenegraph);
	if(w->Initialise() == true){
		m_windows.push_back(w);
		return w;
	}
	delete w;
	return NULL;
}

Window * UserInterface::GetWindowPtr(char *title)
{
	for(WINDOWLIST::iterator w=m_windows.begin();w!=m_windows.end();w++){
		if(strcmp((*w)->GetTitle(), title) == 0) return *w;
	}

	return NULL;
}

void UserInterface::RemoveWindow(Window **window)
{
	for(WINDOWLIST::iterator w=m_windows.begin();w!=m_windows.end();w++){
		if(*w == *window){
			delete *w;
			m_windows.erase(w);
		}
	}
}

bool UserInterface::Update(void)
{
	if(m_active == true){
		for(WINDOWLIST::iterator w=m_windows.begin();w!=m_windows.end();w++){
			(*w)->Update();
		}
	}
	
	return true;
}

void UserInterface::SetActive(bool active)
{
	m_active = active;

	for(int a=0;a<m_windows.size();a++) m_windows[a]->SetActive(active);
}
