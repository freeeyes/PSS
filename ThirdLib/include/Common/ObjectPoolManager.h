#ifndef _OBJECTPOOLMANAGER_H_
#define _OBJECTPOOLMANAGER_H_

#include "ObjectArrayList.h"
#include "HashTable.h"
#include "CreateInfo.h"
#include "ace/FILE_Addr.h"
#include "ace/FILE_Connector.h"
#include "ace/FILE_IO.h"

template<class TYPE, class ACE_LOCK>
class CObjectPoolManager
{
private:
    typedef void(*Init_Callback)(int, TYPE*);    //�����߼�����ָ��
    typedef void(*Close_Callback)(int, TYPE*);   //�����߼�����ָ��

public:
    CObjectPoolManager(void)
    {
        m_blTagCreateInfo = false;
    }

    virtual ~CObjectPoolManager(void)
    {
    }

    void SetCreateFlag(bool blTagCreateInfo)
    {
        m_blTagCreateInfo = blTagCreateInfo;
    }

    void Init(uint32 u4Count, Init_Callback fn_Init_Callback)
    {
        Close();

        //��ʼ��Hash��
        m_objObjectList.Init(u4Count);
        m_objCreateInfoList.Init(u4Count);
        m_objHashObjectList.Init((int32)u4Count);

        uint32 u4Size = m_objHashObjectList.Get_Count();

        for(uint32 i = 0; i < u4Size; i++)
        {
            TYPE* pObject = m_objObjectList.GetObject(i);

            if(NULL != pObject)
            {
                //ִ�к�����ʼ������
                fn_Init_Callback(i, pObject);

                //��ӵ�Hash��������
                int32 nHashPos = m_objHashObjectList.Add_Hash_Data_By_Key_Unit32(i, pObject);

                if(-1 == nHashPos)
                {
                    OUR_DEBUG((LM_INFO, "[CObjectPoolManager::Init]mAdd_Hash_Data_By_Key_Unit32 error.\n"));
                }
            }
        }
    }

    void Close_Object(Close_Callback fn_Close_Callback)
    {
        //�������е������߼�������ִ������ǰ������������
        uint32 u4Size = m_objHashObjectList.Get_Count();

        for (uint32 i = 0; i < u4Size; i++)
        {
            TYPE* pObject = m_objObjectList.GetObject(i);

            if (NULL != pObject)
            {
                fn_Close_Callback(i, pObject);
            }
        }

        //���������Ѵ��ڵ�ָ��
        m_objHashObjectList.Close();
    }

    void Close()
    {
        //���������Ѵ��ڵ�ָ��
        m_objHashObjectList.Close();
    }

    uint32 GetUsedCount()
    {
        return (uint32)m_objHashObjectList.Get_Count() - m_objHashObjectList.Get_Used_Count();
    }

    uint32 GetFreeCount()
    {
        return (uint32)m_objHashObjectList.Get_Used_Count();
    }

    TYPE* Create(const char* pFileName, uint32 u4Line)
    {
        ACE_Guard<ACE_LOCK> WGuard(m_ThreadLock);
        uint32 u4Pos = 0;
        TYPE* pObject = dynamic_cast<TYPE*>(m_objHashObjectList.Pop_Uint32(u4Pos));

        if (NULL != pObject)
        {
            if (true == m_blTagCreateInfo)
            {
                //��¼���õĴ���λ��
                m_objCreateInfoList.GetObject(u4Pos)->SetCreateInfo(pFileName, u4Line);
            }

            return pObject;
        }
        else
        {
            vector<_Object_Create_Info> objCreateList;
            GetCreateInfoList(objCreateList);

            int n4Size = (int)objCreateList.size();

            for (int i = 0; i < n4Size; i++)
            {
                OUR_DEBUG((LM_INFO, "[CObjectPoolManager::Create]FileName=%s,m_u4Line=%d,m_u4Count=%d.\n",
                           objCreateList[i].m_szCreateFileName,
                           objCreateList[i].m_u4Line,
                           objCreateList[i].m_u4Count));
            }

            return NULL;
        }
    }

    bool Delete(uint32 u4Pos, TYPE* pObject)
    {
        ACE_Guard<ACE_LOCK> WGuard(m_ThreadLock);

        int32 nPos = m_objHashObjectList.Push_Uint32(u4Pos, pObject);

        if (-1 == nPos)
        {
            OUR_DEBUG((LM_INFO, "[CObjectPoolManager::Delete]szPacketID=%d(0x%08x).\n", u4Pos, pObject));
            return false;
        }

        m_objCreateInfoList.GetObject(u4Pos)->ClearCreateInfo();
        return true;
    }

    void GetCreateInfoList(vector<_Object_Create_Info>& objCreateList)
    {
        ACE_Guard<ACE_LOCK> WGuard(m_ThreadLock);
        objCreateList.clear();

        if (true == m_blTagCreateInfo)
        {
            //�����������ʹ�õĶ��󴴽���Ϣ
            uint32 u4Count = m_objCreateInfoList.GetCount();

            for (uint32 i = 0; i < u4Count; i++)
            {
                uint32 u4CreateLine = m_objCreateInfoList.GetObject(i)->GetCreateLine();
                char* pCreateFileName = m_objCreateInfoList.GetObject(i)->GetCreateFileName();

                if (strlen(pCreateFileName) > 0 && u4CreateLine > 0)
                {
                    bool blIsFind = false;

                    //����ʹ�õĶ��󣬽���ͳ��
                    for (int j = 0; j < (int)objCreateList.size(); j++)
                    {
                        if (0 == ACE_OS::strcmp(pCreateFileName, objCreateList[j].m_szCreateFileName)
                            && u4CreateLine == objCreateList[j].m_u4Line)
                        {
                            blIsFind = true;
                            objCreateList[j].m_u4Count++;
                            break;
                        }
                    }

                    if (false == blIsFind)
                    {
                        _Object_Create_Info obj_Packet_Create_Info;
                        sprintf_safe(obj_Packet_Create_Info.m_szCreateFileName, MAX_BUFF_100, "%s", pCreateFileName);
                        obj_Packet_Create_Info.m_u4Line = u4CreateLine;
                        obj_Packet_Create_Info.m_u4Count = 1;
                        objCreateList.push_back(obj_Packet_Create_Info);
                    }
                }
            }
        }
    }

    void OutputCreateInfo()
    {
        vector<_Object_Create_Info> objCreateList;
        GetCreateInfoList(objCreateList);
        int n4Size = (int)objCreateList.size();
        string strFileName;
        string strClassInfo = typeid(TYPE).name();

#ifndef WIN32
        char* pTmp = (char*)strClassInfo.c_str();

        if(('4' == pTmp[0])||('7' == pTmp[0]))
        {
            strClassInfo = strClassInfo.erase(0,1);
        }
        else if(('P' == pTmp[0])&&(('4' == pTmp[1])||('7' == pTmp[1])))
        {
            strClassInfo = strClassInfo.erase(0,2);
        }
        else if(('S' == pTmp[0])&&('t' == pTmp[1])&&('9' == pTmp[2]))
        {
            strClassInfo = strClassInfo.erase(0,3);
        }

        strFileName = strClassInfo + "_CreateInfo.log";
#else

        if(0 == strClassInfo.find("class ", 0))
        {
            strClassInfo = strClassInfo.erase(0,strlen("class "));
        }

        if((strClassInfo.size()-1) == strClassInfo.find_last_of("*"))
        {
            strClassInfo = strClassInfo.erase(strClassInfo.size()-2,2);
        }

        strFileName = strClassInfo + "_CreateInfo.log";
#endif

        if(n4Size > 0)
        {
            ACE_FILE_Connector fConnector;
            ACE_FILE_IO ioFile;
            ACE_FILE_Addr fAddr(strFileName.c_str());

            if (fConnector.connect(ioFile, fAddr) != -1)
            {
                time_t tNow = time(NULL);
                struct tm* tmNow = ACE_OS::localtime(&tNow);

                for (int i = 0; i < n4Size; i++)
                {
                    char szLog[MAX_BUFF_1024] = { '\0' };

                    sprintf_safe(szLog,
                                 MAX_BUFF_1024,
                                 "[%04d-%02d-%02d %02d:%02d:%02d]FileName=%s,m_u4Line=%d,m_u4Count=%d",
                                 tmNow->tm_year + 1900,
                                 tmNow->tm_mon + 1,
                                 tmNow->tm_mday,
                                 tmNow->tm_hour,
                                 tmNow->tm_min,
                                 tmNow->tm_sec,
                                 objCreateList[i].m_szCreateFileName,
                                 objCreateList[i].m_u4Line,
                                 objCreateList[i].m_u4Count);

                    if ((int)ioFile.send (szLog, strlen(szLog)) != (int)strlen(szLog))
                    {
                        OUR_DEBUG((LM_INFO, "[CObjectPoolManager::OutputCreateInfo]Write filename:%s Error.\n",strFileName.c_str()));
                    }
                }

                ioFile.close();
            }
            else
            {
                OUR_DEBUG((LM_INFO, "[CObjectPoolManager::OutputCreateInfo]Open filename:%s Error.\n",strFileName.c_str()));
            }
        }
    }

private:
    ACE_LOCK                      m_ThreadLock;
    CObjectArrayList<TYPE>        m_objObjectList;          //��������
    CObjectArrayList<CCreateInfo> m_objCreateInfoList;      //��������
    CHashTable<TYPE>              m_objHashObjectList;      //�洢���ж���ָ���hash�б�
    bool                          m_blTagCreateInfo;        //�Ƿ��¼������Ϣ
};

#endif //_OBJECTPOOLMANAGER_H_