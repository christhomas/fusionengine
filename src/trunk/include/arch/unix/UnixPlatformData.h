#ifndef _UNIXPLATFORMDATA_H_
	#define _UNIXPLATFORMDATA_H_

//#include <windows.h>

struct UnixPlatformData: public PlatformData
{
	virtual ~UnixPlatformData() {}

	inline virtual void Initialise( void )
	{
		//		memset(&m_pfd,0,sizeof(PIXELFORMATDESCRIPTOR));
		//		m_hdc		= NULL;
		//		m_hrc		= NULL;
		//		m_hinst	= NULL;
		//		m_hwnd	= NULL;
	}

	//	PIXELFORMATDESCRIPTOR	m_pfd;
	//	HDC						m_hdc;
	//	HGLRC					m_hrc;
	//	HINSTANCE				m_hinst;
	//	HWND					m_hwnd;
};

#endif // #ifndef _UNIXPLATFORMDATA_H_
