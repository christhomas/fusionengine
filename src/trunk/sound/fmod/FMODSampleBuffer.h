#ifndef _FMODSAMPLEBUFFER_H_
	#define _FMODSAMPLEBUFFER_H_

#include <fmod/fmod.h>
#include <Sound/ISoundBuffer.h>

/** @ingroup	FMOD_Sound_Group
 *	@brief		Derived ISoundBuffer class for playing WAV Samples
 */
class FMODSampleBuffer: public ISoundBuffer{
protected:
	/**	@var		FSOUND_SAMPLE *m_sample
	 *	@brief	FMOD handle to the Sound sample
	 */
	FSOUND_SAMPLE	*m_sample;
public:
					FMODSampleBuffer	();
	virtual			~FMODSampleBuffer	();
	virtual bool	Load				(std::string filename);
	virtual bool	Close				(void);
	virtual int		Play				(void);
	virtual bool	Pause				(bool pause);
	virtual bool	Stop				(void);
	virtual bool	SetPosition			(int position);
	virtual bool	Volume				(unsigned char volume);
	virtual bool	IsPlaying			(void);
};

#endif // #ifndef _FMODSAMPLEBUFFER_H_