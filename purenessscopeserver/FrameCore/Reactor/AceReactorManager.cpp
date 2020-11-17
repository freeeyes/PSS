#include "AceReactorManager.h"

CAceReactor::CAceReactor()
{
}

void CAceReactor::SetReactorID(uint32 u4ReactorID)
{
    m_u4ReactorID = u4ReactorID;
}

uint32 CAceReactor::GetReactorID() const
{
    return m_u4ReactorID;
}

#if PSS_PLATFORM == PLATFORM_WIN
void CAceReactor::Create_Reactor_WFMO()
{
    ACE_WFMO_Reactor* wfmoreactor = new ACE_WFMO_Reactor();

    if (nullptr == wfmoreactor)
    {
        throw std::domain_error("[CAceReactor::Init]New ACE_WFMO_Reactor Error.");
    }

    m_pReactor = new ACE_Reactor(wfmoreactor, 1);

    if (nullptr == m_pReactor)
    {
        throw std::domain_error("[CAceReactor::Init]New m_pReactor Error[ACE_WFMO_Reactor].");
    }

    m_nReactorType = Reactor_WFMO;
}
#endif

void CAceReactor::Create_Reactor_Select()
{
    ACE_Select_Reactor* selectreactor = new ACE_Select_Reactor();

    if (nullptr == selectreactor)
    {
        throw std::domain_error("[CAceReactor::Init]New ACE_Select_Reactor Error.");
    }

    m_pReactor = new ACE_Reactor(selectreactor, true);

    if (nullptr == m_pReactor)
    {
        throw std::domain_error("[CAceReactor::Init]New m_pReactor Error[ACE_Select_Reactor].");
    }

    m_nReactorType = Reactor_Select;
}

void CAceReactor::Create_Reactor_TP()
{
    ACE_TP_Reactor* tpreactor = new ACE_TP_Reactor();

    if (nullptr == tpreactor)
    {
        throw std::domain_error("[CAceReactor::Init]New ACE_TP_Reactor Error.");
    }

    m_pReactor = new ACE_Reactor(tpreactor, true);

    if (nullptr == m_pReactor)
    {
        throw std::domain_error("[CAceReactor::Init]New m_pReactor Error[ACE_TP_Reactor].");
    }

    m_nReactorType = Reactor_TP;
}

void CAceReactor::Create_DEV_POLL(int nMaxHandleCount)
{
#ifdef ACE_HAS_EVENT_POLL
    ACE_Dev_Poll_Reactor* devreactor = new ACE_Dev_Poll_Reactor(nMaxHandleCount);

    if (nullptr == devreactor)
    {
        throw std::domain_error("[CAceReactor::Init]New ACE_Dev_Poll_Reactor Error.");
    }

    m_pReactor = new ACE_Reactor(devreactor, true);

    if (nullptr == m_pReactor)
    {
        throw std::domain_error("[CAceReactor::Init]New m_pReactor Error[ACE_Dev_Poll_Reactor].");
    }

    m_nReactorType = Reactor_DEV_POLL;
#else
    OUR_DEBUG((LM_INFO, "[CAceReactor::Create_DEV_POLL]this OS isn't support.\n"));
#endif
}

void CAceReactor::Close()
{
    OUR_DEBUG((LM_INFO, "[CAceReactor::Close]Begin.\n"));

    if (nullptr != m_pReactor)
    {
        m_pReactor->close();
        SAFE_DELETE(m_pReactor)
    }

    m_nReactorType = 0;
    m_nThreadCount = 0;
    m_blRun        = false;
    OUR_DEBUG((LM_INFO, "[CAceReactor::Close]End.\n"));
}

bool CAceReactor::Init(int nReactorType, int nThreadCount, int nMaxHandleCount)
{
    try
    {
        switch (nReactorType)
        {
#if PSS_PLATFORM == PLATFORM_WIN

        case Reactor_WFMO:    //�������������Windows������Ĭ�Ϸ�Ӧ���������COM����������ʹ��Reactor_WFMO_msg
            Create_Reactor_WFMO();
            break;
#endif

        case Reactor_Select:    //�����Ĺ����Ƿ����ڷ�Windows��Ĭ�Ϸ�Ӧ��
            Create_Reactor_Select();
            break;

        case Reactor_TP:
            Create_Reactor_TP();
            break;

#ifdef ACE_HAS_EVENT_POLL     //Linux�µ�EPollģ��

        case Reactor_DEV_POLL:
        case Reactor_DEV_POLL_ET:
            Create_DEV_POLL(nMaxHandleCount);
            break;
#endif

        default:
            OUR_DEBUG((LM_INFO, "[CAceReactor::Init]Unknow nReactorType(%d).\n", nReactorType));
            return false;
        }

        m_nThreadCount = nThreadCount;
        return true;
    }
    catch (const std::domain_error& ex)
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "%s", ex.what());
        return false;
    }
}

int CAceReactor::open()
{
    OUR_DEBUG((LM_ERROR, "[CAceReactor::Open] Begin nReactorID= [%d].\n", m_u4ReactorID));

    if (activate(THREAD_PARAM, m_nThreadCount)  == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CAceReactor::Open]activate error ReactorType = [%d] nThreadCount = [%d] Start!\n", m_nReactorType, m_nThreadCount));
        return -1;
    }
    else
    {
        OUR_DEBUG((LM_ERROR, "CAceReactor::Open Begin nReactorID= [%d] OK\n", m_u4ReactorID));
        return 0;
    }
}

int CAceReactor::svc()
{
    if (nullptr == m_pReactor)
    {
        OUR_DEBUG((LM_ERROR, "[CAceReactor::Svc]m_pReactor is nullptr.\n", m_nReactorType, m_nThreadCount));
        return -1;
    }
    else
    {
        m_blRun = true;

        m_pReactor->owner(ACE_OS::thr_self());

        while (m_pReactor->reactor_event_loop_done() == 0)
        {
            m_pReactor->run_reactor_event_loop();
        }

        Close();
        OUR_DEBUG((LM_ERROR, "CAceReactor::Svc]nReactorID= [%d] end .... \n", m_u4ReactorID));
        return 0;
    }
}

bool CAceReactor::Start()
{
    OUR_DEBUG((LM_INFO, "[CAceReactor::Start] ReactorID = [%d] ReactorType = [%d] nThreadCount = [%d] Start!\n", GetReactorID(), m_nReactorType, m_nThreadCount));

    if (0 == open())
    {
        return true;
    }
    else
    {
        return false;
    }

}

bool CAceReactor::Stop()
{
    if (nullptr == m_pReactor)
    {
        OUR_DEBUG((LM_ERROR, "[CAceReactor::Stop]m_pReactor is nullptr.\n", m_nReactorType, m_nThreadCount));
        return false;
    }

    if (m_blRun == true)
    {
        OUR_DEBUG((LM_ERROR, "[CAceReactor::Stop] nReactorID= [%d] Begin.\n", m_u4ReactorID));
        m_pReactor->end_reactor_event_loop();
        OUR_DEBUG((LM_ERROR, "[CAceReactor::Stop] nReactorID= [%d] End.\n", m_u4ReactorID));
        m_blRun = false;
    }

    return true;
}

const char* CAceReactor::GetError() const
{
    return m_szError;
}

int CAceReactor::GetThreadCount() const
{
    return m_nThreadCount;
}

int CAceReactor::GetReactorType() const
{
    return m_nReactorType;
}

ACE_Reactor* CAceReactor::GetReactor()
{
    return m_pReactor;
}


CAceReactorManager::CAceReactorManager(void)
{
}

void CAceReactorManager::Close()
{
    OUR_DEBUG((LM_ERROR, "[CAceReactor::Close] Begin.\n"));

    if(nullptr != m_pReactorList)
    {
        for (uint16 i = 0; i < m_u2RectorCount; i++)
        {
            CAceReactor* pAceReactor = m_pReactorList[i];

            if (nullptr != pAceReactor)
            {
                pAceReactor->Stop();

                //�ȴ�SVC����
                ACE_Time_Value tvSleep(0, 50000);
                ACE_OS::sleep(tvSleep);

                SAFE_DELETE(pAceReactor);
            }
        }
    }

    SAFE_DELETE_ARRAY(m_pReactorList);
    m_u2RectorCount = 0;
    OUR_DEBUG((LM_ERROR, "[CAceReactor::Close] End.\n"));
}

void CAceReactorManager::Init(uint16 u2Count)
{
    m_pReactorList  = (CAceReactor** )new char[sizeof(CAceReactor*)*u2Count];
    ACE_OS::memset(m_pReactorList, 0, sizeof(CAceReactor*)*u2Count);
    m_u2RectorCount = u2Count;
}

const char* CAceReactorManager::GetError() const
{
    return m_szError;
}

bool CAceReactorManager::AddNewReactor(int nReactorID, int nReactorType, int nThreadCount, int nMaxHandleCount)
{
    if(nReactorID < 0 || nReactorID > m_u2RectorCount)
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "[CAceProactorManager::AddNewProactor]New CAceProactor is more than max Proactor list.");
        return false;
    }

    CAceReactor* pAceReactor = new CAceReactor();

    pAceReactor->SetReactorID((uint32)nReactorID);
    bool blState = pAceReactor->Init(nReactorType, nThreadCount, nMaxHandleCount);

    if (!blState)
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "%s", pAceReactor->GetError());
        delete pAceReactor;
        return false;
    }

    if(nullptr != m_pReactorList[nReactorID])
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "[CAceReactorManager::AddNewReactor]CAceReactor is exist[%d].", nReactorID);
        delete pAceReactor;
        return false;
    }

    m_pReactorList[nReactorID] = pAceReactor;
    OUR_DEBUG((LM_INFO, "[CAceReactorManager::AddNewReactor]New [%d] ReactorTxype = [%d] nThreadCount = [%d]. pAceReactor=[%@]\n", nReactorID, nReactorType, nThreadCount, pAceReactor));
    return true;
}

bool CAceReactorManager::StartOtherReactor() const
{
    //���������ܵ�Rector
    for (uint16 i = 1; i < m_u2RectorCount; i++)
    {
        CAceReactor* pAceReactor = m_pReactorList[i];

        if (nullptr != pAceReactor && false == pAceReactor->Start())
        {
            OUR_DEBUG((LM_INFO, "[CAceReactorManager::AddNewReactor]Start error.\n"));
        }
    }

    return true;
}


bool CAceReactorManager::StartClientReactor() const
{
    return m_pReactorList[0]->Start();
}

bool CAceReactorManager::StopReactor() const
{
    for (uint16 i = 0; i < m_u2RectorCount; i++)
    {
        CAceReactor* pAceReactor = m_pReactorList[i];

        if (nullptr != pAceReactor)
        {
            OUR_DEBUG((LM_ERROR, "[CAceReactorManager::StopReactor]nReactorID=%d.\n", pAceReactor->GetReactorID()));

            if (false == pAceReactor->Stop())
            {
                OUR_DEBUG((LM_ERROR, "[CAceReactorManager::StopReactor]pAceReactor Stop.\n"));
            }
        }
    }

    return true;
}

CAceReactor* CAceReactorManager::GetAceReactor(int nReactorID)
{
    if(nReactorID < 0 || nReactorID >= m_u2RectorCount)
    {
        return nullptr;
    }

    return m_pReactorList[nReactorID];
}

ACE_Reactor* CAceReactorManager::GetAce_Reactor(int nReactorID)
{
    if(nReactorID < 0 || nReactorID >= m_u2RectorCount)
    {
        return nullptr;
    }

    if (nullptr != m_pReactorList[nReactorID])
    {
        return m_pReactorList[nReactorID]->GetReactor();
    }
    else
    {
        return nullptr;
    }
}

ACE_Reactor* CAceReactorManager::GetAce_Client_Reactor(int nReactorID)
{
    //���ﷵ�ؿͻ������ӷ�������Ҫ�õ��ķ�Ӧ��
    //����ķ�Ӧ��������3��������Ӧ��֮��ģ����ֻ������������Ӧ������Ĭ��ȡ�õ�һ����
    int nClientReactor = nReactorID + 3;

    if(nClientReactor < 0 || nClientReactor >= m_u2RectorCount)
    {
        return nullptr;
    }

    if (nullptr != m_pReactorList[nClientReactor])
    {
        return m_pReactorList[nClientReactor]->GetReactor();
    }
    else
    {
        return nullptr;
    }

}

uint32 CAceReactorManager::GetClientReactorCount() const
{
    uint32 u4Count = (uint32)m_u2RectorCount;

    if(u4Count > 3)
    {
        return u4Count - 3;
    }
    else
    {
        return 0;
    }
}
