#ifndef _VFSFILTER_H_
	#define _VFSFILTER_H_

class Fusion;

/** @ingroup	VFSFilter_Group
 *	@brief		The base class for all VFS data Filters
 */
class VFSFilter{
protected:
	/** @var		char *m_type
	 *	@brief	String identifing the data filter
	 */
	char *m_type;
public:

	/**	@typedef filter_t
	 *	Function pointer to create a filter object
	 */
	typedef	VFSFilter *	(*filter_t)(Fusion *f);
	
	/**	Data Filter Constructor	*/
	VFSFilter(){};

	/**	Data Filter Deconstructor	*/
	virtual ~VFSFilter(){};

	/**	Encodes a bytestream
	 *
	 *	@param data		The bytestrem to encode
	 *	@param length	The length of the bytestream
	 *
	 *	@returns An encoded bytestream
	 */
	virtual unsigned char * Encode(unsigned char *data, unsigned int length) = 0;

	/**	Decodes a bytestream
	 *
	 *	@param data		The bytestream to decode
	 *	@param length	The length of the bytestream
	 *
	 *	@returns A Decoded bytestream
	 */
	virtual unsigned char * Decode(unsigned char *data, unsigned int length) = 0;

	/**	Retrieves the type of filter this is
	 *
	 *	@returns The identifing string of the filter
	 */
	virtual char * Type(void) = 0;
};

#endif // #ifndef _VFSFILTER_H_
