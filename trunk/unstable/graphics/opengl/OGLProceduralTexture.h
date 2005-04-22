#ifndef _OGLPROCEDURALTEXTURE_H_
	#define _OGLPROCEDURALTEXTURE_H_

#include <OGLTexture.h>

/**	@ingroup	OGL_Texture_Group
 *	@brief		Derived ITexture class for Procedural/Function based textures
 */
class OGLProceduralTexture: public OGLTexture{
protected:
	/**	@var		textureproc_t	m_proc
	 *	@brief	The procedure that will generate the texture
	 */
	textureproc_t	m_proc;
public:
				OGLProceduralTexture	(int width, int height, int numcomp, textureproc_t proc);
	virtual		~OGLProceduralTexture	();
	virtual int		UpdateTexture			();
};

#endif // #ifndef _OGLPROCEDURALTEXTURE_H_
