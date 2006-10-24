#include <Win32/Win32ModuleDB.h>
#include <Win32/Win32Module.h>
#include <cstdio>

/**	Win32 ModuleDB Constructor
 *
 *	Sets all internal data to zero/NULL
 */
Win32ModuleDB::Win32ModuleDB()
{
	m_nummodules			= 0;
	m_numsearchpaths	= 0;

	m_module					= NULL;
	m_searchpath			= NULL;
}

/**	Win32 ModuleDB Deconstructor
 *
 *	Operation:
 *		-#	Deletes all DLL Modules and unloads them
 *		-#	Deletes all search paths
 */
Win32ModuleDB::~Win32ModuleDB()
{
	int a;

	for(a=0;a<m_nummodules;a++)			delete m_module[a];
	for(a=0;a<m_numsearchpaths;a++) delete m_searchpath[a];

	delete[] m_module;
	delete[] m_searchpath;
}

/**	Retrieves a DLL Module
 *
 *	@param name	The filename of the DLL Module being searched for
 *	
 *	@returns	A IModule pointer or NULL is the module is not found
 */
IModule * Win32ModuleDB::GetModule(char *name)
{
	for(int a=0;a<m_nummodules;a++){
		if(strcmp(m_module[a]->GetFilename(), name) == 0) return m_module[a];
	}

	return NULL;
}

/**	Adds a searchpath to the ModuleDB
 *
 *	@param path	The path to add to the search list
 *
 *	Operation:
 *		-#	Allocate a new array of searchpath string pointers, increasing the size by one
 *		-#	Copy all the old searchpath pointers to the new array
 *		-#	Delete the old searchpath array
 *		-#	Reset the searchpath array pointer to the new array
 *		-#	Allocate a new searchpath string
 *		-#	Copy the new searchpath to the array
 *		-#	Increase the number of searchpaths by one
 */
void Win32ModuleDB::AddPath(char *path){
	char **temp = new char *[m_numsearchpaths+1];

	for(int a=0;a<m_numsearchpaths;a++)	temp[a] = m_searchpath[a];

	delete[] m_searchpath;

	m_searchpath = temp;

	m_searchpath[m_numsearchpaths] = new char[strlen(path)+1];
	strcpy(m_searchpath[m_numsearchpaths],path);

	m_numsearchpaths++;
}

/**	Adds a module to the database
 *
 *	@param module	An IModule	pointer
 *
 *	Operation:
 *		-#	Allocate a new array of module pointers, increasing the size by one
 *		-#	Copy all the old module pointers to the new array
 *		-#	Delete the old module array
 *		-#	Reset the module array pointer to the new array
 *		-#	Assign the extra pointer to the module paramter passed into the method
 *		-#	Increase the number of modules by one
 */
void Win32ModuleDB::AddModule(IModule *module)
{
	IModule **temp = new IModule *[m_nummodules+1];

	for(int a=0;a<m_nummodules;a++) temp[a] = m_module[a];

	delete[] m_module;

	m_module = temp;

	m_module[m_nummodules] = module;

	m_nummodules++;
}

/**	Loads a DLL Module
 *
 *	@param	name	The filename of the Module to load
 *
 *	@returns	A IModule pointer or NULL if not found
 *
 *	Operation:
 *		-#	Test whether the filename is valid
 *		-#	Attempt to retrieve a pointer to that module, test whether it's already loaded or not
 *		-#	If not loaded, loop through all the searchpaths, attempting to load the DLL Module at each path
 *		-#	Allocate a new Win32Module object
 *		-#	Create a absolute path to the DLL Module based on the current searchpath and the DLL Module filename
 *		-#	Attempt to load the DLL Module
 *		-#	If the attempt was successful, add the module to the Database and clear up, then return
 *		-#	clear up all temporary memory and try the next searchpath
 */
IModule * Win32ModuleDB::LoadModule(char *name){

	Win32Module *m = NULL;

	if(name!=NULL){
		m = (Win32Module *)GetModule(name);
			
		if(m==NULL){
			for(int a=0;a<m_numsearchpaths;a++){
				char *path = m_searchpath[a];

				m = new Win32Module(name,path,NULL);

				char *filename	= new char[strlen(path) + strlen("/") + strlen(name) + 1];

				sprintf(filename,"%s/%s",path,name);

				if(m->Load(filename) != NULL){
					AddModule(m);
					
					delete[] filename;
					return m;
				}

				delete[] filename;
				delete m;
			}
		}else{
			return m;
		}
	}
	
	return NULL;
}

/**	Unloads a DLL module
 *
 *	@param name	The filename of the DLL Module to unload
 *
 *	@returns	Boolean true or false, depending on whether the DLL Module successfully unloaded or not
 *
 *	Operation:
 *		-#	Loop through the loaded modules, testing the filename against the one being searched for
 *		-#	If match is found, attempt to remove that module from the database
 *		-#	Create a new temporary module array, with one less element
 *		-#	Loop through all the loaded DLL modules, if the module being copied is the one being deleted, skip it
 *		-#	Copy each module pointer for all those that are not being removed
 *		-#	Delete the module being unloaded
 *		-#	Delete the module array
 *		-#	Reassign the module array pointer to the temporary array
 *		-#	Reduce the number of modules in the database
 */
bool Win32ModuleDB::UnloadModule(char *name)
{
	int a,b,c;

	for(a=0;a<m_nummodules;a++){
		if(strcmp(name,m_module[a]->GetFilename()) == 0){
			IModule **temp = new IModule *[m_nummodules-1];

			for(b=0,c=0;b<m_nummodules;b++){
				if(b == a) continue;

				temp[c++] = m_module[b];
			}

			delete m_module[a];

			delete[] m_module;

			m_module = temp;

			m_nummodules--;

			return true;
		}
	}

	return false;
}

/**	Unloads all the DLL modules from the database
 *	
 *	Operation:
 *		-#	Constantly requests to unload the first DLL module in the array
 *		-#	When the array is empty, then all the DLL Modules are released
 */
void Win32ModuleDB::UnloadAll(void)
{
	while(m_nummodules > 0){
		UnloadModule(m_module[0]->GetFilename());
	}
}

/**	Retrieve a function pointer from a DLL Module 
 *
 *	@param	name	The filename of the DLL Module
 *	@param	func	The name of the function requested
 *
 *	@returns	A Function pointer to the correct DLL function or NULL if not found
 *
 *	Operation:
 *		-#	Attempt to load the DLL Module
 *		-#	If successful, Attempt to retrieve a function pointer from that module
 */
void * Win32ModuleDB::GetFunction(char *name,char *func){
	Win32Module *m;
	
	if((m = (Win32Module *)LoadModule(name)) == NULL)	return NULL;

	return (void *)GetProcAddress((HINSTANCE)m->GetHandle(),TEXT(func));
}
