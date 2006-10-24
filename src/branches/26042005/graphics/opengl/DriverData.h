#ifndef _DRIVERDATA_H_
	#define _DRIVERDATA_H_

#include <GL/gl.h>
#include <cstdlib>

/** @ingroup	OGL_Graphics_Group
 *	@brief		Reads the driver information
 *
 *	Responsible for reading all the driver information 
 *	about the OpenGL system, including the extensions
 *	available to the Graphics system
 */
class DriverData{
protected:
	/**	@var		char *ext_string
	 *	@brief	A String containing all the extensions
	 *
	 *	This is just used to store the string received
	 *	from the driver, it's chopped up into extensions
	 *	by the Tokenise method
	 */
	char *ext_string;

	/**	Tokenise the extension string received from the opengl driver
	 *
	 *	Chops up the string into tokens, 
	 *	each is an extension string
	 */
	inline void Tokenise(void)
	{
		char *token = strtok(ext_string," \0");
		
		do{
			if(token!=NULL)	extensions.push_back(token);

			token = strtok(NULL," \0");
		}while(token!=NULL);
	}
public:
	/**	@var		char *vendor
	 *	@brief	The vendor this driver belongs to
	 */
	char *vendor;
	
	/** @var		char *renderer
	 *	@brief	Identifier the renderer device 
	 */
	char *renderer;
	
	/** @var		char *version
	 *	@brief	The version of the driver being used
	 */
	char *version;
	/** @var		std::vector<char *> extensions
	 *	@brief	An array of extensions the driver supports
	 */
	std::vector<char *> extensions;

	/**	DriverData Constructor */
	DriverData	(){};

	/**	DriverData Deconstructor */
	~DriverData	(){};
	
	/**	Reads the driver information */
	inline void	Read (void)
	{
		//	Extract the information from the driver
		vendor			=	(char *)glGetString(GL_VENDOR);
		renderer		=	(char *)glGetString(GL_RENDERER);
		version			=	(char *)glGetString(GL_VERSION);
		ext_string	=	(char *)glGetString(GL_EXTENSIONS);

		Tokenise();
	}
};

#endif // #ifndef _DRIVERDATA_H_
