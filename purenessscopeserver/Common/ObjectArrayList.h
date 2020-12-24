#ifndef _OBJECTARRAYLIST_H
#define _OBJECTARRAYLIST_H

#include "define.h"
//数组对象内存管理
//add by ruiqi

template<class T>
class CObjectArrayList
{
public:
    CObjectArrayList()
    {
        m_pMemoryList = nullptr;
        m_u4Count     = 0;
    }

    ~CObjectArrayList(void)
    {
        DestroyMemory();
    }

    void Init(uint32 u4Count)
    {
        CreateMemory(u4Count);
    }

    inline T* GetObject(uint32 u4Index)
    {
        if (u4Index < m_u4Count)
        {
            return m_pMemoryList + u4Index;
        }
        else
        {
            return nullptr;
        }
    }

    inline uint32 GetCount()
    {
        return m_u4Count;
    }

private:
    void CreateMemory(uint32 u4Count)
    {
        try
        {
            m_pMemoryList = new T[u4Count];
        }
        catch (...)
        {
            if (m_pMemoryList == nullptr)
            {
                //程序错误退出枚举
                PSS_LOGGER_DEBUG("[CreateMemory::CreateMemory]Create List({0}) error.", u4Count);
            }
        }

        m_u4Count = u4Count;
    }

    void DestroyMemory()
    {
        if (m_u4Count != 0)
        {
            SAFE_DELETE_ARRAY(m_pMemoryList);
            m_u4Count = 0;
        }
    }
private:
    uint32      m_u4Count;     //对象个数
    T*          m_pMemoryList; //对象头指针
};

#endif
