#include <maths/Matrix.h>
#include <maths/Quaternion.h>
#include <maths/Vector.h>
#include <memory.h>

using namespace Maths;

/**	Matrix Constructor
 *
 *	Zero's the matrix
 */
Matrix::Matrix()
{
	Zero();			
}


/**	Matrix Constructor
 *
 *	Assigns the matrix to another
 */
Matrix::Matrix(Matrix &m)
{
	*this = m;
}

/**	Matrix Constructor
 *
 *	Assigns the matrix to a Quaternion's matrix
 */
Matrix::Matrix(Quaternion &q)
{
	*this = q;
}

/**	Matrix Deconstructor */
Matrix::~Matrix(){}

/**	Retrieves a pointer to the matrix elements
 *
 *	@returns	A pointer to the matrix elements
 *
 *	NOTE: This method is implemented OVER operation[] because of the performance cost operator[] incurs
 */
float * Matrix::GetElements(void){
	return elements;
}

/**	Sets an idenitity matrix */
void Matrix::LoadIdentity(void){
	Zero();
	elements[0]		=	1;
	elements[5]		=	1;
	elements[10]	=	1;
	elements[15]	=	1;
}

/**	Zero's the matrix */
void Matrix::Zero(void){
	memset(elements,0,sizeof(float)*16);
}

/**	Stabilises a matrix
 *
 *	This method makes sure all three axes are normalised and not screwed up
 *
 *	Operation:
 *		-#	Makes vectors of each axis
 *		-#	Calcaulates the lengths of each axis
 *		-#	Assigns axis(x) to the cross(z,y)
 *		-#	Assigns axis(y) to the cross(x,z)
 *		-#	Assigns axis(z) to the cross(x,y)
 */
void Matrix::Stabilise(void){
	Vector cross;

	Vector x(elements[0],elements[1],elements[2]);
	Vector y(elements[4],elements[5],elements[6]);
	Vector z(elements[8],elements[9],elements[10]);

	float xlen = x.Length();
	float ylen = y.Length();
	float zlen = z.Length();

	//	Cross the vectors and normalise them
	cross = z.CrossProduct(y);
	elements[0]		= cross.x / xlen;
	elements[1]		= cross.y / xlen;
	elements[2]		= cross.z / xlen;

	cross = x.CrossProduct(z);
	elements[4]		= cross.x / ylen;
	elements[5]		= cross.y / ylen;
	elements[6]		= cross.z / ylen;

	cross = x.CrossProduct(y);
	elements[8]		= cross.x / zlen;
	elements[9]		= cross.y / zlen;
	elements[10]	= cross.z / zlen;
}

/**	Assigns this matrix to the value of another
 *
 *	@param m	The matrix to assign this matrix to
 *
 *	@returns	This matrix, modified by the assignment
 */
Matrix & Matrix::operator= (Matrix &m){
	float *matrix = m.GetElements();
	
	for(int a=0;a<16;a++) elements[a] = matrix[a];

	return *this;
}

/**	Assigns this matrix to the value of a quaternions matrix
 *
 *	@param q	The quaternion to assign the matrix to
 *
 *	@returns	This matrix, modified by the assignment
 *
 *	Operation:
 *		-#	Retreive the quaternions matrix
 *		-#	Assign the elements to this matrix
 *		-#	Return this matrix
 */
Matrix & Matrix::operator= (Quaternion &q){
	float *m = q.GetMatrix();

	for(int a=0;a<16;a++) elements[a] = m[a];

	return *this;
}

/**	A method of accessing the matrix elements
 *
 *	@param index	The index requested
 *
 *	@returns	A floating point value for the element index requested
 *
 *	WARNING:	This method is REALLY slow compared to retrieving a element pointer and finding it yourself
 */
float & Matrix::operator[] (unsigned int index){
	return elements[index];
}

/**	Calculates the result of multiplying this matrix against another
 *
 *	@param m	The matrix to multiply this matrix with
 *
 *	@returns	A Matrix object, containing the result of the multiplication
 *
 *	NOTE: This method does not alter any of the matrices involved, it produces a separate matrix with the result
 */
Matrix Matrix::operator*	(Matrix &m) const{
	Matrix nm;

	for(int a=0;a<16;a+=4){
		for(int b=0;b<4;b++){
			for(int c=0;c<4;c++){
				nm[a+b] += elements[a+c] * m[(c*4)+b];
			}
		}
	}

	return nm;
}

/**	Multiplies this matrix with another
 *
 *	@param m	The matrix to multiply with
 *
 *	@returns	This matrix, modified by the multiplication
 */
Matrix &	Matrix::operator*=	(Matrix &m){
	float temp[16] = {0};

	for(int a=0;a<16;a+=4){
		for(int b=0;b<4;b++){
			for(int c=0;c<4;c++){
				temp[a+b] += elements[a+c] * m[(c*4)+b];
			}
		}
	}

	memcpy(elements,temp,sizeof(float)*16);

	return *this;
}
