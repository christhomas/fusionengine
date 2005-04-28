#ifndef _ISOUND_H_
	#define _ISOUND_H_

#include <vector>
#include <FusionSubsystem.h>

#include <sound/ISoundBuffer.h>
#include <string>

/** @ingroup	Sound_Group
 *	@brief		The base class for all Sound subsystems
 */
class ISound: public FusionSubsystem{
protected:
	/**	@var		unsigned int m_numbuffers
	 *	@brief	The number of buffers currently available
	 */
	unsigned int		m_numbuffers;

	/**	@var		soundbuffers_t	m_streams
	 *	@brief	An array of Sound streams
	 */
	ISoundBuffer::soundbuffer_t	m_streams;
	
	/**	@var		soundbuffers_t	m_samples
	 *	@brief	An array of Sound samples
	 */
	ISoundBuffer::soundbuffer_t	m_samples;

	/**	@var		soundbuffers_t	m_modules
	 *	@brief	An array of Tracker Modules
	 */
	ISoundBuffer::soundbuffer_t	m_modules;
public:

	/**	ISound Constructor */
	ISound(){};

	/**	ISound Deconstructor */
	virtual	~ISound(){};

	/**	Initialise the sound system */
	virtual bool Initialise(void) = 0;

	/**	Load a sound stream */
	virtual ISoundBuffer * LoadStream(std::string filename) = 0;

	/**	Load a sound sample */
	virtual ISoundBuffer * LoadSample(std::string filename) = 0;

	/**	Load a Tracker Module */
	virtual ISoundBuffer * LoadModule(std::string filename) = 0;

	/**	Remove a sound stream */
	virtual void RemoveStream(unsigned int streamid) = 0;

	/**	Remove a sound sample */
	virtual void RemoveSample(unsigned int sampleid) = 0;

	/**	Remove a tracker module */
	virtual void RemoveModule(unsigned int moduleid) = 0;
};

#endif // #ifndef _ISOUND_H_

