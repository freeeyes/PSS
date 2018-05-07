#ifndef _FILETESTMANAGER_H_
#define _FILETESTMANAGER_H_

#ifndef WIN32
#include "ConnectHandler.h"
#else
//如果是windows
#include "ProConnectHandle.h"
#endif

#include "HashTable.h"
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

    //文件测试方法
    FileTestResultInfoSt FileTestStart(const char* szXmlFileTestName);      //开始文件测试
    int FileTestEnd();                                                      //结束文件测试
    void HandlerServerResponse(uint32 u4ConnectID);                         //当前连接发送数据包的回调方法
    void Close();                                                           //清理

private:
    bool LoadXmlCfg(const char* szXmlFileTestName, FileTestResultInfoSt& objFileTestResult);        //读取测试配置文件
    int  ReadTestFile(const char* pFileName, int nType, FileTestDataInfoSt& objFileTestDataInfo);   //将消息包文件读入数据结构
    int  ResponseRecordList();                                                                      //初始化ResponseRecord

    int handle_timeout(const ACE_Time_Value& tv, const void* arg);   //定时器检查

    ACE_Recursive_Thread_Mutex  m_ThreadWriteLock;
    //文件测试变量
    bool m_bFileTesting;          //是否正在进行文件测试
    int32 m_n4TimerID;            //定时器ID

    CXmlOpeation m_MainConfig;
    string m_strProFilePath;
    uint32 m_u4TimeInterval;      //定时器事件间隔
    uint32 m_u4ConnectCount;      //模拟连接数
    uint32 m_u4ResponseCount;     //期望的应答条数
    uint32 m_u4ExpectTime;        //期望测试时总的耗时(单位毫秒)
    uint32 m_u4ParseID;           //解析包ID
    uint32 m_u4ContentType;       //协议数据类型,1是二进制协议,0是文本协议

    typedef vector<FileTestDataInfoSt> vecFileTestDataInfoSt;
    vecFileTestDataInfoSt m_vecFileTestDataInfoSt;

    //内部使用的映射类
    class ResponseRecordSt
    {
    public:
        uint64 m_u8StartTime;
        uint8  m_u1ResponseCount;
        uint32 m_u4ConnectID;

        ResponseRecordSt()
        {
            Init();
        }

        ResponseRecordSt(const ResponseRecordSt& ar)
        {
            this->m_u8StartTime     = ar.m_u8StartTime;
            this->m_u1ResponseCount = ar.m_u1ResponseCount;
            this->m_u4ConnectID     = ar.m_u4ConnectID;
        }

        void Init()
        {
            m_u8StartTime      = 0;
            m_u1ResponseCount  = 0;
            m_u4ConnectID      = 0;
        }

        ResponseRecordSt& operator= (const ResponseRecordSt& ar)
        {
            this->m_u8StartTime     = ar.m_u8StartTime;
            this->m_u1ResponseCount = ar.m_u1ResponseCount;
            return *this;
        }
    } ;

    //定义接收参数
    CHashTable<ResponseRecordSt> m_objResponseRecordList;

};

typedef ACE_Singleton<CFileTestManager, ACE_Null_Mutex> App_FileTestManager;
#endif //_FILETESTMANAGER_H_