#ifndef _LOGICTHREADMANAGER_H
#define _LOGICTHREADMANAGER_H

#include "ace/Synch.h"
#include "ace/Malloc_T.h"
#include "ace/Singleton.h"
#include "ace/Thread_Mutex.h"

#include "define.h"
#include "HashTable.h"
#include "ILogicThreadManager.h"
#include "ObjectPoolManager.h"
#include "BaseTask.h"
#include "TimerManager.h"

#define LOGICTHREAD_MAX_COUNT         100
#define LOGICTHREAD_MESSAGE_MAX_COUNT 2000

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

    ~CLogicThreadMessage()
    {
        if (NULL != m_pmbQueuePtr)
        {
            m_pmbQueuePtr->release();
            m_pmbQueuePtr = NULL;
        }
    }

    CLogicThreadMessage(const CLogicThreadMessage& ar)
    {
        (*this) = ar;
    }

    CLogicThreadMessage& operator = (const CLogicThreadMessage& ar)
    {
        if (this != &ar)
        {
            this->m_nMessageID = ar.m_nMessageID;
            this->m_pParam = ar.m_pParam;
        }

        return *this;
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
        fn_thread_init(NULL),
        fn_thread_callback_logic(NULL),
        fn_thread_callback_error(NULL),
        fn_thread_exit(NULL)
    {
    }

    ~CLogicThreadInfo()
    {
    }

    CLogicThreadInfo(const CLogicThreadInfo& ar)
    {
        (*this) = ar;
    }

    CLogicThreadInfo& operator = (const CLogicThreadInfo& ar)
    {
        if (this != &ar)
        {
            this->m_nLogicThreadID         = ar.m_nLogicThreadID;
            this->m_nTimeout               = ar.m_nTimeout;
            this->fn_thread_init           = ar.fn_thread_init;
            this->fn_thread_callback_logic = ar.fn_thread_callback_logic;
            this->fn_thread_callback_error = ar.fn_thread_callback_error;
            this->fn_thread_exit           = ar.fn_thread_exit;
        }

        return *this;
    }

    int m_nLogicThreadID;
    int m_nTimeout;
    ThreadInit fn_thread_init;
    ThreadCallbackLogic fn_thread_callback_logic;
    ThreadErrorLogic fn_thread_callback_error;
    ThreadExit fn_thread_exit;
};

//Message对象池
class CLogicThreadMessagePool : public CObjectPoolManager<CLogicThreadMessage, ACE_Recursive_Thread_Mutex>
{
public:
    CLogicThreadMessagePool();
    ~CLogicThreadMessagePool();

    CLogicThreadMessagePool(const CLogicThreadMessagePool& ar);

    CLogicThreadMessagePool& operator = (const CLogicThreadMessagePool& ar)
    {
        if (this != &ar)
        {
            ACE_UNUSED_ARG(ar);
        }

        return *this;
    }

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
    virtual ~CLogicThread();

    CLogicThread(const CLogicThread& ar);

    CLogicThread& operator = (const CLogicThread& ar)
    {
        if (this != &ar)
        {
            ACE_UNUSED_ARG(ar);
        }

        return *this;
    }

    void Init(CLogicThreadInfo objThreadInfo);

    bool Start();

    virtual int handle_signal(int signum,
                              siginfo_t* = 0,
                              ucontext_t* = 0);

    virtual int open(void* args = 0);
    virtual int svc(void);
    int Close();

    bool CheckTimeout(ACE_Time_Value tvNow);

    bool PutMessage(int nMessageID, void* pParam);

    CLogicThreadInfo* GetThreadInfo();

private:
    int CloseMsgQueue();
    bool Dispose_Queue();                                  //队列消费

private:
    CLogicThreadInfo                m_objThreadInfo;
    bool                            m_blRun;
    ACE_Thread_Mutex                m_logicthreadmutex;
    ACE_Condition<ACE_Thread_Mutex> m_logicthreadcond;
    uint32                          m_u4ThreadState;       //当前工作线程状态
    ACE_Time_Value                  m_tvUpdateTime;        //线程最后处理数据的时间
    CLogicThreadMessagePool         m_MessagePool;         //消息池
};

//逻辑线程管理器
class CLogicThreadManager : public ILogicThreadManager, public ACE_Task<ACE_MT_SYNCH>
{
public:
    CLogicThreadManager();
    virtual ~CLogicThreadManager();

    virtual int handle_timeout(const ACE_Time_Value& tv, const void* arg);

    void Init();

    void Close();

    //创建逻辑线程
    virtual int CreateLogicThread(int nLogicThreadID,
                                  int nTimeout,
                                  ThreadInit thread_init,
                                  ThreadCallbackLogic thread_callback_logic,
                                  ThreadErrorLogic thread_callback_error,
                                  ThreadExit thread_exit);

    //关闭逻辑线程
    virtual int KillLogicThread(int nLogicThreadID);

    //添加消息和逻辑线程的映射关系
    virtual int MessageMappingLogicThread(int nLogicThreadID, int nMessageID);

    //发送线程消息
    virtual int SendLogicThreadMessage(int nMessageID, void* arg);

public:
    CHashTable<CLogicThread>     m_objThreadInfoList;
    CHashTable<CLogicThreadInfo> m_objMessageIDList;
    ACE_Recursive_Thread_Mutex   m_ThreadWriteLock;
    uint32                       m_u4TimerID;
};

typedef ACE_Singleton<CLogicThreadManager, ACE_Null_Mutex> App_LogicThreadManager;

#endif
