#include "define.h"
#include "HashTable.h"

#include <map>

using namespace std;

//������Ǽ�¼���е�Http����״̬��
//add by freeeyes

//���δ���ܰ��ĳ���,�Լ������Ĵ�С��������ȱ���������޸�����
#define MAX_ENCRYPTLENGTH 50*MAX_BUFF_1024
//���������ݰ����ȣ����������ݰ������������չ���ֵ
#define MAX_DECRYPTLENGTH 50*MAX_BUFF_1024
//Http�ӿڶ�������
#define MAX_HTTPINFO_COUNT 1000

//��¼websokcet������״̬������ǳ������ӣ�������ΪWEBSOCKET_STATE_HANDIN
struct _HttpInfo
{
    uint16                  m_u2Index;                             //��ǰ����ID
    uint32                  m_u4ConnectID;                         //���ӵ�ID
    char                    m_szData[MAX_ENCRYPTLENGTH];           //��ǰ���������ݵĳ���
    uint32                  m_u4DataLength;                        //��ǰ������е����ݳ���

    _HttpInfo()
    {
        Init();
    }

    void Init()
    {
        m_u4ConnectID      = 0;
        m_u4DataLength     = 0;
        m_u2Index          = 0;
        ACE_OS::memset(m_szData, 0, MAX_DECRYPTLENGTH);
    }

    void Clear()
    {
        m_u4ConnectID  = 0;
        m_u4DataLength = 0;
        ACE_OS::memset(m_szData, 0, MAX_DECRYPTLENGTH);
    }
};

//�������е�������
class CHttpInfoManager
{
public:
    CHttpInfoManager()
    {
        Init();
    }

    ~CHttpInfoManager()
    {
        Close();
    }

    void Init(uint32 u4InfoCount = MAX_HTTPINFO_COUNT)
    {
        m_u4Count = u4InfoCount;

        //��ʼ��_HttpInfo HashTable
        m_objHttpInfoUsedList.Init(m_u4Count);

        //��ʼ��HashTable
        m_objHttpInfoFreeList.Init(m_u4Count);

        for (int i = 0; i < (int)m_u4Count; i++)
        {
            _HttpInfo* ptrHttpInfo = new _HttpInfo();

            if (NULL != ptrHttpInfo)
            {
                ptrHttpInfo->m_u2Index = i;
                char szHandlerID[10] = { '\0' };
                sprintf_safe(szHandlerID, 10, "%d", i);

                if (-1 == m_objHttpInfoFreeList.Add_Hash_Data(szHandlerID, ptrHttpInfo))
                {
                    OUR_DEBUG((LM_INFO, "[Init]ptrHttpInfo Add_Hash_Data is error.\n"));
                    SAFE_DELETE(ptrHttpInfo);
                }
            }
            else
            {
                OUR_DEBUG((LM_INFO, "[Init]ptrHttpInfo new error.\n"));
                return;
            }
        }
    }

    void Close()
    {
        vector<_HttpInfo*> vecHttpInfo;
        m_objHttpInfoFreeList.Get_All_Used(vecHttpInfo);

        for(int i = 0; i < (int)vecHttpInfo.size(); i++)
        {
            SAFE_DELETE(vecHttpInfo[i]);
        }

        m_objHttpInfoFreeList.Close();

        m_objHttpInfoUsedList.Get_All_Used(vecHttpInfo);

        for(int i = 0; i < (int)vecHttpInfo.size(); i++)
        {
            SAFE_DELETE(vecHttpInfo[i]);
        }

        m_objHttpInfoUsedList.Close();
    }

    //����һ���µ���������״̬
    bool Insert(uint32 u4ConnectID)
    {
        uint32 u4Index = u4ConnectID;
        char szIndex[DEF_HASH_KEY_SIZE];
        sprintf_safe(szIndex, DEF_HASH_KEY_SIZE, "%d", u4Index);
        //�ӿ����HttpInfo�����б����ҵ�һ������Ķ���
        _HttpInfo* ptrHttpInfo = Get_Free_List();

        if(NULL != ptrHttpInfo)
        {
            //��������ӵ���ʹ�ö����б�
            if (-1 == m_objHttpInfoUsedList.Add_Hash_Data(szIndex, ptrHttpInfo))
            {
                OUR_DEBUG((LM_INFO, "[Init]m_objHttpInfoUsedList Add_Hash_Data is error.\n"));
                return false;
            }
            else
            {
                return true;
            }
        }
        else
        {
            OUR_DEBUG((LM_INFO, "[Insert]ptrHttpInfo is NULL.\n"));
            return false;
        }
    }

    //ɾ��һ���µ����ݿ�����״̬
    void Delete(uint32 u4ConnectID)
    {
        uint32 u4Index = u4ConnectID;
        char szIndex[DEF_HASH_KEY_SIZE];
        sprintf_safe(szIndex, DEF_HASH_KEY_SIZE, "%d", u4Index);
        //�����Ƿ���ڵ�ǰ��������ʹ�õĶ���
        _HttpInfo* ptrHttpInfo = m_objHttpInfoUsedList.Get_Hash_Box_Data(szIndex);

        if(NULL != ptrHttpInfo)
        {
            if (-1 == m_objHttpInfoUsedList.Del_Hash_Data(szIndex))
            {
                OUR_DEBUG((LM_ERROR, "[Delete]Del_Hash_Data ConnectID=%d fail.\n", szIndex));
            }

            ptrHttpInfo->Clear();

            //�ع鵽�����б����
            Set_Free_List(ptrHttpInfo);
            return;
        }
        else
        {
            OUR_DEBUG((LM_ERROR, "[Delete]ConnectID=%d is no find.\n", szIndex));
        }
    }

    //����ָ��������״̬
    _HttpInfo* GetHttpInfo(uint32 u4ConnectID)
    {
        ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);
        uint32 u4Index = u4ConnectID;
        char szIndex[DEF_HASH_KEY_SIZE];
        sprintf_safe(szIndex, DEF_HASH_KEY_SIZE, "%d", u4Index);
        //���Ҳ����
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
    _HttpInfo* Get_Free_List()
    {
        ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

        //��Hash���е���һ����ʹ�õ�����
        _HttpInfo* ptrHttpInfo = m_objHttpInfoFreeList.Pop();
        return ptrHttpInfo;
    }

    bool Set_Free_List(_HttpInfo* ptrHttpInfo)
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
