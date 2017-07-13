#include "define.h"

#include <map>

using namespace std;

//这个类是记录所有的Http连接状态的
//add by freeeyes

//最大未解密包的长度,以及缓冲块的大小，如果长度比这个大，请修改这里
#define MAX_ENCRYPTLENGTH 50*MAX_BUFF_1024
//最大解密数据包长度，如果最大数据包比这个大，则扩展这个值
#define MAX_DECRYPTLENGTH 50*MAX_BUFF_1024

//记录websokcet的连接状态，如果是初次连接，则设置为WEBSOCKET_STATE_HANDIN
struct _HttpInfo
{
    uint32                  m_u4ConnectID;                         //链接的ID
    char                    m_szData[MAX_ENCRYPTLENGTH];           //当前缓冲中数据的长度
    uint32                  m_u4DataLength;                        //当前缓冲块中的数据长度

    _HttpInfo()
    {
        Init();
    }

    void Init()
    {
        m_u4ConnectID = 0;

        ACE_OS::memset(m_szData, 0, MAX_DECRYPTLENGTH);
        m_u4DataLength     = 0;
    }
};

//声明一个内存管理池,管理所有的_WebSocketInfo指针
class CHttpInfoPool
{
public:
    CHttpInfoPool(uint32 u4Size = 1000)
    {
        for(uint32 i = 0; i < u4Size; i++)
        {
            _HttpInfo* pHttpInfo = new _HttpInfo();

            if(NULL != pHttpInfo)
            {
                //添加到Free map里面
                mapPacket::iterator f = m_mapPacketFree.find(pHttpInfo);

                if(f == m_mapPacketFree.end())
                {
                    m_mapPacketFree.insert(mapPacket::value_type(pHttpInfo, pHttpInfo));
                }
            }
        }
    }

    ~CHttpInfoPool()
    {
        Close();
    }

    void Close()
    {
        //清理所有已存在的指针
        for(mapPacket::iterator itorFreeB = m_mapPacketFree.begin(); itorFreeB != m_mapPacketFree.end(); itorFreeB++)
        {
            _HttpInfo* pHttpInfo = (_HttpInfo* )itorFreeB->second;
            SAFE_DELETE(pHttpInfo);
        }

        for(mapPacket::iterator itorUsedB = m_mapPacketUsed.begin(); itorUsedB != m_mapPacketUsed.end(); itorUsedB++)
        {
            _HttpInfo* pHttpInfo = (_HttpInfo* )itorUsedB->second;
            SAFE_DELETE(pHttpInfo);
        }

        m_mapPacketFree.clear();
        m_mapPacketUsed.clear();

    }

    _HttpInfo* Create()
    {
        ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

        //如果free池中已经没有了，则添加到free池中。
        if(m_mapPacketFree.size() <= 0)
        {
            _HttpInfo* pHttpInfo = new _HttpInfo();

            if(pHttpInfo != NULL)
            {
                //添加到Free map里面
                mapPacket::iterator f = m_mapPacketFree.find(pHttpInfo);

                if(f == m_mapPacketFree.end())
                {
                    m_mapPacketFree.insert(mapPacket::value_type(pHttpInfo, pHttpInfo));
                }
            }
            else
            {
                return NULL;
            }
        }

        //从free池中拿出一个,放入到used池中
        mapPacket::iterator itorFreeB = m_mapPacketFree.begin();
        _HttpInfo* pHttpInfo = (_HttpInfo* )itorFreeB->second;
        m_mapPacketFree.erase(itorFreeB);
        //添加到used map里面
        mapPacket::iterator f = m_mapPacketUsed.find(pHttpInfo);

        if(f == m_mapPacketUsed.end())
        {
            m_mapPacketUsed.insert(mapPacket::value_type(pHttpInfo, pHttpInfo));
        }

        return (_HttpInfo* )pHttpInfo;
    }

    bool Delete(_HttpInfo* pHttpInfo)
    {
        ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

        _HttpInfo* pBuff = (_HttpInfo* )pHttpInfo;

        if(NULL == pBuff)
        {
            return false;
        }

        pHttpInfo->Init();

        mapPacket::iterator f = m_mapPacketUsed.find(pBuff);

        if(f != m_mapPacketUsed.end())
        {
            m_mapPacketUsed.erase(f);

            //添加到Free map里面
            mapPacket::iterator f = m_mapPacketFree.find(pBuff);

            if(f == m_mapPacketFree.end())
            {
                m_mapPacketFree.insert(mapPacket::value_type(pBuff, pBuff));
            }
        }

        return true;
    }

private:
    typedef map<_HttpInfo*, _HttpInfo*> mapPacket;
    mapPacket                  m_mapPacketUsed;                       //已使用的
    mapPacket                  m_mapPacketFree;                       //没有使用的
    ACE_Recursive_Thread_Mutex m_ThreadWriteLock;
};

//管理所有的连接类
class CHttpInfoManager
{
public:
    CHttpInfoManager() {}
    ~CHttpInfoManager()
    {
        Close();
    }

    void Close()
    {
        m_mapHttpInfo.clear();
    }

    //插入一个新的数据连接状态
    bool Insert(uint32 u4ConnectID)
    {
        mapHttpInfo::iterator f = m_mapHttpInfo.find(u4ConnectID);

        if(f != m_mapHttpInfo.end())
        {
            OUR_DEBUG((LM_ERROR, "[CPacketParse::Connect]ConnectID=%d is exist.\n"));
            return false;
        }

        _HttpInfo* pHttpInfo = m_objHttpInfoPool.Create();
        pHttpInfo->m_u4ConnectID  = u4ConnectID;
        m_mapHttpInfo.insert(mapHttpInfo::value_type(u4ConnectID, pHttpInfo));

        return true;
    }

    //删除一个新的数据库连接状态
    void Delete(uint32 u4ConnectID)
    {
        mapHttpInfo::iterator f = m_mapHttpInfo.find(u4ConnectID);

        if(f != m_mapHttpInfo.end())
        {
            _HttpInfo* pHttpInfo = (_HttpInfo* )f->second;
            m_objHttpInfoPool.Delete(pHttpInfo);
            m_mapHttpInfo.erase(f);
        }
    }

    //查找指定的连接状态
    _HttpInfo* GetHttpInfo(uint32 u4ConnectID)
    {
        mapHttpInfo::iterator f = m_mapHttpInfo.find(u4ConnectID);

        if(f == m_mapHttpInfo.end())
        {
            //没有找到
            return NULL;
        }
        else
        {
            //找到了
            return (_HttpInfo* )f->second;
        }
    }

private:
    typedef map<uint32, _HttpInfo*> mapHttpInfo;

    //考虑到大量并发下的数据验证，采用map可能优于vector
    mapHttpInfo m_mapHttpInfo;

    //考虑到大量连接频繁建立和消除，采用内存池机制管理_WebSocketInfo*
    CHttpInfoPool m_objHttpInfoPool;
};

typedef ACE_Singleton<CHttpInfoManager, ACE_Null_Mutex> App_HttpInfoManager;
