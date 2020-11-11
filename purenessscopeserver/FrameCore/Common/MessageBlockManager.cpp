#include "MessageBlockManager.h"

CMessageBlockManager::CMessageBlockManager(void)
{
    //不在使用内存对象池技术，在线程锁频繁的情况下影响数据处理速度
}

void CMessageBlockManager::Init() const 
{
}

void CMessageBlockManager::Close() const
{
}

ACE_Message_Block* CMessageBlockManager::Create(uint32 u4Size)
{
    return new ACE_Message_Block(u4Size);
}

bool CMessageBlockManager::Close(ACE_Message_Block* pMessageBlock, uint32 u4BlockLen)
{
    ACE_UNUSED_ARG(u4BlockLen);

    pMessageBlock->release();

    return true;
}

uint32 CMessageBlockManager::GetUsedSize() const
{
    return 0;
}
