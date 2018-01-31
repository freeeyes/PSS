#include "define.h"
#include "HashTable.h"

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
    uint16                  m_u2Index;                             //当前索引ID
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
        m_u2Index          = 0;
    }

    void Clear()
    {
        m_u4ConnectID = 0;
        ACE_OS::memset(m_szData, 0, MAX_DECRYPTLENGTH);
        m_u4DataLength = 0;
    }
};

//管理所有的连接类
class CHttpInfoManager
{
public:
    CHttpInfoManager()
    {
        m_u4Count = 1000;

        //初始化_HttpInfo HashTable
        m_objHttpInfoUsedList.Init(m_u4Count);

        //初始化HashTable
        m_objHttpInfoFreeList.Init(m_u4Count);

        for(int i = 0; i < (int)m_u4Count; i++)
        {
            _HttpInfo* ptrHttpInfo = new _HttpInfo();

            if (NULL != ptrHttpInfo)
            {
                ptrHttpInfo->m_u2Index = i;
                char szHandlerID[10] = { '\0' };
                sprintf_safe(szHandlerID, 10, "%d", i);
                m_objHttpInfoFreeList.Add_Hash_Data(szHandlerID, ptrHttpInfo);
            }
            else
            {
                return;
            }
        }
    }

    ~CHttpInfoManager()
    {
        Close();
    }

    void Close()
    {
        vector<_HttpInfo*> vecHttpInfo;
        m_objHttpInfoFreeList.Get_All_Used(vecHttpInfo);

        for(int i = 0; i < (int)vecHttpInfo.size(); i++)
        {
            _HttpInfo* ptrHttpInfo = vecHttpInfo[i];

            if(NULL != ptrHttpInfo)
            {
                SAFE_DELETE(ptrHttpInfo);
            }
        }

        m_objHttpInfoFreeList.Close();

        m_objHttpInfoUsedList.Get_All_Used(vecHttpInfo);

        for(int i = 0; i < (int)vecHttpInfo.size(); i++)
        {
            _HttpInfo* ptrHttpInfo = vecHttpInfo[i];

            if(NULL != ptrHttpInfo)
            {
                SAFE_DELETE(ptrHttpInfo);
            }
        }

        m_objHttpInfoUsedList.Close();
    }

    //插入一个新的数据连接状态
    bool Insert(uint32 u4ConnectID)
    {
        uint32 u4Index = u4ConnectID;
        char szIndex[10];
        sprintf(szIndex, "%d", u4Index);
        //查找并添加
        _HttpInfo* ptrHttpInfo = Create();

        if(NULL != ptrHttpInfo)
        {
            //如果已经存在
            m_objHttpInfoUsedList.Add_Hash_Data(szIndex, ptrHttpInfo);
            ptrHttpInfo->Clear();
            return true;
        }
        else
        {
            OUR_DEBUG((LM_INFO, "[Insert]ptrHttpInfo is NULL.\n"));
            return false;
        }
    }

    //删除一个新的数据库连接状态
    void Delete(uint32 u4ConnectID)
    {
        uint32 u4Index = u4ConnectID;
        char szIndex[10];
        sprintf(szIndex, "%d", u4Index);
        //查找并添加
        _HttpInfo* ptrHttpInfo = m_objHttpInfoUsedList.Get_Hash_Box_Data(szIndex);

        if(NULL != ptrHttpInfo)
        {
            if (-1 == m_objHttpInfoUsedList.Del_Hash_Data(szIndex))
            {
                OUR_DEBUG((LM_ERROR, "[Delete]Del_Hash_Data ConnectID=%d fail.\n", szIndex));
            }

            ptrHttpInfo->Clear();
            Delete(ptrHttpInfo);
            return;
        }
    }

    //查找指定的连接状态
    _HttpInfo* GetHttpInfo(uint32 u4ConnectID)
    {
        ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);
        uint32 u4Index = u4ConnectID;
        char szIndex[10];
        sprintf(szIndex, "%d", u4Index);
        //查找并添加
        _HttpInfo* ptrHttpInfo = m_objHttpInfoUsedList.Get_Hash_Box_Data(szIndex);

        if(NULL != ptrHttpInfo)
        {
            return ptrHttpInfo;
        }
        else
        {
            return NULL;
        }
    }

private:
    _HttpInfo* Create()
    {
        ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

        //在Hash表中弹出一个已使用的数据
        _HttpInfo* ptrHttpInfo = m_objHttpInfoFreeList.Pop();
        return ptrHttpInfo;
    }

    bool Delete(_HttpInfo* ptrHttpInfo)
    {
        ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

        if(NULL == ptrHttpInfo)
        {
            return false;
        }

        char szHandlerID[10] = {'\0'};
        sprintf_safe(szHandlerID, 10, "%d", ptrHttpInfo->m_u2Index);

        bool blState = m_objHttpInfoFreeList.Push(szHandlerID, ptrHttpInfo);

        if(false == blState)
        {
            OUR_DEBUG((LM_INFO, "[Delete]szHandlerID=%s(0x%08x).\n", szHandlerID, ptrHttpInfo));
            //m_objHashHandleList.Add_Hash_Data(szHandlerID, pObject);
        }

        return true;
    }
private:
    ACE_Recursive_Thread_Mutex    m_ThreadWriteLock;
    CHashTable<_HttpInfo> m_objHttpInfoFreeList;
    CHashTable<_HttpInfo> m_objHttpInfoUsedList;
    uint32 m_u4Count;
};

typedef ACE_Singleton<CHttpInfoManager, ACE_Null_Mutex> App_HttpInfoManager;
