#ifndef _PLATFORMDATA_H_
	#define _PLATFORMDATA_H_

struct PlatformData{
	virtual ~PlatformData(){}
	inline virtual void Initialise(void) = 0;
};

#ifdef _WIN32
	#include "Win32\Win32PlatformData.h"
#endif	// #ifdef _WIN32

#endif	// #ifndef _PLATFORMDATA_H_

