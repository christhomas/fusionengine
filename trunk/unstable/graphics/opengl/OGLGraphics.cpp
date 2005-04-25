#include <OpenGL12.h>
#include <OGLGraphics.h>
#include <OGLStaticVB.h>
#include <OGLDynamicVB.h>
#include <OGLImageTexture.h>
#include <OGLProceduralTexture.h>

#include <Fusion.h>
 
Fusion *fusion;

#ifdef _WIN32
	#include <win32/WGLGraphics.h>
#else
	#include <unix/GLXGraphics.h>	
#endif

void GetInstance(Fusion &f)
{
	fusion = &f;
	
	if(fusion->Graphics == NULL){
#ifndef _WIN32		
		fusion->Graphics		=	new GLXGraphics();
#else
		fusion->Graphics		=	new WGLGraphics();
#endif

		fusion->Mesh		=	new MeshDB();
		fusion->Scene		=	new SceneGraphDB();
		fusion->Interface	=	new UserInterfaceDB();
		fusion->Font		=	new FontDB();

		if(fusion->Graphics->Initialise()		==	false) delete fusion->Graphics;
		if(fusion->Mesh->Initialise()		==	false) delete fusion->Mesh;
		if(fusion->Scene->Initialise()		==	false) delete fusion->Scene;
		if(fusion->Interface->Initialise()		==	false) delete fusion->Interface;
		if(fusion->Font->Initialise()		==	false) delete fusion->Font;
	}
}

 void DestroyInstance(void)
{
	if(fusion->Graphics != NULL){
		delete fusion->Font;
		delete fusion->Interface;
		delete fusion->Scene;
		delete fusion->Mesh;
		delete fusion->Graphics;

		fusion->Font		=	NULL;
		fusion->Interface	=	NULL;
		fusion->Scene		=	NULL;
		fusion->Mesh		=	NULL;
		fusion->Graphics	=	NULL;
	}
}

/**	OpenGL Constructor
 *	Enables the Graphics subsystem active and in 3D mode by default
 */
OGLGraphics::OGLGraphics()
{
	m_active		=	false;
	Enable3DMode	=	true;

	Enable(IGraphics::POLYGON);
	SetClearColour(0,0,0);
}

/**	OpenGL Deconstructor 
 *	Clears up all the memory assigned during the execution of the graphics subsystem.
 *
 *	Operation:
 *	-#	Deletes all the created ITexture objects
 */
OGLGraphics::~OGLGraphics()
{
	for(unsigned int a=0;a<Textures.size();a++) delete Textures[a];
	Textures.clear();
}

/**
 *	Nothing happens here, this object doesnt initialise any data or objects
 *	TODO:	Change SetupGraphics to Initialise (makes more sense and removes a dud method
 */
bool OGLGraphics::Initialise(void)
{
	return true;
}

/**	Reads the driver information, OpenGL 1.1 is required as a minimum, 
 *	if this is available, enable the default driver options
 *
 *	Operation:
 *	-#	enable GL_SMOOTH
 *	-#	enable GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST
 *	-#	enable GL_VERTEX_ARRAY
 *	-#	enable GL_NORMAL_ARRAY
 *
 *	@return true or false, depending on whether OpenGL 1.1 was found
 */
bool OGLGraphics::SetupGraphics(void)
{
	//	query the opengl driver for information about itself
	m_driverdata.Read();

	//	test whether the version of the driver is 1.1 and above
	if(strcmp(m_driverdata.version,"1.1") > 0)
	{
		glShadeModel(GL_SMOOTH);
		glClearDepth(1.0f);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
				
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		return true;
	}

	return false;
}


/**	Enables different rendering capabilities
 *
 *	@param	flag		The capability to enable
 *	@param	options	Any information the capability might need to enable correctly
 *
 *	Explanation of the different capabilities
 *	
 *************************
 *
 *	LIGHTING:
 *
 *************************
 *
 *	LIGHT:
 *
 *************************
 *
 *	MODE3D:
 *	Sets the OpenGL system to render in a typical 3D display
 *
 *	This method only needs to be called if Begin2D was called previously.
 *	The default operation of the OpenGL system is 3D, so this method is called
 *	automatically to setup a 3D rendering system
 *
 *************************
 *	MODE2D:
 *	Sets the OpenGL system to ignore perspective, therefore creating a "2D" display
 *
 *	Operation:
 *	-#	Disables the Depth Function
 *	-#	Sets the blend function to typical sprite blending (if alpha is 0 sprites pixels are transparent, alpha = 1, opaque)
 *	-#	Stores the original GL_PROJECTION matrix, so you can pop out back into 3D mode if required
 *	-#	Sets the new GL_PROJECTION matrix to this orthoganal view
 *	-#	Stores the original GL_MODELVIEW matrix, so you can pop out back into 3D mode if required
 *
 *************************
 *	WIREFRAME:
 *	Sets the OpenGL rendering mode to wireframe mode
 *
 *************************
 *	POLYGON:
 *	Sets the OpenGL rendering mode to polygon mode (triangle)
 *************************
 *	POINTS:
 *	Sets the OpenGL rendering mode to points mode (single point polygons)
 *************************
 */
bool OGLGraphics::Enable(int flag, int options)
{
	switch(flag){
		case IGraphics::LIGHTING:{
			glEnable(GL_LIGHTING);
			glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
			glEnable(GL_COLOR_MATERIAL);
		}break;

		case IGraphics::LIGHT:{
			options&=7;
			glEnable(GL_LIGHT0+options);
		}break;

		case IGraphics::MODE3D:{
			Enable3DMode = true;

			glDepthFunc(GL_LEQUAL);		//	The Type Of Depth Testing To Do
			glEnable(GL_DEPTH_TEST);		//	Enables Depth Testing

			glDisable(GL_BLEND);			//	Disable blending I set in Begin2D() if called
			glEnable(GL_DEPTH_FUNC);		//	Enable the depth buffer
				
			glMatrixMode(GL_PROJECTION);	//	Select The Projection Matrix
			glPopMatrix();					//	Restore The Old Projection Matrix
			glMatrixMode(GL_MODELVIEW);	//	Select The Modelview Matrix
			glPopMatrix();					//	Restore The Old Projection Matrix
		}break;

		case IGraphics::MODE2D:{
			Enable3DMode = false;
			glDisable(GL_DEPTH_FUNC);					//	Disable the depth buffer
			glBlendFunc(	GL_SRC_ALPHA,
						GL_ONE_MINUS_SRC_ALPHA);	//	Enable cutout type transparency
			glEnable(GL_BLEND);						//	Enable blending
				
			glMatrixMode(GL_PROJECTION);	//	Select The Projection Matrix
			glPushMatrix();					//	Store The Projection Matrix
			glLoadIdentity();				//	Reset The Projection Matrix
				
			// Set Up An Ortho Screen
			glOrtho(0,m_width,m_height,0,-1,1);
				
			glMatrixMode(GL_MODELVIEW);	//	Select The Modelview Matrix
			glPushMatrix();					//	Store The Modelview Matrix
			glLoadIdentity();				//	Reset The Modelview Matrix
		}break;

		case IGraphics::CULLFACE:{
			if(options == IGraphics::FRONTFACE){
				glCullFace(GL_FRONT);
				glEnable(GL_CULL_FACE);

			}else if(options == IGraphics::BACKFACE){
				glCullFace(GL_BACK);
				glEnable(GL_CULL_FACE);
			}
		}break;

		case IGraphics::WIREFRAME:{
			m_polygonmode = GL_LINES;
		}break;

		case IGraphics::POLYGON:{
			m_polygonmode = GL_TRIANGLES;
		}break;

		case IGraphics::POINTS:{
			m_polygonmode = GL_POINTS;
		}break;
	};

	return true;
}

bool OGLGraphics::Disable(int flag, int options)
{
	switch(flag){
		case IGraphics::LIGHTING:{
			glDisable(GL_LIGHTING);
			glDisable(GL_COLOR_MATERIAL);
		}break;

		case IGraphics::LIGHT:{
			options&=7;
			glDisable(GL_LIGHT0+options);
		}break;

		case IGraphics::CULLFACE:{
			glDisable(GL_CULL_FACE);
		}break;
	};

	return true;
}

int OGLGraphics::GetRenderMode(void)
{
	return m_polygonmode;
}

/**	Sets the OpenGL window's clear colour
 *
 *	@param r	The red component of the colour
 *	@param g	The green component of the colour
 *	@param b	The blue component of the colour
 */
void OGLGraphics::SetClearColour(float r, float g, float b)
{
	glClearColor(r,g,b,0.5f);
}

/**	If the window frame is clicked, this method will resize the opengl display within the frame
 *
 *	@param x	The new width for the window to resize to in pixel units
 *	@param y	The new height for the window to resize to in pixel units
 *
 *	Function will return early if the width and height are less than 1
 *	This also prevents division by zero
 *
 *	Operation:
 *	-#	Resets the current viewport
 *	-#	Resets the GL_PROJECTION matrix
 *	-#	Loads an identity projection matrix and then calls gluPerspective, 45deg FOV by default, near=1.0f, far=5000.0f (non-configurable)
 *	-#	Resets the GL_MODELVIEW matrix
 *	-#	Depending on what graphics mode is enabled, 2d or 3d, calls the appropiate Begin[2D/3D] method
 */
void OGLGraphics::ResizeWindowDisplay(int x, int y)
{
	if(x+y < 2) return;
	
	// Reset The Current Viewport
	glViewport(0,0,x,y);

	// Select The Projection Matrix	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	// Reset The Projection Matrix	
	glLoadIdentity();

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)x/(GLfloat)y,1.0f,5000.0f);

	// Select The Modelview Matrix
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	
	// Reset The Modelview Matrix	
	glLoadIdentity();

	if(Enable3DMode == false)	Enable(IGraphics::MODE2D);
	else						Enable(IGraphics::MODE3D);
}

/**	Updates the OpenGL Display
 *	by calling the IWindowDevice::Update() method
 */
void OGLGraphics::Update(void)
{
	Window->Update();
}

/**	After each frame of display, this method will 
 *	clear the display and depth buffer to present 
 *	you with a fresh canvas, so to speak.
 */
void OGLGraphics::ClearScreen(void)
{
	// Clear The Screen And The Depth Buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//=================================
//	Get	Video mode information
//=================================
void OGLGraphics::GetScreenResolution(int &x, int &y, int &bpp)
{
	x		= m_width;
	y		= m_height;
	bpp	= m_bpp;
}

int OGLGraphics::GetScreenWidth(void)
{
	return m_width;
}

int OGLGraphics::GetScreenHeight(void)
{
	return m_height;
}

int OGLGraphics::GetScreenDepth(void)
{
	return m_bpp;
}

/**	Create a Pixelbuffer texture
 *
 *	@param	x	The width of the texture
 *	@param	y	The height of the texture
 *
 *	@returns	An ITexture object or NULL if texture failed to create
 *
 *	@todo	Implement pbuffers
 */
ITexture * OGLGraphics::CreateTexture(int x, int y)
{
	return NULL;
}

/**	Creates an Image based Texture
 *
 *	@param image	The filename containing the image to use as a texture
 *
 *	@returns An ITexture object or NULL if texture failed to create
 *
 *	Operation:
 *		-#	If the number of textures is zero, enable GL_TEXTURE_COORD_ARRAY
 *		-#	Loop through the current existing textures and attempt to find
 *				a copy of the texture you are attempting to load
 *		-#	If a copy is found, return an ITexture pointer to that texture
 *		-#	If the texture is not found, Create a new ImageTexture object
 *		-#	Update the texture (this will load the image contents into the opengl texture object)
 *		-#	Store the texture pointer
 *		-#	Return the texture pointer
 */
ITexture * OGLGraphics::CreateTexture(char *image)
{
	ITexture *texture = NULL;
	
	for(unsigned int a=0;a<Textures.size();a++){
		char *test = Textures[a]->m_filename;

		if(strcmp(test,image) == 0){
			texture = Textures[a];
		}
	}
	
	if(texture == NULL){
		texture	= new OGLImageTexture(image);

		if(texture->UpdateTexture() >= 0){
			Textures.push_back(texture);
		}else{
			delete texture;
			texture = NULL;
		}
	}

	return texture;
}

/**	Creates a Procedural based texture
 *
 *	@param	x				The width of the texture
 *	@param	y				The height of the texture
 *	@param	numcomp	The number of components in each pixel (3 = RGB, 4 = RGBA, etc)
 *	@param	proc		The procedure which will create the texture
 *
 *	@returns	An ITexture object or NULL if the texture failed to create
 *
 *	Operation:
 *		-#	Test how many textures exist, if zero, enable GL_TEXTURE_COORD_ARRAY
 *		-#	Create a new Procedural texture
 *		-#	Update the texture (this will load the image contents into the opengl texture object)
 *		-#	Store the texture pointer
 *		-#	Return the texture pointer
 */
ITexture * OGLGraphics::CreateTexture(int x, int y, int numcomp, ITexture::textureproc_t proc)
{
	if(Textures.size() == 0)	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	ITexture *texture = new OGLProceduralTexture(x,y,numcomp,proc);
	if(texture->UpdateTexture() >= 0){
		Textures.push_back(texture);
	}else{
		delete texture;
		texture = NULL;
	}

	return texture;
}

/**	Deletes a texture
 *
 *	@param texture	An ITexture pointer to remove from the Graphics object
 *
 *	Operation:
 *		-#	Loop through the textures and compare each texture pointer to the one passed in
 *		-#	If a match is found, remove the texture from the array and return
 */
void OGLGraphics::DeleteTexture(ITexture *texture)
{
	for(unsigned int a=0;a<Textures.size();a++){
		if(texture == Textures[a]){
			delete Textures[a];
			Textures.erase(Textures.begin()+a);
			return;
		}
	}
}

/**	Deletes a texture
 *
 *	@param index	The index of a texture to remove from the Graphics object
 *
 *	Operation:
 *		-#	Test the index whether it's valid
 *		-#	Remove the texture from the array
 */
void OGLGraphics::DeleteTexture(unsigned int index)
{
	ITexture *t = NULL;

	if(index < Textures.size()){
		delete Textures[index];
		Textures.erase(Textures.begin()+index);
	}
}

/**	Retrieves a Texture
 *
 *	@param index	An index to a texture object
 *
 *	@return	An ITexture object or NULL if the texture was not found
 */ 
ITexture * OGLGraphics::GetTexture(unsigned int index)
{
	ITexture *t = NULL;

	if(index < Textures.size()) t = Textures[index];
	
	return t;
}

/**	Reloads all the texture objects
 *
 *	Operation:
 *		-#	Loops through all the textures
 *		-#	Calls ITexture::Reload()
 */
void OGLGraphics::ReloadTextures(void)
{
	for(unsigned int a=0;a<Textures.size();a++){
		Textures[a]->Reload();
	}
}

/**	Sets the position of a light
 *
 *	@param light			The ID of the light to enable
 *	@param position		A Maths::Vector object containing the position
 */
void OGLGraphics::LightPosition(unsigned int light, Maths::Vector &position)
{
	light &= 7;
	
	GLfloat p[4] = {position.x,position.y,position.z,1};

	glLightfv(GL_LIGHT0+light, GL_POSITION, p);
}

/**	Gives a light a Diffuse value
 *
 *	@param	light		The ID of the light
 *	@param	diffuse	The Colour4f object containing the colour to use
 */
void OGLGraphics::LightDiffuse(unsigned int light, Colour4f *diffuse)
{
	light &= 7;

	glLightfv(GL_LIGHT0+light,GL_DIFFUSE,(float *)diffuse);
}

/**	Gives a light an Ambient value
 *
 *	@param light		The ID of the light
 *	@param ambient	The Colour4f object containing the colour to use
 */
void OGLGraphics::LightAmbient(unsigned int light, Colour4f *ambient)
{
	light &= 7;

	glLightfv(GL_LIGHT0+light,GL_AMBIENT,(float *)ambient);
}

/**	Creates an VertexBuffer object
 *
 *	@param mode	The type of vertex buffer to create
 *
 *	@returns	An IVertexBuffer obect or NULL if failed to create
 */
IVertexBuffer * OGLGraphics::CreateVertexBuffer(int mode)
{
	if(mode & IVertexBuffer::STATIC)	return new OGLStaticVB();
	if(mode & IVertexBuffer::DYNAMIC)	return new OGLDynamicVB();

	return NULL;
}

/** Sets the colour to render geometry in 
 *
 *	@param	rgba	The rgba colour to set
 */
void OGLGraphics::SetColour(float *rgba)
{
	glColor4fv(rgba);
}

/** Sets the colour to render geometry in 
 *
 *	@param	r	The red component
 *	@param	g	The green component
 *	@param	b	The blue component
 *	@param	a	The alpha component
 */
void OGLGraphics::SetColour(float r, float g, float b, float a)
{
	glColor4f(r,g,b,a);
}

/** Loads an identity matrix onto the matrix stack */
void OGLGraphics::LoadIdentity(void)
{
	glLoadIdentity();
}

/** Translates the current rendering position 
 *
 *	@param	position A Maths::Vector object containing the position to render at
 */
void OGLGraphics::Translate(Maths::Vector &position)
{
	glTranslatef(position.x,position.y,position.z);
}

/** Translates the current rendering position 
 *
 *	@param x	The X value to render at
 *	@param y	The Y value to render at
 *	@param z	The Z value to render at
 */
void OGLGraphics::Translate(float x, float y, float z)
{
	glTranslatef(x,y,z);
}

/** Scales the geometry to be rendered 
 *
 *	@param	scale A Maths::Vector object containing the scale to render at
 */
void OGLGraphics::Scale(Maths::Vector &scale)
{
	glScalef(scale.x,scale.y,scale.z);
}

/** Scales the geometry to be rendered 
 *
 *	@param	x	The X scalar
 *	@param	y	The Y scalar
 *	@param	z	The Z scalar
 */
void OGLGraphics::Scale(float x, float y, float z)
{
	glScalef(x,y,z);
}

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
void OGLGraphics::Rotate(float *rotate)
{
	glRotatef(rotate[0],rotate[1],rotate[2],rotate[3]);
}

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
void OGLGraphics::Rotate(float angle, float x, float y, float z)
{
	m_quat.LoadIdentity();
	m_quat.Rotate(angle,x,y,z);

	MultiplyMatrix(m_quat.GetMatrix());
}

/** Rotates the current rendering position 
 *
 *	@param	r	The Roll component
 *	@param	p	The pitch component
 *	@param	y	The Yaw component
 */
void OGLGraphics::Rotate(float r, float p, float y)
{
	m_quat.LoadIdentity();
	m_quat.Rotate(r,p,y);

	MultiplyMatrix(m_quat.GetMatrix());
}

/** Multiplies a Matrix with the matrix on the top of the matrix stack
 *
 *	@param matrix	A 16 element array of floating point values, which is used as a matrix
 */
void OGLGraphics::MultiplyMatrix(float *matrix)
{
	glMultMatrixf(matrix);
}

