#ifndef _VIRTUALFS_H_
	#define _VIRTUALFS_H_

#include <FusionSubsystem.h>
#include <vector>
#include <IModuleDB.h>
#include <XMLConfig.h>

#include <vfs/FileInfo.h>
#include <vfs/VFSFilter.h>
#include <vfs/VFSPlugin.h>
#include <vfs/VFSHandle.h>
#include <vfs/VFSTransport.h>

class Fusion;
class IModuleDB;

/**	@ingroup	VFS_Group
 *	@brief		The core of the VirtualFS system
 */
class VirtualFS: public FusionSubsystem{
protected:
	/**	@var		IModuleDB *m_moduledb
	 *	@brief	A pointer to Fusion's Module Database
	 */
	IModuleDB	*m_moduledb;

	/**	@var	std::string *m_tempdir
	 *	@brief	The temporary directory that any temp files are written/read
	 */
	std::string m_tempdir;

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

	virtual std::string		FindExtension		(std::string filename);

	virtual void			AddPlugin			(VFSPlugin::plugin_t			CreatePlugin);
	virtual void			AddFilter			(VFSFilter::filter_t			CreateFilter);
	virtual void			AddTransport		(VFSTransport::transport_t	CreateTransport);
public:
							VirtualFS			();
	virtual					~VirtualFS			();
	
	virtual bool			Initialise			(void);

	virtual bool			UnloadModules		(void);
	
	virtual void			SetTempDirectory	(std::string directory=NULL);
	virtual std::string		GetTempDirectory	(void);
	
	virtual	void			LoadConfig			(void);
	virtual void			LoadConfig			(XMLConfig *xml);
	virtual void			LoadPlugin			(VFSTransport::transport_t t, VFSPlugin::plugin_t p, VFSFilter::filter_t f);
		
	virtual VFSHandle *		Open				(std::string filename, bool create=false);
	virtual VFSHandle *		Open				(std::string filename, std::string ext, bool create=false);
	virtual VFSHandle *		OpenLocation		(std::string loc, bool create=false);
	
	virtual VFSTransport *	FindTransport		(std::string filename);
	virtual VFSPlugin *		FindPlugin			(std::string extension);
	virtual VFSFilter *		FindFilter			(std::string type);	
		
	virtual bool			Close				(VFSHandle *handle);
};

/* STATIC LINKING */
void	CreateVFS	(Fusion &f);
void	DestroyVFS	(void);

#endif // #ifndef _VIRTUALFS_H_

