#include <network/NetworkCore.h>

NetworkCore::NetworkCore(){}

NetworkCore::~NetworkCore(){}

unsigned int NetworkCore::ResolveHost(const char *ip)
{
    // for converting dns names to ip's
    char buffer[80];
    memset(buffer, 0, 80);
    hostent *host;

    // for storing the final ip address
    unsigned int address = -1;

    // convert the string to an ip
    address = inet_addr(ip);

    //      if the conversion didnt work, it's cause it has to be resolved first
    //      so this test will succeed if the address needs to be resolved
    if (address == -1) {
		// conver that hostname into a hostent structure
		host = gethostbyname(ip);

		if (host != NULL) {
			// extract the ip address from the hostent structure
			address = (*((in_addr *) host->h_addr)).s_addr;
		}
    }
    //      if the address was a valid ip string, it'll return a correct address
    //      if it's not, it'll attempt to resolve it, if that succeeds, it'll produce a valid ip address
    //      if both of those fail, it'll return -1, to signal error
    return address;
}
