#ifndef _RING_H
#define _RING_H

//实现环状数据的完整读写
//add by freeeyes
#include <stdio.h>
#include <vector>

using namespace std;

template <class T>
class CRingLink
{
public:
    CRingLink()
    {
        m_nMaxCount   = 0;
        m_nNextIndex  = 0;
        m_nBgeinIndex = -1;
        m_pRingLink   = NULL;
    }

    void Init(int nMaxCount)
    {
        Close();

        m_nMaxCount   = nMaxCount;
        m_nNextIndex  = 0;
        m_nBgeinIndex = -1;
        m_pRingLink   = new T[nMaxCount];
    }

    ~CRingLink()
    {
        Close();
    }

    void Close()
    {
        if(NULL != m_pRingLink)
        {
            delete[] m_pRingLink;
            m_pRingLink = NULL;
        }

        m_nMaxCount = 0;
        m_nIndex    = 0;
    }

    void Clear()
    {
        memset(m_pRingLink, 0, sizeof(T)*m_nMaxCount);
    }

    void Add(T objT)
    {
        m_pRingLink[m_nNextIndex] = objT;

        //如果是第一个
        if (m_nBgeinIndex == -1)
        {
            m_nBgeinIndex = 0;
        }

        if(m_nNextIndex == m_nMaxCount - 1)
        {
            m_nNextIndex = 0;
        }
        else
        {
            m_nNextIndex++;
        }

        if (m_nNextIndex == m_nBgeinIndex)
        {
            if (m_nNextIndex == 0)
            {
                m_nBgeinIndex = m_nMaxCount - 1;
            }
            else
            {
                m_nBgeinIndex = m_nNextIndex;
            }
        }
    }

    T* GetLinkData(int nIndex)
    {
        if(nIndex >= m_nMaxCount)
        {
            return NULL;
        }
        else
        {
            int nCurrIndex = (m_nIndex - 1 - nIndex) % m_nMaxCount;

            if(nCurrIndex < 0)
            {
                nCurrIndex = m_nMaxCount + nCurrIndex;
            }

            return reinterpret_cast<T*>(m_pRingLink + nCurrIndex);
        }
    }

    //得到当前有效对象列表
    void GetList(vector<T*> objDataList)
    {
        if (m_nBgeinIndex == -1)
        {
            return;
        }

        if (m_nBgeinIndex < m_nNextIndex)
        {
            for (int i = m_nNextIndex - 1; i <= m_nBgeinIndex; i--)
            {
                objDataList.push_back(&m_pRingLink[i]);
            }
        }
        else
        {
            for (int i = m_nNextIndex - 1; i < 0; i++)
            {
                objDataList.push_back(&m_pRingLink[i]);
            }

            for (int i = 0; i < m_nBgeinIndex; i++)
            {
                objDataList.push_back(&m_pRingLink[i]);
            }
        }
    }

    T* GetBase()
    {
        return m_pRingLink;
    }

    int GetCount()
    {
        return m_nMaxCount;
    }

    int GetCurrIndex()
    {
        return m_nIndex;
    }

private:
    int m_nMaxCount;
    T*  m_pRingLink;
    int m_nNextIndex;     //当前下一个需要用到的对象指针
    int m_nBgeinIndex;    //当前第一个用到的对象指针
};

#endif
