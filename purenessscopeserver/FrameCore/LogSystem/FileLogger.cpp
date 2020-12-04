#include "FileLogger.h"

void CLogFile::Init(const char* pFileRoot, uint32 u4BufferSize, uint32 u4FileMaxSize)
{
    //在这里初始化读取当前文件夹的文件最大序号和文件大小
    ACE_Date_Time dt;
    string strDate = fmt::format("{0:04d}_{1:02d}_{2:02d}_{3}.log", 
        dt.year(),
        dt.month(),
        dt.day(),
        m_u2CurrFileIndex);

    m_u4BufferSize  = u4BufferSize;
    m_u4FileMaxSize = u4FileMaxSize * MAX_BUFF_1024 * MAX_BUFF_1024;
    m_strFileRoot   = pFileRoot;

    //首先判断文件是否存在
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
            //如果文件已存在，判断文件长度是否超过阈值
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
                //找到了不存在的文件
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
    OUR_DEBUG((LM_INFO, "[CLogFile::Close]m_StrlogName=%s.\n", m_strlogName.c_str()));
    m_u4BufferSize = 0;
    m_filestream.close();
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
    //每次自动检测
    CheckTime();

    //记录当前文件写入长度
    m_u4CurrFileSize += pLogBlockInfo->m_u4Length;

    ACE_Date_Time dt;
    string strDate = fmt::format("{0:04d}-{1:02d}-{2:02d} {3:02d}:{4:02d}:{5:02d}",
        dt.year(),
        dt.month(),
        dt.day(),
        dt.hour(),
        dt.minute(),
        dt.second());

    //拼接实际的日志字符串
    string strBuffer = strDate + " " + pLogBlockInfo->m_strBlock;

    if (m_nDisplay == 0 && m_filestream)
    {
        //计入日志
        m_filestream << strBuffer << endl;
    }
    else
    {
        //输出到屏幕
        OUR_DEBUG((LM_INFO, "%s.\n", strBuffer.c_str()));
    }

    //查看是否要发送邮件
    if (pLogBlockInfo->m_u2MailID > 0 && false == SendMail(pLogBlockInfo))
    {
        OUR_DEBUG((LM_INFO, "[CLogFile::doLog](%s)Send mail fail.\n", m_strlogName.c_str()));
    }

    //检查是否超过了文件块，如果超过了，创建一个新日志文件。
    CheckLogFileBlock();

    return 0;
}

bool CLogFile::SendMail(shared_ptr<_LogBlockInfo> pLogBlockInfo, const xmlMails::_Mail* pMailInfo) const
{
    //发送邮件
    const xmlMails::_Mail* pMailAlert = nullptr;

    //如果没有单独配置，则从配置文件中直接获得
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

    string szMailURL = fmt::format("{0}:{1}", pMailAlert->MailUrl, pMailAlert->MailPort);

    //发送smtps邮件
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
    if (nType == 1)  //如果是1，就是运营日志，否则就是错误日志
    {
        m_strlogType = LOGTYPE_OPERATION;
    }
    else
    {
        m_strlogType = LOGTYPE_ERROR;
    }

    OUR_DEBUG((LM_INFO, "[ServerLogger](%d)m_StrlogType=%s.\n", nType, m_strlogType.c_str()));
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
    ACE_Date_Time dt;
    string strDate = fmt::format("{0:04d}_{1:02d}_{2:02d}_{3}.log", 
        dt.year(),
        dt.month(),
        dt.day(),
        m_u2CurrFileIndex);

    dt.update(ACE_OS::gettimeofday());

    CreatePath();       //如果目录不存在则创建目录

    m_strLogTime = fmt::format("{0:04d}-{1:02d}-{2:02d}",
        dt.year(),
        dt.month(),
        dt.day());

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

    //设置当前文件大小是0
    m_u4CurrFileSize = 0;

    OUR_DEBUG((LM_INFO, "[ServerLogger]Create file ok[%s].\n", strLogName.c_str()));
    return true;
}

void CLogFile::CheckTime()
{
    ACE_Date_Time dt;

    dt.update(ACE_OS::gettimeofday());

    string strDate = fmt::format("{0:04d}-{1:02d}-{2:02d}",
        dt.year(),
        dt.month(),
        dt.day());

    if (strDate != m_strLogTime && false == Run())
    {
        OUR_DEBUG((LM_INFO, "[ServerLogger](%s)Run fail.\n", m_strlogName.c_str()));
    }
}

void CLogFile::CheckLogFileBlock()
{
    //查看文件是否超过了最大限制
    if (m_u4CurrFileSize >= m_u4FileMaxSize)
    {
        //创建新文件
        m_u2CurrFileIndex++;

        if (false == Run())
        {
            OUR_DEBUG((LM_INFO, "[ServerLogger](%s)Run fail.\n", m_strlogName.c_str()));
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

    strPath = m_strFileRoot + "/Log/" + m_strlogType.c_str();
    n4Return = ACE_OS::mkdir(strPath.c_str());
    nError = errno;

    if (-1 == n4Return && EEXIST != nError)
    {
        OUR_DEBUG((LM_INFO, "[ServerLogger](%s)CreatePath fail.\n", strPath.c_str()));
    }

    strPath = m_strFileRoot + "/Log/" + m_strlogType + "/" + m_strlogName;
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

    for_each(m_mapLogFileList.begin(), m_mapLogFileList.end(), [](const std::pair<uint16, shared_ptr<CLogFile>>& iter) {
        //关闭写入文件
        iter.second->Close();
        });

    m_mapLogFileList.clear();
    m_nCount = 0;

    OUR_DEBUG((LM_INFO, "[CFileLogger::Close]End.\n"));
}

int CFileLogger::DoLog(uint16 nLogType, shared_ptr<_LogBlockInfo> pLogBlockInfo)
{
    //根据LogType取余，获得当前日志映射位置
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

    //默认单个文件最大是8M
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
        OUR_DEBUG((LM_ERROR,"[CFileLogger::Init] Read Configfile[%s] failed\n", strFile.c_str()));
        return false;
    }

    //得到服务器名称
    objXmlOpeation.Read_XML_Data_Single_String("ServerLogHead", "Text", strServerName);
    OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]strServerName=%s\n", strServerName.c_str()));

    //得到单个日志最大大小
    objXmlOpeation.Read_XML_Data_Single_Uint32("ServerLogHead", "LogFileMaxSize", u4FileMaxSize);

    //得到绝对路径
    objXmlOpeation.Read_XML_Data_Single_String("LogPath", "Path", m_strLogRoot);
    OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]m_strRoot=%s\n", m_strLogRoot.c_str()));

    //得到日志池配置信息，日志块的大小
    objXmlOpeation.Read_XML_Data_Single_Uint32("LogPool", "BlockSize", m_u4BlockSize);

    //得到日志池配置信息，缓冲池中日志块的个数
    objXmlOpeation.Read_XML_Data_Single_Uint32("LogPool", "PoolCount", m_u4PoolCount);

    //得到日志池中的当前日志级别
    //此功能感谢宇/ka程枫 的好想法，赞一个，积少成多就会汇聚洪流
    objXmlOpeation.Read_XML_Data_Single_Uint16("LogLevel", "CurrLevel", m_u2CurrLogLevel);

    //添加子类的个数
    TiXmlElement* pNextTiXmlElement        = nullptr;
    TiXmlElement* pNextTiXmlElementPos     = nullptr;
    TiXmlElement* pNextTiXmlElementIdx     = nullptr;
    TiXmlElement* pNextTiXmlElementDisplay = nullptr;
    TiXmlElement* pNextTiXmlElementLevel   = nullptr;

    while(true)
    {
        //得到日志id
        if(true == objXmlOpeation.Read_XML_Data_Multiple_Uint16("LogInfo", "logid", u2LogID, pNextTiXmlElementIdx))
        {
            OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]u2LogID=%d\n", u2LogID));
        }
        else
        {
            break;
        }

        //得到日志名称
        objXmlOpeation.Read_XML_Data_Multiple_String("LogInfo", "logname", strFileName, pNextTiXmlElement);
        OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]strFileValue=%s\n", strFileName.c_str()));

        //得到日志类型
        objXmlOpeation.Read_XML_Data_Multiple_Uint8("LogInfo", "logtype", u1FileClass, pNextTiXmlElementPos);
        OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]u1FileClass=%d\n", u1FileClass));

        //得到日志输出来源，0为输出到文件，1为输出到屏幕
        objXmlOpeation.Read_XML_Data_Multiple_Uint8("LogInfo", "Display", u1DisPlay, pNextTiXmlElementDisplay);
        OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]u1DisPlay=%d\n", u1DisPlay));

        //得到日志当前级别
        objXmlOpeation.Read_XML_Data_Multiple_Uint16("LogInfo", "Level", u2LogLevel, pNextTiXmlElementLevel);
        OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]u4LogLevel=%d\n", u2LogLevel));

        //加入缓冲
        _Log_File_Info obj_Log_File_Info;
        obj_Log_File_Info.m_u2LogID     = u2LogID;
        obj_Log_File_Info.m_u1FileClass = u1FileClass;
        obj_Log_File_Info.m_u1DisPlay   = u1DisPlay;
        obj_Log_File_Info.m_u2LogLevel  = u2LogLevel;
        obj_Log_File_Info.m_strFileName = strFileName;

        objvecLogFileInfo.push_back(obj_Log_File_Info);
        m_nCount++;
    }

    //创建对象列表
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
            OUR_DEBUG((LM_INFO, "[CFileLogger::Init]Run error.\n"));
        }

        m_mapLogFileList[objFileInfo.m_u2LogID] = pLogFile;
    }

    return true;
}

bool CFileLogger::ReSet(uint16 u2CurrLogLevel)
{
    //重置日志等级
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


