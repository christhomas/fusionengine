#include <Sound/FMODModuleBuffer.h>

/**	FMOD ModuleBuffer Constructor */
FMODModuleBuffer::FMODModuleBuffer()
{
	m_module = NULL;
}

/**	FMOD ModuleBuffer Deconstructor 
 *
 *	Operation:
 *		-#	Stop the module playing
 *		-#	Close the module
 */
FMODModuleBuffer::~FMODModuleBuffer()
{
	Stop();
	Close();
}

/**	Load a Tracker Module
 *
 *	@param filename		The name of the tracker module file
 *
 *	@returns boolean true or false, depending on whether the module loaded or not
 */
bool FMODModuleBuffer::Load(char *filename)
{
	if((m_module = FMUSIC_LoadSong(filename)) != NULL)
	{
		return true;
	}else{
		return false;
	}
}

/**	Closes a Tracker Module
 *
 *	@returns boolean true or false, in this case, true only since you ignore any error condition
 */
bool FMODModuleBuffer::Close(void)
{
	FMUSIC_FreeSong(m_module);

	return true;
}

/**	Plays a Tracker Module
 *
 *	@returns	The channel id that the tracker module is playing on
 */
int FMODModuleBuffer::Play(void)
{
	m_channel = FMUSIC_PlaySong(m_module);

	return m_channel;
}

/**	Pauses a tracker module
 *
 *	@param	pause	Whether to pause the module or not
 *
 *	@returns	If the module paused, return true, otherwise false
 */
bool FMODModuleBuffer::Pause(bool pause)
{
	if(FMUSIC_SetPaused(m_module,pause) == 1) return true;

	return false;
}

/**	Stop the tracker module
 *
 *	@returns	boolean true or false, depending on whether the module stopped successfully or not
 */
bool FMODModuleBuffer::Stop(void)
{
	if(FMUSIC_StopSong(m_module) == 1) return true;

	return false;
}

/**	Sets the play position of the module
 *
 *	@param position	The position to set the module to start at
 *
 *	@returns	boolean true or false, depending on whether it was set successfully or not
 *
 *	NOTE: This method is not implemented yet, as Fusion has no need to set the position of a track as yet
 *				So it is impossible to know whether this method will set the start position in bytes or seconds, or minutes, or hours, or whatever
 */
bool FMODModuleBuffer::SetPosition(int position)
{
	return false;
}

/**	Sets the Volume of the module to play at
 *
 *	@param volume	The volume to set the tracker module to play at
 *
 *	@returns boolean true or false, depending on whether setting the volume was successful or not
 *
 *	The valid range of values for the volume is 0 to 255, silent, to max volume respectively
 */
bool FMODModuleBuffer::Volume(unsigned char volume)
{
	if(FSOUND_SetVolume(m_channel,volume) == 1) return true;

	return false;
}

/**	Tests whether the module is playing or not
 *
 *	@returns	boolean true or false, depending on whether the module is playing or not
 */
bool FMODModuleBuffer::IsPlaying(void)
{
	if(FMUSIC_IsPlaying(m_module) == 1) return true;

	return false;
}