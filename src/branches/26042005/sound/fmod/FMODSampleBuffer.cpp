#include <FMODSampleBuffer.h>

/** FMOD SampleBuffer Constructor */
FMODSampleBuffer::FMODSampleBuffer()
{
	m_sample = NULL;
}

/**	FMOD SampleBuffer Deconstructor
 *
 *	Operation:
 *		-#	Stop the sample playing
 *		-#	Close the sample
 */
FMODSampleBuffer::~FMODSampleBuffer()
{
	Stop();
	Close();
}

/**	Loads a Sound Sample
 *
 *	@param filename		The name of the sample file
 *
 *	@returns boolean true or false, depending on whether the sample loaded or not
 */
bool FMODSampleBuffer::Load(char *filename)
{
	if((m_sample = FSOUND_Sample_Load(FSOUND_FREE,filename, FSOUND_NORMAL, 0)) != NULL){
		return true;
	}else{
		return false;
	}
}

/**	Closes the Sample
 *
 *	@returns boolean true or false, in this case, true only since you ignore any error condition
 */
bool FMODSampleBuffer::Close(void)
{
	FSOUND_Sample_Free(m_sample);

	return true;
}

/**	Plays the Sample
 *
 *	@returns	The channel id that the Sample is playing on
 */
int FMODSampleBuffer::Play(void)
{
	return (m_channel = FSOUND_PlaySound(FSOUND_FREE,m_sample));
}

/**	Pauses a Sample
 *
 *	@param	pause	Whether to pause the sample or not
 *
 *	@returns	If the sample paused, return true, otherwise false
 */
bool FMODSampleBuffer::Pause(bool pause)
{
	if(FSOUND_SetPaused(m_channel,pause) == 1) return true;

	return false;
}

/**	Stop the Sample
 *
 *	@returns	boolean true or false, depending on whether the sample stopped successfully or not
 */
bool FMODSampleBuffer::Stop(void)
{
	if(FSOUND_StopSound(m_channel) == 1) return true;

	return false;
}

/**	Sets the play position of the sample
 *
 *	@param position	The position to set the sample to start at
 *
 *	@returns	boolean true or false, depending on whether it was set successfully or not
 *
 *	NOTE: This method is not implemented yet, as Fusion has no need to set the position of a track as yet
 *				So it is impossible to know whether this method will set the start position in bytes or seconds, or minutes, or hours, or whatever
 */
bool FMODSampleBuffer::SetPosition(int position)
{
	return false;
}

/**	Sets the Volume of the sample to play at
 *
 *	@param volume	The volume to set the sample to play at
 *
 *	@returns boolean true or false, depending on whether setting the volume was successful or not
 *
 *	The valid range of values for the volume is 0 to 255, silent, to max volume respectively
 */
bool FMODSampleBuffer::Volume(unsigned char volume)
{
	if(FSOUND_SetVolume(m_channel,volume) == 1) return true;

	return false;
}

/**	Tests whether the sample is playing or not
 *
 *	@returns	boolean true or false, depending on whether the sample is playing or not
 */
bool FMODSampleBuffer::IsPlaying(void)
{
	if(m_channel != 0 && FSOUND_IsPlaying(m_channel) == 1){
		if(FSOUND_GetCurrentSample(m_channel) == m_sample) return true;
	}

	return false;
}