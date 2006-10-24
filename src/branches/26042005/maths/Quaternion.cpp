#include <maths/Maths.h>
#include <maths/Quaternion.h>
#include <cmath>
#include <memory.h>

using namespace Maths;

/**	Quaternion Constructor
 *
 *	By default an identity quaternion is loaded
 */
Quaternion::Quaternion()
{
	LoadIdentity();
}

/**	Quaternion Constructor
 *
 *	@param q	The quaternion to make this a copy of
 */
Quaternion::Quaternion(Quaternion &q)
{
	m_x = q.m_x;
	m_y = q.m_y;
	m_z = q.m_z;
	m_w = q.m_w;
}

/**	Quaternion Constructor
 *
 *	@param a	The angle by which to rotate
 *	@param x	The x axis
 *	@param y	The y axis
 *	@param z	The z axis
 *
 *	Calls Quaternion::Rotate(float, float, float, float) to rotate the quaternion by the amounts passed in.  
 *	This is effectively a way to initialise a quat with an axis/angle rotation 
 */
Quaternion::Quaternion(float a, float x, float y, float z)
{
	Rotate(a,x,y,z);
}	

/**	Quaternion Deconstructor */
Quaternion::~Quaternion(){}

/**	Loads an identity quaternion
 *
 *	The identity quat is represented by x,y,z,w = 0,0,0,1
 */
void Quaternion::LoadIdentity(void)
{
	m_x = 0;
	m_y = 0;
	m_z = 0;
	m_w = 1;
}

/**	Rotates a quaternion 
 *
 *	@param a	The angle by which to rotate
 *	@param x	The x axis
 *	@param y	The y axis
 *	@param z	The z axis
 *
 *	This is an axis/angle rotation where you specify an axis and an angle to rotate, instead of euler rotations
 */
void Quaternion::Rotate(float a, float x, float y, float z)
{
	float dist, temp, sa;

	a = a * RADIANSDEGREE * 0.5;

	temp = x*x + y*y + z*z;

	// Normalize the axis.
	dist = 1.0 / sqrt(temp);

	x *= dist;
	y *= dist;
	z *= dist;

	sa = sin(a);

	// Build a quaternion!
	m_w = cos(a);
	m_x = x * sa;
	m_y = y * sa;
	m_z = z * sa;

	Normalise();
}

/**	Rotates a quaternion
 *
 *	@param x	The x angle to rotate
 *	@param y	The y angle to rotate
 *	@param z	The z angle to rotate
 *
 *	This is a euler type rotation, it builds three quaternions, each with a separate rotation in, then multiplies them together, producing a rotation
 */	
void Quaternion::Rotate(float x, float y, float z)
{
	Quaternion qx(x,1,0,0);
	Quaternion qy(y,0,1,0);
	Quaternion qz(z,0,0,1);

	*this =	(qx * qy * qz);
}

/**	Normalises the length of the quaternion */
void Quaternion::Normalise(void)
{
	float mag	= m_x*m_x + m_y*m_y + m_z*m_z + m_w*m_w;

	m_x /= mag;
	m_y /= mag;
	m_z /= mag;
	m_w /= mag;
}

/**	Retrieves a matrix for the quaternion
 *
 *	@returns An array of 16 floats, which represents a 4x4 matrix
 */
float * Quaternion::GetMatrix(void)
{
	Normalise();

	float xx = m_x * m_x;
	float yy = m_y * m_y;
	float zz = m_z * m_z;

	//	M(x,y) to access a matrix element (just makes reading a little easier)

	M( 0, 0 ) = 1.0f - 2.0f * ( yy + zz );
	M( 1, 0 ) = 2.0f * ( m_x * m_y + m_w * m_z );
	M( 2, 0 ) = 2.0f * ( m_x * m_z - m_w * m_y );
	M( 3, 0 ) = 0.0f;

	M( 0, 1 ) = 2.0f * ( m_x * m_y - m_w * m_z );
	M( 1, 1 ) = 1.0f - 2.0f * ( xx + zz );
	M( 2, 1 ) = 2.0f * ( m_y * m_z + m_w * m_x );
	M( 3, 1 ) = 0.0f;

	M( 0, 2 ) = 2.0f * ( m_x * m_z + m_w * m_y );
	M( 1, 2 ) = 2.0f * ( m_y * m_z - m_w * m_x );
	M( 2, 2 ) = 1.0f - 2.0f * ( xx + yy );
	M( 3, 2 ) = 0.0f;

	M( 0, 3 ) = 0.0f;
	M( 1, 3 ) = 0.0f;
	M( 2, 3 ) = 0.0f;
	M( 3, 3 ) = 1.0f;

	return m_matrix;
}

/**	Retrieves a Matrix from the quaternion
 *
 *	@param matrix	A pointer to an array of floats, representing the matrix
 */
void Quaternion::GetMatrix(float **matrix)
{
	float *m = GetMatrix();

	memcpy(*matrix,m,16*sizeof(float));
}

/**	Assigns this quaternion to the value of another
 *
 *	@param q	The quaternion to assign this quaternion to
 *
 *	@returns	This quaternion, modified by the assignment
 */
Quaternion & Quaternion::operator=(const Quaternion &q)
{
	m_x = q.m_x;
	m_y = q.m_y;
	m_z = q.m_z;
	m_w = q.m_w;

	return *this;
}

/**	Calculates the result of multiplying this quaternion against another
 *
 *	@param q	The quaternion to multiply this quaternion with
 *
 *	@returns	A Quaternion object, containing the result of the multiplication
 */
Quaternion Quaternion::operator* (const Quaternion &q) const
{
	Quaternion quat;

	quat.m_w	=		m_w * q.m_w
							- m_x * q.m_x
							- m_y * q.m_y
							- m_z * q.m_z;

	quat.m_x	=		m_w * q.m_x
							+ m_x * q.m_w
							+ m_y * q.m_z
							- m_z * q.m_y;

	quat.m_y	=		m_w * q.m_y
							- m_x * q.m_z
							+ m_y * q.m_w
							+ m_z * q.m_x;

	quat.m_z	=		m_w * q.m_z
							+ m_x * q.m_y
							- m_y * q.m_x
							+ m_z * q.m_w;

	return quat;	
}

/**	Multiplies this quaternion with another
 *
 *	@param q	The quaternion to multiply with
 *
 *	@returns	This quaternion, modified by the multiplication
 */
Quaternion & Quaternion::operator*= (Quaternion &q)
{
	Quaternion quat = *this;

	m_w	=		quat.m_w * q.m_w 
				- quat.m_x * q.m_x
				- quat.m_y * q.m_y
				- quat.m_z * q.m_z;

	m_x	=		quat.m_w * q.m_x
				+ quat.m_x * q.m_w
				+ quat.m_y * q.m_z
				- quat.m_z * q.m_y;

	m_y	=		quat.m_w * q.m_y
				- quat.m_x * q.m_z
				+ quat.m_y * q.m_w
				+ quat.m_z * q.m_x;

	m_z	=		quat.m_w * q.m_z
				+ quat.m_x * q.m_y
				- quat.m_y * q.m_x
				+ quat.m_z * q.m_w;

	return *this;
}
