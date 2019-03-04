#include "ReTTyClientManager.h"

CReTTyClientManager::CReTTyClientManager(): m_pReactor(NULL), m_u2MaxListCount(MAX_BUFF_100), m_u2TimeCheck(120), m_nTaskID(-1)
{

}

CReTTyClientManager::~CReTTyClientManager()
{
    Close();
}

bool CReTTyClientManager::StartConnectTask()
{
    CancelConnectTask();
    m_nTaskID = App_TimerManager::instance()->schedule(this, (void*)NULL, ACE_OS::gettimeofday() + ACE_Time_Value(m_u2TimeCheck), ACE_Time_Value(m_u2TimeCheck));

    if (m_nTaskID == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CReTTyClientManager::StartConnectTask].StartConnectTask is fail, time is (%d).\n", m_u2TimeCheck));
        return false;
    }

    return true;
}

void CReTTyClientManager::CancelConnectTask()
{
    if (m_nTaskID != -1)
    {
        //杀死之前的定时器，重新开启新的定时器
        App_TimerManager::instance()->cancel(m_nTaskID);
        m_nTaskID = -1;
    }
}

bool CReTTyClientManager::Init(ACE_Reactor* pReactor, uint16 u2MaxTTyCount, uint16 u2TimeCheck)
{
    m_pReactor       = pReactor;
    m_u2MaxListCount = u2MaxTTyCount;
    m_u2TimeCheck    = u2TimeCheck;

    //初始化Hash数组(TCP)
    m_objTTyClientHandlerList.Init((int)m_u2MaxListCount);

    //启动定时器
    StartConnectTask();

    return true;
}

void CReTTyClientManager::Close()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    //如果有定时器，则删除定时器
    CancelConnectTask();

    //关闭所有已存在的链接
    vector<CReTTyHandler*> vecTTyClientHandlerInfo;
    m_objTTyClientHandlerList.Get_All_Used(vecTTyClientHandlerInfo);

    OUR_DEBUG((LM_INFO, "[CReTTyClientManager::Close]Count=%d.\n", vecTTyClientHandlerInfo.size()));

    for (int i = 0; i < (int)vecTTyClientHandlerInfo.size(); i++)
    {
        CReTTyHandler* pTTyClientHandler = vecTTyClientHandlerInfo[i];

        if (NULL != pTTyClientHandler)
        {
            pTTyClientHandler->Close();
            SAFE_DELETE(pTTyClientHandler);
        }
    }

    m_objTTyClientHandlerList.Close();
}

bool CReTTyClientManager::Close(uint16 u2ConnectID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    //先查找这个设备是否已经注册
    char szConnectID[MAX_BUFF_20] = { '\0' };
    sprintf_safe(szConnectID, MAX_BUFF_20, "%d", u2ConnectID);

    CReTTyHandler* pTTyClientHandler = m_objTTyClientHandlerList.Get_Hash_Box_Data(szConnectID);

    if (NULL == pTTyClientHandler)
    {
        OUR_DEBUG((LM_INFO, "[CReTTyClientManager::Close](u2ConnectID=%d) is no exist.\n", u2ConnectID));
        return false;
    }
    else
    {
        m_objTTyClientHandlerList.Del_Hash_Data(szConnectID);
        pTTyClientHandler->Close();
        SAFE_DELETE(pTTyClientHandler);
        return true;
    }
}

bool CReTTyClientManager::Pause(uint16 u2ConnectID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    //先查找这个设备是否已经注册
    char szConnectID[MAX_BUFF_20] = { '\0' };
    sprintf_safe(szConnectID, MAX_BUFF_20, "%d", u2ConnectID);

    CReTTyHandler* pTTyClientHandler = m_objTTyClientHandlerList.Get_Hash_Box_Data(szConnectID);

    if (NULL == pTTyClientHandler)
    {
        OUR_DEBUG((LM_INFO, "[CReTTyClientManager::Pause](u2ConnectID=%d) is no exist.\n", u2ConnectID));
        return false;
    }
    else
    {
        pTTyClientHandler->SetPause(true);
        return true;
    }
}

bool CReTTyClientManager::Resume(uint16 u2ConnectID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    //先查找这个设备是否已经注册
    char szConnectID[MAX_BUFF_20] = { '\0' };
    sprintf_safe(szConnectID, MAX_BUFF_20, "%d", u2ConnectID);

    CReTTyHandler* pTTyClientHandler = m_objTTyClientHandlerList.Get_Hash_Box_Data(szConnectID);

    if (NULL == pTTyClientHandler)
    {
        OUR_DEBUG((LM_INFO, "[CReTTyClientManager::Resume](u2ConnectID=%d) is no exist.\n", u2ConnectID));
        return false;
    }
    else
    {
        pTTyClientHandler->SetPause(false);
        return true;
    }
}

bool CReTTyClientManager::SendMessage(uint16 u2ConnectID, char*& pMessage, uint32 u4Len)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    //先查找这个设备是否已经注册
    char szConnectID[MAX_BUFF_20] = { '\0' };
    sprintf_safe(szConnectID, MAX_BUFF_20, "%d", u2ConnectID);

    CReTTyHandler* pTTyClientHandler = m_objTTyClientHandlerList.Get_Hash_Box_Data(szConnectID);

    if (NULL == pTTyClientHandler)
    {
        OUR_DEBUG((LM_INFO, "[CReTTyClientManager::SendMessage](u2ConnectID=%d) is no exist.\n", u2ConnectID));
        return false;
    }
    else
    {
        return pTTyClientHandler->Send_Data(pMessage, u4Len);
    }
}

int CReTTyClientManager::handle_timeout(const ACE_Time_Value& current_time, const void* act /*= 0*/)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    ACE_UNUSED_ARG(current_time);
    ACE_UNUSED_ARG(act);

    OUR_DEBUG((LM_INFO, "[CReTTyClientManager::handle_timeout](%d)Run.\n", m_objTTyClientHandlerList.Get_Count()));

    //遍历所有的已存在的连接，看看有没有需要重启的
    vector<CReTTyHandler*> vecTTyClientHandlerInfo;
    m_objTTyClientHandlerList.Get_All_Used(vecTTyClientHandlerInfo);

    for (int i = 0; i < (int)vecTTyClientHandlerInfo.size(); i++)
    {
        CReTTyHandler* pTTyClientHandler = vecTTyClientHandlerInfo[i];

        if (NULL != pTTyClientHandler)
        {
            pTTyClientHandler->ConnectTTy();
        }
    }

    return 0;
}

int CReTTyClientManager::Connect(uint16 u2ConnectID, const char* pName, _TTyDevParam& inParam, ITTyMessage* pMessageRecv)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    //先查找这个设备是否已经注册
    char szConnectID[MAX_BUFF_20] = { '\0' };
    sprintf_safe(szConnectID, MAX_BUFF_20, "%d", u2ConnectID);

    CReTTyHandler* pTTyClientHandler = m_objTTyClientHandlerList.Get_Hash_Box_Data(szConnectID);

    if (NULL != pTTyClientHandler)
    {
        OUR_DEBUG((LM_INFO, "[CReTTyClientManager::Connect](%s) is exist.\n", pName));
        return -1;
    }

    pTTyClientHandler = new CReTTyHandler();

    //匹配相关参数
    ACE_TTY_IO::Serial_Params inTTyParams;
    inTTyParams.baudrate          = inParam.m_nBaudRate;
    inTTyParams.databits          = inParam.m_uDatabits;
    inTTyParams.stopbits          = inParam.m_uStopbits;
    inTTyParams.paritymode        = inParam.m_pParitymode;
    inTTyParams.ctsenb            = inParam.m_blCTSenb;
    inTTyParams.rtsenb            = inParam.m_uRTSenb;
    inTTyParams.dsrenb            = inParam.m_blDSRenb;
    inTTyParams.dtrdisable        = inParam.m_blDTRdisable;
    inTTyParams.readtimeoutmsec   = inParam.m_nReadtimeoutmsec;
    inTTyParams.xinenb            = inParam.m_blXinenb;
    inTTyParams.xoutenb           = inParam.m_blXoutenb;
    inTTyParams.modem             = inParam.m_blModem;
    inTTyParams.rcvenb            = inParam.m_blRcvenb;
    inTTyParams.xonlim            = inParam.m_nXOnlim;
    inTTyParams.xofflim           = inParam.m_nXOfflim;
    inTTyParams.readmincharacters = inParam.m_u4Readmincharacters;

    //绑定反应器
    pTTyClientHandler->reactor(m_pReactor);

    if (false == pTTyClientHandler->Init(u2ConnectID, pName, inTTyParams, pMessageRecv))
    {
        OUR_DEBUG((LM_INFO, "[CReTTyClientManager::Connect](%s)pTTyClientHandler Init Error.\n", pName));
        SAFE_DELETE(pTTyClientHandler);
        return -1;
    }

    if (false == m_objTTyClientHandlerList.Add_Hash_Data(szConnectID, pTTyClientHandler))
    {
        OUR_DEBUG((LM_INFO, "[CReTTyClientManager::Connect](%s)Add_Hash_Data Error.\n", pName));
        SAFE_DELETE(pTTyClientHandler);
        return -1;
    }

    return 0;
}

bool CReTTyClientManager::GetClientDevInfo(uint16 u2ConnectID, _TTyDevParam& outParam)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    //先查找这个设备是否已经注册
    char szConnectID[MAX_BUFF_20] = { '\0' };
    sprintf_safe(szConnectID, MAX_BUFF_20, "%d", u2ConnectID);

    CReTTyHandler* pTTyClientHandler = m_objTTyClientHandlerList.Get_Hash_Box_Data(szConnectID);

    if (NULL == pTTyClientHandler)
    {
        OUR_DEBUG((LM_INFO, "[CReTTyClientManager::GetClientDevInfo](u2ConnectID=%d) is no exist.\n", u2ConnectID));
        return false;
    }

    ACE_TTY_IO::Serial_Params inTTyParams = pTTyClientHandler->GetParams();
    outParam.m_nBaudRate        = inTTyParams.baudrate;
    outParam.m_uDatabits        = inTTyParams.databits;
    outParam.m_uStopbits        = inTTyParams.stopbits;
    outParam.m_pParitymode      = inTTyParams.paritymode;
    outParam.m_blCTSenb         = inTTyParams.ctsenb;
    outParam.m_uRTSenb          = inTTyParams.rtsenb;
    outParam.m_blDSRenb         = inTTyParams.dsrenb;
    outParam.m_blDTRdisable     = inTTyParams.dtrdisable;
    outParam.m_nReadtimeoutmsec = inTTyParams.readtimeoutmsec;
    outParam.m_blXinenb         = inTTyParams.xinenb;
    outParam.m_blXoutenb        = inTTyParams.xoutenb;
    outParam.m_blModem          = inTTyParams.modem;
    outParam.m_blRcvenb         = inTTyParams.rcvenb;
    outParam.m_nXOnlim          = inTTyParams.xonlim;
    outParam.m_nXOfflim         = inTTyParams.xofflim;
    return true;
}

bool CReTTyClientManager::IsConnect(uint16 u2ConnectID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    //先查找这个设备是否已经注册
    char szConnectID[MAX_BUFF_20] = { '\0' };
    sprintf_safe(szConnectID, MAX_BUFF_20, "%d", u2ConnectID);

    CReTTyHandler* pTTyClientHandler = m_objTTyClientHandlerList.Get_Hash_Box_Data(szConnectID);

    if (NULL == pTTyClientHandler)
    {
        OUR_DEBUG((LM_INFO, "[ CReTTyClientManager::IsConnect](u2ConnectID=%d) is no exist.\n", u2ConnectID));
        return false;
    }

    return pTTyClientHandler->GetConnectState();
}

