#include "AceReactorManager.h"

CAceReactor::CAceReactor()
{
    m_pReactor     = NULL;
    m_nReactorType = 0;
    m_nThreadCount = 0;
    m_szError[0]   = '\0';
    m_blRun        = false;
    m_u4ReactorID  = 0;
}

void CAceReactor::SetReactorID(uint32 u4ReactorID)
{
    m_u4ReactorID = u4ReactorID;
}

uint32 CAceReactor::GetReactorID()
{
    return m_u4ReactorID;
}

CAceReactor::~CAceReactor()
{
    OUR_DEBUG((LM_INFO, "[CAceReactor::~CAceReactor].\n"));
    Close();
    OUR_DEBUG((LM_INFO, "[CAceReactor::~CAceReactor] End.\n"));
}

void CAceReactor::Close()
{
    if (NULL != m_pReactor)
    {
        m_pReactor->close();
        SAFE_DELETE(m_pReactor)
    }

    m_nReactorType = 0;
    m_nThreadCount = 0;
    m_blRun        = false;
}

bool CAceReactor::Init(int nReactorType, int nThreadCount, int nMaxHandleCount)
{
    try
    {
        switch (nReactorType)
        {
#ifdef WIN32

        case Reactor_WFMO:    //这个功能限制于Windows操作的默认反应器，如果是COM服务器可以使用Reactor_WFMO_msg
            {
                ACE_WFMO_Reactor* wfmoreactor = new ACE_WFMO_Reactor();

                if (NULL == wfmoreactor)
                {
                    throw "[CAceReactor::Init]New ACE_WFMO_Reactor Error.";
                }

                m_pReactor = new ACE_Reactor(wfmoreactor, 1);

                if (NULL == m_pReactor)
                {
                    throw "[CAceReactor::Init]New m_pReactor Error[ACE_WFMO_Reactor].";
                }

                m_nReactorType = Reactor_WFMO;
                break;
            }

#endif

        case Reactor_Select:    //这个类的功能是服务于非Windows的默认反应器
            {
                ACE_Select_Reactor* selectreactor = new ACE_Select_Reactor();

                if (NULL == selectreactor)
                {
                    throw "[CAceReactor::Init]New ACE_Select_Reactor Error.";
                }

                m_pReactor = new ACE_Reactor(selectreactor, 1);

                if (NULL == m_pReactor)
                {
                    throw "[CAceReactor::Init]New m_pReactor Error[ACE_Select_Reactor].";
                }

                m_nReactorType = Reactor_Select;
                break;
            }

        case Reactor_TP:
            {
                ACE_TP_Reactor* tpreactor = new ACE_TP_Reactor();

                if (NULL == tpreactor)
                {
                    throw "[CAceReactor::Init]New ACE_TP_Reactor Error.";
                }

                m_pReactor = new ACE_Reactor(tpreactor, 1);

                if (NULL == m_pReactor)
                {
                    throw "[CAceReactor::Init]New m_pReactor Error[ACE_TP_Reactor].";
                }

                m_nReactorType = Reactor_TP;
                break;
            }

#ifdef ACE_HAS_EVENT_POLL     //Linux下的EPoll模型

        case Reactor_DEV_POLL:
            {
                ACE_Dev_Poll_Reactor* devreactor = new ACE_Dev_Poll_Reactor(nMaxHandleCount);

                if (NULL == devreactor)
                {
                    throw "[CAceReactor::Init]New ACE_Dev_Poll_Reactor Error.";
                }

                m_pReactor = new ACE_Reactor(devreactor, 1);

                if (NULL == m_pReactor)
                {
                    throw "[CAceReactor::Init]New m_pReactor Error[ACE_Dev_Poll_Reactor].";
                }

                m_nReactorType = Reactor_DEV_POLL;
                break;
            }

        case Reactor_DEV_POLL_ET:
            {
                My_ACE_Dev_Poll_Reactor* devreactor = new My_ACE_Dev_Poll_Reactor(nMaxHandleCount);

                if (NULL == devreactor)
                {
                    throw "[CAceReactor::Init]New ACE_Dev_Poll_Reactor Error.";
                }

                m_pReactor = new ACE_Reactor(devreactor, 1);

                if (NULL == m_pReactor)
                {
                    throw "[CAceReactor::Init]New m_pReactor Error[ACE_Dev_Poll_Reactor].";
                }

                m_nReactorType = Reactor_DEV_POLL_ET;
                break;
            }

#endif

        default:
            {
                OUR_DEBUG((LM_INFO, "[CAceReactor::Init]Unknow nReactorType(%d).\n", nReactorType));
                return false;
            }
        }

        m_nThreadCount = nThreadCount;
        return true;
    }
    catch (const char* szError)
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "%s", szError);
        return false;
    }
}

int CAceReactor::open(void* args)
{
    if (args != NULL)
    {
        OUR_DEBUG((LM_ERROR, "[CAceReactor::open] args is not NULL.\n"));
    }

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
    if (NULL == m_pReactor)
    {
        OUR_DEBUG((LM_ERROR, "[CAceReactor::Svc]m_pReactor is NULL.\n", m_nReactorType, m_nThreadCount));
        return -1;
    }
    else
    {
        m_blRun = true;

        //OUR_DEBUG((LM_ERROR, "CAceReactor::Svc] (%P|%t) Begin nReactorID= [%d] begin.... \n", m_u4ReactorID));
        m_pReactor->owner(ACE_OS::thr_self());

        while (m_pReactor->reactor_event_loop_done() == 0)
        {
            m_pReactor->run_reactor_event_loop();
        }

        OUR_DEBUG((LM_ERROR, "CAceReactor::Svc]Begin nReactorID= [%d] end .... \n", m_u4ReactorID));
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
    if (NULL == m_pReactor)
    {
        OUR_DEBUG((LM_ERROR, "[CAceReactor::Stop]m_pReactor is NULL.\n", m_nReactorType, m_nThreadCount));
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

const char* CAceReactor::GetError()
{
    return m_szError;
}

int CAceReactor::GetThreadCount()
{
    return m_nThreadCount;
}

int CAceReactor::GetReactorType()
{
    return m_nReactorType;
}

ACE_Reactor* CAceReactor::GetReactor()
{
    return m_pReactor;
}


CAceReactorManager::CAceReactorManager(void)
{
    m_pReactorList  = NULL;
    m_u2RectorCount = 0;
    ACE_OS::memset(m_szError, 0, MAX_BUFF_500);
}

CAceReactorManager::~CAceReactorManager(void)
{
    OUR_DEBUG((LM_INFO, "[CAceReactorManager::~CAceReactorManager].\n"));
    Close();
    OUR_DEBUG((LM_INFO, "[CAceReactorManager::~CAceReactorManager]End.\n"));
}

void CAceReactorManager::Close()
{
    if(NULL != m_pReactorList)
    {
        for (uint16 i = 0; i < m_u2RectorCount; i++)
        {
            CAceReactor* pAceReactor = m_pReactorList[i];

            if (NULL != pAceReactor)
            {
                pAceReactor->Close();
                SAFE_DELETE(pAceReactor);
            }
        }
    }

    SAFE_DELETE_ARRAY(m_pReactorList);
    m_u2RectorCount = 0;
}

void CAceReactorManager::Init(uint16 u2Count)
{
    //Close();

    m_pReactorList  = (CAceReactor** )new char[sizeof(CAceReactor*)*u2Count];;
    ACE_OS::memset(m_pReactorList, 0, sizeof(CAceReactor*)*u2Count);
    m_u2RectorCount = u2Count;
}

const char* CAceReactorManager::GetError()
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

    if (NULL == pAceReactor)
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "[CAceReactorManager::AddNewReactor]New CAceReactor is NULL.");
        return false;
    }

    /*
    if (nReactorID == REACTOR_CLIENTDEFINE)
    {
    nThreadCount = 0;
    }
    */

    pAceReactor->SetReactorID((uint32)nReactorID);
    bool blState = pAceReactor->Init(nReactorType, nThreadCount, nMaxHandleCount);

    if (!blState)
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "%s", pAceReactor->GetError());
        delete pAceReactor;
        return false;
    }

    if(NULL != m_pReactorList[nReactorID])
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "[CAceReactorManager::AddNewReactor]CAceReactor is exist[%d].", nReactorID);
        delete pAceReactor;
        return false;
    }

    //if(nReactorID == REACTOR_CLIENTDEFINE)          //第一个Reactor作为默认的
    //{
    //  ACE_Reactor::instance(pAceReactor->GetReactor(), false);
    //}
    m_pReactorList[nReactorID] = pAceReactor;
    OUR_DEBUG((LM_INFO, "[CAceReactorManager::AddNewReactor]New [%d] ReactorTxype = [%d] nThreadCount = [%d]. pAceReactor=[%@]\n", nReactorID, nReactorType, nThreadCount, pAceReactor));
    return true;
}

bool CAceReactorManager::StartReactor()
{
    //先启动非总的Rector
    for (uint16 i = 0; i < m_u2RectorCount; i++)
    {
        CAceReactor* pAceReactor = m_pReactorList[i];

        if (NULL != pAceReactor)
        {
            if (false == pAceReactor->Start())
            {
                OUR_DEBUG((LM_INFO, "[CAceReactorManager::AddNewReactor]Start error.\n"));
            }
        }
    }

    return true;
}

bool CAceReactorManager::StopReactor()
{
    for (uint16 i = 0; i < m_u2RectorCount; i++)
    {
        CAceReactor* pAceReactor = m_pReactorList[i];

        if (NULL != pAceReactor)
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
        return NULL;
    }

    return m_pReactorList[nReactorID];
}

ACE_Reactor* CAceReactorManager::GetAce_Reactor(int nReactorID)
{
    if(nReactorID < 0 || nReactorID >= m_u2RectorCount)
    {
        return NULL;
    }

    if (NULL != m_pReactorList[nReactorID])
    {
        //OUR_DEBUG((LM_INFO, "CAceReactorManager::GetAce_Reactor id=[%d] pAceReactor=[0x%@]\n",nReactorID, pAceReactor));
        return m_pReactorList[nReactorID]->GetReactor();
    }
    else
    {
        return NULL;
    }
}

ACE_Reactor* CAceReactorManager::GetAce_Client_Reactor(int nReactorID)
{
    //这里返回客户端连接服务器需要用到的反应器
    //这里的反应器必须是3个基础反应器之外的，如果只有三个基础反应器，则默认取得第一个。
    int nClientReactor = nReactorID + 3;

    if(nClientReactor < 0 || nClientReactor >= m_u2RectorCount)
    {
        return NULL;
    }

    if (NULL != m_pReactorList[nClientReactor])
    {
        //OUR_DEBUG((LM_INFO, "CAceReactorManager::GetAce_Reactor id=[%d] pAceReactor=[0x%@]\n",nReactorID, pAceReactor));
        return m_pReactorList[nClientReactor]->GetReactor();
    }
    else
    {
        return NULL;
    }

}

uint32 CAceReactorManager::GetClientReactorCount()
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
