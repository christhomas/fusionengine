#include <mesh/Overlay.h>
#include <graphics/IGraphics.h>
#include <Rect.h>

/**	Overlay constructor
 *
 *	Operation:
 *		-#	Create a surface for the overlay's texture
 *		-#	Initialise the mesh base class to hold 4 vertices
 *		-#	Initialise the surface to hold 6 indicies (3 for each triangle, 2 triangles for one rectangle)
 *		-#	Assign the default vertex data for the rectangle
 *		-#	Assign the default normal data for the rectangle
 *		-#	Assign the triangle indices
 *		-#	Set the surfaces vertex/normal/index data
 *		-#	Generate a white material and update the surface with it
 */
Overlay::Overlay()
{
	IVertexBuffer *v = AddVertexBuffer();
	
	Initialise(4);
	v->Initialise(GetNumVertex(),6,3,2);
	int *index = v->GetIndex();

	//	Assign all the m_position data
	m_position[0].x	= 0;	m_position[0].y	= 0;	m_position[0].z	= 0;
	m_position[1].x	= 0;	m_position[1].y	= 1;	m_position[1].z	= 0;
	m_position[2].x	= 1;	m_position[2].y	= 1;	m_position[2].z	= 0;
	m_position[3].x	= 1;	m_position[3].y	= 0;	m_position[3].z	= 0;

	//	Assign all the m_normal data
	m_normal[0].x = 0;	m_normal[0].y = 0;	m_normal[0].z = 1;
	m_normal[1].x = 0;	m_normal[1].y = 0;	m_normal[1].z = 1;
	m_normal[2].x = 0;	m_normal[2].y = 0;	m_normal[2].z = 1;
	m_normal[3].x = 0;	m_normal[3].y = 0;	m_normal[3].z = 1;
	
	//	Assign all the polygon indices
	index[0] = 0;	index[1] = 1;	index[2] = 2;
	index[3] = 0;	index[4] = 3;	index[5] = 2;

	v->SetPosition((float *)m_position);
	v->SetNormal((float *)m_normal);
	v->SetIndex(index);
	
	Material *m = v->GetMaterial();
	m->colour.r = 1;
	m->colour.g = 1;
	m->colour.b = 1;
	m->colour.a = 1;
	v->SetMaterial(m);
}

/**	Overlay Deconstructor
 *
 *	Delete all the assigned texture coordinates for the overlay
 */
Overlay::~Overlay()
{
	for(unsigned int a=0;a<m_texcoords.size();a++)	delete m_texcoords[a];
	m_texcoords.clear();
}

/**	Add an animation frame
 *
 *	@param tr A Rect object containing the texture coordinates for another animation frame
 *
 *	Operation:
 *		-#	Retrieve a pointer to the texture for this overlay
 *		-#	Find it's dimensions
 *		-#	Precalculate a scale value to convert 0->max to 0->1 so you can specify the rectangle in pixels, but convert them into texture coordinates
 *		-#	Convert the rectangle passed into the method into texture coordinates
 *		-#	Invert the y coordinates of the texture coordinates (rectangles are specified where 
 *				the origin is the top left, but texture coordinates specify the origin as the bottom left (hence why the y must be inverted)
 *		-#	Stores the texture coordinates generated, sets the default animation frame to be the first frame
 */
void Overlay::AddFrame(Rect *tr)
{
	int		x,y;
	float	tw,th;

	ITexture *t = m_vertexbuffer[0]->GetTexture();

	if(t != NULL){
		t->GetDimensions(x,y);
		
		tw = (float)x;
		th = (float)y;

		tw = 1/tw;
		th = 1/th;

		if(tr == NULL) tr = new Rect(0,0,x,y);
		
		//	Assign default texture coords
		Vertex2f *texcoords = new Vertex2f[4];

		texcoords[0].x	=	tw*tr->left;
		texcoords[0].y	=	th*tr->top;

		texcoords[1].x	=	tw*tr->left;
		texcoords[1].y	=	th*tr->bottom;

		texcoords[2].x	=	tw*tr->right;
		texcoords[2].y	=	th*tr->bottom;

		texcoords[3].x	=	tw*tr->right;
		texcoords[3].y	=	th*tr->top;

		//	Store the texture coords and set the default frame to zero
		m_texcoords.push_back(texcoords);
		SetFrame(0);
	}

	delete tr;
}

/**	Sets a new animation frame
 *
 *	@param	index	The frame to set
 */
void Overlay::SetFrame(unsigned int index)
{
	Vertex2f *t = m_texcoords[index];

	m_vertexbuffer[0]->SetTextureLayer(0,(float *)t,m_vertexbuffer[0]->GetTexture());
}

/**	Sets the texture of this overlay
 *
 *	@param texture	The texture to use on this overlay
 */
void Overlay::SetTexture(ITexture *texture)
{
	m_vertexbuffer[0]->SetTextureLayer(0,m_vertexbuffer[0]->GetTexcoord(),texture);
}

/**	Stretches an overlays size
 *
 *	@param Direction	The direction in which to stretch
 *	@param amt				The amount to stretch the overlay
 *
 *	This method works like so:
 *
 *	Take a square, chop it into 9 like this
 *
 *	X 2 X \n
 *	4 5 6 \n
 *	X 8 X \n
 *
 *	2,4,5,6,8 are the directions in which the overlay can be stretched
 *
 *	to define stretch 2,	Direction == STRETCH_TOP\n
 *	to define stretch 1,	Direction == STRETCH_TOP | STRETCH_LEFT\n
 *	to define stretch 3,	Direction == STRETCH_TOP | STRETCH_RIGHT\n
 *	to define stretch 7,	Direction == STRETCH_BOTTOM | STRETCH_LEFT\n
 *	to define stretch 5,	Direction == STRETCH_CENTRE\n
 *
 *	etc,etc,etc
 *
 *	There is one rule, if STRETCH_CENTRE is used, it cannot be used with
 *	any other STRETCH command, since STRETCH_CENTRE is ignored when 
 *	any other flag is used
 */
void Overlay::Stretch(int Direction, float amt)
{
	if(Direction & STRETCH_CENTRE){
		m_position[0].x	-= amt;
		m_position[0].y	-= amt;

		m_position[1].x	-= amt;
		m_position[1].y	+= amt;

		m_position[2].x	+= amt;
		m_position[2].y	+= amt;

		m_position[3].x	+= amt;
		m_position[3].y	-= amt;
	}

	if(Direction & STRETCH_TOP){
		m_position[0].y -= amt;
		m_position[3].y -= amt;
	}

	if(Direction & STRETCH_BOTTOM){
		m_position[1].y	+= amt;
		m_position[2].y	+= amt;
	}

	if(Direction & STRETCH_LEFT){
		m_position[0].x -= amt;
		m_position[1].x -= amt;
	}

	if(Direction & STRETCH_RIGHT){
		m_position[2].x += amt;
		m_position[3].x += amt;
	}
}

