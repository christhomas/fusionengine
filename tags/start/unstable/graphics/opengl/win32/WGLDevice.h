#ifndef _WGLDEVICE_H_
	#define _WGLDEVICE_H_

#include <Win32/Win32Device.h>

/** @ingroup	Win32_WindowDeviceGroup
 *	@brief		Derived Win32Device class for the WGL Interface
 */
class WGLDevice: public Win32Device{
public:
				WGLDevice	();
	virtual		~WGLDevice	();

	virtual bool	Close		(void);
	virtual bool	CreateContext	(void);
	virtual void	DeleteContext	(void);
	virtual void	SetContext	(void);
	virtual void *	GetContext	(void);
	virtual void	Update		(void);
};

#endif // #ifndef _WGLDEVICE_H_
