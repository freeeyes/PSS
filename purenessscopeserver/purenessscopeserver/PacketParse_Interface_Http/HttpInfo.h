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

//管理所有的连接类
class CHttpInfoManager
{
public:
    CHttpInfoManager()
    {
        m_u4Count = 1000;

        //初始化_HttpInfo HashTable
        m_objHttpInfoList.Init(m_u4Count);
    }

    ~CHttpInfoManager()
    {
        Close();
    }

    void Close()
    {
        vector<_HttpInfo*> vecHttpInfo;
        m_objHttpInfoList.Get_All_Used(vecHttpInfo);

        for(int i = 0; i < (int)vecHttpInfo.size(); i++)
        {
            _HttpInfo* ptrHttpInfo = vecHttpInfo[i];

            if(NULL != ptrHttpInfo)
            {
                SAFE_DELETE(ptrHttpInfo);
            }
        }

        m_objHttpInfoList.Close();
    }

    //插入一个新的数据连接状态
    bool Insert(uint32 u4ConnectID)
    {
        uint32 u4Index = u4ConnectID%m_u4Count;
        char szIndex[10];
        sprintf(szIndex, "%d", u4Index);
        //查找并添加
        _HttpInfo* ptrHttpInfo = m_objHttpInfoList.Get_Hash_Box_Data(szIndex);

        if(NULL != ptrHttpInfo)
        {
            //如果已经存在
            ptrHttpInfo->Init();
            return true;
        }
        else
        {
            //添加新的命令统计信息
            ptrHttpInfo = new _HttpInfo();

            if(NULL != ptrHttpInfo)
            {
                m_objHttpInfoList.Add_Hash_Data(szIndex, ptrHttpInfo);
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    //删除一个新的数据库连接状态
    void Delete(uint32 u4ConnectID)
    {
        uint32 u4Index = u4ConnectID%m_u4Count;
        char szIndex[10];
        sprintf(szIndex, "%d", u4Index);
        //查找并添加
        _HttpInfo* ptrHttpInfo = m_objHttpInfoList.Get_Hash_Box_Data(szIndex);

        if(NULL != ptrHttpInfo)
        {
            //如果已经存在
            ptrHttpInfo->Init();
            return;
        }
    }

    //查找指定的连接状态
    _HttpInfo* GetHttpInfo(uint32 u4ConnectID)
    {
        uint32 u4Index = u4ConnectID%m_u4Count;
        char szIndex[10];
        sprintf(szIndex, "%d", u4Index);
        //查找并添加
        _HttpInfo* ptrHttpInfo = m_objHttpInfoList.Get_Hash_Box_Data(szIndex);

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
    CHashTable<_HttpInfo> m_objHttpInfoList;
    uint32 m_u4Count;
};

typedef ACE_Singleton<CHttpInfoManager, ACE_Null_Mutex> App_HttpInfoManager;
