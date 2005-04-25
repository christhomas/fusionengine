#ifndef _VECTOR_H_
	#define _VECTOR_H_

//	FIXME:	This shouldnt be here, just use the float x,y,z constructor
//			and remove the Vertex3f constructor, just being lazy	
#include <mesh/Vertex.h>

namespace Maths{
	class	Matrix;

	/**	@ingroup	Maths_Helper_Group
	 *	@brief		Class to deal with 3D vectors and their manipulation
	 */
	class Vector{
	public:
		/**	@var		float x
		 *	@brief	The x value of the vector
		 */
		float x;

		/**	@var		float y
		 *	@brief	The y value of the vector
		 */
		float y;

		/**	@var		float z
		 *	@brief	The z value of the vector
		 */
		float z;

				Vector		();
				Vector		(float x,float y,float z);
				Vector		(const Vertex3f &v);
				Vector		(const Vector &v);
				~Vector		();
		void		Set			(float nx,float ny,float nz);
		float		Length		(void)			const;
		Vector	CrossProduct	(Vector &v)		const;
		float		DotProduct	(Vector &v)		const;
		
		//	Operator overloads
		bool		operator==	(const Vector &v)	const;
		bool		operator!=	(const Vector &v)	const;
		Vector	operator+	(const Vector &v)	const;
		Vector	operator+	(float add)		const;
		Vector	operator-		(const Vector &v)	const;
		Vector &	operator-		(void);
		Vector	operator*		(const Vector &v)	const;
		Vector	operator*		(float scalar)		const;
		Vector	operator/		(const Vector &v)	const;
		Vector	operator/		(float div)			const;
		Vector &	operator=		(const Vector &v);
		Vector &	operator+=	(const Vector &v);
		Vector &	operator+=	(float add);
		Vector &	operator*=	(const Vector &v);
		Vector &	operator*=	(float scalar);
		Vector &	operator/=	(const Vector &v);
		Vector &	operator/=	(float div);
		//	Matrix methods
		Vector	operator*		(Maths::Matrix &m)	const;
		Vector &	operator*=	(Maths::Matrix &m);
	};
};	//	namespace Maths

#endif // #ifndef _VECTOR_H_
