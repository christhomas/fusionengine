#include <FusionPlugin.h>
#include <VFSPlugin_LWO.h>

Fusion		*fusion;
VFSPlugin	*plugin = NULL;

/**	Creates the LWO Plugin
 *	@ingroup	VFSPlugin_LWO_Group
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
	
	if(plugin == NULL) plugin = new VFSPlugin_LWO();
	
	return plugin;
}

/** Function to destroy a LWO Plugin
 *
 */
FUSIONPLUGIN void DestroyPlugin(void)
{
	delete plugin;
}
