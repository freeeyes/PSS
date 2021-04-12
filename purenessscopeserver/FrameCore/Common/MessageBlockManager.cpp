#include "MessageBlockManager.h"

void CMessageBlockManager::Init() 
{
    m_u4UsedSize = 0;
}

void CMessageBlockManager::Close()
{
    m_u4UsedSize = 0;
}

ACE_Message_Block* CMessageBlockManager::Create(uint32 u4Size)
{
    return new ACE_Message_Block(u4Size);
}

bool CMessageBlockManager::Close(ACE_Message_Block* pMessageBlock, uint32 u4BlockLen)
{
    ACE_UNUSED_ARG(u4BlockLen);

    if (nullptr != pMessageBlock)
    {
        pMessageBlock->release();
    }

    return true;
}

uint32 CMessageBlockManager::GetUsedSize() const
{
    return 0;
}
