#ifndef _MESSAGEMANAGER_H
#define _MESSAGEMANAGER_H

#pragma once

#include "IMessageManager.h"
#include "Message.h"
#include "LoadModule.h"
#include "HashTable.h"

//�����޸�һ�£����һ�������Ӧһ��ģ���������Ƶġ�
//�����Ϊһ��������Զ�Ӧ���������Ĵ���ģ�飬�����ͱȽϺ��ˡ�
//��hash������Ի���ۻ����ߵ���������
//add by freeeyes

//��������ߵĸ�ʽ
class _ClientCommandInfo
{
public:
    uint32          m_u4Count                    = 0;                  //��ǰ������õĴ���
    uint32          m_u4TimeCost                 = 0;                  //��ǰ������ʱ������
    uint32          m_u4CurrUsedCount            = 0;                  //��ǰ����ʹ�õ����ô���
    CClientCommand* m_pClientCommand             = nullptr;               //��ǰ����ָ��
    uint16          m_u2CommandID                = 0;                  //��ǰ�����Ӧ��ID
    char            m_szModuleName[MAX_BUFF_100] = {'\0'};             //����ģ������
    ACE_Date_Time   m_dtLoadTime;                                      //��ǰ�������ʱ��
    _ClientIPInfo   m_objListenIPInfo;                                 //��ǰ�����IP�Ͷ˿���ڣ�Ĭ�������е�ǰ�˿�

    _ClientCommandInfo()
    {
    }
};

//ģ���_ClientCommandInfo֮��Ķ�Ӧ��ϵ
class _ModuleClient
{
public:
    vector<_ClientCommandInfo*> m_vecClientCommandInfo;    //һ��ģ�����ж�Ӧ�����б�
};

//һ����Ϣ���Զ�Ӧһ��CClientCommand*�����飬����Ϣ�����ʱ��ַ�����Щ������
class CClientCommandList
{
private:
    uint16  m_u2CommandID = 0;                      //��ǰ��������ID
    uint16  m_u2Timeout   = 0;                      //��ǰ����ʱʱ��
    typedef vector<_ClientCommandInfo*> vecClientCommandList;
    vecClientCommandList m_vecClientCommandList;

public:
    CClientCommandList()
    {
    }

    explicit CClientCommandList(uint16 u2CommandID) : m_u2CommandID(u2CommandID)
    {
    }

    void SetCommandTimeout(uint16 u2Timeout)
    {
        m_u2Timeout = u2Timeout;
    }

    uint16 GetCommandTimeout() const
    {
        return m_u2Timeout;
    }

    void SetCommandID(uint16 u2CommandID)
    {
        m_u2CommandID = u2CommandID;
    }

    uint16 GetCommandID() const
    {
        return m_u2CommandID;
    }

    void Close()
    {
        uint32 u4Size = (uint32)m_vecClientCommandList.size();

        for(uint32 i = 0; i < u4Size; i++)
        {
            SAFE_DELETE(m_vecClientCommandList[i]);
        }

        m_vecClientCommandList.clear();
    }

    _ClientCommandInfo* AddClientCommand(CClientCommand* pClientCommand, const char* pMuduleName, const _ClientIPInfo* pListenInfo)
    {
        _ClientCommandInfo* pClientCommandInfo = new _ClientCommandInfo();
        pClientCommandInfo->m_pClientCommand  = pClientCommand;

        sprintf_safe(pClientCommandInfo->m_szModuleName, MAX_BUFF_100, "%s", pMuduleName);

        if (nullptr != pListenInfo)
        {
            pClientCommandInfo->m_objListenIPInfo = (*pListenInfo);
        }

        m_vecClientCommandList.push_back(pClientCommandInfo);

        return pClientCommandInfo;
    }

    //�������Ϊtrue��֤�������Ϣ�Ѿ�û�ж�Ӧ���Ҫ��ΧHash�г�ȥ
    bool DelClientCommand(const CClientCommand* pClientCommand)
    {
        for(vecClientCommandList::iterator b = m_vecClientCommandList.begin(); b!= m_vecClientCommandList.end(); ++b)
        {
            const _ClientCommandInfo* pClientCommandInfo = (*b);

            if(nullptr != pClientCommandInfo && pClientCommand == pClientCommandInfo->m_pClientCommand)
            {
                SAFE_DELETE(pClientCommandInfo);
                m_vecClientCommandList.erase(b);
                break;
            }
        }

        if((int)m_vecClientCommandList.size() == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    //�õ�����
    int GetCount() const
    {
        return (int)m_vecClientCommandList.size();
    }

    //�õ�ָ��λ�õ�ָ��
    _ClientCommandInfo* GetClientCommandIndex(int nIndex)
    {
        if(nIndex >= (int)m_vecClientCommandList.size())
        {
            return nullptr;
        }
        else
        {
            return m_vecClientCommandList[nIndex];
        }
    }
};

class CMessageManager : public IMessageManager
{
public:
    CMessageManager(void);

    void Init(uint16 u2MaxModuleCount, uint32 u4MaxCommandCount);

    void Close();

    virtual bool AddClientCommand(uint16 u2CommandID, CClientCommand* pClientCommand, const char* pModuleName, _ClientIPInfo* pListenInfo);   //ע������
    virtual bool AddClientCommand(uint16 u2CommandID, CClientCommand* pClientCommand, const char* pModuleName);   //ע������
    virtual bool DelClientCommand(uint16 u2CommandID, CClientCommand* pClientCommand);                            //ж������

    bool UnloadModuleCommand(const char* pModuleName, uint8 u1LoadState, uint32 u4ThreadCount);  //ж��ָ��ģ���¼���u1State= 1 ж�أ�2 ����

    int  GetCommandCount() const;                                      //�õ���ǰע������ĸ���
    CClientCommandList* GetClientCommandExist(uint16 u2CommandID);     //�õ���ǰָ���Ƿ��Ѵ��ڵ�ǰ�б�

    CHashTable<_ModuleClient>* GetModuleClient();                      //��������ģ���ע��������Ϣ

    virtual uint32 GetWorkThreadCount();
    virtual uint32 GetWorkThreadByIndex(uint32 u4Index);

    uint16 GetMaxCommandCount() const;
    uint32 GetUpdateIndex() const;

    CHashTable<CClientCommandList>* GetHashCommandList();              //�õ���ǰHashCommandList�ĸ���

private:
    bool AddClientCommand_Ex(uint16 u2CommandID, CClientCommand* pClientCommand, const char* pModuleName, const _ClientIPInfo* pListenInfo);   //ע������
    void DeleteCommandByModule(const _ClientCommandInfo* pClientCommandInfo);                                                                  //���������б�ɾ��ָ��������
    void Add_ClientCommandList(const xmlCommandsTimeout::_CommandsTimeout* pCommandTimeout, CClientCommandList* pClientCommandList, uint16 u2CommandID, CClientCommand* pClientCommand, const char* pModuleName, const _ClientIPInfo* pListenInfo);

    uint32                         m_u4UpdateIndex       = 0;               //��ǰ����ID
    uint32                         m_u4MaxCommandCount   = 0;               //���������е�����
    uint32                         m_u4CurrCommandCount  = 0;               //��ǰ��Ч������
    uint16                         m_u2MaxModuleCount    = 0;               //ģ��������������
    CHashTable<CClientCommandList> m_objClientCommandList;                  //����ֶ�Ӧ������
    CHashTable<_ModuleClient>      m_objModuleClientList;                   //����ģ���Ӧ����Ϣ
    ACE_Recursive_Thread_Mutex     m_ThreadWriteLock;                       //������

};

typedef ACE_Singleton<CMessageManager, ACE_Null_Mutex> App_MessageManager;
#endif
