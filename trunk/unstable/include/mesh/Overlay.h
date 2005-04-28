#ifndef _OVERLAY_H_
	#define _OVERLAY_H_

#include <mesh/Mesh.h>

class Rect;

//	Overlay stretch types
#define STRETCH_TOP		2	//	stretches an overlay upwards
#define STRETCH_BOTTOM	4	//	stretches an overlay downwards
#define STRETCH_LEFT	8	//	stretches an overlay to the left
#define STRETCH_RIGHT	16	//	stretches an overlay to the right
#define STRETCH_CENTRE	32	//	stretches an overlay in all directions

/**	@ingroup	Mesh_Graphics_Group
 *	@brief		Derived Mesh class which serves as a base class for all Overlay objects
 */
class Overlay: public Mesh{
protected:
	/**	@var		std::vector<Vector2f *> m_texcoords
	 *	@brief	An array of Texture coordinate arrays
	 *
	 *	To allow the overlay to have animations you hold
	 *	multiple sets of texture coordinates, to update
	 *	the animation of the overlay, set the mesh's
	 *	texture coordinate pointer to another set of coordinates
	 *
	 *	Think of animation frames contained within one image, each 
	 *	animation frame == one set of texture coordinates(one rectangle in the image).
	 *	To change animation frame, simply set the mesh's texture coordinates to
	 *	the frame of animation requested
	 */
	std::vector<Vertex2f *> m_texcoords;
public:
					Overlay		();
	virtual			~Overlay	();
	virtual void	AddFrame	(Rect *tr=NULL);
	virtual void	SetFrame	(unsigned int frameid);
	virtual void	SetTexture	(ITexture *texture);
	virtual void	Stretch		(int Direction, float amt);
};

#endif // #ifndef _OVERLAY_H_
