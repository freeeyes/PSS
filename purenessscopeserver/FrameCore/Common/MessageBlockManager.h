#ifndef MESSAGEBLOCKMANAGER_H
#define MESSAGEBLOCKMANAGER_H

//���︺��������д�����mb�����ݣ���ʹ���ڴ�ء�
//�������freebird���뷨����д�ⲿ�ִ��룬��Ȼ������æ�������ⲿ�ֱ�����ɣ�
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
#include <unordered_map>

#include "IMessageBlockManager.h"
#include "XmlConfig.h"

//�����ͷŵ�ACE_Message_Block�ڴ棬��֯�ڴ��
const int32 MAX_MEMORY_BLOCK_COUNT = 32;

class _MemoryBlock_List
{
public:
    uint32 m_u4ListSize       = 0;
    vector<ACE_Message_Block*> m_mbList;

    void Close()
    {
        for(ACE_Message_Block* pmb : m_mbList)
        {
            pmb->release();
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
            m_MemoryBlock_List[i].m_u4ListSize = (uint32)pow((double)2, (int32)i);
        }
    }

    void Init(uint8 u1Debug)
    {
        m_u1Debug = u1Debug;
    }

    void Close()
    {
        for(_MemoryBlock_List& memoryblock : m_MemoryBlock_List)
        {
            memoryblock.Close();
        }
    }

    //���ڴ�������ҿ���û��ƥ��Ļ����ڴ�
    ACE_Message_Block* Get(uint32 uSize)
    {
        if (0 == uSize)
        {
            return NULL;
        }

        int32 nIndex = pow_2(uSize);

        if(nIndex >= MAX_MEMORY_BLOCK_COUNT)
        {
            return NULL;
        }
        else
        {
            if(m_MemoryBlock_List[nIndex].m_mbList.size() == 0)
            {
                //û���ҵ�ƥ����ڴ��
                return NULL;
            }
            else
            {
                //������һ�������ڴ淵��
                vector<ACE_Message_Block*>::iterator b = m_MemoryBlock_List[nIndex].m_mbList.begin();
                ACE_Message_Block* pmb = (*b);
                m_MemoryBlock_List[nIndex].m_mbList.erase(b);
                return pmb;
            }
        }
    }

    bool Add_Used(ACE_Message_Block* pmb)
    {
        if (1 == m_u1Debug)
        {
            //�������ʹ�õĶ���ָ��
            unordered_map<ACE_Message_Block*, uint8>::iterator f = m_mapUsedMemoryMB.find(pmb);

            if (m_mapUsedMemoryMB.end() != f)
            {
                OUR_DEBUG((LM_ERROR, "[CMemoryBlock_Pool::Add_Used]add 0x%08x is used.\n", pmb));
                return false;
            }
            else
            {
                m_mapUsedMemoryMB.insert(std::make_pair(pmb, 1));
                return true;
            }
        }

        return true;
    }

    bool Check_Used(ACE_Message_Block* pmb)
    {
        if (1 == m_u1Debug)
        {
            unordered_map<ACE_Message_Block*, uint8>::iterator f = m_mapUsedMemoryMB.find(pmb);

            //����ָ���Ƿ���ʹ�ù�����Чָ��
            if (m_mapUsedMemoryMB.end() != f)
            {
                //������Լ���ָ��
                m_mapUsedMemoryMB.erase(f);
                return true;
            }
            else
            {
                OUR_DEBUG((LM_ERROR, "[CMemoryBlock_Pool::Check_Used]add 0x%08x is not used.\n", pmb));
                return false;
            }
        }

        return true;
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
                //ֱ���ͷ�
                pmb->release();
            }
            else
            {
                m_MemoryBlock_List[nIndex].m_mbList.push_back(pmb);
            }
        }
    }

private:
    //���һ������2�ļ�����
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

    uint8                                    m_u1Debug = 0;    //0�ǹرգ�1�ǿ���
    unordered_map<ACE_Message_Block*, uint8> m_mapUsedMemoryMB;
    _MemoryBlock_List                        m_MemoryBlock_List[MAX_MEMORY_BLOCK_COUNT];
};

class CMessageBlockManager : public IMessageBlockManager
{
public:
    CMessageBlockManager(void);

    void Init();
    void Close();

    virtual ACE_Message_Block* Create(uint32 u4Size);
    virtual bool Close(ACE_Message_Block* pMessageBlock, uint32 u4BlockLen = 0);

    uint32 GetUsedSize() const;

private:
    uint32                     m_u4UsedSize      = 0;                          //��ǰ����ʹ�õ��ڴ��С
    CMemoryBlock_Pool          m_MemoryBlock_Pool;                    //�����ڴ��
    ACE_Recursive_Thread_Mutex m_ThreadWriteLock;                     //���ƶ��߳���
};

typedef ACE_Singleton<CMessageBlockManager,ACE_Null_Mutex> App_MessageBlockManager;
#endif
