#include <Win32/Win32Device.h>
#include <Fusion.h>

//=======================
//	Win32 Message Loop
//=======================
//	Prevents more than one attempt to kill the window
//	basically locking one possible attempt out until
//	the first attempt has completed, then of course
//	the second attempt will fail anyway, but who cares :D
bool	DestroyWindowMutex	= true;

LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch (message)
	{
		case WM_ACTIVATE:
		{	
			//	Determine whether this is to activate or deactivate the window
			bool active = (LOWORD(wParam) == WA_INACTIVE) ? false : true;
			//	Send this activate event to the fusion engines callback (so it knows when it's active or not)
			fusion->Graphics->ActivateEvent(active);
			//	Show the cursor by default
			fusion->Graphics->Window->ShowCursor();
			//	Hide it if the app is active
			if(active == true)	fusion->Graphics->Window->HideCursor();
			return 0;
		}

		case WM_QUIT:
		case WM_CLOSE:
		{
			//	you are now destroying the window, so 
			//	lock out everyone else from trying to 
			//	simulataniously destroying the same window
			DestroyWindowMutex = false;

			//	Call the fusion's "destroy window" event
			fusion->Graphics->DestroyEvent();

			//	Now you've destroyed the window, allow others to attempt the same thing
			DestroyWindowMutex = true;

			return 0;
		}

		case WM_SIZE:
		{
			//	Resize the window as required (this will also scale the display to the window size too)
			if(fusion->Graphics != NULL) fusion->Graphics->ResizeWindowDisplay(LOWORD(lParam),HIWORD(lParam));
			return 0;
		}
	}

	return DefWindowProc(hwnd,message,wParam,lParam);
}

/**	Win32 Device Constructor
 *
 *	Retrieves a ptr to the platform data, sets the classname, window title and starts the fps counter
 */
Win32Device::Win32Device()
{
	//	Initialise all the platform data
	m_platform = reinterpret_cast<Win32PlatformData *>(fusion->GetPlatformData());

	m_windowname		=	"Win32 Subsystem";
	
	//	Initially set no title
	SetTitle("Loading: ");

	fps.Start();
}

/**	Win32 Device Deconstructor
 *
 *	Deletes all the allocated memory, stops the fps counter and calls method Win32Device::Close()
 */
Win32Device::~Win32Device()
{
	fps.Stop();

	Close();
}

/**	Registers the window to the Win32 system
 *
 *	@returns boolean true or false depending on whether registration of the class was successful of not
 */
bool Win32Device::RegisterWindow(void)
{
	//	Create the window class
	WNDCLASSEX wndClass;

	wndClass.cbSize         = sizeof(wndClass);
	wndClass.style          = CS_HREDRAW | CS_VREDRAW ;
	wndClass.lpfnWndProc    = WndProc;
	wndClass.cbClsExtra     = 0;
	wndClass.cbWndExtra     = 0;
	wndClass.hInstance      = m_platform->m_hinst;
	wndClass.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground  = NULL;
	wndClass.lpszMenuName   = NULL;
	wndClass.lpszClassName  = m_windowname.c_str();
	wndClass.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);

	// Attempt To Register The Window Class
	if(RegisterClassEx(&wndClass)!=0)	return true;

	return false;
}

/**	Opens a new Window
 *	
 *	@param	width				The width of the Win32 window
 *	@param	height			The height of the win32 window
 *	@param	fullscreen	Whether the window is a fullscreen or windowed variety
 *
 *	@returns boolean true or false, depending on whether the Window was created ok
 *
 *	Operation:
 *		-#	Closes any currently open window.
 *		-#	Restores the original Win32 display mode.
 *		-#	Attempts to register a new window.
 *		- If Successful
 *			-#	Sets up the style of the window, depending on whether it's a fullscreen window or not.
 *			-#	Adjusts the window to the correct width and height.
 *			-#	Attempts to create a Win32 Window.
 *		- If Successful
 *			-#	Sets the window to the foreground.
 *			-#	Shows the window.
 *			-#	Hides the cursor (you can make it visible again if you press the windows key, or alt-tab).
 *			-#	Returns true.
 *
 *	If anything fails, false is returned
 */
bool Win32Device::Open(unsigned int width, unsigned int height, bool fullscreen)
{
	//	Attempt to close the window, it'll fail anyway if there isnt an open one
	Close();

	//	Restore the default display mode before switching it again
	fusion->Graphics->RestoreMode();

	m_platform->m_hinst	= GetModuleHandle(NULL);			// Grab An Instance For Our Window

	//	Register a window class
	if(RegisterWindow() == true){
		RECT r = {0,0,width,height};

		//	Setup the window style
		DWORD	dwExStyle;							// Window Extended Style
		DWORD	dwStyle;								// Window Style

		dwExStyle	=	WS_EX_APPWINDOW;										// Window Extended Style
		dwStyle		= WS_CLIPSIBLINGS	| WS_CLIPCHILDREN;	// Required Window Style

		if (fullscreen){							// Are We Still In Fullscreen Mode?
			dwStyle		|= WS_POPUP;			// Windows Style
		}else{
			dwExStyle	|= WS_EX_WINDOWEDGE;		// Window Extended Style
			dwStyle		|= WS_OVERLAPPEDWINDOW;	// Windows Style
		}

		AdjustWindowRect( &r, dwStyle, FALSE );

		//	Create the window
		m_platform->m_hwnd=CreateWindowEx(
					dwExStyle,					// Extended Style For The Window
					m_windowname.c_str(),		// Class Name
					m_apptitle.c_str(),			// Window Title
					dwStyle,					// Defined Window Style
					0, 0,						// Window Position
					r.right-r.left,				// Calculate Window Width
					r.bottom-r.top,				// Calculate Window Height
					NULL,						// No Parent Window
					NULL,						// No Menu
					m_platform->m_hinst,		// Instance
					NULL);						// Dont Pass Anything To WM_CREATE

		//	Was the window created ok?
		if(m_platform->m_hwnd != NULL){
			// Slightly Higher Priority
			SetForegroundWindow(m_platform->m_hwnd);

			//	Show the window, by default, hide the win32 cursor
			ShowWindow();
			HideCursor();

			//	Evertything was ok
			return true;
		}
	}

	//	Something failed, either RegisterWindowClass() or CreateWindowEx()
	return false;
}

/**	Closes any currently open window
 *
 *	@returns boolean true or false, depending on whether there was a window to close
 *
 *	There is a DestroyWindowMutex because there are multiple methods of destroying a window.
 *	Either you can:
 *		-#	Click the X at the top right
 *		-#	Press Alt-F4
 *		-#	The application could crash, destroying the window
 *
 *	Basically, you DONT want multiple methods of closing the window happening at once, cause they happen asynchonously
 *	So it's possible you could have a race condition, Alt-F4 AND the Click X method could be used at once,
 *	causing windows to have two methods to close the window, the mutex effectively excludes one of the methods, by setting
 *	a mutex, you deny anyone else from closing the window, once you have the mutex locked.  So if the window IS closing
 *	and you Alt-F4, it'll attempt to close the window, but since the mutex is locked, fail and your original method will succeed
 *
 *	Operation:
 *		- Tests whether there is a window open.
 *		- If Window is open
 *			-#	Releases a Device Context.
 *			-#	Shows the win32 cursor.
 *			-#	Destroys the window (this is asynchronous to this function, it'll destroy itself when the WndProc next updates.
 *			-#	Unregisters the class with the window system.
 *			-#	Returns true.
 *
 *	If anything fails, false is returned.
 */
bool Win32Device::Close(void)
{
	//	Test whether a window is open or not
	if(IsOpen() == true)
	{
		//	There was a window open, so now you have to close it
		
		// Are We Able To Release The DC
		ReleaseDC(m_platform->m_hwnd,m_platform->m_hdc);
		m_platform->m_hdc = NULL;

		//	Delete the OpenGL rendering context and show the win32 cursor
		ShowCursor();

		//	Destroy the window
		if(DestroyWindowMutex==true)	DestroyWindow(m_platform->m_hwnd);
		DestroyWindowMutex	=	true;
		m_platform->m_hwnd	=	NULL;

		// Are We Able To Unregister Class
		UnregisterClass(m_windowname.c_str(),m_platform->m_hinst);
		m_platform->m_hinst=NULL;

		//	Everything was ok
		return true;
	}

	//	There wasnt an open window to close
	return false;
}

/**	Test whether a window is open or not
 *
 *	@returns boolean true or false, depending on whether there is an open window or not
 */
bool Win32Device::IsOpen(void)
{
	return m_platform->m_hwnd ? true : false;
}

/**	Sets the SYSTEM WINDOW TITLE, once again, this is not anything to do with UserInterface windows, which have their own title
 *
 *	@param title	The string to set as the window title
 *
 *	Firstly deletes any old title the window may hold, the allocates a new string, updating the title with the new title
 */
void Win32Device::SetTitle(std::string title)
{
	m_apptitle = title;
}

/** Shows the win32 Cursor
 *
 *	Because the win32 api function ShowCursor reference counts, 
 *	you want to avoid any nastiness, hence if this application 
 *	wants the cursor shown, it'll dammed well have it shown, so 
 *	it'll call ShowCursor until the cursor has a zero reference 
 *	count, which is like forcing the cursor to appear, whether 
 *	another application likes it or not
 *
 *	Because when the Win32 cursor is hidden, you restrict it's 
 *	movement to the window, you'll have to reset any cursor 
 *	restrictions by called.  ClipCursor(NULL), this removes any 
 *	restrictions the cursor had, when you originally hid the 
 *	cursor from view
 */
void Win32Device::ShowCursor(void)
{
	while(::ShowCursor(true) < 0){}

	ClipCursor(NULL);
}

/** Hides the Win32 Cursor
 *
 *	Similar to Win32Device::ShowCursor() method, 
 *	you have to deal with the reference counting 
 *	of the ShowCursor function.
 *	
 *	Retrieves the rectangle of the Win32 window, 
 *	brings that window in by 50 pixels on each 
 *	side, then sets that as the cursor clip rectangle
 *	So whilst the cursor is hidden, you can't move 
 *	outside the window boundaries, cause if you 
 *	could, you might easily lose focus of your application
 */
void Win32Device::HideCursor(void)
{
	while(::ShowCursor(false) >= 0){}

	RECT r;
	GetWindowRect(m_platform->m_hwnd,&r);
	
	r.left		+=	50;
	r.top			+=	50;
	r.right		-=	50;
	r.bottom	-=	50;

	ClipCursor(&r);
}

/** Shows the Win32 Window
 */
void Win32Device::ShowWindow(void)
{
	::ShowWindow(m_platform->m_hwnd,SW_SHOW);
}

/** Hides the Win32 Window
 */
void Win32Device::HideWindow(void)
{
	::ShowWindow(m_platform->m_hwnd,SW_HIDE);
}

/** Process all the win32 system messages
 *
 *	This function just passes them onto the WndProc function assigned to this window
 */
void Win32Device::MessageLoop(void)
{
	static MSG	msg;
	
	if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ))	if(GetMessage( &msg, NULL, 0, 0 )){
		TranslateMessage(&msg); 
		DispatchMessage(&msg);
	}
}

/**	Updates the Win32 Window
 *
 *	This method just updates the Win32 Window title with the title, followed by a frames per second counter 
 */
void Win32Device::Update(void)
{
	static int	a=0;
	static char buffer[70];
	//	I kept this char string cause it's easier than std::string, 
	//	which has a lot of combining and function calls to do the same thing

	if(a & 64){		
		fps.Tick();

		//	Build the window title (default, I should let people configure this, but fuck em for now)
		sprintf(buffer,"%s -=[ FPS:%d ]=-", m_apptitle, (1000 / (fps.GetMillisecondsDiff() >> 6)));
		SetWindowText(m_platform->m_hwnd,buffer);

		a=0;
	}else{ a++; }
}