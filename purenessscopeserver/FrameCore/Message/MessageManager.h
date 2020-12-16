#ifndef _MESSAGEMANAGER_H
#define _MESSAGEMANAGER_H

#pragma once

#include "IMessageManager.h"
#include "Message.h"
#include "LoadModule.h"
#include "HashTable.h"
#include <unordered_map>

//这里修改一下，如果一个命令对应一个模块是有限制的。
//这里改为一个信令可以对应任意数量的处理模块，这样就比较好了。
//用hash数组可以获得累积更高的性能提升
//add by freeeyes

//命令订阅者者的格式
class _ClientCommandInfo
{
public:
    uint32          m_u4Count                    = 0;                  //当前命令被调用的次数
    uint32          m_u4TimeCost                 = 0;                  //当前命令总时间消耗
    uint32          m_u4CurrUsedCount            = 0;                  //当前正在使用的引用次数
    shared_ptr<CClientCommand> m_pClientCommand  = nullptr;            //当前命令指针
    uint16          m_u2CommandID                = 0;                  //当前命令对应的ID
    string          m_strModuleName;                                   //所属模块名称
    ACE_Date_Time   m_dtLoadTime;                                      //当前命令加载时间
    _ClientIPInfo   m_objListenIPInfo;                                 //当前允许的IP和端口入口，默认是所有当前端口

    _ClientCommandInfo() = default;
};

//模块和_ClientCommandInfo之间的对应关系
class _ModuleClient
{
public:
    vector<shared_ptr<_ClientCommandInfo>> m_vecClientCommandInfo;    //一个模块所有对应命令列表
};

//一个消息可以对应一个CClientCommand*的数组，当消息到达的时候分发给这些订阅者
class CClientCommandList
{
private:
    uint16  m_u2CommandID = 0;                      //当前处理信令ID
    uint16  m_u2Timeout   = 0;                      //当前处理超时时间
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

    //如果返回为true，证明这个消息已经没有对应项，需要外围Hash中除去
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

    //得到个数
    int GetCount() const
    {
        return (int)m_vecClientCommandList.size();
    }

    //得到指定位置的指针
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

    bool AddClientCommand(uint16 u2CommandID, shared_ptr<CClientCommand> pClientCommand, const char* pModuleName, _ClientIPInfo* pListenInfo) final;   //注册命令
    bool AddClientCommand(uint16 u2CommandID, shared_ptr<CClientCommand> pClientCommand, const char* pModuleName) final;   //注册命令
    bool DelClientCommand(uint16 u2CommandID, shared_ptr<CClientCommand> pClientCommand) final;                            //卸载命令

    bool UnloadModuleCommand(const char* pModuleName, uint8 u1LoadState, uint32 u4ThreadCount);  //卸载指定模块事件，u1State= 1 卸载，2 重载

    int  GetCommandCount() const;                                      //得到当前注册命令的个数
    shared_ptr<CClientCommandList> GetClientCommandExist(uint16 u2CommandID);     //得到当前指令是否已存在当前列表

    hashmapModuleClientList GetModuleClient() const;                      //返回所有模块绑定注册命令信息

    uint32 GetWorkThreadCount() final;
    uint32 GetWorkThreadByIndex(uint32 u4Index) final;

    uint16 GetMaxCommandCount() const;
    uint32 GetUpdateIndex() const;

    hashmapClientCommandList GetHashCommandList() const;              //得到当前HashCommandList的副本

private:
    bool AddClientCommand_Ex(uint16 u2CommandID, shared_ptr<CClientCommand> pClientCommand, const char* pModuleName, const _ClientIPInfo* pListenInfo);   //注册命令
    void DeleteCommandByModule(shared_ptr<_ClientCommandInfo> pClientCommandInfo);                                                                  //遍历命令列表，删除指定的命令
    void Add_ClientCommandList(const xmlCommandsTimeout::_CommandsTimeout* pCommandTimeout, 
        shared_ptr<CClientCommandList> 
        pClientCommandList, 
        uint16 u2CommandID, 
        shared_ptr<CClientCommand> pClientCommand, 
        const char* pModuleName, 
        const _ClientIPInfo* pListenInfo);

    uint32                         m_u4UpdateIndex       = 0;               //当前更新ID
    uint32                         m_u4MaxCommandCount   = 0;               //最大命令池中的数量
    uint32                         m_u4CurrCommandCount  = 0;               //当前有效命令数
    uint16                         m_u2MaxModuleCount    = 0;               //模块池里面的最大个数
    hashmapClientCommandList       m_objClientCommandList;                  //命令持对应的数组
    hashmapModuleClientList        m_objModuleClientList;                   //加载模块对应的信息
    ACE_Recursive_Thread_Mutex     m_ThreadWriteLock;                       //数据锁

};

using App_MessageManager = PSS_singleton<CMessageManager>;
#endif
