#include "SendCacheManager.h"

CSendCacheManager::CSendCacheManager()
{
	m_u4UsedCount = 0;
}

CSendCacheManager::~CSendCacheManager()
{
	OUR_DEBUG((LM_INFO, "[CSendCacheManager::~CSendCacheManager]Begin.\n"));
	Close();
	OUR_DEBUG((LM_INFO, "[CSendCacheManager::~CSendCacheManager]End.\n"));
}

void CSendCacheManager::Init(uint32 u4CacheCount, uint32 u4CacheSize)
{
	//初始化HashTable
	int nKeySize = 10;
	size_t nArraySize = (sizeof(_Hash_Table_Cell<ACE_Message_Block>) + nKeySize + sizeof(ACE_Message_Block* )) * u4CacheCount;
	char* pHashBase = new char[nArraySize];
	m_objCacheHashList.Set_Base_Addr(pHashBase, (int)u4CacheCount);
	m_objCacheHashList.Set_Base_Key_Addr(pHashBase + sizeof(_Hash_Table_Cell<ACE_Message_Block>) * u4CacheCount, 
																	nKeySize * u4CacheCount, nKeySize);
	m_objCacheHashList.Set_Base_Value_Addr(pHashBase + (sizeof(_Hash_Table_Cell<ACE_Message_Block>) + nKeySize) * u4CacheCount, 
																	sizeof(ACE_Message_Block* ) * u4CacheCount, sizeof(ACE_Message_Block* ));

	for(uint32 i = 0; i < u4CacheCount; i++)
	{
		ACE_Message_Block* pCache = App_MessageBlockManager::instance()->Create(u4CacheSize);
		//OUR_DEBUG((LM_INFO, "[CSendCacheManager::Init](%d)pMessage=0x%08x.\n", i, pCache));
		char szHashID[10] = {'\0'};
		sprintf_safe(szHashID, 10, "%d", i);
		m_objCacheHashList.Add_Hash_Data(szHashID, pCache);
	}
}

void CSendCacheManager::Close()
{
	//回收内存
	//清理所有已存在的指针
	for(int i = 0; i < m_objCacheHashList.Get_Count(); i++)
	{
		ACE_Message_Block* pCache = m_objCacheHashList.Get_Index(i);
		App_MessageBlockManager::instance()->Close(pCache);
	}
	m_objCacheHashList.Close();
	m_u4UsedCount = 0;
	
	OUR_DEBUG((LM_INFO, "[CSendCacheManager::Close]OK.\n"));
}

uint32 CSendCacheManager::GetFreeCount()
{
	return (uint32)m_objCacheHashList.Get_Count() - m_u4UsedCount;
}

uint32 CSendCacheManager::GetUsedCount()
{
	return m_u4UsedCount;
}

ACE_Message_Block* CSendCacheManager::GetCacheData(uint32 u4ConnectID)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	ACE_Message_Block* pCache = NULL;
	
	//这里特殊处理下，因为是缓冲池，所以直接从这里查找出来即可，不用删除
	char szHashID[10];
	sprintf_safe(szHashID, 10, "%d", u4ConnectID % m_objCacheHashList.Get_Count());
	pCache = m_objCacheHashList.Get_Hash_Box_Data(szHashID);
	if(NULL != pCache)
	{
		m_u4UsedCount++;
	}
	else
	{
		OUR_DEBUG((LM_INFO, "[CSendCacheManager::GetCacheData](%s) is NULL.\n", szHashID));
	}
	return pCache;
}

void CSendCacheManager::FreeCacheData(uint32 u4ConnectID)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	if(u4ConnectID > 100000000)
	{
		OUR_DEBUG((LM_INFO, "[CSendCacheManager::FreeCacheData](%s) is delete.\n", u4ConnectID));
	}
	m_u4UsedCount--;
}
