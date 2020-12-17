#ifndef _PROASYNCHCONNECT_H
#define _PROASYNCHCONNECT_H

#include "ace/Asynch_Connector.h"
#include "ProConnectClient.h"

//����һ���ṹ�壬�����첽֪ͨ��ǰ������Ϣ
struct _ProConnectState_Info
{
    int m_nServerID;

    _ProConnectState_Info()
    {
        m_nServerID = 0;
    }
};

class CProAsynchConnect : public ACE_Asynch_Connector<CProConnectClient>
{
public:
    CProAsynchConnect(void);
    ~CProAsynchConnect(void);

    virtual int validate_connection(const ACE_Asynch_Connect::Result& result, const ACE_INET_Addr& remote, const ACE_INET_Addr& local);
    virtual CProConnectClient* make_handler(void);

    void SetConnectState(bool blConnectState = false);
    bool GetConnectState();

private:
    bool               m_blConnectState;
    int                m_nServerID;
    ACE_INET_Addr      m_AddrClientRemote;
    ACE_INET_Addr      m_AddrClientLocal;
};
#endif
