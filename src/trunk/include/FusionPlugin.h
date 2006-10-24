#ifndef _FUSIONPLUGIN_H_
#define _FUSIONPLUGIN_H_

#if defined(_WIN32)
	#define WIN32_LEAN_AND_MEAN
	//	Solves the winsock2 problem
	#include <PlatformData.h>
	
	#define FUSIONPLUGIN  extern "C" __declspec(dllexport)
	
	BOOL APIENTRY DllMain(HANDLE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved){return TRUE;}
#else
	#define FUSIONPLUGIN  extern "C"
#endif

#endif // #ifndef _FUSIONPLUGIN_H_