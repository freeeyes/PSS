#ifndef _TIMER_INFO_H
#define _TIMER_INFO_H

//定义Timer的执行单元
//add by freeeyes

#ifdef WIN32
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
#include "time.h"
#include "TimerCommon.h"
#include "Lcm.h"

//定时器相关对象声明
//add by freeeyes

namespace ts_timer
{
    enum EM_Timer_State
    {
        TIMER_STATE_OK = 0,    //定时器正常运行
        TIMER_STATE_DEL,       //定时器删除
    };

#define MAX_TIMER_LIST_COUNT 10

    typedef void(*Timeout_Callback)(int, CTime_Value&, void*, EM_Timer_State&);
    typedef void(*Timeout_Error_Callback)(int, int, int, std::vector<CTime_Value>, void*);

    enum EM_Event_Type
    {
        TIMER_STOP = 0,      //停止线程
        TIMER_MODIFY,        //有Timer变化了
        TIMER_DO_EVENT,      //执行Timer
        TIMER_PAUSE,         //暂停定时器
        TIMER_RESTORE,       //恢复暂停定时器
    };

    class ITimerInfo
    {
    public:
        ITimerInfo();
        virtual ~ITimerInfo();

        void Set_Timer_Param(int nTimerID, int nFrequency, CTime_Value ttBegin, Timeout_Callback fn_Timeout_Callback, void* pArgContext, Timeout_Error_Callback fn_Timeout_Error_Callback);

        int Get_Timer_ID();

        int Get_Timer_Frequency();

        int Get_Next_Timer(CTime_Value ttNow, bool blState = false);

        CTime_Value Get_Next_Time();

        EM_Timer_State Do_Timer_Event(CTime_Value& obj_Now);

        void Do_Error_Events(int nLastRunTimerID, int nTimeoutTime, std::vector<CTime_Value>& vecTimoutList);

    private:
        int m_nTimerID;                                  //当前唯一的Timer标识
        int m_nFrequency;                                //当前的操作频度(单位是毫秒)
        CTime_Value m_ttBeginTime;                       //开始定时器的时间
        CTime_Value m_ttLastRunTime;                     //上一次成功运行定时器的时间
        CTime_Value m_ttNextTime;                        //下一次运行的时间

        Timeout_Callback       m_fn_Timeout_Callback;    //回调函数
        Timeout_Error_Callback m_fn_Timeout_Error;       //回调定时器执行超时函数
        void*                  m_pArgContext;            //回调函数上下文
    };

    //定时事件列表
    class CTimerInfoList
    {
    public:
        CTimerInfoList();
        ~CTimerInfoList();

#ifdef WIN32
        CRITICAL_SECTION* Get_mutex();
#else
        pthread_mutex_t* Get_mutex();
#endif

        void Set_Event_Type(EM_Event_Type emEventType);

        EM_Event_Type Get_Event_Type();

#ifdef WIN32
        CONDITION_VARIABLE* Get_cond();
#else
        pthread_cond_t* Get_cond();
#endif

#ifdef WIN32
        void Set_Thread_ID(DWORD nThreadID);
#else
        void Set_Thread_ID(pthread_t nThreadID);
#endif

#ifdef WIN32
        DWORD Get_Thread_ID();
#else
        pthread_t Get_Thread_ID();
#endif

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

#ifdef WIN32
        DWORD                      m_nThreadID;
        CRITICAL_SECTION*          m_pMutex;
        CONDITION_VARIABLE*        m_pCond;
#else
        pthread_t                  m_nThreadID;
        pthread_mutex_t*           m_pMutex;
        pthread_cond_t*            m_pCond;
#endif
        std::vector<ITimerInfo*>             m_TimerList;      //当前定时器对象列表
        std::vector<std::vector<_Lcm_Info> > m_TimerAssemble;  //执行定时器的计划镜像
    };
}

#endif
