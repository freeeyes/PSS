#ifndef DEFINE_H
#define DEFINE_H

#include "define.h"
#include <ace/Basic_Types.h>
#include <ace/Time_Value.h>
#include <ace/OS_NS_sys_time.h>
#include <ace/OS_NS_string.h>

#include <map>

using namespace std;

typedef ACE_UINT32 ID_t;

#define INVALID_ID     0
#define INVALID_OFFSET 0

enum EM_ReaderInitState
{
    READERINITSTATE = 0,   //共享内存未初始化数据
    READERINITSTATED,      //共享内存已经初始化数据
};

enum EM_CACHED_USE_STATE
{
    CACHEDUSED = 0,         //该cached正在被别的进程使用
    CACHEDUNUSED,           //该cached没有被别的进程使用
};

enum EM_CheckState
{
    CHECKS_HIT = 0,      //命中（缓冲自检程序已经搜到当前共享内存数据）
    CHECKS_UNHIT,        //无命中（缓冲自检程序没有搜到当前共享内存数据）
};

enum EM_LRUReturn
{
    LRU_NEED_CHECK = 0,   //需要进行LRU淘汰
    LRU_UNNEED_CHECK,     //不需要进行LRU淘汰
};

//共享内存头
struct _SMHeader
{
    ID_t           m_HeaderID;          //头ID
    uint32         m_u4DataOffset;      //首地址偏移，也就是数据体T*的地址
    ACE_Time_Value m_tvUpdate;          //数据被读取处理时间

    _SMHeader()
    {
        m_HeaderID     = INVALID_ID;
        m_u4DataOffset = INVALID_OFFSET;
    }
};

//用于缓冲当前状态的数据结构
struct _CacheBlock
{
private:
    EM_CheckState       m_emState;           //当前维护状态，对应EM_CheckState
    EM_CACHED_USE_STATE m_emIsUsed;          //在线标记，对应EM_CACHED_USE_STATE状态
    bool                m_blDelete;          //删除标记为，Watch进程维护这个标记，false为正在使用，true为该数据已经删除
    uint32              m_u4CacheIndex;      //当前数据块的块ID

public:
    _CacheBlock()
    {
        m_emState       = CHECKS_HIT;
        m_blDelete      = false;
        m_emIsUsed      = CACHEDUNUSED;
        m_u4CacheIndex  = 0;
    }

    //设置块ID
    void SetCacheIndex(uint32 u4CacheIndex)
    {
        m_u4CacheIndex = u4CacheIndex;
    }

    //获得块ID
    uint32 GetCacheIndex()
    {
        return m_u4CacheIndex;
    }

    //被数据源命中
    void SetHit()
    {
        m_blDelete      = false;
        m_emState       = CHECKS_HIT;
    }

    //没有被数据源命中
    void SetUnHit()
    {
        m_blDelete     = true;
        m_emState      = CHECKS_UNHIT;
    }

    //设置删除状态
    void SetDelete(bool blDelete)
    {
        m_blDelete = blDelete;
    }

    //设置共享内存没有被逻辑进程使用
    void SetUsed()
    {
        m_emIsUsed = CACHEDUSED;
    }

    //设置共享内存没有被逻辑进程使用
    void SetUnUsed()
    {
        m_emIsUsed = CACHEDUNUSED;
    }

    //得到删除状态
    bool GetDelete()
    {
        return m_blDelete;
    }

    //得到是否命中状态
    EM_CheckState GetCheckState()
    {
        return m_emState;
    }

    //设置命中状态
    void SetCheckState(EM_CheckState objCheckState)
    {
        m_emState = objCheckState;
    }
};

//用于LRU的算法类
//共享内存数据淘汰规则
template<class K>
class CCachedLRUList
{
public:
    CCachedLRUList()
    {
        m_u4CheckIndex     = 0;
        m_u4MaxCachedCount = 0;
    };

    ~CCachedLRUList()
    {
    };

    void Set_Lru_Max_Count(uint32 u4MaxCachedCount)
    {
        m_u4MaxCachedCount = u4MaxCachedCount;
    }

    //检测是否需要开始LRU，如果需要则返回最后要删除的
    bool Check_Cached_Lru(K& lrukey)
    {
        if(m_mapKey.size() < m_u4MaxCachedCount)
        {
            return false;
        }
        else
        {
            //找到Lru应该弃掉的key
            lrukey = (K&)m_mapTimeStamp.begin()->second;
            return true;
        }
    };

    //从队列里删除指定的key
    bool Delete_Cached_Lru(K lrukey)
    {
        typename mapKey::iterator f = m_mapKey.find(lrukey);

        if(f == m_mapKey.end())
        {
            return false;
        }

        uint32 u4Index = (uint32)f->second;
        m_mapKey.erase(f);

        typename mapTimeStamp::iterator ft = m_mapTimeStamp.find(u4Index);

        if(ft == m_mapTimeStamp.end())
        {
            return false;
        }

        m_mapTimeStamp.erase(ft);

        //删除Index和key之间的对应关系
        typename mapKey2Index::iterator fi = m_mapKey2Index.find(lrukey);

        if(fi != m_mapKey2Index.end())
        {
            uint32 u4CachedIndex = (uint32)fi->second;

            //删除对应关系
            m_mapKey2Index.erase(fi);

            //删除另一个map
            typename mapIndex2Key::iterator fii = m_mapIndex2Key.find(u4CachedIndex);

            if(fii != m_mapIndex2Key.end())
            {
                m_mapIndex2Key.erase(fii);
            }
        }

        return true;
    };

    //添加一个key，如果已经存在则提升到队列最前面去
    EM_LRUReturn Add_Cached_Lru(K lrukey, uint32 u4CachedIndex)
    {
        typename mapKey::iterator f = m_mapKey.find(lrukey);

        if(f == m_mapKey.end())
        {
            //如果是新key,判断是否需要进行LRU检测
            if(m_mapKey.size() > m_u4MaxCachedCount)
            {
                return LRU_NEED_CHECK;
            }

            //添加新的key
            uint32 u4Index = m_u4CheckIndex++;
            m_mapKey.insert(typename mapKey::value_type(lrukey, u4Index));
            m_mapTimeStamp.insert(typename mapTimeStamp::value_type(u4Index, lrukey));

            m_mapKey2Index.insert(typename mapKey2Index::value_type(lrukey, u4CachedIndex));
            m_mapIndex2Key.insert(typename mapIndex2Key::value_type(u4CachedIndex, lrukey));

            return LRU_UNNEED_CHECK;
        }
        else
        {
            //如果key已经存在，则更新key的时间戳，也就是u4Index;
            uint32& u4CurrIndex = (uint32&)f->second;

            //删除旧的key，添加新的key
            typename mapTimeStamp::iterator ft = m_mapTimeStamp.find(u4CurrIndex);
            m_mapTimeStamp.erase(ft);

            u4CurrIndex = m_u4CheckIndex++;
            m_mapTimeStamp.insert(typename mapTimeStamp::value_type(u4CurrIndex, lrukey));

            return LRU_UNNEED_CHECK;
        }
    }

    //根据实际情况更新Lru的对应index和key的列表
    bool Reload_Cached_IndexList(K lrukey, K& lruBeforekey, uint32 u4CachedIndex)
    {
        //寻找之前的Index对应的key并修改之
        typename mapIndex2Key::iterator fii = m_mapIndex2Key.find(u4CachedIndex);

        if(fii == m_mapIndex2Key.end())
        {
            return false;
        }

        lruBeforekey = (K)fii->second;

        typename mapKey2Index::iterator fi = m_mapKey2Index.find(lruBeforekey);

        if(fi != m_mapKey2Index.end())
        {
            m_mapKey2Index.erase(fi);

            //添加新的key对应关系
            m_mapKey2Index.insert(typename mapKey2Index::value_type(lrukey, u4CachedIndex));
        }

        m_mapIndex2Key[u4CachedIndex] = lrukey;

        //测试代码
        //DisPlay_Index2Key();
        //DisPlay_Key2Index();

        return true;
    }

    //获得指定位置的Index对应信息
    bool Get_Cached_KeyByIndex(uint32 u4CachedIndex, K& lrukey)
    {
        typename mapIndex2Key::iterator fii = m_mapIndex2Key.find(u4CachedIndex);

        if(fii == m_mapIndex2Key.end())
        {
            return false;
        }
        else
        {
            lrukey = (K )fii->second;
            return true;
        }
    }

private:
    //用于测试显示数据映射内容
    void DisPlay_Index2Key()
    {
        OUR_DEBUG((LM_INFO, "[DisPlay_Index2Key]*****Begin DisPlay*****\n"));

        for(typename mapIndex2Key::iterator b = m_mapIndex2Key.begin(); b != m_mapIndex2Key.end(); b++)
        {
            OUR_DEBUG((LM_INFO, "[DisPlay_Index2Key]key=%s.\n", ((string)(b->second)).c_str()));
        }

        OUR_DEBUG((LM_INFO, "[DisPlay_Index2Key]*****End DisPlay*****\n"));
    }

    //用于测试显示数据映射内容
    void DisPlay_Key2Index()
    {
        OUR_DEBUG((LM_INFO, "[DisPlay_Key2Index]*****Begin DisPlay*****\n"));

        for(typename mapKey2Index::iterator b = m_mapKey2Index.begin(); b != m_mapKey2Index.end(); b++)
        {
            OUR_DEBUG((LM_INFO, "[DisPlay_Key2Index]key=%s.\n", ((string)(b->first)).c_str()));
        }

        OUR_DEBUG((LM_INFO, "[DisPlay_Key2Index]*****End DisPlay*****\n"));
    }

private:
    typedef map<K, uint32> mapKey;       //key与Version对应关系
    typedef map<uint32, K> mapTimeStamp; //Version和key的对应关系

    typedef map<K, uint32> mapKey2Index; //记录Index和key的关系
    typedef map<uint32, K> mapIndex2Key; //记录key和Index的关系

    mapKey       m_mapKey;
    mapTimeStamp m_mapTimeStamp;

    mapKey2Index m_mapKey2Index;
    mapIndex2Key m_mapIndex2Key;

    uint32       m_u4CheckIndex;
    uint32       m_u4MaxCachedCount;
};

#endif
