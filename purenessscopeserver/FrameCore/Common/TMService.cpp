#include "TMService.h"

void timer_run_execute(shared_ptr<ITimerInfo> timer_info)
{
    if (nullptr != timer_info)
    {
        timer_info->run();
    }
}

int CTMService::Init(int nNeedLoad)
{
    //读取配置文件
    CXmlOpeation objXmlOperation;

    bool blFileExist = objXmlOperation.Init(XML_CONF_FILE);

    if (false == blFileExist)
    {
        //file is not exist.
        return -3;
    }

    objXmlOperation.Read_XML_Data_Single_Int("Info", "Active", m_nActive);

    if (0 == nNeedLoad && 0 == m_nActive)
    {
        return -4;
    }

    Close();

    objXmlOperation.Read_XML_Data_Single_Int("Info", "TimerCount", m_nTimerMaxCount);

    TiXmlElement* pID       = nullptr;
    TiXmlElement* pName     = nullptr;
    TiXmlElement* pInterval = nullptr;
    TiXmlElement* pMaxEvent = nullptr;

    ACE_UNUSED_ARG(m_nTimerMaxCount);

    string szName;
    int  nID           = 0;
    int  nInterval     = 0;
    int  nMaxQueueList = 0;

    while (objXmlOperation.Read_XML_Data_Multiple_Int("Timer", "EventID", nID, pID)
           && objXmlOperation.Read_XML_Data_Multiple_String("Timer", "Name", szName, pName)
           && objXmlOperation.Read_XML_Data_Multiple_Int("Timer", "Interval", nInterval, pInterval)
           && objXmlOperation.Read_XML_Data_Multiple_Int("Timer", "EventMaxCount", nMaxQueueList, pMaxEvent))
    {
        //写入配置文件
        auto pTimerInfo = std::make_shared<CTimerInfo>();

        pTimerInfo->m_nID                  = nID;
        pTimerInfo->m_szName               = szName;
        pTimerInfo->m_nInterval            = nInterval;
        pTimerInfo->m_nMaxQueueList        = nMaxQueueList;
        pTimerInfo->m_pMessageQueueManager = &m_ThreadQueueManager;

        m_HashTimerList[pTimerInfo->m_szName] = pTimerInfo;

        //插入Timer定时器
        milliseconds timer_interval = milliseconds(pTimerInfo->m_nInterval);
        timer_events_.Add_Timer(pTimerInfo->m_nID, timer_interval, timer_run_execute, pTimerInfo);

    }

    if (m_HashTimerList.size() > (size_t)m_nTimerMaxCount)
    {
        return -1;
    }

    this_thread::sleep_for(std::chrono::milliseconds(100));

    //获得当前工作线程配置列表
    m_T2MList.clear();
    m_M2TList.clear();

    TiXmlElement* pWorkThreadID = nullptr;
    TiXmlElement* pMessageID    = nullptr;

    int nWorkThreadID = 0;
    int nMessageID    = 0;

    while (objXmlOperation.Read_XML_Data_Multiple_Int("MessageInfo", "LogicThreadID", nWorkThreadID, pWorkThreadID)
           && objXmlOperation.Read_XML_Data_Multiple_Int("MessageInfo", "MessageID", nMessageID, pMessageID))
    {
        unordered_map<int, int>::iterator ftm = m_M2TList.find(nMessageID);

        if (m_M2TList.end() == ftm)
        {
            m_M2TList.insert(std::make_pair(nMessageID, nWorkThreadID));
        }

        unordered_map<int, int>::iterator fmt = m_T2MList.find(nWorkThreadID);

        if (m_T2MList.end() == fmt)
        {
            m_T2MList.insert(std::make_pair(nWorkThreadID, nMessageID));
        }
    }

    //根据配置文件创建对应函数
    for (const auto& f : m_T2MList)
    {
        m_ThreadQueueManager.Create(f.first);
    }

    return 0;
}

void CTMService::Close()
{
    //清空
    m_HashTimerList.clear();

    m_ThreadQueueManager.Close();

    timer_events_.Close();

    this_thread::sleep_for(std::chrono::milliseconds(100));
}

int CTMService::AddMessage(const char* pName, unsigned long long nMessagePos, long sec, long usec, int _Message_id, void* _arg, Enum_Timer_Mode emTimerMode, IMessagePrecess* pMessagePrecess)
{
    string strName = (string)pName;
    if (0 == m_nActive)
    {
        return -2;
    }

    auto f =m_HashTimerList.find(strName.c_str());

    if (m_HashTimerList.end() == f)
    {
        return -1;
    }

    auto pTimerInfo = f->second;
    if ((int)pTimerInfo->m_vecEventsList.size() >= pTimerInfo->m_nMaxQueueList)
    {
        return -1;
    }

    //根据MessageID查找对应消息处理线程ID
    int _Message_thread_id = 0;
    unordered_map<int, int>::iterator ftm = m_M2TList.find(_Message_id);

    if (m_M2TList.end() != ftm)
    {
        _Message_thread_id = ftm->second;
    }
    else
    {
        return -1;
    }

    if (sec <= 0 && usec <= 0)
    {
        //需要立即执行
        m_ThreadQueueManager.AddMessageClass(_Message_thread_id,
                                             pMessagePrecess,
                                             &IMessagePrecess::DoMessage,
                                             _Message_id,
                                             _arg);
    }
    else
    {
        CEventsInfo objEventsInfo;

        system_clock::time_point ttNextTime = system_clock::now() + seconds(sec) + milliseconds(usec);

        objEventsInfo.m_ttNextTime       = ttNextTime;
        objEventsInfo.m_pArg             = _arg;
        objEventsInfo.m_nMessageID       = _Message_id;
        objEventsInfo.m_nWorkThreadID    = _Message_thread_id;
        objEventsInfo.m_pIMessagePrecess = pMessagePrecess;
        objEventsInfo.m_nMessagePos      = nMessagePos;
        objEventsInfo.m_nSec             = (int)sec;
        objEventsInfo.m_nUsec            = (int)usec;
        objEventsInfo.m_emTimerMode      = emTimerMode;

        pTimerInfo->AddEventsInfo(objEventsInfo);
    }

    return 0;
}

void* CTMService::DeleteMessage(const char* pName, unsigned long long nMessagePos)
{
    string strName = (string)pName;
    if (0 == m_nActive)
    {
        return nullptr;
    }

    auto f = m_HashTimerList.find(strName.c_str());

    if (m_HashTimerList.end() == f)
    {
        return nullptr;
    }

    auto pTimerInfo = f->second;

    return pTimerInfo->DeleteEventInfo(nMessagePos);
}

bool CTMService::GetRun()
{
    if (0 == m_nActive)
    {
        return false;
    }
    else
    {
        return true;
    }
}
