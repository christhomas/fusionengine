#ifndef _VIRTUALFS_H_
	#define _VIRTUALFS_H_

#include <FusionSubsystem.h>
#include <vector>
#include <IModuleDB.h>

#include <VirtualFS/FileInfo.h>
#include <VirtualFS/VFSFilter.h>
#include <VirtualFS/VFSPlugin.h>
#include <VirtualFS/VFSHandle.h>
#include <VirtualFS/VFSTransport.h>

/**	@ingroup	VFS_Group
 *	@brief		The core of the VirtualFS system
 */
class VirtualFS: public FusionSubsystem{
protected:
	/**	@var		IModuleDB *m_moduledb
	 *	@brief	A pointer to Fusion's Module Database
	 */
	IModuleDB	*m_moduledb;

	/**	@var		char *m_tempdir
	 *	@brief	The temporary directory that any temp files are written/read
	 */
	char *m_tempdir;

	/**	@var		std::vector<VFSTransport *>	m_transport
	 *	@brief	Array of Transport types
	 */
	std::vector<VFSTransport *> m_transport;

	/**	@var		std::vector<VFSPlugin *>	m_plugin
	 *	@brief	Array of File format Plugins
	 */
	std::vector<VFSPlugin *> m_plugin;

	/**	@var		std::vector<VFSFilter *>	m_filter
	 *	@brief	Array of Data filters
	 */
	std::vector<VFSFilter *> m_filter;

	virtual char *					FindExtension			(char *filename);
	virtual VFSTransport *	FindTransport			(char *filename);
	virtual VFSPlugin *			FindPlugin				(char *extension);
	virtual VFSFilter *			FindFilter				(char *type);

	virtual void						AddPlugin					(VFSPlugin::plugin_t				CreatePlugin);
	virtual void						AddFilter					(VFSFilter::filter_t				CreateFilter);
	virtual void						AddTransport			(VFSTransport::transport_t	CreateTransport);
public:
													VirtualFS					();
	virtual									~VirtualFS				();
	
	virtual bool						Initialise				(void);

	virtual bool						UnloadModules			(void);
	
	virtual void						SetTempDirectory	(char *directory=NULL);
	virtual char *					GetTempDirectory	(void);
	
	virtual void						LoadConfig				(char *configfile);
	virtual void						LoadPlugin				(VFSTransport::transport_t t, VFSPlugin::plugin_t p, VFSFilter::filter_t f);
	
	virtual VFSHandle *			Open							(char *filename, bool create=false);
	virtual VFSHandle *			Open							(char *filename, char *ext, bool create=false);
	virtual VFSHandle *			OpenLocation			(char *loc, bool create=false);
	
	virtual bool						Close							(VFSHandle *handle);
};

/* STATIC LINKING */
void	CreateVFS		(Fusion &f);
void	DestroyVFS	(void);

#endif // #ifndef _VIRTUALFS_H_

