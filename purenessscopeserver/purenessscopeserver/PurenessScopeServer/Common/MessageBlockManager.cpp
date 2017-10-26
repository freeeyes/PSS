#include "MessageBlockManager.h"

CMessageBlockManager::CMessageBlockManager(void)
{
    m_pmsgallocator   = NULL;
    m_pbuff_allocator = NULL;
    m_pdata_allocator = NULL;
    m_u4UsedSize      = 0;

    Init();
}

CMessageBlockManager::~CMessageBlockManager(void)
{
    Close();
}

void CMessageBlockManager::Init()
{
    m_pmsgallocator   = new Mutex_MB_Allocator();
    m_pdata_allocator = new Mutex_MB_Allocator();
    m_pbuff_allocator = new Mutex_MB_Allocator();
}

void CMessageBlockManager::Close()
{
    m_MemoryBlock_Pool.Close();

    if(NULL != m_pmsgallocator)
    {
        delete m_pmsgallocator;
        m_pmsgallocator = NULL;
    }

    if(NULL != m_pdata_allocator)
    {
        delete m_pdata_allocator;
        m_pdata_allocator = NULL;
    }

    if(NULL != m_pbuff_allocator)
    {
        delete m_pbuff_allocator;
        m_pbuff_allocator = NULL;
    }
}

ACE_Message_Block* CMessageBlockManager::Create(uint32 u4Size)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);
    ACE_Message_Block* pmb = NULL;

    if(u4Size == 0)
    {
        //如果申请的空间为0,则直接返回空。
        return NULL;
    }

    //获得内存2的整数倍空间
    uint32 u4FormatSize = next_pow_of_2(u4Size);

    pmb = m_MemoryBlock_Pool.Get(u4FormatSize);

    if(NULL == pmb)
    {
        ACE_OS::last_error(0);
        ACE_NEW_MALLOC_NORETURN(pmb,
                                static_cast<ACE_Message_Block*>(m_pmsgallocator->malloc(sizeof(ACE_Message_Block))),
                                ACE_Message_Block(u4FormatSize, // size
                                        ACE_Message_Block::MB_DATA, // type
                                        0,
                                        0,
                                        m_pbuff_allocator, // allocator_strategy
                                        0, // locking strategy
                                        ACE_DEFAULT_MESSAGE_BLOCK_PRIORITY, // priority
                                        ACE_Time_Value::zero,
                                        ACE_Time_Value::max_time,
                                        m_pdata_allocator,
                                        m_pmsgallocator
                                                 ));

        if(0 != ACE_OS::last_error())
        {
            OUR_DEBUG((LM_ERROR,"[CMessageBlockManager::Create]New ACE_Message_Block error errno = [%d].\n", ACE_OS::last_error()));
        }
    }


    m_u4UsedSize += u4FormatSize;
    return pmb;
}

bool CMessageBlockManager::Close(ACE_Message_Block* pMessageBlock)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);
    //ACE_OS::memset(pMessageBlock->base(), 0, pMessageBlock->capacity());
    pMessageBlock->msg_type(ACE_Message_Block::MB_DATA);
    pMessageBlock->reset();
    m_MemoryBlock_Pool.Set(pMessageBlock);

    uint32 u4Size = (uint32)pMessageBlock->size();
    m_u4UsedSize -= u4Size;
    return true;
}

uint32 CMessageBlockManager::GetUsedSize()
{
    return m_u4UsedSize;
}
