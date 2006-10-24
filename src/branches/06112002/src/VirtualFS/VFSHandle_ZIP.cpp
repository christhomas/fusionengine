#include <VirtualFS/VFSHandle_ZIP.h>
#include <fusion.h>
#include <time.h>

#define NO_COMPRESSION			0
#define DEFLATE_COMPRESSION	8

/***************************************
	Start of Debugging code
***************************************/
#include <fstream>
std::ofstream debug("zlib.txt");

void VFSHandle_ZIP::CheckIOState(void)
{
	debug << std::endl << "************************************" << std::endl;

	if(m_stream.is_open() == false){
		debug << "file is no longer open" << std::endl;
	}else{
		debug << "file is open" << std::endl;
	}

	if(m_stream.good() != 0){
		debug << "good was ok" << std::endl;
	}

	if(m_stream.bad() != 0){
		debug << "bad returned > 0, serious IO error" << std::endl;
	}else{
		debug << "bad returned no error" << std::endl;
	}

	if(m_stream.fail() != 0){
		debug << "fail returned > 0, possibly recoverable" << std::endl;
	}else{
		debug << "fail returned no error" << std::endl;
	}

	int state = m_stream.rdstate();

	if(state & std::ios::goodbit)	debug << "No error condition" << std::endl;
	if(state & std::ios::eofbit)	debug << "end of file reached" << std::endl;
	if(state & std::ios::failbit)	debug << "A possibly recoverable formatting or conversion error" << std::endl;
	if(state & std::ios::badbit)	debug << "A severe IO error or unknown state" << std::endl;

	debug << "************************************" << std::endl << std::endl;

	m_stream.clear();
}

#define DEBUG_READLF \
	debug << "Reading LFH = {" << std::endl; \
	debug << "\t version = " << lfh->version << std::endl; \
	debug << "\t general flags = " << lfh->general_flags << std::endl; \
	debug << "\t compression method = " << lfh->compression_method << std::endl; \
	debug << "\t time = " << lfh->time << std::endl; \
	debug << "\t date = " << lfh->date << std::endl; \
	debug << "\t crc32 = " << lfh->crc32 << std::endl; \
	debug << "\t compressed size = " << lfh->comp_size << std::endl; \
	debug << "\t uncompressed size = " << lfh->uncomp_size << std::endl; \
	debug << "\t filename length = " << lfh->filename_length << std::endl; \
	debug << "\t extrafield length = " << lfh->extrafield_length << std::endl; \
	debug << "\t filename = " << lfh->filename << std::endl; \
	debug << "\t extrafield = " << lfh->extrafield << std::endl; \
	debug << "\t file data offset = " << lfh->file_data_offset << std::endl; \
	debug << "};" << std::endl;

#define DEBUG_WRITELF \
	debug << "Writing LFH = {" << std::endl; \
	debug << "\t version = " << lfh->version << std::endl; \
	debug << "\t general flags = " << lfh->general_flags << std::endl; \
	debug << "\t compression method = " << lfh->compression_method << std::endl; \
	debug << "\t time = " << lfh->time << std::endl; \
	debug << "\t date = " << lfh->date << std::endl; \
	debug << "\t crc32 = " << lfh->crc32 << std::endl; \
	debug << "\t compressed size = " << lfh->comp_size << std::endl; \
	debug << "\t uncompressed size = " << lfh->uncomp_size << std::endl; \
	debug << "\t filename length = " << lfh->filename_length << std::endl; \
	debug << "\t extrafield length = " << lfh->extrafield_length << std::endl; \
	debug << "\t filename = " << lfh->filename << std::endl; \
	debug << "\t extrafield = " << lfh->extrafield << std::endl; \
	debug << "\t file data offset = " << lfh->file_data_offset << std::endl; \
	debug << "};" << std::endl;

#define DEBUG_READFH \
	debug << "Reading FH = {" << std::endl; \
	debug << "\t version = " << fh->version << std::endl; \
	debug << "\t version_extract = " << fh->version_extract << std::endl; \
	debug << "\t general_flags = " << fh->general_flags << std::endl; \
	debug << "\t compression_method = " << fh->compression_method << std::endl; \
	debug << "\t time = " << fh->time << std::endl; \
	debug << "\t date = " << fh->date << std::endl; \
	debug << "\t crc32 = " << fh->crc32 << std::endl; \
	debug << "\t comp_size = " << fh->comp_size << std::endl; \
	debug << "\t uncomp_size = " << fh->uncomp_size << std::endl; \
	debug << "\t filename_length = " << fh->filename_length << std::endl; \
	debug << "\t extrafield_length = " << fh->extrafield_length << std::endl; \
	debug << "\t file_comment_length = " << fh->file_comment_length << std::endl; \
	debug << "\t disk_number_start = " << fh->disk_number_start << std::endl; \
	debug << "\t internal_file_attrib = " << fh->internal_file_attrib << std::endl; \
	debug << "\t external_file_attrib = " << fh->external_file_attrib << std::endl; \
	debug << "\t relative_offset_lfh = " << fh->relative_offset_lfh << std::endl; \
	debug << "\t filename = " << fh->filename << std::endl; \
	debug << "\t extrafield = " << fh->extrafield << std::endl; \
	debug << "\t file_comment = " << fh->file_comment << std::endl; \
	debug << "};" << std::endl;

#define DEBUG_WRITEFH \
	CheckIOState(); \
	debug << "Writing FH = {" << std::endl; \
	debug << "\t version = " << fh->version << std::endl; \
	debug << "\t version_extract = " << fh->version_extract << std::endl; \
	debug << "\t general_flags = " << fh->general_flags << std::endl; \
	debug << "\t compression_method = " << fh->compression_method << std::endl; \
	debug << "\t time = " << fh->time << std::endl; \
	debug << "\t date = " << fh->date << std::endl; \
	debug << "\t crc32 = " << fh->crc32 << std::endl; \
	debug << "\t comp_size = " << fh->comp_size << std::endl; \
	debug << "\t uncomp_size = " << fh->uncomp_size << std::endl; \
	debug << "\t filename_length = " << fh->filename_length << std::endl; \
	debug << "\t extrafield_length = " << fh->extrafield_length << std::endl; \
	debug << "\t file_comment_length = " << fh->file_comment_length << std::endl; \
	debug << "\t disk_number_start = " << fh->disk_number_start << std::endl; \
	debug << "\t internal_file_attrib = " << fh->internal_file_attrib << std::endl; \
	debug << "\t external_file_attrib = " << fh->external_file_attrib << std::endl; \
	debug << "\t relative_offset_lfh = " << fh->relative_offset_lfh << std::endl; \
	debug << "\t filename = " << fh->filename << std::endl; \
	debug << "\t extrafield = " << fh->extrafield << std::endl; \
	debug << "\t file_comment = " << fh->file_comment << std::endl; \
	debug << "};" << std::endl;

#define DEBUG_READCD \
	debug << "Reading CD = {" << std::endl; \
	debug << "\t number disk = " << m_cd.number_disk << std::endl; \
	debug << "\t number disk start = " << m_cd.number_disk_start << std::endl; \
	debug << "\t total entries disk = " << m_cd.total_entries_disk << std::endl; \
	debug << "\t total entries = " << m_cd.total_entries << std::endl; \
	debug << "\t size = " << m_cd.size << std::endl; \
	debug << "\t offset = " << m_cd.offset << std::endl; \
	debug << "\t comment length = " << m_cd.comment_length << std::endl; \
	debug << "\t comment = " << m_cd.comment << std::endl; \
	debug << "};" << std::endl;

#define DEBUG_WRITECD \
	debug << "Writing CD = {" << std::endl; \
	debug << "\t number disk = " << m_cd.number_disk << std::endl; \
	debug << "\t number disk start = " << m_cd.number_disk_start << std::endl; \
	debug << "\t total entries disk = " << m_cd.total_entries_disk << std::endl; \
	debug << "\t total entries = " << m_cd.total_entries << std::endl; \
	debug << "\t size = " << m_cd.size << std::endl; \
	debug << "\t offset = " << m_cd.offset << std::endl; \
	debug << "\t comment length = " << m_cd.comment_length << std::endl; \
	debug << "\t comment = " << m_cd.comment << std::endl; \
	debug << "};" << std::endl;

/***************************************
	End of Debugging code
***************************************/

VFSHandle * CreateZipHandle(VFSTransport *t)
{
	return new VFSHandle_ZIP(t);
}

VFSHandle_ZIP::VFSHandle_ZIP(VFSTransport *t): VFSHandle_file(t)
{
	m_filename	=	NULL;
	m_length		=	0;
	m_plugin		=	NULL;
	m_fileinfo	=	NULL;
	m_transport = t;

	m_file			=	NULL;
	m_archive		=	NULL;
	m_handle		=	NULL;
	m_fileid		=	0;

	m_lfh.clear();
	m_fh.clear();

	memset(&m_cd,0,sizeof(CentralDir));

	m_cd.comment = "this is a cd comment";
	m_cd.comment_length = strlen(m_cd.comment);
}

VFSHandle_ZIP::~VFSHandle_ZIP()
{
	Close();
}

//=========================================================
//=========================================================
//	protected Archive actions methods
//=========================================================
//=========================================================

bool VFSHandle_ZIP::Open(char *filename, bool create)
{
	OpenLocation(filename,create);

	FindFilename();

	if(IsFile(m_file) == false){
		if(create == true){
			Createfile(m_file);
		}else{
			return false;
		}
	}

	Length();
	return true;
}

bool VFSHandle_ZIP::OpenLocation(char *loc, bool create)
{
	SetFilename(loc);

	FindArchiveName();

	if(VFSHandle_file::IsFile(m_archive) == false){
		if(create == true){
			VFSHandle_file::Createfile(m_archive);
		}else{
			return false;
		}
	}

	m_stream.clear();
	m_stream.open(m_archive, std::ios::in | std::ios::out | std::ios::binary);

	CreateCache();

	return true;
}

bool VFSHandle_ZIP::Close(void)
{
	m_cd.offset = m_stream.tellg();

	for(int a=0;a<m_fh.size();a++)	WriteFH(m_fh[a]);
	WriteCD();

	bool r1 = VFSHandle_file::Close();

	bool r2 = fusion->vfs->Close(m_handle);

	DeleteCache();

	delete[] m_file;
	delete[] m_archive;

	m_handle	= NULL;
	m_fileid	=	0;

	if(r1 == true && r2 == true) return true;

	return false;
}

unsigned short VFSHandle_ZIP::ReadSignature(void)
{
	return ReadShort();
}

void VFSHandle_ZIP::FindArchiveName(void)
{
	char *hash	= strchr(m_filename,'#');
	char *fn		= m_filename;

	int l;

	if(hash != NULL){
		l = hash-fn;
	}else{
		l = strlen(fn);
	}

	delete[] m_archive;
	m_archive = new char[l+1];
	memset(m_archive,0,l+1);

	strncpy(m_archive, fn, l);
}

void VFSHandle_ZIP::FindFilename(void)
{
	char *hash = strchr(m_filename, '#');
	
	if(hash != NULL){
		hash++;

		if(hash[0] == '/') hash++;

		delete[] m_file;
		m_file = new char[strlen(hash)+1];
		strcpy(m_file,hash);
	}	
}

void VFSHandle_ZIP::CreateCache(void)
{
	m_stream.seekg(0,std::ios::beg);

	unsigned short zipsig,chksig;

	while(!m_stream.eof()){
		if((zipsig = ReadSignature()) == ZIP_SIG){
			
			switch((chksig = ReadSignature())){
				case LF_SIG:{
					ReadLF();
				}break;

				case FH_SIG:{
					ReadFH();
				}break;

				case CD_SIG:{
					ReadCD();
				}break;

				default:{
					debug << "unknown chunk = " << chksig << std::endl;
				}break; 
			};
		}else{
			debug << "Unknown sig = " << zipsig << std::endl;
		}
	}

	m_stream.clear();
}

void VFSHandle_ZIP::DeleteCache(void)
{
	int a;
	
	//	delete all the local file header data
	for(a=0;a<m_lfh.size();a++){
		delete[]	m_lfh[a]->filename;
		//delete[]	m_lfh[a]->extrafield;
		delete		m_lfh[a];
	}
	
	m_lfh.clear();

	//	delete all the fileheader data
	for(a=0;a<m_fh.size();a++){
		delete[]	m_fh[a]->filename;
		//delete[]	m_fh[a]->extrafield;
		//delete[]	m_fh[a]->file_comment;
		delete		m_fh[a];
	}

	m_fh.clear();
}

void VFSHandle_ZIP::ReadLF(void)
{
	LocalFileHeader *lfh = new LocalFileHeader;

	memset(lfh,0,sizeof(LocalFileHeader));

	lfh->version						=	ReadShort();
	lfh->general_flags			=	ReadShort();
	lfh->compression_method	=	ReadShort();
	lfh->time								=	ReadShort();
	lfh->date								=	ReadShort();
	lfh->crc32							=	ReadInt();
	lfh->comp_size					=	ReadInt();
	lfh->uncomp_size				=	ReadInt();
	lfh->filename_length		=	ReadShort();
	lfh->extrafield_length	=	ReadShort();
	lfh->filename						= ReadString(lfh->filename_length);
	lfh->extrafield					= ReadString(lfh->extrafield_length);
	lfh->file_data_offset		= m_stream.tellg();

	m_lfh.push_back(lfh);

	m_stream.seekg(lfh->comp_size,std::ios::cur);

	DEBUG_READLF;
}

void VFSHandle_ZIP::WriteLF(LocalFileHeader *lfh)
{
	WriteShort	(ZIP_SIG);
	WriteShort	(LF_SIG);
	WriteShort	(lfh->version);
	WriteShort	(lfh->general_flags);
	WriteShort	(lfh->compression_method);
	WriteShort	(lfh->time);
	WriteShort	(lfh->date);
	WriteInt		(lfh->crc32);
	WriteInt		(lfh->comp_size);
	WriteInt		(lfh->uncomp_size);
	WriteShort	(lfh->filename_length);
	WriteShort	(lfh->extrafield_length);
	WriteString	(lfh->filename,lfh->filename_length);
	WriteString	(lfh->extrafield,lfh->extrafield_length);

	lfh->file_data_offset = m_stream.tellg();

	DEBUG_WRITELF;
}

void VFSHandle_ZIP::ReadFH(void)
{
	FileHeader *fh = new FileHeader;

	memset(fh,0,sizeof(FileHeader));
	
	fh->version								=	ReadShort();
	fh->version_extract				=	ReadShort();
	fh->general_flags					=	ReadShort();
	fh->compression_method		=	ReadShort();
	fh->time									=	ReadShort();
	fh->date									=	ReadShort();
	fh->crc32									=	ReadInt();
	fh->comp_size							=	ReadInt();
	fh->uncomp_size						=	ReadInt();
	fh->filename_length				=	ReadShort();
	fh->extrafield_length			=	ReadShort();
	fh->file_comment_length		=	ReadShort();
	fh->disk_number_start			=	ReadShort();
	fh->internal_file_attrib	=	ReadShort();
	fh->external_file_attrib	=	ReadInt();
	fh->relative_offset_lfh		=	ReadInt();
	fh->filename							= ReadString(fh->filename_length);
	fh->extrafield						= ReadString(fh->extrafield_length);
	fh->file_comment					= ReadString(fh->file_comment_length);

	DEBUG_READFH;

	m_fh.push_back(fh);
}

void VFSHandle_ZIP::WriteFH(FileHeader *fh)
{
	WriteShort	(ZIP_SIG);
	WriteShort	(FH_SIG);
	WriteShort	(fh->version);
	WriteShort	(fh->version_extract);
	WriteShort	(fh->general_flags);
	WriteShort	(fh->compression_method);
	WriteShort	(fh->time);
	WriteShort	(fh->date);
	WriteInt		(fh->crc32);
	WriteInt		(fh->comp_size);
	WriteInt		(fh->uncomp_size);
	WriteShort	(fh->filename_length);
	WriteShort	(fh->extrafield_length);
	WriteShort	(fh->file_comment_length);
	WriteShort	(fh->disk_number_start);
	WriteShort	(fh->internal_file_attrib);
	WriteInt		(fh->external_file_attrib);
	WriteInt		(fh->relative_offset_lfh);
	WriteString	(fh->filename,fh->filename_length);
	WriteString	(fh->extrafield,fh->extrafield_length);
	WriteString	(fh->file_comment,fh->file_comment_length);

	DEBUG_WRITEFH;
}

void VFSHandle_ZIP::ReadCD(void)
{
	memset(&m_cd,0,sizeof(CentralDir));

	m_cd.number_disk				=	ReadShort();
	m_cd.number_disk_start	=	ReadShort();
	m_cd.total_entries_disk	=	ReadShort();
	m_cd.total_entries			=	ReadShort();
	m_cd.size								=	ReadInt();
	m_cd.offset							=	ReadInt();
	m_cd.comment_length			=	ReadShort();
	m_cd.comment						= ReadString(m_cd.comment_length);

	DEBUG_READCD;
}

void VFSHandle_ZIP::WriteCD(void)
{
	WriteShort	(ZIP_SIG);
	WriteShort	(CD_SIG);
	WriteShort	(m_cd.number_disk);
	WriteShort	(m_cd.number_disk_start);
	WriteShort	(m_cd.total_entries_disk);
	WriteShort	(m_cd.total_entries);
	WriteInt		(m_cd.size);
	WriteInt		(m_cd.offset);
	WriteShort	(m_cd.comment_length);
	WriteString	(m_cd.comment,m_cd.comment_length);

	DEBUG_WRITECD;
}

//=========================================================
//=========================================================
//	public Archive actions methods
//=========================================================
//=========================================================

FileInfo *  VFSHandle_ZIP::Read(void)
{
	if(IsFile(m_file) == true){
		char *buffer = (char *)Read(m_length);

		char	*filename	=	m_lfh[m_fileid]->filename;
		char	*tempdir	=	fusion->vfs->GetTempDirectory();

		if(tempdir == NULL){
			fusion->vfs->SetTempDirectory("vfstemp");
			tempdir = fusion->vfs->GetTempDirectory();
		}

		char fn[256];

		sprintf(fn,"%s/%s",tempdir,filename);

		m_handle = fusion->vfs->Open(fn,"binary",true);

		if(m_handle != NULL){
			m_handle->Write(buffer,m_length);
			fusion->vfs->Close(m_handle);
		}

		m_handle = fusion->vfs->Open(fn);

		m_length = m_handle->Length();

		delete[] buffer;

		return m_handle->Read();
	}

	return NULL;
}

unsigned char * VFSHandle_ZIP::Read(unsigned int &length)
{
	LocalFileHeader *lfh = m_lfh[m_fileid];

	m_stream.clear();
	m_stream.seekg(lfh->file_data_offset,std::ios::beg);

	unsigned char *buffer = NULL;

	if(lfh->compression_method == NO_COMPRESSION){
		//	Files are just stored
		buffer = ReadRaw(m_length);

	}else if(lfh->compression_method == DEFLATE_COMPRESSION){
		//	Files are deflated

		//	Read compressed data from the disk
		unsigned char *src = ReadRaw(lfh->comp_size);

		//	Allocate a buffer for the decompressed data
		buffer = new unsigned char[length];

		//	setup the z_stream structure
		memset(&m_zipstream,0,sizeof(z_stream));

		m_zipstream.avail_in	= lfh->comp_size;
		m_zipstream.next_in		=	src; 

		m_zipstream.avail_out	= m_length;
		m_zipstream.next_out	=	buffer;

		inflateInit2(&m_zipstream,-15);
		inflate(&m_zipstream,Z_FINISH);
		inflateEnd(&m_zipstream);

		delete[] src;
	}

	return buffer;
}

void VFSHandle_ZIP::Write(FileInfo *data)
{
	BinaryFileInfo *bfi = (BinaryFileInfo *)data;

	if(IsFile(m_file) == true){
		LocalFileHeader *lfh = m_lfh[m_fileid];

		lfh->uncomp_size = bfi->filelength;

		Write(bfi->data,bfi->filelength);
	}
}

void VFSHandle_ZIP::Write(unsigned char *data, unsigned int length)
{
	LocalFileHeader	*lfh	=	m_lfh[m_fileid];
	FileHeader			*fh		=	m_fh[m_fileid];

	//	compress data
	unsigned char *dest = new unsigned char[length];

	memset(&m_zipstream,0,sizeof(z_stream));

	m_zipstream.avail_in	= length;
	m_zipstream.next_in		=	data;

	m_zipstream.avail_out	= length;
	m_zipstream.next_out	=	dest;

	deflateInit2(&m_zipstream,9,Z_DEFLATED,-15,9,Z_DEFAULT_STRATEGY);
	deflate(&m_zipstream,Z_FINISH);
	deflateEnd(&m_zipstream);

	int crc = crc32(0L, Z_NULL, 0);
	lfh->crc32							=	crc32(crc,data,length);
	lfh->uncomp_size				=	length;
	lfh->comp_size					= m_zipstream.total_out;
	fh->comp_size						=	m_zipstream.total_out;
	fh->uncomp_size					=	lfh->uncomp_size;
	fh->crc32								=	lfh->crc32;
	fh->relative_offset_lfh	=	m_stream.tellg();

	WriteLF(lfh);

	//	write data to archive
	m_stream.clear();
	m_stream.seekg(lfh->file_data_offset,std::ios::beg);
	m_stream.write((char *)dest,lfh->comp_size);

	delete[] dest;
}

//=========================================================
//=========================================================
//	Archive Information methods
//=========================================================
//=========================================================
char * VFSHandle_ZIP::Filename(void)
{
	return m_file;
}

unsigned int VFSHandle_ZIP::Length(void)
{
	if(IsFile(m_file) == true){
		LocalFileHeader *lf = m_lfh[m_fileid];

		m_length = lf->uncomp_size;
	}

	return m_length;
}

void VFSHandle_ZIP::SetPlugin(VFSPlugin *plugin)
{
	//	archive file handles dont decode files
	//	they do nothing but extract to a temp/perm file
}


//=========================================================
//=========================================================
//	File/Directory manipulation methods 
//=========================================================
//=========================================================

bool VFSHandle_ZIP::IsFile(char *filename)
{
	if(filename != NULL){
		if(strcmp(filename,"*.*") == 0){
			m_fileid = -2;
			return false;
		}

		for(int a=0;a<m_lfh.size();a++){
			if(strcmp(m_lfh[a]->filename, filename) == 0){
				m_fileid = a;
				return true;
			}
		}
	}

	m_fileid = -1;

	return false;
}

bool VFSHandle_ZIP::IsDirectory(char *directory)
{
	return false;
}

bool VFSHandle_ZIP::Createfile(char *filename, bool recurse)
{
	LocalFileHeader *lfh = new LocalFileHeader;
	FileHeader *fh = new FileHeader;

	time_t t;
	time(&t);
	tm *time = localtime(&t);

	time->tm_sec/=2;
	time->tm_mon+=1;
	time->tm_year-=80;

#define MAKETIME(h,m,s)	h<<11	|	m<<5	|	s
#define MAKEDATE(d,m,y)	y<<9	|	m<<5	|	d

	//	Make a Local File Header
	lfh->version						=	20;
	lfh->general_flags			=	2;
	lfh->compression_method	=	8;
	lfh->time								=	MAKETIME(time->tm_hour,time->tm_min,time->tm_sec);
	lfh->date								=	MAKEDATE(time->tm_mday,time->tm_mon,time->tm_year);
	lfh->crc32							=	0;
	lfh->comp_size					=	0;
	lfh->uncomp_size				=	0;
	lfh->filename_length		=	strlen(filename);
	lfh->extrafield_length	=	strlen("");
	lfh->filename						=	new char[strlen(filename)+1];
	lfh->extrafield					=	"";
	lfh->file_data_offset		=	0;
	
	strcpy(lfh->filename,filename);

	//	Make a File Header for the CD
	fh->version								=	lfh->version;
	fh->version_extract				=	lfh->version;
	fh->general_flags					=	lfh->general_flags;
	fh->compression_method		=	lfh->compression_method;
	fh->time									=	lfh->time;
	fh->date									=	lfh->date;
	fh->crc32									=	lfh->crc32;
	fh->comp_size							=	lfh->comp_size;
	fh->uncomp_size						=	lfh->uncomp_size;
	fh->filename_length				=	lfh->filename_length;
	fh->extrafield_length			=	lfh->extrafield_length;
	fh->file_comment_length		=	0;
	fh->disk_number_start			=	0;
	fh->internal_file_attrib	=	0;
	fh->external_file_attrib	=	0;
	fh->relative_offset_lfh		=	0;
	fh->filename							= new char[strlen(filename)+1];
	fh->extrafield						= "";
	fh->file_comment					= "";

	m_cd.size += (FH_LEN + strlen(filename));

	strcpy(fh->filename,filename);

	m_lfh.push_back(lfh);
	m_fh.push_back(fh);

	m_cd.total_entries++;
	m_cd.total_entries_disk++;

	delete[] m_file;
	m_file = new char[strlen(filename)+1];
	strcpy(m_file,filename);

	return true;
}

bool VFSHandle_ZIP::Deletefile(char *filename)
{
	return false;
}

bool VFSHandle_ZIP::Copyfile(char *src, char *dest, bool createpath)
{
	if(IsFile(src) == true){
		m_file = src;
		Length();
		m_file = NULL;

		char *buffer = (char *)Read(m_length);

		m_handle = fusion->vfs->Open(dest,"binary",true);

		if(m_handle != NULL){
			m_handle->Write(buffer,m_length);
			fusion->vfs->Close(m_handle);

			delete[] buffer;

			return true;
		}

		delete[] buffer;
	}

	if(m_fileid == -2){
		//	*.* wildcard was played

		char fn[256];

		for(int a=0;a<m_lfh.size();a++){
			memset(fn,0,256);

			sprintf(fn,"%s/%s",dest,m_lfh[a]->filename);
			
			if(Copyfile(m_lfh[a]->filename,fn,createpath) == false){
				// something went wrong, shall we do something about it?
			}
		}
	}

	return false;
}

bool VFSHandle_ZIP::Movefile(char *src, char *dest, bool createpath)
{
	return false;
}

bool VFSHandle_ZIP::CreateDir(char *directory)
{
	return false;
}

bool VFSHandle_ZIP::DeleteDir(char *directory, bool recurse)
{
	return false;
}
