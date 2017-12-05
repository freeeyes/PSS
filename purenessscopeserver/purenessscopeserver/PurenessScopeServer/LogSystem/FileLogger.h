#ifndef _FILELOGGER_H
#define _FILELOGGER_H

#include "ace/FILE_Addr.h"
#include "ace/FILE_Connector.h"
#include "ace/FILE_IO.h"
#include "ace/Date_Time.h"

#include "define.h"
#include "MainConfig.h"
#include "ILogObject.h"
#include "smtp.h"

#define MAX_CMD_NUM   100
#define MAX_TIME_SIZE 100

//定义Log所属类型
#define LOGTYPE_OPERATION "Operation"
#define LOGTYPE_ERROR     "ServerError"

#define FILELOG_CONFIG    "logger.xml"

//从XML读取的日志文件信息
struct _Log_File_Info
{
    uint16 m_u2LogID;
    uint8  m_u1FileClass;
    uint8  m_u1DisPlay;
    uint16 m_u2LogLevel;
    char   m_szFileName[MAX_BUFF_100];

    _Log_File_Info()
    {
        m_u2LogID       = 0;
        m_u1FileClass   = 0;
        m_u1DisPlay     = 0;
        m_u2LogLevel    = 0;
        m_szFileName[0] = '\0';
    }
};

//单元模式日志类
class CLogFile
{
public:
    CLogFile(const char* pFileRoot, uint32 u4BufferSize)
    {
        m_u2LogID           = 0;
        m_nDisplay          = 0;
        m_StrServerName     = "";
        m_u2Level           = 0;
        m_StrlogType        = "ServerError";
        m_pBuffer           = new char[u4BufferSize];   //这里是用于日志拼接时间所用
        m_u4BufferSize      = u4BufferSize;
        m_szLogTime[0]      = '\0';
        sprintf_safe(m_szFileRoot, MAX_BUFF_100, "%s", pFileRoot);
    }

    virtual ~CLogFile()
    {
        OUR_DEBUG((LM_INFO,"[CLogFile::~CLogFile].\n"));
        SAFE_DELETE_ARRAY(m_pBuffer);
        m_u4BufferSize = 0;
        m_File.close();
        OUR_DEBUG((LM_INFO,"[CLogFile::~CLogFile] End.\n"));
    }

    //拷贝构造
    CLogFile(CLogFile& ar)
    {
        this->SetLoggerID(ar.GetLoggerID());
        this->SetLevel(ar.GetLevel());
        this->SetLoggerName(ar.GetLoggerName().c_str());
        this->SetServerName(ar.GetServerName().c_str());
        this->SetLoggerClass(ar.GetLoggerClass());
        this->SetBufferSize(ar.GetBufferSize());
        this->SetLogTime(ar.GetLogTime());
        this->SetDisplay(ar.GetDisPlay());
        this->SetDisplay(ar.GetDisPlay());
        this->SetFileRoot(ar.GetFileRoot());
        this->SetFileAddr(ar.GetFileAddr());
        this->SetConnector(ar.GetConnector());
        this->SetFileIO(ar.GetFileIO());
    }

    CLogFile& operator = (CLogFile& ar)
    {
        this->SetLoggerID(ar.GetLoggerID());
        this->SetLevel(ar.GetLevel());
        this->SetLoggerName(ar.GetLoggerName().c_str());
        this->SetServerName(ar.GetServerName().c_str());
        this->SetLoggerClass(ar.GetLoggerClass());
        this->SetBufferSize(ar.GetBufferSize());
        this->SetLogTime(ar.GetLogTime());
        this->SetDisplay(ar.GetDisPlay());
        this->SetFileRoot(ar.GetFileRoot());
        this->SetFileAddr(ar.GetFileAddr());
        this->SetConnector(ar.GetConnector());
        this->SetFileIO(ar.GetFileIO());
        return *this;
    }

    void SetFileRoot(const char* pFileRoot)
    {
        sprintf_safe(m_szFileRoot, MAX_BUFF_100, "%s", pFileRoot);
    }

    char* GetFileRoot()
    {
        return m_szFileRoot;
    }

    void SetFileAddr(ACE_FILE_Addr& objFileAddr)
    {
        m_FileAddr = objFileAddr;
    }

    ACE_FILE_Addr& GetFileAddr()
    {
        return m_FileAddr;
    }

    void SetConnector(ACE_FILE_Connector& objConnector)
    {
        m_Connector = objConnector;
    }

    ACE_FILE_Connector& GetConnector()
    {
        return m_Connector;
    }

    void SetFileIO(ACE_FILE_IO& objFile)
    {
        m_File = objFile;
    }

    ACE_FILE_IO& GetFileIO()
    {
        return m_File;
    }

    void SetLogTime(const char* pLogTime)
    {
        sprintf_safe(m_szLogTime, MAX_TIME_SIZE, "%s", pLogTime);
    }

    char* GetLogTime()
    {
        return m_szLogTime;
    }

    void SetBufferSize(uint32 u4BufferSize)
    {
        SAFE_DELETE_ARRAY(m_pBuffer);
        m_pBuffer = new char[u4BufferSize];   //这里是用于日志拼接时间所用
        m_u4BufferSize = u4BufferSize;
    }

    uint32 GetBufferSize()
    {
        return m_u4BufferSize;
    }

    virtual int doLog(_LogBlockInfo* pLogBlockInfo)
    {
        //每次自动检测
        CheckTime();

        ACE_Date_Time dt;
        char szDateBuff[MAX_TIME_SIZE] = {'\0'};

        sprintf_safe(szDateBuff, MAX_TIME_SIZE, "%04d-%02d-%02d %02d:%02d:%02d%02d,", dt.year(), dt.month(), dt.day(), dt.hour(), dt.minute(), dt.second(), dt.microsec()/10000);

        //拼接实际的日志字符串
        sprintf_safe(m_pBuffer, m_u4BufferSize, "%s %s\n", szDateBuff, pLogBlockInfo->m_pBlock);

        if(m_nDisplay == 0)
        {
            //计入日志
            int nLen = (int)m_File.send(m_pBuffer, strlen(m_pBuffer));

            if(nLen != (int)strlen(m_pBuffer))
            {
                OUR_DEBUG((LM_INFO,"[%s]Write error[%s].\n", m_StrlogName.c_str(), m_pBuffer));
            }
        }
        else
        {
            //输出到屏幕
            OUR_DEBUG((LM_INFO,"%s.\n", m_pBuffer));
        }

        //查看是否要发送邮件
        if(pLogBlockInfo->m_u4MailID > 0)
        {
            if (false == SendMail(pLogBlockInfo))
            {
                OUR_DEBUG((LM_INFO, "[CLogFile::doLog](%s)Send mail fail.\n", m_StrlogName.c_str()));
            }
        }

        return 0;
    }

    bool SendMail(_LogBlockInfo* pLogBlockInfo)
    {
        //发送邮件
        _MailAlert* pMailAlert = App_MainConfig::instance()->GetMailAlert(pLogBlockInfo->m_u4MailID);

        if(NULL == pMailAlert)
        {
            OUR_DEBUG((LM_ERROR, "[CLogFile::SendMail]MailID(%d) is no find.\n", pLogBlockInfo->m_u4MailID));
            return false;
        }

        unsigned char* pMail = (unsigned char* )ACE_OS::calloc(1, 1);

        int nRet = 0;
        nRet = mailText(&pMail,
                        (const unsigned char*)pMailAlert->m_szFromMailAddr,
                        (const unsigned char*)pMailAlert->m_szToMailAddr,
                        (const unsigned char*)pLogBlockInfo->m_szMailTitle,
                        (const unsigned char*)pLogBlockInfo->m_pBlock);

        if(nRet != 0)
        {
            OUR_DEBUG((LM_ERROR, "[CLogFile::SendMail]MailID(%d) mailText error.\n", pLogBlockInfo->m_u4MailID));
            free(pMail);
            return false;
        }

        nRet = mailEnd(&pMail);

        if(nRet != 0)
        {
            OUR_DEBUG((LM_ERROR, "[CLogFile::SendMail]MailID(%d) mailEnd error.\n", pLogBlockInfo->m_u4MailID));
            free(pMail);
            return false;
        }

        ACE_HANDLE fd;

        nRet = connectSmtp(fd, (const unsigned char*)pMailAlert->m_szMailUrl,
                           pMailAlert->m_u4MailPort);

        if(nRet != 0)
        {
            OUR_DEBUG((LM_ERROR, "[CLogFile::SendMail]MailID(%d) connectSmtp error.\n", pLogBlockInfo->m_u4MailID));
            free(pMail);
            return false;
        }

        if (fd == ACE_INVALID_HANDLE)
        {
            OUR_DEBUG((LM_ERROR, "[CLogFile::SendMail]MailID(%d) connectSmtp error.\n", pLogBlockInfo->m_u4MailID));
            free(pMail);
            return false;
        }

        nRet = authEmail(fd,
                         (const unsigned char*)pMailAlert->m_szFromMailAddr,
                         (const unsigned char*)pMailAlert->m_szMailPass);

        if(nRet != 0)
        {
            OUR_DEBUG((LM_ERROR, "[CLogFile::SendMail]MailID(%d) authEmail error.\n", pLogBlockInfo->m_u4MailID));
            free(pMail);
            ACE_OS::close(fd);
            return false;
        }

        nRet = sendEmail(fd, (const unsigned char*)pMailAlert->m_szFromMailAddr,
                         (const unsigned char*)pMailAlert->m_szToMailAddr,
                         (const unsigned char*)pMail,
                         (const int)strlen((const char*)pMail));

        if(nRet != 0)
        {
            OUR_DEBUG((LM_ERROR, "[CLogFile::SendMail]MailID(%d) sendEmail error.\n", pLogBlockInfo->m_u4MailID));
            free(pMail);
            ACE_OS::close(fd);
            return false;
        }

        free(pMail);
        ACE_OS::close(fd);
        return true;
    }

    ACE_TString& GetLoggerName()
    {
        return m_StrlogName;
    }

    ACE_TString& GetServerName()
    {
        return m_StrServerName;
    }

    int GetDisPlay()
    {
        return m_nDisplay;
    }

    void SetDisplay(int nDisplay)
    {
        if(nDisplay == 0)
        {
            m_nDisplay = 0;
        }
        else
        {
            m_nDisplay = 1;
        }
    }

    void SetLoggerClass(int nType)
    {
        if(nType == 1)  //如果是1，就是运营日志，否则就是错误日志
        {
            m_StrlogType = LOGTYPE_OPERATION;
        }
        else
        {
            m_StrlogType = LOGTYPE_ERROR;
        }

        OUR_DEBUG((LM_INFO,"[ServerLogger](%d)m_StrlogType=%s.\n", nType, m_StrlogType.c_str()));
    }

    int GetLoggerClass()
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

    void SetLoggerName(const char* szLoggerName)
    {
        m_StrlogName = szLoggerName;
    }

    void SetLoggerID(uint16 u2LogID)
    {
        m_u2LogID = u2LogID;
    }

    uint16 GetLoggerID()
    {
        return m_u2LogID;
    }

    void SetLevel(uint16 u2Level)
    {
        m_u2Level = u2Level;
    }

    uint16 GetLevel()
    {
        return m_u2Level;
    }

    void SetServerName(const char* szServerName)
    {
        m_StrServerName = szServerName;
    }

    bool Run()
    {
        ACE_Date_Time dt;
        char szDateBuff[MAX_TIME_SIZE] = {'\0'};

        dt.update(ACE_OS::gettimeofday());

        CreatePath();       //如果目录不存在则创建目录

        sprintf_safe(szDateBuff, MAX_TIME_SIZE, "_%04d-%02d-%02d.log", dt.year(), dt.month(), dt.day());
        sprintf_safe(m_szLogTime, MAX_TIME_SIZE, "%04d-%02d-%02d", dt.year(), dt.month(), dt.day());

        ACE_TString strLogModulePath = m_szFileRoot;
        ACE_TString strLogName       = strLogModulePath + "/Log/" + m_StrlogType + "/" + m_StrlogName + "/" + m_StrServerName + m_StrlogName + szDateBuff;

        m_File.close();

        m_FileAddr.set((ACE_TCHAR*)strLogName.c_str());

        if(m_Connector.connect(m_File, m_FileAddr, 0, ACE_Addr::sap_any, 0, O_WRONLY|O_CREAT|O_APPEND) == -1)
        {
            OUR_DEBUG((LM_INFO,"[ServerLogger]Create file error[%s].\n", strLogName.c_str()));
            return false;
        }

        OUR_DEBUG((LM_INFO,"[ServerLogger]Create file ok[%s].\n", strLogName.c_str()));
        //m_File.truncate(0);
        return true;
    }

    void CheckTime()
    {
        ACE_Date_Time dt;
        dt.update(ACE_OS::gettimeofday());
        char szDate[MAX_TIME_SIZE] = {'\0'};
        sprintf_safe(szDate, MAX_TIME_SIZE, "%04d-%02d-%02d", dt.year(), dt.month(), dt.day());

        if(ACE_OS::strcmp(szDate, m_szLogTime) != 0)
        {
            if (false == Run())
            {
                OUR_DEBUG((LM_INFO, "[ServerLogger](%s)Run fail.\n", m_StrlogName.c_str()));
            }
        }
    }

    void CreatePath()
    {
        int n4Return = -1;
        char szPath[MAX_CMD_NUM] = {'\0'};
        sprintf_safe(szPath, MAX_CMD_NUM, "%s/Log/", m_szFileRoot);
        n4Return = ACE_OS::mkdir(szPath);

        if(-1 == n4Return)
        {
            OUR_DEBUG((LM_INFO, "[ServerLogger](%s)CreatePath fail.\n", szPath));
        }

        sprintf_safe(szPath, MAX_CMD_NUM, "%s/Log/%s/", m_szFileRoot, m_StrlogType.c_str());
        n4Return = ACE_OS::mkdir(szPath);

        if(-1 == n4Return)
        {
            OUR_DEBUG((LM_INFO, "[ServerLogger](%s)CreatePath fail.\n", szPath));
        }

        sprintf_safe(szPath, MAX_CMD_NUM, "%s/Log/%s/%s", m_szFileRoot, m_StrlogType.c_str(), m_StrlogName.c_str());
        n4Return = ACE_OS::mkdir(szPath);

        if(-1 == n4Return)
        {
            OUR_DEBUG((LM_INFO, "[ServerLogger](%s)CreatePath fail.\n", szPath));
        }
    }

private:
    uint32              m_u4BufferSize;               //日志缓冲最大大小
    uint16              m_u2LogID;                    //日志编号
    uint16              m_u2Level;                    //日志等级
    int                 m_nDisplay;                   //显示还是记录文件
    char*               m_pBuffer;                    //日志缓冲指针
    char                m_szLogTime[MAX_TIME_SIZE];   //Log当前时间
    char                m_szFileRoot[MAX_BUFF_100];   //路径的主目录
    ACE_TString         m_StrlogName;                 //模块名字
    ACE_TString         m_StrlogType;                 //日志类型
    ACE_TString         m_StrServerName;              //服务器前缀
    ACE_FILE_Connector  m_Connector;                  //I/O操作连接器
    ACE_FILE_IO         m_File;
    ACE_FILE_Addr       m_FileAddr;
};

class CFileLogger : public CServerLogger
{
public:
    CFileLogger();
    ~CFileLogger();

    int DoLog(int nLogType, _LogBlockInfo* pLogBlockInfo);
    int GetLogTypeCount();

    bool Init();
    bool ReSet(uint32 u4CurrLogLevel);
    void Close();

    uint32 GetBlockSize();
    uint32 GetPoolCount();

    uint32 GetCurrLevel();

    uint16 GetLogID(uint16 u2Index);
    char*  GetLogInfoByServerName(uint16 u2LogID);
    char*  GetLogInfoByLogName(uint16 u2LogID);
    int    GetLogInfoByLogDisplay(uint16 u2LogID);
    uint16 GetLogInfoByLogLevel(uint16 u2LogID);

private:
    CLogFile**                     m_pLogFileList;
    char                           m_szLogRoot[MAX_BUFF_100];
    int                            m_nCount;

    uint32                         m_u4BlockSize;
    uint32                         m_u4PoolCount;
    uint32                         m_u4CurrLogLevel;
};

#endif
