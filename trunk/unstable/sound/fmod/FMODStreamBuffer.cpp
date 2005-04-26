#include <FMODStreamBuffer.h>

//	This is the function to call when the stream ends (stops playing)
/**	Callback for streams where you cannot know the end of the stream, but when it ends, FMOD will call this method to tell the sound system
 *	@ingroup FMOD_Sound_Group
 *
 *	@param	@stream	The FMOD stream that called the callback
 *	@param	buff		Pointer to the stream data buffer to write to
 *	@param	len			Length of buffer specified in BYTES.
 *	@param	param		A user data value specified from FSOUND_Stream_Create
 *
 *	@return	TRUE, because fusion doesnt use the return value, true is as good as any value to return
 */
signed char StreamEndCallback(FSOUND_STREAM *stream, void *buff, int len, int param)
{
	FMODStreamBuffer *buffer = (FMODStreamBuffer *)&param;

	buffer->m_channel = 0;

	return 1;
}

/**	FMOD StreamBuffer Constructor */
FMODStreamBuffer::FMODStreamBuffer()
{
	m_stream = NULL;
}

/**	FMOD StreamBuffer Deconstructor
 *
 *	Operation:
 *		-#	Stops the stream playing
 *		-#	Closes the stream
 */
FMODStreamBuffer::~FMODStreamBuffer()
{
	Stop();
	Close();
}

/**	Loads a Sound stream
 *
 *	@param filename		The name of the stream file
 *
 *	@returns boolean true or false, depending on whether the stream loaded or not
 */
bool FMODStreamBuffer::Load(char *filename)
{
	m_stream = FSOUND_Stream_OpenFile(filename, FSOUND_NORMAL, 0);

	FSOUND_Stream_SetEndCallback(m_stream,StreamEndCallback,(int)this);

	if(m_stream != NULL) return true;

	return false;
}

/**	Closes the stream
 *
 *	@returns boolean true or false, in this case, true only since you ignore any error condition
 */
bool FMODStreamBuffer::Close(void)
{
	if(FSOUND_Stream_Close(m_stream) == 1) return true;

	return false;
}

/**	Plays the stream
 *
 *	@returns	The channel id that the stream is playing on
 */
int FMODStreamBuffer::Play(void)
{
	//	Play the buffer, return the channel playing on
	m_channel = FSOUND_Stream_Play(FSOUND_FREE,m_stream);
	return m_channel;
}

/**	Pauses a stream
 *
 *	@param	pause	Whether to pause the stream or not
 *
 *	@returns	If the stream paused, return true, otherwise false
 */
bool FMODStreamBuffer::Pause(bool pause)
{
	if(IsPlaying() == true && FSOUND_SetPaused(m_channel,pause) == 1) return true;

	return false;
}

/**	Stop the stream
 *
 *	@returns	boolean true or false, depending on whether the stream stopped successfully or not
 */
bool FMODStreamBuffer::Stop(void)
{
	if(FSOUND_Stream_Stop(m_stream) == 1) return true;

	return false;
}

/**	Sets the play position of the stream
 *
 *	@param position	The position to set the stream to start at in BYTES
 *
 *	@returns	boolean true or false, depending on whether it was set successfully or not
 */
bool FMODStreamBuffer::SetPosition(int position)
{
	if(FSOUND_Stream_SetPosition(m_stream,position) == 1) return true;

	return false;
}

/**	Sets the Volume of the stream to play at
 *
 *	@param volume	The volume to set the stream to play at
 *
 *	@returns boolean true or false, depending on whether setting the volume was successful or not
 *
 *	The valid range of values for the volume is 0 to 255, silent, to max volume respectively
 */
bool FMODStreamBuffer::Volume(unsigned char volume)
{
	if(FSOUND_SetVolume(m_channel,volume) == 1) return true;

	return false;
}

/**	Tests whether the stream is playing or not
 *
 *	@returns	boolean true or false, depending on whether the stream is playing or not
 */
bool FMODStreamBuffer::IsPlaying(void)
{
	if(m_channel!=0)	return true;
	else							return false;
}
