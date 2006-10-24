#ifndef _OGLGRAPHICS_H_
	#define _OGLGRAPHICS_H_

#include <OpenGL12.h>
#include <graphics/IGraphics.h>
#include <maths/Quaternion.h>
#include <DriverData.h>

class Maths::Vector;

#include <fstream>

/** @ingroup	OGL_Graphics_Group
 *	@brief		Derived IGraphics class for the OpenGL Platform
 */
class OGLGraphics: public IGraphics{
protected:
	/**	@var		DriverData m_driverdata
	 *	@brief	DriverData interface to retrieve information about the OpenGL driver
	 */
	DriverData	m_driverdata;

	/**	@var		Quaternion m_quat
	 *	@brief	Quaternion object to help with Euler based rotations
	 */
	Maths::Quaternion	m_quat;

	/**	@var		int m_polygonmode
	 *	@brief	The mode to render polygons in, solid triangles, or wireframe
	 */
	int m_polygonmode;
	
public:
							OGLGraphics			();
	virtual					~OGLGraphics		();

	virtual bool			Initialise			(void);
	virtual bool			SetupGraphics		(void);

	virtual void			SetClearColour		(float r, float g, float b);
	virtual void			ClearScreen			(void);

	virtual void			ResizeWindowDisplay	(int x, int y);
	virtual void			Update				(void);

	virtual void			GetScreenResolution	(int &x, int &y, int &bpp);
	virtual int				GetScreenWidth		(void);
	virtual int				GetScreenHeight		(void);
	virtual int				GetScreenDepth		(void);

	virtual bool			Enable				(int flag,int options=0);
	virtual bool			Disable				(int flag,int options=0);

	virtual int				GetRenderMode		(void);

	virtual void *			GetExtension		(std::string extension) = 0;

	//=======================
	//	Lighting Methods
	//=======================
	virtual void			LightPosition		(unsigned int light, Maths::Vector &position);
	virtual void			LightDiffuse		(unsigned int light, Colour4f *diffuse);
	virtual void			LightAmbient		(unsigned int light, Colour4f *ambient);

	//=======================================
	//	Rendering Methods
	//=======================================
	virtual IVertexBuffer *	CreateVertexBuffer	(int mode);
	virtual void			SetColour			(float *rgba);
	virtual void			SetColour			(float r, float g, float b, float a);
	virtual void			LoadIdentity		(void);
	virtual void			Translate			(Maths::Vector &position);
	virtual void			Translate			(float x, float y, float z);
	virtual void			Scale				(Maths::Vector &scale);
	virtual void			Scale				(float x, float y, float z);
	virtual void			Rotate				(float *rotate);
	virtual void			Rotate				(float angle, float x, float y, float z);
	virtual void			Rotate				(float r, float p, float y);
	virtual void			MultiplyMatrix		(float *matrix);

	//==============================
	//	Texture Management
	//==============================
	virtual ITexture *		CreateTexture		(int x, int y);
	virtual ITexture *		CreateTexture		(std::string image);
	virtual ITexture *		CreateTexture		(int x, int y, int numcomp, ITexture::textureproc_t proc);
	virtual void			DeleteTexture		(ITexture *texture);
	virtual void			DeleteTexture		(unsigned int index);
	virtual ITexture *		GetTexture			(unsigned int index);
	virtual void			ReloadTextures		(void);
};

#endif // #ifndef _OGLGRAPHICS_H_
