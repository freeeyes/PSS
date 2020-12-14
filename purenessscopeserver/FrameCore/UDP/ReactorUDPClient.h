#ifndef _REAACTORUDPCLIENT_H
#define _REAACTORUDPCLIENT_H

#include "ace/ACE.h"
#include "ace/OS_main.h"
#include "ace/OS_NS_string.h"
#include "ace/Event_Handler.h"
#include "ace/INET_Addr.h"
#include "ace/Reactor.h"
#include "ace/SOCK_Dgram.h"

#include "define.h"
#include "MessageBlockManager.h"
#include "ClientUDPMassage.h"
#include "TimeStamp.hpp"

class CReactorUDPClient : public ACE_Event_Handler
{
public:
    CReactorUDPClient(void) = default;
    ~CReactorUDPClient(void) final = default;

    ACE_HANDLE get_handle(void) const final;
    int handle_input(ACE_HANDLE fd = ACE_INVALID_HANDLE) final;
    int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask) final;

    int  OpenAddress(const ACE_INET_Addr& AddrRemote, EM_UDP_TYPE emType, ACE_Reactor* pReactor, IClientUDPMessage* pClientUDPMessage);
    void Close();
    bool SendMessage(const char* pMessage, uint32 u4Len, const char* szIP, uint16 u2Port);
    _ClientConnectInfo GetClientConnectInfo() const;

private:
    bool CheckMessage(const char* pData, uint32 u4Len);     //����������ݰ����������ݶ���

    ACE_SOCK_Dgram          m_skRemote;
    ACE_INET_Addr           m_addrRemote;                   //���ݷ��ͷ���IP��Ϣ
    IClientUDPMessage*      m_pClientUDPMessage = nullptr;     //��Ϣ������

    PSS_Time_Point          m_atvInput;                     //���հ���ʱ��
    PSS_Time_Point          m_atvOutput;                    //���Ͱ���ʱ��
    uint32                  m_u4RecvPacketCount = 0;        //�������ݰ�������
    uint32                  m_u4SendPacketCount = 0;        //�������ݰ�������
    uint32                  m_u4RecvSize        = 0;        //�������ݵ��ܴ�С
    uint32                  m_u4SendSize        = 0;        //�������ݵ��ܴ�С
};
#endif
