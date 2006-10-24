#include <VirtualFS/VirtualFS.h>
#include <fusion.h>

/*	
		DOCUMENT PROPERLY YOU ASSHOLE

		FOR THE BENEFIT OF STATIC LINKING, 
		THESE FUNCTIONS ARE PROVIDED TO 
		ALLOW AN APPLICATION TO CREATE 
		THE OBJECT AND DESTROY IT
*/
void CreateVFS(Fusion &f)
{
	f.vfs = new VirtualFS();

	if(f.vfs->Initialise() == false)	DestroyVFS();
}

void DestroyVFS(void)
{
	delete fusion->vfs;

	fusion->vfs = NULL;
}