#include <graphics/Camera.h>
#include <cmath>
#include <cstring>
#include <Fusion.h>

Camera::Camera()
{
	m_Name = NULL;
}

Camera::~Camera()
{
	delete m_Name;
}

void Camera::SetName(char *n)
{
	delete m_Name;
	m_Name = new char[strlen(n)+1];
	strcpy(m_Name,n);
}

char * Camera::GetName(void)
{
	return m_Name;
}

void Camera::Zoom(float z)
{
	//	put code to "zoom" the camera here
}

void Camera::Rotate(float x, float y, float z)
{
	Maths::Vector r(y,x,z);
	Rotate(r);
}

void Camera::Rotate(Maths::Vector &r)
{
	//	Increment the angle, then constrain to between 0 and 360
	m_angle += r;

	m_angle.x = (float)( ((int)m_angle.x)%360 );
	m_angle.y = (float)( ((int)m_angle.y)%360 );
	m_angle.z = (float)( ((int)m_angle.z)%360 );

	m_quat.LoadIdentity();
	m_quat.Rotate(m_angle.x,m_angle.y,m_angle.z);

	m_view = m_quat;
}

void Camera::Translate(float dist, int direction)
{
	//	NOTE:	Since the 6DOF axes are rotated, they represent easy ways to move in their direction
	//			Merely by adding them to the position, using them like increments
	//			we of course only store 3 vectors, since you can move in two directions per vector +'vely and -'vely 

	float *elements = m_view.GetElements();

	Maths::Vector t(elements[0+direction],elements[4+direction],elements[8+direction]);

	m_position += t*dist;
}

void Camera::Update(void)
{
	fusion->Graphics->LoadIdentity();
	
	//	Multiply your rotation matrix against the identity and then translate to your new position
	fusion->Graphics->MultiplyMatrix(m_view.GetElements());
	fusion->Graphics->Translate(-m_position.x,-m_position.y,-m_position.z);
}

