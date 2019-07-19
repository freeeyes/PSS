#ifndef _LOGICTHREADMANAGER_H
#define _LOGICTHREADMANAGER_H

#include "ace/Synch.h"
#include "ace/Malloc_T.h"
#include "ace/Singleton.h"
#include "ace/Thread_Mutex.h"

#include "define.h"
#include "HashTable.h"
#include "IMessageQueueManager.h"
#include "ObjectPoolManager.h"
#include "BaseTask.h"
#include "TimerManager.h"

const uint16 LOGICTHREAD_MAX_COUNT = 100;
const uint32 LOGICTHREAD_MESSAGE_MAX_COUNT = 2000;

const uint16 LOGICTHREAD_CREATE_TIMEWAIT = 1000;
const uint16 LOGICTHREAD_CLOSE_TIMEWAIT = 1000;

//逻辑线程的消息体
class CLogicThreadMessage
{
public:
    CLogicThreadMessage() : m_nHashID(0), m_nMessageID(0), m_pParam(NULL)
    {
        //指针关系也可以在这里直接指定，不必使用的使用再指定
        m_pmbQueuePtr = new ACE_Message_Block(sizeof(CLogicThreadMessage*));

        CLogicThreadMessage** ppMessage = (CLogicThreadMessage**)m_pmbQueuePtr->base();
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

    void SetHashID(int nHasnID)
    {
        m_nHashID = nHasnID;
    }

    int GetHashID()
    {
        return m_nHashID;
    }

    void Clear()
    {
        m_nMessageID = 0;
        m_pParam     = NULL;
    }

    ACE_Message_Block* GetQueueMessage()
    {
        return m_pmbQueuePtr;
    }

    int                 m_nHashID;
    int                 m_nMessageID;
    void*               m_pParam;
    ACE_Message_Block*  m_pmbQueuePtr;        //消息队列指针块
};

//逻辑线程参数
class CLogicThreadInfo
{
public:
    CLogicThreadInfo() : m_nLogicThreadID(0),
        m_nTimeout(0),
        m_pLogicQueue(NULL)
    {
    }

    int          m_nLogicThreadID;
    int          m_nTimeout;
    ILogicQueue* m_pLogicQueue;
};

//Message对象池
class CLogicThreadMessagePool : public CObjectPoolManager<CLogicThreadMessage, ACE_Recursive_Thread_Mutex>
{
public:
    CLogicThreadMessagePool();

    static void Init_Callback(int nIndex, CLogicThreadMessage* pMessage);
    static void Close_Callback(int nIndex, CLogicThreadMessage* pMessage);

    CLogicThreadMessage* Create();
    bool Delete(CLogicThreadMessage* pMakePacket);
};

//插件逻辑线程
class CLogicThread : public ACE_Task<ACE_MT_SYNCH>
{
public:
    CLogicThread();

    void Init(CLogicThreadInfo objThreadInfo);

    bool Start();

    virtual int handle_signal(int signum,
                              siginfo_t* = 0,
                              ucontext_t* = 0);

    virtual int open(void* args = 0);
    virtual int svc(void);
    int Close();

    bool CheckTimeout(ACE_Time_Value const& tvNow);

    bool PutMessage(int nMessageID, void* pParam);

    CLogicThreadInfo* GetThreadInfo();

private:
    int CloseMsgQueue();
    bool Dispose_Queue();                                  //队列消费

    CLogicThreadInfo                m_objThreadInfo;
    bool                            m_blRun;
    ACE_Thread_Mutex                m_logicthreadmutex;
    ACE_Condition<ACE_Thread_Mutex> m_logicthreadcond;
    uint32                          m_u4ThreadState;       //当前工作线程状态
    ACE_Time_Value                  m_tvUpdateTime;        //线程最后处理数据的时间
    CLogicThreadMessagePool         m_MessagePool;         //消息池
};

//逻辑线程管理器
class CMessageQueueManager : public IMessageQueueManager, public ACE_Task<ACE_MT_SYNCH>
{
public:
    CMessageQueueManager();

    virtual int handle_timeout(const ACE_Time_Value& tv, const void* arg);

    void Init();

    void Close();

    //创建逻辑线程
    int CreateLogicThread(int nLogicThreadID,
                          int nTimeout,
                          ILogicQueue* pLogicQueue);

    virtual int CreateLogicThread(ILogicQueue* pLogicQueue)
    {

        return CreateLogicThread(pLogicQueue->GetLogicThreadID(),
                                 pLogicQueue->GetTimeOut(),
                                 pLogicQueue);
    };

    //关闭逻辑线程
    virtual int KillLogicThread(int nLogicThreadID);

    //添加消息和逻辑线程的映射关系
    virtual int MessageMappingLogicThread(int nLogicThreadID, int nMessageID);

    //发送线程消息
    virtual int SendLogicThreadMessage(int nMessageID, void* arg);

    CHashTable<CLogicThread>     m_objThreadInfoList;
    CHashTable<CLogicThreadInfo> m_objMessageIDList;
    ACE_Recursive_Thread_Mutex   m_ThreadWriteLock;
    uint32                       m_u4TimerID;
};

typedef ACE_Singleton<CMessageQueueManager, ACE_Null_Mutex> App_MessageQueueManager;

#endif
