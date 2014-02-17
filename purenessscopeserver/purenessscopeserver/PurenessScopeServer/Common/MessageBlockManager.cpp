#include "MessageBlockManager.h"

CMessageBlockManager::CMessageBlockManager(void)
{
	m_pmsgallocator   = NULL;
	m_pbuff_allocator = NULL;
	m_pdata_allocator = NULL;

	Init();
}

CMessageBlockManager::~CMessageBlockManager(void)
{
}

void CMessageBlockManager::Init()
{
	m_pmsgallocator   = new Mutex_Allocator();
	m_pdata_allocator = new Mutex_Allocator();
	m_pbuff_allocator = new Mutex_Allocator();
}

void CMessageBlockManager::Close()
{
	delete m_pmsgallocator;
	delete m_pdata_allocator;
	delete m_pbuff_allocator;
}

ACE_Message_Block* CMessageBlockManager::Create(uint32 u4Size)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);
	ACE_Message_Block* pmb = NULL;

	if(u4Size <= 0)
	{
		//如果申请的空间为0,则直接返回空。
		return NULL;
	}

	ACE_NEW_MALLOC_NORETURN(pmb, 
		                    static_cast<ACE_Message_Block*>(m_pmsgallocator->malloc(sizeof(ACE_Message_Block))),
	 	                    ACE_Message_Block(u4Size, // size
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

	return pmb;
}

bool CMessageBlockManager::Close(ACE_Message_Block* pMessageBlock)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);
	if(pMessageBlock != NULL)
	{
		pMessageBlock->release();
	}
	return true;
}
