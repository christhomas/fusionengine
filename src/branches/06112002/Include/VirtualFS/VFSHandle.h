#ifndef _VFSHANDLE_H_
	#define _VFSHANDLE_H_

#include <VirtualFS/VFSPlugin.h>

/**	@ingroup	VFSHandle_Group
 *	@brief		The base class for all VFS Handles
 */
class VFSHandle{
protected:
	friend class VFSTransport;

	/**	@var		char *m_filename
	 *	@brief	The filename the handle has open
	 */
	char *m_filename;

	/**	@var		unsigned int m_length
	 *	@brief	The length of the file
	 */
	unsigned int m_length;

	/**	@var		VFSPlugin *m_plugin
	 *	@brief	The plugin this handle will use to decode the file
	 */
	VFSPlugin *m_plugin;

	/**	@var		FileInfo *m_fileinfo
	 *	@brief	A structure containing the data read from the file
	 */
	FileInfo *m_fileinfo;

	/**	@var		VFSTransport *m_transport
	 *	@brief	The transport which created the file handle
	 */
	VFSTransport *m_transport;

	/**	Opens a file
	 *
	 *	@param filename	The filename to open
	 *	@param create		Whether to create a non-existant file or return false to say it wasnt opened
	 *
	 *	@returns boolean true or false, depending on whether the file was found/opened successfully
	 *
	 *	This method is protected so only the VFSTransport
	 *	can call it, because handles must not be allowed 
	 *	to open and close themselves, that would orphan 
	 *	a ptr in the transport object, causing problems
	 */
	virtual bool Open(char *filename, bool create=false) = 0;

	/**	Opens a location/directory/filesystem
	 *
	 *	@param filename	The location to open
	 *
	 *	This method can be used to open a location without opening a file
	 *	It's helpful with applications that read/write many files to a location
	 *	without having to specify the location of the files EVERY time
	 *
	 *	With network locations, it can open the server, send the username/password and store the directory chosen
	 *	then multiple files can be read/written without having to login/read/logout multiple times for each file
	 *
	 *	With archive locations, it can open the archive and perform caching of the archive contents
	 *	then like network locations, multiple files can be read/written without having to open/read/close 
	 *	the archive multiple times
	 *
	 *	With the local file system, it doesnt have much use, it's mainly of use if you want to create directories and files
	 *	but need a local filesystem handle to be able to do such a thing.  This is common to all transports, you could want 
	 *	a transport because you want to do file management, not because you want to read any file data, perhaps clean up temp
	 *	directories that were created previously.  That sort of thing.
	 */
	virtual bool OpenLocation(char *loc, bool create) = 0;

	/**	Close the file
	 *
	 *	@returns boolean true or false, depending on whether the file was open in order to close, or whether a fault occured whilst closing the file
	 *
	 *	This method is protected so only the VFSTransport
	 *	can call it, because handles must not be allowed 
	 *	to open and close themselves, that would orphan 
	 *	a ptr in the transport object, causing problems
	 */
	virtual bool Close(void) = 0;
public:
	/**	@typedef handle_t
	 *	Function pointer to create a handle of a specific type
	 */
	typedef VFSHandle * (*handle_t)(VFSTransport *t);

	/**	File Handle Constructor	*/
	VFSHandle(){}

	/**	File Handle Deconstructor	*/
	virtual ~VFSHandle(){}

//=========================================================
//=========================================================
//	File Actions (open/close/read/etc)
//=========================================================
//=========================================================
	/**	Reads the open file
	 *
	 *	@returns	A FileInfo structure containing the contents of the file
	 */
	virtual FileInfo * Read(void) = 0;

	/**	Reads an open files raw data
	 *
	 *	@param length	The length of the bytestream
	 *
	 *	@returns A bytestream containing the raw file data
	 */
	virtual unsigned char * Read(unsigned int &length) = 0;

	/**	Writes to the open file
	 *	
	 *	@param	data	A FileInfo structure containing the data to write to the file
	 */
	virtual void Write(FileInfo *data) = 0;

	/**	Writes raw data (or preformatted data) to the open file
	 *
	 *	@param data		A Bytestream containing the data to be written
	 *	@param length	The length of the bytestream
	 */
	virtual void Write(char *data, unsigned int length) = 0;
	

//=========================================================
//=========================================================
//	File Information (filename/length)
//=========================================================
//=========================================================
	/**	Sets the filename for the handle
	 *
	 *	@param filename	The name of the opened file
	 */
	virtual void SetFilename(char *filename) = 0;

	/**	Retrieves the length of the file
	 *
	 *	@returns	A Value indiciating the length of the file
	 */
	virtual unsigned int Length(void) = 0;

	/**	Retrieves the name of the open file
	 *
	 *	@returns A string containing the name of the file
	 */
	virtual char * Filename(void) = 0;

	/**	Sets the handles plugin
	 *
	 *	@param plugin	A Plugin to use when reading files with this file handle
	 */
	virtual void SetPlugin(VFSPlugin *plugin) = 0;

//=========================================================
//=========================================================
//	File/Directory manipulation
//=========================================================
//=========================================================
	/**	Tests whether the filename opened is a file
	 *
	 *	@returns	boolean true or false, depending on whether the filename was a valid file.
	 *						will return false, if the file does not exist
	 */
	virtual bool IsFile(char *filename) = 0;

	/**	Tests whether the filename opened is a directory
	 *
	 *	@returns	boolean true or false, depending on whether the filename was a valid directory.
	 *						will return false, if the directory does not exist
	 */
	virtual bool IsDirectory(char *directory) = 0;

	/**	Retrieves information about a file
	 *
	 *	@param filename	The name of the file to retrieve information about
	 */
	virtual FileInfo * GetFileInfo(char *filename) = 0;

	/**	Creates a file
	 *
	 *	@param filename	The name of the file to create
	 *	@param recurse	Whether to create the path the file resides in
	 *
	 *	@returns	boolean true or false, depending on whether creating the file was successful.
	 */
	virtual bool Createfile(char *filename, bool recurse=true) = 0;

	/**	Deletes a file
	 *
	 *	@param filename	The name of the file to delete
	 *
	 *	@returns	boolean true or false, depending on whether deletion was successful.
	 *						will return false if the file does not exist
	 */
	virtual bool Deletefile(char *filename) = 0;

	/**	Copies a file to a new location
	 *
	 *	@param src				The source path and filename to copy
	 *	@param dest				The destination path and filename to create
	 *	@param createpath	Whether it's allowed to create all the directories to store the file if necessary
	 *
	 *	@returns boolean true or false, depending on whether the copying was successful
	 */
	virtual bool Copyfile(char *src, char *dest, bool createpath=true) = 0;

	/**	Moves a file to a new location
	 *
	 *	@param src				The source path and filename to move
	 *	@param dest				The destination path and filename to create
	 *	@param createpath	Whether it's allowed to create all the directories to store the file if necessary
	 *
	 *	@returns boolean true of false, depending on whether the copying was successful
	 */
	virtual bool Movefile(char *src, char *dest, bool createpath=true) = 0;

	/**	Creates a directory
	 *
	 *	@param	directory		The name of the directory to create
	 *
	 *	@returns	boolean true or false, depending on whether creation of the direction succeeded
	 *						Will return false, if the directory already exists
	 */
	virtual bool CreateDir(char *directory) = 0;

	/**	Delete a directory
	 *
	 *	@param	directory		The name of the directory to delete
	 *	@param	recurse			Whether to recurse and delete all subdirectories + files
	 *
	 *	@returns	boolean true or false, depending on whether deletion was successful.
	 *						Will return false if the directory could not be found
	 */
	virtual bool DeleteDir(char *directory, bool recurse=false) = 0;

//=========================================================
//=========================================================
//	File Data Manipulation (reading/writing bytes)
//=========================================================
//=========================================================
	/**	Reads a byte
	 *
	 *	@returns A byte of data from the current file position
	 */
	virtual	unsigned char ReadChar(void) = 0;

	/**	Reads a short int (2 bytes/word)
	 *
	 *	@returns A word of data from the current file position
	 */
	virtual	unsigned short ReadShort(void) = 0;

	/**	Reads an integer (4 bytes/dword)
	 *
	 *	@returns A dword of data from the current file position
	 */
	virtual	unsigned int ReadInt(void) = 0;

	/**	Reads a floating point value (4 bytes/ float)
	 *
	 *	@returns A floating point value from the current file position
	 */
	virtual float ReadFloat(void) = 0;

	/**	Reads an entire string of data
	 *
	 *	@param length	The length of data to read
	 *
	 *	@returns	A block of data from the current file position
	 */
	virtual unsigned char *	ReadRaw(unsigned int length) = 0;

	/**	Reads a null terminated string
	 *
	 *	@param length	The length of the string to read
	 *
	 *	@returns	A null terminated string
	 */
	virtual char * ReadString(unsigned int length) = 0;

	/**	Writes a character
	 *
	 *	@param c	The character to write
	 */
	virtual void WriteChar(unsigned char c) = 0;

	/**	Writes a short int (word)
	 *
	 *	@param s	The short int to write
	 */
	virtual void WriteShort(unsigned short s) = 0;

	/**	Writes an integer (dword)
	 *
	 *	#param i	The integer to write
	 */
	virtual void WriteInt(unsigned int i) = 0;

	/**	Writes a floating point value
	 *
	 *	@param f	The floating point value to write
	 */
	virtual void WriteFloat(float f) = 0;

	/**	Writes an entire string of data
	 *
	 *	@param s			The chunk of data to write
	 *	@param length	The length of the chunk
	 */
	virtual void WriteRaw(unsigned char *s, unsigned int length) = 0;

	/**	Writes a null terminated string
	 *
	 *	@param s			The string
	 *	@param length	The length of the string (including null)
	 */
	virtual void WriteString(char *s, unsigned int length) = 0;
};

#endif // #ifndef _VFSHANDLE_H_
