#ifndef _PROCONSOLEHANDLE_H
#define _PROCONSOLEHANDLE_H

//�����ڶԷ��������Ƶ�ģ�飬����Զ�̿����������
//������汾��һֱ��ӣ���������ſ�ʼ�㣬��Ҫ���Լ�������ξ������ٸ��Լ�������ɡ�
//add by freeeyes
#include "ace/Svc_Handler.h"
#include "ace/Synch.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Asynch_IO.h"
#include "ace/Asynch_Acceptor.h"
#include "ace/Proactor.h"
#include "ace/Thread_Mutex.h"

#include "AceProactorManager.h"
#include "IConnectManager.h"
#include "BaseConsoleHandle.h"
#include "TimerManager.hpp"
#include "Fast_Asynch_Read_Stream.h"


class CProConsoleHandle : public ACE_Service_Handler
{
public:
    CProConsoleHandle();
    ~CProConsoleHandle();

    virtual void open(ACE_HANDLE h, ACE_Message_Block&);                                 //�û�����һ������
    virtual void handle_read_stream(const ACE_Asynch_Read_Stream::Result& result);
    virtual void handle_write_stream(const ACE_Asynch_Write_Stream::Result& result);
    virtual void addresses(const ACE_INET_Addr& remote_address, const ACE_INET_Addr& local_address);

    bool SendMessage(shared_ptr<IBuffPacket> pBuffPacket, uint8 u1OutputType);
    void Close(int nIOCount = 1);
    bool ServerClose();

    const char* GetError();
    void        SetConnectID(uint32 u4ConnectID);
    uint32      GetConnectID();
    CONNECTSTATE GetConnectState();                                          //�õ�����״̬
    CONNECTSTATE GetSendBuffState();                                         //�õ�����״̬
    bool        GetIsClosing();                                              //�����Ƿ�Ӧ�ùر�

private:
    bool RecvClinetPacket(uint32 u4PackeLen);                                 //�������ݰ�
    bool CheckMessage();                                                      //������յ�����
    bool PutSendPacket(ACE_Message_Block* pMbData);                           //���������ݷ������

private:
    uint32           m_u4HandlerID;                  //��Hander����ʱ��ID
    uint32           m_u4ConnectID;                  //��ǰConnect����ˮ��
    uint32           m_u4AllRecvCount;               //��ǰ���ӽ������ݰ��ĸ���
    uint32           m_u4AllSendCount;               //��ǰ���ӷ������ݰ��ĸ���
    uint32           m_u4AllRecvSize;                //��ǰ���ӽ����ֽ�����
    uint32           m_u4AllSendSize;                //��ǰ���ӷ����ֽ�����
    uint32           m_u4SendThresHold;              //���ͷ�ֵ(��Ϣ���ĸ���)
    uint32           m_u4SendCheckTime;              //���ͼ��ʱ��ķ�ֵ
    int              m_u4RecvPacketCount;            //���ܰ��ĸ���
    int              m_nIOCount;                     //��ǰIO�����ĸ���
    uint16           m_u2SendQueueMax;               //���Ͷ�����󳤶�
    uint16           m_u2MaxConnectTime;             //�������ʱ���ж�
    CONNECTSTATE     m_u1ConnectState;               //Ŀǰ���Ӵ���״̬
    CONNECTSTATE     m_u1SendBuffState;              //Ŀǰ�������Ƿ��еȴ����͵�����
    bool             m_blCanWrite;                   //��һ�����ݰ��Ƿ��ͽ���
    bool             m_blTimeClose;                  //�Ƿ����ڹر�
    char             m_szError[MAX_BUFF_500];
    ACE_INET_Addr    m_addrRemote;
    PSS_Time_Point   m_atvConnect;
    PSS_Time_Point   m_atvInput;
    PSS_Time_Point   m_atvOutput;
    PSS_Time_Point   m_atvSendAlive;

    shared_ptr<CConsolePacketParse> m_pPacketParse;             //���ݰ�������

    ACE_Recursive_Thread_Mutex m_ThreadWriteLock;

    Fast_Asynch_Read_Stream  m_Reader;
    Fast_Asynch_Write_Stream m_Writer;
};

#endif
