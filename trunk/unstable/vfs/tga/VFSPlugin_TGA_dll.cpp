#include <FusionPlugin.h>
#include <VFSPlugin_TGA.h>

Fusion		*fusion;
VFSPlugin	*plugin = NULL;

/**	Creates the TGA Plugin
 *	@ingroup	VFSPlugin_TGA_Group
 *
 *	@param	f	The Fusion object, if this plugin needs to call fusion for something
 *
 *	@returns	A VFSPlugin object for this plugin type
 *
 *	The plugin function is simple, each time called
 *	create the appropiate plugin, increment the counter
 *	so next time it's called, it'll create the NEXT plugin
 *	then when you have no more plugins to create, return NULL
 */
FUSIONPLUGIN VFSPlugin * CreatePlugin(Fusion *f)
{
	fusion = f;
	
	if(plugin == NULL) plugin = new VFSPlugin_TGA();
	
	return plugin;
}

/** Function to destroy a TGA Plugin
 *
 */
FUSIONPLUGIN void DestroyPlugin(void)
{
	delete plugin;
}
