#ifndef _QUATERNION_H_
	#define _QUATERNION_H_

#if defined (WIN32)
#pragma warning (disable:4244)	// disable conversion warnings (dbl -> fl)
#endif

namespace Maths{

	#define M(x, y)	m_matrix[ x + y * 4 ]	//	Accessing a matrix element

	/**	@ingroup	Maths_Helper_Group
	 *	@brief		Class to deal with Quaternions and their manipulation
	 */
	class Quaternion{
	protected:
		/**	@var	float m_matrix[16]
		 *	@brief	An array of floats representing a matrix for the quaternion
		 */
		float m_matrix[16];

		/** @var	float m_x
		 *	@brief	The X component of the quaternion */
		float m_x;
		
		/** @var	float m_y
		 *	@brief	The Y Component of the quaternion */
		float m_y;
		
		/** @var	float m_z
		 *	@brief	The Z Component of the quaternion */
		float m_z;
		
		/** @var	float m_w
		 *	@brief	The W component of the quaternion */
		float m_w;
	public:
						Quaternion		();
						Quaternion		(Quaternion &q);
						Quaternion		(float a, float x, float y, float z);
						~Quaternion		();
		
		void			LoadIdentity	(void);
		void			Rotate			(float a, float x, float y, float z);
		void			Rotate			(float x, float y, float z);
		void			Normalise		(void);
		float *			GetMatrix		(void);
		void			GetMatrix		(float **matrix);
		Quaternion &	operator =		(const Quaternion &q);
		Quaternion		operator *		(const Quaternion &q) const;
		Quaternion &	operator *=		(Quaternion &q);
	};
};	// namespace Maths

#endif // #ifndef _QUATERNION_H_
