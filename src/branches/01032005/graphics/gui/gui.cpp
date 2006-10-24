// UserInterface.cpp: implementation of the UserInterface class.
//
//////////////////////////////////////////////////////////////////////

#include <gui/gui.h>

UserInterface::UserInterface(IModuleDB *moduledb,SceneGraph *scene)
{
	m_moduledb	=	moduledb;
	m_scenegraph	=	scene;
	m_active		=	false;
}

UserInterface::~UserInterface()
{
//	empty the window list
	for(windowlist_t::iterator w=m_windows.begin();w!=m_windows.end();w++)	delete *w;

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

Window * UserInterface::GetWindowPtr(std::string title)
{
	for(windowlist_t::iterator w=m_windows.begin();w!=m_windows.end();w++){
		if((*w)->GetTitle() == title) return *w;
	}

	return NULL;
}

void UserInterface::RemoveWindow(Window **window)
{
	for(windowlist_t::iterator w=m_windows.begin();w!=m_windows.end();w++){
		if(*w == *window){
			delete *w;
			m_windows.erase(w);
		}
	}
}

bool UserInterface::Update(void)
{
	if(m_active == true){
		for(windowlist_t::iterator w=m_windows.begin();w!=m_windows.end();w++){
			(*w)->Update();
		}
	}
	
	return true;
}

void UserInterface::SetActive(bool active)
{
	m_active = active;

	for(unsigned int a=0;a<m_windows.size();a++) m_windows[a]->SetActive(active);
}
