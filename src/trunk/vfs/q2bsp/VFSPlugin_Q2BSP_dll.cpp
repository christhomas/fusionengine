#include <FusionPlugin.h>
#include <VFSPlugin_Q2BSP.h>
#include <VFSPlugin_Q2WAL.h>

Fusion		*fusion;
VFSPlugin	*plugin_bsp = NULL;
VFSPlugin	*plugin_wal = NULL;
std::string plugin_str = "VFSPlugin_Q2BSP: ";

/**	Creates the Quake2 Plugin
 *	@ingroup	VFSPlugin_Q2BSP_Group
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
	VFSPlugin *p = NULL;
	fusion = f;
	
	if(fusion->vfs->FindPlugin("pcx") != NULL){
		if(plugin_bsp == NULL){
			p = plugin_bsp = new VFSPlugin_Q2BSP();
		}else if(plugin_wal == NULL){
			p = plugin_wal = new VFSPlugin_Q2WAL();
		}
	}else{
		//	FIXME:	Perhaps here I should attempt to load the plugins 
		//			required as opposed to dropping out with an error?
		fusion->errlog << plugin_str << "Sorry I cannot load" << std::endl;
		fusion->errlog << plugin_str << "I depend on VFSPlugin_PCX to work correctly" << std::endl;
		fusion->errlog << plugin_str << "and I can't work without them" << std::endl;	
	}
	
	return p;
}

/** Function to destroy a 3DS Plugin
 *
 */
FUSIONPLUGIN void DestroyPlugin(void)
{
	delete plugin_bsp;
	delete plugin_wal;
}
