#ifndef _MESSAGEMANAGER_H
#define _MESSAGEMANAGER_H

#pragma once

#include "IMessageManager.h"
#include "Message.h"
#include "LoadModule.h"
#include "HashTable.h"

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
    CClientCommand* m_pClientCommand             = NULL;               //当前命令指针
    uint16          m_u2CommandID                = 0;                  //当前命令对应的ID
    char            m_szModuleName[MAX_BUFF_100] = {'\0'};             //所属模块名称
    ACE_Date_Time   m_dtLoadTime;                                      //当前命令加载时间
    _ClientIPInfo   m_objListenIPInfo;                                 //当前允许的IP和端口入口，默认是所有当前端口

    _ClientCommandInfo()
    {
    }
};

//模块和_ClientCommandInfo之间的对应关系
class _ModuleClient
{
public:
    vector<_ClientCommandInfo*> m_vecClientCommandInfo;    //一个模块所有对应命令列表
};

//一个消息可以对应一个CClientCommand*的数组，当消息到达的时候分发给这些订阅者
class CClientCommandList
{
private:
    uint16  m_u2CommandID = 0;                      //当前处理信令ID
    uint16  m_u2Timeout   = 0;                      //当前处理超时时间
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

    uint16 GetCommandTimeout()
    {
        return m_u2Timeout;
    }

    void SetCommandID(uint16 u2CommandID)
    {
        m_u2CommandID = u2CommandID;
    }

    uint16 GetCommandID()
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

        if (NULL != pListenInfo)
        {
            pClientCommandInfo->m_objListenIPInfo = (*pListenInfo);
        }

        m_vecClientCommandList.push_back(pClientCommandInfo);

        return pClientCommandInfo;
    }

    //如果返回为true，证明这个消息已经没有对应项，需要外围Hash中除去
    bool DelClientCommand(const CClientCommand* pClientCommand)
    {
        for(vecClientCommandList::iterator b = m_vecClientCommandList.begin(); b!= m_vecClientCommandList.end(); ++b)
        {
            const _ClientCommandInfo* pClientCommandInfo = (*b);

            if(NULL != pClientCommandInfo && pClientCommand == pClientCommandInfo->m_pClientCommand)
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

    //得到个数
    int GetCount()
    {
        return (int)m_vecClientCommandList.size();
    }

    //得到指定位置的指针
    _ClientCommandInfo* GetClientCommandIndex(int nIndex)
    {
        if(nIndex >= (int)m_vecClientCommandList.size())
        {
            return NULL;
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

    virtual bool AddClientCommand(uint16 u2CommandID, CClientCommand* pClientCommand, const char* pModuleName, _ClientIPInfo* pListenInfo);   //注册命令
    virtual bool AddClientCommand(uint16 u2CommandID, CClientCommand* pClientCommand, const char* pModuleName);   //注册命令
    virtual bool DelClientCommand(uint16 u2CommandID, CClientCommand* pClientCommand);                            //卸载命令

    bool UnloadModuleCommand(const char* pModuleName, uint8 u1LoadState, uint32 u4ThreadCount);  //卸载指定模块事件，u1State= 1 卸载，2 重载

    int  GetCommandCount();                                            //得到当前注册命令的个数
    CClientCommandList* GetClientCommandExist(uint16 u2CommandID);     //得到当前指令是否已存在当前列表

    CHashTable<_ModuleClient>* GetModuleClient();                      //返回所有模块绑定注册命令信息

    virtual uint32 GetWorkThreadCount();
    virtual uint32 GetWorkThreadByIndex(uint32 u4Index);

    uint16 GetMaxCommandCount();
    uint32 GetUpdateIndex();

    CHashTable<CClientCommandList>* GetHashCommandList();              //得到当前HashCommandList的副本

private:
    bool AddClientCommand_Ex(uint16 u2CommandID, CClientCommand* pClientCommand, const char* pModuleName, const _ClientIPInfo* pListenInfo);   //注册命令
    void DeleteCommandByModule(const _ClientCommandInfo* pClientCommandInfo);                                                                  //遍历命令列表，删除指定的命令

    uint32                         m_u4UpdateIndex       = 0;               //当前更新ID
    uint32                         m_u4MaxCommandCount   = 0;               //最大命令池中的数量
    uint32                         m_u4CurrCommandCount  = 0;               //当前有效命令数
    uint16                         m_u2MaxModuleCount    = 0;               //模块池里面的最大个数
    CHashTable<CClientCommandList> m_objClientCommandList;                  //命令持对应的数组
    CHashTable<_ModuleClient>      m_objModuleClientList;                   //加载模块对应的信息
    ACE_Recursive_Thread_Mutex     m_ThreadWriteLock;                       //数据锁

};

typedef ACE_Singleton<CMessageManager, ACE_Null_Mutex> App_MessageManager;
#endif
