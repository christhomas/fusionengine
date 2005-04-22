#ifndef _FILEINFO_H_
	#define _FILEINFO_H_

/**	@ingroup	FileInfo_Group
 *	@brief		Base class for all FileInfo structures
 */
class FileInfo{
public:
	/**	FileInfo Destructor */
	virtual ~FileInfo(){}

	/**	@var		char *filename
	 *	@brief	The name of the file this FileInfo data represents
	 */
	char	*filename;

	/**	@var		int filelength
	 *	@brief	The length of the file
	 */
	int		filelength;
};

/**	@ingroup	FileInfo_Group
 *	@brief		Derived FileInfo class for all bytestreams
 *
 *	NOTE:	This class makes use of the FileInfo::filelength 
 *				member to represent the length of the bytestream, 
 *				which, co-incidentally, is identical to the length of the file
 */
class BinaryFileInfo: public FileInfo{
public:
	/**	BinaryFileInfo Destructor */
	virtual ~BinaryFileInfo(){}

	/**	@var		unsigned char *data
	 *	@brief	A Bytestream containing the file data
	 */
	unsigned char *data;
};

/**	@ingroup	FileInfo_Group
 *	@brief		Derived BinaryFileInfo class for simple text files
 *
 *	This class inherits the bytestream and acts as an array of pointers to where
 *	each line of the text file starts in the bytestream, then the length of each text line
 *	is represented by the length array member of this class
 */
class TextFileInfo: public BinaryFileInfo{
public:
	/**	TextFileInfo Destructor */
	virtual ~TextFileInfo(){}

	/**	@var		std::vector<char *> lines
	 *	@brief	An array of ptrs to within the bytestream
	 */
	std::vector<char *>	lines;

	/**	@var		std::vector<int> length
	 *	@brief	The length of each line
	 */
	std::vector<int>		length;
};

/**	@ingroup	FileInfo_Group
 *	@brief		Derived BinaryFileInfo class for Image files
 *
 *	This class stores the image data in the BinaryFileInfo's bytestream, 
 *	extending the class to store details about the image, it's dimensions, 
 *	bit depth, etc, etc.
 */
class ImageFileInfo: public BinaryFileInfo{
public:
	/**	ImageFileInfo Destructor */
	virtual ~ImageFileInfo(){}

	/**	@var		int width
	 *	@brief	The width of the image
	 */
	int		width;

	/**	@var		int height
	 *	@brief	The height of the image
	 */
	int		height;

	/**	@var		int bpp
	 *	@brief	The bit depth of the image
	 */
	int		bpp;

	/**	@var		unsigned char *palette
	 *	@brief	The palette to use when displaying the image
	 *
	 *	NOTE:	Only valid when ImageFileInfo::bpp is 8 (8-bit), otherwise will be NULL
	 */
	unsigned char	*palette;
};

class Mesh;
/**	@ingroup	FileInfo_Group
 *	@brief		Derived FileInfo class for 3D Mesh files
 */
class MeshFileInfo: public FileInfo{
public:
	/**	MeshFileInfo Destructor */
	virtual ~MeshFileInfo(){}
	
	/**	@var		Mesh *mesh
	 *	@brief	A Mesh class to store the mesh data in
	 */
	Mesh *mesh;
};

#endif // #ifndef _FILEINFO_H_
