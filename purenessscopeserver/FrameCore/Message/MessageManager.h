#ifndef _MESSAGEMANAGER_H
#define _MESSAGEMANAGER_H

#pragma once

#include "IMessageManager.h"
#include "Message.h"
#include "LoadModule.h"
#include "HashTable.h"
#include <unordered_map>

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
    shared_ptr<CClientCommand> m_pClientCommand  = nullptr;            //��ǰ����ָ��
    uint16          m_u2CommandID                = 0;                  //��ǰ�����Ӧ��ID
    string          m_strModuleName;                                   //����ģ������
    ACE_Date_Time   m_dtLoadTime;                                      //��ǰ�������ʱ��
    _ClientIPInfo   m_objListenIPInfo;                                 //��ǰ�����IP�Ͷ˿���ڣ�Ĭ�������е�ǰ�˿�

    _ClientCommandInfo() = default;
};

//ģ���_ClientCommandInfo֮��Ķ�Ӧ��ϵ
class _ModuleClient
{
public:
    vector<shared_ptr<_ClientCommandInfo>> m_vecClientCommandInfo;    //һ��ģ�����ж�Ӧ�����б�
};

//һ����Ϣ���Զ�Ӧһ��CClientCommand*�����飬����Ϣ�����ʱ��ַ�����Щ������
class CClientCommandList
{
private:
    uint16  m_u2CommandID = 0;                      //��ǰ��������ID
    uint16  m_u2Timeout   = 0;                      //��ǰ����ʱʱ��
    using vecClientCommandList = vector<shared_ptr<_ClientCommandInfo>>;
    vecClientCommandList m_vecClientCommandList;

public:
    CClientCommandList() = default;

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
        m_vecClientCommandList.clear();
    }

    shared_ptr<_ClientCommandInfo> AddClientCommand(shared_ptr<CClientCommand> pClientCommand, const char* pMuduleName, const _ClientIPInfo* pListenInfo)
    {
        auto pClientCommandInfo = std::make_shared<_ClientCommandInfo>();
        pClientCommandInfo->m_pClientCommand   = pClientCommand;
        pClientCommandInfo->m_strModuleName    = pMuduleName;

        if (nullptr != pListenInfo)
        {
            pClientCommandInfo->m_objListenIPInfo = (*pListenInfo);
        }

        m_vecClientCommandList.emplace_back(pClientCommandInfo);

        return pClientCommandInfo;
    }

    //�������Ϊtrue��֤�������Ϣ�Ѿ�û�ж�Ӧ���Ҫ��ΧHash�г�ȥ
    bool DelClientCommand(shared_ptr<CClientCommand> pClientCommand)
    {
        for(vecClientCommandList::iterator b = m_vecClientCommandList.begin(); b!= m_vecClientCommandList.end(); ++b)
        {
            auto pClientCommandInfo = *b;

            if(nullptr != pClientCommandInfo 
                && pClientCommand == pClientCommandInfo->m_pClientCommand)
            {
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
    shared_ptr<_ClientCommandInfo> GetClientCommandIndex(int nIndex)
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

using hashmapClientCommandList = unordered_map<uint16, shared_ptr<CClientCommandList>>;
using hashmapModuleClientList = unordered_map<string, shared_ptr<_ModuleClient>>;

class CMessageManager : public IMessageManager
{
public:
    CMessageManager(void) = default;

    void Init(uint16 u2MaxModuleCount, uint32 u4MaxCommandCount);

    void Close();

    bool AddClientCommand(uint16 u2CommandID, shared_ptr<CClientCommand> pClientCommand, const char* pModuleName, _ClientIPInfo* pListenInfo) final;   //ע������
    bool AddClientCommand(uint16 u2CommandID, shared_ptr<CClientCommand> pClientCommand, const char* pModuleName) final;   //ע������
    bool DelClientCommand(uint16 u2CommandID, shared_ptr<CClientCommand> pClientCommand) final;                            //ж������

    bool UnloadModuleCommand(const char* pModuleName, uint8 u1LoadState, uint32 u4ThreadCount);  //ж��ָ��ģ���¼���u1State= 1 ж�أ�2 ����

    int  GetCommandCount() const;                                      //�õ���ǰע������ĸ���
    shared_ptr<CClientCommandList> GetClientCommandExist(uint16 u2CommandID);     //�õ���ǰָ���Ƿ��Ѵ��ڵ�ǰ�б�

    hashmapModuleClientList GetModuleClient() const;                      //��������ģ���ע��������Ϣ

    uint32 GetWorkThreadCount() final;
    uint32 GetWorkThreadByIndex(uint32 u4Index) final;

    uint16 GetMaxCommandCount() const;
    uint32 GetUpdateIndex() const;

    hashmapClientCommandList GetHashCommandList() const;              //�õ���ǰHashCommandList�ĸ���

private:
    bool AddClientCommand_Ex(uint16 u2CommandID, shared_ptr<CClientCommand> pClientCommand, const char* pModuleName, const _ClientIPInfo* pListenInfo);   //ע������
    void DeleteCommandByModule(shared_ptr<_ClientCommandInfo> pClientCommandInfo);                                                                  //���������б�ɾ��ָ��������
    void Add_ClientCommandList(const xmlCommandsTimeout::_CommandsTimeout* pCommandTimeout, 
        shared_ptr<CClientCommandList> 
        pClientCommandList, 
        uint16 u2CommandID, 
        shared_ptr<CClientCommand> pClientCommand, 
        const char* pModuleName, 
        const _ClientIPInfo* pListenInfo);

    uint32                         m_u4UpdateIndex       = 0;               //��ǰ����ID
    uint32                         m_u4MaxCommandCount   = 0;               //���������е�����
    uint32                         m_u4CurrCommandCount  = 0;               //��ǰ��Ч������
    uint16                         m_u2MaxModuleCount    = 0;               //ģ��������������
    hashmapClientCommandList       m_objClientCommandList;                  //����ֶ�Ӧ������
    hashmapModuleClientList        m_objModuleClientList;                   //����ģ���Ӧ����Ϣ
    ACE_Recursive_Thread_Mutex     m_ThreadWriteLock;                       //������

};

using App_MessageManager = ACE_Singleton<CMessageManager, ACE_Null_Mutex>;
#endif
