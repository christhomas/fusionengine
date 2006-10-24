#ifndef _OGLIMAGETEXTURE_H_
	#define _OGLIMAGETEXTURE_H_

#include <OGLTexture.h>

class ImageFileInfo;

/**	@ingroup	OGL_Texture_Group
 *	@brief		Derived ITexture class for Image based textures
 */
class OGLImageTexture: public OGLTexture{
protected:
	/**	@var		ImageFileInfo *m_fileinfo
	 *	@brief	A FileInfo structure to contain details about the image loaded from the source
	 */
	ImageFileInfo	*m_fileinfo;
public:
					OGLImageTexture		(std::string filename);
	virtual			~OGLImageTexture	();
	virtual int		UpdateTexture		();
};

#endif // #ifndef _OGLIMAGETEXTURE_H_
