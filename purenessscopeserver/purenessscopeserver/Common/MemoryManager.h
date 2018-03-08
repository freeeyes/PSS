#ifndef _MEMORYMANAGER_H_
#define _MEMORYMANAGER_H_

#include "ObjectArrayList.h"
#include "HashTable.h"
#include "define.h"
using namespace std;

template<class TYPE, class ACE_LOCK>
class CMemoryManager 
{
private:
    template<class TYPE>
    class CMemoryObject 
    {
    public:
        CMemoryObject(void)
        {
            ACE_OS::memset(m_szUsedFileName, 0, MAX_BUFF_100);
            m_u4UsedLine = 0;
        }

        ~CMemoryObject(void)
        {
        }

        void SetCreateInfo(const char* pFileName, uint32 u4Line)
        {
            sprintf_safe(m_szUsedFileName, MAX_BUFF_100, "%s", pFileName);
            m_u4UsedLine = u4Line;
        }

        char* GetCreateFileName()
        {
            return m_szUsedFileName;
        }

        uint32 GetCreateLine()
        {
            return m_u4UsedLine;
        }

        TYPE* GetObject()
        {
            return &m_Object;
        }

        void Clear()
        {
            m_szUsedFileName[0] = '\0';
            m_u4UsedLine        = 0;
        }

    private:
        TYPE    m_Object;                       //对象
        char    m_szUsedFileName[MAX_BUFF_100]; //创建此对象的文件名
        uint32  m_u4UsedLine;                   //创建此对象的行号
    };

public:
    CMemoryManager(void)
    {
        m_bRecord    = false；
    }

    ~CMemoryManager(void)
    {
    }

    void SetCreateFlag(bool bRecord)
    {
        m_bRecord = bRecord;
    }

    void Init(uint32 u4Count)
    {
        Close();

        //初始化Hash表
        m_objObjectList.Init(u4Count);
        m_objHashObjectList.Init((int32)u4Count);
        m_objType2ObjectList.Init((int32)u4Count);

        uint32 u4Size = m_objHashObjectList.Get_Count();

        for(uint32 i = 0; i < u4Size; i++)
        {
            CMemoryObject<TYPE>* pMemoryObject = m_objObjectList.GetObject(i);

            if(NULL != pMemoryObject)
            {
                //添加到Hash数组里面
                int32 nHashPos = m_objHashObjectList.Add_Hash_Data_By_Key_Unit32(pMemoryObject->GetObject(), pMemoryObject);

                if(-1 == nHashPos)
                {
                    OUR_DEBUG((LM_INFO, "[CMemoryManager::Init]mAdd_Hash_Data_By_Key_Unit32 error.\n"));
                }
            }
        }
    }

    void GetAllObjectForInit(vector<TYPE*>& objAllObject)
    {
        objAllObject.clear();
        vector<CMemoryObject<TYPE>*> vecMemoryObject;
        m_objHashObjectList.Get_All_Used(vecMemoryObject);

        for(int i = 0; i < (int)vecMemoryObject.size(); i++)
        {
            CMemoryObject<TYPE>* pMemoryObject = vecMemoryObject[i];

            objAllObject.push_back(pMemoryObject->GetObject());
        }
    }

    void Close()
    {
        //清理所有已存在的指针
        m_objHashObjectList.Close();
        m_objType2ObjectList.Close();
    }

    uint32 GetUsedCount()
    {
        return (uint32)m_objHashObjectList.Get_Count() - m_objHashObjectList.Get_Used_Count();
    }

    uint32 GetFreeCount()
    {
        return (uint32)m_objHashObjectList.Get_Used_Count();
    }

    TYPE* Create(const char* pFileName = __FILE__, uint32 u4Line = __LINE__)
    {
        ACE_Guard<ACE_LOCK> WGuard(m_ThreadLock);
        CMemoryObject<TYPE>* pMemoryObject = dynamic_cast<CMemoryObject<TYPE>*>(m_objHashObjectList.Pop_Uint32());

        if (NULL != pMemoryObject)
        {
            if (m_bRecord)
            {
                pMemoryObject->SetCreateInfo(pFileName, u4Line);
            }
            
            int32 nHashPos = m_objType2ObjectList.Add_Hash_Data_By_Key_Unit32(pMemoryObject->GetObject(), pMemoryObject);

            if(-1 == nHashPos)
            {
                OUR_DEBUG((LM_INFO, "[CMemoryManager::Create]mAdd_Hash_Data_By_Key_Unit32 error.\n"));
            }
            return pMemoryObject->GetObject();
        }
        else 
        {
            return NULL;
        }
    }

    bool Delete(TYPE* pObject)
    {
        ACE_Guard<ACE_LOCK> WGuard(m_ThreadLock);
        if(NULL == pObject)
        {
            return false;
        }

        CMemoryObject<TYPE>* pMemoryObject = m_objType2ObjectList.Add_Hash_Data_By_Key_Unit32(pObject);
        if(NULL != pMemoryObject)
        {
            pMemoryObject->Clear();

            int nPos = m_objType2ObjectList.Del_Hash_Data_By_Unit32(pObject);

            if(0 < nPos)
            {
                bool blState = m_objHashObjectList.Push_Uint32(pObject, pMemoryObject);

                if(false == blState)
                {
                    OUR_DEBUG((LM_INFO, "[CMemoryManager::Delete]pObject=%s(0x%08x).\n", pObject, pMemoryObject));
                }
                else
                {
                    //OUR_DEBUG((LM_INFO, "[CBuffPacketManager::Delete]szPacketID=%s(0x%08x) nPos=%d.\n", szPacketID, pBuff, nPos));
                }

                return true;
            }
            else
            {
                sprintf_safe(m_szError, MAX_BUFF_500, "[CMemoryManager::Delete] pObject[0x%08x] is not find.", pObject);
                return false;
            }
        }
        else
        {
            sprintf_safe(m_szError, MAX_BUFF_500, "[CMemoryManager::Delete] pObject[0x%08x] is not find.", pObject);
            return false;
        }
    }

    void GetCreateInfoList(vector<_Packet_Create_Info>& objCreateList)
    {
        ACE_Guard<ACE_LOCK> WGuard(m_ThreadLock);
        objCreateList.clear();

        //输出所有正在使用的对象创建信息
        uint32 u4Count = m_objHashObjectList.GetCount();

        for (uint32 i = 0; i < u4Count; i++)
        {
            CMemoryObject<TYPE>* pMemoryObject = m_objHashObjectList.GetObject(i);

            int nCreateLine       = pMemoryObject->GetCreateLine();
            char* pCreateFileName = pMemoryObject->GetCreateFileName();

            if (strlen(pCreateFileName) > 0 && nCreateLine > 0)
            {
                bool blIsFind = false;

                //正在使用的对象，进行统计
                for (int j = 0; j < (int)objCreateList.size(); j++)
                {
                    if (0 == ACE_OS::strcmp(pCreateFileName, objCreateList[j].m_szCreateFileName)
                        && nCreateLine == objCreateList[j].m_u4Line)
                    {
                        blIsFind = true;
                        objCreateList[j].m_u4Count++;
                        break;
                    }
                }

                if (false == blIsFind)
                {
                    _Packet_Create_Info obj_Packet_Create_Info;
                    sprintf_safe(obj_Packet_Create_Info.m_szCreateFileName, MAX_BUFF_100, "%s", pCreateFileName);
                    obj_Packet_Create_Info.m_u4Line  = nCreateLine;
                    obj_Packet_Create_Info.m_u4Count = 1;
                    objCreateList.push_back(obj_Packet_Create_Info);
                }
            }
        }
    }

private:
    ACE_LOCK                                  m_ThreadLock;
    CObjectArrayList<CMemoryObject<TYPE>>     m_objObjectList;          //对象数组
    CHashTable<CMemoryObject<TYPE>>           m_objHashObjectList;      //存储空闲对象指针的hash列表
    CHashTable<CMemoryObject<TYPE>>           m_objType2ObjectList;     //存储TYPE类型到MemoryObject映射关系的hash列表
    bool    m_bRecord;                      //是否记录创建信息
};

#endif //_MEMORYMANAGER_H_
