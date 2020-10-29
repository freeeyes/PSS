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
    /*
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);
    ACE_Message_Block* pmb = nullptr;

    if(u4Size == 0)
    {
        //如果申请的空间为0,则直接返回空。
        OUR_DEBUG((LM_INFO, "[CMessageBlockManager::Create]Size=0.\n"));
        return nullptr;
    }

    //获得内存2的整数倍空间
    uint32 u4FormatSize = next_pow_of_2(u4Size);

    pmb = m_MemoryBlock_Pool.Get(u4FormatSize);

    if(nullptr == pmb)
    {
        ACE_OS::last_error(0);
        pmb = new ACE_Message_Block(u4FormatSize);

        if(0 != ACE_OS::last_error())
        {
            OUR_DEBUG((LM_ERROR,"[CMessageBlockManager::Create]New ACE_Message_Block error errno = [%d].\n", ACE_OS::last_error()));
        }
    }

    m_u4UsedSize += u4FormatSize;
    m_MemoryBlock_Pool.Add_Used(pmb);
    return pmb;
    */

    return new ACE_Message_Block(u4Size);
}

bool CMessageBlockManager::Close(ACE_Message_Block* pMessageBlock, uint32 u4BlockLen)
{
    /*
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    if (nullptr == pMessageBlock)
    {
        return false;
    }

    if (false == m_MemoryBlock_Pool.Check_Used(pMessageBlock))
    {
        return false;
    }

    pMessageBlock->msg_type(ACE_Message_Block::MB_DATA);
    pMessageBlock->reset();
    m_MemoryBlock_Pool.Set(pMessageBlock);

    uint32 u4Size = (uint32)pMessageBlock->size();

    if (u4Size <= m_u4UsedSize)
    {
        m_u4UsedSize -= u4Size;
    }
    else
    {
        OUR_DEBUG((LM_ERROR, "[CMessageBlockManager::Close]m_u4UsedSize = [%d],u4FormatSize=[%d] realse Error!\n", m_u4UsedSize, u4Size));
    }
    */

    pMessageBlock->release();

    return true;
}

uint32 CMessageBlockManager::GetUsedSize() const
{
    return m_u4UsedSize;
}
