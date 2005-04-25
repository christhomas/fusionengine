#include <FusionPlugin.h>
#include <VFSPlugin_Q3BSP.h>

Fusion		*fusion;
VFSPlugin	*plugin = NULL;
std::string plugin_str = "VFSPlugin_Q3BSP: ";

/**	Creates the Quake3 Plugin
 *	@ingroup	VFSPlugin_Q3BSP_Group
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

	VFSPlugin *p = NULL;

	if(plugin == NULL){
		if(fusion->vfs->FindPlugin("jpg") != NULL && fusion->vfs->FindPlugin("tga") != NULL){
			p = new VFSPlugin_Q3BSP();
		}else{
			//	FIXME:	Perhaps here I should attempt to load the plugins 
			//			required as opposed to dropping out with an error?
			fusion->errlog << plugin_str << "Sorry I cannot load" << std::endl;
			fusion->errlog << plugin_str << "I depend on VFSPlugin_JPEG and VFSPlugin_TGA to work correctly" << std::endl;
			fusion->errlog << plugin_str << "and I can't work without them" << std::endl;
		}
	}

	return plugin;
}

/** Function to destroy a Quake3 Plugin
 *
 */
FUSIONPLUGIN void DestroyPlugin(void)
{
	delete plugin;
}