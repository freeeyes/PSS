#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "define.h"

enum EM_HASH_STATE
{
    EM_SELECT = 0,
    EM_INSERT,
};

enum EM_HASH_DEBUG
{
    HASH_DEBUG_ON = 0,
    HASH_DEBUG_OFF,
};

#define DEF_HASH_KEY_SIZE 20           //默认的Hash key长度

//hash表结构
template <class T>
struct _Hash_Table_Cell
{
    int32 m_nPosIndex;                     //当前对象在对象数组的下标
    char* m_pKey;                          //当前的key值
    T*    m_pValue;                        //当前数据体指针
    short m_sKeyLen;                       //当前的key长度
    char  m_cExists;                       //当前块是否已经使用,1已经使用，0没有被使用

    _Hash_Table_Cell()
    {
        m_cExists       = 0;
        m_nPosIndex     = 0;
        m_pKey          = NULL;
        m_sKeyLen       = DEF_HASH_KEY_SIZE;
        m_pValue        = NULL;
    }

    //初始化对象
    void Init(char* pKey, int32 nKeyLen, int32 nIndex)
    {
        m_cExists       = 0;
        m_pKey          = pKey;
        m_sKeyLen       = nKeyLen;
        m_pValue        = NULL;
        m_nPosIndex     = nIndex;
    }

    //清理对象信息
    void Clear()
    {
        m_cExists       = 0;

        if(NULL != m_pKey)
        {
            m_pKey[0] = '\0';
        }

        if(NULL != m_pValue)
        {
            m_pValue = NULL;
        }
    }

    //得到当前对象大小
    int32 Get_Size(short sKeyLen)
    {
        return sizeof(_Hash_Table_Cell) + sKeyLen * sizeof(char);
    }
};

//hash链表结构
template <class T>
struct _Hash_Link_Info
{
    int32   m_cExists;                          //当前块是否已经使用,1已经使用，0没有被使用
    int32   m_nPosIndex;                        //当前对象在对象数组的下标
    _Hash_Table_Cell<T>* m_pData;             //当前数据
    _Hash_Link_Info*     m_pNext;             //下一个对象指针(用于链表)
    _Hash_Link_Info*     m_pPerv;             //上一个对象指针(用于链表)

    _Hash_Link_Info()
    {
        m_cExists   = 0;
        m_nPosIndex = 0;
        m_pData     = NULL;
        m_pNext     = NULL;
        m_pPerv     = NULL;
    }

    //初始化对象
    void Init(int32 nIndex)
    {
        m_cExists   = 0;
        m_nPosIndex = nIndex;
    }

    //清理对象信息
    void Clear()
    {
        m_cExists   = 0;
        m_pData     = NULL;
        m_pNext     = NULL;
        m_pPerv     = NULL;
    }

    //得到当前对象大小
    int32 Get_Size()
    {
        return sizeof(_Hash_Link_Info);
    }
};

//Hash对象池结构(管理Hash数据对象实体)
template <class T>
class CHashPool
{
public:
    CHashPool()
    {
        m_pBase      = NULL;
        m_lpTable    = NULL;
        m_nCount     = 0;
        m_nUsedCount = 0;
        m_sKeyLen    = 0;
        m_nCurrIndex = 0;
    }

    ~CHashPool()
    {
        Close();
    }

    int32 Get_Size(int32 nHashCount, short sKeySize = DEF_HASH_KEY_SIZE)
    {
        _Hash_Table_Cell<T> objCell;
        int32 nCellSize = objCell.Get_Size(sKeySize);
        return nCellSize * nHashCount;
    }

    void Init(char* pData, int32 nHashCount, short sKeySize = DEF_HASH_KEY_SIZE)
    {
        Set_Base_Addr(pData, nHashCount, sKeySize);
    }

    _Hash_Table_Cell<T>* Get_Index(int32 nIndex)
    {
        return &m_lpTable[nIndex];
    }

    void Close()
    {
        if(NULL != m_pBase)
        {
            //delete[] m_pBase;
            m_pBase   = NULL;
            m_lpTable = NULL;
        }

        m_nCount     = 0;
        m_nUsedCount = 0;
        m_nCurrIndex = 0;
        m_sKeyLen    = 0;
    }

    //设置一个已知的内存数组块(必须初始化调用)
    void Set_Base_Addr(char* pData, int32 nCount, short sKeySize)
    {
        m_pBase      = pData;
        m_lpTable    = reinterpret_cast<_Hash_Table_Cell<T>*>(pData);
        m_nCount     = nCount;
        m_sKeyLen    = sKeySize;
        m_nCurrIndex = 0;
        int32 nKeyPool = sizeof(_Hash_Table_Cell<T>) * nCount;

        for(int32 i = 0; i < m_nCount; i++)
        {
            char* pKey = pData + nKeyPool + i * sKeySize;
            m_lpTable[i].Init(pKey, sKeySize, i);
        }
    }

    //得到当前key的长度
    short Get_Key_Length()
    {
        return m_sKeyLen;
    }

    //得到当前缓冲块总个数
    int32 Get_Count()
    {
        return m_nCount;
    }

    //得到目前正在使用的对象个数
    int32 Get_Used_Count()
    {
        return m_nUsedCount;
    }

    //获得一个空余的缓冲块
    _Hash_Table_Cell<T>* Create()
    {
        _Hash_Table_Cell<T>* pCurrCell = NULL;

        if(NULL == m_lpTable || m_nUsedCount == m_nCount)
        {
            return pCurrCell;
        }

        //首先从当前点往后找
        for(int32 i = m_nCurrIndex; i < m_nCount; i++)
        {
            //printf("[CHashPool::Create]i=%d, m_nCurrIndex=%d, m_nCount=%d.\n", i, m_nCurrIndex, m_nCount);
            if(m_lpTable[i].m_cExists == 1)
            {
                continue;
            }
            else
            {
                pCurrCell            = &m_lpTable[i];
                pCurrCell->m_cExists = 1;

                if(i < m_nCount - 1)
                {
                    m_nCurrIndex = i + 1;
                }
                else
                {
                    m_nCurrIndex = 0;
                }

                m_nUsedCount++;
                return pCurrCell;
            }
        }

        //如果没找到，从0到当前点
        for(int32 i = 0; i < m_nCurrIndex; i++)
        {
            if(m_lpTable[i].m_cExists == 1)
            {
                continue;
            }
            else
            {
                pCurrCell = &m_lpTable[i];
                pCurrCell->m_cExists = 1;
                m_nCurrIndex = i + 1;
                m_nUsedCount++;
                return pCurrCell;
            }
        }

        //已经没有空余
        return pCurrCell;
    }

    //释放一个正在使用的缓冲块
    void Delete(_Hash_Table_Cell<T>* pData)
    {
        if (NULL != pData)
        {
            int32 nIndex = pData->m_nPosIndex;

            if (nIndex >= 0 && nIndex < m_nCount && pData->m_cExists == 1)
            {
                m_nUsedCount--;
                pData->Clear();
            }
        }
    }

private:
    char*                m_pBase;        //当前内存起始位置
    _Hash_Table_Cell<T>* m_lpTable;      //当前Hash对象数组
    int32                m_nCount;       //当前Hash数组个数
    int32                m_nUsedCount;   //当前已使用的Hash对象
    int32                m_nCurrIndex;   //已经运行到的当前Hash数组位置
    short                m_sKeyLen;      //当前key的长度
};

//HashTable链表类
template <class T>
class CHashLinkPool
{
public:
    CHashLinkPool()
    {
        m_pBase      = NULL;
        m_lpTable    = NULL;
        m_nCount     = 0;
        m_nUsedCount = 0;
        m_nCurrIndex = 0;
    }

    ~CHashLinkPool()
    {
        Close();
    }

    int32 Get_Size(int32 nHashCount)
    {
        _Hash_Link_Info<T> objLink;
        int32 nLinkSize = objLink.Get_Size();
        return nLinkSize * nHashCount;
    }

    void Init(char* pData, int32 nHashCount)
    {
        Set_Base_Addr(pData, nHashCount);
    }

    void Close()
    {
        if(NULL != m_pBase)
        {
            //delete[] m_pBase;
            m_pBase   = NULL;
            m_lpTable = NULL;
        }

        m_nCount     = 0;
        m_nUsedCount = 0;
        m_nCurrIndex = 0;
    }

    //设置一个已知的内存数组块(必须初始化调用)
    void Set_Base_Addr(char* pData, int32 nCount)
    {
        m_pBase      = pData;
        m_lpTable    = reinterpret_cast<_Hash_Link_Info<T>*>(pData);
        m_nCount     = nCount;
        m_nCurrIndex = 0;

        for(int32 i = 0; i < m_nCount; i++)
        {
            m_lpTable[i].Init(i);
        }
    }

    //得到当前缓冲块总个数
    int32 Get_Count()
    {
        return m_nCount;
    }

    //得到目前正在使用的对象个数
    int32 Get_Used_Count()
    {
        return m_nUsedCount;
    }

    //获得一个空余的缓冲块
    _Hash_Link_Info<T>* Create()
    {
        _Hash_Link_Info<T>* pCurrCell = NULL;

        if(NULL == m_lpTable || m_nUsedCount == m_nCount)
        {
            return pCurrCell;
        }

        //首先从当前点往后找
        for(int32 i = m_nCurrIndex; i < m_nCount; i++)
        {
            if(m_lpTable[i].m_cExists == 1)
            {
                continue;
            }
            else
            {
                pCurrCell            = &m_lpTable[i];
                pCurrCell->m_cExists = 1;

                if(i < m_nCount - 1)
                {
                    m_nCurrIndex = i + 1;
                }
                else
                {
                    m_nCurrIndex = 0;
                }

                m_nUsedCount++;
                return pCurrCell;
            }
        }

        //如果没找到，从0到当前点
        for(int32 i = 0; i < m_nCurrIndex; i++)
        {
            if(m_lpTable[i].m_cExists == 1)
            {
                continue;
            }
            else
            {
                pCurrCell = &m_lpTable[i];
                pCurrCell->m_cExists = 1;
                m_nCurrIndex = i + 1;
                m_nUsedCount++;
                return pCurrCell;
            }
        }

        //已经没有空余
        return pCurrCell;
    }

    //释放一个正在使用的缓冲块
    void Delete(_Hash_Link_Info<T>* pData)
    {
        if (NULL != pData)
        {
            int32 nIndex = pData->m_nPosIndex;

            if (nIndex >= 0 && nIndex < m_nCount && pData->m_cExists == 1)
            {
                m_nUsedCount--;
                pData->Clear();
            }
        }
    }

private:
    char*                m_pBase;        //当前内存起始位置
    _Hash_Link_Info<T>*  m_lpTable;      //当前Hash对象数组
    int32                m_nCount;       //当前Hash数组个数
    int32                m_nUsedCount;   //当前已使用的Hash对象
    int32                m_nCurrIndex;   //已经运行到的当前Hash数组位置
};

//hashTable类
template <class T>
class CHashTable
{
public:
    CHashTable()
    {
        m_pBase          = NULL;
        m_lpTable        = NULL;
        m_cIsDelete      = 0;
        m_nCurrLinkIndex = 0;
        m_emHashDebug    = HASH_DEBUG_OFF; //默认DEBUG是关闭。
    }

    ~CHashTable()
    {
        Close();
    }

    //得到整体数据内存大小
    size_t Get_Size(uint32 u4HashCount, short sKeySize = DEF_HASH_KEY_SIZE)
    {
        return m_objHashPool.Get_Size((int32)u4HashCount, sKeySize)
               + m_objHashLinkPool.Get_Size((int32)u4HashCount)
               + sizeof(_Hash_Link_Info<T>* ) * u4HashCount;
    }

    //关闭Hash块
    void Close()
    {
        if(0 == m_cIsDelete)
        {
            delete[] m_pBase;
        }

        m_pBase   = NULL;
        m_lpTable = NULL;
    }

    //初始化Hash块
    void Init(int32 nHashCount, int32 nKeySize = DEF_HASH_KEY_SIZE, EM_HASH_DEBUG emHashDebug = HASH_DEBUG_OFF)
    {
        size_t stSize = Get_Size(nHashCount, nKeySize);
        char* pData = new char[stSize];
        memset(pData, 0, stSize);
        int32 nPos         = 0;
        m_pBase          = pData;
        m_nCurrLinkIndex = 0;
        m_cIsDelete      = 0;
        m_objHashPool.Init(&pData[nPos], nHashCount, nKeySize);
        nPos += m_objHashPool.Get_Size(nHashCount, nKeySize);
        m_objHashLinkPool.Init(&pData[nPos], nHashCount);
        nPos += m_objHashLinkPool.Get_Size(nHashCount);
        m_lpTable = (_Hash_Link_Info<T>** )&pData[nPos];

        for(int32 i = 0; i < nHashCount; i++)
        {
            m_lpTable[i] = NULL;
        }

        m_emHashDebug = emHashDebug;
        //nPos += sizeof(_Hash_Link_Info<T>* ) * nHashCount;
    }

    //初始化Hash块(给定内存地址)
    void Init_By_Memory(char* pData, int32 nHashCount, int32 nKeySize = DEF_HASH_KEY_SIZE, EM_HASH_DEBUG emHashDebug = HASH_DEBUG_OFF, char cIsDelete = 0)
    {
        memset(pData, 0, Get_Size(nHashCount, nKeySize));
        int32 nPos         = 0;
        m_pBase          = pData;
        m_nCurrLinkIndex = 0;
        m_cIsDelete      = cIsDelete;
        m_objHashPool.Init(&pData[nPos], nHashCount, nKeySize);
        nPos += m_objHashPool.Get_Size(nHashCount, nKeySize);
        m_objHashLinkPool.Init(&pData[nPos], nHashCount);
        nPos += m_objHashLinkPool.Get_Size(nHashCount);
        m_lpTable = (_Hash_Link_Info<T>** )&pData[nPos];

        for(int32 i = 0; i < nHashCount; i++)
        {
            m_lpTable[i] = NULL;
        }

        m_emHashDebug = emHashDebug;
        //nPos += sizeof(_Hash_Link_Info<T>* ) * nHashCount;
        //printf("[Init]nPos=%d.\n", nPos);
    }

    //得到当前对象总数
    int32 Get_Count()
    {
        return m_objHashPool.Get_Count();
    }

    //得到数据中正在使用的个数
    int32 Get_Used_Count()
    {
        return m_objHashPool.Get_Used_Count();
    }

    //弹出一个在链表中的_Hash_Link_Info<T>* pT
    T* Pop()
    {
        T* pT = NULL;

        if(NULL == m_lpTable)
        {
            //没有找到共享内存
            if (HASH_DEBUG_ON == m_emHashDebug)
            {
                OUR_DEBUG((LM_INFO, "[Pop]m_lpTable is NULL.\n"));
            }

            return pT;
        }

        //寻找一个可以用的对象，弹出来。
        for(int32 i = m_nCurrLinkIndex; i < m_objHashPool.Get_Count(); i++)
        {
            if (m_lpTable[i] != NULL)
            {
                //取出当前的数据
                pT = m_lpTable[i]->m_pData->m_pValue;
                char* pKey = m_lpTable[i]->m_pData->m_pKey;

                //设置状态
                m_nCurrLinkIndex = i;

                if (HASH_DEBUG_ON == m_emHashDebug)
                {
                    OUR_DEBUG((LM_INFO, "[Pop]1 pKey = %s.\n", pKey));
                }

                //回收数据
                int32 nRet = Del_Hash_Data(pKey);

                if (HASH_DEBUG_ON == m_emHashDebug)
                {
                    OUR_DEBUG((LM_INFO, "[Pop]1 index=%d, Del_Hash_Data(%d), Currused=%d pT=0x%08x.\n", m_nCurrLinkIndex, nRet, Get_Used_Count(), pT));
                }

                return pT;
            }
        }

        for(int32 i = 0; i < m_nCurrLinkIndex; i++)
        {
            if (m_lpTable[i] != NULL)
            {
                //取出当前的数据
                pT = m_lpTable[i]->m_pData->m_pValue;
                char* pKey = m_lpTable[i]->m_pData->m_pKey;

                if (HASH_DEBUG_ON == m_emHashDebug)
                {
                    OUR_DEBUG((LM_INFO, "[Pop]2 pKey = %s.\n", pKey));
                }

                //设置状态
                m_nCurrLinkIndex = i;

                //回收数据
                int32 nRet = Del_Hash_Data(pKey);

                if (HASH_DEBUG_ON == m_emHashDebug)
                {
                    OUR_DEBUG((LM_INFO, "[Pop]2 index=%d, Del_Hash_Data(%d), Currused=%d, pT=0x%08x.\n", m_nCurrLinkIndex, nRet, Get_Used_Count(), pT));
                }

                return pT;
            }
        }

        if (HASH_DEBUG_ON == m_emHashDebug)
        {
            OUR_DEBUG((LM_INFO, "[Pop]2 index=%d, no Find, Currused=%d.\n", m_nCurrLinkIndex, Get_Used_Count()));
        }

        return pT;
    }

    //将一个已经使用完成的对象，放回到链表
    bool Push(const char* pKey, T* pT)
    {
        if(NULL == m_lpTable)
        {
            //没有找到共享内存
            if (HASH_DEBUG_ON == m_emHashDebug)
            {
                OUR_DEBUG((LM_INFO, "[Push] m_lpTable is NULL, pKey=%s, pT=0x%08x.\n", pKey, pT));
            }

            return false;
        }

        //根据key计算这个点的hash位置
        unsigned long uHashStart = HashString(pKey, m_objHashPool.Get_Count());

        if (HASH_DEBUG_ON == m_emHashDebug)
        {
            OUR_DEBUG((LM_INFO, "[Push] index =%d, pKey=%s, pT=0x%08x.\n", uHashStart, pKey, pT));
        }

        _Hash_Link_Info<T>* pLastLink = m_lpTable[uHashStart];

        while(NULL != pLastLink)
        {
            _Hash_Link_Info<T>* pTempLink = pLastLink->m_pNext;

            if (NULL != pTempLink)
            {
                pLastLink = pTempLink;
            }
            else
            {
                break;
            }
        }

        //从对象池中获取一个新对象
        _Hash_Table_Cell<T>* pData = m_objHashPool.Create();

        if(NULL == pData)
        {
            if (HASH_DEBUG_ON == m_emHashDebug)
            {
                OUR_DEBUG((LM_INFO, "[Push]m_objHashPool pData = NULL.\n"));
            }

            return false;
        }

        _Hash_Link_Info<T>* pLink = m_objHashLinkPool.Create();

        if(NULL == pLink)
        {
            if (HASH_DEBUG_ON == m_emHashDebug)
            {
                OUR_DEBUG((LM_INFO, "[Push]m_objHashLinkPool pData = NULL.\n"));
            }

            return false;
        }

        sprintf_safe(pData->m_pKey, pData->m_sKeyLen, "%s", pKey);
        pData->m_pValue    = pT;
        pLink->m_pData     = pData;
        pLink->m_pPerv     = pLastLink;

        if(pLastLink != NULL)
        {
            pLastLink->m_pNext = pLink;
        }
        else
        {
            m_lpTable[uHashStart] = pLink;
        }

        if (HASH_DEBUG_ON == m_emHashDebug)
        {
            OUR_DEBUG((LM_INFO, "[Push] index =%d, pKey=%s, Currused=%d, pT=0x%08x OK.\n", m_emHashDebug, pKey, Get_Used_Count(), pT));
        }

        return true;
    }

    //得到所有正在使用的指针
    void Get_All_Used(vector<T*>& vecList)
    {
        if(NULL == m_lpTable)
        {
            return;
        }

        vecList.clear();

        for(int32 i = 0; i < m_objHashPool.Get_Count(); i++)
        {
            if(NULL != m_lpTable[i])
            {
                _Hash_Link_Info<T>* pLastLink = m_lpTable[i];

                while(NULL != pLastLink)
                {
                    if (NULL != pLastLink->m_pData)
                    {
                        vecList.push_back(pLastLink->m_pData->m_pValue);
                    }
                    else
                    {
                        OUR_DEBUG((LM_INFO, "[CHashTable::Get_All_Used]pLastLink->m_pData is NULL.\n"));
                    }

                    pLastLink = pLastLink->m_pNext;
                }
            }
        }
    }

    //得到所有正在使用的key
    void Get_All_Used_Key(vector<string>& vecList)
    {
        if (NULL == m_lpTable)
        {
            return;
        }

        vecList.clear();

        for (int32 i = 0; i < m_objHashPool.Get_Count(); i++)
        {
            if (NULL != m_lpTable[i])
            {
                _Hash_Link_Info<T>* pLastLink = m_lpTable[i];

                while (NULL != pLastLink)
                {
                    if (NULL != pLastLink->m_pData)
                    {
                        vecList.push_back(pLastLink->m_pData->m_pKey);
                    }
                    else
                    {
                        OUR_DEBUG((LM_INFO, "[CHashTable::Get_All_Used_Key]pLastLink->m_pData is NULL.\n"));
                    }

                    pLastLink = pLastLink->m_pNext;
                }
            }
        }
    }

    //清理当前Hash数组中的所有对象指针
    void Clear()
    {
        vector<string> vecList;

        if (NULL == m_lpTable)
        {
            return;
        }

        if (HASH_DEBUG_ON == m_emHashDebug)
        {
            OUR_DEBUG((LM_INFO, "[Clear](%d)Data is Clear.\n", Get_Used_Count()));
        }

        for (int32 i = 0; i < m_objHashPool.Get_Count(); i++)
        {
            if (NULL != m_lpTable[i])
            {
                _Hash_Link_Info<T>* pLastLink = m_lpTable[i];

                while (NULL != pLastLink)
                {
                    if (NULL != pLastLink->m_pData)
                    {
                        vecList.push_back(pLastLink->m_pData->m_pKey);
                    }
                    else
                    {
                        OUR_DEBUG((LM_INFO, "[CHashTable::Clear]pLastLink->m_pData is NULL.\n"));
                    }

                    pLastLink = pLastLink->m_pNext;
                }
            }
        }

        //清理数据
        for (int32 i = 0; i < (int32)vecList.size(); i++)
        {
            if (-1 == Del_Hash_Data(vecList[i].c_str()))
            {
                OUR_DEBUG((LM_INFO, "[CHashTable::Clear](%s) is Delete error.\n", vecList[i].c_str()));
            }
        }
    }

    //添加一个Hash数据块
    int32 Add_Hash_Data(const char* pKey, T* pValue)
    {
        if(NULL == m_lpTable)
        {
            //没有找到共享内存
            if (HASH_DEBUG_ON == m_emHashDebug)
            {
                OUR_DEBUG((LM_INFO, "[Add_Hash_Data]m_lpTable is NULL.\n"));
            }

            return -1;
        }

        if((short)strlen(pKey) >= m_objHashPool.Get_Key_Length())
        {
            //当前key的长度超过了HashTable的key长度。
            if (HASH_DEBUG_ON == m_emHashDebug)
            {
                OUR_DEBUG((LM_INFO, "[Add_Hash_Data](short)strlen(pKey) >= m_objHashPool.Get_Key_Length().\n"));
            }

            return -1;
        }

        int32 nPos = GetHashTablePos(pKey, EM_INSERT);

        if(-1 == nPos)
        {
            //内存已满，或者数据已经存在
            if (HASH_DEBUG_ON == m_emHashDebug)
            {
                OUR_DEBUG((LM_INFO, "[Add_Hash_Data]GetHashTablePos is -1.\n", nPos));
            }

            return -1;
        }
        else
        {
            if(NULL == m_lpTable[nPos])
            {
                //从对象池中获取一个新对象
                _Hash_Table_Cell<T>* pData = m_objHashPool.Create();

                if(NULL == pData)
                {
                    return -1;
                }

                _Hash_Link_Info<T>* pLink = m_objHashLinkPool.Create();

                if(NULL == pLink)
                {
                    return -1;
                }
                else
                {
                    sprintf_safe(pData->m_pKey, pData->m_sKeyLen, "%s", pKey);
                    pData->m_pValue = pValue;
                    pLink->m_pData = pData;
                    m_lpTable[nPos] = pLink;
                }
            }
            else
            {
                //如果已经存在对象，则找到当前链表中最后一个，添加之
                _Hash_Link_Info<T>* pLastLink = m_lpTable[nPos];

                while(NULL != pLastLink)
                {
                    if(pLastLink->m_pNext == NULL)
                    {
                        break;
                    }

                    pLastLink = pLastLink->m_pNext;
                }

                //从对象池中获取一个新对象
                _Hash_Table_Cell<T>* pData = m_objHashPool.Create();

                if(NULL == pData)
                {
                    return -1;
                }

                _Hash_Link_Info<T>* pLink = m_objHashLinkPool.Create();

                if(NULL == pLink)
                {
                    return -1;
                }
                else
                {
                    sprintf_safe(pData->m_pKey, pData->m_sKeyLen, "%s", pKey);
                    pData->m_pValue = pValue;
                    pLink->m_pData = pData;
                    pLink->m_pPerv = pLastLink;

                    if (NULL != pLastLink)
                    {
                        pLastLink->m_pNext = pLink;
                    }
                }
            }

            return nPos;
        }
    }

    //获得一个已有映射对应数值
    T* Get_Hash_Box_Data(const char* pKey)
    {
        if(NULL == m_lpTable)
        {
            //没有找到共享内存
            return NULL;
        }

        int32 nPos = GetHashTablePos(pKey, EM_SELECT);

        if(-1 == nPos)
        {
            //没有找到
            return NULL;
        }
        else
        {
            T* pT = NULL;
            _Hash_Link_Info<T>* pLastLink = m_lpTable[nPos];

            while(NULL != pLastLink)
            {
                if(NULL != pLastLink->m_pData && strcmp(pLastLink->m_pData->m_pKey, pKey) == 0)
                {
                    pT = pLastLink->m_pData->m_pValue;
                }

                if(pLastLink->m_pNext == NULL)
                {
                    break;
                }

                pLastLink = pLastLink->m_pNext;
            }

            return pT;
        }
    }

    //清理一个hash数据块
    int32 Del_Hash_Data(const char* pKey)
    {
        return DelHashTablePos(pKey);
    }

private:
    //计算key对应的hash数值
    unsigned long HashString(const char* pKey, int32 nCount)
    {
        unsigned char* pukey = (unsigned char*)pKey;
        uint32 seed = 131; /* 31 131 1313 13131 131313 etc..*/
        uint32 h = 0;

        while (*pukey)
        {
            h = h * seed + (*pukey++);
        }

        h = h & 0x7FFFFFFF;

        if(0 == nCount)
        {
            return h;
        }
        else
        {
            return h % nCount;
        }
    }

    //得到hash指定的位置
    int32 GetHashTablePos(const char* lpszString, EM_HASH_STATE emHashState)
    {
        unsigned long uHashStart = HashString(lpszString, m_objHashPool.Get_Count());

        //获取链表，并比对
        if(NULL == m_lpTable[uHashStart])
        {
            if(EM_INSERT == emHashState)
            {
                return uHashStart;
            }
            else
            {
                return -1;
            }
        }
        else
        {
            _Hash_Link_Info<T>* pLastLink = m_lpTable[uHashStart];

            while(NULL != pLastLink)
            {
                //printf("[CHashTable::GetHashTablePos]pLastLink->m_pData=0x%08x.\n", pLastLink->m_pData);
                //printf("[CHashTable::GetHashTablePos]pLastLink->m_pData->m_pKey=%s.\n", pLastLink->m_pData->m_pKey);
                //printf("[CHashTable::GetHashTablePos]lpszString=%s.\n", lpszString);
                if(NULL != pLastLink->m_pData && strcmp(pLastLink->m_pData->m_pKey, lpszString) == 0)
                {
                    //找到了对应的key,这个数据已经存在
                    if(EM_INSERT == emHashState)
                    {
                        return -1;
                    }
                    else
                    {
                        return uHashStart;
                    }
                }

                pLastLink = pLastLink->m_pNext;
            }

            return uHashStart;
        }

    }

    //删除指定的数据
    int32 DelHashTablePos(const char* lpszString)
    {
        unsigned long uHashStart = HashString(lpszString, m_objHashPool.Get_Count());

        //获取链表，并比对
        if(NULL == m_lpTable[uHashStart])
        {
            return -1;
        }
        else
        {
            _Hash_Link_Info<T>* pLastLink = m_lpTable[uHashStart];

            while(NULL != pLastLink)
            {
                if(NULL != pLastLink->m_pData && strcmp(pLastLink->m_pData->m_pKey, lpszString) == 0)
                {
                    //找到了对应的key,这个数据已经存在
                    if(pLastLink->m_pPerv == NULL)
                    {
                        m_lpTable[uHashStart] = pLastLink->m_pNext;

                        if(NULL != pLastLink->m_pNext)
                        {
                            pLastLink->m_pNext->m_pPerv = NULL;
                        }
                    }
                    else
                    {
                        pLastLink->m_pPerv->m_pNext = pLastLink->m_pNext;

                        if(NULL != pLastLink->m_pNext)
                        {
                            pLastLink->m_pNext->m_pPerv = pLastLink->m_pPerv;
                        }
                    }

                    //回收指针链表和对象
                    m_objHashPool.Delete(pLastLink->m_pData);
                    m_objHashLinkPool.Delete(pLastLink);
                    return 0;
                }

                pLastLink = pLastLink->m_pNext;
            }
        }

        return 0;
    }

private:
    CHashPool<T>         m_objHashPool;      //Hash对象池
    CHashLinkPool<T>     m_objHashLinkPool;  //Hash链表对象池
    _Hash_Link_Info<T>** m_lpTable;          //当前Hash对象数组
    int32                m_nCurrLinkIndex;   //当前链表位置
    char*                m_pBase;            //内存块的基础地址
    char                 m_cIsDelete;        //当前类析构的时候是否回收内存，0是回收，1是不回收。
    EM_HASH_DEBUG        m_emHashDebug;  //是否开启DEBUG开关
};


#endif
