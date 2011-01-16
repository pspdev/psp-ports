#ifndef _SQLITE_OS_OTHER_H_
#define _SQLITE_OS_OTHER_H_

#if OS_PSP
#define sqlite3OsOpenReadWrite      sqlite3PspOpenReadWrite
#define sqlite3OsOpenExclusive      sqlite3PspOpenExclusive
#define sqlite3OsOpenReadOnly       sqlite3PspOpenReadOnly
#define sqlite3OsDelete             sqlite3PspDelete
#define sqlite3OsFileExists         sqlite3PspFileExists
#define sqlite3OsFullPathname       sqlite3PspFullPathname
#define sqlite3OsIsDirWritable      sqlite3PspIsDirWritable
#define sqlite3OsSyncDirectory      sqlite3PspSyncDirectory
#define sqlite3OsTempFileName       sqlite3PspTempFileName
#define sqlite3OsRandomSeed         sqlite3PspRandomSeed
#define sqlite3OsSleep              sqlite3PspSleep
#define sqlite3OsCurrentTime        sqlite3PspCurrentTime
#define sqlite3OsEnterMutex         sqlite3PspEnterMutex
#define sqlite3OsLeaveMutex         sqlite3PspLeaveMutex
#define sqlite3OsInMutex            sqlite3PspInMutex
#define sqlite3OsThreadSpecificData sqlite3PspThreadSpecificData
#define sqlite3OsMalloc             sqlite3GenericMalloc
#define sqlite3OsRealloc            sqlite3GenericRealloc
#define sqlite3OsFree               sqlite3GenericFree
#define sqlite3OsAllocationSize     sqlite3GenericAllocationSize
#define sqlite3OsDlopen             sqlite3PspDlopen
#define sqlite3OsDlsym              sqlite3PspDlsym
#define sqlite3OsDlclose            sqlite3PspDlclose
#endif

#endif /* _SQLITE_OS_OTHER_H_ */
