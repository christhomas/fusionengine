#ifndef _VFSHANDLE_ZIP_H_
	#define _VFSHANDLE_ZIP_H_

#include <VFSHandle_file.h>
#include <zlib.h>

#define ZIP_SIG	0x4b50
#define LF_SIG	0x0403
#define FH_SIG	0x0201
#define CD_SIG	0x0605

#define FH_LEN	46

struct LocalFileHeader{
	unsigned short	version;
	unsigned short	general_flags;
	unsigned short	compression_method;
	unsigned short	time;
	unsigned short	date;
	unsigned int	crc32;
	unsigned int	comp_size;
	unsigned int	uncomp_size;
	unsigned short	filename_length;
	unsigned short	extrafield_length;
	char			*filename;
	char			*extrafield;
	int			file_data_offset;
};

struct FileHeader{
	unsigned short	version;
	unsigned short	version_extract;
	unsigned short	general_flags;
	unsigned short	compression_method;
	unsigned short	time;
	unsigned short	date;
	unsigned int	crc32;
	unsigned int	comp_size;
	unsigned int	uncomp_size;
	unsigned short	filename_length;
	unsigned short	extrafield_length;
	unsigned short	file_comment_length;
	unsigned short	disk_number_start;
	unsigned short	internal_file_attrib;
	unsigned int	external_file_attrib;
	unsigned int	relative_offset_lfh;
	char			*filename;
	char			*extrafield;
	char			*file_comment;
};

struct CentralDir{
	unsigned short	number_disk;
	unsigned short	number_disk_start;
	unsigned short	total_entries_disk;
	unsigned short	total_entries;
	unsigned int	size;
	unsigned int	offset;
	unsigned short	comment_length;
	char			*comment;
};

class VFSHandle_ZIP: public VFSHandle_file{
protected:
	char							*m_file;
	char							*m_archive;
	VFSHandle					*m_handle;
	int							m_fileid;
	z_stream						m_zipstream;
		
	CentralDir						m_cd;
	std::vector<LocalFileHeader *>	m_lfh;
	std::vector<FileHeader *>		m_fh;

	void CheckIOState(void);

//=========================================================
//=========================================================
//	protected Archive actions methods
//=========================================================
//=========================================================
	virtual bool			Open			(char *filename, bool create);
	virtual bool			OpenLocation		(char *loc, bool create);
	virtual bool			Close			(void);

//=========================================================
//=========================================================
//	protected Archive manipulation methods
//=========================================================
//=========================================================
	virtual void			FindArchiveName	(void);
	virtual void			FindFilename		(void);
	virtual void			DeleteCache		(void);
	virtual void			CreateCache		(void);
	virtual void			ReadLF			(void);
	virtual void			WriteLF			(LocalFileHeader *lfh);
	virtual void			ReadFH			(void);
	virtual void			WriteFH			(FileHeader *fh);
	virtual void			ReadCD			(void);
	virtual void			WriteCD			(void);
	virtual unsigned short	ReadSignature		(void);
public:
						VFSHandle_ZIP	(VFSTransport *t);
	virtual				~VFSHandle_ZIP	();

//=========================================================
//=========================================================
//	public Archive actions methods
//=========================================================
//=========================================================
	virtual FileInfo *		Read			(void);
	virtual unsigned char *	Read			(unsigned int &length);
	virtual void			Write			(FileInfo *data);
	virtual void			Write			(unsigned char *data, unsigned int length);

//=========================================================
//=========================================================
//	Archive Information methods
//=========================================================
//=========================================================
	virtual char *			Filename			(void);
	virtual unsigned int		Length			(void);
	virtual void			SetPlugin			(VFSPlugin *plugin);

//=========================================================
//=========================================================
//	Archive/Directory manipulation methods 
//=========================================================
//=========================================================
	virtual bool			IsFile			(char *filename);
	virtual bool			IsDirectory		(char *directory);
	
	virtual FileInfo *		GetFileInfo		(char *filename);

	virtual bool			Createfile			(char *filename, bool recurse=true);
	virtual bool			Deletefile			(char *filename);

	virtual bool			Copyfile			(char *src, char *dest, bool createpath);
	virtual bool			Movefile			(char *src, char *dest, bool createpath);

	virtual bool			CreateDir			(char *directory);
	virtual bool			DeleteDir			(char *directory, bool recurse);
};

/*	FOR STATIC LINKING */
VFSTransport * CreateTransportZIP(Fusion *f);

#endif // #ifndef _VFSHANDLE_ZIP_H_

