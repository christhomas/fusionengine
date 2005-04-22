#ifndef _MESHDB_H_
	#define _MESHDB_H_

#include <graphics/IGraphics.h>
#include <mesh/Mesh.h>
#include <mesh/Overlay.h>
#include <mesh/Entity.h>
#include <FusionSubsystem.h>

/**	@ingroup	Mesh_Graphics_Group
 *	@brief		The base class for the Mesh Database
 */
class MeshDB: public FusionSubsystem{
protected:
	/**	@var		std::vector<Mesh *> m_mesh
	 *	@brief	An array of Mesh objects
	 */
	std::vector<Mesh *>	m_mesh;

	/**	@var		std::vector<Overlay *> m_overlay
	 *	@brief	An array of Overlay objects
	 */
	std::vector<Overlay *>	m_overlay;

	/**	@var		std::vector<Overlay *> m_entity
	 *	@brief	An array of Entity objects
	 */
	std::vector<Entity *>	m_entity;

public:
					MeshDB		();
	virtual			~MeshDB		();
	virtual	bool		Initialise		(void);

	//==========================================
	//	Methods to create supported mesh objects
	//==========================================
	virtual Mesh *		CreateMesh	(void);
	virtual Mesh *		CreateMesh	(char *filename);
	virtual Overlay *	CreateOverlay	(ITexture *texture=NULL);
	virtual Entity *		CreateEntity	(Mesh *mesh);

	//==========================================
	//	Methods to Delete supported mesh objects
	//==========================================
	virtual	void		DeleteMesh	(unsigned int id);
	virtual	void		DeleteOverlay	(unsigned int id);

	//====================================================
	//	Methods to Obtain ptrs to supported mesh objects
	//====================================================
	virtual	Mesh *	GetMeshPtr	(unsigned int id);
	virtual	Overlay *	GetOverlayPtr	(unsigned int id);
};

#endif // #ifndef _MESHDB_H_

