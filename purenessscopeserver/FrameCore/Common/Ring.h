#ifndef _RING_H
#define _RING_H

//实现环状数据的完整读写
//在现代C++中，使用vector替换可变数组。array是静态替换的方法
//add by freeeyes

#include <stdio.h>
#include <memory>
#include <vector>

template <class T>
class CRingLink
{
public:
    CRingLink() = default;

    void Init(int nMaxCount)
    {
        Close();

        m_nMaxCount = nMaxCount;
        m_nIndex    = 0;

        for (int i = 0; i < m_nMaxCount; i++)
        {
            auto t = std::make_shared<T>();
            m_pRingLink.emplace_back(t);
        }
    }

    void Close()
    {
        m_pRingLink.clear();

        m_nMaxCount = 0;
        m_nIndex    = 0;
    }

    std::shared_ptr<T> GetFreeData()
    {
        return m_pRingLink[m_nIndex];
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

    std::shared_ptr<T> GetLinkData(int nIndex)
    {
        int nCurrIndex = (m_nIndex - 1 - nIndex) % m_nMaxCount;

        if (nCurrIndex < 0)
        {
            nCurrIndex = m_nMaxCount + nCurrIndex;
        }

        return m_pRingLink[nCurrIndex];
    }

    std::shared_ptr<T> GetBase()
    {
        return m_pRingLink[0];
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
    std::vector<std::shared_ptr<T>> m_pRingLink;
};

#endif
