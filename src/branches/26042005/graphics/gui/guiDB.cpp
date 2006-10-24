#include <gui/gui.h>
#include <Fusion.h>

ComponentLibrary UserInterfaceDB::m_library = ComponentLibrary();

// This function prototype are for use with the standard objects compiled into this dll
void update(LIBRARY *library);

UserInterfaceDB::UserInterfaceDB(){}

UserInterfaceDB::~UserInterfaceDB()
{
	RemoveAllUI();
}

bool UserInterfaceDB::Initialise(void)
{
	m_moduledb = fusion->GetModuleDB();

	m_library.Initialise(m_moduledb);
	m_library.AddLibrary(update);

	return true;
}

UserInterface * UserInterfaceDB::AddUI(SceneGraph *scene)
{
	UserInterface *ui = new UserInterface(m_moduledb,scene);
	m_interface_list.push_back(ui);
	return ui;
}

void UserInterfaceDB::RemoveUI(UserInterface *ui)
{
	for(INTERFACELIST::iterator temp=m_interface_list.begin();temp!=m_interface_list.end();temp++)
	{
		if(*temp == ui){
			delete *temp;
			m_interface_list.erase(temp);
		}
	}
}

void UserInterfaceDB::RemoveAllUI(void)
{
	for(INTERFACELIST::iterator ui=m_interface_list.begin();ui!=m_interface_list.end();ui++)
	{
		delete *ui;
	}
	m_interface_list.clear();
}

bool UserInterfaceDB::Update(void)
{
	for(INTERFACELIST::iterator ui=m_interface_list.begin();ui!=m_interface_list.end();ui++){
		if((*ui)->Update() == false){
			return false;
		}
	}

	return true;
}

bool UserInterfaceDB::AddComponentLibrary(char *libfilename)
{
	m_library.AddLibrary(libfilename);

	return true;
}
