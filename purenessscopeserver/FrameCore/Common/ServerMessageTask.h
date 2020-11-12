#ifndef _SERVERMESSAGETASK_H
#define _SERVERMESSAGETASK_H

#include "ace/Synch.h"
#include "ace/Malloc_T.h"
#include "ace/Singleton.h"
#include "ace/Thread_Mutex.h"
#include "ace/Date_Time.h"

#include "IClientManager.h"
#include "MessageBlockManager.h"
#include "HashTable.h"
#include "ObjectArrayList.h"
#include "BaseTask.h"
#include <string>
#include <sstream>

//�����������������ݰ����̴���
//������������̴߳�������ˣ��᳢����������
//add by freeeyes

const uint32 MAX_SERVER_MESSAGE_QUEUE = 1000;    //���������г���
const uint32 MAX_DISPOSE_TIMEOUT      = 30;      //�������ȴ�����ʱ��

const uint32 ADD_SERVER_CLIENT = ACE_Message_Block::MB_USER + 1;    //���ClientMessage�첽����
const uint32 DEL_SERVER_CLIENT = ACE_Message_Block::MB_USER + 2;    //ɾ��ClientMessage�첽����

//��������ͨѶ�����ݽṹ�����հ���
class _Server_Message_Info
{
public:
    int                m_nHashID        = 0;
    uint16             m_u2CommandID    = 0;
    IClientMessage*    m_pClientMessage = NULL;
    ACE_Message_Block* m_pRecvFinish    = NULL;
    ACE_Message_Block* m_pmbQueuePtr    = NULL;        //��Ϣ����ָ���
    _ClientIPInfo      m_objServerIPInfo;

    _Server_Message_Info()
    {
        //����������Ϣ����ģ��ָ�����ݣ������Ͳ��ط�����new��delete����������
        //ָ���ϵҲ����������ֱ��ָ��������ʹ�õ�ʹ����ָ��
        m_pmbQueuePtr = new ACE_Message_Block(sizeof(_Server_Message_Info*));

        auto ppMessage = (_Server_Message_Info**)m_pmbQueuePtr->base();
        *ppMessage = this;
    }

    void Close()
    {
        if (NULL != m_pmbQueuePtr)
        {
            m_pmbQueuePtr->release();
            m_pmbQueuePtr = NULL;
        }
    }

    ACE_Message_Block* GetQueueMessage()
    {
        return m_pmbQueuePtr;
    }

    void SetHashID(int nHashID)
    {
        m_nHashID = nHashID;
    }

    int GetHashID() const
    {
        return m_nHashID;
    }

};

const uint32 MAX_SERVER_MESSAGE_INFO_COUNT = 100;

//_Server_Message_Info�����
class CServerMessageInfoPool
{
public:
    CServerMessageInfoPool();

    void Init(uint32 u4PacketCount = MAX_SERVER_MESSAGE_INFO_COUNT);
    void Close();

    _Server_Message_Info* Create();
    bool Delete(_Server_Message_Info* pMakePacket);

    int GetUsedCount();
    int GetFreeCount();

private:
    CObjectArrayList<_Server_Message_Info> m_objArrayList;              //_Server_Message_Info�ڴ�����б�
    CHashTable<_Server_Message_Info> m_objServerMessageList;           //Server Message�����
    ACE_Recursive_Thread_Mutex       m_ThreadWriteLock;                //���ƶ��߳���
};

//�����������ݰ���Ϣ���д������
class CServerMessageTask : public ACE_Task<ACE_MT_SYNCH>
{
public:
    CServerMessageTask();

    int open();
    virtual int svc (void);

    virtual int handle_signal (int signum,siginfo_t*   = 0,ucontext_t* = 0);

    bool Start();
    int  Close();

    uint32 GetThreadID() const;

    bool PutMessage(_Server_Message_Info* pMessage);

    bool PutMessage_Add_Client(IClientMessage* pClientMessage);

    bool PutMessage_Del_Client(IClientMessage* pClientMessage);

    bool CheckServerMessageThread(ACE_Time_Value const& tvNow) const;

private:
    bool CheckValidClientMessage(const IClientMessage* pClientMessage) const;
    bool ProcessMessage(const _Server_Message_Info* pMessage, uint32 u4ThreadID);

    virtual int CloseMsgQueue();

    void Add_ValidIClientMessage(IClientMessage* pClientMessage);
    void Update_ValidIClientMessage(const IClientMessage* pClientMessage);

    //�ر���Ϣ������������
    ACE_Thread_Mutex m_mutex;
    ACE_Condition<ACE_Thread_Mutex> m_cond;
    uint32               m_u4ThreadID         = 0;  //��ǰ�߳�ID
    bool                 m_blRun              = false;       //��ǰ�߳��Ƿ�����
    uint32               m_u4MaxQueue         = MAX_SERVER_MESSAGE_QUEUE;  //�ڶ����е�����������
    EM_Server_Recv_State m_emState            = EM_Server_Recv_State::SERVER_RECV_INIT;     //����״̬
    ACE_Time_Value       m_tvDispose;   //�������ݰ�����ʱ��

    //��¼��ǰ��Ч��IClientMessage����Ϊ���첽�Ĺ�ϵ��
    //������뱣֤�ص���ʱ��IClientMessage�ǺϷ��ġ�
    typedef vector<IClientMessage*> vecValidIClientMessage;
    vecValidIClientMessage m_vecValidIClientMessage;
};

class CServerMessageManager
{
public:
    CServerMessageManager();

    void Init();

    bool Start();
    int  Close();
    bool PutMessage(_Server_Message_Info* pMessage);
    bool CheckServerMessageThread(ACE_Time_Value const& tvNow) const;

    bool AddClientMessage(IClientMessage* pClientMessage);
    bool DelClientMessage(IClientMessage* pClientMessage);

private:
    CServerMessageTask*         m_pServerMessageTask = NULL;
    ACE_Recursive_Thread_Mutex  m_ThreadWritrLock;
};

typedef ACE_Singleton<CServerMessageManager, ACE_Recursive_Thread_Mutex> App_ServerMessageTask;
typedef ACE_Singleton<CServerMessageInfoPool, ACE_Recursive_Thread_Mutex> App_ServerMessageInfoPool;
#endif
