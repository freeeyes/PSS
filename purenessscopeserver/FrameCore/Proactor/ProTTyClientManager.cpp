#include "ProTTyClientManager.h"

CProTTyClientManager::CProTTyClientManager(): m_pProactor(nullptr), m_u2MaxListCount(MAX_BUFF_100), m_u2TimeCheck(120)
{

}

CProTTyClientManager::~CProTTyClientManager()
{

}

bool CProTTyClientManager::StartConnectTask()
{
    CancelConnectTask();
    
    m_blTimerState = true;

    timer_task(App_TimerManager::instance()->GetTimerPtr());

    return true;
}

void CProTTyClientManager::CancelConnectTask()
{
    m_blTimerState = false;
}

bool CProTTyClientManager::Init(ACE_Proactor* pProactor, uint16 u2MaxTTyCount, uint16 u2TimeCheck)
{
    m_pProactor      = pProactor;
    m_u2MaxListCount = u2MaxTTyCount;
    m_u2TimeCheck    = u2TimeCheck;

    //初始化Hash数组(TCP)
    m_objTTyClientHandlerList.Init((int)m_u2MaxListCount);

    //启动定时器
    StartConnectTask();

    return true;
}

void CProTTyClientManager::Close()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    //关闭定时器
    CancelConnectTask();

    //关闭所有已存在的链接
    vector<CProTTyHandler*> vecTTyClientHandlerInfo;
    m_objTTyClientHandlerList.Get_All_Used(vecTTyClientHandlerInfo);

    for (int i = 0; i < (int)vecTTyClientHandlerInfo.size(); i++)
    {
        CProTTyHandler* pTTyClientHandler = vecTTyClientHandlerInfo[i];

        if (nullptr != pTTyClientHandler)
        {
            pTTyClientHandler->Close(pTTyClientHandler->GetConnectID());
            SAFE_DELETE(pTTyClientHandler);
        }
    }

    m_objTTyClientHandlerList.Close();
}

bool CProTTyClientManager::Close(uint16 u2ConnectID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    //先查找这个设备是否已经注册
    char szConnectID[MAX_BUFF_20] = { '\0' };
    sprintf_safe(szConnectID, MAX_BUFF_20, "%d", u2ConnectID);

    CProTTyHandler* pTTyClientHandler = m_objTTyClientHandlerList.Get_Hash_Box_Data(szConnectID);

    if (nullptr == pTTyClientHandler)
    {
        PSS_LOGGER_DEBUG("[CProTTyClientManager::Close](u2ConnectID={0}) is no exist.", u2ConnectID);
        return false;
    }
    else
    {
        m_objTTyClientHandlerList.Del_Hash_Data(szConnectID);
        pTTyClientHandler->Close(pTTyClientHandler->GetConnectID());
        SAFE_DELETE(pTTyClientHandler);
        return true;
    }
}

bool CProTTyClientManager::Pause(uint16 u2ConnectID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    //先查找这个设备是否已经注册
    char szConnectID[MAX_BUFF_20] = { '\0' };
    sprintf_safe(szConnectID, MAX_BUFF_20, "%d", u2ConnectID);

    CProTTyHandler* pTTyClientHandler = m_objTTyClientHandlerList.Get_Hash_Box_Data(szConnectID);

    if (nullptr == pTTyClientHandler)
    {
        PSS_LOGGER_DEBUG("[CProTTyClientManager::Pause](u2ConnectID={0}) is no exist.", u2ConnectID);
        return false;
    }
    else
    {
        pTTyClientHandler->SetPause(true);
        return true;
    }
}

bool CProTTyClientManager::Resume(uint16 u2ConnectID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    //先查找这个设备是否已经注册
    char szConnectID[MAX_BUFF_20] = { '\0' };
    sprintf_safe(szConnectID, MAX_BUFF_20, "%d", u2ConnectID);

    CProTTyHandler* pTTyClientHandler = m_objTTyClientHandlerList.Get_Hash_Box_Data(szConnectID);

    if (nullptr == pTTyClientHandler)
    {
        PSS_LOGGER_DEBUG("[CProTTyClientManager::Resume](u2ConnectID={0}) is no exist.", u2ConnectID);
        return false;
    }
    else
    {
        pTTyClientHandler->SetPause(false);
        return true;
    }
}

bool CProTTyClientManager::SendMessage(uint16 u2ConnectID, char*& pMessage, uint32 u4Len)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    //先查找这个设备是否已经注册
    char szConnectID[MAX_BUFF_20] = { '\0' };
    sprintf_safe(szConnectID, MAX_BUFF_20, "%d", u2ConnectID);

    CProTTyHandler* pTTyClientHandler = m_objTTyClientHandlerList.Get_Hash_Box_Data(szConnectID);

    if (nullptr == pTTyClientHandler)
    {
        PSS_LOGGER_DEBUG("[CProTTyClientManager::SendMessage](u2ConnectID={0}) is no exist.", u2ConnectID);
        return false;
    }
    else
    {
        return pTTyClientHandler->Send_Data(pMessage, u4Len);
    }
}

int CProTTyClientManager::timer_task(brynet::TimerMgr::Ptr timerMgr)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    PSS_LOGGER_DEBUG("[CProTTyClientManager::handle_timeout]({0}) Run.", m_objTTyClientHandlerList.Get_Count());

    //关闭所有已存在的链接
    vector<CProTTyHandler*> vecTTyClientHandlerInfo;
    m_objTTyClientHandlerList.Get_All_Used(vecTTyClientHandlerInfo);

    for (int i = 0; i < (int)vecTTyClientHandlerInfo.size(); i++)
    {
        CProTTyHandler* pTTyClientHandler = vecTTyClientHandlerInfo[i];

        if (nullptr != pTTyClientHandler)
        {
            pTTyClientHandler->ConnectTTy();
        }
    }

    if (true == m_blTimerState)
    {
        start_new_task(timerMgr);
    }

    return 0;
}

void CProTTyClientManager::start_new_task(brynet::TimerMgr::Ptr timerMgr)
{
    PSS_LOGGER_DEBUG("[CProTTyClientManager::start_new_task]new timer is set({0}).", m_u2TimeCheck);
    auto timer = timerMgr->addTimer(std::chrono::seconds(m_u2TimeCheck), [this, timerMgr]() {
        timer_task(timerMgr);
        });
}

int CProTTyClientManager::Connect(uint16 u2ConnectID, const char* pName, _TTyDevParam& inParam, ITTyMessage* pMessageRecv)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    //先查找这个设备是否已经注册
    char szConnectID[MAX_BUFF_20] = { '\0' };
    sprintf_safe(szConnectID, MAX_BUFF_20, "%d", u2ConnectID);

    CProTTyHandler* pTTyClientHandler = m_objTTyClientHandlerList.Get_Hash_Box_Data(szConnectID);

    if (nullptr != pTTyClientHandler)
    {
        PSS_LOGGER_DEBUG("[CProTTyClientManager::Connect]({0}) is exist.", pName);
        return -1;
    }

    pTTyClientHandler = new CProTTyHandler();

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
    pTTyClientHandler->proactor(m_pProactor);

    if (false == pTTyClientHandler->Init(u2ConnectID, pName, inTTyParams, pMessageRecv))
    {
        PSS_LOGGER_DEBUG("[CProTTyClientManager::Connect]({0})pTTyClientHandler Init Error.", pName);
        SAFE_DELETE(pTTyClientHandler);
        return -1;
    }

    if (false == m_objTTyClientHandlerList.Add_Hash_Data(szConnectID, pTTyClientHandler))
    {
        PSS_LOGGER_DEBUG("[CProTTyClientManager::Connect]({0})Add_Hash_Data Error.", pName);
        SAFE_DELETE(pTTyClientHandler);
        return -1;
    }

    return 0;
}

int CProTTyClientManager::ConnectFrame(uint16 u2ConnectID, const char* pName, _TTyDevParam& inParam, uint32 u4PacketParseID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    //先查找这个设备是否已经注册
    char szConnectID[MAX_BUFF_20] = { '\0' };
    sprintf_safe(szConnectID, MAX_BUFF_20, "%d", u2ConnectID);

    CProTTyHandler* pTTyClientHandler = m_objTTyClientHandlerList.Get_Hash_Box_Data(szConnectID);

    if (nullptr != pTTyClientHandler)
    {
        PSS_LOGGER_DEBUG("[CProTTyClientManager::ConnectFrame]({0}) is exist.", pName);
        return -1;
    }

    pTTyClientHandler = new CProTTyHandler();

    //匹配相关参数
    ACE_TTY_IO::Serial_Params inTTyParams;
    inTTyParams.baudrate = inParam.m_nBaudRate;
    inTTyParams.databits = inParam.m_uDatabits;
    inTTyParams.stopbits = inParam.m_uStopbits;
    inTTyParams.paritymode = inParam.m_pParitymode;
    inTTyParams.ctsenb = inParam.m_blCTSenb;
    inTTyParams.rtsenb = inParam.m_uRTSenb;
    inTTyParams.dsrenb = inParam.m_blDSRenb;
    inTTyParams.dtrdisable = inParam.m_blDTRdisable;
    inTTyParams.readtimeoutmsec = inParam.m_nReadtimeoutmsec;
    inTTyParams.xinenb = inParam.m_blXinenb;
    inTTyParams.xoutenb = inParam.m_blXoutenb;
    inTTyParams.modem = inParam.m_blModem;
    inTTyParams.rcvenb = inParam.m_blRcvenb;
    inTTyParams.xonlim = inParam.m_nXOnlim;
    inTTyParams.xofflim = inParam.m_nXOfflim;
    inTTyParams.readmincharacters = inParam.m_u4Readmincharacters;

    //绑定反应器
    pTTyClientHandler->proactor(m_pProactor);

    if (false == pTTyClientHandler->Init(u2ConnectID, pName, inTTyParams, nullptr, EM_CONNECT_IO_DISPOSE::CONNECT_IO_FRAME, u4PacketParseID))
    {
        PSS_LOGGER_DEBUG("[CProTTyClientManager::ConnectFrame]({0})pTTyClientHandler Init Error.", pName);
        SAFE_DELETE(pTTyClientHandler);
        return -1;
    }

    if (false == m_objTTyClientHandlerList.Add_Hash_Data(szConnectID, pTTyClientHandler))
    {
        PSS_LOGGER_DEBUG("[CProTTyClientManager::ConnectFrame](%s)Add_Hash_Data Error.", pName);
        SAFE_DELETE(pTTyClientHandler);
        return -1;
    }

    return 0;
}

bool CProTTyClientManager::GetClientDevInfo(uint16 u2ConnectID, _TTyDevParam& outParam)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    //先查找这个设备是否已经注册
    char szConnectID[MAX_BUFF_20] = { '\0' };
    sprintf_safe(szConnectID, MAX_BUFF_20, "%d", u2ConnectID);

    CProTTyHandler* pTTyClientHandler = m_objTTyClientHandlerList.Get_Hash_Box_Data(szConnectID);

    if (nullptr == pTTyClientHandler)
    {
        PSS_LOGGER_DEBUG("[CProTTyClientManager::GetClientDevInfo](u2ConnectID={0}) is no exist.", u2ConnectID);
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

bool CProTTyClientManager::IsConnect(uint16 u2ConnectID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

    //先查找这个设备是否已经注册
    char szConnectID[MAX_BUFF_20] = { '\0' };
    sprintf_safe(szConnectID, MAX_BUFF_20, "%d", u2ConnectID);

    CProTTyHandler* pTTyClientHandler = m_objTTyClientHandlerList.Get_Hash_Box_Data(szConnectID);

    if (nullptr == pTTyClientHandler)
    {
        PSS_LOGGER_DEBUG("[ CProTTyClientManager::IsConnect](u2ConnectID={0}) is no exist.", u2ConnectID);
        return false;
    }

    return pTTyClientHandler->GetConnectState();
}

