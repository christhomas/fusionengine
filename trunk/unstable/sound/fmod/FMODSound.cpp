#include <FMODSound.h>

/**	FMOD Constructor */
FMODSound::FMODSound()
{
	m_numbuffers = 0;
}

/**	FMOD Deconstructor
 *
 *	Remove all the streams/samples/tracker modules
 *
 *	Close the FMOD system
 */
FMODSound::~FMODSound()
{
	for(ISoundBuffer::soundbuffer_t::iterator stream=m_streams.begin();stream!=m_streams.end();stream++) delete *stream;
	for(ISoundBuffer::soundbuffer_t::iterator sample=m_samples.begin();sample!=m_samples.end();sample++) delete *sample;
	for(ISoundBuffer::soundbuffer_t::iterator module=m_modules.begin();module!=m_modules.end();module++) delete *module;

	m_streams.clear();
	m_samples.clear();
	m_modules.clear();

	FSOUND_Close();
}

/**	Initialise the FMOD Sound System
 *
 *	@returns	boolean true or false, but this method only returns true, crazy huh!
 *
 *	Operation:
 *		-#	Initialise the FMOD System
 *		-#	Retrieve a platform data structure
 *		-#	Set FMODs window handle (Win32 Implementation only)
 *		-#	return true
 */ 
bool FMODSound::Initialise(void)
{
	if(FSOUND_Init(44100, 32, 0) == false) return false;

#ifdef _WIN32
	Win32PlatformData *pd = reinterpret_cast<Win32PlatformData *>(fusion->GetPlatformData());
	if(FSOUND_SetHWND(pd->m_hwnd) == false)	return false;
#else
	//	Put a linux platform cast here or sommat...
#endif

	return true;
}

/**	Loads a sound stream
 *
 *	@returns	An ISoundBuffer object or NULL, if it failed to load
 *
 *	Operation:
 *		-#	Create a FMODStreamBuffer object
 *		-#	Load the stream
 *		-#	set the Fusion SoundBuffer ID
 */
ISoundBuffer * FMODSound::LoadStream(std::string filename)
{
	FMODStreamBuffer *buffer = new FMODStreamBuffer();

	if(buffer->Load(filename) == true){
		buffer->m_id = m_numbuffers++;
		m_streams.push_back(buffer);
		return buffer;
	}

	return NULL;
}

/**	Loads a sound sample
 *
 *	@returns	An ISoundBuffer object or NULL, if it failed to load
 *
 *	Operation:
 *		-#	Create a FMODSampleBuffer object
 *		-#	Load the sample
 *		-#	Set the Fusion SoundBuffer ID
 */
ISoundBuffer * FMODSound::LoadSample(std::string filename)
{
	FMODSampleBuffer *buffer = new FMODSampleBuffer();

	if(buffer->Load(filename) == true){
		buffer->m_id = m_numbuffers++;
		m_samples.push_back(buffer);
		return buffer;
	}

	return NULL;
}

/**	Loads a Tracker Module
 *
 *	@returns	An ISoundBuffer object or NULL, if it failed to load
 *
 *	Operation:
 *		-#	Create a FMODModuleBuffer object
 *		-#	Load the tracker module
 *		-#	Set the Fusion SoundBuffer ID
 */
ISoundBuffer * FMODSound::LoadModule(std::string filename)
{
	FMODModuleBuffer *buffer = new FMODModuleBuffer();

	if(buffer->Load(filename) == true){
		buffer->m_id = m_numbuffers++;
		m_modules.push_back(buffer);
		return buffer;
	}

	return NULL;
}

/**	Removes a stream
 *
 *	@param index	The SoundBuffer to remove
 *
 *	Operation:
 *		-#	Validate the index passed
 *		-#	If valid, delete the SoundBuffer
 *		-#	If deleted, remove the SoundBuffer from the Array of streams
 */
void FMODSound::RemoveStream(unsigned int index)
{
	if(index < m_streams.size()){
		m_numbuffers--;
		delete m_streams[index];
	}

	m_streams.erase(m_streams.begin()+index);
}

/**	Removes a Sample
 *
 *	@param index	The SoundBuffer to remove
 *
 *	Operation:
 *		-#	Validate the index passed
 *		-#	If valid, delete the SoundBuffer
 *		-#	If deleted, remove the SoundBuffer from the Array of samples
 */
void FMODSound::RemoveSample(unsigned int index)
{
	if(index < m_samples.size()){
		m_numbuffers--;
		delete m_samples[index];
	}

	m_samples.erase(m_samples.begin()+index);
}

/**	Removes a Tracker Module
 *
 *	@param index	The SoundBuffer to remove
 *
 *	Operation:
 *		-#	Validate the index passed
 *		-#	If valid, delete the SoundBuffer
 *		-#	If deleted, remove the SoundBuffer from the Array of tracker modules
 */
void FMODSound::RemoveModule(unsigned int index)
{
	if(index < m_modules.size()){
		m_numbuffers--;
		delete m_modules[index];
	}

	m_modules.erase(m_modules.begin()+index);
}
