#ifndef _ISOUNDBUFFER_H_
	#define _ISOUNDBUFFER_H_

#include <vector>

/** @ingroup	Sound_Group
 *	@brief		The base class for all Sound buffers
 */
class ISoundBuffer{
protected:
	/**	@var		unsigned int m_channel
	 *	@brief	The id of the sound channel
	 */
	unsigned int m_channel;
public:
	/**	@var		unsigned int m_id;
	 *	@brief	The id of the sound object within fusion
	 */
	unsigned int m_id;

	/**	@typedef	std::vector<ISoundBuffer *>	soundbuffer_t
	 *	@brief		An array of soundbuffers
	 */
	typedef std::vector<ISoundBuffer *> soundbuffer_t;

	/**	Soundbuffer base class Constructor */
	ISoundBuffer(){};

	/**	Soundbuffer base class Deconstructor */
	virtual ~ISoundBuffer(){};

	/**	Loads a sound source from a file */
	virtual bool Load(std::string filename) = 0;

	/**	Closes the sound object */
	virtual bool Close(void) = 0;

	/**	Plays the sound object */
	virtual int Play(void) = 0;

	/**	Pauses the sound object */
	virtual bool Pause(bool pause) = 0;

	/**	Stops the sound object */
	virtual bool Stop(void) = 0;

	/**	Sets the play position within the sound source */
	virtual bool SetPosition(int position) = 0;

	/**	Sets the volume of the sound source
	 *
	 *	@todo	Should I implement the volume as a percentage? map 0 -> 255 onto 0 -> 100 for example
	 */
	virtual bool Volume(unsigned char volume) = 0;

	/**	Requests the status of the sound object */
	virtual bool IsPlaying(void) = 0;
};

#endif // #ifndef _ISOUNDBUFFER_H_
