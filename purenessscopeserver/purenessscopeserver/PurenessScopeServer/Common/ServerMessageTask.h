#ifndef _SERVERMESSAGETASK_H
#define _SERVERMESSAGETASK_H

#include "define.h"
#include "ace/Task.h"
#include "ace/Synch.h"
#include "ace/Malloc_T.h"
#include "ace/Singleton.h"
#include "ace/Thread_Mutex.h"
#include "ace/Date_Time.h"

#include "IClientManager.h"
#include "MessageBlockManager.h"
#include "HashTable.h"

using namespace std;

//处理服务器间接收数据包过程代码
//如果服务器间线程处理挂起了，会尝试重启服务
//add by freeeyes

#define MAX_SERVER_MESSAGE_QUEUE 1000    //允许最大队列长度
#define MAX_DISPOSE_TIMEOUT      30      //允许最大等待处理时间  

#define ADD_SERVER_CLIENT  ACE_Message_Block::MB_USER + 1    //添加ClientMessage异步对象
#define DEL_SERVER_CLIENT  ACE_Message_Block::MB_USER + 2    //删除ClientMessage异步对象

//服务器间通讯的数据结构（接收包）
struct _Server_Message_Info
{
    IClientMessage*    m_pClientMessage;
    uint16             m_u2CommandID;
    ACE_Message_Block* m_pRecvFinish;
    _ClientIPInfo      m_objServerIPInfo;
    int                m_nHashID;

    ACE_Message_Block* m_pmbQueuePtr;        //消息队列指针块

    _Server_Message_Info()
    {
        m_nHashID        = 0;
        m_u2CommandID    = 0;
        m_pClientMessage = NULL;
        m_pRecvFinish    = NULL;

        //这里设置消息队列模块指针内容，这样就不必反复的new和delete，提升性能
        //指针关系也可以在这里直接指定，不必使用的使用再指定
        m_pmbQueuePtr  = new ACE_Message_Block(sizeof(_Server_Message_Info*));

        _Server_Message_Info** ppMessage = (_Server_Message_Info**)m_pmbQueuePtr->base();
        *ppMessage = this;
    }

    //拷贝构造函数
    _Server_Message_Info(const _Server_Message_Info& ar)
    {
        this->m_pClientMessage           = ar.m_pClientMessage;
        this->m_u2CommandID              = ar.m_u2CommandID;
        this->m_pRecvFinish              = ar.m_pRecvFinish;
        sprintf_safe(this->m_objServerIPInfo.m_szClientIP, MAX_BUFF_50, "%s", ar.m_objServerIPInfo.m_szClientIP);
        this->m_objServerIPInfo.m_nPort  = ar.m_objServerIPInfo.m_nPort;
        this->m_nHashID                  = ar.m_nHashID;
        this->m_pmbQueuePtr              = new ACE_Message_Block(sizeof(_Server_Message_Info*));
        _Server_Message_Info** ppMessage = (_Server_Message_Info**)m_pmbQueuePtr->base();
        *ppMessage = this;
    }

    _Server_Message_Info& operator = (const _Server_Message_Info& ar)
    {
        this->m_pClientMessage = ar.m_pClientMessage;
        this->m_u2CommandID = ar.m_u2CommandID;
        this->m_pRecvFinish = ar.m_pRecvFinish;
        this->m_objServerIPInfo = ar.m_objServerIPInfo;
        this->m_nHashID = ar.m_nHashID;

        memcpy_safe((char* )ar.m_pmbQueuePtr->base(), (uint32)ar.m_pmbQueuePtr->length(), m_pmbQueuePtr->base(), (uint32)m_pmbQueuePtr->length());
        _Server_Message_Info** ppMessage = (_Server_Message_Info**)m_pmbQueuePtr->base();
        *ppMessage = this;
        return *this;
    }

    ~_Server_Message_Info()
    {
        if(NULL != m_pmbQueuePtr)
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

    int GetHashID()
    {
        return m_nHashID;
    }

};

#define MAX_SERVER_MESSAGE_INFO_COUNT 100

//_Server_Message_Info对象池
class CServerMessageInfoPool
{
public:
    CServerMessageInfoPool();
    ~CServerMessageInfoPool();

    void Init(uint32 u4PacketCount = MAX_SERVER_MESSAGE_INFO_COUNT);
    void Close();

    _Server_Message_Info* Create();
    bool Delete(_Server_Message_Info* pMakePacket);

    int GetUsedCount();
    int GetFreeCount();

private:
    CHashTable<_Server_Message_Info> m_objServerMessageList;           //Server Message缓冲池
    ACE_Recursive_Thread_Mutex       m_ThreadWriteLock;                //控制多线程锁
};

//服务器间数据包消息队列处理过程
class CServerMessageTask : public ACE_Task<ACE_MT_SYNCH>
{
public:
    CServerMessageTask();
    ~CServerMessageTask();

    virtual int open(void* args = 0);
    virtual int svc (void);

    virtual int handle_signal (int signum,siginfo_t*   = 0,ucontext_t* = 0);

    bool Start();
    int  Close();

    uint32 GetThreadID();

    bool PutMessage(_Server_Message_Info* pMessage);

    bool PutMessage_Add_Client(IClientMessage* pClientMessage);

    bool PutMessage_Del_Client(IClientMessage* pClientMessage);

    bool CheckServerMessageThread(ACE_Time_Value tvNow);

private:
    bool CheckValidClientMessage(IClientMessage* pClientMessage);
    bool ProcessMessage(_Server_Message_Info* pMessage, uint32 u4ThreadID);

    virtual int CloseMsgQueue();

private:
    //关闭消息队列条件变量
    ACE_Thread_Mutex m_mutex;
    ACE_Condition<ACE_Thread_Mutex> m_cond;
private:
    uint32               m_u4ThreadID;  //当前线程ID
    bool                 m_blRun;       //当前线程是否运行
    uint32               m_u4MaxQueue;  //在队列中的数据最多个数
    EM_Server_Recv_State m_emState;     //处理状态
    ACE_Time_Value       m_tvDispose;   //接收数据包处理时间

    //记录当前有效的IClientMessage，因为是异步的关系。
    //这里必须保证回调的时候IClientMessage是合法的。
    typedef vector<IClientMessage*> vecValidIClientMessage;
    vecValidIClientMessage m_vecValidIClientMessage;
};

class CServerMessageManager
{
public:
    CServerMessageManager();
    ~CServerMessageManager();

    void Init();

    bool Start();
    int  Close();
    bool PutMessage(_Server_Message_Info* pMessage);
    bool CheckServerMessageThread(ACE_Time_Value tvNow);

    bool AddClientMessage(IClientMessage* pClientMessage);
    bool DelClientMessage(IClientMessage* pClientMessage);

private:
    CServerMessageTask*         m_pServerMessageTask;
    ACE_Recursive_Thread_Mutex  m_ThreadWritrLock;
};

typedef ACE_Singleton<CServerMessageManager, ACE_Recursive_Thread_Mutex> App_ServerMessageTask;
typedef ACE_Singleton<CServerMessageInfoPool, ACE_Recursive_Thread_Mutex> App_ServerMessageInfoPool;
#endif
