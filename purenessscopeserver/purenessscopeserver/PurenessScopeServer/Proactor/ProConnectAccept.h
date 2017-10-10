#ifndef _PROACOOECTACCEPT_H
#define _PROACOOECTACCEPT_H

#include <vector>

using namespace std;

#include "ForbiddenIP.h"
#include "ProConnectHandle.h"
#include "AceProactorManager.h"

//平常客户端的Acceptor
class ProConnectAcceptor : public ACE_Asynch_Acceptor<CProConnectHandle>
{
public:
    ProConnectAcceptor();
    void InitClientProactor(uint32 u4ClientProactorCount);
    void SetPacketParseInfoID(uint32 u4PaccketParseInfoID);
    uint32 GetPacketParseInfoID();

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

class CProConnectAcceptManager
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
};

typedef ACE_Singleton<CProConnectAcceptManager, ACE_Null_Mutex> App_ProConnectAcceptManager;
#endif
