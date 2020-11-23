#include "FileLogger.h"

CLogFile::CLogFile(const char* pFileRoot, uint32 u4BufferSize, uint32 u4FileMaxSize)
    : m_u4BufferSize(u4BufferSize),
    m_u4FileMaxSize(u4FileMaxSize* MAX_BUFF_1024 * MAX_BUFF_1024),
    m_strFileRoot(pFileRoot)
{
}

void CLogFile::Init()
{
    //�������ʼ����ȡ��ǰ�ļ��е��ļ������ź��ļ���С
    ACE_Date_Time dt;
    stringstream ss_format;
    ss_format << dt.year()
        << "-" << dt.month()
        << "-" << dt.day()
        << m_u2CurrFileIndex
        << ".log";
    string strDate = ss_format.str();

    m_pBuffer = new char[m_u4BufferSize];   //������������־ƴ��ʱ������

    //�����ж��ļ��Ƿ����
    while (true)
    {
        ACE_TString strLogModulePath = m_strFileRoot.c_str();
        ACE_TString strLogName = strLogModulePath + "/Log/" 
            + m_StrlogType + "/" 
            + m_StrlogName + "/" 
            + m_StrServerName 
            + m_StrlogName 
            + strDate.c_str();

        auto nRet = ACE_OS::access(strLogName.c_str(), R_OK);

        if (0 == nRet)
        {
            //����ļ��Ѵ��ڣ��ж��ļ������Ƿ񳬹���ֵ
            FILE* fp = ACE_OS::fopen(strLogName.c_str(), "r");

            if (nullptr == fp)
            {
                OUR_DEBUG((LM_INFO, "[CLogFile::Init]File(%s) fopen error.\n", strLogName.c_str()));
                return;
            }

            fseek(fp, 0L, SEEK_END);
            auto u4FileSize = (uint32)ftell(fp);
            fclose(fp);

            if (u4FileSize >= m_u4FileMaxSize)
            {
                m_u2CurrFileIndex++;
            }
            else
            {
                m_u4CurrFileSize = u4FileSize;
                return;
            }
        }
        else
        {
            int nError = errno;

            if (EACCES == nError || ENOENT == nError)
            {
                //�ҵ��˲����ڵ��ļ�
                return;
            }
            else
            {
                OUR_DEBUG((LM_INFO, "[CLogFile::Init]File(%s) access error(%d).\n", strLogName.c_str(), nError));
                return;
            }
        }
    }
}

void CLogFile::Close()
{
    OUR_DEBUG((LM_INFO, "[CLogFile::Close]m_StrlogName=%s.\n", m_StrlogName.c_str()));
    SAFE_DELETE_ARRAY(m_pBuffer);
    m_u4BufferSize = 0;
    m_File.close();
    OUR_DEBUG((LM_INFO, "[CLogFile::Close] End.\n"));
}

void CLogFile::SetFileRoot(const char* pFileRoot)
{
    m_strFileRoot = pFileRoot;
}

const char* CLogFile::GetFileRoot() const
{
    return m_strFileRoot.c_str();
}

void CLogFile::SetFileAddr(const ACE_FILE_Addr& objFileAddr)
{
    m_FileAddr = objFileAddr;
}

ACE_FILE_Addr& CLogFile::GetFileAddr()
{
    return m_FileAddr;
}

void CLogFile::SetConnector(const ACE_FILE_Connector& objConnector)
{
    m_Connector = objConnector;
}

ACE_FILE_Connector& CLogFile::GetConnector()
{
    return m_Connector;
}

void CLogFile::SetFileIO(const ACE_FILE_IO& objFile)
{
    m_File = objFile;
}

ACE_FILE_IO& CLogFile::GetFileIO()
{
    return m_File;
}

void CLogFile::SetLogTime(const char* pLogTime)
{
    m_strLogTime = pLogTime;
}

const char* CLogFile::GetLogTime() const
{
    return m_strLogTime.c_str();
}

void CLogFile::SetBufferSize(uint32 u4BufferSize)
{
    SAFE_DELETE_ARRAY(m_pBuffer);
    m_pBuffer = new char[u4BufferSize];   //������������־ƴ��ʱ������
    m_u4BufferSize = u4BufferSize;
}

void CLogFile::SetFileMaxSize(uint32 u4FileMaxSize)
{
    m_u4FileMaxSize = u4FileMaxSize;
}

void CLogFile::SetCurrFileIndex(uint16 u2CurrFileIndex)
{
    m_u2CurrFileIndex = u2CurrFileIndex;
}

void CLogFile::SetCurrFileSize(uint32 u4CirrFileSize)
{
    m_u4CurrFileSize = u4CirrFileSize;
}

uint32 CLogFile::GetBufferSize() const
{
    return m_u4BufferSize;
}

uint32 CLogFile::GetFileMaxSize() const
{
    return m_u4FileMaxSize;
}

uint16 CLogFile::GetCurrFileIndex() const
{
    return m_u2CurrFileIndex;
}

uint32 CLogFile::GetCurrFileSize() const
{
    return m_u4CurrFileSize;
}

int CLogFile::doLog(_LogBlockInfo* pLogBlockInfo)
{
    //ÿ���Զ����
    CheckTime();

    //��¼��ǰ�ļ�д�볤��
    m_u4CurrFileSize += pLogBlockInfo->m_u4Length;

    ACE_Date_Time dt;
    stringstream ss_format;
    ss_format << dt.year()
        << "-" << dt.month()
        << "-" << dt.day()
        << " " << dt.hour()
        << ":" << dt.minute()
        << ":" << dt.second()
        << dt.microsec() / 10000;
    string strDate = ss_format.str();

    //ƴ��ʵ�ʵ���־�ַ���
    sprintf_safe(m_pBuffer, m_u4BufferSize, "%s %s\n", strDate.c_str(), pLogBlockInfo->m_pBlock);

    if (m_nDisplay == 0)
    {
        //������־
        auto nLen = (int)m_File.send(m_pBuffer, strlen(m_pBuffer));

        if (nLen != (int)strlen(m_pBuffer))
        {
            OUR_DEBUG((LM_INFO, "[%s]Write error[%s].\n", m_StrlogName.c_str(), m_pBuffer));
        }
    }
    else
    {
        //�������Ļ
        OUR_DEBUG((LM_INFO, "%s.\n", m_pBuffer));
    }

    //�鿴�Ƿ�Ҫ�����ʼ�
    if (pLogBlockInfo->m_u2MailID > 0 && false == SendMail(pLogBlockInfo))
    {
        OUR_DEBUG((LM_INFO, "[CLogFile::doLog](%s)Send mail fail.\n", m_StrlogName.c_str()));
    }

    //����Ƿ񳬹����ļ��飬��������ˣ�����һ������־�ļ���
    CheckLogFileBlock();

    return 0;
}

bool CLogFile::SendMail(const _LogBlockInfo* pLogBlockInfo, const xmlMails::_Mail* pMailInfo) const
{
    //�����ʼ�
    const xmlMails::_Mail* pMailAlert = nullptr;

    //���û�е������ã���������ļ���ֱ�ӻ��
    if(nullptr == pMailInfo)
    {
        pMailAlert = GetXmlConfigAttribute(xmlMails)->GetMailAlert(pLogBlockInfo->m_u2MailID);
    }
    else
    {
        pMailAlert = pMailInfo;
    }

    if (nullptr == pMailAlert)
    {
        OUR_DEBUG((LM_ERROR, "[CLogFile::SendMail]MailID(%d) is no find.\n", pLogBlockInfo->m_u2MailID));
        return false;
    }

    stringstream ss_format;
    ss_format << pMailAlert->MailUrl << ":" << pMailAlert->MailPort;
    string szMailURL = ss_format.str();

    //����smtps�ʼ�
    int nRet = Send_Mail_From_Ssl(pMailAlert->fromMailAddr.c_str(),
        pMailAlert->MailPass.c_str(),
        pMailAlert->fromMailAddr.c_str(),
        pMailAlert->toMailAddr.c_str(),
        szMailURL.c_str(),
        pLogBlockInfo->m_szMailTitle,
        pLogBlockInfo->m_pBlock);

    if (0 != nRet)
    {
        return false;
    }
    else
    {
        return true;
    }
}

ACE_TString& CLogFile::GetLoggerName()
{
    return m_StrlogName;
}

ACE_TString& CLogFile::GetServerName()
{
    return m_StrServerName;
}

int CLogFile::GetDisPlay() const
{
    return m_nDisplay;
}

void CLogFile::SetDisplay(int nDisplay)
{
    if (nDisplay == 0)
    {
        m_nDisplay = 0;
    }
    else
    {
        m_nDisplay = 1;
    }
}

void CLogFile::SetLoggerClass(int nType)
{
    if (nType == 1)  //�����1��������Ӫ��־��������Ǵ�����־
    {
        m_StrlogType = LOGTYPE_OPERATION;
    }
    else
    {
        m_StrlogType = LOGTYPE_ERROR;
    }

    OUR_DEBUG((LM_INFO, "[ServerLogger](%d)m_StrlogType=%s.\n", nType, m_StrlogType.c_str()));
}

int CLogFile::GetLoggerClass() const
{
    if (LOGTYPE_OPERATION == m_StrlogType)
    {
        return 1;
    }
    else
    {
        return 2;
    }
}

void CLogFile::SetLoggerName(const char* szLoggerName)
{
    m_StrlogName = szLoggerName;
}

void CLogFile::SetLoggerID(uint16 u2LogID)
{
    m_u2LogID = u2LogID;
}

uint16 CLogFile::GetLoggerID() const
{
    return m_u2LogID;
}

void CLogFile::SetLevel(uint16 u2Level)
{
    m_u2Level = u2Level;
}

uint16 CLogFile::GetLevel() const
{
    return m_u2Level;
}

void CLogFile::SetServerName(const char* szServerName)
{
    m_StrServerName = szServerName;
}

bool CLogFile::Run()
{
    ACE_Date_Time dt;
    stringstream ss_format;
    ss_format << dt.year()
        << "-" << dt.month()
        << "-" << dt.day()
        << m_u2CurrFileIndex
        << ".log";
    string strDate = ss_format.str();


    dt.update(ACE_OS::gettimeofday());

    CreatePath();       //���Ŀ¼�������򴴽�Ŀ¼

    ss_format.clear();
    ss_format << dt.year() << "-" << dt.month() << "-" << dt.day();
    m_strLogTime = ss_format.str();

    ACE_TString strLogModulePath = m_strFileRoot.c_str();
    ACE_TString strLogName = strLogModulePath 
        + "/Log/" 
        + m_StrlogType 
        + "/" + m_StrlogName 
        + "/" + m_StrServerName 
        + m_StrlogName 
        + strDate.c_str();

    m_File.close();

    m_FileAddr.set(strLogName.c_str());

    if (m_Connector.connect(m_File, m_FileAddr, nullptr, ACE_Addr::sap_any, 0, O_WRONLY | O_CREAT | O_APPEND) == -1)
    {
        OUR_DEBUG((LM_INFO, "[ServerLogger]Create file error[%s].\n", strLogName.c_str()));
        return false;
    }

    //���õ�ǰ�ļ���С��0
    m_u4CurrFileSize = 0;

    OUR_DEBUG((LM_INFO, "[ServerLogger]Create file ok[%s].\n", strLogName.c_str()));
    return true;
}

void CLogFile::CheckTime()
{
    ACE_Date_Time dt;
    stringstream ss_format;

    dt.update(ACE_OS::gettimeofday());

    ss_format.clear();
    ss_format << dt.year() << "-" << dt.month() << "-" << dt.day();
    string strDate = ss_format.str();

    if (strDate != m_strLogTime && false == Run())
    {
        OUR_DEBUG((LM_INFO, "[ServerLogger](%s)Run fail.\n", m_StrlogName.c_str()));
    }
}

void CLogFile::CheckLogFileBlock()
{
    //�鿴�ļ��Ƿ񳬹����������
    if (m_u4CurrFileSize >= m_u4FileMaxSize)
    {
        //�������ļ�
        m_u2CurrFileIndex++;

        if (false == Run())
        {
            OUR_DEBUG((LM_INFO, "[ServerLogger](%s)Run fail.\n", m_StrlogName.c_str()));
        }
    }
}

void CLogFile::CreatePath() const
{
    int n4Return = -1;
    int nError = 0;
    string strPath = m_strFileRoot + "/Log/";
    n4Return = ACE_OS::mkdir(strPath.c_str());
    nError = errno;

    if (-1 == n4Return && EEXIST != nError)
    {
        OUR_DEBUG((LM_INFO, "[ServerLogger](%s)CreatePath fail.\n", strPath.c_str()));
    }

    strPath = m_strFileRoot + "/Log/" + m_StrlogType.c_str();
    n4Return = ACE_OS::mkdir(strPath.c_str());
    nError = errno;

    if (-1 == n4Return && EEXIST != nError)
    {
        OUR_DEBUG((LM_INFO, "[ServerLogger](%s)CreatePath fail.\n", strPath.c_str()));
    }

    strPath = m_strFileRoot + "/Log/" + m_StrlogType.c_str() + "/" + m_StrlogName.c_str();
    n4Return = ACE_OS::mkdir(strPath.c_str());
    nError = errno;

    if (-1 == n4Return && EEXIST != nError)
    {
        OUR_DEBUG((LM_INFO, "[ServerLogger](%s)CreatePath fail.\n", strPath.c_str()));
    }
}

//********************************************************

void CFileLogger::Close()
{
    OUR_DEBUG((LM_INFO, "[CFileLogger::Close]Begin.\n"));

    for (auto objLogFile : m_vecLogFileList)
    {
        objLogFile->Close();
    }

    m_vecLogFileList.clear();
    m_nCount = 0;

    OUR_DEBUG((LM_INFO, "[CFileLogger::Close]End.\n"));
}

int CFileLogger::DoLog(int nLogType, _LogBlockInfo* pLogBlockInfo)
{
    //����LogTypeȡ�࣬��õ�ǰ��־ӳ��λ��
    int nIndex = nLogType % m_nCount;

    m_vecLogFileList[nIndex]->doLog(pLogBlockInfo);

    return 0;
}

int CFileLogger::GetLogTypeCount()
{
    return m_nCount;
}

bool CFileLogger::Init()
{
    CXmlOpeation objXmlOpeation;

    //Ĭ�ϵ����ļ������8M
    uint32 u4FileMaxSize            = MAX_BUFF_500;
    uint16 u2LogID                  = 0;
    uint16 u2LogLevel               = 0;
    uint8  u1FileClass              = 0;
    uint8  u1DisPlay                = 0;
    char szFile[MAX_BUFF_1024]      = {'\0'};
    char szFileName[MAX_BUFF_100]   = {'\0'};
    char szServerName[MAX_BUFF_100] = {'\0'};
    char szLogRoot[MAX_BUFF_100]    = {'\0'};
    vector<_Log_File_Info> objvecLogFileInfo;

    Close();

    sprintf_safe(szFile, MAX_BUFF_1024, "%s", FILELOG_CONFIG);

    if(false == objXmlOpeation.Init(szFile))
    {
        OUR_DEBUG((LM_ERROR,"[CFileLogger::Init] Read Configfile[%s] failed\n", szFile));
        return false;
    }

    //�õ�����������
    objXmlOpeation.Read_XML_Data_Single_String("ServerLogHead", "Text", szServerName, MAX_BUFF_100);
    OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]strServerName=%s\n", szServerName));

    //�õ�������־����С
    objXmlOpeation.Read_XML_Data_Single_Uint32("ServerLogHead", "LogFileMaxSize", u4FileMaxSize);

    //�õ�����·��
    objXmlOpeation.Read_XML_Data_Single_String("LogPath", "Path", szLogRoot, MAX_BUFF_100);
    m_strLogRoot = szLogRoot;
    OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]m_strRoot=%s\n", m_strLogRoot.c_str()));

    //�õ���־��������Ϣ����־��Ĵ�С
    objXmlOpeation.Read_XML_Data_Single_Uint32("LogPool", "BlockSize", m_u4BlockSize);

    //�õ���־��������Ϣ�����������־��ĸ���
    objXmlOpeation.Read_XML_Data_Single_Uint32("LogPool", "PoolCount", m_u4PoolCount);

    //�õ���־���еĵ�ǰ��־����
    //�˹��ܸ�л��/ka�̷� �ĺ��뷨����һ�������ٳɶ�ͻ��ۺ���
    objXmlOpeation.Read_XML_Data_Single_Uint16("LogLevel", "CurrLevel", m_u2CurrLogLevel);

    //�������ĸ���
    TiXmlElement* pNextTiXmlElement        = nullptr;
    TiXmlElement* pNextTiXmlElementPos     = nullptr;
    TiXmlElement* pNextTiXmlElementIdx     = nullptr;
    TiXmlElement* pNextTiXmlElementDisplay = nullptr;
    TiXmlElement* pNextTiXmlElementLevel   = nullptr;

    while(true)
    {
        //�õ���־id
        if(true == objXmlOpeation.Read_XML_Data_Multiple_Uint16("LogInfo", "logid", u2LogID, pNextTiXmlElementIdx))
        {
            OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]u2LogID=%d\n", u2LogID));
        }
        else
        {
            break;
        }

        //�õ���־����
        objXmlOpeation.Read_XML_Data_Multiple_String("LogInfo", "logname", szFileName, MAX_BUFF_100, pNextTiXmlElement);
        OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]strFileValue=%s\n", szFileName));

        //�õ���־����
        objXmlOpeation.Read_XML_Data_Multiple_Uint8("LogInfo", "logtype", u1FileClass, pNextTiXmlElementPos);
        OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]u1FileClass=%d\n", u1FileClass));

        //�õ���־�����Դ��0Ϊ������ļ���1Ϊ�������Ļ
        objXmlOpeation.Read_XML_Data_Multiple_Uint8("LogInfo", "Display", u1DisPlay, pNextTiXmlElementDisplay);
        OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]u1DisPlay=%d\n", u1DisPlay));

        //�õ���־��ǰ����
        objXmlOpeation.Read_XML_Data_Multiple_Uint16("LogInfo", "Level", u2LogLevel, pNextTiXmlElementLevel);
        OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]u4LogLevel=%d\n", u2LogLevel));

        //���뻺��
        _Log_File_Info obj_Log_File_Info;
        obj_Log_File_Info.m_u2LogID     = u2LogID;
        obj_Log_File_Info.m_u1FileClass = u1FileClass;
        obj_Log_File_Info.m_u1DisPlay   = u1DisPlay;
        obj_Log_File_Info.m_u2LogLevel  = u2LogLevel;
        sprintf_safe(obj_Log_File_Info.m_szFileName, 100, "%s", szFileName);

        objvecLogFileInfo.push_back(obj_Log_File_Info);
        m_nCount++;
    }

    //���������б�
    for(const auto& objFileInfo : objvecLogFileInfo)
    {
        auto pLogFile = std::make_shared<CLogFile>(m_strLogRoot.c_str(), m_u4BlockSize, u4FileMaxSize);

        pLogFile->SetLoggerName(objFileInfo.m_szFileName);
        pLogFile->SetLoggerID(objFileInfo.m_u2LogID);
        pLogFile->SetLoggerClass(objFileInfo.m_u1FileClass);
        pLogFile->SetLevel(objFileInfo.m_u2LogLevel);
        pLogFile->SetServerName(szServerName);
        pLogFile->SetDisplay(objFileInfo.m_u1DisPlay);
        pLogFile->Init();

        if (false == pLogFile->Run())
        {
            OUR_DEBUG((LM_INFO, "[CFileLogger::Init]Run error.\n"));
        }

        m_vecLogFileList.emplace_back(pLogFile);
    }

    return true;
}

bool CFileLogger::ReSet(uint16 u2CurrLogLevel)
{
    //������־�ȼ�
    m_u2CurrLogLevel = u2CurrLogLevel;
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

uint16 CFileLogger::GetCurrLevel()
{
    return m_u2CurrLogLevel;
}

uint16 CFileLogger::GetLogID(uint16 u2Index)
{
    if(u2Index >= m_nCount)
    {
        return 0;
    }

    return m_vecLogFileList[u2Index]->GetLoggerID();
}

const char* CFileLogger::GetLogInfoByServerName(uint16 u2LogID)
{
    int nIndex = u2LogID % m_nCount;

    return m_vecLogFileList[nIndex]->GetServerName().c_str();
}

const char* CFileLogger::GetLogInfoByLogName(uint16 u2LogID)
{
    int nIndex = u2LogID % m_nCount;

    return m_vecLogFileList[nIndex]->GetLoggerName().c_str();
}

int CFileLogger::GetLogInfoByLogDisplay(uint16 u2LogID)
{
    int nIndex = u2LogID % m_nCount;

    return m_vecLogFileList[nIndex]->GetDisPlay();
}

uint16 CFileLogger::GetLogInfoByLogLevel(uint16 u2LogID)
{
    int nIndex = u2LogID % m_nCount;

    return m_vecLogFileList[nIndex]->GetLevel();
}


