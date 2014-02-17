#ifndef MESSAGEBLOCKMANAGER_H
#define MESSAGEBLOCKMANAGER_H

//这里负责管理所有创建的mb的数据，并使用内存池。
//最近深受freebird的想法，重写这部分代码，虽然工作很忙，但是这部分必须完成！
//add by freeeyes
//2010-06-12

#include "ace/Thread_Mutex.h"
#include "ace/Malloc.h"
#include "ace/Malloc_T.h"
#include "ace/Local_Memory_Pool.h"
#include "ace/Synch_Traits.h"
#include "ace/Singleton.h"
#include "ace/Thread_Mutex.h"
#include "ace/Task.h"
#include "ace/Synch.h"

#include "IMessageBlockManager.h"

typedef  ACE_Malloc<ACE_LOCAL_MEMORY_POOL, ACE_SYNCH_MUTEX> MUTEX_MALLOC;
typedef ACE_Allocator_Adapter<MUTEX_MALLOC> Mutex_Allocator;

class CMessageBlockManager : public IMessageBlockManager
{
public:
	CMessageBlockManager(void);
	virtual ~CMessageBlockManager(void);

	void Init();
	void Close();

	ACE_Message_Block* Create(uint32 u4Size);
	bool Close(ACE_Message_Block* pMessageBlock);

private:
	ACE_Allocator * m_pmsgallocator;
	ACE_Allocator * m_pdata_allocator;
	ACE_Allocator * m_pbuff_allocator;

	ACE_Recursive_Thread_Mutex m_ThreadWriteLock;                     //控制多线程锁
};

typedef ACE_Singleton<CMessageBlockManager,ACE_Null_Mutex> App_MessageBlockManager; 
#endif
