#include <win32/WGLDevice.h>
#include <win32/WGLGraphics.h>

/**
 *	Sets the default values for the Activate/Destroy events 
 *	to NULL, which are later overriden as required by the 
 *	application, the default screenmode will be windowed, 
 *	but this will be overridden when the screenmode is requested
 */
WGLGraphics::WGLGraphics()
{
	ActivateEvent	=	NULL;
	DestroyEvent	=	NULL;
	Window			=	NULL;
	m_width			=	0;
	m_height		=	0;
	m_bpp			=	0;
	m_fullscreen	=	false;
}

/**
 *	Destroy the Win32 window which was opened when the screenmode was requested
 */
WGLGraphics::~WGLGraphics()
{
	//	Upon killing the graphics object, close the window
	delete Window;
}

/**
 *	Sets the Video mode
 *
 *	@param width			The width of the window
 *	@param height			The Height of the window
 *	@param fullscreen	Whether the window is fullscreen or windowed
 *	@return true or false, depending on whether the mode was set successfully
 *
 *	Firstly deletes any current window and creates a new one, then 
 *	attempts to set the window with the parameters passed if successful, 
 *	you'll go onto any Win32 OpenGL (more commonly known as WGL) functions 
 *	which setup windows for use with OpenGL.  If the Window->Open() method fails
 *	the window will be deleted, the ptr set to null and this method will return false
 */
bool WGLGraphics::SetMode(int width, int height, bool fullscreen)
{
	m_width			= width;
	m_height		= height;
	m_bpp			= 32;
	m_fullscreen	= fullscreen;

	delete Window;
	Window = new WGLDevice();

	//	If a window opened successfully and if the resolution is valid
	if( Window->Open(width,height,fullscreen) == true ){

		//	Is this a fullscreen mode?
		if(m_fullscreen == true){

			//	Setup the mode
			DEVMODE devmode;

			devmode.dmSize				= sizeof(DEVMODE);
			devmode.dmDriverExtra	= 0;
			devmode.dmBitsPerPel	= 32;
			devmode.dmPelsWidth		= m_width;
			devmode.dmPelsHeight	= m_height;
			devmode.dmFields			= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			//	Speaks for itself this one......
			if(ChangeDisplaySettings(&devmode,CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL){
				//	If changing the display failed, set to windowed mode, then attempt the same display mode
				//	if it fails again, tough, just fail the function, you tried fullscreen+windowed and both
				//	didnt work, so you'll just fail the application and let them try another screenmode
				//	or perhaps I should have a fallback mode here?
				m_fullscreen = false;
				return SetMode(m_width,m_height,m_fullscreen);
			}				
		}

		//	Create the context and resize the display and finish with Initialising the opengl system
		WGLDevice *w = reinterpret_cast<WGLDevice *>(Window);
		if(w->CreateContext()){
			ResizeWindowDisplay(m_width, m_height);
			return SetupGraphics();
		}
	}

	delete Window;
	Window = NULL;

	//	Something failed, either Open(), CreateContext()
	return false;
}

/**
 *	Restores the original Win32 display mode
 */
bool WGLGraphics::RestoreMode(void)
{
	//	This basically restores the display to the original
	//	mode it was in before the opengl app took over, very
	//	important if you dont wanna piss people off by leaving
	//	them in a shit res on their lovely 21inch monitor like
	//	some games do

	return ChangeDisplaySettings(NULL,0) != DISP_CHANGE_SUCCESSFUL ? false : true;
}

void * WGLGraphics::GetExtension(std::string extension)
{
	return wglGetProcAddress(extension.c_str());
}
