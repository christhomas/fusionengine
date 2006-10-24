#include <VirtualFS/VFSHandle_file.h>
#include <fusion.h>
#include <sys/stat.h>

/**	Creates a Transport object that is associated with reading files from the local disk
 *	@ingroup	VFSHandle_file_Group
 *
 *	@returns	A Transport object setup to read/write files from the local disk
 */
VFSTransport * CreateFileTransport(Fusion *f)
{
	static int count = 0;

	if(count == 0){
		count++;
		return new VFSTransport("file://",VFSTransport::LOCALFS,CreateFileHandle);
	}

	return NULL;
}

/**	Creates a handle that can read files from the local disk
 *	@ingroup	VFSHandle_file_Group
 *
 *	@returns A Filehandle for reading/writing to the file
 */
VFSHandle * CreateFileHandle(VFSTransport *t)
{
	return new VFSHandle_file(t);
}

/**	Local file system Constructor
 *
 *	Resets all the handle data
 */
VFSHandle_file::VFSHandle_file(VFSTransport *t)
{
	m_filename	=	NULL;
	m_length		=	0;
	m_plugin		=	NULL;
	m_fileinfo	=	NULL;
	m_transport = t;
}

/**	Local file system Deconstructor
 *
 *	Calls VFSHandle_file::Close() to make sure the file is closed properly
 */
VFSHandle_file::~VFSHandle_file()
{
	Close();
}


//=========================================================
//=========================================================
//	protected File Information methods
//=========================================================
//=========================================================

/**	Sets the name of the file to open
 *
 *	@param filename	The name of the file being opened
 */
void VFSHandle_file::SetFilename(char *filename)
{
	delete[] m_filename;
	m_filename = NULL;

	if(filename != NULL){
		if(strncmp("file://",filename,strlen("file://")) == 0) filename+=strlen("file://");

		m_filename = new char[strlen(filename)+1];
		strcpy(m_filename,filename);
	}
}

//=========================================================
//=========================================================
//	protected File Actions methods
//=========================================================
//=========================================================

/**	Opens a file
 *
 *	@param filename	The name of the file to open
 *	@param create		Whether to create a non-existant file or return false
 *
 *	@returns boolean true or false, depending on whether was able to open
 *
 *	Operation:
 *		-#	The filename will be preceeded by "file://" so move the filename 
 *				pointer forward, past the vfs specific section of the file, and 
 *				to the actual filename itself e.g. file://image.jpg => image.jpg
 *		-#	Opens a filestream with read/write capabilities in binary mode
 *		-#	Sets the handles filename
 *		-#	Attempts to open the file
 *		-#	If successful, calculates the length of the file
 *		-#	returns whether the file was opened successfully
 *
 *	@todo	Add functionality to decide whether to create a new file, if the file requested does not exist
 */
bool VFSHandle_file::Open(char *filename, bool create)
{
	SetFilename(filename);

	if(IsFile(m_filename) == false && create == true)	Createfile(m_filename);

	if(IsFile(m_filename) == true){
		m_stream.clear();
		m_stream.open(m_filename, std::ios::in | std::ios::out | std::ios::binary);

		Length();

		return (bool)m_stream.is_open();
	}

	return false;
}

bool VFSHandle_file::OpenLocation(char *loc, bool create)
{
	SetFilename(loc);

	if(IsDirectory(m_filename) == false && IsFile(m_filename) == false && create == true) CreateDir(m_filename);

	return IsDirectory(m_filename);
}

/**	Closes a file
 *
 *	@returns boolean true or false, depending on whether the file was closed successfully or not
 *
 *	Deletes the memory allocated for the filename, closes the stream and returns whether the stream is open or not
 *	This method will return true to notify the filestream was closed successfully and false otherwise
 */
bool VFSHandle_file::Close(void)
{
	delete[] m_filename;
	
	m_filename	=	NULL;
	m_length		=	0;
	
	m_stream.close();
	m_stream.clear();

	return (bool)!(m_stream.is_open());
}

//=========================================================
//=========================================================
//	public File Actions methods
//=========================================================
//=========================================================

/**	Reads the file
 *
 *	@returns	A FileInfo structure containing the data read from the file, along with any helpful information
 *
 *	Operation:
 *		-#	Creates a bytestream to contain the filedata
 *		-#	Seeks to the start of the file and then reads it into the bytestream
 *		-#	Passes the bytestream to the plugin to decode
 *		-#	deletes the memory allocated in the original bytestream
 *		-#	Returns the FileInfo structure containing the decoded file data
 */
FileInfo * VFSHandle_file::Read(void)
{
	unsigned char *buffer = Read(m_length);
	
	FileInfo *info = m_plugin->Read(buffer,m_length);
	
	if(info != NULL) info->filename = m_filename;

	delete[] buffer;

	return info;
}

/**	Reads the raw data of a file
 *
 *	@param length	The length of the bytestream
 *
 *	@returns a bytestream containing the file contents, unmanipulated by the vfs
 *
 *	This method could be used by an application that may want the raw data 
 *	because it's an application specific data file, containing high scores, 
 *	user information, etc.
 */
unsigned char * VFSHandle_file::Read(unsigned int &length)
{
	unsigned char *buffer = new unsigned char[m_length];

	m_stream.seekg(0,std::ios::beg);
	m_stream.read((char *)buffer,m_length);

	length = m_length;

	return buffer;
}

/**	Writes to the file
 *
 *	@param	data	A FileInfo structure containing the data to write
 *
 *	Operation:
 *		-#	Asks the plugin to encode the information
 *		-#	passes this encoded data to be written to the file
 *		-#	Delete the encoded file data (it's been written, so it's safe to do so)
 */
void VFSHandle_file::Write(FileInfo *data)
{
	unsigned int	length	=	0;
	char					*bs			= m_plugin->Write(data,length);

	Write(bs,length);

	delete[] bs;
}

/**	Writes raw data (or preformatted data) to the file
 *
 *	@param	data		A Bytestream containing the file data to write
 *	@param	length	The length of the bytestream
 *
 *	Writes the raw data to the open filestream
 */
void VFSHandle_file::Write(char *data, unsigned int length)
{
	m_stream.write(data,length);

	if(m_length == 0) m_length = length;
}

//=========================================================
//=========================================================
//	public File Information methods
//=========================================================
//=========================================================

/**	Retrieves the name of the open file
 *
 *	@returns	A string containing the name of the file open
 */
char * VFSHandle_file::Filename(void)
{
	return m_filename;
}

/**	Retrieves the length of the file
 *
 *	@returns	A value indicating the length of the file
 */
unsigned int VFSHandle_file::Length(void)
{
	int cur,l;

	if(m_stream.is_open() == true){
		m_stream.seekg(0,std::ios::end);
		l = m_stream.tellg();

		if(l == -1){
			m_length	=	0;
			cur				=	0;
		}else{
			m_length	=	l;
		}

		m_stream.seekg(0,std::ios::beg);
	}

	return m_length;
}

/**	Sets this handles plugin
 *
 *	@param plugin	The plugin to use decoding the file when read/written
 */
void VFSHandle_file::SetPlugin(VFSPlugin *plugin)
{
	m_plugin = plugin;
}

//=========================================================
//=========================================================
//	public File/Directory manipulation methods
//=========================================================
//=========================================================

#ifdef _WIN32
	#include <io.h>
	#include <direct.h>
#endif

bool VFSHandle_file::IsFile(char *filename)
{
	_finddata_t find;
	
	long Filehandle = _findfirst(filename,&find);

	if(Filehandle == -1){
		if(errno == ENOENT){}
		if(errno == EINVAL){}

		_findclose(Filehandle);
		return false;
	}
	
	_findclose(Filehandle);

	return true;
}

bool VFSHandle_file::IsDirectory(char *directory)
{
	if(strcmp(directory,"") != 0){
		_finddata_t find;
		
		long Filehandle = _findfirst(directory,&find);

		if(Filehandle == -1){
			if(errno == ENOENT){}
			if(errno == EINVAL){}

			_findclose(Filehandle);
			return false;
		}
		
		_findclose(Filehandle);
	}
	
	return true;
}

//	dummy for the moment, since I dont actually need to extract this kind of information yet
FileInfo * VFSHandle_file::GetFileInfo(char *filename)
{
	struct stat s;
	
	if(stat(filename,&s) == -1){
		if(errno == ENOENT){};

		return NULL;
	}

	return NULL;
}

bool VFSHandle_file::Createfile(char *filename, bool recurse)
{
	//	TODO:	make sure the file is created, make all the directories required too
	if(recurse==true)	CreateDir(filename);

	m_stream.clear();
	m_stream.open(filename, std::ios::out);

	bool r = (bool)m_stream.is_open();

	m_stream.close();

	return r;
}

bool VFSHandle_file::Deletefile(char *filename)
{
	if(remove(filename) == -1){
		if(errno == EACCES) return false; // read only file
		if(errno == ENOENT)	return false;	// file does not exist
	}

	return true;
}

bool VFSHandle_file::Copyfile(char *src, char *dest, bool createpath)
{
	return false;
}

bool VFSHandle_file::Movefile(char *src, char *dest, bool createpath)
{
	return false;
}

bool VFSHandle_file::CreateDir(char *directory)
{
	char *dir = directory;

	for(int a=0;a<strlen(directory);a++) if(dir[a] == '\\') dir[a] = '/';

	if(dir[0] == '/') dir++;

	char *token = dir;

	while(token != NULL){
		token = strchr(token,'/');

		if(token != NULL){
			int bytes = token-directory;

			char *temp = new char[bytes+1];

			memset(temp,0,bytes+1);

			strncpy(temp,directory,bytes);

			if(mkdir(temp) == -1)	if(errno == ENOENT) return false;

			delete[] temp;

			token++;
		}
	}	

	return true;
}

bool VFSHandle_file::DeleteDir(char *directory, bool recurse)
{
	char path[256];

	sprintf(path,"%s/*.*",directory);

	if(recurse==true){
		_finddata_t find;
		
		long Filehandle = _findfirst(path,&find);

		if(Filehandle == -1){
			if(errno == ENOENT){
				_findclose(Filehandle);
				return DeleteDir(directory,false);
			}

			if(errno == EINVAL){
				_findclose(Filehandle);
				return false; // should this happen ever with *.* ? I dont think so....
			}
		}else{
			do{
				if(strcmp(find.name,".") != 0 && strcmp(find.name,"..") != 0){
					memset(path,0,256);
					sprintf(path,"%s/%s",directory,find.name);
					Deletefile(path);
				}
			}while(_findnext(Filehandle,&find) == 0);

			_findclose(Filehandle);
			return DeleteDir(directory,false);
		}

		//	have to delete all the files in the folder, then remove it, all the way back to the root directory requested to delete
		
		return false;
	}

	if(rmdir(directory) == -1){
		if(errno == ENOTEMPTY)	return false;
		if(errno == ENOENT)			return false;
	}

	return true;
}

//=========================================================
//=========================================================
//	public File Data Manipulation methods
//=========================================================
//=========================================================

unsigned char VFSHandle_file::ReadChar(void)
{
	unsigned char c;
	m_stream.read((char *)&c,sizeof(char));

	return c;
}

unsigned short VFSHandle_file::ReadShort(void)
{
	unsigned short s;
	m_stream.read((char *)&s,sizeof(short));

	return s;
}

unsigned int VFSHandle_file::ReadInt(void)
{
	int i;
	m_stream.read((char *)&i,sizeof(int));

	return i;
}

float VFSHandle_file::ReadFloat(void)
{
	float f;
	m_stream.read((char *)&f,sizeof(float));

	return f;
}

unsigned char * VFSHandle_file::ReadRaw(unsigned int length)
{
	unsigned char *string = new unsigned char[length];
	memset(string,0,length);
	m_stream.read((char *)string,length);

	return string;
}

char * VFSHandle_file::ReadString(unsigned int length)
{
	char *string = new char[length+1];
	memset(string,0,length+1);

	if(length > 0)	m_stream.read(string,length);

	return string;
}

void VFSHandle_file::WriteChar(unsigned char c)
{
	m_stream.write((char *)&c,sizeof(char));
}

void VFSHandle_file::WriteShort(unsigned short s)
{
	m_stream.write((char *)&s,sizeof(short));
}

void VFSHandle_file::WriteInt(unsigned int i)
{
	m_stream.write((char *)&i,sizeof(int));
}

void VFSHandle_file::WriteFloat(float f)
{
	m_stream.write((char *)&f,sizeof(float));
}

void VFSHandle_file::WriteRaw(unsigned char *s, unsigned int length)
{
	m_stream.write((char *)s,length);
}

void VFSHandle_file::WriteString(char *s, unsigned int length)
{
	WriteRaw((unsigned char *)s,length);
}
