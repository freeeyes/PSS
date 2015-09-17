#include "SendCacheManager.h"

CSendCacheManager::CSendCacheManager()
{
	m_u4CacheSize = 0;
}

CSendCacheManager::~CSendCacheManager()
{
	OUR_DEBUG((LM_INFO, "[CSendCacheManager::~CSendCacheManager]Begin.\n"));
	Close();
	OUR_DEBUG((LM_INFO, "[CSendCacheManager::~CSendCacheManager]End.\n"));
}

void CSendCacheManager::Init(uint32 u4CacheCount, uint32 u4CacheSize)
{
	for(uint32 i = 0; i < u4CacheCount; i++)
	{
		ACE_Message_Block* pCache = new ACE_Message_Block(u4CacheSize);	
		//OUR_DEBUG((LM_INFO, "[CSendCacheManager::Init](%d)pMessage=0x%08x.\n", i, pCache));
		if(NULL != pCache)
		{
			m_vecFreeCache.push_back(pCache);
		}
	}
	m_u4CacheSize = u4CacheSize;
}

void CSendCacheManager::Close()
{
	//回收内存
	OUR_DEBUG((LM_INFO, "[CSendCacheManager::Close]m_mapUsedCache=%d,Unused=%d.\n", m_mapUsedCache.size(), m_vecFreeCache.size()));
	for(mapUsedCache::iterator b = m_mapUsedCache.begin(); b != m_mapUsedCache.end(); b++)
	{
		ACE_Message_Block* pCache = (ACE_Message_Block* )b->second;
		pCache->release();
	}
	m_mapUsedCache.clear();

	uint32 u4CacheCount = m_vecFreeCache.size();
	for(uint32 i = 0; i < u4CacheCount; i++)
	{
		ACE_Message_Block* pMessage = (ACE_Message_Block* )m_vecFreeCache[i];
		pMessage->release();
	}
	m_vecFreeCache.clear();
	
	OUR_DEBUG((LM_INFO, "[CSendCacheManager::Close]OK.\n"));
}

uint32 CSendCacheManager::GetFreeCount()
{
	return (uint32)m_vecFreeCache.size();
}

uint32 CSendCacheManager::GetUsedCount()
{
	return (uint32)m_mapUsedCache.size();
}

ACE_Message_Block* CSendCacheManager::GetCacheData(uint32 u4ConnectID)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	ACE_Message_Block* pCache = NULL;
	
	//先从已有缓冲中去找
	mapUsedCache::iterator f = m_mapUsedCache.find(u4ConnectID);
	if(f != m_mapUsedCache.end())
	{
		return (ACE_Message_Block* )f->second;
	}

	//如果没有找到，就从缓冲中去找
	if(m_vecFreeCache.size() == 0)
	{
		pCache = App_MessageBlockManager::instance()->Create(m_u4CacheSize);	
		//OUR_DEBUG((LM_INFO, "[CSendCacheManager::GetCacheData]***(%d)New pCache=0x%08x.\n", u4ConnectID, pCache));
		m_mapUsedCache.insert(mapUsedCache::value_type(u4ConnectID, pCache));
	}
	else
	{
		pCache = m_vecFreeCache[0];
		m_vecFreeCache.erase(m_vecFreeCache.begin());
		m_mapUsedCache.insert(mapUsedCache::value_type(u4ConnectID, pCache));
		//OUR_DEBUG((LM_INFO, "[CSendCacheManager::GetCacheData]***(%d)Pool pCache=0x%08x.\n", u4ConnectID, pCache));
	}
		
	return pCache;
}

void CSendCacheManager::FreeCacheData(uint32 u4ConnectID)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
	ACE_Message_Block* pCache = NULL;
	mapUsedCache::iterator f = m_mapUsedCache.find(u4ConnectID);
	if(f != m_mapUsedCache.end())
	{
		pCache = (ACE_Message_Block* )f->second;

		//OUR_DEBUG((LM_INFO, "[CSendCacheManager::GetCacheData]***(%d)free pCache=0x%08x.\n", u4ConnectID, pCache));

		//重置缓冲区
		if(NULL != pCache)
		{
			pCache->reset();
			m_vecFreeCache.push_back(pCache);
		}
		m_mapUsedCache.erase(f);
	}
}
