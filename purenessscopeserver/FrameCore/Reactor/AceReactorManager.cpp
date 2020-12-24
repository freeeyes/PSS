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
    PSS_LOGGER_DEBUG("[CAceReactor::Create_DEV_POLL]this OS isn't support.");
#endif
}

void CAceReactor::Close()
{
    PSS_LOGGER_DEBUG("[CAceReactor::Close]Begin.");

    if (nullptr != m_pReactor)
    {
        m_pReactor->close();
        SAFE_DELETE(m_pReactor)
    }

    m_emReactorType = EM_REACTOR_MODULE::Reactor_DEV_POLL;
    m_nThreadCount  = 0;
    m_blRun         = false;
    PSS_LOGGER_DEBUG("[CAceReactor::Close]End.");
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
            PSS_LOGGER_DEBUG("[CAceReactor::Init]Unknow nReactorType({0}).", emReactorType);
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
    PSS_LOGGER_DEBUG("[CAceReactor::Open] Begin nReactorID= [{0}].", m_u4ReactorID);

    if (activate(THREAD_PARAM, m_nThreadCount)  == -1)
    {
        PSS_LOGGER_DEBUG("[CAceReactor::Open]activate error ReactorType = [{0}] nThreadCount = [{1}] Start!", m_emReactorType, m_nThreadCount);
        return -1;
    }
    else
    {
        PSS_LOGGER_DEBUG("CAceReactor::Open Begin nReactorID= [{0}] OK.", m_u4ReactorID);
        return 0;
    }
}

int CAceReactor::svc()
{
    if (nullptr == m_pReactor)
    {
        PSS_LOGGER_DEBUG("[CAceReactor::Svc]m_pReactor is nullptr.");
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
        PSS_LOGGER_DEBUG("CAceReactor::Svc]nReactorID= [{0}] end .... ", m_u4ReactorID);
        return 0;
    }
}

bool CAceReactor::Start()
{
    PSS_LOGGER_DEBUG("[CAceReactor::Start] ReactorID = [{0}] ReactorType = [{1}] nThreadCount = [{2}] Start!", GetReactorID(), m_emReactorType, m_nThreadCount);

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
        PSS_LOGGER_DEBUG("[CAceReactor::Stop]m_pReactor is nullptr.");
        return false;
    }

    if (m_blRun == true)
    {
        PSS_LOGGER_DEBUG("[CAceReactor::Stop] nReactorID= [{0}] Begin.", m_u4ReactorID);
        m_pReactor->end_reactor_event_loop();
        PSS_LOGGER_DEBUG("[CAceReactor::Stop] nReactorID= [{0}] End.", m_u4ReactorID);
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
    PSS_LOGGER_DEBUG("[CAceReactor::Close] Begin.");

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
    PSS_LOGGER_DEBUG("[CAceReactor::Close] End.");
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
    PSS_LOGGER_DEBUG("[CAceReactorManager::AddNewReactor]New [{0}] ReactorTxype = [{1}] nThreadCount = [{2}]. pAceReactor=[{3}]", nReactorID, emReactorType, nThreadCount, fmt::ptr(pAceReactor));
    return true;
}

bool CAceReactorManager::StartOtherReactor() const
{
    for_each(m_pReactorList.begin(), m_pReactorList.end(), [](const std::pair<uint16, CAceReactor*>& iter) {
        //先启动非总的Rector
        auto pAceReactor = iter.second;

        if (nullptr != pAceReactor && pAceReactor->GetReactorID() != 0 && false == pAceReactor->Start())
        {
            PSS_LOGGER_DEBUG("[CAceReactorManager::AddNewReactor]Start error.");
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
            PSS_LOGGER_DEBUG("[CAceReactorManager::StopReactor]pAceReactor Stop.");
        }
        });

    return true;
}

CAceReactor* CAceReactorManager::GetAceReactor(int nReactorID)
{
    auto u2ReactorID = (uint16)nReactorID;
    auto f = m_pReactorList.find(u2ReactorID);

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
    auto u2ReactorID = (uint16)nReactorID;
    auto f = m_pReactorList.find(u2ReactorID);

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
    auto u2ReactorID = (uint16)nReactorID;
    auto f = m_pReactorList.find(u2ReactorID);

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
