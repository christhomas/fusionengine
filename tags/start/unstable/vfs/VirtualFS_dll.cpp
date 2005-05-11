#include <fusion.h>

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	
	#define LIBVFS_API  extern "C" __declspec(dllexport)
	
	BOOL APIENTRY DllMain(HANDLE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved){return TRUE;}
#else
	#define LIBVFS_API  extern "C"
#endif

/**	Method to create the VirtualFS object
 *
 *	This function is required for the application to create a VFS object from a library
 */
LIBVFS_API void GetInstance(Fusion &f)
{
	if(f.vfs == NULL){
		CreateVFS(f);

		if(f.vfs->Initialise() == false) DestroyVFS();
	}
}

LIBVFS_API void DestroyInstance(void)
{
	DestroyVFS();
}