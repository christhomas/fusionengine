#ifndef _GUICOMPONENTS_H_
	#define _GUICOMPONENTS_H_

#ifdef _WIN32
	#include <win32/Timer.h>
#else
	#include <unix/Timer.h>
#endif

#include <vector>
#include <mesh/Entity.h>
#include <mesh/Overlay.h>
#include <graphics/SceneGraph.h>

//	external predeclared classes
class IFont;

//	Window Component ID values, to declare 
//	which component the app is creating
enum{
	BUTTON=0,
	TEXTBOX,
	PROGRESSBAR,
	MAXIDCODE		//	The maximum number of ID values this library supports
};

/*
 *	Capability flags
 *	Give particular functionality to components
 */

enum ComponentCaps{
	//	Tells the component it's got no caps extra to just drawing itself
	WNDCOMP_NOCAPS = 0,
	
	//	Allows resizing of the component to suit the contents
	WNDCOMP_RESIZE = 1,

	//	Allows/Denies the ability to change the contents once set
	WNDCOMP_CONTENTSLOCKED = 2,

	//	Tells the component to flash periodically
	WNDCOMP_FLASHING = 4,

	//	Tells the component to scoll it's contents horizontally 
	//	periodically at a certain speed (atm, only applies to textboxes)
	WNDCOMP_MARQUEE = 8,

	//	Tells the component that it can automatically determine 
	//	the with of the component when it recieves the data it 
	//	is storing
	WNDCOMP_AUTOWIDTH = 65535
};

//	Data to properly define the marquee properties
struct MarqueeData{
		int MarqueeMilliseconds;
		int speed;
};

union CapabilityData{
	// WNDCOMP_FLASHING
	int FlashingMilliseconds;
	
	// WNDCOMP_MARQUEE
	MarqueeData marquee;	
};

typedef bool (*guicallback_t)(void);

class WndComponentSetup{
public:
	virtual ~WndComponentSetup(){};
	unsigned int m_componentid;
};

/*
 *	IWindowComponent - The base class which all component derive from
 */
class IWindowComponent{
protected:
	guicallback_t	m_click_func;
	guicallback_t	m_highlight_func;
	bool			m_active;
	Entity			*m_entity;
public:
	int				m_component_type;

	IWindowComponent(){
		m_click_func = NULL;
		m_highlight_func = NULL;
		m_active = false;
		m_entity = NULL;	
	}
	
	virtual			~IWindowComponent	(){}
	virtual	void	SetCaps				(int caps, union CapabilityData *pd)			= 0;
	virtual	void	SetActive			(bool active)									= 0;
	virtual	bool	Update				(void)											= 0;
	virtual	void	Initialise			(WndComponentSetup *e, SceneGraph *scenegraph)	= 0;
	virtual	bool	Highlight			(int hx, int hy)								= 0;
	virtual	bool	Click				(void)											= 0;
	virtual	void	SetPosition			(float x, float y, float depth)					= 0;
	virtual	void	SetScale			(float x, float y)								= 0;
};

/*
 *	typedefs for ease of use when querying for function ptr's
 */
typedef IWindowComponent *	(*init_guicomp_t)		(void);
typedef IWindowComponent *	(*destroy_guicomp_t)	(IWindowComponent *wc);

/*
 *	Structure to hold the init and destroy function ptrs to all components
 *	**necessary to build the database of listed objects available**
 */
struct WindowComponentFunctionPtrs{
	init_guicomp_t		m_init;
	destroy_guicomp_t	m_destroy;
};

/*
 *	types to handle the libraries loaded and the components stored
 */
typedef std::vector<struct WindowComponentFunctionPtrs *>	library_t;
typedef std::vector<IWindowComponent *>						wndcomp_t;

/*
 *	Button class + setup object
 */

class Button : public IWindowComponent{
protected:
	Overlay *m_button;
public:
					Button		();
	virtual			~Button		();
	virtual void	Initialise	(WndComponentSetup *e, SceneGraph *scenegraph);
	virtual void	SetCaps		(int caps,union CapabilityData *cd){};
	virtual void	SetActive	(bool active);
	virtual bool	Update		(void);
	virtual bool	Highlight	(int hx, int hy);
	virtual bool	Click		(void);
	virtual void	SetPosition	(float x, float y, float depth);
	virtual void	SetScale	(float x, float y);
};

class ButtonSetup: public WndComponentSetup{
public:
	inline ButtonSetup(Entity *entity,guicallback_t clickfunc, guicallback_t highlightfunc){
		m_componentid	=	BUTTON;
		m_button		=	entity;
		m_clickfunc		=	clickfunc;
		m_highlightfunc	=	highlightfunc;
	}

	virtual ~ButtonSetup(){};
	Entity				*m_button;
	guicallback_t		m_clickfunc;
	guicallback_t		m_highlightfunc;
};

/*
 *	ProgressBar class + setup object
 */
typedef std::vector<int *> percentmeter_t;

class ProgressBar: public IWindowComponent{
protected:
	float			m_onepercent;
	percentmeter_t	m_meter_list;
	int				m_width, m_position_x;
	bool			m_Finalise;
public:
					ProgressBar			();
	virtual			~ProgressBar		();
	virtual void	SetCaps				(int caps, union CapabilityData *pd);
	virtual void	SetActive			(bool active);
	virtual bool	Update				(void);
	virtual void	Initialise			(WndComponentSetup *e, SceneGraph *scenegraph);
	virtual bool	Highlight			(int hx, int hy);
	virtual bool	Click				(void);
	virtual void	SetPosition			(float x, float y, float depth);
	virtual void	SetScale			(float x, float y);
	virtual void	AddPercentageMeter	(int *meter);
	virtual void	Finalise			(void);
};

class ProgressBarSetup: public WndComponentSetup{
public:
	inline ProgressBarSetup(Entity *entity, unsigned int width){
		m_componentid	=	PROGRESSBAR;
		m_entity		=	entity;
		m_width			=	width;
	}
	virtual ~ProgressBarSetup(){}

	Entity			*m_entity;
	unsigned int	m_width;
};	

/*
 *	Textbox class + setup object
 */

class Textbox: public IWindowComponent{
protected:
	IFont			*m_font;		//	font used to render this text
	std::string		m_string;

	float			m_x,			//	the x coordinate on screen
					m_y,			//	the y coordinate on screen
					m_depth;		//	the depth into the screen this textbox displays at

	int				m_caps;
	unsigned int	m_width;

	Timer			m_flashtimer;
	int				m_flashms;
	bool			m_flash;

	float			m_rgba[4];
	float			m_scale[2];
public:
						Textbox		();
	virtual				~Textbox	();
	virtual	void		SetCaps		(int caps, union CapabilityData *pd);
	virtual	void		SetActive	(bool active);
	virtual	bool		Update		(void);
	virtual	void		Initialise	(WndComponentSetup *e, SceneGraph *scenegraph);
	virtual	void		UpdateString(std::string str);
	virtual void		UpdateString(unsigned int number);
	virtual	bool		Highlight	(int hx, int hy)	{	return false;	};
	virtual	bool		Click		(void)				{	return false;	};
	virtual	void		SetPosition	(float x, float y, float depth);
	virtual	void		SetScale	(float x, float y);
	virtual	void		SetColour	(float r, float g, float b, float a);
	virtual	std::string	GetString	(void);
};

class TextBoxSetup: public WndComponentSetup{
public:
	inline TextBoxSetup(float x,float y, float depth, IFont *f, 
							std::string t, int caps=WNDCOMP_NOCAPS,int width=WNDCOMP_AUTOWIDTH){
		m_componentid	= TEXTBOX;
		m_x				= x;
		m_y				= y;
		m_depth			= depth;
		m_font			= f;
		m_text			= t;
		m_caps			= caps;
		m_width			= width;
	}
	virtual ~TextBoxSetup(){};
	
	float		m_x,m_y,m_depth;
	int			m_width,m_caps;
	IFont		*m_font;
	std::string m_text;
};

#endif // #ifndef _GUICOMPONENTS_H_
