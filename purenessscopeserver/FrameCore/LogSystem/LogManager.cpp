// LogManager.h
// Log�Ĺ����࣬����Log��־�Ķ������
// һ��ʼ�������Ϊһ��DLL����ʵ�֣����ǿ���һ�£����Ƿ��ڿ������ʵ�ֱȽϺá�
// add by freeeyes
// 2009-04-04

#include "LogManager.h"

void CLogBlockPool::Init(uint32 u4BlockSize, uint32 u4PoolCount)
{
    //��ʼ������־��
    if(nullptr != m_pLogBlockInfo)
    {
        Close();
    }

    m_pLogBlockInfo = new _LogBlockInfo[u4PoolCount];

    for(uint32 i = 0; i < u4PoolCount; i++)
    {
        m_pLogBlockInfo[i].m_pBlock = new char[u4BlockSize];
    }

    m_u4MaxBlockSize = u4BlockSize;
    m_u4PoolCount    = u4PoolCount;
    m_u4CurrIndex    = 0;
}

void CLogBlockPool::Close()
{
    OUR_DEBUG((LM_INFO, "[CLogBlockPool::Close]Begin.\n"));

    for(uint32 i = 0; i < m_u4PoolCount; i++)
    {
        SAFE_DELETE_ARRAY(m_pLogBlockInfo[i].m_pBlock);
    }

    SAFE_DELETE_ARRAY(m_pLogBlockInfo);

    OUR_DEBUG((LM_INFO, "[CLogBlockPool::Close]End.\n"));
}

_LogBlockInfo* CLogBlockPool::GetLogBlockInfo()
{
    if(nullptr == m_pLogBlockInfo)
    {
        return nullptr;
    }

    _LogBlockInfo* pLogBlockInfo = nullptr;

    if(m_u4CurrIndex  == m_u4PoolCount - 1)
    {
        m_u4CurrIndex = 0;
    }

    pLogBlockInfo = &m_pLogBlockInfo[m_u4CurrIndex];

    if(pLogBlockInfo->m_blIsUsed == false)
    {
        pLogBlockInfo->m_blIsUsed = true;
    }
    else
    {
        OUR_DEBUG((LM_ERROR,"[CLogBlockPool::GetLogBlockInfo]***CLogBlockPool is all used!***\n"));
        return nullptr;
    }

    m_u4CurrIndex++;

    return pLogBlockInfo;
}

void CLogBlockPool::ReturnBlockInfo(_LogBlockInfo* pLogBlockInfo) const
{
    pLogBlockInfo->clear();
    pLogBlockInfo->m_blIsUsed = false;
}

uint32 CLogBlockPool::GetBlockSize() const
{
    return m_u4MaxBlockSize;
}
//******************************************************************

CLogManager::CLogManager(void): m_cond(m_mutex)
{
}

int CLogManager::open()
{
    if(activate(THREAD_PARAM, m_nThreadCount) == -1)
    {
        m_blRun = false;
        OUR_DEBUG((LM_ERROR,"[CLogManager::open] activate is error[%d].", errno));
        return -1;
    }

    m_blRun = true;
    return 0;
}

int CLogManager::svc(void)
{
    OUR_DEBUG((LM_INFO,"[CLogManager::svc] svc run.\n"));

    while(true)
    {
        //��Ϣ����
        if (false == Dispose_Queue())
        {
            break;
        }
    }

    //������־����
    m_pServerLogger->Close();

    //������־���
    m_objLogBlockPool.Close();

    OUR_DEBUG((LM_ERROR, "[CLogManager::svc]Close OK.\n"));
    return 0;
}

int CLogManager::Close()
{
    if(m_blRun)
    {
        if (false == this->CloseMsgQueue())
        {
            OUR_DEBUG((LM_ERROR, "[CLogManager::Close] CloseMsgQueue is false.\n"));
        }
    }
    else
    {
        msg_queue()->deactivate();
        msg_queue()->flush();
    }

    return 0;
}

void CLogManager::Init(int nThreadCount, int nQueueMax, uint32 u4MailID)
{
    m_nThreadCount = nThreadCount;
    m_nQueueMax    = nQueueMax;

    if(u4MailID > 0)
    {
        m_blIsMail = true;
    }
}

int CLogManager::Start()
{
    if(m_blRun == true)
    {
        return 0;
    }
    else
    {
        SetReset(false);
        return open();
    }
}

int CLogManager::Stop()
{
    Close();
    return 0;
}

bool CLogManager::IsRun() const
{
    return m_blRun;
}

int CLogManager::PutLog(_LogBlockInfo* pLogBlockInfo)
{
    ACE_Message_Block* mb = pLogBlockInfo->GetQueueMessage();

    //����������¼���
    if(m_blIsNeedReset == true)
    {
        //������־��
        m_objLogBlockPool.ReturnBlockInfo(pLogBlockInfo);
        return 0;
    }

    if(mb)
    {
        int msgcount = (int)msg_queue()->message_count();

        if (msgcount >= m_nQueueMax)
        {
            OUR_DEBUG((LM_INFO,"[CLogManager::PutLog] CLogManager queue is full!\n"));
            //������־��
            m_objLogBlockPool.ReturnBlockInfo(pLogBlockInfo);
            return -1;
        }

        ACE_Time_Value xtime = ACE_OS::gettimeofday()+ACE_Time_Value(0, MAX_MSG_PUTTIMEOUT);

        if(this->putq(mb, &xtime) == -1)
        {
            OUR_DEBUG((LM_ERROR,"[CLogManager::PutLog] CLogManager putq error(%s)!\n", pLogBlockInfo->m_pBlock));
            //������־��
            m_objLogBlockPool.ReturnBlockInfo(pLogBlockInfo);
            return -1;
        }

        return 0;
    }

    OUR_DEBUG((LM_ERROR,"[CLogManager::PutLog] CLogManager new ACE_Message_Block error!\n"));
    return -1;
}

int CLogManager::RegisterLog(shared_ptr<IServerLogger> pServerLogger)
{
    m_pServerLogger = pServerLogger;

    //��ʼ����־��
    OUR_DEBUG((LM_ERROR,"[CLogManager::RegisterLog] GetBlockSize=%d, GetPoolCount=%d!\n", 
        m_pServerLogger->GetBlockSize(),
        m_pServerLogger->GetPoolCount()));
    m_objLogBlockPool.Init(m_pServerLogger->GetBlockSize(), m_pServerLogger->GetPoolCount());

    return 0;
}

int CLogManager::UnRegisterLog()
{
    if(m_pServerLogger != nullptr)
    {
        m_pServerLogger->Close();
    }

    return 0;
}

bool CLogManager::Dispose_Queue()
{
    ACE_Message_Block* mb = nullptr;
    ACE_OS::last_error(0);

    if (getq(mb, 0) == -1)
    {
        OUR_DEBUG((LM_ERROR, "[CLogManager::svc] get error errno = [%d].\n", ACE_OS::last_error()));
        m_blRun = false;
        return false;
    }
    else if (mb->msg_type() == ACE_Message_Block::MB_STOP)
    {
        m_mutex.acquire();
        mb->release();
        this->msg_queue()->deactivate();
        m_cond.signal();
        m_mutex.release();
        m_blRun = false;
    }
    else
    {
        _LogBlockInfo* pLogBlockInfo = *((_LogBlockInfo**)mb->base());

        if (!pLogBlockInfo)
        {
            OUR_DEBUG((LM_ERROR, "[CLogManager::svc] CLogManager mb log == nullptr!\n"));
            return true;
        }

        if (0 != ProcessLog(pLogBlockInfo))
        {
            OUR_DEBUG((LM_ERROR, "[CLogManager::svc] ProcessLog is false.\n"));
        }

        //������־��
        m_objLogBlockPool.ReturnBlockInfo(pLogBlockInfo);
    }

    return true;
}

int CLogManager::ProcessLog(_LogBlockInfo* pLogBlockInfo)
{
    if(nullptr == m_pServerLogger)
    {
        return -1;
    }

    m_pServerLogger->DoLog((int)pLogBlockInfo->m_u4LogID, pLogBlockInfo);
    return 0;
}

//*****************************************************************************
int CLogManager::WriteLogBinary(int nLogType, const char* pData, int nLen)
{
    int nRet = 0;

    if ((uint32)(nLen * 5) >= m_objLogBlockPool.GetBlockSize())
    {
        OUR_DEBUG((LM_INFO, "[CLogManager::WriteLogBinary]nLen(%d) is more than GetBlockSize.\n"));
        return 1;
    }

    //����־��������ҵ�һ��������־��
    m_Logger_Mutex.acquire();
    _LogBlockInfo* pLogBlockInfo = m_objLogBlockPool.GetLogBlockInfo();

    if (nullptr != pLogBlockInfo)
    {
        //����������
        char szLog[10] = { '\0' };

        for (int i = 0; i < nLen; i++)
        {
            sprintf_safe(szLog, 10, "0x%02X ", (unsigned char)pData[i]);
            sprintf_safe(pLogBlockInfo->m_pBlock + 5 * i, m_objLogBlockPool.GetBlockSize() - 5 * i, "%s", szLog);
        }

        pLogBlockInfo->m_u4Length = (uint32)(nLen * 5);

        nRet = Update_Log_Block(nLogType, nullptr, nullptr, pLogBlockInfo);
    }

    m_Logger_Mutex.release();
    return nRet;
}

int CLogManager::WriteLog_r(int nLogType, const char* fmt, uint32 u4Len)
{
    int nRet = 0;

    if (u4Len >= m_objLogBlockPool.GetBlockSize())
    {
        OUR_DEBUG((LM_ERROR, "[CLogManager::WriteLog_r]Log length is more than max BlockSize.\n"));
        return 1;
    }

    m_Logger_Mutex.acquire();
    _LogBlockInfo* pLogBlockInfo = m_objLogBlockPool.GetLogBlockInfo();

    if (nullptr != pLogBlockInfo)
    {
        ACE_OS::snprintf(pLogBlockInfo->m_pBlock, m_objLogBlockPool.GetBlockSize() - 1, "%s", fmt);
        nRet = Update_Log_Block(nLogType, nullptr, nullptr, pLogBlockInfo);
    }

    m_Logger_Mutex.release();
    return nRet;
}

int CLogManager::WriteToMail_r(int nLogType, uint16 u2MailID, const char* pTitle, const char* fmt, uint32 u4Len)
{
    int nRet = 0;

    if (u4Len >= m_objLogBlockPool.GetBlockSize())
    {
        OUR_DEBUG((LM_ERROR, "[CLogManager::WriteLog_r]Log length is more than max BlockSize.\n"));
        return 1;
    }

    m_Logger_Mutex.acquire();
    _LogBlockInfo* pLogBlockInfo = m_objLogBlockPool.GetLogBlockInfo();

    if (nullptr != pLogBlockInfo)
    {
        ACE_OS::snprintf(pLogBlockInfo->m_pBlock, m_objLogBlockPool.GetBlockSize() - 1, "%s", fmt);
        nRet = Update_Log_Block(nLogType, &u2MailID, pTitle, pLogBlockInfo);
    }

    m_Logger_Mutex.release();
    return nRet;
}

//*****************************************************************************


void CLogManager::SetReset(bool blReset)
{
    m_blIsNeedReset = blReset;
}

void CLogManager::ResetLogData(uint16 u2LogLevel)
{
    //����������־�ȼ���������־�ļ�
    SetReset(true);

    //����ȴ�һ��ʱ�䣬�ȴ�������־ȫ��д����ɣ���������־ģ�顣
    //�������ټ�һ����
    ACE_Time_Value tvSleep(0, 1000);
    ACE_OS::sleep(tvSleep);

    m_pServerLogger->ReSet(u2LogLevel);
    SetReset(false);
}

uint32 CLogManager::GetLogCount()
{
    if(m_pServerLogger != nullptr)
    {
        return m_pServerLogger->GetLogTypeCount();
    }
    else
    {
        return (uint32)0;
    }
}

uint32 CLogManager::GetCurrLevel()
{
    if(m_pServerLogger != nullptr)
    {
        return m_pServerLogger->GetCurrLevel();
    }
    else
    {
        return (uint32)0;
    }
}

uint16 CLogManager::GetLogID(uint16 u2Index)
{
    if(m_pServerLogger != nullptr)
    {
        return m_pServerLogger->GetLogID(u2Index);
    }
    else
    {
        return (uint16)0;
    }
}

const char* CLogManager::GetLogInfoByServerName(uint16 u2LogID)
{
    if(m_pServerLogger != nullptr)
    {
        return m_pServerLogger->GetLogInfoByServerName(u2LogID);
    }
    else
    {
        return nullptr;
    }
}

const char* CLogManager::GetLogInfoByLogName(uint16 u2LogID)
{
    if(m_pServerLogger != nullptr)
    {
        return m_pServerLogger->GetLogInfoByLogName(u2LogID);
    }
    else
    {
        return nullptr;
    }
}

int CLogManager::GetLogInfoByLogDisplay(uint16 u2LogID)
{
    if(m_pServerLogger != nullptr)
    {
        return m_pServerLogger->GetLogInfoByLogDisplay(u2LogID);
    }
    else
    {
        return 0;
    }
}

uint16 CLogManager::GetLogInfoByLogLevel(uint16 u2LogID)
{
    if(m_pServerLogger != nullptr)
    {
        return m_pServerLogger->GetLogInfoByLogLevel(u2LogID);
    }
    else
    {
        return 0;
    }
}

int CLogManager::CloseMsgQueue()
{
    return Task_Common_CloseMsgQueue((ACE_Task<ACE_MT_SYNCH>*)this, m_cond, m_mutex);
}

int CLogManager::Update_Log_Block(int nLogType, const uint16* pMailID, const char* pTitle, _LogBlockInfo* pLogBlockInfo)
{
    //�鿴��ǰ��־�Ƿ���Ҫ���
    if (GetLogInfoByLogLevel((uint16)nLogType) < m_pServerLogger->GetCurrLevel())
    {
        //���ڵ�ǰ��־�ȼ���ȫ������
        return 0;
    }

    pLogBlockInfo->m_u4Length = (uint32)strlen(pLogBlockInfo->m_pBlock);
    pLogBlockInfo->m_u4LogID = (uint32)nLogType;

    if (nullptr != pMailID && nullptr != pTitle)
    {
        if (m_blIsMail == false)
        {
            pLogBlockInfo->m_u2MailID = 0;
        }
        else
        {
            pLogBlockInfo->m_u2MailID = *pMailID;
        }

        ACE_OS::snprintf(pLogBlockInfo->m_szMailTitle, MAX_BUFF_200, "%s", pTitle);
    }

    if (IsRun())
    {
        if (0 != PutLog(pLogBlockInfo))
        {
            OUR_DEBUG((LM_INFO, "[CLogManager::WriteToMail]PutLog error.\n"));
        }
    }
    else
    {
        m_objLogBlockPool.ReturnBlockInfo(pLogBlockInfo);
    }

    return 0;
}

