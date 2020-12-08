#ifndef _RETTYHANDLER_H
#define _RETTYHANDLER_H

//TTy�豸�����reactorʵ��
//add by freeeyes

#include "ace/Event_Handler.h"
#include "ace/Message_Queue.h"
#include "ace/Synch.h"
#include "ace/Reactor.h"
#include "ace/TTY_IO.h"
#include "ace/DEV_Connector.h"
#include "ITTyMessage.h"
#include "define.h"
#include "LoadPacketParse.h"
#include "MessageBlockManager.h"
#include "BaseHander.h"
#include "TcpRedirection.h"
#include "IHandler.h"

class CReTTyHandler : public ACE_Event_Handler, public IHandler
{
public:
    CReTTyHandler();
    ~CReTTyHandler() final;

    bool ConnectTTy();                          //����ָ�����豸

    void Close(uint32 u4ConnectID) final;
    bool SendMessage(const CSendMessageInfo& objSendMessageInfo, uint32& u4PacketSize) final;
    bool PutSendPacket(uint32 u4ConnectID, ACE_Message_Block* pMbData, uint32 u4Size, const ACE_Time_Value& tvSend) final;
    void SetIsLog(bool blIsLog) final;

    bool Init(uint32 u4ConnectID,
              const char* pName,
              ACE_TTY_IO::Serial_Params inParams,
              ITTyMessage* pTTyMessage,
              EM_CONNECT_IO_DISPOSE emDispose = EM_CONNECT_IO_DISPOSE::CONNECT_IO_PLUGIN,
              uint32 u4PacketParseInfoID = 0);

    bool GetConnectState();
    ACE_TTY_IO::Serial_Params GetParams();
    void SetPause(bool blPause);                //������ͣ״̬
    bool GetPause();                            //�õ���ͣ״̬

    int handle_input(ACE_HANDLE handle);
    int handle_signal(int signum, siginfo_t* psinfo, ucontext_t* context);
    int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask);
    int handle_timeout(const ACE_Time_Value& tvNow, const void*  p);

    bool Send_Data(const char* pData, ssize_t nLen);                         //���豸��������
    bool Device_Send_Data(const char* pData, ssize_t nLen) final;            //͸�����ݽӿ�
    uint32 GetConnectID();

private:
    void Send_Hander_Event(uint8 u1Option);                                  //����Handler���¼�֪ͨҵ���߳�

private:
    char                                  m_szName[MAX_BUFF_100] = {'\0'};
    ACE_TTY_IO                            m_ReTtyio;
    ACE_DEV_Connector                     m_ReConnector;
    ACE_Message_Block*                    m_pmbReadBuff          = nullptr;
    ACE_TTY_IO::Serial_Params             m_ObjParams;                                //�豸�ӿڲ���
    bool                                  m_blState              = false;             //��ǰ�豸����״̬
    bool                                  m_blPause              = false;             //�Ƿ���ͣ
    uint32                                m_u4ConnectID          = 0;                 //��ǰ�豸ID
    ITTyMessage*                          m_pTTyMessage          = nullptr;              //TTyMessage����
    EM_CONNECT_IO_DISPOSE                 m_emDispose            = EM_CONNECT_IO_DISPOSE::CONNECT_IO_PLUGIN; //����ģʽ����ܴ��� or ҵ����
    uint32                                m_u4PacketParseInfoID  = 0;                 //��ܴ���ģ��ID
    string                                m_strDeviceName;                            //ת���ӿ�����
    shared_ptr<_Packet_Parse_Info>        m_pPacketParse         = nullptr;           //���ݰ�Packetparse�����ӿ�
};

#endif
