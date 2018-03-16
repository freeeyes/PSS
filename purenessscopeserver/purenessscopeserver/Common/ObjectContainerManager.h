#ifndef _OBJECTCONTAINERMANAGER_H_
#define _OBJECTCONTAINERMANAGER_H_

#include "define.h"
#include <list>
#include <vector>
using namespace std;

template<class TYPE, class ACE_LOCK>
class CObjectContainerManager
{
public:
    CObjectContainerManager(void)
    {
        m_u4Max = 0;
        m_u4Index = 0;
        m_blIsFull = false;
    }

    virtual ~CObjectContainerManager(void)
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
            for(int32 iLoop = m_u4Index; 0 < iLoop; iLoop--)
            {
                vecObject.push_back(m_vecObject[iLoop-1]);
            }

            for(int32 iLoop = m_u4Max; m_u4Index < iLoop; iLoop--)
            {
                vecObject.push_back(m_vecObject[iLoop-1]);
            }
        }
        else
        {
            for(int32 iLoop = m_u4Index; 0 < iLoop; iLoop--)
            {
                vecObject.push_back(m_vecObject[iLoop-1]);
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

#endif //_OBJECTCONTAINERMANAGER_H_
