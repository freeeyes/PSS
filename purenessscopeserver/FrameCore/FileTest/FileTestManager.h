#ifndef _FILETESTMANAGER_H_
#define _FILETESTMANAGER_H_

#include "HashTable.h"
#include "XmlOpeation.h"
#include "HandlerManager.h"

#include "ace/FILE_Addr.h"
#include "ace/FILE_Connector.h"
#include "ace/FILE_IO.h"
#include "FileTest.h"
#include <sstream>
#include <unordered_map>

class CFileTestManager : public IFileTestManager
{
public:
    CFileTestManager(void) = default;

    //文件测试方法
    FileTestResultInfoSt FileTestStart(const char* szXmlFileTestName);      //开始文件测试
    int FileTestEnd() const;                                                //结束文件测试
    void HandlerServerResponse(uint32 u4ConnectID) final;                   //当前连接发送数据包的回调方法
    void Close();                                                           //清理

private:
    bool LoadXmlCfg(const char* szXmlFileTestName, FileTestResultInfoSt& objFileTestResult);        //读取测试配置文件
    FILE_TEST_RESULT ReadTestFile(const char* pFileName, int nType, FileTestDataInfoSt& objFileTestDataInfo) const;   //将消息包文件读入数据结构
    int  InitResponseRecordList();                                                                  //初始化ResponseRecord
    bool AddResponseRecordList(uint32 u4ConnectID, const ACE_Time_Value& tv);                       //添加ResponseRecordList

    ACE_Recursive_Thread_Mutex  m_ThreadWriteLock;
    //文件测试变量
    bool m_bFileTesting       = false;       //是否正在进行文件测试

    CXmlOpeation m_MainConfig;
    string m_strProFilePath;
    uint32 m_u4TimeInterval   = 1000;   //定时器事件间隔
    uint32 m_u4ConnectCount   = 0;      //模拟连接数
    uint32 m_u4ResponseCount  = 0;      //期望的应答条数
    uint32 m_u4ExpectTime     = 0;      //期望测试时总的耗时(单位毫秒)
    uint32 m_u4ParseID        = 0;      //解析包ID
    uint32 m_u4ContentType    = 0;      //协议数据类型,1是二进制协议,0是文本协议

    using vecFileTestDataInfoSt = vector<FileTestDataInfoSt>;
    vecFileTestDataInfoSt m_vecFileTestDataInfoSt;

    //内部使用的映射类
    class ResponseRecordSt
    {
    public:
        uint64 m_u8StartTime     = 0;
        uint8  m_u1ResponseCount = 0;
        uint32 m_u4ConnectID     = 0;

        ResponseRecordSt() = default;
    } ;

    //定义接收参数
    using hashmapResponseRecord = unordered_map<uint32, shared_ptr<ResponseRecordSt>>;
    hashmapResponseRecord m_objResponseRecordList;

};

using App_FileTestManager = PSS_singleton<CFileTestManager>;
#endif //_FILETESTMANAGER_H_