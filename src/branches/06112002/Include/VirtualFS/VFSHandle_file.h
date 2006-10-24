#ifndef _VFSHANDLE_FILE_H_
	#define _VFSHANDLE_FILE_H_

#include <VirtualFS/VFSTransport.h>
#include <fstream>

#pragma warning(disable: 4800)

/**	@ingroup	VFSHandle_file_Group
 *	@brief		Allows reading files from the local disk
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
	virtual void						SetFilename			(char *filename);

//=========================================================
//=========================================================
//	protected File Actions methods
//=========================================================
//=========================================================
	virtual bool						Open						(char *filename, bool create);
	virtual bool						OpenLocation		(char *loc, bool create);
	virtual bool						Close						(void);
public:
													VFSHandle_file	(VFSTransport *t);
	virtual									~VFSHandle_file	();

//=========================================================
//=========================================================
//	public File Actions methods
//=========================================================
//=========================================================
	virtual FileInfo *			Read						(void);
	virtual unsigned char *	Read						(unsigned int &length);
	virtual void						Write						(FileInfo *data);
	virtual void						Write						(char *data, unsigned int length);

//=========================================================
//=========================================================
//	public File Information methods
//=========================================================
//=========================================================
	virtual char *					Filename				(void);
	virtual unsigned int		Length					(void);
	virtual void						SetPlugin				(VFSPlugin *plugin);

//=========================================================
//=========================================================
//	public File/Directory manipulation methods 
//=========================================================
//=========================================================
	virtual bool						IsFile					(char *filename);
	virtual bool						IsDirectory			(char *directory);

	virtual FileInfo *			GetFileInfo			(char *filename);

	virtual bool						Createfile			(char *filename, bool recurse=true);
	virtual bool						Deletefile			(char *filename);

	virtual bool						Copyfile				(char *src, char *dest, bool createpath);
	virtual bool						Movefile				(char *src, char *dest, bool createpath);

	virtual bool						CreateDir				(char *directory);
	virtual bool						DeleteDir				(char *directory, bool recurse);

//=========================================================
//=========================================================
//	public File Data Manipulation methods
//=========================================================
//=========================================================
	virtual	unsigned char		ReadChar				(void);
	virtual	unsigned short	ReadShort				(void);
	virtual	unsigned int		ReadInt					(void);
	virtual float						ReadFloat				(void);
	virtual unsigned char *	ReadRaw					(unsigned int length);
	virtual char *					ReadString			(unsigned int length);

	virtual void						WriteChar				(unsigned char c);
	virtual void						WriteShort			(unsigned short s);
	virtual void						WriteInt				(unsigned int i);
	virtual void						WriteFloat			(float f);
	virtual void						WriteRaw				(unsigned char *s, unsigned int length);
	virtual void						WriteString			(char *s, unsigned int length);
};

VFSTransport *	CreateFileTransport	(Fusion *f);
VFSHandle *			CreateFileHandle		(VFSTransport *t);

#endif // #ifndef _VFSHANDLE_FILE_H_