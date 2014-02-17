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

	m_u4CurrIndex++;

	memset(pLogBlockInfo->m_pBlock, 0, m_u4MaxBlockSize);
	return pLogBlockInfo;
}

void CLogBlockPool::ReturnBlockInfo(_LogBlockInfo* pLogBlockInfo)
{
	memset(pLogBlockInfo->m_pBlock, 0, m_u4MaxBlockSize);
	pLogBlockInfo->m_blIsUsed = false;
}

uint32 CLogBlockPool::GetBlockSize()
{
	return m_u4MaxBlockSize;
}
//******************************************************************

CLogManager::CLogManager(void)
{
	m_blRun         = false;
	m_nThreadCount  = 1;
	m_nQueueMax     = MAX_MSG_THREADQUEUE;
}

CLogManager::~CLogManager(void)
{
	OUR_DEBUG((LM_ERROR,"[CLogManager::~CLogManager].\n"));
	m_mapServerLogger.Clear();
	OUR_DEBUG((LM_ERROR,"[CLogManager::~CLogManager]End.\n"));
}

int CLogManager::open(void *args)
{
	if(args != NULL)
	{
		OUR_DEBUG((LM_ERROR,"[CLogManager::open]args is not NULL.\n"));
	}
	
	if(activate(THR_NEW_LWP | THR_DETACHED, m_nThreadCount) == -1)
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
	ACE_Message_Block* mb = NULL;
	//ACE_Time_Value     xtime;
	while(m_blRun)
	{
		mb = NULL;
		//xtime=ACE_OS::gettimeofday()+ACE_Time_Value(0, MAX_MSG_PUTTIMEOUT);
		if(getq(mb, 0) == -1)
		{
			OUR_DEBUG((LM_ERROR,"[CLogManager::svc] get error errno = [%d].\n", errno));
			m_blRun = false;
			break;
		}

		if (mb == NULL)
		{
			continue;
		}

		_LogBlockInfo* pLogBlockInfo = *((_LogBlockInfo**)mb->base());
		if (!pLogBlockInfo)
		{
			OUR_DEBUG((LM_ERROR,"[CLogManager::svc] CLogManager mb log == NULL!\n"));
			mb->release();
			continue;
		}

		ProcessLog(mb->msg_type(), pLogBlockInfo);
		mb->release();
		//OUR_DEBUG((LM_ERROR,"[CLogManager::svc] delete pstrLogText BEGIN!\n"));
		//回收日志块
		m_objLogBlockPool.ReturnBlockInfo(pLogBlockInfo);
		//OUR_DEBUG((LM_ERROR,"[CLogManager::svc] delete pstrLogText END!\n"));

	}
	OUR_DEBUG((LM_INFO,"[CLogManager::svc] CLogManager::svc finish!\n"));
	return 0;
}

int CLogManager::Close()
{
	msg_queue()->deactivate();
	msg_queue()->flush();
	return 0;
}

void CLogManager::Init(int nThreadCount, int nQueueMax)
{
	m_nThreadCount = nThreadCount;
	m_nQueueMax    = nQueueMax;
}

int CLogManager::Start()
{
	if(m_blRun == true)
	{
		return 0;
	}
	else
	{
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

int CLogManager::PutLog(int nLogType, _LogBlockInfo* pLogBlockInfo)
{
	ACE_Message_Block* mb = NULL;

	ACE_NEW_MALLOC_NORETURN (mb,
		static_cast<ACE_Message_Block*>(_log_service_mb_allocator.malloc (sizeof (ACE_Message_Block))),
		ACE_Message_Block
		(	sizeof(ACE_TString *), // size
		nLogType,
		0,
		0,
		&_log_service_mb_allocator, // allocator_strategy
		0, // locking strategy
		ACE_DEFAULT_MESSAGE_BLOCK_PRIORITY, // priority
		ACE_Time_Value::zero,
		ACE_Time_Value::max_time,
		&_log_service_mb_allocator,
		&_log_service_mb_allocator
		)
		);		

	if(mb)
	{
		_LogBlockInfo** loadin = (_LogBlockInfo **)mb->base();
		*loadin = pLogBlockInfo;

		int msgcount = (int)msg_queue()->message_count();
		if (msgcount >= m_nQueueMax) 
		{
			OUR_DEBUG((LM_INFO,"[CLogManager::PutLog] CLogManager queue is full!\n"));
			mb->release();
			return 1;
		}
		ACE_Time_Value xtime;
		xtime = ACE_OS::gettimeofday();
		if(this->putq(mb, &xtime) == -1)
		{
			OUR_DEBUG((LM_ERROR,"[CLogManager::PutLog] CLogManager putq error(%s)!\n", pLogBlockInfo->m_pBlock));
			mb->release();
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

	int nLogTypeCount = pServerLogger->GetLogTypeCount();

	for(int i = 0; i < nLogTypeCount; i++)
	{
		int nLogType = pServerLogger->GetLogType(i);
		if(nLogType <= 0)
		{
			continue;
		}

		m_mapServerLogger.AddMapData(nLogType, pServerLogger);
	}

	//初始化日志池
	OUR_DEBUG((LM_ERROR,"[CLogManager::RegisterLog] GetBlockSize=%d, GetPoolCount=%d!\n", pServerLogger->GetBlockSize(), pServerLogger->GetPoolCount()));	
	m_objLogBlockPool.Init(pServerLogger->GetBlockSize(), pServerLogger->GetPoolCount());

	return 0;
}

int CLogManager::UnRegisterLog(CServerLogger* pServerLogger)
{
	CServerLogger* pTempServerLogger = NULL;
	vector<int>    vecDel;
	int            i = 0;

	int nLogTypeCount = m_mapServerLogger.GetSize();
	for(i = 0; i < nLogTypeCount; i++)
	{
		pTempServerLogger = m_mapServerLogger.GetMapData(i);
		if(pTempServerLogger == pServerLogger)
		{
			int nLogType = m_mapServerLogger.GetMapDataKey(i);
			vecDel.push_back(nLogType);
		}
	}

	for(i = 0; i < (int)vecDel.size(); i++)
	{
		m_mapServerLogger.DelMapData(vecDel[i]);
	}

	return 0;
}

int CLogManager::ProcessLog(int nLogType, _LogBlockInfo* pLogBlockInfo)
{
	CServerLogger* pServerLogger = NULL;

	pServerLogger = m_mapServerLogger.SearchMapData(nLogType);
	if(NULL == pServerLogger)
	{
		return -1;
	}

	//m_Logger_Mutex.acquire();
	pServerLogger->DoLog(nLogType, pLogBlockInfo);
	//m_Logger_Mutex.release();
	return 0;
}

//*****************************************************************************

int CLogManager::WriteLog(int nLogType, const char* fmt, ...)
{
	int  nRet = 0;
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

	if (IsRun()) 
	{
		nRet = PutLog(nLogType, pLogBlockInfo);
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
	return 0;
}


int CLogManager::WriteLogBinary(int nLogType, const char* pData, int nLen)
{
	int nRet = 0;
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

	char szLog[10]  = {'\0'};
	for(int i = 0; i < nLen; i++)
	{
		sprintf_safe(szLog, 10, "0x%02X ", (unsigned char)pData[i]);
		sprintf_safe(pLogBlockInfo->m_pBlock, m_objLogBlockPool.GetBlockSize(), "%s%s", pLogBlockInfo->m_pBlock, szLog);
	}

	pLogBlockInfo->m_u4Length = (uint32)(nLen * 5);

	if (IsRun()) 
	{
		nRet = PutLog(nLogType, pLogBlockInfo);
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
