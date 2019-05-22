#ifndef _TIMER_INFO_H
#define _TIMER_INFO_H

//定义Timer的执行单元
//add by freeeyes

#include "time.h"
#include "TimerCommon.h"
#include "Lcm.h"
#if PSS_PLATFORM == PLATFORM_WIN
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#else
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#endif

//定时器相关对象声明
//add by freeeyes

namespace ts_timer
{
#define MAX_TIMER_LIST_COUNT 10

    enum EM_Timer_State
    {
        TIMER_STATE_OK = 0,    //定时器正常运行
        TIMER_STATE_DEL,       //定时器删除
    };

    enum EM_Event_Type
    {
        TIMER_STOP = 0,      //停止线程
        TIMER_MODIFY,        //有Timer变化了
        TIMER_DO_EVENT,      //执行Timer
        TIMER_PAUSE,         //暂停定时器
        TIMER_RESTORE,       //恢复暂停定时器
    };

#if PSS_PLATFORM == PLATFORM_WIN
    typedef CRITICAL_SECTION TIMER_THREAD_MUTEX;
    typedef CONDITION_VARIABLE TIMER_THREAD_COND;
    typedef DWORD TIMER_THREAD_ID;
#else
    typedef pthread_mutex_t TIMER_THREAD_MUTEX;
    typedef pthread_cond_t TIMER_THREAD_COND;
    typedef pthread_t TIMER_THREAD_ID;
#endif

    //定时器节点基础类，用于集成实现
    class ITimeNode
    {
    public:
        ITimeNode() : m_nTimerID(0), m_nFrequency(0), m_ttBegin(GetTimeofDay()) {};
        virtual ~ITimeNode() {};

        void SetTimerID(int nTimerID)
        {
            m_nTimerID = nTimerID;
        }

        int GetTimerID()
        {
            return m_nTimerID;
        }

        void SetFrequency(int nFrequency)
        {
            m_nFrequency = nFrequency;
        }

        int GetFrequency()
        {
            return m_nFrequency;
        }

        void SetBeginTime(CTime_Value ttBegin)
        {
            m_ttBegin = ttBegin;
        }

        CTime_Value GetBeginTime()
        {
            return m_ttBegin;
        }

        //定时器执行函数
        virtual void Run(CTime_Value& tvNow, void* pArg, EM_Timer_State& emState) = 0;

        //定时器执行超时函数
        //nLastRunTimerID 执行时间被占用的定时器ID
        //nTimeoutTime 被占据的定时器时间长度
        //vecTimoutList 被占用的定时器时段
        virtual void Error(int nLastRunTimerID, int nTimeoutTime, std::vector<CTime_Value>& vecTimoutList, void* pArg) = 0;

    private:
        int         m_nTimerID;     //定时器ID
        int         m_nFrequency;   //当前定时器时间间隔(ms)
        CTime_Value m_ttBegin;      //当前定时器需要的开始时间
    };

    class ITimerInfo
    {
    public:
        ITimerInfo();
        virtual ~ITimerInfo();

        void Set_Timer_Param(ITimeNode* pTimeNode, void* pArgContext);

        int Get_Timer_ID();

        int Get_Timer_Frequency();

        int Get_Next_Timer(CTime_Value ttNow, bool blState = false);

        CTime_Value Get_Next_Time();

        EM_Timer_State Do_Timer_Event(CTime_Value& obj_Now);

        void Do_Error_Events(int nLastRunTimerID, int nTimeoutTime, std::vector<CTime_Value>& vecTimoutList);

    private:
        CTime_Value m_ttBeginTime;                       //开始定时器的时间
        CTime_Value m_ttLastRunTime;                     //上一次成功运行定时器的时间
        CTime_Value m_ttNextTime;                        //下一次运行的时间

        ITimeNode*  m_pTimeNode;                         //定时器ID
        void*       m_pArgContext;                       //回调函数上下文
    };

    //定时事件列表
    class CTimerInfoList
    {
    public:
        CTimerInfoList();
        ~CTimerInfoList();

        TIMER_THREAD_MUTEX* Get_mutex();

        void Set_Event_Type(EM_Event_Type emEventType);

        EM_Event_Type Get_Event_Type();

        TIMER_THREAD_COND* Get_cond();

        void Set_Thread_ID(TIMER_THREAD_ID nThreadID);

        TIMER_THREAD_ID Get_Thread_ID();

        void Lock();

        void UnLock();

        void Init(int nMaxCount = MAX_TIMER_LIST_COUNT);

        void Close();

        void Set_Run(bool blRun);

        bool Get_Run();

        bool Add_Timer(ITimerInfo* pTimerInfo);

        bool Del_Timer(int nTimerID);

        std::vector<_Lcm_Info>* Get_Curr_Timer();

        void Calculation_Run_Assemble(CTime_Value obj_Now);        //计算定时器执行镜像集合

        std::vector<_Lcm_Info>* Get_Curr_Assemble();                    //获得当前要执行的列表

        std::vector<_Lcm_Info>* Get_Next_Assemble();                    //获得下一个要执行的列表

        int GetCurrTimerCount();                                   //得到当前定时器的Count

        int GetAssembleCount();                                    //得到当前镜像列表的数量

        ITimerInfo* GetTimerInfo(int nIndex);                      //得到指定的Timer指针

    private:
        int                                  m_nCurrTimerIndex;//记录当前TimerID
        int                                  m_nMaxCount;      //当前定时器对象最大容量
        ITimerInfo*                          m_NextRunTimer;   //下一次要运行的定时器对象
        bool                                 m_blRun;          //是否运行
        EM_Event_Type                        m_emEventType;    //当前事件执行状态
        TIMER_THREAD_ID                      m_nThreadID;
        TIMER_THREAD_MUTEX*                  m_pMutex;
        TIMER_THREAD_COND*                   m_pCond;

        std::vector<ITimerInfo*>             m_TimerList;      //当前定时器对象列表
        std::vector<std::vector<_Lcm_Info> > m_TimerAssemble;  //执行定时器的计划镜像
    };
}

#endif
