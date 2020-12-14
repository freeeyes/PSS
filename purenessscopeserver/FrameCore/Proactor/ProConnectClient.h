#ifndef _PROCONNECTCLIENT_H
#define _PROCONNECTCLIENT_H

//�ṩ�ͻ��˷������ӵĽ������
//�û������ֻ࣬�����ݵ��շ����Զ������������ϲ�ָ��
//add by freeeyes 2010-12-24

#include "ace/Recursive_Thread_Mutex.h"
#include "ace/INET_Addr.h"
#include "ace/Asynch_IO.h"
#include "ace/Mutex.h"
#include "ace/Thread_Mutex.h"

#include "AceProactorManager.h"
#include "BaseConnectClient.h"
#include "LogManager.h"
#include "BaseHander.h"
#include "TcpRedirection.h"

#define MAX_BUFF_1024 1024
#define MAX_IP_LEN    16

class CProConnectClient : public ACE_Service_Handler, public IHandler
{
public:
    CProConnectClient(void);
    virtual ~CProConnectClient(void);

    virtual void open(ACE_HANDLE h, ACE_Message_Block&);                                               //�û�����һ������
    virtual void handle_read_stream(const ACE_Asynch_Read_Stream::Result& result);                     //�����û�����
    virtual void handle_write_stream(const ACE_Asynch_Write_Stream::Result& result);                   //�����û�����
    virtual void addresses(const ACE_INET_Addr& remote_address, const ACE_INET_Addr& local_address);   //������ӵ�ַ
    virtual bool Device_Send_Data(const char* pData, ssize_t nLen);                                    //͸�����ݽӿ�

    void Close(uint32 u4ConnectID) final;
    bool SendMessage(const CSendMessageInfo& objSendMessageInfo, uint32& u4PacketSize) final;
    bool PutSendPacket(uint32 u4ConnectID, ACE_Message_Block* pMbData, uint32 u4Size, const PSS_Time_Point& tvSend) final;
    void SetIsLog(bool blIsLog) final;

    void Close();                                          //���ӹر�
    void ClientClose(EM_s2s& ems2s);                       //�ͻ������ҹر�
    void SetServerID(int nServerID);                       //���õ�ǰ��ServerID
    void SetPacketParseInfoID(uint32 u4PacketParseInfoID); //����PacketParseID
    void SetClientMessage(IClientMessage* pClientMessage); //������Ϣ���մ�����
    int  GetServerID();                                    //��õ�ǰServerID
    bool SendData(ACE_Message_Block* pmblk);               //��������

    bool GetTimeout(PSS_Time_Point const& tvNow);                 //��õ�ǰ���ݴ����Ƿ�ʱ

    _ClientConnectInfo GetClientConnectInfo();             //�õ���ǰ������Ϣ

private:
    bool RecvData(uint32 u4PacketLen, ACE_Message_Block* pmbSave);        //��������
    int SendMessageGroup(uint16 u2CommandID, ACE_Message_Block* pmblk);   //�����յ����ݰ����������Ϣ����ģ��

private:
    ACE_INET_Addr               m_AddrRemote;                //����Զ�̷������ĵ�ַ
    ACE_Message_Block*          m_mbRecv;                    //��ȡ���ݻ���
    ACE_Asynch_Read_Stream      m_Reader;                    //��ȡ��
    ACE_Asynch_Write_Stream     m_Writer;                    //д����
    IClientMessage*             m_pClientMessage;            //������Ϣ���յ���
    int                         m_nIOCount;                  //��ǰ���ø���
    uint32                      m_u4MaxPacketSize;           //�����հ���
    int                         m_nServerID;                 //��ǰServerID
    ACE_Recursive_Thread_Mutex  m_ThreadWritrLock;           //�߳���

    PSS_Time_Point              m_atvBegin;                  //���ӽ���ʱ��
    uint32                      m_u4SendSize;                //�����ֽ���
    uint32                      m_u4SendCount;               //�������ݰ���
    uint32                      m_u4RecvSize;                //�����ֽ���
    uint32                      m_u4RecvCount;               //�������ݰ���
    uint32                      m_u4CostTime;                //��Ϣ������ʱ��
    EM_s2s                      m_ems2s;                     //�Ƿ���Ҫ�ص�
    PSS_Time_Point              m_atvRecv;                   //���ݽ���ʱ��
    EM_Server_Recv_State        m_emRecvState;               //0Ϊδ�������ݣ�1Ϊ����������ɣ�2Ϊ�����������
    EM_CONNECT_IO_DISPOSE       m_emDispose;                 //����ģʽ����ܴ��� or ҵ����
    uint32                      m_u4PacketParseInfoID;       //��ܴ���ģ��ID
    string                      m_strDeviceName;             //ת���ӿ�����
    CMakePacket                 m_MakePacket;
};
#endif