#include "AceProactorManager.h"

CAceProactor::CAceProactor()
{
    m_pProactor     = nullptr;
    m_nProactorType = 0;
    m_nThreadCount  = 0;
    m_szError[0]    = '\0';
    m_blRun         = false;
    m_u4ProactorID  = 0;
    ACE_OS::memset(m_szError, 0, MAX_BUFF_500);
}

CAceProactor::~CAceProactor()
{
    Close();
}

void CAceProactor::SetProactorID(uint32 u4ProactorID)
{
    m_u4ProactorID = u4ProactorID;
}

uint32 CAceProactor::GetProactorID()
{
    return m_u4ProactorID;
}

void CAceProactor::Close()
{
    if (nullptr != m_pProactor)
    {
        ACE_Time_Value tvSleep(0, 1000);
        m_pProactor->proactor_end_event_loop();
        ACE_OS::sleep(tvSleep);
        SAFE_DELETE(m_pProactor);
    }

    m_nProactorType = 0;
    m_nThreadCount  = 0;
    m_blRun         = false;
}

bool CAceProactor::Init(int nProactorType, int nThreadCount)
{
    try
    {
        switch (nProactorType)
        {
#if PSS_PLATFORM == PLATFORM_WIN

        case Proactor_WIN32:    //�������������Windows������Ĭ�Ϸ�Ӧ���������COM����������ʹ��Proactor_WFMO_msg
            {
                ACE_WIN32_Proactor* pWin32Proactor = new ACE_WIN32_Proactor(nThreadCount);

                if(nullptr == pWin32Proactor)
                {
                    throw std::domain_error("[CAceProactor::Init]New ACE_WIN32_Proactor Error.");
                }

                m_pProactor = new ACE_Proactor(pWin32Proactor, 1);

                if(nullptr == m_pProactor)
                {
                    throw std::domain_error("[CAceProactor::Init]New m_pProactor Error[ACE_WIN32_Proactor].");
                }

                m_nProactorType = Proactor_WIN32;
                break;
            }

#else

        case Proactor_POSIX:    //�����Ĺ����Ƿ����ڷ�Windows��Ĭ�Ϸ�Ӧ��
            {
                ACE_POSIX_Proactor* pPosixProactor = new ACE_POSIX_Proactor();

                if(nullptr == pPosixProactor)
                {
                    throw std::domain_error("[CAceProactor::Init]New ACE_POSIX_Proactor Error.");
                }

                m_pProactor = new ACE_Proactor(pPosixProactor, 1);

                if(nullptr == m_pProactor)
                {
                    throw std::domain_error("[CAceProactor::Init]New m_pProactor Error[ACE_POSIX_Proactor].");
                }

                m_nProactorType = Proactor_POSIX;
                break;
            }

#endif

        default:
            {
                OUR_DEBUG((LM_INFO, "[CAceReactor::Init]Unknow nReactorType(%d).\n", nProactorType));
                return false;
            }
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

int CAceProactor::open(void* args)
{
    OUR_DEBUG((LM_ERROR, "CAceProactor::Open Begin nProactorID= [%d].\n", m_u4ProactorID));

    if(activate(THREAD_PARAM, MAX_MSG_THREADCOUNT)  == -1)
    {
        m_blRun = false;
        OUR_DEBUG((LM_ERROR, "[CAceProactor::Open]activate error ProactorType = [%d] nThreadCount = [%d] Start!\n", m_nProactorType, m_nThreadCount));
        return -1;
    }
    else
    {
        m_blRun = true;
        OUR_DEBUG((LM_ERROR, "CAceProactor::Open Begin nProactorID= [%d] OK\n", m_u4ProactorID));
        return 0;
    }
}

int CAceProactor::svc()
{
    if(nullptr == m_pProactor)
    {
        OUR_DEBUG((LM_ERROR, "[CAceProactor::Svc]m_pProactor is nullptr.\n", m_nProactorType, m_nThreadCount));
        return -1;
    }
    else
    {
        m_blRun = true;
        m_pProactor->proactor_run_event_loop();
        OUR_DEBUG((LM_ERROR, "[CAceProactor::Svc] (%P|%t) Begin nProactorID= [%d] end .... \n", m_u4ProactorID));
        return 0;
    }
}

bool CAceProactor::Start()
{
    OUR_DEBUG((LM_INFO, "[CAceProactor::Start] ProactorID = [%d] ProactorType = [%d] nThreadCount = [%d] Start!\n", GetProactorID(), m_nProactorType, m_nThreadCount));

    if(0 == open())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CAceProactor::Stop()
{
    if(nullptr == m_pProactor)
    {
        OUR_DEBUG((LM_ERROR, "[CAceProactor::Stop]m_pProactor is nullptr.\n", m_nProactorType, m_nThreadCount));
        return false;
    }

    if(m_blRun == true)
    {
        m_pProactor->proactor_end_event_loop();
    }

    m_blRun = false;

    return true;
}

const char* CAceProactor::GetError()
{
    return m_szError;
}

int CAceProactor::GetThreadCount()
{
    return m_nThreadCount;
}

int CAceProactor::GetProactorType()
{
    return m_nProactorType;
}

ACE_Proactor* CAceProactor::GetProactor()
{
    return m_pProactor;
}

CAceProactorManager::CAceProactorManager(void)
{
    m_pAceProactorList = nullptr;
    m_u2ProactorCount  = 0;
    ACE_OS::memset(m_szError, 0, MAX_BUFF_500);
}

CAceProactorManager::~CAceProactorManager(void)
{
    Close();
}

void CAceProactorManager::Close()
{
    if(nullptr != m_pAceProactorList)
    {
        for(uint16 i = 0; i < m_u2ProactorCount; i++)
        {
            CAceProactor* pAceProactor = m_pAceProactorList[i];

            if(nullptr != pAceProactor)
            {
                pAceProactor->Close();
                SAFE_DELETE(pAceProactor);
            }
        }
    }

    SAFE_DELETE_ARRAY(m_pAceProactorList);
    m_u2ProactorCount = 0;
}

void CAceProactorManager::Init(uint16 u2Count)
{
    Close();

    m_pAceProactorList = new CAceProactor*[u2Count];
    ACE_OS::memset(m_pAceProactorList, 0, sizeof(CAceProactor*)*u2Count);
    m_u2ProactorCount  = u2Count;
}

const char* CAceProactorManager::GetError()
{
    return m_szError;
}

bool CAceProactorManager::AddNewProactor(int nProactorID, int nProactorType, int nThreadCount)
{
    if(nProactorID < 0 || nProactorID > m_u2ProactorCount)
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "[CAceProactorManager::AddNewProactor]New CAceProactor is more than max Proactor list.");
        return false;
    }

    CAceProactor* pAceProactor = new CAceProactor();

    if(nullptr == pAceProactor)
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "[CAceProactorManager::AddNewProactor]New CAceProactor is nullptr.");
        return false;
    }

    pAceProactor->SetProactorID((uint32)nProactorID);
    bool blState = pAceProactor->Init(nProactorType, nThreadCount);

    if(!blState)
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "%s", pAceProactor->GetError());
        SAFE_DELETE(pAceProactor);
        return false;
    }

    if(nullptr != m_pAceProactorList[nProactorID])
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "[CAceProactorManager::AddNewProactor]CAceProactor is exist[%d].", nProactorID);
        SAFE_DELETE(pAceProactor);
        return false;
    }

    m_pAceProactorList[nProactorID] = pAceProactor;
    OUR_DEBUG((LM_INFO, "[CAceProactorManager::AddNewProactor]New [%d] ProactorType = [%d] nThreadCount = [%d]. pAceProactor=[%@]\n", nProactorID, nProactorType, nThreadCount,pAceProactor));
    return true;
}

bool CAceProactorManager::StartOtherProactor()
{
    //���������ܵ�Rector
    for(uint16 i = 1; i < m_u2ProactorCount; i++)
    {
        if(nullptr != m_pAceProactorList[i])
        {
            m_pAceProactorList[i]->Start();
        }
    }

    return true;
}

bool CAceProactorManager::StartClientProactor()
{
    return m_pAceProactorList[0]->Start();
}

bool CAceProactorManager::StopProactor()
{
    for(uint16 i = 0; i < m_u2ProactorCount; i++)
    {
        CAceProactor* pAceProactor = m_pAceProactorList[i];

        if(nullptr != pAceProactor)
        {
            OUR_DEBUG((LM_ERROR, "[CAceProactorManager::StopProactor]ProactorID=%d.\n", pAceProactor->GetProactorID()));

            if (false == pAceProactor->Stop())
            {
                OUR_DEBUG((LM_ERROR, "[CAceProactorManager::StopProactor]ProactorID=%d stop error.\n", pAceProactor->GetProactorID()));
            }
        }
    }

    return true;
}

CAceProactor* CAceProactorManager::GetAceProactor(int nProactorID)
{
    if(nProactorID < 0 || nProactorID >= m_u2ProactorCount)
    {
        return nullptr;
    }

    return m_pAceProactorList[nProactorID];
}

ACE_Proactor* CAceProactorManager::GetAce_Proactor()
{
    return m_pAceProactorList[0]->GetProactor();
}

ACE_Proactor* CAceProactorManager::GetAce_Client_Proactor(int nProactorID)
{
    int nClientProactor = nProactorID + 3;

    if(nClientProactor < 0 || nClientProactor >= m_u2ProactorCount)
    {
        return nullptr;
    }

    if(nullptr != m_pAceProactorList[nClientProactor])
    {
        return m_pAceProactorList[nClientProactor]->GetProactor();
    }
    else
    {
        return nullptr;
    }
}

uint32 CAceProactorManager::GetClientReactorCount()
{
    uint32 u4Count = (uint32)m_u2ProactorCount;

    if(u4Count > 3)
    {
        return u4Count - 3;
    }
    else
    {
        return 0;
    }
}
