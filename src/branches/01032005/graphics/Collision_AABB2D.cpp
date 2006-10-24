#include <graphics/Collision.h>

Collision_AABB2D::Collision_AABB2D()
{
}

Collision_AABB2D::~Collision_AABB2D()
{
}

void Collision_AABB2D::CalculateBox(Entity *e, int id)
{
	Maths::Vector	*scale		= e->GetScale();
	Maths::Vector	*position	=	e->GetPosition();

	int	width_reduce	= (int)(m_reduce_x * (scale->x/100));
	int	height_reduce	= (int)(m_reduce_y * (scale->y/100));

	rect[id].Initialise(	
					(int)position->x+width_reduce,
					(int)position->y+height_reduce,
					(int)(position->x+scale->x-width_reduce),
					(int)(position->y+scale->y-height_reduce)
					);
}

bool Collision_AABB2D::Intersect(void)
{
	if( ((rect[DST].left >= rect[SRC].left)&&(rect[DST].left <= rect[SRC].right)) || ((rect[DST].right >= rect[SRC].left)&&(rect[DST].right <= rect[SRC].right)) )
	{
		if( ((rect[DST].top >= rect[SRC].top)&&(rect[DST].top <= rect[SRC].bottom)) || ((rect[DST].bottom >= rect[SRC].top)&&(rect[DST].bottom <= rect[SRC].bottom)))
		{
			return true;
		}
	}

	return false;
}

void Collision_AABB2D::def_VectorCorrection(Entity *e1,Entity *e2)
{	

}
