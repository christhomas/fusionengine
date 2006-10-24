#include <Fusion.h>
#include <FusionUtil.h>
#include <virtualfs/VFSHandle_ZIP.h>

/**********************************
*	Fusion Utility functions
**********************************/
void UnzipFusion(char *arc)
{
	fusion->InitSystem(Fusion::VFS,CreateVFS);
	fusion->vfs->LoadPlugin(CreateZIP,NULL,NULL);

	VFSHandle *h = fusion->vfs->OpenLocation(arc);
	h->Copyfile("*.*", "file://system");
	fusion->vfs->Close(h);
}

void CleanupFusion(void)
{
	fusion->UnloadModules();
	//fusion->vfs->UnloadModules();
	VFSHandle *h = fusion->vfs->OpenLocation("file://");
	h->DeleteDir("system",true);

	DestroyVFS();
}
