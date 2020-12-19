#ifndef _MESSAGESERVICE_H
#define _MESSAGESERVICE_H

#include "ace/Synch.h"
#include "ace/Malloc_T.h"
#include "ace/Singleton.h"
#include "ace/Thread_Mutex.h"
#include "ace/Date_Time.h"

#include "BaseTask.h"
#include "Message.h"
#include "MessageManager.h"
#include "LogManager.h"
#include "ThreadInfo.h"
#include "BuffPacket.h"
#include "TimerManager.hpp"
#include "WorkThreadAI.h"
#include "MessageDyeingManager.h"
#include "ObjectLru.h"
#include "PerformanceCounter.h"
#include "BuffPacketManager.h"

#if PSS_PLATFORM == PLATFORM_WIN
#include "WindowsCPU.h"
#else
#include "LinuxCPU.h"
#endif

enum class MESSAGE_SERVICE_THREAD_STATE
{
    THREAD_RUN = 0,               //线程正常运行
    THREAD_MODULE_UNLOAD,         //模块重载，需要线程支持此功能
    THREAD_STOP,                  //线程停止
};

class CWorkThread_Packet_Info
{
public:
    uint8  m_u1ThreadID  = 0;
    uint32 m_u4PacketIn  = 0;
    uint32 m_u4PacketOut = 0;
    uint32 m_u4RecvSize  = 0;
    uint32 m_u4SendSize  = 0;
    
    uint8 m_u1Minute    = 0;    //当前时间，按照分钟统计

    void Clear()
    {
        m_u4PacketIn  = 0;
        m_u4PacketOut = 0;
        m_u4RecvSize  = 0;
        m_u4SendSize  = 0;
    }
};

//工作线程数据包和数据量统计
class CWorkThread_Process
{
public:
    CWorkThread_Packet_Info m_objPacketCounter;

    void Clear(uint8 u1NowMinite)
    {
        if (u1NowMinite != m_objPacketCounter.m_u1Minute)
        {
            m_objPacketCounter.Clear();
            m_objPacketCounter.m_u1Minute = u1NowMinite;
        }
    }

    void AddPacketIn(uint32 m_u4PacketIn, uint8 u1Minute)
    {
        Clear(u1Minute);
        
        m_objPacketCounter.m_u4PacketIn++;
        m_objPacketCounter.m_u4RecvSize = m_u4PacketIn;
    }

    void AddPacketOut(uint32 m_u4PacketOut, uint8 u1Minute)
    {
        Clear(u1Minute);

        m_objPacketCounter.m_u4PacketOut++;
        m_objPacketCounter.m_u4SendSize = m_u4PacketOut;
    }

    CWorkThread_Packet_Info GetCurrInfo(uint8 u1Minute)
    {
        Clear(u1Minute);
        return m_objPacketCounter;
    }
};

//工作线程时序模式
class CMessageService
{
public:
    CMessageService();

    int open();
    int svc (void);
    int Close();

    void Init(uint32 u4ThreadID, uint32 u4MaxQueue = MAX_MSG_THREADQUEUE, uint32 u4LowMask = MAX_MSG_MASK, uint32 u4HighMask = MAX_MSG_MASK, bool blIsCpuAffinity = false);

    bool Start();

    bool PutMessage(shared_ptr<CWorkThreadMessageList> pMessage);
    bool PutUpdateCommandMessage(uint32 u4UpdateIndex);

    _ThreadInfo* GetThreadInfo();
    bool SaveThreadInfoData(const PSS_Time_Point& tvNow);   //记录当前工作线程状态信息日志

    void GetAIInfo(_WorkThreadAIInfo& objAIInfo) const;           //得到所有工作线程的AI配置
    void GetAITO(vecCommandTimeout& objTimeout) const;            //得到所有的AI超时数据包信息
    void GetAITF(vecCommandTimeout& objTimeout) const;            //得到所有的AI封禁数据包信息
    void SetAI(uint8 u1AI, uint32 u4DisposeTime, uint32 u4WTCheckTime, uint32 u4WTStopTime);  //设置AI

    void SetThreadState(MESSAGE_SERVICE_THREAD_STATE emState);             //设置线程状态
    MESSAGE_SERVICE_THREAD_STATE GetThreadState() const;                   //得到当前线程状态
    THREADSTATE GetStepState() const;                                      //得到当前步数相关信息
    uint32 GetUsedMessageCount();                                          //得到正在使用的Message对象个数

    uint32 GetThreadID() const;

    uint32 GetHandlerCount() const;                                        //得到当前线程中Handler的个数

    void CopyMessageManagerList();                                         //从MessageManager中获得信令列表副本

    shared_ptr<CWorkThreadMessage> CreateMessage() const;
    void DeleteMessage(shared_ptr<CWorkThreadMessage> pMessage) const;

    void GetFlowPortList(const PSS_Time_Point& tvNow, vector<CWorkThread_Packet_Info>& vec_Port_Data_Account); //得到当前列表描述信息

    bool Synchronize_SendPostMessage(shared_ptr<CWorkThread_Handler_info> pHandlerInfo, const PSS_Time_Point& tvMessage);
    bool SendPostMessage(const CSendMessageInfo& objSendMessageInfo);          //发送数据
    bool SendCloseMessage(uint32 u4ConnectID);                                //关闭指定链接 

    _ClientIPInfo GetClientIPInfo(uint32 u4ConnectID);                        //得到客户端接入端口
    _ClientIPInfo GetLocalIPInfo(uint32 u4ConnectID);                         //得到客户端监听端口

    void Check_Handler_Recv_Timeout();                                        //检查终端连接时间超时
    EM_Client_Connect_status GetConnectState(uint32 u4ConnectID);             //得到当前连接状态 

    uint32 GetThreadID();

private:
    shared_ptr<CWorkThread_Handler_info> ProcessRecvMessage(shared_ptr<CWorkThreadMessage> pMessage, uint32 u4ThreadID); //处理接收事件
    bool ProcessSendMessage(shared_ptr<CWorkThreadMessage> pMessage, uint32 u4ThreadID); //处理发送事件
    bool ProcessSendClose(shared_ptr<CWorkThreadMessage> pMessage, uint32 u4ThreadID);   //处理发送事件
    bool ProcessSendIsLog(shared_ptr<CWorkThreadMessage> pMessage, uint32 u4ThreadID);   //处理发送事件
    void CloseCommandList();                                                  //清理当前信令列表副本
    shared_ptr<CClientCommandList> GetClientCommandList(uint16 u2CommandID);
    bool DoMessage(IMessage* pMessage, uint16& u2CommandID, uint16& u2Count, bool& bDeleteFlag);

    void UpdateCommandList(uint32 u4UpdateIndex);                        //更新指令列表
    bool Dispose_Queue(shared_ptr<CWorkThreadMessageList> msgList);      //队列消费

    uint32                         m_u4ThreadID         = 0;                     //当前线程ID
    uint32                         m_u4MaxQueue         = MAX_MSG_THREADQUEUE;   //线程中最大消息对象个数
    uint32                         m_u4HighMask         = 0;        
    uint32                         m_u4LowMask          = 0;
    uint32                         m_u4WorkQueuePutTime = 0;                     //入队超时时间
    uint32                         m_u4MaxRecvWait      = 0;                     //最大等待时间  
    uint16                         m_u2ThreadTimeOut    = 0;
    uint8                          m_u1PacketCounter    = 0;                     //是否开启性能统开关,0是关闭，1是开启
    bool                           m_blRun              = false;                 //线程是否在运行
    bool                           m_blIsCpuAffinity    = false;                 //是否CPU绑定

    MESSAGE_SERVICE_THREAD_STATE   m_emThreadState      = MESSAGE_SERVICE_THREAD_STATE::THREAD_STOP; //当前工作线程状态

    CBuffPacket                    m_objBuffSendPacket;

    _ThreadInfo                    m_ThreadInfo;           //当前线程信息
    CWorkThreadAI                  m_WorkThreadAI;         //线程自我监控的AI逻辑
    CWorkThread_Process            m_objWorkThreadProcess; //统计线程一分钟的数据包处理量

    CDeviceHandlerPool             m_DeviceHandlerPool;    //对象池 

    using hashmapClientCommandList = unordered_map<uint16, shared_ptr<CClientCommandList>>;
    using hashmapHandlerInfoList   = unordered_map<uint32, shared_ptr<CWorkThread_Handler_info>>;
    hashmapClientCommandList                            m_objClientCommandList;  //可执行的信令列表
    hashmapHandlerInfoList                              m_objHandlerList;        //对应的Handler列表

    CPerformanceCounter m_PerformanceCounter;
    std::thread                                         m_ttQueue;               //消息队列线程
    CMessageQueue<shared_ptr<CWorkThreadMessageList>>   m_objThreadQueue;        //业务消息队列
};

//add by freeeyes
//添加线程管理，用户可以创建若干个ACE_Task，每个Task对应一个线程，一个Connectid只对应一个线程。
class CMessageServiceGroup
{
public:
    CMessageServiceGroup();

    int timer_task(brynet::TimerMgr::Ptr timerMgr);
    void start_new_task(brynet::TimerMgr::Ptr timerMgr);

    bool Init(uint32 u4ThreadCount = MAX_MSG_THREADCOUNT, uint32 u4MaxQueue = MAX_MSG_THREADQUEUE, uint32 u4LowMask = MAX_MSG_MASK);
    bool PutMessage(shared_ptr<CWorkThreadMessageList> pMessage);                            //发送到相应的线程去处理
    bool PutUpdateCommandMessage(uint32 u4UpdateIndex) const;                                //发送消息同步所有的工作线程命令副本
    void Close();

    bool Start();
    void GetThreadInfo(vector<_ThreadInfo>& vecWorkThreadList) const;
    uint32 GetUsedMessageCount() const;

    uint32 GetWorkThreadCount() const;                                                        //得到当前工作线程的数量
    uint32 GetWorkThreadIDByIndex(uint32 u4Index);                                            //得到指定工作线程的线程ID
    void GetWorkThreadAIInfo(vecWorkThreadAIInfo& objvecWorkThreadAIInfo) const;              //得到线程工作AI配置信息
    void GetAITO(vecCommandTimeout& objTimeout) const;                                        //得到所有的AI超时数据包信息
    void GetAITF(vecCommandTimeout& objTimeout) const;                                        //得到所有的AI封禁数据包信息
    void SetAI(uint8 u1AI, uint32 u4DisposeTime, uint32 u4WTCheckTime, uint32 u4WTStopTime) const;  //设置AI

    shared_ptr<CWorkThreadMessage> CreateMessage(uint32 u4ConnectID, EM_CONNECT_IO_TYPE u1PacketType);   //从子线程中获取一个Message对象
    void DeleteMessage(shared_ptr<CWorkThreadMessage> pMessage);                                         //从子线程中回收一个Message对象

    void CopyMessageManagerList() const;                                                      //从MessageManager中获得信令列表副本

    void AddDyringIP(const char* pClientIP, uint16 u2MaxCount);                               //染色指定的IP
    bool AddDyeingCommand(uint16 u2CommandID, uint16 u2MaxCount);                             //染色指定的CommandID
    void GetDyeingCommand(vec_Dyeing_Command_list& objList) const;                            //获得当前命令染色状态

    void GetFlowPortList(vector<CWorkThread_Packet_Info>& vec_Port_Data_Account) const;       //得到当前列表描述信息
    bool CheckCPUAndMemory(bool blTest = false) const;                                        //检查CPU和内存

    bool Send_Post_Message(const CSendMessageInfo& objSendMessageInfo);                       //发送数据信息
    bool Send_Close_Message(uint32 u4ConnectID);                                              //关闭客户端连接
    _ClientIPInfo GetClientIPInfo(uint32 u4ConnectID);                                        //得到指定链接的客户单ID 
    _ClientIPInfo GetLocalIPInfo(uint32 u4ConnectID);                                         //得到监听的IP信息
    uint32 GetHandlerCount() const;                                                           //得到当前连接总数
    EM_Client_Connect_status GetConnectState(uint32 u4ConnectID);                             //得到当前连接状态 

private:
    uint32 GetWorkThreadID(uint32 u4ConnectID, EM_CONNECT_IO_TYPE emPacketType) const;        //根据操作类型和ConnectID计算出那个工作线程ID

    bool StartTimer();
    bool KillTimer();

    bool CheckRecvTimeout() const;                                                            //检查所有的超时链接
    bool CheckWorkThread(const PSS_Time_Point& tvNow) const;                                  //检查所有的工作线程状态
    bool CheckPlugInState() const;                                                            //检查所有插件状态
   
	using vecMessageService = vector<shared_ptr<CMessageService>>;
	vecMessageService                                   m_vecMessageService;
    uint32                                              m_u4MaxQueue           = 0;              //线程中最大消息对象个数
    uint32                                              m_u4HighMask           = 0;              //线程高水位
    uint32                                              m_u4LowMask            = 0;              //线程低水位
    uint16                                              m_u2ThreadTimeCheck    = 0;              //线程自检时间
    uint16                                              m_u2CurrThreadID       = 0;              //当前轮询到的线程ID
    uint16                                              m_u2CpuNumber          = 0;              //当前CPU的核数
    bool                                                m_blTimerState         = true;           //定时器是否运行
    CMessageDyeingManager                               m_objMessageDyeingManager; //数据染色类
    ACE_Recursive_Thread_Mutex                          m_ThreadLock;              //用于线程操作的线程锁，保证CurrThreadID的数据正常
};

using App_MessageServiceGroup = PSS_singleton<CMessageServiceGroup>;
#endif
