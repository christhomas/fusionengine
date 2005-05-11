#include <VirtualFS/VFSHandle_ZIP.h>

VFSTransport * CreateZIP(Fusion *f)
{
	return new VFSTransport("zip",VFSTransport::ARCHIVE,CreateZipHandle);	
}
