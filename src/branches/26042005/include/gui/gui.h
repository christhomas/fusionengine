#ifndef _USERINTERFACE_H_
	#define _USERINTERFACE_H_

#include <input/InputDeviceDB.h>
#include <gui/ComponentLibrary.h>
#include <gui/GUIComponents.h>
#include <FusionSubsystem.h>

//	external predeclared classes
class SceneGraph;
class IModuleDB;

//=====================================
//	Window -	holds components which
//				make up the display of 
//				this object	
//=====================================
class Window{
private:
	bool					m_active;
	int					m_comp_id;
	char					*m_title;					//	To hold a title for the window
	WINDOWCOMPONENT	m_components;
	InputDeviceDB			*m_inputdevicedb;
	IWindowComponent		*m_highlighted_component;
	InputEvent			*m_event;
	SceneGraph			*m_scenegraph;
	
	//	For state based machine
	IWindowComponent	*m_cache_wc;
	//=========================

	void							ProcessEvents		(void);
public:
	int							m_WindowID;
								Window			(InputDeviceDB *inputdevicedb, SceneGraph *scenegraph);
	virtual						~Window			();

	virtual	bool					Initialise			(void);
	virtual void					SetTitle			(char *title);
	virtual char *					GetTitle			(void);
	virtual	void					SetActive			(bool Active);
	virtual	bool					Update			(void);
	virtual	IWindowComponent *	AddComponent		(WndComponentSetup *wcs);
	virtual	void					SetCaps			(int caps, union CapabilityData *cd);
	virtual	IWindowComponent *	GetComponentPtr	(int type,int value);
};
typedef std::vector<Window *> WINDOWLIST;

//==================
//	UserInterface 
//==================
class UserInterface
{
private:
	WINDOWLIST		m_windows;
	IModuleDB		*m_moduledb;
	SceneGraph		*m_scenegraph;
	bool				m_active;

public:
					UserInterface		(IModuleDB *moduledb,SceneGraph *scene);
	virtual			~UserInterface		();
	virtual	Window *	AddWindow		(InputDeviceDB *inputdevicedb);
	virtual	void		RemoveWindow	(Window **window);
	virtual Window *	GetWindowPtr		(char *title);
	virtual	bool		Update			(void);
	virtual	void		SetActive			(bool active);
};
typedef std::vector<UserInterface *> INTERFACELIST;

//==========================
//	UserInterfaceDB class
//==========================
class UserInterfaceDB: public FusionSubsystem{
protected:
	INTERFACELIST			m_interface_list;
	IModuleDB			*m_moduledb;
public:
	static ComponentLibrary	m_library;

							UserInterfaceDB		();
	virtual					~UserInterfaceDB		();
	virtual bool				Initialise				(void);
	virtual	UserInterface *	AddUI				(SceneGraph *scene);
	virtual	void				RemoveUI			(UserInterface *ui);
	virtual	void				RemoveAllUI			(void);
	virtual	bool				Update				(void);
	virtual	bool				AddComponentLibrary	(char *libfilename);
};

#endif // #ifndef _USERINTERFACE_H_

