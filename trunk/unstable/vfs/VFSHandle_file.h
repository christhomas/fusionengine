#ifndef _VFSHANDLE_FILE_H_
	#define _VFSHANDLE_FILE_H_

#include <vfs/VFSHandle.h>
#include <fstream>
#include <string>

#pragma warning(disable: 4800) //FIXME: what warning is this again?

class VFSTransport;

/**	@ingroup	VFSHandle_file_Group
 *	@brief	Allows reading files from the local disk
 */
class VFSHandle_file: public VFSHandle{
protected:
	/**	@var		std::fstream m_stream
	 *	@brief	The file handle
	 */
	std::fstream m_stream;

//=========================================================
//=========================================================
//	protected File Information methods
//=========================================================
//=========================================================
	virtual void				SetFilename		(std::string filename);

//=========================================================
//=========================================================
//	protected File Actions methods
//=========================================================
//=========================================================
	virtual bool				Open			(std::string filename, bool create);
	virtual bool				OpenLocation	(std::string loc, bool create);
	virtual bool				Close			(void);
public:
								VFSHandle_file	(VFSTransport *t);
	virtual						~VFSHandle_file	();
	
//=========================================================
//=========================================================
//	public File Actions methods
//=========================================================
//=========================================================
	virtual FileInfo *			Read			(void);
	virtual unsigned char *		Read			(unsigned int &length);
	virtual void				Write			(FileInfo *data);
	virtual void				Write			(char *data, unsigned int length);

//=========================================================
//=========================================================
//	public File Information methods
//=========================================================
//=========================================================
	virtual std::string			Filename		(void);
	virtual unsigned int		Length			(void);
	virtual void				SetPlugin		(VFSPlugin *plugin);

//=========================================================
//=========================================================
//	public File/Directory manipulation methods 
//=========================================================
//=========================================================
	virtual bool				IsFile			(std::string filename) = 0;
	virtual bool				IsDirectory		(std::string directory) = 0;

	virtual FileInfo *			GetFileInfo		(std::string filename) = 0;
	
	virtual bool				Createfile		(std::string filename, bool recurse=true) = 0;
	virtual bool				Deletefile		(std::string filename) = 0;

	virtual bool				Copyfile		(std::string src, std::string dest, bool createpath) = 0;
	virtual bool				Movefile		(std::string src, std::string dest, bool createpath) = 0;

	virtual bool				CreateDir		(std::string directory) = 0;
	virtual bool				DeleteDir		(std::string directory, bool recurse) = 0;

//=========================================================
//=========================================================
//	public File Data Manipulation methods
//=========================================================
//=========================================================
	virtual	unsigned char		ReadChar		(void);
	virtual	unsigned short		ReadShort		(void);
	virtual	unsigned int		ReadInt			(void);
	virtual	float				ReadFloat		(void);
	virtual	unsigned char *		ReadRaw			(unsigned int length);
	virtual	char *				ReadString		(unsigned int length);

	virtual	void				WriteChar		(unsigned char c);
	virtual	void				WriteShort		(unsigned short s);
	virtual	void				WriteInt		(unsigned int i);
	virtual	void				WriteFloat		(float f);
	virtual	void				WriteRaw		(unsigned char *s, unsigned int length);
	virtual	void				WriteString		(char *s, unsigned int length);
};

VFSTransport *	CreateFileTransport	(Fusion *f);
VFSHandle *		CreateFileHandle	(VFSTransport *t);

#endif // #ifndef _VFSHANDLE_FILE_H_

