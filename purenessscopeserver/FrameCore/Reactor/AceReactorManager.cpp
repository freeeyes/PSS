#include "AceReactorManager.h"

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

    m_emReactorType = EM_REACTOR_MODULE::Reactor_WFMO;
}
#endif

void CAceReactor::Create_Reactor_Select()
{
    auto selectreactor = new ACE_Select_Reactor();

    if (nullptr == selectreactor)
    {
        throw std::domain_error("[CAceReactor::Init]New ACE_Select_Reactor Error.");
    }

    m_pReactor = new ACE_Reactor(selectreactor, true);

    if (nullptr == m_pReactor)
    {
        throw std::domain_error("[CAceReactor::Init]New m_pReactor Error[ACE_Select_Reactor].");
    }

    m_emReactorType = EM_REACTOR_MODULE::Reactor_Select;
}

void CAceReactor::Create_Reactor_TP()
{
    auto tpreactor = new ACE_TP_Reactor();

    if (nullptr == tpreactor)
    {
        throw std::domain_error("[CAceReactor::Init]New ACE_TP_Reactor Error.");
    }

    m_pReactor = new ACE_Reactor(tpreactor, true);

    if (nullptr == m_pReactor)
    {
        throw std::domain_error("[CAceReactor::Init]New m_pReactor Error[ACE_TP_Reactor].");
    }

    m_emReactorType = EM_REACTOR_MODULE::Reactor_TP;
}

void CAceReactor::Create_DEV_POLL(int nMaxHandleCount)
{
#ifdef ACE_HAS_EVENT_POLL
    auto devreactor = new ACE_Dev_Poll_Reactor(nMaxHandleCount);

    if (nullptr == devreactor)
    {
        throw std::domain_error("[CAceReactor::Init]New ACE_Dev_Poll_Reactor Error.");
    }

    m_pReactor = new ACE_Reactor(devreactor, true);

    if (nullptr == m_pReactor)
    {
        throw std::domain_error("[CAceReactor::Init]New m_pReactor Error[ACE_Dev_Poll_Reactor].");
    }

    m_emReactorType = EM_REACTOR_MODULE::Reactor_DEV_POLL;
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

    m_emReactorType = EM_REACTOR_MODULE::Reactor_DEV_POLL;
    m_nThreadCount  = 0;
    m_blRun         = false;
    OUR_DEBUG((LM_INFO, "[CAceReactor::Close]End.\n"));
}

bool CAceReactor::Init(EM_REACTOR_MODULE emReactorType, int nThreadCount, int nMaxHandleCount)
{
    try
    {
        switch (emReactorType)
        {
#if PSS_PLATFORM == PLATFORM_WIN

        case EM_REACTOR_MODULE::Reactor_WFMO:    //这个功能限制于Windows操作的默认反应器，如果是COM服务器可以使用Reactor_WFMO_msg
            Create_Reactor_WFMO();
            break;
#endif

        case EM_REACTOR_MODULE::Reactor_Select:    //这个类的功能是服务于非Windows的默认反应器
            Create_Reactor_Select();
            break;

        case EM_REACTOR_MODULE::Reactor_TP:
            Create_Reactor_TP();
            break;

#ifdef ACE_HAS_EVENT_POLL     //Linux下的EPoll模型

        case EM_REACTOR_MODULE::Reactor_DEV_POLL:
        case EM_REACTOR_MODULE::Reactor_DEV_POLL_ET:
            Create_DEV_POLL(nMaxHandleCount);
            break;
#endif

        default:
            OUR_DEBUG((LM_INFO, "[CAceReactor::Init]Unknow nReactorType(%d).\n", emReactorType));
            return false;
        }

        m_nThreadCount = nThreadCount;
        return true;
    }
    catch (const std::domain_error& ex)
    {
        m_strError = ex.what();
        return false;
    }
}

int CAceReactor::open()
{
    OUR_DEBUG((LM_ERROR, "[CAceReactor::Open] Begin nReactorID= [%d].\n", m_u4ReactorID));

    if (activate(THREAD_PARAM, m_nThreadCount)  == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CAceReactor::Open]activate error ReactorType = [%d] nThreadCount = [%d] Start!\n", m_emReactorType, m_nThreadCount));
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
        OUR_DEBUG((LM_ERROR, "[CAceReactor::Svc]m_pReactor is nullptr.\n", m_emReactorType, m_nThreadCount));
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
    OUR_DEBUG((LM_INFO, "[CAceReactor::Start] ReactorID = [%d] ReactorType = [%d] nThreadCount = [%d] Start!\n", GetReactorID(), m_emReactorType, m_nThreadCount));

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
        OUR_DEBUG((LM_ERROR, "[CAceReactor::Stop]m_pReactor is nullptr.\n", m_emReactorType, m_nThreadCount));
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
    return m_strError.c_str();
}

int CAceReactor::GetThreadCount() const
{
    return m_nThreadCount;
}

EM_REACTOR_MODULE CAceReactor::GetReactorType() const
{
    return m_emReactorType;
}

ACE_Reactor* CAceReactor::GetReactor()
{
    return m_pReactor;
}

void CAceReactorManager::Close()
{
    OUR_DEBUG((LM_ERROR, "[CAceReactor::Close] Begin.\n"));

    for_each(m_pReactorList.begin(), m_pReactorList.end(), [](const std::pair<uint16, CAceReactor*>& iter) {
        //清除对象
        auto pAceReactor = iter.second;
        pAceReactor->Stop();

        //等待SVC结束
        ACE_Time_Value tvSleep(0, 50000);
        ACE_OS::sleep(tvSleep);

        SAFE_DELETE(pAceReactor);
        });

    m_pReactorList.clear();

    m_u2RectorCount = 0;
    OUR_DEBUG((LM_ERROR, "[CAceReactor::Close] End.\n"));
}

void CAceReactorManager::Init(uint16 u2Count)
{
    m_u2RectorCount = u2Count;
}

const char* CAceReactorManager::GetError() const
{
    return m_strError.c_str();
}

bool CAceReactorManager::AddNewReactor(int nReactorID, EM_REACTOR_MODULE emReactorType, int nThreadCount, int nMaxHandleCount)
{
    auto pAceReactor = new CAceReactor();
    auto u2ReactorID = (uint16)nReactorID;

    pAceReactor->SetReactorID((uint32)nReactorID);
    bool blState = pAceReactor->Init(emReactorType, nThreadCount, nMaxHandleCount);

    if (!blState)
    {
        m_strError = pAceReactor->GetError();
        delete pAceReactor;
        return false;
    }

    if(nullptr != m_pReactorList[u2ReactorID])
    {
        m_strError = fmt::format("[CAceReactorManager::AddNewReactor]CAceReactor is exist[{0}].", nReactorID);
        delete pAceReactor;
        return false;
    }

    m_pReactorList[u2ReactorID] = pAceReactor;
    OUR_DEBUG((LM_INFO, "[CAceReactorManager::AddNewReactor]New [%d] ReactorTxype = [%d] nThreadCount = [%d]. pAceReactor=[%@]\n", nReactorID, emReactorType, nThreadCount, pAceReactor));
    return true;
}

bool CAceReactorManager::StartOtherReactor() const
{
    for_each(m_pReactorList.begin(), m_pReactorList.end(), [](const std::pair<uint16, CAceReactor*>& iter) {
        //先启动非总的Rector
        auto pAceReactor = iter.second;

        if (nullptr != pAceReactor && pAceReactor->GetReactorID() != 0 && false == pAceReactor->Start())
        {
            OUR_DEBUG((LM_INFO, "[CAceReactorManager::AddNewReactor]Start error.\n"));
        }
        });

    return true;
}


bool CAceReactorManager::StartClientReactor() const
{
    auto f = m_pReactorList.find(0);

    if (m_pReactorList.end() != f)
    {
        return f->second->Start();
    }
    else
    {
        return false;
    }
}

bool CAceReactorManager::StopReactor() const
{
    for_each(m_pReactorList.begin(), m_pReactorList.end(), [](const std::pair<uint16, CAceReactor*>& iter) {
        //先启动非总的Rector
        auto pAceReactor = iter.second;

        if (false == pAceReactor->Stop())
        {
            OUR_DEBUG((LM_ERROR, "[CAceReactorManager::StopReactor]pAceReactor Stop.\n"));
        }
        });

    return true;
}

CAceReactor* CAceReactorManager::GetAceReactor(int nReactorID)
{
    auto f = m_pReactorList.find(nReactorID);

    if (m_pReactorList.end() != f)
    {
        return f->second;
    }
    else
    {
        return nullptr;
    }
}

ACE_Reactor* CAceReactorManager::GetAce_Reactor(int nReactorID)
{
    auto f = m_pReactorList.find(nReactorID);

    if (m_pReactorList.end() != f)
    {
        return f->second->GetReactor();
    }
    else
    {
        return nullptr;
    }
}

ACE_Reactor* CAceReactorManager::GetAce_Client_Reactor(int nReactorID)
{
    //这里返回客户端连接服务器需要用到的反应器
    auto f = m_pReactorList.find(nReactorID);

    if (m_pReactorList.end() != f)
    {
        return f->second->GetReactor();
    }
    else
    {
        return nullptr;
    }

}

uint32 CAceReactorManager::GetClientReactorCount() const
{
    return (uint32)m_pReactorList.size();
}
