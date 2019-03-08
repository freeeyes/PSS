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
    //WindowsĿ¼����
    //�鿴��ǰ·���Ƿ�Ϊһ��Ŀ¼
    struct ACE_DIR* objopendir = ACE_OS::opendir(pPath);

    if(!objopendir)
    {
        //��ǰ��һ���ļ�����Ŀ¼
        return DIR_ERR_ISFILE;
    }
    else
    {
        //��Ŀ¼����Ŀ¼
        struct ACE_DIRENT* objreaddir = ACE_OS::readdir(objopendir);

        //�Թ�.��..
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
    //LinuxĿ¼��������
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
            //�����Ŀ¼
            sprintf(objFileInfo.m_szFileName, "%s", objreaddir->d_name);
            objFileInfo.m_u4FileType = IS_DIRRNT;
            objFileInfo.m_u4FileSize = 0;
        }
        else
        {
            //������ļ�
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

    //�жϻ���ļ�λ���Ƿ�Խ��
    u4FileBlockSize = u4BlockSize;
    uint32 u4FileSize = (int)ACE_OS::filesize(pPath);

    if(u4FileSize <= u4BlockSize * u4BlockIndex)
    {
        return false;
    }

    //����ļ���С�ڹ涨���ļ��ߴ磬��ֵΪʵ�ʳߴ�
    if(u4BlockSize * (u4BlockIndex + 1) > u4FileSize)
    {
        u4FileBlockSize = u4FileSize - u4BlockSize * u4BlockIndex;
    }

    FILE* pFile = fopen(pPath, "rb");

    if(NULL == pFile)
    {
        return false;
    }

    //���ö�ȡλ��
    fseek(pFile, (long)(u4BlockSize * u4BlockIndex), SEEK_CUR);

    //��ȡ��Ӧλ�õ��ļ���
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
    //������ļ��ĵ�һ��������ɾ����ǰ�ļ�
    if(u4BlockIndex == 0)
    {
        remove(pPath);
    }

    FILE* pFile = fopen(pPath, "ab+");

    if(NULL == pFile)
    {
        return false;
    }

    //д����Ӧλ�õ��ļ���
    uint32 u4WriteSize = (uint32)fwrite(pFileBuffer, sizeof(char), u4FileBlockSize, pFile);

    if(u4WriteSize != u4FileBlockSize)
    {
        fclose(pFile);
        return false;
    }

    fclose(pFile);
    return true;
}

