#include "TMService.h"

void timer_run_execute(void* arg)
{
    CTimerInfo* timer_info = (CTimerInfo*)arg;

    timer_info->run();
}

CTMService::CTMService() :m_nTimerMaxCount(0), m_nActive(0)
{
}

CTMService::~CTMService()
{

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

    TiXmlElement* pID       = NULL;
    TiXmlElement* pName     = NULL;
    TiXmlElement* pInterval = NULL;
    TiXmlElement* pMaxEvent = NULL;

    m_HashTimerList.Init(m_nTimerMaxCount, MAX_BUFF_100);

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
        CTimerInfo* pTimerInfo = new CTimerInfo();

        pTimerInfo->m_nID                  = nID;
        pTimerInfo->m_szName               = szName;
        pTimerInfo->m_nInterval            = nInterval;
        pTimerInfo->m_nMaxQueueList        = nMaxQueueList;
        pTimerInfo->m_pMessageQueueManager = &m_ThreadQueueManager;

        if (0 > m_HashTimerList.Add_Hash_Data(pTimerInfo->m_szName.c_str(), pTimerInfo))
        {
            delete pTimerInfo;
            return -2;
        }
    }

    if (m_HashTimerList.Get_Used_Count() > m_nTimerMaxCount)
    {
        return -1;
    }

    this_thread::sleep_for(std::chrono::milliseconds(100));

    vector<CTimerInfo* > vecInfoList;
    m_HashTimerList.Get_All_Used(vecInfoList);


    //添加定时器
    int nTimerSize = (int)vecInfoList.size();

    for (int i = 0; i < nTimerSize; i++)
    {
        milliseconds timer_interval = milliseconds(vecInfoList[i]->m_nInterval);
        timer_events_.Add_Timer(vecInfoList[i]->m_nID, timer_interval, timer_run_execute, vecInfoList[i]);
    }

    //获得当前工作线程配置列表
    m_T2MList.clear();
    m_M2TList.clear();

    TiXmlElement* pWorkThreadID = NULL;
    TiXmlElement* pMessageID    = NULL;

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
    for (auto it = m_T2MList.begin(); it != m_T2MList.end(); ++it)
    {
        m_ThreadQueueManager.Create(it->first);
    }

    return 0;
}

void CTMService::Close()
{
    //清空
    vector<CTimerInfo* > vecInfoList;
    m_HashTimerList.Get_All_Used(vecInfoList);

    for (int i = 0; i < (int)vecInfoList.size(); i++)
    {
        delete (CTimerInfo*)vecInfoList[i];
    }

    m_HashTimerList.Close();

    m_ThreadQueueManager.Close();

    timer_events_.Close();

    this_thread::sleep_for(std::chrono::milliseconds(100));
}

int CTMService::AddMessage(string strName, unsigned long long nMessagePos, long sec, long usec, int _Message_id, void* _arg, Enum_Timer_Mode emTimerMode, IMessagePrecess* pMessagePrecess)
{
    if (0 == m_nActive)
    {
        return -2;
    }

    CTimerInfo* pTimerInfo = m_HashTimerList.Get_Hash_Box_Data(strName.c_str());

    if (NULL == pTimerInfo)
    {
        return -1;
    }

    if ((int)pTimerInfo->m_vecEventsList.size() >= pTimerInfo->m_nMaxQueueList)
    {
        return -1;
    }

    //根据MessageID查找对应消息处理线程ID
    int _Message_thread_id = 0;
    unordered_map<int, int>::iterator ftm = m_M2TList.find(_Message_id);

    if (m_M2TList.end() != ftm)
    {
        _Message_thread_id = (int)ftm->second;
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
        objEventsInfo.m_nSec             = sec;
        objEventsInfo.m_nUsec            = usec;
        objEventsInfo.m_emTimerMode      = emTimerMode;

        pTimerInfo->AddEventsInfo(objEventsInfo);
    }

    return 0;
}

void* CTMService::DeleteMessage(string strName, unsigned long long nMessagePos)
{
    if (0 == m_nActive)
    {
        return nullptr;
    }

    CTimerInfo* pTimerInfo = m_HashTimerList.Get_Hash_Box_Data(strName.c_str());

    if (NULL == pTimerInfo)
    {
        return nullptr;
    }

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
