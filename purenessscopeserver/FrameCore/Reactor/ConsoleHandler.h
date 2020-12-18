#ifndef _CONSOLEHANDLE_H
#define _CONSOLEHANDLE_H

//�����ڶԷ��������Ƶ�ģ�飬����Զ�̿����������
//������汾��һֱ��ӣ���������ſ�ʼ�㣬��Ҫ���Լ�������ξ������ٸ��Լ�������ɡ�
//add by freeeyes
#include "define.h"
#include "TimeStamp.hpp"

#include "ace/Reactor.h"
#include "ace/Svc_Handler.h"
#include "ace/Synch.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Reactor_Notification_Strategy.h"

#include "AceReactorManager.h"
#include "MessageService.h"
#include "IConnectManager.h"
#include "BaseConsoleHandle.h"
#include "PacketConsoleParse.h"

class CConsoleHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>
{
public:
    CConsoleHandler(void) = default;

    //��д�̳з���
    int open(void*) final;                                                //�û�����һ������
    int handle_input(ACE_HANDLE fd = ACE_INVALID_HANDLE) final;
    int handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask) final;

    bool SendMessage(shared_ptr<IBuffPacket> pBuffPacket, uint8 u1OutputType);

    void Close(int nIOCount = 1);
    bool ServerClose();

    const char*  GetError() const;
    void         SetConnectID(uint32 u4ConnectID);
    uint32       GetConnectID() const;
    CONNECTSTATE GetConnectState() const;                                     //�õ�����״̬
    CONNECTSTATE GetSendBuffState() const;                                    //�õ�����״̬
    uint8        GetIsClosing() const;                                        //�����Ƿ�Ӧ�ùر�

private:
    bool CheckMessage();                                                      //������յ�����
    bool PutSendPacket(ACE_Message_Block* pMbData);                           //��������
    void Clear_PacketParse();                                                 //������Ҫ��PacketParse
    bool CompareConsoleClinetIP(const char* pIP) const;                       //���һ��Console���ӵ�IP�Ƿ���ָ���������б���

    uint32                     m_u4ConnectID           = 0;                  //���ӵ�ID
    uint32                     m_u4AllRecvCount        = 0;                  //��ǰ���ӽ������ݰ��ĸ���
    uint32                     m_u4AllSendCount        = 0;                  //��ǰ���ӷ������ݰ��ĸ���
    uint32                     m_u4AllRecvSize         = 0;                  //��ǰ���ӽ����ֽ�����
    uint32                     m_u4AllSendSize         = 0;                  //��ǰ���ӷ����ֽ�����
    uint32                     m_u4CurrSize            = 0;                  //��ǰMB�����ַ�����
    int                        m_nIOCount              = 1;                  //��ǰIO�����ĸ���
    CONNECTSTATE               m_u1ConnectState        = CONNECTSTATE::CONNECT_INIT;       //Ŀǰ���Ӵ���״̬
    CONNECTSTATE               m_u1SendBuffState       = CONNECTSTATE::CONNECT_SENDNON;    //Ŀǰ�������Ƿ��еȴ����͵�����
    uint8                      m_u1IsClosing           = HANDLE_ISCLOSE_NO;  //�Ƿ�Ӧ�ùر� 0Ϊ��1Ϊ��
    string                     m_strError;
    ACE_INET_Addr              m_addrRemote;
    PSS_Time_Point             m_atvConnect;
    PSS_Time_Point             m_atvInput;
    PSS_Time_Point             m_atvOutput;
    PSS_Time_Point             m_atvSendAlive;
    ACE_Recursive_Thread_Mutex m_ThreadLock;
    _TimerCheckID*                  m_pTCClose             = nullptr;
    shared_ptr<CConsolePacketParse> m_pPacketParse         = nullptr;                 //���ݰ�������
    ACE_Message_Block*              m_pCurrMessage         = nullptr;                 //��ǰ��MB����
};

#endif
