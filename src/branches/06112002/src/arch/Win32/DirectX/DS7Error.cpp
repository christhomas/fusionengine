// DSoundError.cpp: implementation of the DirectDrawErrorCodes class.
//
//////////////////////////////////////////////////////////////////////

#include <dsound.h>

char * DSoundError(HRESULT errorcode)
{
	switch(errorcode)
  {
		case DS_OK:
			return "The request completed successfully.";
		case DSERR_ACCESSDENIED:
			return "The request failed because access was denied.";
		case DSERR_ALLOCATED:
			return "The request failed because resources, such as a priority level, were already in use by another caller.";
		case DSERR_ALREADYINITIALIZED:
			return "The object is already initialized.";
		case DSERR_BADFORMAT:
			return "The specified wave format is not supported.";
		case DSERR_BUFFERLOST:
			return "The buffer memory has been lost and must be restored.";
		case DSERR_CONTROLUNAVAIL:
			return "The buffer control (volume, pan, and so on) requested by the caller is not available.";
		case DSERR_GENERIC:
			return "An undetermined error occurred inside the DirectSound subsystem.";
//	why isnt this defined within dsound.h??? it's in the help file??? weird as fuck...
//		case DSERR_HWUNAVAIL:
//			return "The DirectSound hardware device is unavailable.";
		case DSERR_INVALIDCALL:
			return "This function is not valid for the current state of this object.";
		case DSERR_INVALIDPARAM:
			return "An invalid parameter was passed to the returning function.";
		case DSERR_NOAGGREGATION:
			return "The object does not support aggregation.";
		case DSERR_NODRIVER:
			return "No sound driver is available for use.";
		case DSERR_NOINTERFACE:
			return "The requested COM interface is not available.";
		case DSERR_OTHERAPPHASPRIO:
			return "Another application has a higher priority level, preventing this call from succeeding ";
		case DSERR_OUTOFMEMORY:
			return "The DirectSound subsystem could not allocate sufficient memory to complete the caller's request.";
		case DSERR_PRIOLEVELNEEDED:
			return "The caller does not have the priority level required for the function to succeed. ";
		case DSERR_UNINITIALIZED:
			return "The IDirectSound::Initialize method has not been called or has not been called successfully before other methods were called.";
		case DSERR_UNSUPPORTED:
			return "The function called is not supported at this time.";
    default:
      return "Unrecognized error value.";
  }
}