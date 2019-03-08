#ifndef _TIMEWHEELLINK_H
#define _TIMEWHEELLINK_H

//ʱ������
//add by freeeyes
#include "HashTable.h"
#include "define.h"
#include <vector>

using namespace std;

#define MAX_TIMEWHEEL_KEY 20
#define INVAILD_BLOCKID   -1  //��Ч�Ŀ��

//����ָ��ģ��
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

    //��ʼ������
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

        //��������п���ܴ�С
        if (nTimeCycle % m_nTimeInterval != 0)
        {
            m_nBlackCount = nTimeCycle / m_nTimeInterval + 1;
        }
        else
        {
            m_nBlackCount = nTimeCycle / m_nTimeInterval;
        }

        //��ʼ��ʱ���
        for (int i = 0; i < m_nBlackCount; i++)
        {
            CHashTable<T>* pHashContainer = new CHashTable<T>();
            pHashContainer->Init(m_nContainSize, MAX_TIMEWHEEL_KEY);
            m_vecHashContain.push_back(pHashContainer);
        }

        int nListSize = m_nBlackCount * m_nContainSize;
        //��ʼ�������ʱ����ӳ���
        m_htIndexList.Init(nListSize, MAX_TIMEWHEEL_KEY);

        m_pBlockIDList = new int[nListSize];

        for (int i = 0; i < nListSize; i++)
        {
            m_pBlockIDList[i] = INVAILD_BLOCKID;
        }

        m_nBlockIDListIndex = 0;

        m_nCurrBlockID = 0;
    }

    //��������
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

    //�����������һ������
    bool Add_TimeWheel_Object(T* pEntey)
    {
        //�����жϸ�ָ���Ƿ������ĳһ��������
        char szKey[MAX_TIMEWHEEL_KEY] = { '\0' };

        if (NULL == pEntey)
        {
            return false;
        }

        sprintf_safe(szKey, MAX_TIMEWHEEL_KEY, "0x%08x", pEntey);

        int* pCurrBlockListID = m_htIndexList.Get_Hash_Box_Data(szKey);

        if (NULL == pCurrBlockListID || INVAILD_BLOCKID == *pCurrBlockListID)
        {
            //û�����κ�ָ�����������ӵ�������������ָ����
            if (0 > m_vecHashContain[m_nCurrBlockID]->Add_Hash_Data(szKey, pEntey))
            {
                return false;
            }
            else
            {
                //���������
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
            //�Ѿ�������ָ��������
            //��������������У���ʲôҲ������
            if(*pCurrBlockListID == m_nCurrBlockID)
            {
                return true;
            }
            else
            {
                //ɾ��ָ��ʱ����е�����ָ��
                m_vecHashContain[*pCurrBlockListID]->Del_Hash_Data(szKey);

                //ɾ����Ӧ��ʱ�������е�����
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
                    //���������
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

    //ɾ�����������һ������
    void Del_TimeWheel_Object(T* pEntey)
    {
        char szKey[MAX_TIMEWHEEL_KEY] = { '\0' };
        sprintf_safe(szKey, MAX_TIMEWHEEL_KEY, "0x%08x", pEntey);

        int* pCurrBlockListID = m_htIndexList.Get_Hash_Box_Data(szKey);

        if (NULL != pCurrBlockListID && INVAILD_BLOCKID != *pCurrBlockListID)
        {
            // ɾ�����ָ���ھ�������������ݣ���������ݷ������µ������С�
            m_vecHashContain[*pCurrBlockListID]->Del_Hash_Data(szKey);
            *pCurrBlockListID = INVAILD_BLOCKID;
        }

        //����������Ϣ
        if (-1 == m_htIndexList.Del_Hash_Data(szKey))
        {
            OUR_DEBUG((LM_INFO, "[CTimeWheelLink::Del_TimeWheel_Object]Delete(%s) no exist.\n", szKey));
        }
    }

    //�õ�һ�����е�m_pBlockIDListλ��
    int* Get_BlockIDList_Cell()
    {
        int nListSize = m_nBlackCount * m_nContainSize;

        for (int i = m_nBlockIDListIndex; i < nListSize; i++)
        {
            if (m_pBlockIDList[i] == INVAILD_BLOCKID)
            {
                if(i == nListSize - 1)
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

    //����ת��
    void Tick()
    {
        //������һ������ָ���ID
        int nLastBlockID = m_nCurrBlockID;

        if (m_nCurrBlockID >= (int)m_vecHashContain.size() - 1)
        {
            nLastBlockID = 0;
        }
        else
        {
            nLastBlockID++;
        }

        //�õ����һ����Ҫ����Ķ�����Ϣ
        if (NULL != m_vecHashContain[nLastBlockID])
        {
            vector<T*> vecEntey;
            m_vecHashContain[nLastBlockID]->Get_All_Used(vecEntey);

            //����������Ϣ
            for (int i = 0; i < (int)vecEntey.size(); i++)
            {
                char szKey[MAX_TIMEWHEEL_KEY] = { '\0' };
                sprintf_safe(szKey, MAX_TIMEWHEEL_KEY, "0x%08x", vecEntey[i]);

                //����Key����
                Del_TimeWheel_Object(vecEntey[i]);
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
    int              m_nTimeCycle;                        //������ʱ������
    int              m_nTimeInterval;                     //���̼��ʱ����
    int              m_nContainSize;                      //ÿ���Ӽ�������С
    int              m_nCurrBlockID;                      //��ǰvector�������±�
    int*             m_pBlockIDList;                      //��¼BlockID��Ӧ������
    int              m_nBlockIDListIndex;                 //��¼BlockID����ĵ�ǰλ��
    int              m_nBlackCount;                       //������ָ��������
    CHashTable<int>  m_htIndexList;                       //��¼��ǰ����ӳ��
    vecHashContain   m_vecHashContain;                    //ÿ���������������������ɸ�CEntey����
    Timeout_Callback m_fn_Timeout_Callback;               //�ص������ӿ�
    void*            m_pArgContext;                       //�ص������Ĳ���ָ��
};
#endif

