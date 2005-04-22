#ifndef _MATRIX_H_
	#define _MATRIX_H_

#if defined (WIN32)
#pragma warning (disable:4244)	// disable conversion warnings (dbl -> fl)
#endif

namespace Maths{
	class Quaternion;

	/**	@ingroup	Maths_Helper_Group
	 *	@brief		Class to deal with mathematical matrices and their manipulation
	 */
	class Matrix{
	private:
		/**	@var		float elements[16]
		 *	@brief	An array of elements representing the matrix
		 */
		float elements[16];
	public:
							Matrix				();
							Matrix				(Matrix &m);
							Matrix				(Quaternion &q);
							~Matrix				();
		float *		GetElements		(void);
		void			LoadIdentity	(void);
		void			Zero					(void);
		void			Stabilise			(void);
		
		Matrix &	operator=			(Matrix &m);
		Matrix &	operator=			(Quaternion &q);

		float &		operator[]		(unsigned int index);

		Matrix		operator*			(Matrix &m) const;
		Matrix &	operator*=		(Matrix &m);
	};
}; // namespace Maths

#endif // #ifndef _MATRIX_H_
