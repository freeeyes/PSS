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
        m_pMemonyList = NULL;
        m_u4Count     = 0;
    }

    ~CObjectArrayList(void)
    {
        DestroyMemony();
    }

    void Init(uint32 u4Count)
    {
        CreateMemony(u4Count);
    }

    inline T* GetObject(uint32 u4Index)
    {
        if (u4Index < m_u4Count)
        {
            return m_pMemonyList + u4Index;
        }
        else
        {
            return NULL;
        }
    }
private:
    void CreateMemony(uint32 u4Count)
    {
        try
        {
            //m_pMemonyList = (T*)(m_pMem->malloc(sizeof(T) * u4Count));
            m_pMemonyList = new T[u4Count];
        }
        catch (...)
        {
            if (m_pMemonyList == NULL)
            {
                //程序错误退出枚举
                OUR_DEBUG((LM_INFO, "[CreateMemony]Create List(%d) error.\n", u4Count));
            }
        }

        m_u4Count = u4Count;
    }

    void DestroyMemony()
    {
        if (m_u4Count != 0)
        {
            SAFE_DELETE_ARRAY(m_pMemonyList);
            m_u4Count = 0;
        }
    }
private:
    uint32      m_u4Count;     //对象个数
    T*          m_pMemonyList; //对象头指针
};

#endif
