#ifndef _FMODMODULEBUFFER_H_
	#define _FMODMODULEBUFFER_H_

#include <fmod/fmod.h>
#include <Sound/ISoundBuffer.h>

/**	@ingroup	FMOD_Sound_Group
 *	@brief		Derived ISoundBuffer class for playing Tracker Modules
 */
class FMODModuleBuffer: public ISoundBuffer{
protected:
	/**	@var		FMUSIC_MODULE *m_module
	 *	@brief	FMOD handle to the tracker module
	 */
	FMUSIC_MODULE *m_module;
public:
					FMODModuleBuffer	();
	virtual			~FMODModuleBuffer	();
	virtual bool	Load				(std::string filename);
	virtual bool	Close				(void);
	virtual int		Play				(void);
	virtual bool	Pause				(bool pause);
	virtual bool	Stop				(void);
	virtual bool	SetPosition			(int position);
	virtual bool	Volume				(unsigned char volume);
	virtual bool	IsPlaying			(void);
};

#endif // #ifndef _FMODMODULEBUFFER_H_