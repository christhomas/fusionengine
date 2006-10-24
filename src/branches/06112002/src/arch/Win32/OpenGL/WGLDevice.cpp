#include <Win32/OpenGL/WGLDevice.h>

/** WGL Device Constructor */
WGLDevice::WGLDevice(){}

/** WGL Device Deconstructor */
WGLDevice::~WGLDevice(){}

/** Closes a WGL Enabled window
 *
 *	This derived version deletes the WGL Device 
 *	context, then calls Win32Device::Close() to 
 *	finish off the generic window functions.
 */
bool WGLDevice::Close(void)
{
	DeleteContext();
	return Win32Device::Close();
}	

/** Creates a WGL Device Context
 *
 *	@returns boolean true or false, depending on whether there 
 *	was a window open, if not, will return false to indicate no window available.
 *
 *	Operation:
 *		- Finds whether there is a window open.
 *		-	If Window open
 *			-#	resets the Pixel Format Descriptor.
 *			-#	Creates another Pixel Format Descriptor.
 *			-#	Tests the Pixel format, whether it's compatible.
 *		- If Pixel Format succeeds
 *			-#	Sets the Pixel Format.
 *			-#	Creates the WGL Device Context.
 *			-#	Makes the Device Context current (sets it to active, basically).
 *			-#	Returns true.
 *
 *	If anything fails, false is returned
 */
bool WGLDevice::CreateContext(void)
{
	//	Is there a window open
	if(IsOpen() == true)
	{
		//	Create the Pixel format descriptor
		memset(&m_platform->m_pfd,0,sizeof(m_platform->m_pfd));
		
		m_platform->m_pfd.nSize			= sizeof(m_platform->m_pfd);																  // Size Of This Pixel Format Descriptor
		m_platform->m_pfd.nVersion		= 1;																											  // Version Number
		m_platform->m_pfd.dwFlags		= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; // Format Must Support Window,OpenGL,Double Buffering
		m_platform->m_pfd.iPixelType	= PFD_TYPE_RGBA;																						// Request An RGBA Format
		m_platform->m_pfd.cColorBits	= 32;																												// Select Our Color Depth
		m_platform->m_pfd.cDepthBits	= 16;																												// 16Bit Z-Buffer (Depth Buffer)
		m_platform->m_pfd.iLayerType	= PFD_MAIN_PLANE;																						// Main Drawing Layer

		//	Get a device context for this window
		m_platform->m_hdc=GetDC(m_platform->m_hwnd);

		int PixelFormat;

		//	Choose a pixel format, Set the window to that format
		if(PixelFormat = ChoosePixelFormat(m_platform->m_hdc,&m_platform->m_pfd)){
			if(SetPixelFormat(m_platform->m_hdc,PixelFormat,&m_platform->m_pfd)){
				
				//	Create an OpenGL Rendering context and make that context current
				if(m_platform->m_hrc=wglCreateContext(m_platform->m_hdc)){
					if(wglMakeCurrent(m_platform->m_hdc,m_platform->m_hrc)){
						
						//	Everything was ok, context created
						return true;
					}
				}
			}
		}
	}

	//	Something failed, either a window wasnt open, or one other function failed
	return false;
}

/** Deletes the WGL Device Context */
void WGLDevice::DeleteContext(void)
{
	wglDeleteContext(m_platform->m_hrc);
}

/** Sets the WGL Device's context to current (makes it active) */
void WGLDevice::SetContext(void)
{
	wglMakeCurrent(m_platform->m_hdc, m_platform->m_hrc);
}

/**	Retrieves the WGL Devices context */
void * WGLDevice::GetContext(void)
{
	return (void *)m_platform->m_hrc;
}

/** Updates the WGL Window Device
 *	First updates the Win32 Window, then Updates OpenGl's display
 */
void WGLDevice::Update(void)
{
	Win32Device::Update();

	SwapBuffers(m_platform->m_hdc);
}