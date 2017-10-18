// LogManager.h
// Log的管理类，负责Log日志的对象管理。
// 一开始想把它作为一个DLL单独实现，但是考虑一下，还是放在框架里面实现比较好。
// add by freeeyes
// 2009-04-04

#include "LogManager.h"

Mutex_Allocator _log_service_mb_allocator;

CLogBlockPool::CLogBlockPool()
{
    //初始化，日志池
    m_pLogBlockInfo = NULL;

    m_u4MaxBlockSize = 0;
    m_u4PoolCount    = 0;
    m_u4CurrIndex    = 0;
}

CLogBlockPool::~CLogBlockPool()
{
    Close();
}

void CLogBlockPool::Init(uint32 u4BlockSize, uint32 u4PoolCount)
{
    //初始化，日志池
    if(NULL != m_pLogBlockInfo)
    {
        Close();
    }

    m_pLogBlockInfo = new _LogBlockInfo[u4PoolCount];

    if(NULL == m_pLogBlockInfo)
    {
        OUR_DEBUG((LM_INFO, "[CLogBlockPool::CLogBlockPool]CLogBlockPool new (%d) fail.\n", u4PoolCount));
    }

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
    for(uint32 i = 0; i < m_u4PoolCount; i++)
    {
        SAFE_DELETE_ARRAY(m_pLogBlockInfo[i].m_pBlock);
    }

    SAFE_DELETE_ARRAY(m_pLogBlockInfo);
}

_LogBlockInfo* CLogBlockPool::GetLogBlockInfo()
{
    if(NULL == m_pLogBlockInfo)
    {
        return NULL;
    }

    _LogBlockInfo* pLogBlockInfo = NULL;

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
        return NULL;
    }

    m_u4CurrIndex++;

    //memset(pLogBlockInfo->m_pBlock, 0, m_u4MaxBlockSize);
    return pLogBlockInfo;
}

void CLogBlockPool::ReturnBlockInfo(_LogBlockInfo* pLogBlockInfo)
{
    //memset(pLogBlockInfo->m_pBlock, 0, m_u4MaxBlockSize);
    pLogBlockInfo->clear();
    pLogBlockInfo->m_blIsUsed = false;
}

uint32 CLogBlockPool::GetBlockSize()
{
    return m_u4MaxBlockSize;
}
//******************************************************************

CLogManager::CLogManager(void):m_mutex(), m_cond(m_mutex)
{
    m_blIsNeedReset = false;
    m_blRun         = false;
    m_nThreadCount  = 1;
    m_nQueueMax     = MAX_MSG_THREADQUEUE;
    m_pServerLogger = NULL;
    m_blIsMail      = false;
}

CLogManager::~CLogManager(void)
{
    OUR_DEBUG((LM_ERROR,"[CLogManager::~CLogManager].\n"));
    SAFE_DELETE(m_pServerLogger);
    OUR_DEBUG((LM_ERROR,"[CLogManager::~CLogManager]End.\n"));
}

int CLogManager::open(void* args)
{
    if(args != NULL)
    {
        OUR_DEBUG((LM_ERROR,"[CLogManager::open]args is not NULL.\n"));
    }

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

    //ACE_Time_Value     xtime;
    while(true)
    {
        ACE_Message_Block* mb = NULL;
        ACE_OS::last_error(0);

        //xtime=ACE_OS::gettimeofday()+ACE_Time_Value(0, MAX_MSG_PUTTIMEOUT);
        if(getq(mb, 0) == -1)
        {
            OUR_DEBUG((LM_ERROR,"[CLogManager::svc] get error errno = [%d].\n", ACE_OS::last_error()));
            m_blRun = false;
            break;
        }
        else
        {
            if (mb == NULL)
            {
                continue;
            }

            if ((0 == mb->size ()) && (mb->msg_type () == ACE_Message_Block::MB_STOP))
            {
                m_mutex.acquire();
                mb->release ();
                this->msg_queue ()->deactivate ();
                m_cond.signal();
                m_mutex.release();
                break;
            }

            _LogBlockInfo* pLogBlockInfo = *((_LogBlockInfo**)mb->base());

            if (!pLogBlockInfo)
            {
                OUR_DEBUG((LM_ERROR,"[CLogManager::svc] CLogManager mb log == NULL!\n"));
                continue;
            }

            if (0 != ProcessLog(pLogBlockInfo))
            {
                OUR_DEBUG((LM_ERROR, "[CLogManager::svc] ProcessLog is false.\n"));
            }

            //OUR_DEBUG((LM_ERROR,"[CLogManager::svc] delete pstrLogText BEGIN!\n"));
            //回收日志块
            m_objLogBlockPool.ReturnBlockInfo(pLogBlockInfo);
            //OUR_DEBUG((LM_ERROR,"[CLogManager::svc] delete pstrLogText END!\n"));
        }
    }

    //OUR_DEBUG((LM_INFO,"[CLogManager::svc] CLogManager::svc finish!\n"));
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
        m_blRun = false;
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
    m_blRun = false;
    return 0;
}

bool CLogManager::IsRun()
{
    return m_blRun;
}

int CLogManager::PutLog(_LogBlockInfo* pLogBlockInfo)
{
    ACE_Message_Block* mb = pLogBlockInfo->GetQueueMessage();

    //如果正在重新加载
    if(m_blIsNeedReset == true)
    {
        //回收日志块
        m_objLogBlockPool.ReturnBlockInfo(pLogBlockInfo);
        return 0;
    }

    if(mb)
    {
        int msgcount = (int)msg_queue()->message_count();

        if (msgcount >= m_nQueueMax)
        {
            OUR_DEBUG((LM_INFO,"[CLogManager::PutLog] CLogManager queue is full!\n"));
            //回收日志块
            m_objLogBlockPool.ReturnBlockInfo(pLogBlockInfo);
            return -1;
        }

        ACE_Time_Value xtime = ACE_OS::gettimeofday()+ACE_Time_Value(0, MAX_MSG_PUTTIMEOUT);

        if(this->putq(mb, &xtime) == -1)
        {
            OUR_DEBUG((LM_ERROR,"[CLogManager::PutLog] CLogManager putq error(%s)!\n", pLogBlockInfo->m_pBlock));
            //回收日志块
            m_objLogBlockPool.ReturnBlockInfo(pLogBlockInfo);
            return -1;
        }

        return 0;
    }

    OUR_DEBUG((LM_ERROR,"[CLogManager::PutLog] CLogManager new ACE_Message_Block error!\n"));
    return -1;
}

int CLogManager::RegisterLog(CServerLogger* pServerLogger)
{
    if(pServerLogger == NULL)
    {
        return -1;
    }

    //填入日志类型对象
    if(m_pServerLogger != NULL)
    {
        SAFE_DELETE(m_pServerLogger);
    }

    m_pServerLogger = pServerLogger;

    //初始化日志池
    OUR_DEBUG((LM_ERROR,"[CLogManager::RegisterLog] GetBlockSize=%d, GetPoolCount=%d!\n", pServerLogger->GetBlockSize(), pServerLogger->GetPoolCount()));
    m_objLogBlockPool.Init(pServerLogger->GetBlockSize(), pServerLogger->GetPoolCount());

    return 0;
}

int CLogManager::UnRegisterLog()
{
    if(m_pServerLogger != NULL)
    {
        SAFE_DELETE(m_pServerLogger);
    }

    return 0;
}

int CLogManager::ProcessLog(_LogBlockInfo* pLogBlockInfo)
{
    if(NULL == m_pServerLogger)
    {
        return -1;
    }

    //m_Logger_Mutex.acquire();
    m_pServerLogger->DoLog((int)pLogBlockInfo->m_u4LogID, pLogBlockInfo);
    //m_Logger_Mutex.release();
    return 0;
}

//*****************************************************************************

int CLogManager::WriteLog(int nLogType, const char* fmt, ...)
{
    //从日志块池里面找到一块空余的日志块
    //查看当前日志是否需要入库
    if(GetLogInfoByLogLevel(nLogType) < m_pServerLogger->GetCurrLevel())
    {
        //低于当前日志等级的全部忽略
        return 0;
    }

    m_Logger_Mutex.acquire();
    _LogBlockInfo* pLogBlockInfo = m_objLogBlockPool.GetLogBlockInfo();

    if(NULL == pLogBlockInfo)
    {
        OUR_DEBUG((LM_ERROR,"[CLogManager::WriteLog] m_objLogBlockPool is full!\n"));
        m_Logger_Mutex.release();
        return -1;
    }

    va_list ap;
    va_start(ap, fmt);
    ACE_OS::vsnprintf(pLogBlockInfo->m_pBlock, m_objLogBlockPool.GetBlockSize() - 1, fmt, ap);
    va_end(ap);

    pLogBlockInfo->m_u4Length = (uint32)strlen(pLogBlockInfo->m_pBlock);
    pLogBlockInfo->m_u4LogID  = (uint32)nLogType;

    if (IsRun())
    {
        if (0 != PutLog(pLogBlockInfo))
        {
            OUR_DEBUG((LM_INFO, "[CLogManager::WriteLog]PutLog error.\n"));
        }
    }
    else
    {
        m_objLogBlockPool.ReturnBlockInfo(pLogBlockInfo);
    }

    m_Logger_Mutex.release();
    return 0;
}

int CLogManager::WriteLogBinary(int nLogType, const char* pData, int nLen)
{
    int nRet = 0;

    //查看当前日志是否需要入库
    if(GetLogInfoByLogLevel(nLogType) < m_pServerLogger->GetCurrLevel())
    {
        //低于当前日志等级的全部忽略
        return 0;
    }

    //从日志块池里面找到一块空余的日志块
    m_Logger_Mutex.acquire();
    _LogBlockInfo* pLogBlockInfo = m_objLogBlockPool.GetLogBlockInfo();

    if(NULL == pLogBlockInfo)
    {
        OUR_DEBUG((LM_ERROR,"[ILogManager::WriteLogBinary] m_objLogBlockPool is full!\n"));
        m_Logger_Mutex.release();
        return -1;
    }

    //把二进制转换成明文存储
    if((uint32)(nLen * 5) >= m_objLogBlockPool.GetBlockSize())
    {
        OUR_DEBUG((LM_ERROR,"[ILogManager::WriteLogBinary] write length is more than BlockSize!\n"));
        m_Logger_Mutex.release();
        return -1;
    }

    pLogBlockInfo->m_u4LogID = nLogType;
    char szLog[10]  = {'\0'};

    for(int i = 0; i < nLen; i++)
    {
        sprintf_safe(szLog, 10, "0x%02X ", (unsigned char)pData[i]);
        sprintf_safe(pLogBlockInfo->m_pBlock + 5*i, m_objLogBlockPool.GetBlockSize() - 5*i, "%s", szLog);
    }

    pLogBlockInfo->m_u4Length = (uint32)(nLen * 5);

    if (IsRun())
    {
        nRet = PutLog(pLogBlockInfo);

        if (nRet)
        {
            m_objLogBlockPool.ReturnBlockInfo(pLogBlockInfo);
        }
    }
    else
    {
        m_objLogBlockPool.ReturnBlockInfo(pLogBlockInfo);
    }

    m_Logger_Mutex.release();
    return nRet;
}


int CLogManager::WriteToMail( int nLogType, uint32 u4MailID, char* pTitle, const char* fmt, ... )
{
    //查看当前日志是否需要入库
    if(GetLogInfoByLogLevel(nLogType) < m_pServerLogger->GetCurrLevel())
    {
        //低于当前日志等级的全部忽略
        return 0;
    }

    //从日志块池里面找到一块空余的日志块
    m_Logger_Mutex.acquire();
    _LogBlockInfo* pLogBlockInfo = m_objLogBlockPool.GetLogBlockInfo();


    if(NULL == pLogBlockInfo)
    {
        OUR_DEBUG((LM_ERROR,"[CLogManager::WriteLog] m_objLogBlockPool is full!\n"));
        m_Logger_Mutex.release();
        return -1;
    }

    va_list ap;
    va_start(ap, fmt);
    ACE_OS::vsnprintf(pLogBlockInfo->m_pBlock, m_objLogBlockPool.GetBlockSize() - 1, fmt, ap);
    va_end(ap);

    pLogBlockInfo->m_u4Length = (uint32)strlen(pLogBlockInfo->m_pBlock);
    pLogBlockInfo->m_u4LogID  = (uint32)nLogType;

    if(m_blIsMail == false)
    {
        pLogBlockInfo->m_u4MailID = 0;
    }
    else
    {
        pLogBlockInfo->m_u4MailID = u4MailID;
    }

    ACE_OS::sprintf(pLogBlockInfo->m_szMailTitle, "%s", pTitle);

    if (IsRun())
    {
        if (false == PutLog(pLogBlockInfo))
        {
            OUR_DEBUG((LM_INFO, "[CLogManager::WriteToMail]PutLog error.\n"));
        }
    }
    else
    {
        m_objLogBlockPool.ReturnBlockInfo(pLogBlockInfo);
    }

    m_Logger_Mutex.release();
    return 0;
}

//*****************************************************************************


void CLogManager::SetReset(bool blReset)
{
    m_blIsNeedReset = blReset;
}

void CLogManager::ResetLogData(uint16 u2LogLevel)
{
    //重新设置日志等级，加载日志文件
    SetReset(true);

    //这里等待一段时间，等待其他日志全部写入完成，在重载日志模块。
    //这样做少加一个锁
    ACE_Time_Value tvSleep(0, 1000);
    ACE_OS::sleep(tvSleep);

    m_pServerLogger->ReSet(u2LogLevel);
    SetReset(false);
}

uint32 CLogManager::GetLogCount()
{
    if(m_pServerLogger != NULL)
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
    if(m_pServerLogger != NULL)
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
    if(m_pServerLogger != NULL)
    {
        return m_pServerLogger->GetLogID(u2Index);
    }
    else
    {
        return (uint16)0;
    }
}

char* CLogManager::GetLogInfoByServerName(uint16 u2LogID)
{
    if(m_pServerLogger != NULL)
    {
        return m_pServerLogger->GetLogInfoByServerName(u2LogID);
    }
    else
    {
        return NULL;
    }
}

char* CLogManager::GetLogInfoByLogName(uint16 u2LogID)
{
    if(m_pServerLogger != NULL)
    {
        return m_pServerLogger->GetLogInfoByLogName(u2LogID);
    }
    else
    {
        return NULL;
    }
}

int CLogManager::GetLogInfoByLogDisplay(uint16 u2LogID)
{
    if(m_pServerLogger != NULL)
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
    if(m_pServerLogger != NULL)
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
    // We can choose to process the message or to differ it into the message
    // queue, and process them into the svc() method. Chose the last option.
    int retval;

    ACE_Message_Block* mblk = 0;
    ACE_NEW_RETURN(mblk,ACE_Message_Block (0, ACE_Message_Block::MB_STOP),-1);

    // If queue is full, flush it before block in while
    if (msg_queue ()->is_full())
    {
        if ((retval=msg_queue ()->flush()) == -1)
        {
            OUR_DEBUG((LM_ERROR, "[CLogManager::CloseMsgQueue]put error flushing queue\n"));
            return -1;
        }
    }

    m_mutex.acquire();

    while ((retval = putq (mblk)) == -1)
    {
        if (msg_queue ()->state () != ACE_Message_Queue_Base::PULSED)
        {
            OUR_DEBUG((LM_ERROR,ACE_TEXT("[CLogManager::CloseMsgQueue]put Queue not activated.\n")));
            break;
        }
    }

    m_cond.wait();
    m_mutex.release();

    return retval;
}


