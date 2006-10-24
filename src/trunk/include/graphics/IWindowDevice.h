#ifndef _IWINDOWDEVICE_H_
	#define _IWINDOWDEVICE_H_
	
#include <string>

/** @ingroup	WindowDevice_Group
 *	@brief		The base class for all Windows devices
 *
 *	Window Device's abstract class, 
 *	functionality to provide a platform 
 *	independant method of controlling a 
 *	window, whether on Win32 or X11
 */
class IWindowDevice{
public:
	/**
	 *	Constructor, no implementation
	 */
	IWindowDevice(){};

	/**
	 *	Destructor, no implementation
	 */
	virtual ~IWindowDevice(){};

	/** 
	 *	Opens a window
	 *
	 *	@param width			The width of the window
	 *	@param height			The height of the window
	 *	@param fullscreen	Whether it'll be a fullscreen window or not
	 *
	 *	@returns boolean true or false, depending on whether the window was successfully created
	 */
	virtual bool Open(unsigned int width,unsigned int height,bool fullscreen) = 0;

	/**
	 *	Closes the Window (if open)
	 *
	 *	@returns boolean true or false, depending on whether the close operation was successful
	 *
	 *	Tests whether there is an open window, is so, will close it and return true,
	 *	otherwise, it will return false and not attempt anything
	 */
	virtual bool Close(void) = 0;

	/**
	 *	Shows the Window system cursor
	 */
	virtual void ShowCursor(void) = 0;

	/**
	 *	Hides the Window system cursor
	 */
	virtual void HideCursor(void) = 0;

	/**
	 *	Shows the Window from view
	 */
	virtual void ShowWindow(void) = 0;

	/**
	 *	Hides the window from view
	 */ 
	virtual void HideWindow(void) = 0;

	/**
	 *	Processes all the Window system messages
	 */
	virtual void MessageLoop(void) = 0;

	/**
	 *	Updates the Window system display for the application
	 */
	virtual void Update(void) = 0;

	/**
	 *	Sets the title of the Window Titlebar (this is not the same as creating a window in the UserInterface object and setting that title)
	 *	This refers to the **SYSTEM WINDOW**, as in, the Window your display is contained within, whether thats a Win32 Window or X11 Window
	 */
	virtual void SetTitle(std::string title) = 0;
};

#endif // #ifndef _IWINDOWDEVICE_H_
