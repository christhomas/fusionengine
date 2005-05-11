#ifndef _WIN32DEVICE_H_
	#define _WIN32DEVICE_H_

#include <PlatformData.h>
#include <Graphics/IWindowDevice.h>
#include <Timer.h>

/** @ingroup	Win32_WindowDevice_Group
 *	@brief		Derived IWindowDevice class for the Win32 Platform
 */
class Win32Device: public IWindowDevice{
protected:
	/** @var		Win32PlatformData *m_platform
	 *	@brief	A pointer to the Win32 PlatformData structure
	 *
	 *	This structure contains all the information to run a Win32 implementation correctly, the window handle, opengl rendering context, etc.
	 */
	Win32PlatformData	*m_platform;

	/**	@var		char *m_windowname
	 *	@brief	The name of the window class
	 *
	 *	This is the name of the class which win32 uses to identify the window within it's system
	 */
	char *m_windowname;

	/**	@var		char *m_apptitle
	 *	@brief	The window title
	 *
	 *	NOTE: This is NOT the same as a UserInterface window structure, which contains it's own window title, 
	 *	this is a SYSTEM WINDOW TITLE, which if you like, is the root level window of the whole application
	 */
	char *m_apptitle;

	/** @var		Timer fps
	 *	@brief	Allows a frames per second counter to be displayed in the title
	 */
	Timer fps;
	
	virtual bool	RegisterWindow	(void);
public:
								Win32Device			();
	virtual				~Win32Device		();

	virtual bool	Open						(int width,int height,bool fullscreen);
	virtual bool	Close						(void);

	virtual void	MessageLoop			(void);

	virtual bool	IsOpen					(void);
	virtual void	SetTitle				(char *title);

	virtual void	ShowCursor			(void);
	virtual void	HideCursor			(void);

	virtual void	ShowWindow			(void);
	virtual void	HideWindow			(void);

	virtual void	Update					(void);
};

#endif // #ifndef _WIN32DEVICE_H_