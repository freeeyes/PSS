#ifndef _ISENDCACHEMANAGER_H
#define _ISENDCACHEMANAGER_H

#include "define.h"

class ISendCacheManager
{
public:
    virtual ~ISendCacheManager() {}

    //得到空闲块的数量
    virtual uint32 GetFreeCount() = 0;
    //得到正在使用块的数量
    virtual uint32 GetUsedCount() = 0;

    //得到指定的一个连接的缓冲
    virtual ACE_Message_Block* GetCacheData(uint32 u4ConnectID) = 0;
    //释放指定一个连接的缓冲
    virtual void FreeCacheData(uint32 u4ConnectID) = 0;
};

#endif
