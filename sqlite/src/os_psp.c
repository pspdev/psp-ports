/*
** SQLite port for PSP
** by kolaaaa
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
******************************************************************************
**
** This file contains code that is specific to PSP.
*/
#include "sqliteInt.h"
#include "os.h"
#if OS_PSP				/* This file is used on PSP only */

/*
** Include code that is common to all os_*.c files
*/
#include "os_common.h"

/*
** Include code specific for PSP 
*/
#define u8 SceU8
#define u16 SceU16
#define u32 SceU32
#define u64 SceU64
#include <pspiofilemgr.h>
#include <psptypes.h>
#undef u8
#undef u16
#undef u32
#undef u64
#include <sys/stat.h>
#include <unistd.h>

#define SQLITE_PSP_FILE_ATTR		0777	/* rwxrwxrwx */
#define SQLITE_PSP_FULLPATH_SIZE	512	/* Max len of absolute path */

#ifdef SQLITE_DEBUG

static const char * getSqliteReturnCode (int rc){
	switch(rc)
	{
		case SQLITE_OK: return "SQLITE_OK";
		case SQLITE_CANTOPEN: return "SQLITE_CANTOPEN";
		case SQLITE_NOMEM: return "SQLITE_NOMEM";
		case SQLITE_IOERR: return "SQLITE_IOERR";
		case SQLITE_IOERR_READ: return "SQLITE_IOERR_READ";
		case SQLITE_IOERR_SHORT_READ: return "SQLITE_IOERR_SHORT_READ";
		case SQLITE_IOERR_WRITE: return "SQLITE_IOERR_WRITE";
		case SQLITE_IOERR_DELETE: return "SQLITE_IOERR_DELETE";
		case SQLITE_IOERR_FSYNC: return "SQLITE_IOERR_FSYNC";
		case SQLITE_IOERR_TRUNCATE: return "SQLITE_IOERR_TRUNCATE";
		case SQLITE_IOERR_FSTAT: return "SQLITE_IOERR_FSTAT";
		default: return "UNKNOWN CODE";
	}
}

static const char * getBooleanReturnCode (int rc){
	if(rc>0) return "TRUE";
	else return "FALSE";
}

#endif /* SQLITE_DEBUG */

/*
** The pspFile structure is subclass of OsFile specific for the PSP
** protability layer.
*/
typedef struct pspFile pspFile;
struct pspFile {
	IoMethod const *pMethod;	/* Always the first entry */
	SceUID fd;			/* PSP File descriptor */
	char *zFilename;		/* File name */
	int locktype;			/* Lock Type */
	int delFlags;			/* Delete file on close */
	int openFlags;			/* Flags passed to sceIoOpen when 
  					** opening the file (usefull for pspSync)*/
};

/*
** Do not include any of the File I/O interface procedures if the
** SQLITE_OMIT_DISKIO macro is defined (indicating that there database
** will be in-memory only)
*/
#ifndef SQLITE_OMIT_DISKIO

/*
** Delete the named file
*/
int sqlite3PspDelete( const char *zFilename ){
	int rc = ( (sceIoRemove(zFilename) >= 0) ? SQLITE_OK:SQLITE_IOERR_DELETE );
	OSTRACE3( "DELETE %s : %s\n", zFilename, getSqliteReturnCode(rc) );
	return rc;
}

/*
** Return TRUE if the named file exists.
*/
int sqlite3PspFileExists( const char *zFilename ){
	int rc = ( access(zFilename, F_OK)==0 );
	OSTRACE3("FILE EXIST %s : %s\n", zFilename, getBooleanReturnCode(rc));
	return rc;
}

/*
** Attempt to open a file for both reading and writing.  If that
** fails, try opening it read-only.  If the file does not exist,
** try to create it.
**
** On success, a handle for the open file is written to *id
** and *pReadonly is set to 0 if the file was opened for reading and
** writing or 1 if the file was opened read-only.  The function returns
** SQLITE_OK.
**
** On failure, the function returns SQLITE_CANTOPEN and leaves
** *id and *pReadonly unchanged.
*/
int sqlite3PspOpenReadWrite( const char *zFilename, OsFile **pId, int *pReadonly){
	SceUID file;
	*pReadonly=0;
	int rc=SQLITE_OK;
	int openFlags;
	
	file = sceIoOpen( zFilename, openFlags=(PSP_O_RDWR | PSP_O_CREAT), SQLITE_PSP_FILE_ATTR);
	if (file<0)
	{
		file = sceIoOpen( zFilename, openFlags=(PSP_O_RDONLY | PSP_O_CREAT), SQLITE_PSP_FILE_ATTR);
		if (file<0)
		{
			rc = SQLITE_CANTOPEN;
		}
		else
		{
			*pReadonly=1;
		}
	}

	if (rc==SQLITE_OK)
	{
		rc=allocatePspFile( &file, pId, zFilename, 0, openFlags);
	}

	OSTRACE3("OPEN R/W %s : %s\n", zFilename, getSqliteReturnCode(rc));
	return rc;
}

/*
** Attempt to open a new file for exclusive access by this process.
** The file will be opened for both reading and writing.  To avoid
** a potential security problem, we do not allow the file to have
** previously existed.  Nor do we allow the file to be a symbolic
** link.
**
** If delFlag is true, then make arrangements to automatically delete
** the file when it is closed.
**
** On success, write the file handle into *pId and return SQLITE_OK.
**
** On failure, return SQLITE_CANTOPEN.
*/
int sqlite3PspOpenExclusive( const char *zFilename, OsFile **pId, int delFlag){
	SceUID file;
	int rc=SQLITE_OK;
	int openFlags;

	file = sceIoOpen( zFilename, openFlags=(PSP_O_RDWR | PSP_O_CREAT | PSP_O_EXCL), SQLITE_PSP_FILE_ATTR);
	if (file<0)
	{
		rc = SQLITE_CANTOPEN;
	}
	else
	{
		rc = allocatePspFile( &file, pId, zFilename, delFlag, openFlags );
	}
	OSTRACE3("OPEN EX %s : %s\n", zFilename, getSqliteReturnCode(rc));
	return rc;
}

/*
** Attempt to open a new file for read-only access.
**
** On success, write the file handle into *id and return SQLITE_OK.
**
** On failure, return SQLITE_CANTOPEN.
*/
int sqlite3PspOpenReadOnly( const char *zFilename, OsFile **pId ){
	SceUID file;
	int rc=SQLITE_OK;
	int openFlags;

	file = sceIoOpen( zFilename, openFlags=(PSP_O_RDONLY | PSP_O_CREAT), SQLITE_PSP_FILE_ATTR);
	if (file<0)
	{
		rc = SQLITE_CANTOPEN;
	}
	else
	{
		rc = allocatePspFile( &file, pId, zFilename, 0, openFlags );
	}
	OSTRACE3("OPEN RO %s : %s\n", zFilename, getSqliteReturnCode(rc));
	return rc;
}

/*
** Attempt to open a file descriptor for the directory that contains a
** file.  This file descriptor can be used to fsync() the directory
** in order to make sure the creation of a new file is actually written
** to disk.
**
** This routine is only meaningful for Unix.  It is a no-op under
** PSP since PSP does not support hard links.
**
** On success, a handle for a previously open file is at *id is
** updated with the new directory file descriptor and SQLITE_OK is
** returned.
**
** On failure, the function returns SQLITE_CANTOPEN and leaves
** *id unchanged.
*/
int pspOpenDirectory( OsFile *id, const char *zDirname){
	return SQLITE_OK;
}

/*
** Create a temporary file name in zBuf.  zBuf must be big enough to
** hold at least SQLITE_TEMPNAME_SIZE characters.
*/
int sqlite3PspTempFileName( char *zBuf ){
	static const char zDir[] = "ms0:/";
	static const unsigned char zChars[] =
		"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789";
	int i,j;
	do{
		sprintf(zBuf, "%s"TEMP_FILE_PREFIX, zDir);
		j = strlen(zBuf);
		for(i=0; i<15; i++, j++)
		{
			zBuf[j] = (char)zChars[ ((unsigned char)zBuf[j])%(sizeof(zChars)-1) ];
		}
		zBuf[j] = 0;
	} while( access(zBuf, F_OK)==0 );

	return SQLITE_OK;
}

/*
** Close a file.
*/
int pspClose( OsFile **pId ){
	pspFile *pFile = (pspFile*)*pId;
	int rc = SQLITE_OK;

	if( pFile )
	{
		OSTRACE2("CLOSE %s : ", pFile->zFilename);
		if( sceIoClose(pFile->fd) < 0 )
		{
			rc = SQLITE_IOERR;
		}
		else
		{
			if( pFile->delFlags )
			{
				rc = sqlite3OsDelete(pFile->zFilename);
			}
		}

		sqlite3OsFree(pFile->zFilename);
		sqlite3OsFree(pFile);

		*pId = 0;
		OSTRACE2("CLOSE : %s\n", getSqliteReturnCode(rc)); 
	}
	return rc;
}

/*
** Read data from a file into a buffer.  Return SQLITE_OK if all
** bytes were read successfully and SQLITE_IOERR if anything goes
** wrong.
*/
int pspRead( OsFile *id, void *pBuf, int amt ){
	int got;
	pspFile * pFile = (pspFile*)id;
	int rc = SQLITE_OK;

	got=sceIoRead(pFile->fd, pBuf, amt);

	if( got!=amt )
	{
		if( got<0 )
		{
			rc = SQLITE_IOERR_READ;
		}
		else
		{
			memset(&((char*)pBuf)[got], 0, amt-got);
			rc = SQLITE_IOERR_SHORT_READ;
		}
	}
	OSTRACE3("READ %s : %s\n", pFile->zFilename, getSqliteReturnCode(rc));
	return rc;
}

/*
** Write data from a buffer into a file.  Return SQLITE_OK on success
** or some other error code on failure.
*/
int pspWrite( OsFile *id, const void *pBuf, int amt ){
	int rc = SQLITE_OK;
	pspFile * pFile = (pspFile*)id;

	if( sceIoWrite(pFile->fd, pBuf, amt) != amt )
	{
		rc = SQLITE_IOERR_WRITE;
	}
	OSTRACE3("WRITE %s : %s\n", pFile->zFilename, getSqliteReturnCode(rc));
	return rc;
}

/*
** Move the read/write pointer in a file.
*/
int pspSeek( OsFile *id, i64 offset ){
	int rc = SQLITE_OK;
	pspFile * pFile = (pspFile*)id;

	if( sceIoLseek(pFile->fd, offset, SEEK_SET) != offset)
	{
		rc = SQLITE_IOERR;
	}

	OSTRACE3("SEEK %s : %s\n", pFile->zFilename, getSqliteReturnCode(rc));
	return rc;
}

/*
** Make sure all writes to a particular file are committed to disk.
*/
int pspSync( OsFile *id, int dataOnly ){
/*
** sceIoSync() exist but is not documented enough.
** The file is close, and then reopen to synchronize data on the device.
** The position in the file is save (don't really matter because SQLITE
** seek the file before access to it, but I'm not sure).
*/
	int openFlags;
	int rc=SQLITE_OK;
	pspFile* pFile=(pspFile*)id;
	
	if(pFile)
	{
		SceOff currentOffset = sceIoLseek(pFile->fd, 0, SEEK_CUR);
		if( sceIoClose(pFile->fd) < 0 )
		{
			rc = SQLITE_IOERR_FSYNC;
		}
		else
		{
			openFlags=(pFile->openFlags)&((~PSP_O_CREAT)&(~PSP_O_EXCL));
			pFile->fd=sceIoOpen(pFile->zFilename, openFlags, SQLITE_PSP_FILE_ATTR);
			if(pFile->fd)
			{
				sceIoLseek(pFile->fd, currentOffset, SEEK_SET);
			}
			else
			{
				rc = SQLITE_CANTOPEN;
			}
		}
	}
	else
	{
		rc = SQLITE_IOERR_FSYNC;
	}
	OSTRACE3("SYNC %s : %s\n", pFile->zFilename, getSqliteReturnCode(rc));
 	return rc;
}

/*
** Sync the directory zDirname. This is a no-op on operating systems other
** than UNIX.
*/
int sqlite3PspSyncDirectory( const char *zDirname ){
	return SQLITE_OK;
}


/*
** Truncate an open file to a specified size
*/
int pspTruncate( OsFile *id, i64 nByte ){
	int rc;
	pspFile* pFile=(pspFile*)id;
	rc = (truncate(pFile->zFilename, nByte)==0) ? SQLITE_OK:SQLITE_IOERR_TRUNCATE;
	OSTRACE3("TRUNCATE %s : %s\n", pFile->zFilename, getSqliteReturnCode(rc));
	return rc;
}

/*
** Determine the current size of a file in bytes
*/
int pspFileSize( OsFile *id, i64 *pSize ){
	pspFile * pFile = (pspFile*)id;
	SceOff curPos = sceIoLseek(pFile->fd, 0, SEEK_CUR);
	SceOff size = sceIoLseek(pFile->fd, 0, SEEK_END);
	sceIoLseek(pFile->fd, curPos, SEEK_SET);
	*pSize = (i64) size;
	OSTRACE3("SIZE %s : %d bytes\n",pFile->zFilename,*pSize);
	return SQLITE_OK;
}

/*
** Check that a given pathname is a directory and is writable
** Code from os_unix.c
*/
int sqlite3PspIsDirWritable( char *zDirname ){
#ifndef SQLITE_OMIT_PAGER_PRAGMAS
	struct stat buf;
	if( zDirname==0 ) return 0;
	if( zDirname[0]==0 ) return 0;
	if( stat(zDirname, &buf) ) return 0;
	if( !S_ISDIR(buf.st_mode) ) return 0;
	if( access(zDirname, 07) ) return 0;
#endif /* SQLITE_OMIT_PAGER_PRAGMAS */
  return 1;
}

/*
** Lock the file with the lock specified by parameter locktype - one
** of the following:
**
**     (1) SHARED_LOCK
**     (2) RESERVED_LOCK
**     (3) PENDING_LOCK
**     (4) EXCLUSIVE_LOCK
**
** Sometimes when requesting one lock state, additional lock states
** are inserted in between.  The locking might fail on one of the later
** transitions leaving the lock state different from what it started but
** still short of its goal.  The following chart shows the allowed
** transitions and the inserted intermediate states:
**
**    UNLOCKED -> SHARED
**    SHARED -> RESERVED
**    SHARED -> (PENDING) -> EXCLUSIVE
**    RESERVED -> (PENDING) -> EXCLUSIVE
**    PENDING -> EXCLUSIVE
**
*/
int pspLock( OsFile *id, int locktype ){
	((pspFile*)id)->locktype = locktype;
	return SQLITE_OK; 
}

/*
** This routine checks if there is a RESERVED lock held on the specified
** file by this or any other process. If such a lock is held, return
** non-zero, otherwise zero.
*/
int pspCheckReservedLock( OsFile *id ){
	return (((pspFile*)id)->locktype==RESERVED_LOCK);
}

/*
** Lower the locking level on file descriptor id to locktype.  locktype
** must be either NO_LOCK or SHARED_LOCK.
**
** If the locking level of the file descriptor is already at or below
** the requested locking level, this routine is a no-op.
**
** It is not possible for this routine to fail if the second argument
** is NO_LOCK.  If the second argument is SHARED_LOCK then this routine
** might return SQLITE_IOERR;
*/
int pspUnlock( OsFile *id, int locktype ){
	pspFile* pFile = (pspFile*)id;
	if(pFile->locktype!=NO_LOCK)
	{
		pFile->locktype = locktype;
	}
	return SQLITE_OK;
}

/*
** Turn a relative pathname into a full pathname.  Return a pointer
** to the full pathname stored in space obtained from sqliteMalloc().
** The calling function is responsible for freeing this space once it
** is no longer needed.
*/
char *sqlite3PspFullPathname( const char *zRelative ){
	/* Don't need to convert cause the relative path is
	 * handle by the PSP's os
	 */
	char *zAbsolute;
	zAbsolute=sqlite3OsMalloc( strlen(zRelative)+1 );
	strcpy(zAbsolute, zRelative);
	return zAbsolute;
}

/*
** The fullSync option is meaningless on PSP.   This is a no-op.
*/
static void pspSetFullSync( OsFile *id, int setting ){
	return;
}

/*
** Return the underlying file handle for an OsFile
*/
static int pspFileHandle( OsFile *id ){
	return ((pspFile*)id)->fd;
}

/*
** Return an integer that indices the type of lock currently held
** by this handle.  (Used for testing and analysis only.)
*/
static int pspLockState( OsFile *id ){
	return ((pspFile*)id)->locktype;
}

/*
** Return the sector size in bytes of the underlying block device for
** the specified file. This is almost always 512 bytes, but may be
** larger for some devices.
**
** SQLite code assumes this function cannot fail. It also assumes that
** if two files are created in the same file-system directory (i.e.
** a database and it's journal file) that the sector size will be the
** same for both.
*/
static int pspSectorSize(OsFile *id){
	return SQLITE_DEFAULT_SECTOR_SIZE;
}

/*
** This vector defines all the methods that can operate on an OsFile
** for PSP.
*/
static const IoMethod sqlite3PspIoMethod = {
	pspClose,
	pspOpenDirectory,
	pspRead,
	pspWrite,
	pspSeek,
	pspTruncate,
	pspSync,
	pspSetFullSync,
	pspFileHandle,
	pspFileSize,
	pspLock,
	pspUnlock,
	pspLockState,
	pspCheckReservedLock,
	pspSectorSize,
};

/*
** Allocate memory for an OsFile.  Initialize the new OsFile
** to the value given in pInit and return a pointer to the new
** OsFile.  If we run out of memory, close the file and return NULL.
*/
int allocatePspFile(
	SceUID *pInit,
	OsFile **pId,
	char * zFilename,
	int delFlags,
	int openFlags
){
	int rc = SQLITE_OK;
	pspFile * pNew ;
	
	pNew = sqlite3OsMalloc( sizeof(pspFile) );

	if(pNew==0)
	{
		*pId=0;
		rc = SQLITE_NOMEM;
	}
	else
	{
		*pId = (OsFile*)pNew;

		pNew->pMethod = &sqlite3PspIoMethod;
		pNew->fd = *pInit;
		pNew->locktype = NO_LOCK;
		pNew->delFlags = delFlags;
		pNew->openFlags = openFlags;

		pNew->zFilename = sqlite3OsMalloc( strlen(zFilename) + 1 );
		if(pNew->zFilename==0)
		{
			sqlite3OsFree( pNew );
			*pId=0;
			rc = SQLITE_NOMEM;
		}
		else
		{
			strcpy(pNew->zFilename, zFilename);
		}
	}
	return rc;
}

#endif /* SQLITE_OMIT_DISKIO */
/***************************************************************************
** Everything above deals with file I/O.  Everything that follows deals
** with other miscellanous aspects of the operating system interface
****************************************************************************/

#ifndef SQLITE_OMIT_LOAD_EXTENSION
/*
** Interfaces for opening a shared library, finding entry points
** within the shared library, and closing the shared library.
*/
void *sqlite3PspDlopen(const char *zFilename){
}

void *sqlite3PspDlsym(void *pHandle, const char *zSymbol){
}

int sqlite3PspDlclose(void *pHandle){
	return SQLITE_OK;
}
#endif /* SQLITE_OMIT_LOAD_EXTENSION */

/*
** Get information to seed the random number generator.  The seed
** is written into the buffer zBuf[256].  The calling function must
** supply a sufficiently large buffer.
*/
int sqlite3PspRandomSeed( char *zBuf ){
  /* 
  ** When testing, initializing zBuf[] to zero is all we do.  That means
  ** that we always use the same random number sequence. This makes the
  ** tests repeatable.
  */
	memset( zBuf, 0, 256 );	
	sceKernelLibcTime((time_t*)zBuf);
	return SQLITE_OK;
}

/*
** Sleep for a little while.  Return the amount of time slept.
*/
int sqlite3PspSleep( int ms ){
	sceKernelDelayThread(ms*1000);
	return ms;
}

/*
** The following pair of routines implement mutual exclusion for
** multi-threaded processes.  Only a single thread is allowed to
** executed code that is surrounded by EnterMutex() and LeaveMutex().
**
** SQLite uses only a single Mutex.  There is not much critical
** code and what little there is executes quickly and without blocking.
*/

static int inMutex = 0;

void sqlite3PspEnterMutex(){
	inMutex++;
}

void sqlite3PspLeaveMutex(){
	if(inMutex>0) inMutex--;
}

/*
** Return TRUE if the mutex is currently held.
**
** If the thisThreadOnly parameter is true, return true if and only if the
** calling thread holds the mutex.  If the parameter is false, return
** true if any thread holds the mutex.
*/
int sqlite3PspInMutex( int thisThreadOnly ){
	return inMutex>0;
}

/*
** Find the current time (in Universal Coordinated Time).  Write the
** current time and date as a Julian Day number into *prNow and
** return 0.  Return 1 if the time and date cannot be found.
*/
int sqlite3PspCurrentTime( double *prNow ){
	time_t time;
	sceKernelLibcTime(&time);
	*prNow = (time / 86400.0) + 2440587.5;
	return 0;
}

/*
** If called with allocateFlag>1, then return a pointer to thread
** specific data for the current thread.  Allocate and zero the
** thread-specific data if it does not already exist necessary.
**
** If called with allocateFlag==0, then check the current thread
** specific data.  Return it if it exists.  If it does not exist,
** then return NULL.
**
** If called with allocateFlag<0, check to see if the thread specific
** data is allocated and is all zero.  If it is then deallocate it.
** Return a pointer to the thread specific data or NULL if it is
** unallocated or gets deallocated.
*/
ThreadData *sqlite3PspThreadSpecificData( int allocateFlag ){
	return NULL;
}

#endif /* OS_PSP */
