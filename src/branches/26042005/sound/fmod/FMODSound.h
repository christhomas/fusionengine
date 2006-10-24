#ifndef _FMODSOUND_H_
	#define _FMODSOUND_H_

#include <sound/ISound.h>
#include <FMODStreamBuffer.h>
#include <FMODSampleBuffer.h>
#include <FMODModuleBuffer.h>

#include <fmod/fmod.h>
#include <Fusion.h>

/** @ingroup	FMOD_Sound_Group
 *	@brief		Derived ISound class for the FMOD Platform
 */
class FMODSound: public ISound{
public:
													FMODSound			();
	virtual									~FMODSound		();
	virtual bool						Initialise		(void);
	virtual ISoundBuffer *	LoadStream		(char *filename);
	virtual ISoundBuffer *	LoadSample		(char *filename);
	virtual ISoundBuffer *	LoadModule		(char *filename);
	virtual void						RemoveStream	(unsigned int index);
	virtual void						RemoveSample	(unsigned int index);
	virtual void						RemoveModule	(unsigned int index);
};

#endif // #ifndef _FMODSOUND_H_