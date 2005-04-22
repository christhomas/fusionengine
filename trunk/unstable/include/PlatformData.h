#ifndef _PLATFORMDATA_H_
	#define _PLATFORMDATA_H_

struct PlatformData{
	virtual ~PlatformData(){}
	inline virtual void Initialise(void) = 0;
};

#ifdef _WIN32
	#include <win32/Win32PlatformData.h>
#else
	#include <unix/UnixPlatformData.h>
#endif 

#endif	// #ifndef _PLATFORMDATA_H_

