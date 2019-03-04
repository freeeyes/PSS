#ifndef _OBJECTLRULIST_H_
#define _OBJECTLRULIST_H_

#include "define.h"
#include <list>
#include <vector>
using namespace std;

template<class TYPE, class ACE_LOCK>
class CObjectLruList
{
public:
    CObjectLruList(void)
    {
        m_u4Max = 0;
        m_u4Index = 0;
        m_blIsFull = false;
    }

    virtual ~CObjectLruList(void)
    {
    }

    void Init(uint32 u4Count)
    {
        m_u4Max = u4Count;
        m_vecObject.resize(u4Count);
    }

    void AddObject(TYPE object)
    {
        ACE_Guard<ACE_LOCK> WGuard(m_ThreadLock);
        m_vecObject[m_u4Index++] = object;

        if (m_u4Index == m_u4Max)
        {
            m_u4Index = 0;
            m_blIsFull = true;
        }
    }

    void GetAllSavingObject(vector<TYPE>& vecObject)
    {
        vecObject.clear();
        ACE_Guard<ACE_LOCK> WGuard(m_ThreadLock);

        if(true == m_blIsFull)
        {
            for(uint32 iLoop = m_u4Index; iLoop < m_u4Max; iLoop++)
            {
                vecObject.push_back(m_vecObject[iLoop]);
            }

            for(uint32 iLoop = 0; iLoop < m_u4Index; iLoop++)
            {
                vecObject.push_back(m_vecObject[iLoop]);
            }
        }
        else
        {
            for(uint32 iLoop = 0; iLoop < m_u4Index; iLoop++)
            {
                vecObject.push_back(m_vecObject[iLoop]);
            }
        }
    }

private:
    ACE_LOCK      m_ThreadLock;
    vector<TYPE>  m_vecObject;
    uint32        m_u4Max;
    uint32        m_u4Index;
    bool          m_blIsFull;
};

#endif //_OBJECTLRULIST_H_