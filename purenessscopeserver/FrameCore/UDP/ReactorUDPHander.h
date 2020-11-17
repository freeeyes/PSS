#ifndef _REAACTORUDPHANDLER_H
#define _REAACTORUDPHANDLER_H

#include "ace/ACE.h"
#include "ace/OS_main.h"
#include "ace/OS_NS_string.h"
#include "ace/Event_Handler.h"
#include "ace/INET_Addr.h"
#include "ace/Reactor.h"
#include "ace/SOCK_Dgram.h"

#include "BaseHander.h"
#include "CommandAccount.h"
#include "UDPConnectManager.h"

class CReactorUDPHander : public ACE_Event_Handler, public IHandler
{
public:
    CReactorUDPHander(void);
    virtual ~CReactorUDPHander(void);

    virtual ACE_HANDLE get_handle(void) const;
    virtual int handle_input(ACE_HANDLE fd = ACE_INVALID_HANDLE);
    virtual int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask);

    void SetPacketParseInfoID(uint32 u4PacketParseInfoID);                    //���ö�Ӧ��m_u4PacketParseInfoID
    int  OpenAddress(const ACE_INET_Addr& AddrRemote, ACE_Reactor* pReactor);

    virtual void Close(uint32 u4ConnectID);
    virtual bool SendMessage(CSendMessageInfo objSendMessageInfo, uint32& u4PacketSize);
    virtual bool PutSendPacket(uint32 u4ConnectID, ACE_Message_Block* pMbData, uint32 u4Size, const ACE_Time_Value tvSend);
    virtual void SetIsLog(bool blIsLog);
    _ClientConnectInfo GetClientConnectInfo() const;
    void GetFlowInfo(uint32& u4FlowIn, uint32& u4FlowOut);                    //�õ����еĳ�������

private:
    bool CheckMessage(uint32 u4ConnectID, const char* pData, uint32 u4Len, ACE_INET_Addr addrRemote); //����������ݰ����������ݶ���
    int  Init_Open_Address(const ACE_INET_Addr& AddrRemote);                                          //��ʼ��UDP���Ӷ���
    void SaveSendInfo(uint32 u4Len);                                                                  //��¼������Ϣ
    void Send_Hander_Event(uint32 u4ConnandID, uint8 u1Option, ACE_INET_Addr addrRemote);             //�������ӽ�����Ϣ

    ACE_SOCK_Dgram          m_skRemote;
    ACE_INET_Addr           m_addrLocal;                            //��������IP��Ϣ
    CPacketParse            m_objPacketParse;                       //���ݰ�������

    ACE_Time_Value          m_atvInput;                             //���հ���ʱ��
    ACE_Time_Value          m_atvOutput;                            //���Ͱ���ʱ��
    uint32                  m_u4RecvPacketCount   = 0;              //�������ݰ�������
    uint32                  m_u4SendPacketCount   = 0;              //�������ݰ�������
    uint32                  m_u4RecvSize          = 0;              //�������ݵ��ܴ�С
    uint32                  m_u4SendSize          = 0;              //�������ݵ��ܴ�С
    uint32                  m_u4PacketParseInfoID = 0;              //��Ӧ����packetParse��ģ��ID
    CCommandAccount         m_CommandAccount;                       //���ݰ�ͳ��
	ACE_Message_Block*      m_pBlockMessage            = nullptr;   //��ǰ���ͻ���ȴ����ݿ�
	ACE_Message_Block*      m_pBlockRecv               = nullptr;   //�������ݻ����
    _Packet_Parse_Info*     m_pPacketParseInfo         = nullptr;   //PacketParse������

};
#endif
