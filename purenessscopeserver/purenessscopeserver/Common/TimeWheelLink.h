#ifndef _TIMEWHEELLINK_H
#define _TIMEWHEELLINK_H

//时间轮盘
//add by freeeyes
#include "HashTable.h"
#include "define.h"
#include <vector>

using namespace std;

#define MAX_TIMEWHEEL_KEY 20
#define INVAILD_BLOCKID   -1  //无效的块号

//函数指针模板
template<class T, void(*Timeout_Callback)(void*, vector<T*>)>
void Timeout_Callback_Ex(void* p, vector<T*> a)
{
    Timeout_Callback(p, a);
}

template <class T>
class CTimeWheelLink
{
private:
    typedef void(*Timeout_Callback)(void*, vector<T*>);

public:
    CTimeWheelLink()
    {
        m_nTimeInterval       = 0;
        m_nTimeCycle          = 0;
        m_nContainSize        = 0;
        m_nCurrBlockID        = 0;
        m_fn_Timeout_Callback = NULL;
        m_pBlockIDList        = NULL;
        m_nBlockIDListIndex   = 0;
        m_nBlackCount         = 0;
        m_pArgContext         = NULL;
    }

    ~CTimeWheelLink()
    {
        Close();
    }

    //初始化轮盘
    void Init(int nTimeCycle, int nTimeInterval, int nContainSize, Timeout_Callback fn_Timeout_Callback, void* pArgContext)
    {
        Close();

        m_nTimeInterval       = nTimeInterval;
        m_nTimeCycle          = nTimeCycle;
        m_nContainSize        = nContainSize;
        m_fn_Timeout_Callback = fn_Timeout_Callback;
        m_pArgContext         = pArgContext;

        if(m_nTimeInterval < 1)
        {
            m_nTimeInterval = 1;
        }

        //获得容器中块的总大小
        if (nTimeCycle % m_nTimeInterval != 0)
        {
            m_nBlackCount = nTimeCycle / m_nTimeInterval + 1;
        }
        else
        {
            m_nBlackCount = nTimeCycle / m_nTimeInterval;
        }

        //初始化时间块
        for (int i = 0; i < m_nBlackCount; i++)
        {
            CHashTable<T>* pHashContainer = new CHashTable<T>();
            pHashContainer->Init(m_nContainSize, MAX_TIMEWHEEL_KEY);
            m_vecHashContain.push_back(pHashContainer);
        }

        int nListSize = m_nBlackCount * m_nContainSize;
        //初始化对象和时间块的映射表
        m_htIndexList.Init(nListSize, MAX_TIMEWHEEL_KEY);

        m_pBlockIDList = new int[nListSize];

        for (int i = 0; i < nListSize; i++)
        {
            m_pBlockIDList[i] = INVAILD_BLOCKID;
        }

        m_nBlockIDListIndex = 0;

        m_nCurrBlockID = 0;
    }

    //清理轮盘
    void Close()
    {
        for (int i = 0; i < (int)m_vecHashContain.size(); i++)
        {
            delete m_vecHashContain[i];
        }

        m_vecHashContain.clear();

        if (NULL != m_pBlockIDList)
        {
            delete[] m_pBlockIDList;
            m_pBlockIDList = NULL;
        }

        m_nBlockIDListIndex = 0;
        m_nBlackCount = 0;
    }

    //往轮盘里添加一个对象
    bool Add_TimeWheel_Object(T* pEntey)
    {
        //首先判断该指针是否存在在某一个容器中
        char szKey[MAX_TIMEWHEEL_KEY] = { '\0' };

        if (NULL == pEntey)
        {
            return false;
        }

        sprintf_safe(szKey, MAX_TIMEWHEEL_KEY, "0x%08x", pEntey);

        int* pCurrBlockListID = m_htIndexList.Get_Hash_Box_Data(szKey);

        if (NULL == pCurrBlockListID || INVAILD_BLOCKID == *pCurrBlockListID)
        {
            //没有在任何指定的容器里，添加到现有最新容器指针中
            if (0 > m_vecHashContain[m_nCurrBlockID]->Add_Hash_Data(szKey, pEntey))
            {
                return false;
            }
            else
            {
                //添加索引表
                int* pBlockID = Get_BlockIDList_Cell();

                if(NULL != pBlockID)
                {
                    *pBlockID = m_nCurrBlockID;
                    int n4Result = m_htIndexList.Add_Hash_Data(szKey, pBlockID);

                    if(-1 == n4Result)
                    {
                        OUR_DEBUG ((LM_INFO, "[Add_TimeWheel_Object](%s)(%d)\n", szKey,n4Result));
                        return false;
                    }

                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
        else
        {
            //已经存在于指定的容器
            //如果在最新容器中，则什么也不做。
            if(*pCurrBlockListID == m_nCurrBlockID)
            {
                return true;
            }
            else
            {
                //删除指定时间块中的数据指针
                m_vecHashContain[*pCurrBlockListID]->Del_Hash_Data(szKey);

                //删除对应在时间索引中的数据
                *pCurrBlockListID = INVAILD_BLOCKID;

                if (-1 == m_htIndexList.Del_Hash_Data(szKey))
                {
                    OUR_DEBUG((LM_INFO, "[CTimeWheelLink::Add_TimeWheel_Object]Deltet(%s) error.\n", szKey));
                }

                if (0 > m_vecHashContain[m_nCurrBlockID]->Add_Hash_Data(szKey, pEntey))
                {
                    return false;
                }
                else
                {
                    //添加索引表
                    int* pBlockID = Get_BlockIDList_Cell();

                    if(NULL != pBlockID)
                    {
                        *pBlockID = m_nCurrBlockID;
                        int n4Result = m_htIndexList.Add_Hash_Data(szKey, pBlockID);

                        if(-1 == n4Result)
                        {
                            OUR_DEBUG ((LM_INFO, "[Add_TimeWheel_Object](%s)(%d)\n", szKey,n4Result));
                            return false;
                        }

                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
        }
    }

    //删除轮盘里面的一个对象
    void Del_TimeWheel_Object(T* pEntey)
    {
        char szKey[MAX_TIMEWHEEL_KEY] = { '\0' };
        sprintf_safe(szKey, MAX_TIMEWHEEL_KEY, "0x%08x", pEntey);

        int* pCurrBlockListID = m_htIndexList.Get_Hash_Box_Data(szKey);

        if (NULL != pCurrBlockListID && INVAILD_BLOCKID != *pCurrBlockListID)
        {
            // 删除这个指针在旧容器里面的数据，将这个数据放在最新的容器中。
            m_vecHashContain[*pCurrBlockListID]->Del_Hash_Data(szKey);
            *pCurrBlockListID = INVAILD_BLOCKID;
        }

        //清理索引信息
        if (-1 == m_htIndexList.Del_Hash_Data(szKey))
        {
            OUR_DEBUG((LM_INFO, "[CTimeWheelLink::Del_TimeWheel_Object]Delete(%s) no exist.\n", szKey));
        }
    }

    //得到一个空闲的m_pBlockIDList位置
    int* Get_BlockIDList_Cell()
    {
        int nListSize = m_nBlackCount * m_nContainSize;

        for (int i = m_nBlockIDListIndex; i < nListSize; i++)
        {
            if (m_pBlockIDList[i] == INVAILD_BLOCKID)
            {
                if(i == nListSize -1)
                {
                    m_nBlockIDListIndex = 0;
                }
                else
                {
                    m_nBlockIDListIndex = i + 1;
                }

                return &m_pBlockIDList[i];
            }
        }

        for (int i = 0; i < m_nBlockIDListIndex; i++)
        {
            if (m_pBlockIDList[i] == INVAILD_BLOCKID)
            {
                m_nBlockIDListIndex = i + 1;
                return &m_pBlockIDList[i];
            }
        }

        return NULL;
    }

    //轮盘转动
    void Tick()
    {
        //获得最后一个轮盘指针的ID
        int nLastBlockID = m_nCurrBlockID;

        if (m_nCurrBlockID >= (int)m_vecHashContain.size() - 1)
        {
            nLastBlockID = 0;
        }
        else
        {
            nLastBlockID++;
        }

        //得到最后一次需要清除的对象信息
        if (NULL != m_vecHashContain[nLastBlockID])
        {
            vector<T*> vecEntey;
            m_vecHashContain[nLastBlockID]->Get_All_Used(vecEntey);

            //清理索引信息
            for (int i = 0; i < (int)vecEntey.size(); i++)
            {
                char szKey[MAX_TIMEWHEEL_KEY] = { '\0' };
                sprintf_safe(szKey, MAX_TIMEWHEEL_KEY, "0x%08x", vecEntey[i]);

                if (-1 == m_htIndexList.Del_Hash_Data(szKey))
                {
                    OUR_DEBUG((LM_INFO, "[CTimeWheelLink::Tick]Delete(%s) error.\n", szKey));
                }
            }

            if (NULL != m_fn_Timeout_Callback)
            {
                m_fn_Timeout_Callback(m_pArgContext, vecEntey);
            }

            //OUR_DEBUG((LM_INFO, "[CTimeWheelLink::Tick]nLastBlockID=%d, m_nCurrBlockID=%d.\n", nLastBlockID, m_nCurrBlockID));
            m_vecHashContain[nLastBlockID]->Clear();
        }
        else
        {
            OUR_DEBUG((LM_INFO, "[CTimeWheelLink::Tick]m_vecHashContain is NULL,nLastBlockID=%d.\n", nLastBlockID));
        }

        m_nCurrBlockID = nLastBlockID;
    }
private:
    typedef vector<CHashTable<T>*> vecHashContain;

private:
    int              m_nTimeCycle;                        //轮盘总时间周期
    int              m_nTimeInterval;                     //轮盘检查时间间隔
    int              m_nContainSize;                      //每个子集容器大小
    int              m_nCurrBlockID;                      //当前vector容器的下标
    int*             m_pBlockIDList;                      //记录BlockID对应的数组
    int              m_nBlockIDListIndex;                 //记录BlockID数组的当前位置
    int              m_nBlackCount;                       //轮盘里指针块的数量
    CHashTable<int>  m_htIndexList;                       //记录当前引用映射
    vecHashContain   m_vecHashContain;                    //每个块的容器，里面包含若干个CEntey对象
    Timeout_Callback m_fn_Timeout_Callback;               //回调函数接口
    void*            m_pArgContext;                       //回调上下文参数指针
};
#endif

