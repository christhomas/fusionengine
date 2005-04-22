#ifndef _WIN32PLATFORMDATA_H_
	#define _WIN32PLATFORMDATA_H_

#include <windows.h>

struct Win32PlatformData: public PlatformData{
	virtual ~Win32PlatformData(){}
	
	inline virtual void Initialise(void){
		memset(&m_pfd,0,sizeof(PIXELFORMATDESCRIPTOR));
		m_hdc		= NULL;
		m_hrc		= NULL;
		m_hinst	= NULL;
		m_hwnd	= NULL;
	}

	PIXELFORMATDESCRIPTOR	m_pfd;
	HDC										m_hdc;
	HGLRC									m_hrc;
	HINSTANCE							m_hinst;
	HWND									m_hwnd;
};

#endif // #ifndef _WIN32PLATFORMDATA_H_
