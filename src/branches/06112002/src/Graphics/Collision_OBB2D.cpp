#include <Graphics/Collision.h>
#include <Maths/Maths.h>
#include <Maths/Quaternion.h>
#include <Maths/Matrix.h>

#include <cmath>

//	This header is win32 only and provides the prototype for _isnan()
#include <float.h>

int point;

Collision_OBB2D::Collision_OBB2D()
{
	point = 0;
}

Collision_OBB2D::~Collision_OBB2D()
{
}

void Collision_OBB2D::CalculateBox(Entity *e, int id)
{
	//	Basic algorithm for creating a correct collision box
	//*********
	//	1.	Create a collision box from the entity
	//	2.	reduce the box by the required amount
	//	3.	rotate the box to the entities rotation angle
	//	4.	translate the box to the entities position
	//*********	
	
	Maths::Vector			axis;
	Maths::Vector			*scale		= e->GetScale();
	Maths::Vector			*position	=	e->GetPosition();
	Maths::Vector			*rotation	=	e->GetRotation(axis);
	Maths::Quaternion	q;
	Maths::Matrix			m;

	int	width_reduce	= m_reduce_x	*	(scale->x/100);
	int	height_reduce	= m_reduce_y	*	(scale->y/100);

	//	Calculate the box
	rect[id][0].x	=	-(axis.x+width_reduce);
	rect[id][0].y	=	-(axis.y+height_reduce);
	rect[id][0].z	=	0;

	rect[id][1].x	=	scale->x-axis.x-width_reduce;
	rect[id][1].y	=	rect[id][0].y;
	rect[id][1].z	=	0;

	rect[id][2].x	=	rect[id][1].x;
	rect[id][2].y	=	scale->y-axis.y-height_reduce;
	rect[id][2].z	=	0;

	rect[id][3].x	=	rect[id][0].x;
	rect[id][3].y	=	rect[id][2].y;
	rect[id][3].z	=	0;

	//	Create a matrix containing the rotation required
	q.LoadIdentity();
	q.Rotate(0,0,rotation->z);

	m = q;

	//	Rotate the box
	rect[id][0] *= m;
	rect[id][1] *= m;
	rect[id][2] *= m;
	rect[id][3] *= m;

	//	Now translate the resized, rotated box to it's correct screen position
	rect[id][0] += *position;
	rect[id][1] += *position;
	rect[id][2] += *position;
	rect[id][3] += *position;
}

bool Collision_OBB2D::Intersect(void)
{
	Maths::Vector v1,v2;
	float angle,temp;

	if(rect[SRC][0].z == rect[DST][0].z){
		for(int a=0;a<4;a++){
			angle = 0;

			for(int b=0;b<4;b++){
				//	test the rect[DEST][b] rect[DEST][(b+1)%4] edge against the rect[SRC][a] point
				//	an early exit is made when one point in rect[SRC][*] is found within rect[DEST][*]

				v1 = rect[DST][b]				- rect[SRC][a];
				v2 = rect[DST][(b+1)%4]	- rect[SRC][a];

				v1/=v1.Length();
				v2/=v2.Length();				

				temp = acos(v1.DotProduct(v2));

				if(_isnan(temp)) temp = 0;

				angle += temp;
			}

			if(angle >= (2*PI)){
				point = a;
				return true;
			}
		}
	}
	return false;
}

#include <fstream>
std::ofstream logfile("output.txt");

void Collision_OBB2D::def_VectorCorrection(Entity *e1,Entity *e2)
{
	Maths::Vector *v = e1->GetVector();
	Maths::Vector p[2][2];

#define A	p[SRC][0]
#define B p[SRC][1]
#define C p[DST][0]
#define D p[DST][1]

	A = rect[SRC][0];
	B = A-(*v);

	for(int a=0;a<4;a++){
		C	=	rect[DST][a];
		D	=	rect[DST][(a+1)%4];

		float d = ((B.x-A.x)*(D.y-C.y))-((B.y-A.y)*(D.x-C.x));

		if(d!=0){
			float rn,sn,r,s;

			rn = ((A.y-C.y)*(D.x-C.x))-((A.x-C.x)*(D.y-C.y));
			sn = ((A.y-C.y)*(B.x-A.x))-((A.x-C.x)*(B.y-A.y));

			r = rn/d;
			s = sn/d;

			if(r>=0&&r<=1&&s>=0&&r<=1){
				Maths::Vector c(r*(B.x-A.x),r*(B.y-A.y),0);

				//c/=2;

				e1->SetTranslate(TRANSLATE_REL,c);
				//e2->SetTranslate(TRANSLATE_REL,-c);
				return;
			}
		}
	}
}
