#ifndef _FILELOGGER_H
#define _FILELOGGER_H

#include "ace/FILE_Addr.h"
#include "ace/FILE_Connector.h"
#include "ace/FILE_IO.h"
#include "ace/Date_Time.h"

#include "define.h"
#include "TimeStamp.hpp"
#include "ILogObject.h"
#include "smtps.h"
#include "XmlConfig.h"
#include <string>
#include <unordered_map>
#include <fstream>

const uint16 MAX_CMD_NUM = 100;
const uint16 MAX_TIME_SIZE = 100;

//����Log��������
static const char LOGTYPE_OPERATION[] = "Operation";
static const char LOGTYPE_ERROR[] = "ServerError";
static const char FILELOG_CONFIG[] = "logger.xml";

//��XML��ȡ����־�ļ���Ϣ
class _Log_File_Info
{
public:
    uint16 m_u2LogID                  = 0;
    uint8  m_u1FileClass              = 0;
    uint8  m_u1DisPlay                = 0;
    uint16 m_u2LogLevel               = 0;
    string m_strFileName;

    _Log_File_Info() = default;
};

//��Ԫģʽ��־��
class CLogFile
{
public:
    CLogFile() = default;

    virtual ~CLogFile() = default;

    void Init(const char* pFileRoot, uint32 u4BufferSize, uint32 u4FileMaxSize);

    void Close();

    void SetFileRoot(const char* pFileRoot);

    const char* GetFileRoot() const;

    void SetLogTime(const char* pLogTime);

    const char* GetLogTime() const;

    void SetBufferSize(uint32 u4BufferSize) const;

    void SetFileMaxSize(uint32 u4FileMaxSize);

    void SetCurrFileIndex(uint16 u2CurrFileIndex);

    void SetCurrFileSize(uint32 u4CirrFileSize);

    uint32 GetBufferSize() const;

    uint32 GetFileMaxSize() const;

    uint16 GetCurrFileIndex() const;

    uint32 GetCurrFileSize() const;

    int doLog(shared_ptr<_LogBlockInfo> pLogBlockInfo);

    bool SendMail(shared_ptr<_LogBlockInfo> pLogBlockInfo, const xmlMails::_Mail* pMailInfo = nullptr) const;

    string GetLoggerName() const;

    string GetServerName() const;

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

    void CreatePath() const;

private:
    uint32              m_u4BufferSize             = 0;               //��־��������С
    uint32              m_u4CurrFileSize           = 0;               //��¼��ǰ�ļ���С
    uint32              m_u4FileMaxSize            = 0;               //������־�ļ�������С
    uint16              m_u2CurrFileIndex          = 1;               //��ǰ��־�����
    uint16              m_u2LogID                  = 0;               //��־���
    uint16              m_u2Level                  = 0;               //��־�ȼ�
    int                 m_nDisplay                 = 0;               //��ʾ���Ǽ�¼�ļ�
    string              m_strLogTime;                                 //Log��ǰʱ��
    string              m_strFileRoot;                                //·������Ŀ¼
    string              m_strlogName;                                 //ģ������
    string              m_strlogType               = "ServerError";   //��־����
    string              m_strServerName;                              //������ǰ׺
    ofstream            m_filestream;                                 //�ļ���
};

class CFileLogger : public IServerLogger
{
public:
    CFileLogger() = default;
    ~CFileLogger() final = default;

    int DoLog(uint16 nLogType, shared_ptr<_LogBlockInfo> pLogBlockInfo) final;
    int GetLogTypeCount() final;

    bool Init();
    bool ReSet(uint16 u2CurrLogLevel) final;
    void Close() final;

    uint32 GetBlockSize() final;
    uint32 GetPoolCount() final;

    uint16 GetCurrLevel() final;

    uint16 GetLogID(uint16 u2LogID) final;
    const char*  GetLogInfoByServerName(uint16 u2LogID) final;
    const char*  GetLogInfoByLogName(uint16 u2LogID) final;
    int    GetLogInfoByLogDisplay(uint16 u2LogID) final;
    uint16 GetLogInfoByLogLevel(uint16 u2LogID) final;

private:
    using hashmapFileList = unordered_map<uint16, shared_ptr<CLogFile>>;
    hashmapFileList                m_mapLogFileList;
    string                         m_strLogRoot;
    int                            m_nCount                  = 0;

    uint32                         m_u4BlockSize             = 0;
    uint32                         m_u4PoolCount             = 0;
    uint16                         m_u2CurrLogLevel          = 0;
};

#endif
