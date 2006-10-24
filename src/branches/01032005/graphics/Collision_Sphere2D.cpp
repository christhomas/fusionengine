#include <graphics/Collision.h>
#include <cmath>

Collision_Sphere2D::Collision_Sphere2D()
{

}

Collision_Sphere2D::~Collision_Sphere2D()
{

}

#include <fstream>
extern std::ofstream logfile;

void Collision_Sphere2D::CalculateBox(Entity *e, int id)
{
	//	Make a m_centre point for this entity and store the radius
	Maths::Vector *p	=	e->GetPosition();
	Maths::Vector *s	=	e->GetScale();
	Maths::Vector *v	=	e->GetVector();

	if(s->x>s->y){
		m_rad[id] = (int)(s->x/2);
	}else{
		m_rad[id] = (int)(s->y/2);
	}

	m_centre[id][DST].Set(p->x+m_rad[id],p->y+m_rad[id],0);
	m_centre[id][SRC] = m_centre[id][DST] - (*v);
}

bool Collision_Sphere2D::Intersect(void)
{
//	logfile << "\n\nIntersect()" << std::endl;

#define A0	m_centre[SRC][0]
#define A1	m_centre[SRC][1]
#define B0	m_centre[DST][0]
#define B1	m_centre[DST][1]

	Maths::Vector va	=	A1 - A0;
	Maths::Vector vb	=	B1 - B0;
	Maths::Vector AB	=	B0 - A0;
	Maths::Vector vab	=	vb - va;
	
	int		rab		=	m_rad[SRC]+m_rad[DST];
	double	a		=	vab.DotProduct(vab);
	double	b		=	2*vab.DotProduct(AB);
	double	c		=	AB.DotProduct(AB) - (rab*rab);
	double	v1len	=	va.Length();
	double	v2len	=	vb.Length();

	if((v1len+v2len) == 0){
		// both of them are stationary, they can't collide
		return false;
	}

	if(AB.DotProduct(AB) <= (rab*rab)){
		m_time = 0;
		return true;
	}

	double q = (b*b) - (4*a*c);
	if(q>=0){
		double sq = sqrt(q);
		double d = 1 / (2*a);
		double r1 = (-b + sq) * d;
		double r2 = (-b - sq) * d;

		if(r1>r2){
			std::swap(r1,r2);
		}

		m_time = r1;

		if((v1len*v2len) == 0){
			// one of them is stationary
			m_time = r2;
		}

		if(m_time >= 0 && m_time <= 1){
			//logfile << "Time of impact = " << m_time << std::endl;
			return true;
		}
	}

	return false;
}


void Collision_Sphere2D::def_VectorCorrection(Entity *e1,Entity *e2)
{
	//logfile << "Correcting vectors, Time = " << m_time << "\t e1 = " << e1 << ", e2 = " << e2 << std::endl;

	m_time = 1 - m_time;

	Maths::Vector *v1 = e1->GetVector();
	Maths::Vector *v2 = e2->GetVector();

	Maths::Vector c1 = (*v1)*m_time;
	Maths::Vector c2 = (*v2)*m_time;

	e1->SetTranslate(TRANSLATE_REL,c1);
	e2->SetTranslate(TRANSLATE_REL,c2);

	v1->Set(0,0,0);
	v2->Set(0,0,0);
}
