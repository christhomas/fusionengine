#include <graphics/Collision.h>
#include <maths/Maths.h>
#include <maths/Quaternion.h>
#include <maths/Matrix.h>

#include <cmath>

//	This header is win32 only and provides the prototype for _isnan()
#include <float.h>

Collision_OBB2D::Collision_OBB2D()
{
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

	int	width_reduce	= (int)(m_reduce_x * (scale->x/100));
	int	height_reduce	= (int)(m_reduce_y * (scale->y/100));
	float	sx	=	scale->x;
	float	sy	=	scale->y;

	//	Calculate the box
	rect[id][0].x	=	0;
	rect[id][0].y	=	0;
	rect[id][0].z	=	0;

	rect[id][1].x	=	sx;
	rect[id][1].y	=	0;
	rect[id][1].z	=	0;

	rect[id][2].x	=	sx;
	rect[id][2].y	=	sy;
	rect[id][2].z	=	0;

	rect[id][3].x	=	0;
	rect[id][3].y	=	sy;
	rect[id][3].z	=	0;

	//	Create a matrix containing the rotation required
	q.LoadIdentity();
	q.Rotate(0,0,rotation->z);

	//	Decompose the quaternion into a matrix
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
	Maths::Vector	v1,v2;
	float					angle,temp;
	int						a,b;
	int						obb[2]={SRC,DST};

	if(rect[SRC][0].z == rect[DST][0].z){
		for(a=0;a<4;a++){
			angle = 0;

			for(b=0;b<4;b++){
				//	test the rect[DEST][b] rect[DEST][(b+1)%4] edge against the rect[SRC][a] point
				//	an early exit is made when one point in rect[SRC][*] is found within rect[DEST][*]

				v1 = rect[obb[DST]][b]				- rect[obb[SRC]][a];
				v2 = rect[obb[DST]][(b+1)%4]	- rect[obb[SRC]][a];

				v1/=v1.Length();
				v2/=v2.Length();				

				temp = acos(v1.DotProduct(v2));

				//	TODO: PORTING required here _isnan is a win32 function
				#ifdef _WIN32
				if(_isnan(temp)) temp = 0;
				#endif

				angle += temp;
			}

			intersect[SRC][a] = false;
			if(angle >= (2*PI)){
				intersect[SRC][a] = true;
				return true;
			}
		}

		obb[0] = DST;
		obb[1] = SRC;

		for(a=0;a<4;a++){
			angle = 0;

			for(b=0;b<4;b++){
				//	test the rect[DEST][b] rect[DEST][(b+1)%4] edge against the rect[SRC][a] point
				//	an early exit is made when one point in rect[SRC][*] is found within rect[DEST][*]

				v1 = rect[obb[DST]][b] - rect[obb[SRC]][a];
				v2 = rect[obb[DST]][(b+1)%4] - rect[obb[SRC]][a];

				v1/=v1.Length();
				v2/=v2.Length();

				temp = acos(v1.DotProduct(v2));

				//	TODO: PORTING required here _isnan is a win32 function
				#ifdef WIN32
				if(_isnan(temp)) temp = 0;
				#endif

				angle += temp;
			}

			intersect[DST][a] = false;
			if(angle >= (2*PI)){
				intersect[DST][a] = true;
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
	logfile << "\n\nVectorCorrection" << std::endl;
	Entity		*e = e2;
	Maths::Vector	*v = e->GetVector();
	Maths::Vector	p[2][2];
	int			a,b;
	float			max=0;
	int			obb[2] = {DST,SRC};

	//	If src moves,		dest is stationary,	src is repositioned
	//	If src moves,		dest is moving,		dest is repositioned (first come, first served)
	//	If src is stationary,	dest is moving,		dest is repositioned

	if(v->Length() == 0){
		e = e1;
		v = e->GetVector();
		obb[0] = SRC;
		obb[1] = DST;

		logfile << "Src is being corrected" << std::endl;
	}else{
		logfile << "Dest is being corrected" << std::endl;
	}

#define A	p[SRC][0]
#define B p[SRC][1]
#define C p[DST][0]
#define D p[DST][1]

	for(a=0;a<4;a++){
		A = rect[obb[SRC]][a];
		B = A-(*v);

		for(b=0;b<4;b++){
			C	=	rect[obb[DST]][b];
			D	=	rect[obb[DST]][(b+1)%4];

			float d = ((B.x-A.x)*(D.y-C.y))-((B.y-A.y)*(D.x-C.x));

			if(d!=0){
				float rn,sn,r,s;

				rn	=	((A.y-C.y)*(D.x-C.x))-((A.x-C.x)*(D.y-C.y));
				sn	=	((A.y-C.y)*(B.x-A.x))-((A.x-C.x)*(B.y-A.y));

				r		=	rn/d;
				s		=	sn/d;

				if(r>=0&&r<=1&&s>=0&&s<=1){
					if(r>max) max = r;
				}
			}
		}
	}

	for(a=0;a<4;a++){
		A=rect[obb[SRC]][a];
		B=A-(*v);

		logfile << "Vector_A["<<a<<"] = {" << A.x << ", " << A.y << "};" << std::endl;
		logfile << "Vector_B["<<a<<"] = {" << B.x << ", " << B.y << "};" << std::endl;

		if(intersect[SRC][a] == true){
			if(obb[SRC] == SRC)	logfile << "A is within DEST" << std::endl;
			else								logfile << "A is within SRC" << std::endl;
		}	
	}

	if(obb[SRC]==DST) v=e1->GetVector();

	for(a=0;a<4;a++){
		C	=	rect[obb[DST]][a];
		D	=	C-(*v);

		logfile << "Vector_C["<<a<<"] = {" << C.x << ", " << C.y << "};" << std::endl;
		logfile << "Vector_D["<<a<<"] = {" << D.x << ", " << D.y << "};" << std::endl;

		if(intersect[DST][a] == true){
			if(obb[DST] == SRC)	logfile << "C is within DEST" << std::endl;
			else								logfile << "C is within SRC" << std::endl;
		}	
	}

	if(max>0){
		logfile << "r = " << max << std::endl;
		Maths::Vector c(max*0.01*(B.x-A.x),max*0.01*(B.y-A.y),0);
		e->SetTranslate(TRANSLATE_REL,c);
	}else{
		logfile << "r = " << max << ",\tCollision could not be determined" << std::endl;
	}
}

/*
void Collision_OBB2D::def_VectorCorrection(Entity *e1,Entity *e2)
{
	/*
		
			If src collided with dst, then src needs to move back
			if dst collided with src, then dst needs to move back
			if src AND dst collided together, then ???

	*

	logfile << "\n\nVectorCorrection" << std::endl;
	Entity				*e;
	Maths::Vector *v,p[2][2];
	int						a=0;

	if(col[SRC] == DST)	e	=	e1;
	else								e	=	e2;

	v	=	e->GetVector();

	Maths::Vector *temp;

	if(col[SRC] == DST){
		logfile << "Detecting collision between DST and SRC" << std::endl;
	}else{
		logfile << "Detecting collision between SRC and DST" << std::endl;
	}

	temp = e1->GetVector();
	logfile << "e1 vector = {" << temp->x << ", " << temp->y << ", " << temp->z << "};" << std::endl;
	temp = e2->GetVector();
	logfile << "e2 vector = {" << temp->x << ", " << temp->y << ", " << temp->z << "};" << std::endl;

#define A	p[SRC][0]
#define B p[SRC][1]
#define C p[DST][0]
#define D p[DST][1]

//	A = rect[col[SRC]][point];
	A = rect[col[SRC]][point];
	B = A-(*v);

	logfile << "A = {" << A.x << ", " << A.y << ", " << A.z << "}" << std::endl;
	logfile << "B = {" << B.x << ", " << B.y << ", " << B.z << "}" << std::endl;

	A = rect[col[SRC]][point];
	B = A-(*v);

	logfile << "A = {" << A.x << ", " << A.y << ", " << A.z << "}" << std::endl;
	logfile << "B = {" << B.x << ", " << B.y << ", " << B.z << "}" << std::endl;

	logfile << "Checking against edge[SRC] " << std::endl;
	for(a=0;a<4;a++){
		logfile << rect[SRC][a].x << ", " << rect[SRC][a].y << ", " << rect[SRC][a].z << std::endl;
		logfile << rect[SRC][a].x << ", " << rect[SRC][a].y << ", " << rect[SRC][a].z << std::endl;
	}	

	logfile << "Checking against edge[DST] " << std::endl;
	for(a=0;a<4;a++){
		logfile << rect[DST][a].x << ", " << rect[DST][a].y << ", " << rect[DST][a].z << std::endl;
		logfile << rect[DST][a].x << ", " << rect[DST][a].y << ", " << rect[DST][a].z << std::endl;
	}	

	for(a=0;a<4;a++){
		logfile << "Checking edge " << (a+1) << std::endl;
		C	=	rect[col[DST]][a];
		D	=	rect[col[DST]][(a+1)%4];

		float d = ((B.x-A.x)*(D.y-C.y))-((B.y-A.y)*(D.x-C.x));

		if(d!=0){
			logfile << "Denominator is not zero, collision upon this line, checking against edge" << std::endl;
			float rn,sn,r,s;

			rn = ((A.y-C.y)*(D.x-C.x))-((A.x-C.x)*(D.y-C.y));
			sn = ((A.y-C.y)*(B.x-A.x))-((A.x-C.x)*(B.y-A.y));

			r = rn/d;
			s = sn/d;

			logfile << "r = " << r << std::endl;
			logfile << "s = " << s << std::endl;

			if(r>=0&&s>=0&&r<=1&&s<=1){
				logfile << "r AND s were within boundaries, that means a collision on this edge" << std::endl;
				Maths::Vector c(r*(B.x-A.x),r*(B.y-A.y),0);

				logfile << "Adjustment vector = {" << c.x << ", " << c.y << ", " << c.z << "} distance = " << c.Length() << std::endl;

				e->SetTranslate(TRANSLATE_REL,c);
				return;
			}else{
				logfile << "The edges do not intersect" << std::endl;
			}
		}else{
			logfile << "The lines do not intersect" << std::endl;
		}
	}
}
*/
/*
void Collision_OBB2D::def_VectorCorrection(Entity *e1,Entity *e2)
{
	Maths::Vector *v = e1->GetVector();
	Maths::Vector p[2][2];

#define A	p[SRC][0]
#define B p[SRC][1]
#define C p[DST][0]
#define D p[DST][1]

	A = rect[SRC][point];
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

				e1->SetTranslate(TRANSLATE_REL,c);
				return;
			}
		}
	}
}
*/

