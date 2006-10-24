#include <FusionPlugin.h>
#include <VFSHandle_ZIP.h>
#include <vfs/VFSTransport.h>

Fusion			*fusion;
VFSTransport	*transport = NULL;

/**	Function to create a ZIP transport from a shared library
 *
 *	This function is needed to create a transport object from this plugin
 *	it has the added functionality to block more than 1 being created, since
 *	it would be pointless and problematic.
 *
 */
FUSIONPLUGIN VFSTransport * CreateTransport(Fusion &f)
{
	fusion = &f;
	
	if(transport == NULL) transport = CreateTransportZIP(fusion);
	
	return transport;
}

/** Function to destroy a ZIP transport previously created
 *
 */
FUSIONPLUGIN void DestroyTransport(void)
{
	delete transport;
}