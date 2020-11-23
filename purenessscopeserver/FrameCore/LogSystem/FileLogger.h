#ifndef _FILELOGGER_H
#define _FILELOGGER_H

#include "ace/FILE_Addr.h"
#include "ace/FILE_Connector.h"
#include "ace/FILE_IO.h"
#include "ace/Date_Time.h"

#include "define.h"
#include "ILogObject.h"
#include "smtps.h"
#include "XmlConfig.h"
#include <string>
#include <sstream>
#include <vector>

const uint16 MAX_CMD_NUM = 100;
const uint16 MAX_TIME_SIZE = 100;

//定义Log所属类型
static const char LOGTYPE_OPERATION[] = "Operation";
static const char LOGTYPE_ERROR[] = "ServerError";
static const char FILELOG_CONFIG[] = "logger.xml";

//从XML读取的日志文件信息
class _Log_File_Info
{
public:
    uint16 m_u2LogID                  = 0;
    uint8  m_u1FileClass              = 0;
    uint8  m_u1DisPlay                = 0;
    uint16 m_u2LogLevel               = 0;
    char   m_szFileName[MAX_BUFF_100] = {'\0'};

    _Log_File_Info()
    {
    }
};

//单元模式日志类
class CLogFile
{
public:
    CLogFile(const char* pFileRoot, uint32 u4BufferSize, uint32 u4FileMaxSize);

    virtual ~CLogFile() = default;

    void Init();

    void Close();

    void SetFileRoot(const char* pFileRoot);

    const char* GetFileRoot();

    void SetFileAddr(const ACE_FILE_Addr& objFileAddr);

    ACE_FILE_Addr& GetFileAddr();

    void SetConnector(const ACE_FILE_Connector& objConnector);

    ACE_FILE_Connector& GetConnector();

    void SetFileIO(const ACE_FILE_IO& objFile);

    ACE_FILE_IO& GetFileIO();

    void SetLogTime(const char* pLogTime);

    const char* GetLogTime();

    void SetBufferSize(uint32 u4BufferSize);

    void SetFileMaxSize(uint32 u4FileMaxSize);

    void SetCurrFileIndex(uint16 u2CurrFileIndex);

    void SetCurrFileSize(uint32 u4CirrFileSize);

    uint32 GetBufferSize() const;

    uint32 GetFileMaxSize() const;

    uint16 GetCurrFileIndex() const;

    uint32 GetCurrFileSize() const;

    virtual int doLog(_LogBlockInfo* pLogBlockInfo);

    bool SendMail(const _LogBlockInfo* pLogBlockInfo, const xmlMails::_Mail* pMailInfo = nullptr) const;

    ACE_TString& GetLoggerName();

    ACE_TString& GetServerName();

    int GetDisPlay() const;

    void SetDisplay(int nDisplay);

    void SetLoggerClass(int nType);

    int GetLoggerClass() const;

    void SetLoggerName(const char* szLoggerName);

    void SetLoggerID(uint16 u2LogID);

    uint16 GetLoggerID() const;

    void SetLevel(uint16 u2Level);

    uint16 GetLevel() const;

    void SetServerName(const char* szServerName);

    bool Run();

    void CheckTime();

    void CheckLogFileBlock();

    void CreatePath();

private:
    uint32              m_u4BufferSize             = 0;               //日志缓冲最大大小
    uint32              m_u4CurrFileSize           = 0;               //记录当前文件大小
    uint32              m_u4FileMaxSize            = 0;               //单个日志文件的最大大小
    uint16              m_u2CurrFileIndex          = 1;               //当前日志块序号
    uint16              m_u2LogID                  = 0;               //日志编号
    uint16              m_u2Level                  = 0;               //日志等级
    int                 m_nDisplay                 = 0;               //显示还是记录文件
    char*               m_pBuffer                  = nullptr;         //日志缓冲指针
    string              m_strLogTime;                                 //Log当前时间
    string              m_strFileRoot;                                //路径的主目录
    ACE_TString         m_StrlogName;                                 //模块名字
    ACE_TString         m_StrlogType               = "ServerError";   //日志类型
    ACE_TString         m_StrServerName;                              //服务器前缀
    ACE_FILE_Connector  m_Connector;                                  //I/O操作连接器
    ACE_FILE_IO         m_File;
    ACE_FILE_Addr       m_FileAddr;
};

class CFileLogger : public IServerLogger
{
public:
    CFileLogger() = default;
    ~CFileLogger() final = default;

    int DoLog(int nLogType, _LogBlockInfo* pLogBlockInfo) final;
    int GetLogTypeCount() final;

    bool Init();
    bool ReSet(uint16 u2CurrLogLevel);
    void Close() final;

    uint32 GetBlockSize() final;
    uint32 GetPoolCount() final;

    uint16 GetCurrLevel() final;

    uint16 GetLogID(uint16 u2Index) final;
    const char*  GetLogInfoByServerName(uint16 u2LogID) final;
    const char*  GetLogInfoByLogName(uint16 u2LogID) final;
    int    GetLogInfoByLogDisplay(uint16 u2LogID) final;
    uint16 GetLogInfoByLogLevel(uint16 u2LogID) final;

private:
    vector<shared_ptr<CLogFile>>   m_vecLogFileList;
    string                         m_strLogRoot;
    int                            m_nCount                  = 0;

    uint32                         m_u4BlockSize             = 0;
    uint32                         m_u4PoolCount             = 0;
    uint16                         m_u2CurrLogLevel          = 0;
};

#endif
