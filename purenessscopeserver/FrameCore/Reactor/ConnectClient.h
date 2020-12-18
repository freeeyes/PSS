#ifndef _CONNECTCLIENT_H
#define _CONNECTCLIENT_H

//����ͻ�����Ϣ
//Reactor����ģʽ���������ݰ��������ݷ����߼��ദ��
//�������Proactor�µĿͻ��˷��ͣ���ʼ���reactor��ϵͳ��
//add by freeeyes
//2011-01-17

#include "define.h"

#include "ace/Reactor.h"
#include "ace/Synch.h"
#include "ace/SOCK_Stream.h"
#include "ace/Svc_Handler.h"
#include "ace/Reactor_Notification_Strategy.h"

#include "AceReactorManager.h"
#include "BaseConnectClient.h"
#include "LogManager.h"
#include "BaseHander.h"
#include "TcpRedirection.h"

class CConnectClient : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>, public IHandler
{
public:
    CConnectClient(void) = default;
    ~CConnectClient(void) final = default;

    int open(void*) final;
    int handle_input(ACE_HANDLE fd = ACE_INVALID_HANDLE) final;
    int handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask) final;
    bool Device_Send_Data(const char* pData, ssize_t nLen) final; //͸�����ݽӿ�

    void Close(uint32 u4ConnectID) final;
    bool SendMessage(const CSendMessageInfo& objSendMessageInfo, uint32& u4PacketSize) final;
    bool PutSendPacket(uint32 u4ConnectID, ACE_Message_Block* pMbData, uint32 u4Size, const PSS_Time_Point& tvSend) final;
    void SetIsLog(bool blIsLog) final;

    void SetClientMessage(IClientMessage* pClientMessage); //������Ϣ���մ�����
    void SetServerID(int nServerID);                       //���õ�ǰ��ServerID
    int  GetServerID() const;                              //��õ�ǰServerID
    void SetPacketParseInfoID(uint32 u4PacketParseInfoID); //����PacketParseID
    bool SendData(ACE_Message_Block* pmblk);
    void Close();

    bool GetTimeout(PSS_Time_Point const& tvNow) const;           //��õ�ǰ���ݴ����Ƿ�ʱ

    void ClientClose();                                           //�����ر�
    _ClientConnectInfo GetClientConnectInfo() const;              //�õ���ǰ������Ϣ

private:
    int RecvData();                                                       //�������ݣ�����ģʽ
    int SendMessageGroup(uint16 u2CommandID, ACE_Message_Block* pmblk) const;   //�����յ����ݰ����������Ϣ����ģ��

    int Dispose_Recv_Data(ACE_Message_Block* pCurrMessage);               //������յ�������

    uint32                      m_u4SendSize            = 0;          //�����ֽ���
    uint32                      m_u4SendCount           = 0;          //�������ݰ���
    uint32                      m_u4RecvSize            = 0;          //�����ֽ���
    uint32                      m_u4RecvCount           = 0;          //�������ݰ���
    uint32                      m_u4CostTime            = 0;          //��Ϣ������ʱ��
    uint32                      m_u4CurrSize            = 0;          //��ǰ���յ����ֽ���
    uint32                      m_u4MaxPacketSize       = MAX_MSG_PACKETLENGTH;      //�����հ���
    uint32                      m_u4PacketParseInfoID   = 0;          //��ܴ���ģ��ID
    int                         m_nIOCount              = 1;          //��ǰIO�����ĸ���
    int                         m_nServerID             = 0;          //������ID
    CONNECTSTATE                m_u1ConnectState        = CONNECTSTATE::CONNECT_INIT; //����״̬
    string                      m_strError;
    ACE_INET_Addr               m_addrRemote;

    ACE_Recursive_Thread_Mutex  m_ThreadLock;
    IClientMessage*             m_pClientMessage = nullptr;              //��Ϣ�������ָ��
    ACE_Message_Block*          m_pCurrMessage   = nullptr;              //��ǰ��MB����
    PSS_Time_Point              m_atvBegin;                           //���ӽ���ʱ��

    EM_s2s                      m_ems2s       = EM_s2s::S2S_NEED_CALLBACK;    //�Ƿ���Ҫ�ص�״̬
	EM_Server_Recv_State        m_emRecvState = EM_Server_Recv_State::SERVER_RECV_INIT;     //0Ϊδ�������ݣ�1Ϊ����������ɣ�2Ϊ�����������
	EM_CONNECT_IO_DISPOSE       m_emDispose   = EM_CONNECT_IO_DISPOSE::CONNECT_IO_PLUGIN;    //����ģʽ����ܴ��� or ҵ����
    PSS_Time_Point              m_atvRecv;                            //���ݽ���ʱ��
    string                      m_strDeviceName;                      //ת���ӿ�����
    CMakePacket                 m_MakePacket;
};
#endif
