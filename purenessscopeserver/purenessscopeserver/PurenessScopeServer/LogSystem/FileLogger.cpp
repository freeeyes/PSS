#include "FileLogger.h"



//********************************************************

CFileLogger::CFileLogger()
{
    m_pLogFileList   = NULL;
    m_nCount         = 0;
    m_u4BlockSize    = 0;
    m_u4PoolCount    = 0;
    m_u4CurrLogLevel = 0;
    m_szLogRoot[0]   = '\0';
}

CFileLogger::~CFileLogger()
{
    OUR_DEBUG((LM_INFO, "[CFileLogger::~CFileLogger].\n"));
    Close();
    OUR_DEBUG((LM_INFO, "[CFileLogger::~CFileLogger]End.\n"));
}

void CFileLogger::Close()
{
    if(NULL != m_pLogFileList)
    {
        for(int i = 0; i < m_nCount; i++)
        {
            if(NULL != m_pLogFileList[i])
            {
                SAFE_DELETE(m_pLogFileList[i]);
            }
        }

        SAFE_DELETE_ARRAY(m_pLogFileList);
        m_nCount = 0;
    }
}

int CFileLogger::DoLog(int nLogType, _LogBlockInfo* pLogBlockInfo)
{
    //根据LogType取余，获得当前日志映射位置
    int nIndex = nLogType % m_nCount;

    if(NULL != m_pLogFileList[nIndex])
    {
        m_pLogFileList[nIndex]->doLog(pLogBlockInfo);
    }

    return 0;
}

int CFileLogger::GetLogTypeCount()
{
    return m_nCount;
}

bool CFileLogger::Init()
{
    CXmlOpeation objXmlOpeation;
    uint16 u2LogID                  = 0;
    uint8  u1FileClass              = 0;
    uint8  u1DisPlay                = 0;
    uint16 u2LogLevel               = 0;
    char szFile[MAX_BUFF_1024]      = {'\0'};
    char szFileName[MAX_BUFF_100]   = {'\0'};
    char szServerName[MAX_BUFF_100] = {'\0'};
    char* pData = NULL;
    vector<_Log_File_Info> objvecLogFileInfo;

    Close();

    sprintf_safe(szFile, MAX_BUFF_1024, "%s", FILELOG_CONFIG);

    if(false == objXmlOpeation.Init(szFile))
    {
        OUR_DEBUG((LM_ERROR,"[CFileLogger::Init] Read Configfile[%s] failed\n", szFile));
        return false;
    }

    //得到服务器名称
    pData = objXmlOpeation.GetData("ServerLogHead", "Text");

    if(pData != NULL)
    {
        sprintf_safe(szServerName, MAX_BUFF_100, "%s", pData);
    }

    OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]strServerName=%s\n", szServerName));

    //得到绝对路径
    pData = objXmlOpeation.GetData("LogPath", "Path");

    if(pData != NULL)
    {
        sprintf_safe(m_szLogRoot, MAX_BUFF_100, "%s", pData);
    }

    OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]m_strRoot=%s\n", m_szLogRoot));

    //得到日志池配置信息，日志块的大小
    pData = objXmlOpeation.GetData("LogPool", "BlockSize");

    if(pData != NULL)
    {
        m_u4BlockSize = (uint32)ACE_OS::atoi(pData);
    }

    //得到日志池配置信息，缓冲池中日志块的个数
    pData = objXmlOpeation.GetData("LogPool", "PoolCount");

    if(pData != NULL)
    {
        m_u4PoolCount = (uint32)ACE_OS::atoi(pData);
    }

    //得到日志池中的当前日志级别
    //此功能感谢宇/ka程枫 的好想法，赞一个，积少成多就会汇聚洪流
    pData = objXmlOpeation.GetData("LogLevel", "CurrLevel");

    if(pData != NULL)
    {
        m_u4CurrLogLevel = (uint32)ACE_OS::atoi(pData);
    }

    //添加子类的个数
    TiXmlElement* pNextTiXmlElement        = NULL;
    TiXmlElement* pNextTiXmlElementPos     = NULL;
    TiXmlElement* pNextTiXmlElementIdx     = NULL;
    TiXmlElement* pNextTiXmlElementDisplay = NULL;
    TiXmlElement* pNextTiXmlElementLevel   = NULL;

    while(true)
    {
        //得到日志id
        pData = objXmlOpeation.GetData("LogInfo", "logid", pNextTiXmlElementIdx);

        if(pData != NULL)
        {
            u2LogID = (uint16)atoi(pData);
            OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]u2LogID=%d\n", u2LogID));
        }
        else
        {
            break;
        }

        //得到日志名称
        pData = objXmlOpeation.GetData("LogInfo", "logname", pNextTiXmlElement);

        if(pData != NULL)
        {
            sprintf_safe(szFileName, MAX_BUFF_100, "%s", pData);
            OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]strFileValue=%s\n", szFileName));
        }
        else
        {
            break;
        }

        //得到日志类型
        pData = objXmlOpeation.GetData("LogInfo", "logtype", pNextTiXmlElementPos);

        if(pData != NULL)
        {
            u1FileClass = (uint8)atoi(pData);
            OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]u1FileClass=%d\n", u1FileClass));
        }
        else
        {
            break;
        }

        //得到日志输出来源，0为输出到文件，1为输出到屏幕
        pData = objXmlOpeation.GetData("LogInfo", "Display", pNextTiXmlElementDisplay);

        if(pData != NULL)
        {
            u1DisPlay = (uint8)atoi(pData);
            OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]u1DisPlay=%d\n", u1DisPlay));
        }
        else
        {
            break;
        }

        //得到日志当前级别
        pData = objXmlOpeation.GetData("LogInfo", "Level", pNextTiXmlElementLevel);

        if(pData != NULL)
        {
            u2LogLevel = (uint16)atoi(pData);
            OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]u4LogLevel=%d\n", u2LogLevel));
        }
        else
        {
            break;
        }

        //加入缓冲
        _Log_File_Info obj_Log_File_Info;
        obj_Log_File_Info.m_u2LogID     = u2LogID;
        obj_Log_File_Info.m_u1FileClass = u1FileClass;
        obj_Log_File_Info.m_u1DisPlay   = u1DisPlay;
        obj_Log_File_Info.m_u2LogLevel  = u2LogLevel;
        sprintf_safe(obj_Log_File_Info.m_szFileName, 100, "%s", szFileName);

        objvecLogFileInfo.push_back(obj_Log_File_Info);
        m_nCount++;
    }

    //创建对象列表
    m_pLogFileList = new CLogFile*[m_nCount];
    memset(m_pLogFileList, 0, sizeof(CLogFile*)*m_nCount);

    for(int i = 0; i < (int)objvecLogFileInfo.size(); i++)
    {
        int nPos = objvecLogFileInfo[i].m_u2LogID % m_nCount;
        CLogFile* pLogFile = new CLogFile(m_szLogRoot, m_u4BlockSize);

        pLogFile->SetLoggerName(objvecLogFileInfo[i].m_szFileName);
        pLogFile->SetLoggerID(objvecLogFileInfo[i].m_u2LogID);
        pLogFile->SetLoggerClass(objvecLogFileInfo[i].m_u1FileClass);
        pLogFile->SetLevel(objvecLogFileInfo[i].m_u2LogLevel);
        pLogFile->SetServerName(szServerName);
        pLogFile->SetDisplay(objvecLogFileInfo[i].m_u1DisPlay);

        if (false == pLogFile->Run())
        {
            OUR_DEBUG((LM_INFO, "[CFileLogger::Init]Run error.\n"));
        }

        m_pLogFileList[nPos] = pLogFile;
    }

    return true;
}

bool CFileLogger::ReSet(uint32 u4CurrLogLevel)
{
    //重置日志等级
    m_u4CurrLogLevel = u4CurrLogLevel;
    return true;
}

uint32 CFileLogger::GetBlockSize()
{
    return m_u4BlockSize;
}

uint32 CFileLogger::GetPoolCount()
{
    return m_u4PoolCount;
}

uint32 CFileLogger::GetCurrLevel()
{
    return m_u4CurrLogLevel;
}

uint16 CFileLogger::GetLogID(uint16 u2Index)
{
    if(u2Index >= m_nCount)
    {
        return 0;
    }

    return m_pLogFileList[u2Index]->GetLoggerID();
}

char* CFileLogger::GetLogInfoByServerName(uint16 u2LogID)
{
    int nIndex = u2LogID % m_nCount;

    return (char* )m_pLogFileList[nIndex]->GetServerName().c_str();
}

char* CFileLogger::GetLogInfoByLogName(uint16 u2LogID)
{
    int nIndex = u2LogID % m_nCount;

    return (char* )m_pLogFileList[nIndex]->GetLoggerName().c_str();
}

int CFileLogger::GetLogInfoByLogDisplay(uint16 u2LogID)
{
    int nIndex = u2LogID % m_nCount;

    return m_pLogFileList[nIndex]->GetDisPlay();
}

uint16 CFileLogger::GetLogInfoByLogLevel(uint16 u2LogID)
{
    int nIndex = u2LogID % m_nCount;

    return m_pLogFileList[nIndex]->GetLevel();
}

