#ifndef _ITEXTURE_H_
	#define _ITEXTURE_H_

#include <string>

/** @ingroup	Graphics_Group
 *	@brief		The base class for all Texture Objects
 */
class ITexture{
protected:

	/** @var		unsigned int m_tbid
	 *	@brief	Each texture has an ID code assigned to it, for identification purposes
	 */
	int m_tbid;

	/**	@var		unsigned int m_width
	 *	@brief	The width of the texture object
	 */
	int m_width;

	/**	@var		unsigned int m_height
	 *	@brief	The height of the texture object
	 */
	int m_height;

	/**	@var		unsigned int m_numcomp
	 *	@brief	The number of components in each pixel
	 */
	int m_numcomp;

	/**	Sets the name of the file this texture was loaded from 
	 *
	 *	@param filename	The name of file containing the texture
	 */
	virtual void SetFilename(std::string filename) = 0;

public:
	/**	@var		char *m_filename
	 *	@brief	The filename from where this texture was loaded
	 *
	 *	Procedural textures will have PROCEDURAL as their filename, because they are not loaded from a file
	 *	@todo	Alter the filename in procedural textures like this: m_filename = "CreateFuzzyEffect256"
	 */
	std::string m_filename;

	/**	@var		textureproc_t
	 *	@brief	If the texture is procedural, this function ptr is the type of function that can generate a correct texture
	 */
	typedef unsigned char * (*textureproc_t)(unsigned char *buffer, int width, int height, int numcomp);

	/**	ITexture Constructor */
	ITexture(){};

	/** ITexture Deconstructor */
	virtual ~ITexture(){};

	/**	Loads the texture from it's source
	 *
	 *	@returns the TextureID for this texture
	 */
	virtual int	UpdateTexture(void) = 0;

	/**	Destroys the texture contents, if possible, and then Updates the texture again
	 *
	 *	@returns the TextureID for this texture
	 */
	virtual int Reload(void) = 0;

	/** Retrieves the width and height of the texture
	 *
	 *	@param x	The variable will contain the width of the texture
	 *	@param y	The variable will contain the height of the texture
	 */
	virtual void GetDimensions	(int &x, int &y) = 0;

	/**	Retrieves the width of the texture
	 *
	 *	@returns	The width of the texture
	 */
	virtual int	GetWidth(void) = 0;
	
	/**	Retrieves the height of the texture
	 *
	 *	@returns	The height of the texture
	 */
	virtual int	GetHeight(void) = 0;

	/**	Sets this texture object as the current rendering texture */
	virtual void Set(void) = 0;

	/**	Creates the Texture object */
	virtual void CreateTexture(unsigned char *buffer) = 0;

	/**	Deletes the Texture object */
	virtual void DeleteTexture(void) = 0;
};

#endif // #ifndef _ITEXTURE_H_
