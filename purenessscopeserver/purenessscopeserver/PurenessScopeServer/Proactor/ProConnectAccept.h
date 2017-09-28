#ifndef _PROACOOECTACCEPT_H
#define _PROACOOECTACCEPT_H

#include <vector>

using namespace std;

#include "ForbiddenIP.h"
#include "ProConnectHandle.h"
#include "AceProactorManager.h"
#include "XmlOpeation.h"

#include "ace/FILE_Addr.h"
#include "ace/FILE_Connector.h"
#include "ace/FILE_IO.h"

//平常客户端的Acceptor
class ProConnectAcceptor : public ACE_Asynch_Acceptor<CProConnectHandle>
{
public:
    ProConnectAcceptor();
    void InitClientProactor(uint32 u4ClientProactorCount);
    void SetPacketParseInfoID(uint32 u4PaccketParseInfoID);
    uint32 GetPacketParseInfoID();
    CProConnectHandle* file_test_make_handler(void);
private:
    virtual CProConnectHandle* make_handler (void);
    virtual int validate_connection (const ACE_Asynch_Accept::Result& result,
                                     const ACE_INET_Addr& remote,
                                     const ACE_INET_Addr& local);

public:
    void SetListenInfo(const char* pIP, uint32 u4Port);
    char*  GetListenIP();
    uint32 GetListenPort();

private:
    char   m_szListenIP[MAX_BUFF_20];
    uint32 m_u4Port;
    uint32 m_u4AcceptCount;             //接收的总连接数
    uint32 m_u4ClientProactorCount;     //客户端反应器的个数
    uint32 m_u4PacketParseInfoID;       //处理包解析的模块ID
};

class CProConnectAcceptManager : public ACE_Task<ACE_MT_SYNCH>
{
public:
    CProConnectAcceptManager(void);
    ~CProConnectAcceptManager(void);

    bool InitConnectAcceptor(int nCount, uint32 u4ClientProactorCount);
    void Close();
    int GetCount();
    ProConnectAcceptor* GetConnectAcceptor(int nIndex);
    ProConnectAcceptor* GetNewConnectAcceptor();
    const char* GetError();

    bool Close(const char* pIP, uint32 n4Port);
    bool CheckIPInfo(const char* pIP, uint32 n4Port);

private:
    typedef vector<ProConnectAcceptor*> vecProConnectAcceptor;
    vecProConnectAcceptor m_vecConnectAcceptor;
    int                   m_nAcceptorCount;
    char                  m_szError[MAX_BUFF_500];

public:
    //文件测试方法
    FileTestResultInfoSt FileTestStart(const char* szXmlFileTestName);      //开始文件测试
    int FileTestEnd();        //结束文件测试
private:
    bool LoadXmlCfg(const char* szXmlFileTestName, FileTestResultInfoSt& objFileTestResult);        //读取测试配置文件
    int  ReadTestFile(const char* pFileName, int nType, FileTestDataInfoSt& objFileTestDataInfo);   //将消息包文件读入数据结构

    virtual int handle_timeout(const ACE_Time_Value& tv, const void* arg);   //定时器检查
private:
    //文件测试变量
    bool m_bFileTesting;          //是否正在进行文件测试
    int32 m_n4TimerID;            //定时器ID

    CXmlOpeation m_MainConfig;
    string m_strProFilePath;
    int    m_n4TimeInterval;      //定时器事件间隔
    int    m_n4ConnectCount;      //模拟连接数
    uint32 m_u4ParseID;           //解析包ID
    int    m_n4ContentType;       //协议数据类型,1是二进制协议,0是文本协议
    
    typedef vector<FileTestDataInfoSt> vecFileTestDataInfoSt;
    vecFileTestDataInfoSt m_vecFileTestDataInfoSt;
};

typedef ACE_Singleton<CProConnectAcceptManager, ACE_Null_Mutex> App_ProConnectAcceptManager;
#endif
