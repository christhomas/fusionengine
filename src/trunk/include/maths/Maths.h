#ifndef _MATHS_H_
	#define _MATHS_H_

#define PI						3.141592654f		//	The value of PI
#define RADIANSDEGREE	0.01745329252f	//	The number of radians in ONE degree rotation

/**	@namespace	Maths
 *	@brief			Namespace to group all the Maths classes and constructs within
 */
namespace Maths{
	
	//	Templated class to reverse the order of bytes in a value (reverses the endian)
	template<class T>
	T byteswap(const T& t)
	{
			T out(0);
			for (unsigned int i=0; i < sizeof(T); ++i)
			{
					int rshft = i * 8;
					int lshft = (sizeof(T) - i - 1) * 8;
					out |= ( ((t >> rshft) & 0xFF) << (lshft));
			}
			return out;
	}
};	//	namespace Maths

#endif // #ifndef _MATHS_H_
