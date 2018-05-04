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

    //默认单个文件最大是8M
    int nData                       = -1;
    uint32 u4FileMaxSize            = MAX_BUFF_500;
    uint16 u2LogID                  = 0;
    uint16 u2LogLevel               = 0;
    uint8  u1FileClass              = 0;
    uint8  u1DisPlay                = 0;
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
    Read_XML_Data_Single_String(objXmlOpeation, "ServerLogHead", "Text", szServerName, MAX_BUFF_100);
    OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]strServerName=%s\n", szServerName));

    //得到单个日志最大大小
    Read_XML_Data_Single_Uint32(objXmlOpeation, "ServerLogHead", "LogFileMaxSize", u4FileMaxSize);

    //得到绝对路径
    Read_XML_Data_Single_String(objXmlOpeation, "LogPath", "Path", m_szLogRoot, MAX_BUFF_100);
    OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]m_strRoot=%s\n", m_szLogRoot));

    //得到日志池配置信息，日志块的大小
    Read_XML_Data_Single_Uint32(objXmlOpeation, "LogPool", "BlockSize", m_u4BlockSize);

    //得到日志池配置信息，缓冲池中日志块的个数
    Read_XML_Data_Single_Uint32(objXmlOpeation, "LogPool", "PoolCount", m_u4PoolCount);

    //得到日志池中的当前日志级别
    //此功能感谢宇/ka程枫 的好想法，赞一个，积少成多就会汇聚洪流
    Read_XML_Data_Single_Uint32(objXmlOpeation, "LogLevel", "CurrLevel", m_u4CurrLogLevel);

    //添加子类的个数
    TiXmlElement* pNextTiXmlElement        = NULL;
    TiXmlElement* pNextTiXmlElementPos     = NULL;
    TiXmlElement* pNextTiXmlElementIdx     = NULL;
    TiXmlElement* pNextTiXmlElementDisplay = NULL;
    TiXmlElement* pNextTiXmlElementLevel   = NULL;

    while(true)
    {
        //得到日志id
        if(true == Read_XML_Data_Multiple_Uint16(objXmlOpeation, "LogInfo", "logid", u2LogID, pNextTiXmlElementIdx))
        {
            OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]u2LogID=%d\n", u2LogID));
        }
        else
        {
            break;
        }

        //得到日志名称
        Read_XML_Data_Multiple_String(objXmlOpeation, "LogInfo", "logname", szFileName, MAX_BUFF_100, pNextTiXmlElement);
        OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]strFileValue=%s\n", szFileName));

        //得到日志类型
        Read_XML_Data_Multiple_Uint8(objXmlOpeation, "LogInfo", "logtype", u1FileClass, pNextTiXmlElementPos);
        OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]u1FileClass=%d\n", u1FileClass));

        //得到日志输出来源，0为输出到文件，1为输出到屏幕
        Read_XML_Data_Multiple_Uint8(objXmlOpeation, "LogInfo", "Display", u1DisPlay, pNextTiXmlElementDisplay);
        OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]u1DisPlay=%d\n", u1DisPlay));

        //得到日志当前级别
        Read_XML_Data_Multiple_Uint16(objXmlOpeation, "LogInfo", "Level", u2LogLevel, pNextTiXmlElementLevel);
        OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]u4LogLevel=%d\n", u2LogLevel));

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
        CLogFile* pLogFile = new CLogFile(m_szLogRoot, m_u4BlockSize, u4FileMaxSize);

        pLogFile->SetLoggerName(objvecLogFileInfo[i].m_szFileName);
        pLogFile->SetLoggerID(objvecLogFileInfo[i].m_u2LogID);
        pLogFile->SetLoggerClass(objvecLogFileInfo[i].m_u1FileClass);
        pLogFile->SetLevel(objvecLogFileInfo[i].m_u2LogLevel);
        pLogFile->SetServerName(szServerName);
        pLogFile->SetDisplay(objvecLogFileInfo[i].m_u1DisPlay);
        pLogFile->Init();

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

void CFileLogger::Read_XML_Data_Single_String(CXmlOpeation& objXmlOpeation, const char* pTag, const char* pName, char* pValue, int nMaxSize)
{
    char* pData = objXmlOpeation.GetData(pTag, pName);

    if (pData != NULL)
    {
        sprintf_safe(pValue, nMaxSize, "%s", pData);
    }
}

void CFileLogger::Read_XML_Data_Single_Uint32(CXmlOpeation& objXmlOpeation, const char* pTag, const char* pName, uint32& u4Value)
{
    char* pData = objXmlOpeation.GetData(pTag, pName);

    if (pData != NULL)
    {
        u4Value = (uint32)ACE_OS::atoi(pData);
    }
}

bool CFileLogger::Read_XML_Data_Multiple_String(CXmlOpeation& objXmlOpeation, const char* pTag, const char* pName, char* pValue, int nMaxSize, TiXmlElement*& pTi)
{
    char* pData = objXmlOpeation.GetData(pTag, pName, pTi);

    if (pData != NULL)
    {
        sprintf_safe(pValue, nMaxSize, "%s", pData);
        return true;
    }
    else
    {
        return false;
    }
}

bool CFileLogger::Read_XML_Data_Multiple_Uint16(CXmlOpeation& objXmlOpeation, const char* pTag, const char* pName, uint16& u2Value, TiXmlElement*& pTi)
{
    char* pData = objXmlOpeation.GetData(pTag, pName, pTi);

    if (pData != NULL)
    {
        u2Value = (uint16)atoi(pData);
        return true;
    }
    else
    {
        return false;
    }
}

bool CFileLogger::Read_XML_Data_Multiple_Uint8(CXmlOpeation& objXmlOpeation, const char* pTag, const char* pName, uint8& u1Value, TiXmlElement*& pTi)
{
    char* pData = objXmlOpeation.GetData(pTag, pName, pTi);

    if (pData != NULL)
    {
        u1Value = (uint8)atoi(pData);
        return true;
    }
    else
    {
        return false;
    }
}

