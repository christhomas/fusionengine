#ifndef _FMODSTREAMBUFFER_H_
	#define _FMODSTREAMBUFFER_H_

#include <fmod/fmod.h>
#include <Sound/ISoundBuffer.h>

/** @ingroup	FMOD_Sound_Group
 *	@brief		Derived ISoundBuffer class for playing streamable files like mp3, etc
 */
class FMODStreamBuffer: public ISoundBuffer{
protected:
	/**	@var		FSOUND_STREAM *m_stream
	 *	@brief	FMOD handle to the Sound stream
	 */
	FSOUND_STREAM	*m_stream;
	
	//	To execute some code when the stream ends (in this case, zero out the channel)
	friend signed char StreamEndCallback(FSOUND_STREAM *stream, void *buff, int len, int param);
public:
								FMODStreamBuffer	();
	virtual				~FMODStreamBuffer	();
	virtual bool	Load							(char *filename);
	virtual bool	Close							(void);
	virtual int		Play							(void);
	virtual bool	Pause							(bool pause);
	virtual bool	Stop							(void);
	virtual bool	SetPosition				(int position);
	virtual bool	Volume						(unsigned char volume);
	virtual bool	IsPlaying					(void);
};

#endif // #ifndef _FMODSTREAMBUFFER_H_