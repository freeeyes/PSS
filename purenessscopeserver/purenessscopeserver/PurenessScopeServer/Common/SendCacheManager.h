#ifndef _SENDCACHEMANAGER_H
#define _SENDCACHEMANAGER_H

#include "ISendCacheManager.h"
#include "MessageBlockManager.h"

#include <map>
using namespace std;

//负责记录发送缓冲池，用于反复利用内存。
//add by freeeyes

//默认缓冲池是10个
#define MAX_CACHE_POOL_SIZE 10

class CSendCacheManager : public ISendCacheManager
{
public:
	CSendCacheManager();
	~CSendCacheManager();

	void Init(uint32 u4CacheCount, uint32 u4CacheSize);
	void Close();

	uint32 GetFreeCount();
	uint32 GetUsedCount();

	//得到指定的一个连接的缓冲
	ACE_Message_Block* GetCacheData(uint32 u4ConnectID);
	//释放指定一个连接的缓冲
	void FreeCacheData(uint32 u4ConnectID);

private:
	typedef map<uint32, ACE_Message_Block*> mapUsedCache;
	typedef vector<ACE_Message_Block*> vecFreeCache;

	mapUsedCache               m_mapUsedCache;
	vecFreeCache               m_vecFreeCache;
	uint32                     m_u4CacheSize;
};

#endif
