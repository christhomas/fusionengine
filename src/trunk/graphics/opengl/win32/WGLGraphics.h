#ifndef _WGLGRAPHICS_H_
	#define _WGLGRAPHICS_H_

#include <OGLGraphics.h>
#include <Fusion.h>

/** @ingroup	OGL_Graphics_Group
 *	@brief	Derived OGLGraphics class for the Win32 Platform
 */
class WGLGraphics: public OGLGraphics{
public:
				WGLGraphics			();
	virtual		~WGLGraphics		();
	
	virtual bool	SetMode			(int w, int h, bool f);
	virtual bool	RestoreMode		(void);
	virtual void *	GetExtension	(std::string extension);
};

#endif // #ifndef _WGLGRAPHICS_H_
