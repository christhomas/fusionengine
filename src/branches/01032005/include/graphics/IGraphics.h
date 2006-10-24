#ifndef _IGRAPHICS_H_
	#define _IGRAPHICS_H_

#include <FusionSubsystem.h>
#include <mesh/Colour.h>
#include <maths/Vector.h>

#include <graphics/IWindowDevice.h>
#include <graphics/ITexture.h>
#include <graphics/IVertexBuffer.h>

#include <vector>

/**	@ingroup	Graphics_Group
 *	@brief		The base class for all Graphics objects
 */
class IGraphics: public FusionSubsystem{
protected:
	/**	@var		bool m_active
	 *	@brief	Whether the graphics object is active or not
	 *
	 *	This affects whether the graphics object will render anything, when you
	 *	deselect the Fusion application, you may want the graphics to stop
	 *	sucking all the cpu time rendering an application which is not the
	 *	focus of the user at this time, like a game, minimised.
	 */
	bool m_active;

	/**	@var		bool m_fullscreen
	 *	@brief	Whether the application is fullscreen or not
	 */
	bool m_fullscreen;

	/**	@var		int m_width
	 *	@brief	The width of the graphics viewport in pixels
	 */
	int m_width;

	/**	@var		int m_height
	 *	@brief	The height of the graphics viewport in pixels
	 */
	int m_height;

	/**	@var		int m_bpp
	 *	@brief	The bit depth of the graphics viewport
	 *
	 *	This is really not needed, since Fusion REQUIRES 32bit to work and wont sacrifice quality 
	 *	for inferior hardware, if this is a problem the solution is simple.  Dont use fusion, or upgrade the hardware
	 */
	int m_bpp;
		
	/**	@var		bool Enable3DMode
	 *	@brief	Whether the system is rendering to a 3d system or 2d system
	 *
	 *	This is helpful when you want to render a 3d world, but then render 2d overlays
	 *	containing information about the player, say their health, weapon status, etc
	 *	Simply rendering your 3d world, then flipping to 2d, rendering all your overlays
	 *	then flipping back and looping around and around in this manner will ensure Fusion 
	 *	can render 3d AND 2d information without too many problems
	 */
	bool	Enable3DMode;

public:

	/**	@enum Capabilities
	 *
	 *	Enumerated set of graphics capabilities
	 */
	enum Capabilities{
		LIGHTING	=	2,		/**<	Enable/Disable Lighting							*/
		LIGHT			=	4,		/**<	Enable/Disable a specific light			*/
		
		MODE2D		=	8,		/**<	Enable 2D Rendering mode						*/
		MODE3D		=	16,		/**<	Enable 3D Rendering mode						*/
		
		CULLFACE	=	32,		/**<	Enable/Disable Face culling					*/
		FRONTFACE	=	64,		/**<	Frontface polygon culling						*/
		BACKFACE	=	128,	/**<	Backface polygon culling						*/

		WIREFRAME	=	256,	/**<	Wireframe rendering mode						*/	
		POLYGON		=	512,	/**<	Polygon rendering mode (triangles)	*/	
		POINTS		=	1024	/**<	Point rendering mode								*/
	};

	/**	@var		IWindowDevice *Window
	 *	@brief	The window device which is the destination for rendering
	 */
	IWindowDevice *Window;

	/**	IGraphics Constructor */
	IGraphics(){}
	
	/**	IGraphics Deconstructor */
	virtual ~IGraphics(){}

	/**	Initialise the graphics subsystem 
	 *
	 *	@returns boolean true or false, depending on whether Initialisation was successful
	 */
	virtual	bool Initialise(void) = 0;

	/**	Set the details of the graphics system 
	 *
	 *	@returns boolean true or false, depending on whether the graphics system setup ok
	 */
	virtual bool SetupGraphics(void) = 0;

	/**	Set the clear colour to be applied each new frame 
	 *
	 *	@param r	The Red component
	 *	@param g	The green component
	 *	@param b	The blue component
	 */
	virtual void SetClearColour(float r, float g, float b) = 0;

	/** Clears the screen */
	virtual void ClearScreen(void) = 0;
	
	/** Resizes the display viewport 
	 *
	 *	@param x	The width to resize to
	 *	@param y	The height to resize to
	 */
	virtual void ResizeWindowDisplay(int x, int y) = 0;
	
	/** Updates the graphics subsystem */
	virtual void Update(void) = 0;

	/** @var		DestroyEvent(void)
	 *	@brief	Function pointer Callback for destroying the Window Device 
	 *
	 *	This allows the Application to decide what happens when the Window device is destroyed
	 */
	void (*DestroyEvent)(void);
	
	/** @var		ActivateEvent(bool activate)
	 *	@brief	Function pointer Callback for [De]Activating the Window Device 
	 *
	 *	@param	activate	Whether the window is being activated or deactivated
	 *
	 *	This allows the application to decide what happens when the window device is [De]Activated
	 */
	void (*ActivateEvent)(bool activate);

	/**	Enables graphics capabilities
	 *
	 *	@param flag			The capability to enable
	 *	@param options	Any options the capability needs to enable with (light number for example)
	 */
	virtual bool Enable(int flag,int options=0) = 0;

	/**	Disables graphics capabilities
	 *
	 *	@param flag			The capability to disable	
	 *	@param options	Any options the capability needs to disable with (light number for example)
	 */
	virtual bool Disable(int flag,int options=0) = 0;

	/**	Retrieves the mode to render polygons in
	 *
	 *	@returns	The mode to render in
	 */
	virtual int GetRenderMode(void) = 0;
	
	//=======================================
	//	Set/Retrieve Video mode information
	//=======================================

	/** Sets a graphics mode 
	 *
	 *	@param	x	The width of the requested mode
	 *	@param	y	The height of the requested mode
	 *	@param	f	Whether the mode is fullscreen or not
	 *
	 *	@returns boolean true or false, depening on whether the mode was successfully changed
	 */
	virtual bool SetMode(int x, int y, bool f) = 0;

	/** Restores the original graphics mode 
	 *
	 *	@returns boolean true or false, depending on whether restoring the video mode was successful
	 */
	virtual bool RestoreMode(void) = 0;

	/** Retrieves the graphics modem 
	 *
	 *	@param x			The variable to store the width
	 *	@param y			The variable to store the height
	 *	@param bpp		The variable to store the depth
	 */
	virtual	void GetScreenResolution(int &x, int &y, int &bpp) = 0;

	/** Retrieves the width of the viewport 
	 *
	 *	@returns	The width of the viewport
	 */
	virtual	int GetScreenWidth(void) = 0;

	/** Retrieves the height of the viewport 
	 *
	 *	@returns	The height of the viewport
	 */
	virtual	int GetScreenHeight(void) = 0;

	/** Retrieves the bit depth of the viewport 
	 *
	 *	@returns	The depth of the viewport
	 */
	virtual	int GetScreenDepth(void) = 0;

	//=======================================
	//	Lighting Methods
	//=======================================
	
	/** Sets the position of a specific light
	 *
	 *	@param	light			The ID of the light to enable
	 *	@param	position	A Vector3d containing the position of the light
	 */
	virtual	void LightPosition(unsigned int light, Maths::Vector &position) = 0;

	/** Gives a light a Diffuse value 
	 *
	 *	@param	light		The ID of the light
	 *	@param	diffuse	A Colour4f object containing the colour to use
	 */
	virtual void LightDiffuse(unsigned int light, Colour4f *diffuse) = 0;

	/** Gives a light an Ambient value 
	 *
	 *	@param	light		The ID of the light
	 *	@param	ambient	A Colour4f object containing the colour to use
	 */
	virtual void LightAmbient(unsigned int light, Colour4f *ambient) = 0;

	//=======================================
	//	Rendering Methods
	//=======================================
	
	/** Creates a IVertexPool object 
	 *
	 *	@param	mode	The type of VertexBuffer to create
	 *	
	 *	@returns	A IVertexBuffer object or NULL if failed to create
	 */
	virtual IVertexBuffer * CreateVertexBuffer(int mode) = 0;
	
	/** Sets the colour to render geometry in 
	 *
	 *	@param	rgba	The rgba colour to set
	 */
	virtual	void SetColour(float *rgba) = 0;

	/** Sets the colour to render geometry in 
	 *
	 *	@param	r	The red component
	 *	@param	g	The green component
	 *	@param	b	The blue component
	 *	@param	a	The alpha component
	 */
	virtual	void SetColour(float r, float g, float b, float a) = 0;

	/** Loads an identity matrix onto the matrix stack */
	virtual	void LoadIdentity(void) = 0;
	
	/** Translates the current rendering position 
	 *
	 *	@param	position A Vector3d object containing the position to render at
	 */
	virtual void Translate(Maths::Vector &position) = 0;

	/** Translates the current rendering position 
	 *
	 *	@param x	The X value to render at
	 *	@param y	The Y value to render at
	 *	@param z	The Z value to render at
	 */
	virtual	void Translate(float x, float y, float z) = 0;
	
	/** Scales the geometry to be rendered 
	 *
	 *	@param	scale A Vector3d object containing the scale to render at
	 */
	virtual void Scale(Maths::Vector &scale) = 0;

	/** Scales the geometry to be rendered 
	 *
	 *	@param	x	The X scalar
	 *	@param	y	The Y scalar
	 *	@param	z	The Z scalar
	 */
	virtual	void Scale(float x, float y, float z) = 0;

	/** Rotates the current rendering position 
	 *
	 *	@param	rotate	The angle to render to
	 *
	 *	WARNING: The array of values must contain four values
	 *
	 *	value 1:	The axis to rotate around
	 *	value 2:	The x angle
	 *	value 3:	The y angle
	 *	value 4:	The z angle
	 */
	virtual void Rotate(float *rotate) = 0;

	/** Rotates the current rendering position 
	 *
	 *	@param	angle	The angle by which to rotate
	 *	@param	x			The x value (1 or 0)
	 *	@param	y			The y value (1 or 0)
	 *	@param	z			The z value (1 or 0)
	 *
	 *	This method specifies a type of Angle/Axis rotation, 
	 *	you specify an angle to rotate, then give an axis 
	 *	to rotate around, therefore x/y/z must be 1 or 0.
	 *	Any other value will result in error
	 */
	virtual void Rotate(float angle, float x, float y, float z) = 0;

	/** Rotates the current rendering position 
	 *
	 *	@param	r	The Roll component
	 *	@param	p	The pitch component
	 *	@param	y	The Yaw component
	 */
	virtual void Rotate(float r, float p, float y) = 0;

	/** Multiplies a Matrix with the matrix on the top of the matrix stack
	 *
	 *	@param matrix	A 16 element array of floating point values, which is used as a matrix
	 */
	virtual void MultiplyMatrix(float *matrix) = 0;

	//==============================
	//	Texture Management
	//==============================
	/**	@var		std::vector<ITexture *> Textures
	 *	@brief	An array of ITexture objects
	 */
	std::vector<ITexture *>	Textures;

	/** Creates a Fusion Pixelbuffer texture object
	 *
	 *	@param	x	The width of the texture
	 *	@param	y	The height of the texture
	 *
	 *	@returns An ITexture object or NULL if failed to create
	 */
	virtual ITexture * CreateTexture(int x, int y) = 0;

	/** Creates a Fusion Image texture object
	 *
	 *	@param	image	The filename of the image to load into the texture
	 *
	 *	@returns	An ITexture object or NULL if failed to create
	 */
	virtual ITexture * CreateTexture(std::string image) = 0;

	/** Creates a Fusion Procedural texture object 
	 *
	 *	@param	x				The width of the texture
	 *	@param	y				The height of the texture
	 *	@param	numcomp	The number of components in each pixel (3 = RGB, 4 = RGBA)
	 *	@param	proc		The procedure to create the texture
	 */
	virtual ITexture *CreateTexture(int x, int y, int numcomp, ITexture::textureproc_t proc) = 0;

	/** Deletes a Fusion texture object 
	 *
	 *	@param	texture An ITexture pointer to the texture to delete
	 */
	virtual void DeleteTexture(ITexture *texture) = 0;

	/** Deletes a Fusion texture object 
	 *
	 *	@param	index An index to the texture to delete
	 */
	virtual void DeleteTexture(unsigned int index) = 0;

	/** Retrieves a pointer to a ITexture object 
	 *
	 *	@param	index An index to the texture to retrieve
	 *
	 *	@returns	An ITexture object or NULL if the texture is not found
	 */
	virtual ITexture * GetTexture(unsigned int index) = 0;

	/** Reloads all the current Texture objects	*/
	virtual void ReloadTextures(void) = 0;
};

#endif // #ifndef _IGRAPHICS_H_

