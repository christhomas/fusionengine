#include <Fusion.h>

/**	Mesh Database Constructor 
 *
 *	Sets the name of the Fusion subsystem and it's module name
 */
MeshDB::MeshDB()
{
	object_name			= "MeshDB Version 1.0 <chris_thomas@hotmail.com>";
	object_filename	= "libOpenGL.dll";
}

/**	Mesh Database deconstructor
 *
 *	Operation:
 *		-#	Deletes all the mesh objects
 *		-#	Deletes all the overlay objects
 *		-#	Deletes all the entity objects
 *
 *	@todo	Need to remove creation/deletion of entity objects from the Mesh Database and into the Scenegraph
 */
MeshDB::~MeshDB()
{
	int a;
	
	for(a=0;a<m_mesh.size();a++){
		Mesh *m = m_mesh[a];
		delete m;
	}
	m_mesh.clear();

	for(a=0;a<m_overlay.size();a++){
		Overlay *o = m_overlay[a];
		delete o;
	}
	m_overlay.clear();

	//	Until I move the entity system completely into the scenegraph, clear up it's mess here
	for(a=0;a<m_entity.size();a++){
		Entity *e = m_entity[a];
		delete e;
	}

	m_entity.clear();
}

/**	Initialises the Mesh Database
 *
 *	@returns	true
 *	@todo	Look into deprecating this method in FusionSubsystem cause I Dont need it in most Fusionsubsystems
 */
bool MeshDB::Initialise(void)
{
	return true;
}

/**	Creates a mesh
 *
 *	@returns	An empty Mesh object
 */
Mesh * MeshDB::CreateMesh(void)
{
	Mesh *m = new Mesh();

	if(m != NULL) m_mesh.push_back(m);

	return m;
}

/**	Creates a Mesh and loads a mesh into it
 *
 *	@param	filename	The file containing the mesh to load
 *
 *	@returns	A Mesh object containing a mesh loaded from the file
 *
 *	Operation:
 *		-#	Ask the VFS to open the file
 *		-#	Read the file
 *		-#	Test whether reading the file was successful
 *		-#	Ask the VFS to close the file
 *		-#	delete the FileInfo structure
 *		-#	Return the mesh object
 *
 *	This method doesnt store the mesh object since the VFSPlugin that is reading the mesh file will inevitable call MeshDB::CreateMesh()
 *	which stores the mesh object there, so storing it here as well, would mean it's stored twice, which is a bad idea.
 */
Mesh * MeshDB::CreateMesh(char *filename)
{
	MeshFileInfo	*meshinfo		=	NULL;
	Mesh					*m					=	NULL;

	VFSHandle *handle = fusion->vfs->Open(filename);
		
	if(handle != NULL){
		meshinfo = reinterpret_cast<MeshFileInfo *>(handle->Read());
	
		if(meshinfo!=NULL) m = meshinfo->mesh;

		fusion->vfs->Close(handle);

		delete meshinfo;
	}

	return m;
}

/**	Creates an Overlay and assigns it a texture
 *
 *	@param	texture	The texture to assign to the overlay
 *
 *	@returns	An Overlay object
 *
 *	Operation:
 *		-#	Create the overlay object
 *		-#	call Overlay::SetTexture(ITexture *) to assign the texture
 *		-#	Store the overlay pointer
 */
Overlay * MeshDB::CreateOverlay(ITexture *texture)
{
	Overlay *o = new Overlay();

	if(texture != NULL) o->SetTexture(texture);

	m_overlay.push_back(o);

	return o;
}

/**	Creates an Entity and assigns it a Mesh object
 *
 *	@param	mesh	The mesh to assign to the entity
 *
 *	@returns	An Entity object
 */
Entity * MeshDB::CreateEntity(Mesh *mesh)
{
	Entity *e = new Entity(mesh);

	m_entity.push_back(e);

	return e;
}

/**	Deletes a mesh from the database
 *
 *	@param index	The index of the mesh to delete
 */
void MeshDB::DeleteMesh(int index)
{
	if(index < m_mesh.size()) delete m_mesh[index];
	m_mesh.erase(m_mesh.begin()+index);
}

/**	Deletes an Overlay from the database
 *
 *	@param index	The index of the mesh to delete
 */
void MeshDB::DeleteOverlay(int index)
{
	if(index < m_overlay.size()) delete m_overlay[index];
	m_overlay.erase(m_overlay.begin()+index);
}

//	There is no method to remove entities, since once they are created, 
//	they are managed by the scenegraph (perhaps I should move their creation there too.....fuck it....do it later....)

/**	Retrieve a pointer to a mesh object
 *
 *	@param index	The index of the mesh to retrieve
 *
 *	@returns	A Mesh object or NULL, if the object was not found
 */
Mesh * MeshDB::GetMeshPtr(int index)
{
	if(index < m_mesh.size()) return m_mesh[index];

	return NULL;
}

/**	Retrieve a pointer to an Overlay object
 *
 *	@param index	The index of the overlay to retrieve
 *
 *	@returns	An Overlay object or NULL, if the object was not found
 */
Overlay * MeshDB::GetOverlayPtr(int index)
{
	if(index < m_overlay.size()) return m_overlay[index];

	return NULL;
}

//	There is no method to obtain a entity ptr, ask the scenegraph
