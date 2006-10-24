#ifndef _FUSIONSUBSYSTEM_H_
	#define _FUSIONSUBSYSTEM_H_
	
#include <string>

//	Predeclared Fusion class to avoid circular #include'ing
class Fusion;

/** @ingroup	Fusion_Group
 *	@brief		The base class for all Fusion subsystems
 */
class FusionSubsystem{
public:
	/**	@var		char *object_name
	 *	@brief	Name of the Subsystem
	 */
	std::string object_name;

	/**	@var		char *object_filename
	 *	@brief	Name of the DLL Module this object was created by
	 */
	std::string object_filename;

	/**	Fusion subsystems Constructor	*/
	FusionSubsystem(){};

	/**	Fusion subsystems Deconstructor	*/
	virtual	~FusionSubsystem(){};

	/**
	 *	Initialise
	 *
	 *	This method takes care of all the 
	 *	pre-start details of hooking the 
	 *	class into the Fusion Engine
	 */
	virtual bool Initialise() = 0;
};

#endif // #ifndef _FUSIONSUBSYSTEM_H_

