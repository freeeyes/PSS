#include "DirView.h"

CDirView::CDirView()
{
}

CDirView::~CDirView()
{
}

uint32 CDirView::GetDir( const char* pPath, vevFileInfo& objvevFileInfo )
{
    objvevFileInfo.clear();
#ifdef WIN32
    //Windows目录遍历
    //查看当前路径是否为一个目录
    struct ACE_DIR* objopendir = ACE_OS::opendir(pPath);

    if(!objopendir)
    {
        //当前是一个文件不是目录
        return DIR_ERR_ISFILE;
    }
    else
    {
        //是目录，打开目录
        struct ACE_DIRENT* objreaddir = ACE_OS::readdir(objopendir);

        //略过.和..
        while(objreaddir && objreaddir->d_name[0] == '.')
        {
            objreaddir = ACE_OS::readdir(objopendir);
        }

        while(objreaddir)
        {
            char szFilePath[500] = {'\0'};
            sprintf_safe(szFilePath, MAX_BUFF_500, "%s%s", pPath, objreaddir->d_name);
            _FileInfo objFileInfo;
            sprintf_safe(objFileInfo.m_szFileName, MAX_BUFF_500, "%s", objreaddir->d_name);

            struct ACE_DIR* objIsdir = ACE_OS::opendir(szFilePath);

            if(objIsdir == NULL)
            {
                objFileInfo.m_u4FileType = IS_FILE;
                objFileInfo.m_u4FileSize = (int)ACE_OS::filesize(szFilePath);
            }
            else
            {
                objFileInfo.m_u4FileType = IS_DIRRNT;
                objFileInfo.m_u4FileSize = 0;
            }

            objvevFileInfo.push_back(objFileInfo);

            objreaddir = ACE_OS::readdir_emulation(objopendir);
        }

        ACE_OS::closedir_emulation(objopendir);
    }

#else
    //Linux目录遍历部分
    struct dirent* objreaddir = NULL;
    DIR* pDir = opendir(pPath);

    while((objreaddir=readdir(pDir))!=NULL)
    {
        if(strcmp(objreaddir->d_name,".")==0 || strcmp(objreaddir->d_name,"..")==0)
        {
            continue;
        }

        char szFilePath[500] = {'\0'};
        sprintf(szFilePath, "%s%s", pPath, objreaddir->d_name);

        _FileInfo objFileInfo;

        if(objreaddir->d_type & DT_DIR)
        {
            //如果是目录
            sprintf(objFileInfo.m_szFileName, "%s", objreaddir->d_name);
            objFileInfo.m_u4FileType = IS_DIRRNT;
            objFileInfo.m_u4FileSize = 0;
        }
        else
        {
            //如果是文件
            sprintf(objFileInfo.m_szFileName, "%s", objreaddir->d_name);
            objFileInfo.m_u4FileType = IS_FILE;
            objFileInfo.m_u4FileSize = (int)ACE_OS::filesize(szFilePath);
        }

        objvevFileInfo.push_back(objFileInfo);
    }

#endif

    return DIR_OK;
}

bool CDirView::GetFileBuffer( const char* pPath, char* pFileBuffer, uint32& u4FileBlockSize, uint32 u4BlockSize, uint32 u4BlockIndex )
{
    if(NULL == pFileBuffer)
    {
        return false;
    }

    //判断获得文件位置是否越界
    u4FileBlockSize = u4BlockSize;
    uint32 u4FileSize = (int)ACE_OS::filesize(pPath);

    if(u4FileSize <= u4BlockSize * u4BlockIndex)
    {
        return false;
    }

    //如果文件块小于规定的文件尺寸，则赋值为实际尺寸
    if(u4BlockSize * (u4BlockIndex + 1) > u4FileSize)
    {
        u4FileBlockSize = u4FileSize - u4BlockSize * u4BlockIndex;
    }

    FILE* pFile = fopen(pPath, "rb");

    if(NULL == pFile)
    {
        return false;
    }

    //设置读取位置
    fseek(pFile, (long)(u4BlockSize * u4BlockIndex), SEEK_CUR);

    //读取相应位置的文件块
    uint32 u4ReadSize = (uint32)fread(pFileBuffer, sizeof(char), u4FileBlockSize, pFile);

    if(u4ReadSize != u4FileBlockSize)
    {
        fclose(pFile);
        return false;
    }

    fclose(pFile);
    return true;
}

bool CDirView::GetFileBufferCount( const char* pPath, uint32 u4BlockSize, uint32& u4BlockCount )
{
    uint32 u4FileSize = (int)ACE_OS::filesize(pPath);

    if(u4FileSize % u4BlockSize != 0)
    {
        u4BlockCount = u4FileSize / u4BlockSize + 1;
    }
    else
    {
        u4BlockCount = u4FileSize / u4BlockSize;
    }

    return true;
}

bool CDirView::SetFileBuffer( const char* pPath, char* pFileBuffer, uint32& u4FileBlockSize, uint32 u4BlockIndex )
{
    //如果是文件的第一个包，则删除当前文件
    if(u4BlockIndex == 0)
    {
        remove(pPath);
    }

    FILE* pFile = fopen(pPath, "ab+");

    if(NULL == pFile)
    {
        return false;
    }

    //写入相应位置的文件块
    uint32 u4WriteSize = (uint32)fwrite(pFileBuffer, sizeof(char), u4FileBlockSize, pFile);

    if(u4WriteSize != u4FileBlockSize)
    {
        fclose(pFile);
        return false;
    }

    fclose(pFile);
    return true;
}

