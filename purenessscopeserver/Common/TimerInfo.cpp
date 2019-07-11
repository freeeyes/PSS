#include "TimerInfo.h"

ts_timer::ITimerInfo::ITimerInfo() : m_pTimeNode(NULL), m_pArgContext(NULL)
{
    //初始化时间
    m_ttBeginTime.Set_time(0, 0);
    m_ttLastRunTime.Set_time(0, 0);;
}

ts_timer::ITimerInfo::~ITimerInfo()
{

}

void ts_timer::ITimerInfo::Set_Timer_Param(ITimeNode* pTimeNode, void* pArgContext)
{
    m_pTimeNode           = pTimeNode;
    m_pArgContext         = pArgContext;
    m_ttBeginTime         = m_pTimeNode->GetBeginTime();
}

int ts_timer::ITimerInfo::Get_Timer_ID()
{
    if(NULL != m_pTimeNode)
    {
        return m_pTimeNode->GetTimerID();
    }
    else
    {
        return -1;
    }
}

int ts_timer::ITimerInfo::Get_Timer_Frequency()
{
    if (NULL != m_pTimeNode)
    {
        return m_pTimeNode->GetFrequency();
    }
    else
    {
        return -1;
    }
}

int ts_timer::ITimerInfo::Get_Next_Timer(CTime_Value ttNow, bool blState)
{
    CTime_Value ttInterval;

    if (NULL == m_pTimeNode)
    {
        return -1;
    }

    int nSeconds = m_pTimeNode->GetFrequency() / 1000;
    int nUseconds = (m_pTimeNode->GetFrequency() % 1000) * 1000;

    if (m_ttLastRunTime.IsZero() == true && m_ttNextTime.IsZero() == true)
    {
        //如果是第一次计算,看看有没有初始化时间参数
        m_ttNextTime = m_ttBeginTime + CTime_Value(nSeconds, nUseconds);
    }

    if (false == blState)
    {
        //如果下一次运行时间小于当前时间
        if (m_ttNextTime.Get_milliseconds() > ttNow.Get_milliseconds())
        {
            ttInterval = m_ttNextTime - ttNow;
            int nIntervalFrquency = ttInterval.Get_milliseconds();
            return nIntervalFrquency;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        //计算当前下个要指定的时间
        while (true)
        {
            if (m_ttNextTime.Get_milliseconds() < ttNow.Get_milliseconds())
            {
                m_ttNextTime = m_ttNextTime + CTime_Value(nSeconds, nUseconds);
            }
            else
            {
                ttInterval = m_ttNextTime - ttNow;
                int nIntervalFrquency = ttInterval.Get_milliseconds();
                return nIntervalFrquency;
            }
        }
    }
}

ts_timer::CTime_Value ts_timer::ITimerInfo::Get_Next_Time()
{
    return m_ttNextTime;
}

ts_timer::EM_Timer_State ts_timer::ITimerInfo::Do_Timer_Event(ts_timer::CTime_Value& obj_Now)
{
    //设置最后执行时间
    m_ttLastRunTime = obj_Now;

    //执行回调函数
    EM_Timer_State emState = TIMER_STATE_OK;

    if (NULL != m_pTimeNode)
    {
        m_pTimeNode->Run(obj_Now, m_pArgContext, emState);

        //设置下次运行时间
        int nSeconds = m_pTimeNode->GetFrequency() / 1000;
        int nUseconds = (m_pTimeNode->GetFrequency() % 1000) * 1000;

        m_ttNextTime = m_ttNextTime + CTime_Value(nSeconds, nUseconds);
    }

    return emState;
}

void ts_timer::ITimerInfo::Do_Error_Events(int nLastRunTimerID, int nTimeoutTime, std::vector<CTime_Value>& vecTimoutList)
{
    if (NULL != m_pTimeNode)
    {
        m_pTimeNode->Error(nLastRunTimerID, nTimeoutTime, vecTimoutList, m_pArgContext);
    }
}

//定时器列表类
ts_timer::TIMER_THREAD_MUTEX* ts_timer::CTimerInfoList::Get_mutex()
{
    return m_pMutex;
}


ts_timer::CTimerInfoList::CTimerInfoList() :
    m_nCurrTimerIndex(0),
    m_nMaxCount(0),
    m_NextRunTimer(NULL),
    m_blRun(false),
    m_emEventType(TIMER_STOP),
    m_nThreadID(0),
    m_pMutex(NULL),
    m_pCond(NULL)
{
}

ts_timer::CTimerInfoList::~CTimerInfoList()
{
    Close();
}

void ts_timer::CTimerInfoList::Set_Event_Type(EM_Event_Type emEventType)
{
    m_emEventType = emEventType;

    if (emEventType == TIMER_STOP)
    {
        m_blRun = false;
    }
}

ts_timer::EM_Event_Type ts_timer::CTimerInfoList::Get_Event_Type()
{
    return m_emEventType;
}


ts_timer::TIMER_THREAD_COND* ts_timer::CTimerInfoList::Get_cond()
{
    return m_pCond;
}

void ts_timer::CTimerInfoList::Set_Thread_ID(TIMER_THREAD_ID nThreadID)
{
    m_nThreadID = nThreadID;
}

ts_timer::TIMER_THREAD_ID ts_timer::CTimerInfoList::Get_Thread_ID()
{
    return m_nThreadID;
}

void ts_timer::CTimerInfoList::Lock()
{
#if PSS_PLATFORM == PLATFORM_WIN
    EnterCriticalSection(m_pMutex);
#else
    pthread_mutex_lock(m_pMutex);
#endif
}

void ts_timer::CTimerInfoList::UnLock()
{
#if PSS_PLATFORM == PLATFORM_WIN
    LeaveCriticalSection(m_pMutex);
#else
    pthread_mutex_unlock(m_pMutex);
#endif
}

void ts_timer::CTimerInfoList::Init(int nMaxCount /*= MAX_TIMER_LIST_COUNT*/)
{
#if PSS_PLATFORM == PLATFORM_WIN
    m_pMutex = new CRITICAL_SECTION();
    m_pCond = new CONDITION_VARIABLE();
#else
    m_pMutex = new pthread_mutex_t();
    m_pCond = new pthread_cond_t();
#endif

#if PSS_PLATFORM == PLATFORM_WIN
    InitializeConditionVariable(m_pCond);
    InitializeCriticalSection(m_pMutex);
#else
    pthread_cond_init(m_pCond, NULL);
    pthread_mutex_init(m_pMutex, NULL);
#endif

    m_nMaxCount = nMaxCount;
}

void ts_timer::CTimerInfoList::Close()
{
    if (NULL != m_pMutex && NULL != m_pCond)
    {
#if PSS_PLATFORM == PLATFORM_WIN
        DeleteCriticalSection(m_pMutex);
#else
        pthread_mutex_destroy(m_pMutex);
        pthread_cond_destroy(m_pCond);
#endif
        delete m_pMutex;
        delete m_pCond;
        m_pMutex = NULL;
        m_pCond  = NULL;
    }
}

void ts_timer::CTimerInfoList::Set_Run(bool blRun)
{
    m_blRun = blRun;
}

bool ts_timer::CTimerInfoList::Get_Run()
{
    return m_blRun;
}

bool ts_timer::CTimerInfoList::Add_Timer(ITimerInfo* pTimerInfo)
{
    //首先寻找是否已经是内部的定时器了，如果是则直接返回true
    for (std::vector<ITimerInfo*>::iterator it = m_TimerList.begin(); it != m_TimerList.end(); ++it)
    {
        if ((*it)->Get_Timer_ID() == pTimerInfo->Get_Timer_ID())
        {
            return false;
        }
    }

    if ((int)m_TimerList.size() >= m_nMaxCount)
    {
        return false;
    }

    m_TimerList.push_back(pTimerInfo);
    return true;
}

bool ts_timer::CTimerInfoList::Del_Timer(int nTimerID)
{
    for (std::vector<ITimerInfo*>::iterator it = m_TimerList.begin(); it != m_TimerList.end(); ++it)
    {
        if ((*it)->Get_Timer_ID() == nTimerID)
        {
            ITimerInfo* pTimerInfo = (ITimerInfo*)(*it);
            it = m_TimerList.erase(it);
            delete pTimerInfo;
            pTimerInfo = NULL;
            return true;
        }
    }

    return false;
}


std::vector<_Lcm_Info>* ts_timer::CTimerInfoList::Get_Curr_Timer()
{
    if (m_TimerAssemble.size() == 0)
    {
        return NULL;
    }

    if (m_nCurrTimerIndex >= (int)m_TimerAssemble.size() - 1)
    {
        m_nCurrTimerIndex = 0;
    }

    return (std::vector<_Lcm_Info>*)&m_TimerAssemble[m_nCurrTimerIndex];
}

void ts_timer::CTimerInfoList::Calculation_Run_Assemble(CTime_Value obj_Now)
{
    std::vector<_Lcm_Info> vec_Lcm_Info;
    int nIndex       = 0;

    for (int i = 0; i < (int)m_TimerList.size(); i++)
    {
        _Lcm_Info obj_Lcm_Info;
        obj_Lcm_Info.m_nID      = m_TimerList[i]->Get_Timer_ID();
        obj_Lcm_Info.m_nTimeout = m_TimerList[i]->Get_Timer_Frequency();
        vec_Lcm_Info.push_back(obj_Lcm_Info);

        m_TimerList[i]->Get_Next_Timer(obj_Now, true);
    }

    //得到最小公倍数
    int nData = Get_LeastCommonMultiple(vec_Lcm_Info);

    //得到定时器的最小运行时
    m_TimerAssemble.clear();
    Get_Minimum_Set(vec_Lcm_Info, nData, m_TimerAssemble);

    //计算出应该从哪个ID开始
    if (0 == m_TimerAssemble.size())
    {
        return;
    }

    CTime_Value ttBeginTime = GetTimerInfo(m_TimerAssemble[0][0].m_nIndex)->Get_Next_Time();

    for (int i = 0; i < (int)m_TimerAssemble.size(); i++)
    {
        CTime_Value ttInterval = ttBeginTime - obj_Now;

        if (ttInterval.Get_milliseconds() > 0)
        {
            nIndex = i;
        }

        int nSeconds = m_TimerAssemble[i][0].m_nTimeout / 1000;
        int nUseconds = (m_TimerAssemble[i][0].m_nTimeout % 1000) * 1000;

        ttBeginTime = ttBeginTime + CTime_Value(nSeconds, nUseconds);
    }

    //计算起始位置
    m_nCurrTimerIndex = nIndex;
}

std::vector<_Lcm_Info>* ts_timer::CTimerInfoList::Get_Curr_Assemble()
{
    return &m_TimerAssemble[m_nCurrTimerIndex];
}

std::vector<_Lcm_Info>* ts_timer::CTimerInfoList::Get_Next_Assemble()
{
    if (GetCurrTimerCount() == 0 || GetAssembleCount() == 0)
    {
        return NULL;
    }

    if (m_nCurrTimerIndex == GetAssembleCount() - 1)
    {
        m_nCurrTimerIndex = 0;
    }
    else
    {
        m_nCurrTimerIndex++;
    }

    std::vector<_Lcm_Info>* pLcm = &m_TimerAssemble[m_nCurrTimerIndex];
    return pLcm;
}

int ts_timer::CTimerInfoList::GetCurrTimerCount()
{
    return (int)m_TimerList.size();
}

int ts_timer::CTimerInfoList::GetAssembleCount()
{
    return (int)m_TimerAssemble.size();
}

ts_timer::ITimerInfo* ts_timer::CTimerInfoList::GetTimerInfo(int nIndex)
{
    if (nIndex < GetCurrTimerCount() && nIndex >= 0)
    {
        return m_TimerList[nIndex];
    }
    else
    {
        return NULL;
    }
}
