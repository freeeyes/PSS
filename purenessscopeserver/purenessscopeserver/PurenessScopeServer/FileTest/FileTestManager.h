#ifndef _FILETESTMANAGER_H_
#define _FILETESTMANAGER_H_

#include <vector>
#include <map>
using namespace std;

#ifndef WIN32
#include "ConnectHandler.h"
#else
//如果是windows
#include "ProConnectHandle.h"
#endif

#include "XmlOpeation.h"

#include "ace/FILE_Addr.h"
#include "ace/FILE_Connector.h"
#include "ace/FILE_IO.h"
#include "FileTest.h"

class CFileTestManager : public ACE_Task<ACE_MT_SYNCH>, public IFileTestManager
{
public:
    CFileTestManager(void);
    virtual ~CFileTestManager(void);
public:
    //文件测试方法
    FileTestResultInfoSt FileTestStart(const char* szXmlFileTestName);      //开始文件测试
    int FileTestEnd();                                                      //结束文件测试
    void HandlerServerResponse(uint32 u4ConnectID);                         //当前连接发送数据包的回调方法
private:
    bool LoadXmlCfg(const char* szXmlFileTestName, FileTestResultInfoSt& objFileTestResult);        //读取测试配置文件
    int  ReadTestFile(const char* pFileName, int nType, FileTestDataInfoSt& objFileTestDataInfo);   //将消息包文件读入数据结构

    virtual int handle_timeout(const ACE_Time_Value& tv, const void* arg);   //定时器检查
private:
    ACE_Recursive_Thread_Mutex  m_ThreadWriteLock;
    ACE_Time_Value m_atvLastCheck;
    //文件测试变量
    bool m_bFileTesting;          //是否正在进行文件测试
    int32 m_n4TimerID;            //定时器ID

    CXmlOpeation m_MainConfig;
    string m_strProFilePath;
    int    m_n4TimeInterval;      //定时器事件间隔
    int    m_n4ConnectCount;      //模拟连接数
    int    m_n4ResponseCount;     //期望的应答条数
    int    m_n4ExpectTime;        //期望测试时总的耗时(单位毫秒)
    uint32 m_u4ParseID;           //解析包ID
    int    m_n4ContentType;       //协议数据类型,1是二进制协议,0是文本协议

    typedef vector<FileTestDataInfoSt> vecFileTestDataInfoSt;
    vecFileTestDataInfoSt m_vecFileTestDataInfoSt;

    typedef struct RESPONSERECORD
    {
        uint64 m_u8StartTime;
        uint8 m_u1ResponseCount;

        RESPONSERECORD()
        {
            Init();
        }

        RESPONSERECORD(const RESPONSERECORD& ar)
        {
            this->m_u8StartTime = ar.m_u8StartTime;
            this->m_u1ResponseCount = ar.m_u1ResponseCount;
        }

        void Init()
        {
            m_u8StartTime      = 0;
            m_u1ResponseCount  = 0;
        }

        RESPONSERECORD& operator= (const RESPONSERECORD& ar)
        {
            this->m_u8StartTime     = ar.m_u8StartTime;
            this->m_u1ResponseCount = ar.m_u1ResponseCount;
            return *this;
        }
    } ResponseRecordSt;

    typedef map<uint32,ResponseRecordSt> mapResponseRecordSt;
    mapResponseRecordSt m_mapResponseRecordSt;
};

typedef ACE_Singleton<CFileTestManager, ACE_Null_Mutex> App_FileTestManager;
#endif //_FILETESTMANAGER_H_