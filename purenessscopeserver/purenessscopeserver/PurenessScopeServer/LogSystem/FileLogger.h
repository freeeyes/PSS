#ifndef _FILELOGGER_H
#define _FILELOGGER_H

#include "ace/FILE_Addr.h"
#include "ace/FILE_Connector.h"
#include "ace/FILE_IO.h"
#include "ace/Date_Time.h"

#include "define.h"
#include "ILogObject.h"
#include "smtp.h"
#include "XmlConfig.h"

#define MAX_CMD_NUM   100
#define MAX_TIME_SIZE 100

//定义Log所属类型
#define LOGTYPE_OPERATION "Operation"
#define LOGTYPE_ERROR     "ServerError"

#define FILELOG_CONFIG    "logger.xml"

//从XML读取的日志文件信息
class _Log_File_Info
{
public:
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
    CLogFile(const char* pFileRoot, uint32 u4BufferSize, uint32 u4FileMaxSize);

    void Init();

    virtual ~CLogFile();

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
        this->SetCurrFileIndex(ar.GetCurrFileIndex());
        this->SetFileMaxSize(ar.GetFileMaxSize());
        this->SetCurrFileSize(ar.GetCurrFileSize());
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
        this->SetCurrFileIndex(ar.GetCurrFileIndex());
        this->SetFileMaxSize(ar.GetFileMaxSize());
        this->SetCurrFileSize(ar.GetCurrFileSize());
        return *this;
    }

    void SetFileRoot(const char* pFileRoot);

    char* GetFileRoot();

    void SetFileAddr(ACE_FILE_Addr& objFileAddr);

    ACE_FILE_Addr& GetFileAddr();

    void SetConnector(ACE_FILE_Connector& objConnector);

    ACE_FILE_Connector& GetConnector();

    void SetFileIO(ACE_FILE_IO& objFile);

    ACE_FILE_IO& GetFileIO();

    void SetLogTime(const char* pLogTime);

    char* GetLogTime();

    void SetBufferSize(uint32 u4BufferSize);

    void SetFileMaxSize(uint32 u4FileMaxSize);

    void SetCurrFileIndex(uint16 u2CurrFileIndex);

    void SetCurrFileSize(uint32 u4CirrFileSize);

    uint32 GetBufferSize();

    uint32 GetFileMaxSize();

    uint16 GetCurrFileIndex();

    uint32 GetCurrFileSize();

    virtual int doLog(_LogBlockInfo* pLogBlockInfo);

    bool SendMail(_LogBlockInfo* pLogBlockInfo);

    ACE_TString& GetLoggerName();

    ACE_TString& GetServerName();

    int GetDisPlay();

    void SetDisplay(int nDisplay);

    void SetLoggerClass(int nType);

    int GetLoggerClass();

    void SetLoggerName(const char* szLoggerName);

    void SetLoggerID(uint16 u2LogID);

    uint16 GetLoggerID();

    void SetLevel(uint16 u2Level);

    uint16 GetLevel();

    void SetServerName(const char* szServerName);

    bool Run();

    void CheckTime();

    void CheckLogFileBlock();

    void CreatePath();

private:
    uint32              m_u4BufferSize;               //日志缓冲最大大小
    uint32              m_u4CurrFileSize;             //记录当前文件大小
    uint32              m_u4FileMaxSize;              //单个日志文件的最大大小
    uint16              m_u2CurrFileIndex;            //当前日志块序号
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

class CFileLogger : public IServerLogger
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
