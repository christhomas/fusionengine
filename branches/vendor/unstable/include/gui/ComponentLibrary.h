#ifndef _COMPONENTLIBRARY_H_
	#define _COMPONENTLIBRARY_H_

#include <vector>
#include <IModuleDB.h>
#include <gui/GUIComponents.h>

class ComponentLibrary{
private:
	IModuleDB	*m_moduledb;
	LIBRARY		library;

public:
	inline ComponentLibrary(){};
	inline ~ComponentLibrary()
	{
		for(LIBRARY::iterator l=library.begin();l!=library.end();l++){
			delete *l;
		}

		library.clear();
	}

	typedef void (*UPDATELIBRARY)(LIBRARY *l);

	inline void Initialise(IModuleDB *moduledb){
		m_moduledb = moduledb;
	}

	inline void AddLibrary(char *filename){
		UPDATELIBRARY update = (UPDATELIBRARY)m_moduledb->GetFunction(filename,(char *)"update");
		AddLibrary(update);
	}

	inline void AddLibrary(UPDATELIBRARY update){
		if(update != NULL) update(&library);
	}

	inline IWindowComponent * CreateComponent(unsigned int c_id){
		IWindowComponent *g = NULL;
		if(c_id < library.size())	g = library[c_id]->m_init();

		return g;
	}

	inline void DestroyComponent(IWindowComponent *gc){
		unsigned int c_id = gc->m_component_type;
		if(c_id < library.size())	gc = library[c_id]->m_destroy(gc);
	}

	inline int NumberRegisteredComponents(void){
		return (int)library.size();
	}
};

#endif // #ifndef _COMPONENTLIBRARY_H_

