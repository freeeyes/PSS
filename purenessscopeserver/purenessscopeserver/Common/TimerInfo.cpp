#include "TimerInfo.h"

ts_timer::ITimerInfo::ITimerInfo() : m_nTimerID(-1), m_nFrequency(-1)
{
    //初始化时间
    m_ttBeginTime.Set_time(0, 0);
    m_ttLastRunTime.Set_time(0, 0);
    m_fn_Timeout_Error    = NULL;
    m_fn_Timeout_Callback = NULL;
}

ts_timer::ITimerInfo::~ITimerInfo()
{

}

void ts_timer::ITimerInfo::Set_Timer_Param(int nTimerID, int nFrequency, CTime_Value ttBegin, Timeout_Callback fn_Timeout_Callback, void* pArgContext, Timeout_Error_Callback fn_Timeout_Error_Callback)
{
    m_nTimerID            = nTimerID;
    m_nFrequency          = nFrequency;
    m_ttBeginTime         = ttBegin;
    m_fn_Timeout_Callback = fn_Timeout_Callback;
    m_pArgContext         = pArgContext;
    m_fn_Timeout_Error    = fn_Timeout_Error_Callback;
}

int ts_timer::ITimerInfo::Get_Timer_ID()
{
    return m_nTimerID;
}

int ts_timer::ITimerInfo::Get_Timer_Frequency()
{
    return m_nFrequency;
}

int ts_timer::ITimerInfo::Get_Next_Timer(CTime_Value ttNow, bool blState)
{
    CTime_Value ttInterval;

    int nSeconds = m_nFrequency / 1000;
    int nUseconds = (m_nFrequency % 1000) * 1000;

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
    m_fn_Timeout_Callback(Get_Timer_ID(), obj_Now, m_pArgContext, emState);


    //设置下次运行时间
    int nSeconds = m_nFrequency / 1000;
    int nUseconds = (m_nFrequency % 1000) * 1000;

    m_ttNextTime = m_ttNextTime + CTime_Value(nSeconds, nUseconds);

    return emState;
}

void ts_timer::ITimerInfo::Do_Error_Events(int nLastRunTimerID, int nTimeoutTime, std::vector<CTime_Value>& vecTimoutList)
{
    if (NULL != m_fn_Timeout_Error)
    {
        m_fn_Timeout_Error(nLastRunTimerID, nTimeoutTime, Get_Timer_ID(), vecTimoutList, m_pArgContext);
    }
}

//定时器列表类
#ifdef WIN32
CRITICAL_SECTION* ts_timer::CTimerInfoList::Get_mutex()
#else
pthread_mutex_t* ts_timer::CTimerInfoList::Get_mutex()
#endif
{
    return m_pMutex;
}


ts_timer::CTimerInfoList::CTimerInfoList() : m_nCurrTimerIndex(0), m_nMaxCount(0), m_NextRunTimer(NULL), m_blRun(false), m_emEventType(TIMER_DO_EVENT), m_pCond(NULL), m_pMutex(NULL)
{
}

ts_timer::CTimerInfoList::~CTimerInfoList()
{

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

#ifdef WIN32
CONDITION_VARIABLE* ts_timer::CTimerInfoList::Get_cond()
#else
pthread_cond_t* ts_timer::CTimerInfoList::Get_cond()
#endif
{
    return m_pCond;
}

#ifdef WIN32
void ts_timer::CTimerInfoList::Set_Thread_ID(DWORD nThreadID)
#else
void ts_timer::CTimerInfoList::Set_Thread_ID(pthread_t nThreadID)
#endif
{
    m_nThreadID = nThreadID;
}

#ifdef WIN32
DWORD ts_timer::CTimerInfoList::Get_Thread_ID()
#else
pthread_t ts_timer::CTimerInfoList::Get_Thread_ID()
#endif
{
    return m_nThreadID;
}

void ts_timer::CTimerInfoList::Lock()
{
#ifdef WIN32
    EnterCriticalSection(m_pMutex);
#else
    pthread_mutex_lock(m_pMutex);
#endif
}

void ts_timer::CTimerInfoList::UnLock()
{
#ifdef WIN32
    LeaveCriticalSection(m_pMutex);
#else
    pthread_mutex_unlock(m_pMutex);
#endif
}

void ts_timer::CTimerInfoList::Init(int nMaxCount /*= MAX_TIMER_LIST_COUNT*/)
{
#ifdef WIN32
    m_pMutex = new CRITICAL_SECTION();
    m_pCond = new CONDITION_VARIABLE();
#else
    m_pMutex = new pthread_mutex_t();
    m_pCond = new pthread_cond_t();
#endif

#ifdef WIN32
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
#ifdef WIN32
        DeleteCriticalSection(m_pMutex);
        delete m_pMutex;
        delete m_pCond;
        m_pMutex = NULL;
        m_pCond = NULL;
#else
        pthread_mutex_destroy(m_pMutex);
        pthread_cond_destroy(m_pCond);

        delete m_pMutex;
        delete m_pCond;
        m_pMutex = NULL;
        m_pCond = NULL;
#endif
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

    if (m_TimerList.size() >= m_nMaxCount)
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

    int nCurrTimerIndex = m_nCurrTimerIndex;

    if (m_nCurrTimerIndex >= m_TimerAssemble.size() - 1)
    {
        m_nCurrTimerIndex = 0;
    }

    return (std::vector<_Lcm_Info>*)&m_TimerAssemble[m_nCurrTimerIndex];
}

void ts_timer::CTimerInfoList::Calculation_Run_Assemble(CTime_Value obj_Now)
{
    std::vector<_Lcm_Info> vec_Lcm_Info;
    int nMinInterval = -1;
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
    int milliseconds = 0;

    if (0 == m_TimerAssemble.size())
    {
        return;
    }

    CTime_Value ttBeginTime = GetTimerInfo(m_TimerAssemble[0][0].m_nIndex)->Get_Next_Time();

    for (int i = 0; i < m_TimerAssemble.size(); i++)
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
