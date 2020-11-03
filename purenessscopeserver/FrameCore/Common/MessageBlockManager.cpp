#include "MessageBlockManager.h"

CMessageBlockManager::CMessageBlockManager(void)
{
    Init();
}

void CMessageBlockManager::Init()
{
    //m_MemoryBlock_Pool.Init(GetXmlConfigAttribute(xmlServerType)->Debug);
}

void CMessageBlockManager::Close()
{
    //m_MemoryBlock_Pool.Close();
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
    return m_u4UsedSize;
}
