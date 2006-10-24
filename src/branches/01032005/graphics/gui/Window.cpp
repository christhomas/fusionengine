// Window.cpp: implementation of the Window class.
//
//////////////////////////////////////////////////////////////////////

#include <gui/gui.h>

Window::Window(InputDeviceDB *inputdevicedb, SceneGraph *scenegraph)
{
	m_active				= true;
	m_comp_id				= 0;
	m_highlighted_component	= NULL;
	m_inputdevicedb			= inputdevicedb;
	m_cache_wc				= NULL;
	m_scenegraph			= scenegraph;
	m_event					= NULL;

	SetTitle("DefaultWindow");
}

Window::~Window()
{
	for(unsigned int a=0;a<m_components.size();a++) UserInterfaceDB::m_library.DestroyComponent(m_components[a]);
	
	m_components.clear();
	m_event = NULL;
}

void Window::SetActive(bool Active)
{
	m_active = Active;

	//	Clear the event queue, you didnt want those other events anyway :)
	if(m_active == true) m_event = NULL;
}

bool Window::Initialise(void)
{
	if(m_inputdevicedb!=NULL){
		IInputDevice *input = m_inputdevicedb->GetDevicePtr(IInputDevice::MOUSE);
		if(input != NULL)	input->AddListener(&m_event);
		return true;
	}

	return false;
}

void Window::SetTitle(std::string title)
{
	if(title.empty() == false){
		m_title = title;
	}
}

std::string Window::GetTitle(void)
{
	return m_title;
}

IWindowComponent * Window::AddComponent(WndComponentSetup *wcs)
{
	//	Find the component creation function
	//	Create the function, test the object created ok
	if(wcs->m_componentid < UserInterfaceDB::m_library.NumberRegisteredComponents()){
		m_cache_wc = UserInterfaceDB::m_library.CreateComponent(wcs->m_componentid);
		if(m_cache_wc!=NULL){
			
			//	Initialise the component
			//	Store the component
			m_cache_wc->Initialise(wcs, m_scenegraph);
			m_components.push_back(m_cache_wc);

			//	Set the components initial state to active
			//	Delete the setup object
			//	Return a ptr to the created object
			m_cache_wc->SetActive(true);
			delete wcs;
			return m_cache_wc;
		}
	}

	return NULL;
}

void Window::SetCaps(int caps, union CapabilityData *cd)
{
	if(m_cache_wc != NULL)	m_cache_wc->SetCaps(caps,cd);
}

IWindowComponent * Window::GetComponentPtr(int type,int value)
{
//	TODO:	complete this function (rewrite if necessary)
	return NULL;
}

void Window::ProcessEvents(void)
{
	for(InputEvent *e=m_event;e!=NULL;e=e->prev)
	{
		switch(e->m_InputEvent)
		{
			case IInputDevice::EVENT_XY:
			{
				MouseXYEvent *xy = (MouseXYEvent *)e;

				if(m_highlighted_component != NULL)
				{
					if(m_highlighted_component->Highlight(xy->m_x,xy->m_y) == false) m_highlighted_component = NULL;
				}else{
					for(wndcomp_t::iterator wc=m_components.begin();wc!=m_components.end();wc++){
						if((*wc)->Highlight(xy->m_x,xy->m_y) == true)
						{
							m_highlighted_component = (*wc);
							break;
						}
					}
				}
			}break;

			case IInputDevice::EVENT_BUTTON:
			{
				InputButtonEvent *ib = (InputButtonEvent *)e;
					
				if(ib->m_EventAction == IInputDevice::BUTTON_DOWN){
					if(m_highlighted_component != NULL)	m_highlighted_component->Click();
				}
			}break;
		};
	}
	m_event = NULL;
}

bool Window::Update(void)
{
	if(m_active==true)
	{
		ProcessEvents();

		for(wndcomp_t::iterator wc=m_components.begin();wc!=m_components.end();wc++){
			if((*wc)->Update() == false)	return false;
		}
	}

	return true;
}
