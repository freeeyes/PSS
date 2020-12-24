#include "FileLogger.h"

void CLogFile::Init(const char* pFileRoot, uint32 u4BufferSize, uint32 u4FileMaxSize)
{
    //�������ʼ����ȡ��ǰ�ļ��е��ļ������ź��ļ���С
    ACE_Date_Time dt;
    string strDate = fmt::format("{0:04d}_{1:02d}_{2:02d}_{3}.log", 
        dt.year(),
        dt.month(),
        dt.day(),
        m_u2CurrFileIndex);

    m_u4BufferSize  = u4BufferSize;
    m_u4FileMaxSize = u4FileMaxSize * MAX_BUFF_1024 * MAX_BUFF_1024;
    m_strFileRoot   = pFileRoot;

    //�����ж��ļ��Ƿ����
    while (true)
    {
        string strLogModulePath = m_strFileRoot.c_str();
        string strLogName = strLogModulePath + "/Log/"
            + m_strlogType + "/" 
            + m_strlogName + "/" 
            + m_strServerName 
            + m_strlogName 
            + strDate.c_str();

        auto nRet = ACE_OS::access(strLogName.c_str(), R_OK);

        if (0 == nRet)
        {
            //����ļ��Ѵ��ڣ��ж��ļ������Ƿ񳬹���ֵ
            FILE* fp = ACE_OS::fopen(strLogName.c_str(), "r");

            if (nullptr == fp)
            {
                PSS_LOGGER_DEBUG("[CLogFile::Init]File({0}) fopen error.", strLogName);
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
                PSS_LOGGER_DEBUG("[CLogFile::Init]File({0}) access error({1}).", strLogName, nError);
                return;
            }
        }
    }
}

void CLogFile::Close()
{
    PSS_LOGGER_DEBUG("[CLogFile::Close]m_StrlogName={0}.", m_strlogName);
    m_u4BufferSize = 0;
    m_filestream.close();
    PSS_LOGGER_DEBUG("[CLogFile::Close] End.");
}

void CLogFile::SetFileRoot(const char* pFileRoot)
{
    m_strFileRoot = pFileRoot;
}

const char* CLogFile::GetFileRoot() const
{
    return m_strFileRoot.c_str();
}

void CLogFile::SetLogTime(const char* pLogTime)
{
    m_strLogTime = pLogTime;
}

const char* CLogFile::GetLogTime() const
{
    return m_strLogTime.c_str();
}

void CLogFile::SetBufferSize(uint32 u4BufferSize) const
{
    ACE_UNUSED_ARG(u4BufferSize);
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

int CLogFile::doLog(shared_ptr<_LogBlockInfo> pLogBlockInfo)
{
    //ÿ���Զ����
    CheckTime();

    //��¼��ǰ�ļ�д�볤��
    m_u4CurrFileSize += pLogBlockInfo->m_u4Length;

    ACE_Date_Time dt;
    string strDate = fmt::format("{0:04d}-{1:02d}-{2:02d} {3:02d}:{4:02d}:{5:02d}",
        dt.year(),
        dt.month(),
        dt.day(),
        dt.hour(),
        dt.minute(),
        dt.second());

    //ƴ��ʵ�ʵ���־�ַ���
    string strBuffer = strDate + " " + pLogBlockInfo->m_strBlock;

    if (m_nDisplay == 0 && m_filestream)
    {
        //������־
        m_filestream << strBuffer << endl;
    }
    else
    {
        //�������Ļ
        PSS_LOGGER_DEBUG("{0}.", strBuffer);
    }

    //�鿴�Ƿ�Ҫ�����ʼ�
    if (pLogBlockInfo->m_u2MailID > 0 && false == SendMail(pLogBlockInfo))
    {
        PSS_LOGGER_DEBUG("[CLogFile::doLog]({0})Send mail fail.", m_strlogName);
    }

    //����Ƿ񳬹����ļ��飬��������ˣ�����һ������־�ļ���
    CheckLogFileBlock();

    return 0;
}

bool CLogFile::SendMail(shared_ptr<_LogBlockInfo> pLogBlockInfo, const xmlMails::_Mail* pMailInfo) const
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
        PSS_LOGGER_DEBUG("[CLogFile::SendMail]MailID({0}) is no find.", pLogBlockInfo->m_u2MailID);
        return false;
    }

    string szMailURL = fmt::format("{0}:{1}", pMailAlert->MailUrl, pMailAlert->MailPort);

    //����smtps�ʼ�
    int nRet = Send_Mail_From_Ssl(pMailAlert->fromMailAddr.c_str(),
        pMailAlert->MailPass.c_str(),
        pMailAlert->fromMailAddr.c_str(),
        pMailAlert->toMailAddr.c_str(),
        szMailURL.c_str(),
        pLogBlockInfo->m_strMailTitle.c_str(),
        pLogBlockInfo->m_strBlock.c_str());

    if (0 != nRet)
    {
        return false;
    }
    else
    {
        return true;
    }
}

string CLogFile::GetLoggerName() const
{
    return m_strlogName;
}

string CLogFile::GetServerName() const
{
    return m_strServerName;
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
        m_strlogType = LOGTYPE_OPERATION;
    }
    else
    {
        m_strlogType = LOGTYPE_ERROR;
    }

    PSS_LOGGER_DEBUG("[ServerLogger]({0})m_StrlogType={1}.", nType, m_strlogType);
}

int CLogFile::GetLoggerClass() const
{
    if (LOGTYPE_OPERATION == m_strlogType)
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
    m_strlogName = szLoggerName;
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
    m_strServerName = szServerName;
}

bool CLogFile::Run()
{
    auto tvNow = CTimeStamp::Get_Time_Stamp();
    string strDate = fmt::format("{0:04d}_{1:02d}_{2:02d}_{3}.log", 
        CTimeStamp::Get_Date_Year(tvNow),
        CTimeStamp::Get_Date_Month(tvNow),
        CTimeStamp::Get_Date_Day(tvNow),
        m_u2CurrFileIndex);

    CreatePath();       //���Ŀ¼�������򴴽�Ŀ¼

    m_strLogTime = fmt::format("{0:04d}-{1:02d}-{2:02d}",
        CTimeStamp::Get_Date_Year(tvNow),
        CTimeStamp::Get_Date_Month(tvNow),
        CTimeStamp::Get_Date_Day(tvNow));

    string strLogModulePath = m_strFileRoot.c_str();
    string strLogName = strLogModulePath
        + "/Log/" 
        + m_strlogType 
        + "/" + m_strlogName 
        + "/" + m_strServerName 
        + m_strlogName 
        + strDate.c_str();

    if (m_filestream)
    {
        m_filestream.close();
    }

    m_filestream.open(strLogName.c_str(), ios::app);

    //���õ�ǰ�ļ���С��0
    m_u4CurrFileSize = 0;

    PSS_LOGGER_DEBUG("[ServerLogger]Create file ok[{0}].", strLogName);
    return true;
}

void CLogFile::CheckTime()
{
    auto tvNow = CTimeStamp::Get_Time_Stamp();
    string strDate = fmt::format("{0:04d}-{1:02d}-{2:02d}",
        CTimeStamp::Get_Date_Year(tvNow),
        CTimeStamp::Get_Date_Month(tvNow),
        CTimeStamp::Get_Date_Day(tvNow));

    if (strDate != m_strLogTime && false == Run())
    {
        PSS_LOGGER_DEBUG("[ServerLogger]({0})Run fail.", m_strlogName);
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
            PSS_LOGGER_DEBUG("[ServerLogger]({0})Run fail.", m_strlogName);
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
        PSS_LOGGER_DEBUG("[ServerLogger]({0})CreatePath fail.", strPath);
    }

    strPath = m_strFileRoot + "/Log/" + m_strlogType.c_str();
    n4Return = ACE_OS::mkdir(strPath.c_str());
    nError = errno;

    if (-1 == n4Return && EEXIST != nError)
    {
        PSS_LOGGER_DEBUG("[ServerLogger]({0})CreatePath fail.", strPath);
    }

    strPath = m_strFileRoot + "/Log/" + m_strlogType + "/" + m_strlogName;
    n4Return = ACE_OS::mkdir(strPath.c_str());
    nError = errno;

    if (-1 == n4Return && EEXIST != nError)
    {
        PSS_LOGGER_DEBUG("[ServerLogger]({0})CreatePath fail.", strPath);
    }
}

//********************************************************

void CFileLogger::Close()
{
    PSS_LOGGER_DEBUG("[CFileLogger::Close]Begin.");

    for_each(m_mapLogFileList.begin(), m_mapLogFileList.end(), [](const std::pair<uint16, shared_ptr<CLogFile>>& iter) {
        //�ر�д���ļ�
        iter.second->Close();
        });

    m_mapLogFileList.clear();
    m_nCount = 0;

    PSS_LOGGER_DEBUG("[CFileLogger::Close]End.");
}

int CFileLogger::DoLog(uint16 nLogType, shared_ptr<_LogBlockInfo> pLogBlockInfo)
{
    //����LogTypeȡ�࣬��õ�ǰ��־ӳ��λ��
    if (nullptr != m_mapLogFileList[nLogType])
    {
        m_mapLogFileList[nLogType]->doLog(pLogBlockInfo);
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

    //Ĭ�ϵ����ļ������8M
    uint32 u4FileMaxSize            = MAX_BUFF_500;
    uint16 u2LogID                  = 0;
    uint16 u2LogLevel               = 0;
    uint8  u1FileClass              = 0;
    uint8  u1DisPlay                = 0;
    string strFile;
    string strFileName;
    string strServerName;
    vector<_Log_File_Info> objvecLogFileInfo;

    Close();

    strFile = FILELOG_CONFIG;

    if(false == objXmlOpeation.Init(strFile.c_str()))
    {
        PSS_LOGGER_DEBUG("[CFileLogger::Init] Read Configfile[{0}] failed.", strFile);
        return false;
    }

    //�õ�����������
    objXmlOpeation.Read_XML_Data_Single_String("ServerLogHead", "Text", strServerName);
    PSS_LOGGER_DEBUG("[CFileLogger::readConfig]strServerName={0}.", strServerName);

    //�õ�������־����С
    objXmlOpeation.Read_XML_Data_Single_Uint32("ServerLogHead", "LogFileMaxSize", u4FileMaxSize);

    //�õ�����·��
    objXmlOpeation.Read_XML_Data_Single_String("LogPath", "Path", m_strLogRoot);
    PSS_LOGGER_DEBUG("[CFileLogger::readConfig]m_strRoot={0}.", m_strLogRoot);

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
            PSS_LOGGER_DEBUG("[CFileLogger::readConfig]u2LogID={0}.", u2LogID);
        }
        else
        {
            break;
        }

        //�õ���־����
        objXmlOpeation.Read_XML_Data_Multiple_String("LogInfo", "logname", strFileName, pNextTiXmlElement);
        PSS_LOGGER_DEBUG("[CFileLogger::readConfig]strFileValue={0}.", strFileName);

        //�õ���־����
        objXmlOpeation.Read_XML_Data_Multiple_Uint8("LogInfo", "logtype", u1FileClass, pNextTiXmlElementPos);
        PSS_LOGGER_DEBUG("[CFileLogger::readConfig]u1FileClass={0}.", u1FileClass);

        //�õ���־�����Դ��0Ϊ������ļ���1Ϊ�������Ļ
        objXmlOpeation.Read_XML_Data_Multiple_Uint8("LogInfo", "Display", u1DisPlay, pNextTiXmlElementDisplay);
        PSS_LOGGER_DEBUG("[CFileLogger::readConfig]u1DisPlay={0}.", u1DisPlay);

        //�õ���־��ǰ����
        objXmlOpeation.Read_XML_Data_Multiple_Uint16("LogInfo", "Level", u2LogLevel, pNextTiXmlElementLevel);
        PSS_LOGGER_DEBUG("[CFileLogger::readConfig]u4LogLevel={0}.", u2LogLevel);

        //���뻺��
        _Log_File_Info obj_Log_File_Info;
        obj_Log_File_Info.m_u2LogID     = u2LogID;
        obj_Log_File_Info.m_u1FileClass = u1FileClass;
        obj_Log_File_Info.m_u1DisPlay   = u1DisPlay;
        obj_Log_File_Info.m_u2LogLevel  = u2LogLevel;
        obj_Log_File_Info.m_strFileName = strFileName;

        objvecLogFileInfo.push_back(obj_Log_File_Info);
        m_nCount++;
    }

    //���������б�
    for(const auto& objFileInfo : objvecLogFileInfo)
    {
        auto pLogFile = std::make_shared<CLogFile>();

        pLogFile->SetLoggerName(objFileInfo.m_strFileName.c_str());
        pLogFile->SetLoggerID(objFileInfo.m_u2LogID);
        pLogFile->SetLoggerClass(objFileInfo.m_u1FileClass);
        pLogFile->SetLevel(objFileInfo.m_u2LogLevel);
        pLogFile->SetServerName(strServerName.c_str());
        pLogFile->SetDisplay(objFileInfo.m_u1DisPlay);
        pLogFile->Init(m_strLogRoot.c_str(), m_u4BlockSize, u4FileMaxSize);

        if (false == pLogFile->Run())
        {
            PSS_LOGGER_DEBUG("[CFileLogger::Init]Run error.");
        }

        m_mapLogFileList[objFileInfo.m_u2LogID] = pLogFile;
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

uint16 CFileLogger::GetLogID(uint16 u2LogID)
{
    auto f = m_mapLogFileList.find(u2LogID);
    if (m_mapLogFileList.end() == f)
    {
        return 0;
    }

    return m_mapLogFileList[u2LogID]->GetLoggerID();
}

const char* CFileLogger::GetLogInfoByServerName(uint16 u2LogID)
{
    auto f = m_mapLogFileList.find(u2LogID);
    if (m_mapLogFileList.end() == f)
    {
        return nullptr;
    }

    return m_mapLogFileList[u2LogID]->GetServerName().c_str();
}

const char* CFileLogger::GetLogInfoByLogName(uint16 u2LogID)
{
    auto f = m_mapLogFileList.find(u2LogID);
    if (m_mapLogFileList.end() == f)
    {
        return nullptr;
    }

    return m_mapLogFileList[u2LogID]->GetLoggerName().c_str();
}

int CFileLogger::GetLogInfoByLogDisplay(uint16 u2LogID)
{
    auto f = m_mapLogFileList.find(u2LogID);
    if (m_mapLogFileList.end() == f)
    {
        return 0;
    }

    return m_mapLogFileList[u2LogID]->GetDisPlay();
}

uint16 CFileLogger::GetLogInfoByLogLevel(uint16 u2LogID)
{
    auto f = m_mapLogFileList.find(u2LogID);
    if (m_mapLogFileList.end() == f)
    {
        return 0;
    }

    return m_mapLogFileList[u2LogID]->GetLevel();
}


