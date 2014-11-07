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
        delete m_pReactor;
        m_pReactor = NULL;
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

                /*
                int nDevOpen = devreactor->open(MAX_DEV_POLL_COUNT);
                if(nDevOpen == -1)
                {
                    int nErr = errno;
                    char szError[MAX_BUFF_200] = {'\0'};
                    sprintf_safe(szError, MAX_BUFF_200, "[CAceReactor::Init]ACE_Dev_Poll_Reactor Open Error = (%d).", nErr);

                    throw szError;
                }
                */
                m_pReactor = new ACE_Reactor(devreactor, 1);

                if (NULL == m_pReactor)
                {
                    throw "[CAceReactor::Init]New m_pReactor Error[ACE_Dev_Poll_Reactor].";
                }

                m_nReactorType = Reactor_DEV_POLL;
                break;
            }
#endif
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

    if (activate(THR_NEW_LWP | THR_BOUND | THR_JOINABLE | THR_INHERIT_SCHED, m_nThreadCount)  == -1)
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
        m_pReactor->run_reactor_event_loop();
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
}

CAceReactorManager::~CAceReactorManager(void)
{
    OUR_DEBUG((LM_INFO, "[CAceReactorManager::~CAceReactorManager].\n"));
    Close();
    OUR_DEBUG((LM_INFO, "[CAceReactorManager::~CAceReactorManager]End.\n"));
}

void CAceReactorManager::Close()
{
    for (mapAceReactor::iterator b = m_mapAceReactor.begin(); b != m_mapAceReactor.end(); b++)
    {
        CAceReactor* pAceReactor = (CAceReactor*)b->second;

        if (NULL != pAceReactor)
        {
            pAceReactor->Close();
            SAFE_DELETE(pAceReactor);
        }
    }
}

const char* CAceReactorManager::GetError()
{
    return m_szError;
}

bool CAceReactorManager::AddNewReactor(int nReactorID, int nReactorType, int nThreadCount, int nMaxHandleCount)
{
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

    mapAceReactor::iterator f = m_mapAceReactor.find(nReactorID);

    if (f != m_mapAceReactor.end())
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "[CAceReactorManager::AddNewReactor]CAceReactor is exist[%d].", nReactorID);
        delete pAceReactor;
        return false;
    }

    //if(nReactorID == REACTOR_CLIENTDEFINE)          //第一个Reactor作为默认的
    //{
    //  ACE_Reactor::instance(pAceReactor->GetReactor(), false);
    //}
    m_mapAceReactor.insert(mapAceReactor::value_type(nReactorID, pAceReactor));
    OUR_DEBUG((LM_INFO, "[CAceReactorManager::AddNewReactor]New [%d] ReactorTxype = [%d] nThreadCount = [%d]. pAceReactor=[%@]\n", nReactorID, nReactorType, nThreadCount, pAceReactor));
    return true;
}

bool CAceReactorManager::StartReactor()
{
    //先启动非总的Rector
    for (mapAceReactor::iterator b = m_mapAceReactor.begin(); b != m_mapAceReactor.end(); b++)
    {
        //int nReactorID           = (int)b->first;
        CAceReactor* pAceReactor = (CAceReactor*)b->second;

        if (NULL != pAceReactor)
        {
        		pAceReactor->Start();
        }
    }

    return true;
}

bool CAceReactorManager::StartReactorDefault()
{
    OUR_DEBUG((LM_INFO, "[CAceReactorManager::StartReactorDefault]..1. .\n"));
    //启动默认的 Reactor
    mapAceReactor::iterator f = m_mapAceReactor.find(REACTOR_CLIENTDEFINE);

    if (f != m_mapAceReactor.end())
    {
        CAceReactor* pAceReactor = (CAceReactor*)f->second;

        if (NULL != pAceReactor)
        {
            pAceReactor->Start();
        }

        OUR_DEBUG((LM_INFO, "[CAceReactorManager::StartReactorDefault]... 2.\n"));
    }

    return true;
}

bool CAceReactorManager::StopReactor()
{
    for (mapAceReactor::iterator b = m_mapAceReactor.begin(); b != m_mapAceReactor.end(); b++)
    {
    		int nReactorID           = (int)b->first;
        CAceReactor* pAceReactor = (CAceReactor*)b->second;
        OUR_DEBUG((LM_ERROR, "[CAceReactorManager::StopReactor]nReactorID=%d.\n", nReactorID));

        if (NULL != pAceReactor)
        {
            pAceReactor->Stop();
        }
    }

    return true;
}

CAceReactor* CAceReactorManager::GetAceReactor(int nReactorID)
{
    mapAceReactor::iterator f = m_mapAceReactor.find(nReactorID);

    if (f != m_mapAceReactor.end())
    {
        return (CAceReactor*)f->second;
    }
    else
    {
        return NULL;
    }
}

ACE_Reactor* CAceReactorManager::GetAce_Reactor(int nReactorID)
{
    mapAceReactor::iterator f = m_mapAceReactor.find(nReactorID);

    if (f != m_mapAceReactor.end())
    {
        CAceReactor* pAceReactor = (CAceReactor*)f->second;

        if (NULL != pAceReactor)
        {
            //OUR_DEBUG((LM_INFO, "CAceReactorManager::GetAce_Reactor id=[%d] pAceReactor=[0x%@]\n",nReactorID, pAceReactor));
            return pAceReactor->GetReactor();
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
}
