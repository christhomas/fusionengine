#include <Maths/Vector.h>
#include <Maths/Matrix.h>
#include <cmath>

using namespace Maths;

/**	Vector Constructor
 *
 *	Set the vector to zero
 */
Vector::Vector()
{
	Set(0,0,0);
}

/**	Vector Constructor
 *
 *	@param	x	The X value of the vector
 *	@param	y	The Y value of the vector
 *	@param	z	The Z value of the vector
 */
Vector::Vector(float x,float y,float z)
{
	Set(x,y,z);
}

/**	Vector Constructor
 *
 *	@param v	A Vertex position to copy xyz values from
 */
Vector::Vector(Vertex3f &v)
{
	Set(v.x,v.y,v.z);
}

/**	Vector Constructor
 *
 *	Assigns the vector to copy the values of another vector
 */
Vector::Vector(Vector &v)
{
	*this = v;
}

/**	Vector Deconstructor */
Vector::~Vector(){}

/**	Sets the vector
 *
 *	@param nx	The new X value of the vector
 *	@param ny	The new Y value of the vector
 *	@param nz	The new Z value of the vector
 */
void Vector::Set(float nx,float ny,float nz)
{
	x = nx;
	y = ny;
	z = nz;		
}

/**	Calculates the length of the vector
 *
 *	@returns	The length of the vector
 */
float Vector::Length(void) const{
	return sqrt( (x*x)+(y*y)+(z*z) );
}

/**	Calculates the cross product of two vectors
 *
 *	@param v	The vector to cross product against
 *
 *	@returns	A Vector object containing the cross product
 */
Vector Vector::CrossProduct(Vector &v) const{
	Vector c;

	c.x	= (y * v.z)-(z * v.y);
	c.y	= (z * v.x)-(x * v.z);
	c.z	= (x * v.y)-(y * v.x);

	return c;
}

/**	Calculates the dot product of two vectors
 *
 *	@param v	The vector to use in the dot product calculation
 *
 *	@returns	The dot product value
 */
float Vector::DotProduct(Vector &v) const{
	return (x*v.x)+(y*v.y)+(z*v.z);
}

/**	Compares this vector against another
 *
 *	@param	v	The vector to compare against
 *
 *	@returns boolean true or false, true if vectors are identical
 */
bool Vector::operator==	(const Vector &v)	const{
	return (x==v.x && y==v.y && z==v.z);
}

/**	Tests whether this vector is NOT identical to another
 *
 *	@param v	The Vector to compare against
 *
 *	@returns boolean true or false, false if vectors are identical
 */
bool Vector::operator!=	(const Vector &v)	const{
	return !(*this==v);
}

/**	Calculates the result of adding this vector to another
 *
 *	@param v	The vector to add to this vector
 *
 *	@returns	A Vector object containing the result of the addition
 *
 *	NOTE: This method does not alter any of the vectors involved, it produces a separate vector with the result
 */
Vector Vector::operator+ (const Vector &v) const{
	Vector nv(x+v.x, y+v.y, z+v.z);
	return nv;
}

/**	Calcaulates the result of adding a value to all three axes
 *
 *	@param add	The value to add to all three axes
 *
 *	@returns	A Vector object containing the result of the addition
 *
 *	NOTE: This method does not alter any of the vectors involved, it produces a separate vector with the result
 */
Vector Vector::operator+ (float add) const{
	Vector nv( x+add, y+add, z+add );
	return nv;
}

/**	Calcaulates the result of subtracting a vector from this vector
 *
 *	@param v	The vector to subtract from this vector
 *
 *	@returns	A Vector object containing the result of the subtraction
 *
 *	NOTE: This method does not alter any of the vectors involved, it produces a separate vector with the result
 */
Vector Vector::operator-	(const Vector &v) const{
	Vector nv( x-v.x, y-v.y, z-v.z );
	return nv;
}

/**	Inverts a vector
 *
 *	@returns	This Vector inverted
 *
 *	This method will turn V(1,1,1) into V(-1,-1,-1) for example.
 */
Vector & Vector::operator-	(void){
	x=-x;
	y=-y;
	z=-z;
		
	return *this;
}

/**	Calculates the result of multiplying this vector with another
 *
 *	@param v	The vector to multiply against this vector
 *
 *	@returns	A Vector object containing the result of the multiplication
 *
 *	NOTE: This method does not alter any of the vectors involved, it produces a separate vector with the result
 */
Vector Vector::operator*	(const Vector &v) const{
	Vector nv( x*v.x, y*v.y, z*v.z );
	return nv;
}

/**	Calculates the result of multiplying this vector with a scalar
 *
 *	@param scalar	The scalar value to multiply this vector with
 *
 *	@returns	A Vector object containing the result of the multiplication
 *
 *	NOTE: This method does not alter any of the vectors involved, it produces a separate vector with the result
 */
Vector Vector::operator*	(float scalar) const{
	Vector nv( x*scalar, y*scalar, z*scalar );
	return nv;
}

/**	Calculates the result of dividing this vector with another
 *
 *	@param v	The vector to divide this vector with
 *
 *	@returns	A Vector object containing the result of the division
 *
 *	NOTE: This method does not alter any of the vectors involved, it produces a separate vector with the result
 */
Vector Vector::operator/	(const Vector &v) const{
	Vector nv( x/v.x, y/v.y, z/v.z );
	return nv;
}

/**	Calculates the result of dividing this vector with a scalar
 *
 *	@param div	The scalar to divide this vector with
 *
 *	@returns	A Vector object containing the result of the division
 *
 *	NOTE: This method does not alter any of the vectors involved, it produces a separate vector with the result
 */
Vector Vector::operator/	(float div) const{
	Vector nv( x/div, y/div, z/div );
	return nv;
}

/**	Assigns this vector to the value of another
 *
 *	@param v	The vector to set this vector to
 *
 *	@returns	This vector, set to the value of the one passed
 */
Vector & Vector::operator=	(const Vector &v){
	x = v.x;
	y = v.y;
	z = v.z;
	
	return *this;
}

/**	Adds this vector to another
 *
 *	@param v	The vector to add to this vector
 *
 *	@returns	This vector, modified by the addition
 */
Vector & Vector::operator+= (const Vector &v){
	x+=v.x;		
	y+=v.y;		
	z+=v.z;
			
	return *this;
}

/**	Adds a value to this vector
 *
 *	@param add	The value to add to this vector
 *
 *	@returns	This vector, modified by the addition
 */
Vector & Vector::operator+= (float add){
	x+=add;
	y+=add;
	z+=add;

	return *this;
}

/**	Multiplies this vector with another
 *
 *	@param v	The vector to multiply against
 *
 *	@returns	This vector, modified by the multiplication
 */
Vector & Vector::operator*= (const Vector &v){
	x*=v.x;		
	y*=v.y;		
	z*=v.z;
			
	return *this;
}

/**	Multiplies this vector with a scalar
 *
 *	@param scalar	The scale to multiply against
 *
 *	@returns	This vector, modified by the multiplication
 */
Vector & Vector::operator*= (float scalar){
	x*=scalar;
	y*=scalar;
	z*=scalar;

	return *this;
}

/**	Divides this vector with another
 *
 *	@param v	The vector to divide this vector against
 *
 *	@returns	This vector, modified by the division
 */
Vector & Vector::operator/= (const Vector &v){
	x/=v.x;
	y/=v.y;
	z/=v.z;

	return *this;
}

/**	Divides this vector with a scalar
 *
 *	@param div	The scalar to divide against
 *
 *	@returns	This vector, modified by the division
 */
Vector & Vector::operator/= (float div){
	x/=div;
	y/=div;
	z/=div;

	return *this;
}

/**	Calculates the result of multiplying this vector against a matrix
 *
 *	@param m	The matrix to multiply this vector with
 *
 *	@returns	A Vector object containing the result of the multiplication
 *
 *	NOTE: Look in most maths book for the technique of multiplying a matrix with a vector
 *
 *	NOTE: This method does not alter the vector or matrix involved, it produces a separate vector with the result
 */
Vector Vector::operator* (Matrix &m) const{
	Vector nv;

	float temp[4]	= { x, y, z, 1 };
	float e[4]		= {0};

	for(int a=0;a<4;a++){
		for(int b=0;b<4;b++){
			e[a] += m[(a*4)+b] * temp[b];
		}
	}
	
	nv.Set(e[0],e[1],e[2]);

	return nv;
}

/**	Multiplies this vector against a matrix
 *
 *	@param m	The matrix to multiply this vector with
 *
 *	@returns	This vector, modified by the multiplication
 *
 *	NOTE: Look in most maths book for the technique of multiplying a matrix with a vector
 */
Vector & Vector::operator*= (Matrix &m){
	float temp[4]	= { x, y, z, 1 };
	float e[4]		= {0};

	for(int a=0;a<4;a++){
		for(int b=0;b<4;b++){
			e[a] += m[(a*4)+b] * temp[b];
		}
	}

	Set(e[0],e[1],e[2]);

	return *this;
}
