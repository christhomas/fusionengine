#ifndef _FUSIONPLUGIN_H_
#define _FUSIONPLUGIN_H_

#if defined(_WIN32) && defined(_WINDLL)
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	
	#define FUSIONPLUGIN  extern "C" __declspec(dllexport)
	
	BOOL APIENTRY DllMain(HANDLE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved){return TRUE;}
#else
	#define FUSIONPLUGIN  extern "C"
#endif

#endif // #ifndef _FUSIONPLUGIN_H_