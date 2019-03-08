#ifndef _ISENDCACHEMANAGER_H
#define _ISENDCACHEMANAGER_H

#include "define.h"

class ISendCacheManager
{
public:
    virtual ~ISendCacheManager() {}

    //�õ����п������
    virtual uint32 GetFreeCount() = 0;
    //�õ�����ʹ�ÿ������
    virtual uint32 GetUsedCount() = 0;

    //�õ�ָ����һ�����ӵĻ���
    virtual ACE_Message_Block* GetCacheData(uint32 u4ConnectID) = 0;
    //�ͷ�ָ��һ�����ӵĻ���
    virtual void FreeCacheData(uint32 u4ConnectID) = 0;
};

#endif
