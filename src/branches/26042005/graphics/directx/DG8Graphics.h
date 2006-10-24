#ifndef _DG8GRAPHICS_H_
	#define _DG8GRAPHICS_H_

#include <Graphics/IGraphics.h>
#include <Fusion.h>

#include <d3d8.h>
#include <d3dx8.h>
#include <Win32/Directx/DG8DynamicVB.h>
#include <Win32/Directx/DG8StaticVB.h>

/** @ingroup	DG8_Graphics_Group
 *	@brief		Derived IGraphics class for the DirectGraphics8 Platform
 */
class DG8Graphics: public IGraphics{
protected:
	/**	@var		Colour3b m_clear
	 *	@brief	The background clear colour
	 */
	Colour3b m_clear;

	/**	@var		Win32PlatformData *m_platform
	 *	@brief	A Pointer to Fusions platform information
	 */
	Win32PlatformData *m_platform;

	/**	@var		Quaternion m_quat
	 *	@brief	A quaternion object to help with rotations
	 */
	Maths::Quaternion m_quat;

	/**	@var		IDirect3D8 *m_Direct3D
	 *	@brief	The Main Direct3D object
	 */
	IDirect3D8 *m_Direct3D;

	/**	@var		IDirect3DDevice8 *m_RenderDevice
	 *	@brief	The Direct3D rendering device
	 */
	IDirect3DDevice8 *m_RenderDevice;

	/**	@var		D3DXMATRIX m_tempmatrix
	 *	@brief	A scratchboard matrix to upload to dg8
	 */
	D3DXMATRIX m_tempmatrix;

	/**	@var		D3DDISPLAYMODE m_DisplayMode
	 *	@brief	The display mode for the device
	 */
	D3DDISPLAYMODE m_DisplayMode;

	/**	@var		D3DLIGHT8 lightdata[8]
	 *	@brief	A light data set for each light available (limited to 8 for now)
	 */
	D3DLIGHT8 lightdata[8];

	friend class DG8DynamicVB;
	friend class DG8StaticVB;

public:
										DG8Graphics					();
	virtual						~DG8Graphics				();

	virtual bool			Initialise					(void);												//	Initialise the graphics system
	virtual bool			SetupGraphics				(void);												//	Sets up all that d3d needs to function

	virtual void			SetClearColour			(float r, float g, float b);	//	Sets the default background colour of the window
	virtual void			ClearScreen					(void);												//	Clears the window with the background colour

	virtual void			ResizeWindowDisplay	(int x, int y);								//	Allows the window to resize it's display
	virtual void			Update							(void);												//	Updates the graphics system

	virtual bool			Enable							(int flag,int options=0);
	virtual bool			Disable							(int flag,int options=0);

	virtual int				GetRenderMode				(void);

	//=======================================
	//	Set/Retrieve Video mode information
	//=======================================
	virtual bool			SetMode							(int x, int y, bool f);										//	Set a Display mode
	virtual bool			RestoreMode					(void);																		//	Restore the default display mode
	virtual void			GetScreenResolution	(int &x, int &y, int &bpp);								//	Retrieves information about the video resolution
	virtual int				GetScreenWidth			(void);																		//	Retrieves the width of the window
	virtual int				GetScreenHeight			(void);																		//	Retrieves the height of the window
	virtual int				GetScreenDepth			(void);																		//	Retrieves the depth of the window (did I need to tell you?)

	//=======================
	//	Lighting Methods
	//=======================
	virtual void			LightPosition		(unsigned int light, Maths::Vector &position);	//	Enable a light, set it's position
	virtual void			LightDiffuse		(unsigned int light, Colour4f *diffuse);				//	Set Diffuse component
	virtual void			LightAmbient		(unsigned int light, Colour4f *ambient);				//	Set Ambient component

	//=======================================
	//	Rendering Methods
	//=======================================
	virtual	IVertexBuffer *	CreateVertexBuffer	(int mode);																//	Creates a vertex buffer object
	virtual	void						SetTexture					(ITexture *texobj);												//	Sets the texture
	virtual	void						SetColour						(float *rgba);														//	Sets the colour
	virtual	void						SetColour						(float r, float g, float b, float a);			//	Sets the colour from components
	virtual	void						LoadIdentity				(void);																		//	Loads an identity matrix
	virtual void						Translate						(Maths::Vector &position);								//	Translates the current rendering position
	virtual	void						Translate						(float x, float y, float z);							//	Translates the current rendering position
	virtual void						Scale								(Maths::Vector &scale);										//	Scales the current rendering matrix
	virtual	void						Scale								(float x, float y, float z);							//	Scales the current rendering matrix
	virtual void						Rotate							(float *rotate);													//	Rotates the current rendering matrix (axis/angle, float array)
	virtual void						Rotate							(float angle, float x, float y, float z);	//	Rotates the current rendering matrix (axis/angle)
	virtual void						Rotate							(float r, float p, float y);							//	Rotates the current rendering matrix (euler)
	virtual void						MultiplyMatrix			(float *matrix);													//	Multiplies a matrix with the current selected matrix

	//==============================
	//	Texture Management
	//==============================
	virtual ITexture *		CreateTexture		(int x, int y);																//	Creates a pixelbuffer texture
	virtual ITexture *		CreateTexture		(char *image);																//	Creates an image texture
	virtual ITexture *		CreateTexture		(int x, int y, ITexture::textureproc_t proc);	//	Creates a procedural texture
	virtual void					DeleteTexture		(ITexture *texture);													//	Deletes a texture
	virtual void					DeleteTexture		(unsigned int index);													//	Deletes a texture
	virtual ITexture *		GetTexture			(unsigned int index);													//	Retrieves a texture object ptr
	virtual void					ReloadTextures		(void);																			//	Calls reload on all textures (refresh contents)
};

#endif // #ifndef _DG8GRAPHICS_H_