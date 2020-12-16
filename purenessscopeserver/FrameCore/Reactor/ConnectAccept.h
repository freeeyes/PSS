#ifndef _CONNECTACCEPT_H
#define _CONNECTACCEPT_H

#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/INET_Addr.h"
#include "ConnectHandler.h"
#include "AceReactorManager.h"
#include <stdexcept>

//用于普通客户端
class ConnectAcceptor : public ACE_Acceptor<CConnectHandler, ACE_SOCK_ACCEPTOR>
{
public:
    ConnectAcceptor() = default;

    void InitClientReactor(uint32 u4ClientReactorCount);
    void SetPacketParseInfoID(uint32 u4PaccketParseInfoID);
    uint32 GetPacketParseInfoID() const;

protected:
    int make_svc_handler(CConnectHandler*& sh) final;

public:
    //因为基础的open不能满足需求，所以在这里重载了一个open接口
    //提供设置backlog的服务
    int open2(const ACE_INET_Addr& local_addr,
              ACE_Reactor* reactor,
              int flags,
              int backlog);

    int Init_Open(const ACE_INET_Addr& local_addr,
                  int flags = 0,
                  int use_select = 1,
                  int reuse_addr = 1,
                  int backlog = ACE_DEFAULT_BACKLOG);

    int Run_Open(ACE_Reactor* reactor);

    const char*  GetListenIP() const;
    uint16 GetListenPort() const;

private:
    string m_strListenIP;
    uint16 m_u2Port                  = 0;
    uint32 m_u4AcceptCount           = 0;        //接收的总连接数
    uint32 m_u4ClientReactorCount    = 1;        //客户端反应器的个数
    uint32 m_u4PacketParseInfoID     = 0;        //处理包解析的模块ID
};

class CConnectAcceptorManager
{
public:
    CConnectAcceptorManager(void) = default;

    bool InitConnectAcceptor(int nCount, uint32 u4ClientReactorCount);
    void Close();
    int GetCount() const;
    shared_ptr<ConnectAcceptor> GetConnectAcceptor(int nIndex);
    shared_ptr<ConnectAcceptor> GetNewConnectAcceptor();
    const char* GetError() const;

    bool Close(const char* pIP, uint16 n4Port);
    bool CheckIPInfo(const char* pIP, uint16 n4Port);

private:
    using vecConnectAcceptor = vector<shared_ptr<ConnectAcceptor>>;
    vecConnectAcceptor m_vecConnectAcceptor;
    int                m_nAcceptorCount        = 0;
    string             m_strError;
};

using App_ConnectAcceptorManager = PSS_singleton<CConnectAcceptorManager>;
#endif
