#ifndef _OBJECTARRAYLIST_H
#define _OBJECTARRAYLIST_H

#include "define.h"
//��������ڴ����
//add by ruiqi

template<class T>
class CObjectArrayList
{
public:
    CObjectArrayList()
    {
        m_pMemoryList = NULL;
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
            return NULL;
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
            if (m_pMemoryList == NULL)
            {
                //��������˳�ö��
                OUR_DEBUG((LM_INFO, "[CreateMemory::CreateMemory]Create List(%d) error.\n", u4Count));
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
    uint32      m_u4Count;     //�������
    T*          m_pMemoryList; //����ͷָ��
};

#endif
