#ifndef _PROACTORUDPCLIENT_H
#define _PROACTORUDPCLIENT_H

#include "ace/ACE.h"
#include "ace/OS_main.h"
#include "ace/Proactor.h"
#include "ace/Asynch_IO.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram.h"
#include "TimeStamp.hpp"

#include "define.h"
#include "MessageBlockManager.h"
#include "ClientUDPMassage.h"

class CProactorUDPClient : public ACE_Service_Handler
{
public:
    CProactorUDPClient(void);
    ~CProactorUDPClient(void);

    virtual void handle_read_dgram(const ACE_Asynch_Read_Dgram::Result& result);

    int  OpenAddress(const ACE_INET_Addr& AddrLocal, EM_UDP_TYPE emType, ACE_Proactor* pProactor, IClientUDPMessage* pClientUDPMessage);
    void Close();
    bool SendMessage(const char* pMessage, uint32 u4Len, const char* szIP, int nPort);
    _ClientConnectInfo GetClientConnectInfo();

private:
    bool CheckMessage(ACE_Message_Block* pMbData, uint32 u4Len);     //����������ݰ����������ݶ���

    ACE_INET_Addr           m_addrRemote;                   //���ݷ��ͷ���IP��Ϣ
    ACE_SOCK_Dgram          m_skRemote;
    ACE_Asynch_Read_Dgram   m_Read;
    ACE_Asynch_Write_Dgram  m_Write;
    IClientUDPMessage*      m_pClientUDPMessage;            //��Ҫ�ص�����Ϣ��

    PSS_Time_Point          m_atvInput;                     //���հ���ʱ��
    PSS_Time_Point          m_atvOutput;                    //���Ͱ���ʱ��
    uint32                  m_u4RecvPacketCount;            //�������ݰ�������
    uint32                  m_u4SendPacketCount;            //�������ݰ�������
    uint32                  m_u4RecvSize;                   //�������ݵ��ܴ�С
    uint32                  m_u4SendSize;                   //�������ݵ��ܴ�С
};
#endif
