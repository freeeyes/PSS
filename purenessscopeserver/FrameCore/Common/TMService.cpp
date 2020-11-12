#include "TMService.h"

void timer_run_execute(ITimerInfo* timer_info)
{
    if (NULL != timer_info)
    {
        timer_info->run();
    }
}

CTMService::CTMService()
{
}

CTMService::~CTMService()
{

}

int CTMService::Init(int nNeedLoad)
{
    //��ȡ�����ļ�
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
        //д�������ļ�
        auto pTimerInfo = new CTimerInfo();

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


    //��Ӷ�ʱ��
    auto nTimerSize = (int)vecInfoList.size();

    for (int i = 0; i < nTimerSize; i++)
    {
        milliseconds timer_interval = milliseconds(vecInfoList[i]->m_nInterval);
        timer_events_.Add_Timer(vecInfoList[i]->m_nID, timer_interval, timer_run_execute, vecInfoList[i]);
    }

    //��õ�ǰ�����߳������б�
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

    //���������ļ�������Ӧ����
    for (const auto& f : m_T2MList)
    {
        m_ThreadQueueManager.Create(f.first);
    }

    return 0;
}

void CTMService::Close()
{
    //���
    vector<CTimerInfo* > vecInfoList;
    m_HashTimerList.Get_All_Used(vecInfoList);

    for (const CTimerInfo * pTimerList : vecInfoList)
    {
        SAFE_DELETE(pTimerList);
    }

    m_HashTimerList.Close();

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

    CTimerInfo* pTimerInfo = m_HashTimerList.Get_Hash_Box_Data(strName.c_str());

    if (NULL == pTimerInfo)
    {
        return -1;
    }

    if ((int)pTimerInfo->m_vecEventsList.size() >= pTimerInfo->m_nMaxQueueList)
    {
        return -1;
    }

    //����MessageID���Ҷ�Ӧ��Ϣ�����߳�ID
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
        //��Ҫ����ִ��
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
