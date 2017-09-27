#ifndef _CONNECTACCEPT_H
#define _CONNECTACCEPT_H

#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/INET_Addr.h"
#include "ConnectHandler.h"
#include "AceReactorManager.h"

#include <vector>

using namespace std;

//用于普通客户端
class ConnectAcceptor : public ACE_Acceptor<CConnectHandler, ACE_SOCK_ACCEPTOR>
{
public:
    ConnectAcceptor();

    void InitClientReactor(uint32 u4ClientReactorCount);
    void SetPacketParseInfoID(uint32 u4PaccketParseInfoID);

protected:
    virtual int make_svc_handler(CConnectHandler*& sh);

public:
    //因为基础的open不能满足需求，所以在这里重载了一个open接口
    //提供设置backlog的服务
    int open2(ACE_INET_Addr& local_addr,
              ACE_Reactor* reactor,
              int flags,
              int backlog);

    int Init_Open(const ACE_INET_Addr& local_addr,
                  int flags = 0,
                  int use_select = 1,
                  int reuse_addr = 1,
                  int backlog = ACE_DEFAULT_BACKLOG);

    int Run_Open(ACE_Reactor* reactor);

    char*  GetListenIP();
    uint32 GetListenPort();

private:
    char   m_szListenIP[MAX_BUFF_20];
    uint32 m_u4Port;
    uint32 m_u4AcceptCount;             //接收的总连接数
    uint32 m_u4ClientReactorCount;      //客户端反应器的个数
    uint32 m_u4PacketParseInfoID;       //处理包解析的模块ID
};

class CConnectAcceptorManager : public ACE_Task<ACE_MT_SYNCH>
{
public:
    CConnectAcceptorManager(void);
    ~CConnectAcceptorManager(void);

    bool InitConnectAcceptor(int nCount, uint32 u4ClientReactorCount);
    void Close();
    int GetCount();
    ConnectAcceptor* GetConnectAcceptor(int nIndex);
    ConnectAcceptor* GetNewConnectAcceptor();
    const char* GetError();

    bool Close(const char* pIP, uint32 n4Port);
    bool CheckIPInfo(const char* pIP, uint32 n4Port);

private:
    typedef vector<ConnectAcceptor*> vecConnectAcceptor;
    vecConnectAcceptor m_vecConnectAcceptor;
    int                m_nAcceptorCount;
    char               m_szError[MAX_BUFF_500];

public:
    //文件测试方法
    FileTestResultInfoSt FileTestStart(string strXmlCfg);      //开始文件测试
    int FileTestEnd();        //结束文件测试
private:
    int LoadXmlCfg(string strXmlCfg);

    virtual int handle_timeout(const ACE_Time_Value& tv, const void* arg);   //定时器检查
private:
    //文件测试变量
    bool m_bFileTesting;          //是否正在进行文件测试
    bool m_bLoadCfgFile;          //是否已经加载配置文件
    uint32 m_u4TimerID;           //定时器ID

};

typedef ACE_Singleton<CConnectAcceptorManager, ACE_Null_Mutex> App_ConnectAcceptorManager;
#endif
