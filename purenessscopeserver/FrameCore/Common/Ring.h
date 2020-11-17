#ifndef _RING_H
#define _RING_H

//ʵ�ֻ�״���ݵ�������д
//add by freeeyes
#include <stdio.h>

template <class T>
class CRingLink
{
public:
    CRingLink()
    {
    }

    void Init(int nMaxCount)
    {
        Close();

        m_nMaxCount = nMaxCount;
        m_nIndex    = 0;
        m_pRingLink = new T[nMaxCount];
    }

    void Close()
    {
        if(nullptr != m_pRingLink)
        {
            delete[] m_pRingLink;
            m_pRingLink = nullptr;
        }

        m_nMaxCount = 0;
        m_nIndex    = 0;
    }

    T* GetFreeData()
    {
        if(nullptr != m_pRingLink)
        {
            return reinterpret_cast<T*>(m_pRingLink + m_nIndex);
        }
        else
        {
            return nullptr;
        }
    }

    void Clear()
    {
        memset(m_pRingLink, 0, sizeof(T)*m_nMaxCount);
    }

    void Add()
    {
        if(m_nIndex == m_nMaxCount - 1)
        {
            m_nIndex = 0;
        }
        else
        {
            m_nIndex++;
        }
    }

    T* GetLinkData(int nIndex)
    {
        if(nIndex >= m_nMaxCount)
        {
            return nullptr;
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

    T* GetBase()
    {
        return m_pRingLink;
    }

    int GetCount() const
    {
        return m_nMaxCount;
    }

    int GetCurrIndex() const
    {
        return m_nIndex;
    }

private:
    int m_nMaxCount = 0;
    int m_nIndex    = 0;
    T*  m_pRingLink = nullptr;
};

#endif
