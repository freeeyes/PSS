#ifndef _DIRVIEW_H
#define _DIRVIEW_H

#include "define.h"
#include <ace/Dirent.h>

#ifndef WIN32
#include <ace/OS_main.h>
#include <ace/OS_NS_stdio.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>
#include <sys/types.h>
#endif

#define DIR_OK             0
#define DIR_ERR_ISFILE     1
#define DIR_ERR_ISNOEXIST  2

#define IS_FILE            0
#define IS_DIRRNT          1

//文件列表结构
struct _FileInfo
{
    char   m_szFileName[MAX_BUFF_500];
    uint32 m_u4FileType;
    uint32 m_u4FileSize;

    _FileInfo()
    {
        m_szFileName[0] = '\0';
        m_u4FileType     = 0;
        m_u4FileSize     = 0;
    }
};

typedef vector<_FileInfo> vevFileInfo;

class CDirView
{
public:
    CDirView();
    ~CDirView();

    uint32 GetDir(const char* pPath, vevFileInfo& objvevFileInfo);   //得到目录结构
    bool GetFileBuffer(const char* pPath, char* pFileBuffer, uint32& u4FileBlockSize, uint32 u4BlockSize, uint32 u4BlockIndex); //得到文件块
    bool SetFileBuffer(const char* pPath, char* pFileBuffer, uint32& u4FileBlockSize, uint32 u4BlockIndex);                     //上传文件块
    bool GetFileBufferCount(const char* pPath, uint32 u4BlockSize, uint32& u4BlockCount);
};

#endif

