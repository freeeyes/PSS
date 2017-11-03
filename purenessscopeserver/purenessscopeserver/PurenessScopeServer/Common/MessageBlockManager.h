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
#include <math.h>

#include "IMessageBlockManager.h"

typedef ACE_Malloc<ACE_LOCAL_MEMORY_POOL, ACE_SYNCH_MUTEX> MUTEX_MB_MALLOC;
typedef ACE_Allocator_Adapter<MUTEX_MB_MALLOC> Mutex_MB_Allocator;

//管理释放的ACE_Message_Block内存，组织内存池
#define MAX_MEMORY_BLOCK_COUNT 32

struct _MemoryBlock_List
{
    uint32 m_u4ListSize;
    vector<ACE_Message_Block*> m_mbList;

    void Close()
    {
        for(uint32 i = 0; i < m_mbList.size(); i++)
        {
            m_mbList[i]->release();
        }

        m_mbList.clear();
    }
};

class CMemoryBlock_Pool
{
public:
    CMemoryBlock_Pool()
    {
        for(uint32 i = 0; i < MAX_MEMORY_BLOCK_COUNT; i++)
        {
            m_MemoryBlock_List[i].m_u4ListSize = (uint32)pow((double)2, (int32)i);;
        }
    }

    ~CMemoryBlock_Pool()
    {
        Close();
    }

    void Close()
    {
        for(uint32 i = 0; i < MAX_MEMORY_BLOCK_COUNT; i++)
        {
            m_MemoryBlock_List[i].Close();
        }
    }

    //从内存池里找找看有没有匹配的回收内存
    ACE_Message_Block* Get(uint32 uSize)
    {
        int32 nIndex = pow_2(uSize);

        if(nIndex >= MAX_MEMORY_BLOCK_COUNT)
        {
            return NULL;
        }
        else
        {
            if(m_MemoryBlock_List[nIndex].m_mbList.size() == 0)
            {
                //没有找到匹配的内存池
                return NULL;
            }
            else
            {
                //弹出第一个空余内存返回
                vector<ACE_Message_Block*>::iterator b = m_MemoryBlock_List[nIndex].m_mbList.begin();
                ACE_Message_Block* pmb = (*b);
                m_MemoryBlock_List[nIndex].m_mbList.erase(b);
                return pmb;
            }
        }
    }

    void Set(ACE_Message_Block* pmb)
    {
        if(NULL != pmb)
        {
            pmb->reset();
            uint32 u4Size = (uint32)pmb->size();
            int32 nIndex = pow_2(u4Size);

            if(nIndex >= MAX_MEMORY_BLOCK_COUNT)
            {
                //直接释放
                pmb->release();
            }
            else
            {
                m_MemoryBlock_List[nIndex].m_mbList.push_back(pmb);
            }
        }
    }

private:
    //获得一个数是2的几次幂
    int32 pow_2(int32 x)
    {
        if(x==1)
        {
            return 0;
        }
        else
        {
            return 1+pow_2(x>>1);
        }
    }

private:
    _MemoryBlock_List m_MemoryBlock_List[MAX_MEMORY_BLOCK_COUNT];
};

class CMessageBlockManager : public IMessageBlockManager
{
public:
    CMessageBlockManager(void);
    virtual ~CMessageBlockManager(void);

    void Init();
    void Close();

    ACE_Message_Block* Create(uint32 u4Size);
    bool Close(ACE_Message_Block* pMessageBlock);

    uint32 GetUsedSize();

private:
    uint32                     m_u4UsedSize;                          //当前正在使用的内存大小
    ACE_Allocator* m_pmsgallocator;
    ACE_Allocator* m_pdata_allocator;
    ACE_Allocator* m_pbuff_allocator;
    CMemoryBlock_Pool          m_MemoryBlock_Pool;                    //回收内存池
    ACE_Recursive_Thread_Mutex m_ThreadWriteLock;                     //控制多线程锁
};

typedef ACE_Singleton<CMessageBlockManager,ACE_Null_Mutex> App_MessageBlockManager;
#endif
