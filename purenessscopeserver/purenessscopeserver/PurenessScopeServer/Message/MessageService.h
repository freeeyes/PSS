#ifndef _MESSAGESERVICE_H
#define _MESSAGESERVICE_H

#include "define.h"
#include "ace/Task.h"
#include "ace/Synch.h"
#include "ace/Malloc_T.h"
#include "ace/Singleton.h"
#include "ace/Thread_Mutex.h"
#include "ace/Date_Time.h"

#include "Message.h"
#include "MessageManager.h"
#include "LogManager.h"
#include "ThreadInfo.h"
#include "BuffPacket.h"
#include "MainConfig.h"
#include "TimerManager.h"
#include "RandomNumber.h"
#include "WorkThreadAI.h"
#include "CommandAccount.h"
#include "MessageDyeingManager.h"

#ifdef WIN32
#include "ProConnectHandle.h"
#include "WindowsCPU.h"
#else
#include "ConnectHandler.h"
#include "LinuxCPU.h"
#endif

//AI配置信息表
typedef vector<_WorkThreadAIInfo> vecWorkThreadAIInfo;

enum MESSAGE_SERVICE_THREAD_STATE
{
    THREAD_RUN = 0,               //线程正常运行
    THREAD_MODULE_UNLOAD,         //模块重载，需要线程支持此功能
    THREAD_STOP,                  //线程停止
};

//工作线程时序模式
class CMessageService : public ACE_Task<ACE_MT_SYNCH>
{
public:
    CMessageService();
    ~CMessageService();

    virtual int handle_signal (int signum,
                               siginfo_t*   = 0,
                               ucontext_t* = 0);

    virtual int open(void* args = 0);
    virtual int svc (void);
    int Close();

    bool SaveThreadInfo();

    void Init(uint32 u4ThreadID, uint32 u4MaxQueue = MAX_MSG_THREADQUEUE, uint32 u4LowMask = MAX_MSG_MASK, uint32 u4HighMask = MAX_MSG_MASK);

    bool Start();

    bool PutMessage(CMessage* pMessage);
    bool PutUpdateCommandMessage(uint32 u4UpdateIndex);

    _ThreadInfo* GetThreadInfo();

    void GetAIInfo(_WorkThreadAIInfo& objAIInfo);           //得到所有工作线程的AI配置
    void GetAITO(vecCommandTimeout& objTimeout);            //得到所有的AI超时数据包信息
    void GetAITF(vecCommandTimeout& objTimeout);            //得到所有的AI封禁数据包信息
    void SetAI(uint8 u1AI, uint32 u4DisposeTime, uint32 u4WTCheckTime, uint32 u4WTStopTime);  //设置AI

    _CommandData* GetCommandData(uint16 u2CommandID);                      //得到命令相关信息
    _CommandFlowAccount GetCommandFlowAccount();                           //得到流量相关信息
    void GetCommandTimeOut(vecCommandTimeOut& CommandTimeOutList);         //得到所有超时命令
    void GetCommandAlertData(vecCommandAlertData& CommandAlertDataList);   //得到所有超过告警阀值的命令
    void ClearCommandTimeOut();                                            //清除所有的超时告警
    void SaveCommandDataLog();                                             //存储统计日志
    void SetThreadState(MESSAGE_SERVICE_THREAD_STATE emState);             //设置线程状态
    MESSAGE_SERVICE_THREAD_STATE GetThreadState();                         //得到当前线程状态
    uint32 GetStepState();                                                 //得到当前步数相关信息
    uint32 GetUsedMessageCount();                                          //得到正在使用的Message对象个数

    uint32 GetThreadID();

    void CopyMessageManagerList();                                         //从MessageManager中获得信令列表副本

    CMessage* CreateMessage();
    void DeleteMessage(CMessage* pMessage);

private:
    bool ProcessMessage(CMessage* pMessage, uint32 u4ThreadID);
    bool SaveThreadInfoData();
    void CloseCommandList();                                                //清理当前信令列表副本
    CClientCommandList* GetClientCommandList(uint16 u2CommandID);
    bool DoMessage(ACE_Time_Value& tvBegin, IMessage* pMessage, uint16& u2CommandID, uint32& u4TimeCost, uint16& u2Count, bool& bDeleteFlag);

    virtual int CloseMsgQueue();

private:
    uint64                         m_u8TimeCost;           //Put到队列信息的数据处理时间
    uint32                         m_u4ThreadID;           //当前线程ID
    uint32                         m_u4MaxQueue;           //线程中最大消息对象个数
    uint32                         m_u4HighMask;
    uint32                         m_u4LowMask;
    uint32                         m_u4Count;              //消息队列接受个数
    uint32                         m_u4WorkQueuePutTime;   //入队超时时间
    uint16                         m_u2ThreadTimeOut;
    uint16                         m_u2ThreadTimeCheck;
    bool                           m_blRun;                //线程是否在运行

    MESSAGE_SERVICE_THREAD_STATE   m_emThreadState;        //当前工作线程状态

    _ThreadInfo                    m_ThreadInfo;           //当前线程信息
    CWorkThreadAI                  m_WorkThreadAI;         //线程自我监控的AI逻辑
    CCommandAccount                m_CommandAccount;       //当前线程命令统计数据
    CMessagePool                   m_MessagePool;          //消息池

    CHashTable<CClientCommandList> m_objClientCommandList; //可执行的信令列表

    ACE_Thread_Mutex m_mutex;
    ACE_Condition<ACE_Thread_Mutex> m_cond;
};

//add by freeeyes
//添加线程管理，用户可以创建若干个ACE_Task，每个Task对应一个线程，一个Connectid只对应一个线程。
class CMessageServiceGroup : public ACE_Task<ACE_MT_SYNCH>
{
public:
    CMessageServiceGroup();
    ~CMessageServiceGroup();

    virtual int handle_timeout(const ACE_Time_Value& tv, const void* arg);

    bool Init(uint32 u4ThreadCount = MAX_MSG_THREADCOUNT, uint32 u4MaxQueue = MAX_MSG_THREADQUEUE, uint32 u4LowMask = MAX_MSG_MASK, uint32 u4HighMask = MAX_MSG_MASK);
    bool PutMessage(CMessage* pMessage);                                                     //发送到相应的线程去处理
    bool PutUpdateCommandMessage(uint32 u4UpdateIndex);                                      //发送消息同步所有的工作线程命令副本
    void Close();

    bool Start();
    CThreadInfo* GetThreadInfo();
    uint32 GetUsedMessageCount();

    uint32 GetWorkThreadCount();                                                              //得到当前工作线程的数量
    uint32 GetWorkThreadIDByIndex(uint32 u4Index);                                            //得到指定工作线程的线程ID
    void GetWorkThreadAIInfo(vecWorkThreadAIInfo& objvecWorkThreadAIInfo);                    //得到线程工作AI配置信息
    void GetAITO(vecCommandTimeout& objTimeout);                                              //得到所有的AI超时数据包信息
    void GetAITF(vecCommandTimeout& objTimeout);                                              //得到所有的AI封禁数据包信息
    void SetAI(uint8 u1AI, uint32 u4DisposeTime, uint32 u4WTCheckTime, uint32 u4WTStopTime);  //设置AI

    void GetCommandData(uint16 u2CommandID, _CommandData& objCommandData);                    //获得指定命令统计信息
    void GetFlowInfo(_CommandFlowAccount& objCommandFlowAccount);                             //获得指定流量相关信息

    void GetCommandTimeOut(vecCommandTimeOut& CommandTimeOutList);                            //得到所有超时命令
    void GetCommandAlertData(vecCommandAlertData& CommandAlertDataList);                      //得到所有超过告警阀值的命令
    void ClearCommandTimeOut();                                                               //清理所有的超时告警
    void SaveCommandDataLog();                                                                //存储统计日志

    CMessage* CreateMessage(uint32 u4ConnectID, uint8 u1PacketType);                          //从子线程中获取一个Message对象
    void DeleteMessage(uint32 u4ConnectID, CMessage* pMessage);                               //从子线程中回收一个Message对象

    void CopyMessageManagerList();                                                            //从MessageManager中获得信令列表副本

    void AddDyringIP(const char* pClientIP, uint16 u2MaxCount);                               //染色指定的IP
    bool AddDyeingCommand(uint16 u2CommandID, uint16 u2MaxCount);                             //染色指定的CommandID
    void GetDyeingCommand(vec_Dyeing_Command_list& objList);                                  //获得当前命令染色状态

private:
    bool StartTimer();
    bool KillTimer();

    bool CheckWorkThread();                                                                  //检查所有的工作线程状态
    bool CheckPacketParsePool();                                                             //检查正在使用的消息解析对象
    bool CheckCPUAndMemory();                                                                //检查CPU和内存
    bool CheckPlugInState();                                                                 //检查所有插件状态
    int32 GetWorkThreadID(uint32 u4ConnectID, uint8 u1PacketType);                           //根据操作类型和ConnectID计算出那个工作线程ID

private:
    typedef vector<CMessageService*> vecMessageService;
    vecMessageService m_vecMessageService;

public:
    uint32                     m_u4MaxQueue;              //线程中最大消息对象个数
    uint32                     m_u4HighMask;              //线程高水位
    uint32                     m_u4LowMask;               //线程低水位
    uint32                     m_u4TimerID;               //定时器ID
    uint16                     m_u2ThreadTimeCheck;       //线程自检时间
    CThreadInfo                m_objAllThreadInfo;        //当前所有线程信息
    CRandomNumber              m_objRandomNumber;         //随机数，仅UDP使用
    CMessageDyeingManager      m_objMessageDyeingManager; //数据染色类
};

typedef ACE_Singleton<CMessageServiceGroup,ACE_Null_Mutex> App_MessageServiceGroup;
#endif
