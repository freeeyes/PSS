// MessageService.h
// 处理消息，将消息分派给具体的逻辑处理类去执行
// 今天到了国家图书馆，感觉新馆真的很气派，在这里写代码很有意思。
// 有时候，在朋友们的支持下，PSS才会走的更远。
// 不断的有好建议提出，可见大家都在认真使用中。
// 有时候平凡就是这样，
// add by freeeyes
// 2009-01-29

#include "MessageManager.h"
#ifdef WIN32
#include "ProConnectHandle.h"
#else
#include "ConnectHandler.h"
#endif

bool Delete_CommandInfo(_ClientCommandInfo* pClientCommandInfo)
{
    return pClientCommandInfo->m_u4CurrUsedCount == 0;
}

CMessageManager::CMessageManager(void)
{
    m_u2MaxModuleCount     = 0;
    m_u4MaxCommandCount    = 0;
    m_u4CurrCommandCount   = 0;
    m_u4UpdateIndex        = 0;
}

CMessageManager::~CMessageManager(void)
{
    OUR_DEBUG((LM_INFO, "[CMessageManager::~CMessageManager].\n"));
    //Close();
}

void CMessageManager::Init(uint16 u2MaxModuleCount, uint32 u4MaxCommandCount)
{
    //初始化对象数组
    m_objClientCommandList.Init((int)u4MaxCommandCount);

    //初始化HashTable
    m_objModuleClientList.Init((int)u2MaxModuleCount);

    m_u2MaxModuleCount  = u2MaxModuleCount;
    m_u4MaxCommandCount = u4MaxCommandCount;
    m_u4UpdateIndex     = 0;
}

CClientCommandList* CMessageManager::GetClientCommandList(uint16 u2CommandID)
{
    char szCommandID[10] = {'\0'};
    sprintf_safe(szCommandID, 10, "%d", u2CommandID);
    return m_objClientCommandList.Get_Hash_Box_Data(szCommandID);
}

bool CMessageManager::AddClientCommand(uint16 u2CommandID, CClientCommand* pClientCommand, const char* pModuleName)
{
    if(NULL == pClientCommand)
    {
        OUR_DEBUG((LM_ERROR, "[CMessageManager::AddClientCommand] u2CommandID = %d pClientCommand is NULL.\n", u2CommandID));
        return false;
    }

    char szCommandID[10] = {'\0'};
    sprintf_safe(szCommandID, 10, "%d", u2CommandID);
    CClientCommandList* pClientCommandList = m_objClientCommandList.Get_Hash_Box_Data(szCommandID);

    if(NULL != pClientCommandList)
    {
        //该命令已存在
        _ClientCommandInfo* pClientCommandInfo = pClientCommandList->AddClientCommand(pClientCommand, pModuleName);

        if(NULL != pClientCommandInfo)
        {
            //设置命令绑定ID
            pClientCommandInfo->m_u2CommandID = u2CommandID;

            //添加到模块里面
            string strModule = pModuleName;
            _ModuleClient* pModuleClient = m_objModuleClientList.Get_Hash_Box_Data(strModule.c_str());

            if(NULL == pModuleClient)
            {
                //找不到，创建新的模块信息
                pModuleClient = new _ModuleClient();

                if(NULL != pModuleClient)
                {
                    pModuleClient->m_vecClientCommandInfo.push_back(pClientCommandInfo);
                    m_objModuleClientList.Add_Hash_Data(strModule.c_str(), pModuleClient);
                }
            }
            else
            {
                //找到了，添加进去
                pModuleClient->m_vecClientCommandInfo.push_back(pClientCommandInfo);
            }

            OUR_DEBUG((LM_ERROR, "[CMessageManager::AddClientCommand] u2CommandID = %d Add OK***.\n", u2CommandID));
        }
        else
        {
            return false;
        }
    }
    else
    {
        //该命令尚未添加
        pClientCommandList = new CClientCommandList(u2CommandID);

        if(NULL != pClientCommandList)
        {
            _ClientCommandInfo* pClientCommandInfo = pClientCommandList->AddClientCommand(pClientCommand, pModuleName);

            if(NULL != pClientCommandInfo)
            {
                //设置命令绑定ID
                pClientCommandInfo->m_u2CommandID = u2CommandID;

                //添加到模块里面
                string strModule = pModuleName;
                _ModuleClient* pModuleClient = m_objModuleClientList.Get_Hash_Box_Data(strModule.c_str());

                if(NULL == pModuleClient)
                {
                    //找不到，创建新的模块信息
                    pModuleClient = new _ModuleClient();

                    if(NULL != pModuleClient)
                    {
                        pModuleClient->m_vecClientCommandInfo.push_back(pClientCommandInfo);
                        m_objModuleClientList.Add_Hash_Data(strModule.c_str(), pModuleClient);
                    }
                }
                else
                {
                    //找到了，添加进去
                    pModuleClient->m_vecClientCommandInfo.push_back(pClientCommandInfo);
                }

                m_objClientCommandList.Add_Hash_Data(szCommandID, pClientCommandList);
                m_u4CurrCommandCount++;
                OUR_DEBUG((LM_ERROR, "[CMessageManager::AddClientCommand] u2CommandID = %d Add OK***.\n", u2CommandID));
            }
        }
        else
        {
            return false;
        }
    }

    return true;
}

bool CMessageManager::AddClientCommand(uint16 u2CommandID, CClientCommand* pClientCommand, const char* pModuleName, _ClientIPInfo objListenInfo)
{
    if(NULL == pClientCommand)
    {
        OUR_DEBUG((LM_ERROR, "[CMessageManager::AddClientCommand] u2CommandID = %d pClientCommand is NULL.\n", u2CommandID));
        return false;
    }

    char szCommandID[10] = {'\0'};
    sprintf_safe(szCommandID, 10, "%d", u2CommandID);
    CClientCommandList* pClientCommandList = m_objClientCommandList.Get_Hash_Box_Data(szCommandID);

    if(NULL != pClientCommandList)
    {
        //该命令已存在
        _ClientCommandInfo* pClientCommandInfo = pClientCommandList->AddClientCommand(pClientCommand, pModuleName, objListenInfo);

        if(NULL != pClientCommandInfo)
        {
            //设置命令绑定ID
            pClientCommandInfo->m_u2CommandID = u2CommandID;

            //添加到模块里面
            string strModule = pModuleName;
            _ModuleClient* pModuleClient = m_objModuleClientList.Get_Hash_Box_Data(strModule.c_str());

            if(NULL == pModuleClient)
            {
                //找不到，创建新的模块信息
                pModuleClient = new _ModuleClient();

                if(NULL != pModuleClient)
                {
                    pModuleClient->m_vecClientCommandInfo.push_back(pClientCommandInfo);
                    m_objModuleClientList.Add_Hash_Data(strModule.c_str(), pModuleClient);
                }
            }
            else
            {
                //找到了，添加进去
                pModuleClient->m_vecClientCommandInfo.push_back(pClientCommandInfo);
            }

            OUR_DEBUG((LM_ERROR, "[CMessageManager::AddClientCommand] u2CommandID = %d Add OK***.\n", u2CommandID));
        }
        else
        {
            return false;
        }
    }
    else
    {
        //该命令尚未添加
        pClientCommandList = new CClientCommandList(u2CommandID);

        if(NULL == pClientCommandList)
        {
            return false;
        }

        _ClientCommandInfo* pClientCommandInfo = pClientCommandList->AddClientCommand(pClientCommand, pModuleName, objListenInfo);

        if(NULL != pClientCommandInfo)
        {
            //设置命令绑定ID
            pClientCommandInfo->m_u2CommandID = u2CommandID;

            //添加到模块里面
            string strModule = pModuleName;
            _ModuleClient* pModuleClient = m_objModuleClientList.Get_Hash_Box_Data(strModule.c_str());

            if(NULL == pModuleClient)
            {
                //找不到，创建新的模块信息
                pModuleClient = new _ModuleClient();

                if(NULL != pModuleClient)
                {
                    pModuleClient->m_vecClientCommandInfo.push_back(pClientCommandInfo);
                    m_objModuleClientList.Add_Hash_Data(strModule.c_str(), pModuleClient);
                }
            }
            else
            {
                //找到了，添加进去
                pModuleClient->m_vecClientCommandInfo.push_back(pClientCommandInfo);
            }

            m_objClientCommandList.Add_Hash_Data(szCommandID, pClientCommandList);
            m_u4CurrCommandCount++;
            OUR_DEBUG((LM_ERROR, "[CMessageManager::AddClientCommand] u2CommandID = %d Add OK***.\n", u2CommandID));
        }
        else
        {
            return false;
        }
    }

    return true;
}

bool CMessageManager::DelClientCommand(uint16 u2CommandID, CClientCommand* pClientCommand)
{
    char szCommandID[10] = {'\0'};
    sprintf_safe(szCommandID, 10, "%d", u2CommandID);
    CClientCommandList* pClientCommandList = m_objClientCommandList.Get_Hash_Box_Data(szCommandID);

    if(NULL != pClientCommandList)
    {
        if(true == pClientCommandList->DelClientCommand(pClientCommand))
        {
            SAFE_DELETE(pClientCommandList);
            m_objClientCommandList.Del_Hash_Data(szCommandID);
        }

        OUR_DEBUG((LM_ERROR, "[CMessageManager::DelClientCommand] u2CommandID = %d Del OK.\n", u2CommandID));
        return true;
    }
    else
    {
        OUR_DEBUG((LM_ERROR, "[CMessageManager::DelClientCommand] u2CommandID = %d is not exist.\n", u2CommandID));
        return false;
    }
}

bool CMessageManager::UnloadModuleCommand(const char* pModuleName, uint8 u1LoadState, uint32 u4ThreadCount)
{
    string strModuleName  = pModuleName;
    string strModuleN     = "";
    string strModulePath  = "";
    string strModuleParam = "";
    uint32 u4TmpUpdateIndex = 0;

    //获得重载对象相关参数
    _ModuleInfo* pModuleInfo = App_ModuleLoader::instance()->GetModuleInfo(pModuleName);

    if(NULL != pModuleInfo)
    {
        //获取对象信息
        strModuleN     = pModuleInfo->strModuleName;
        strModulePath  = pModuleInfo->strModulePath;
        strModuleParam = pModuleInfo->strModuleParam;
    }

    _ModuleClient* pModuleClient = m_objModuleClientList.Get_Hash_Box_Data(strModuleName.c_str());

    if(NULL != pModuleClient)
    {
        //从插件目前注册的命令里面找到所有该插件的信息，一个个释放
        for(uint32 u4Index = 0; u4Index < (uint32)pModuleClient->m_vecClientCommandInfo.size(); u4Index++)
        {
            _ClientCommandInfo* pClientCommandInfo = pModuleClient->m_vecClientCommandInfo[u4Index];

            if(NULL != pClientCommandInfo)
            {
                uint16 u2CommandID = pClientCommandInfo->m_u2CommandID;
                CClientCommandList* pCClientCommandList = GetClientCommandList(u2CommandID);

                if(NULL != pCClientCommandList)
                {
                    for(int i = 0; i < pCClientCommandList->GetCount(); i++)
                    {
                        //找到那个唯一
                        if(pCClientCommandList->GetClientCommandIndex(i) == pClientCommandInfo)
                        {
                            //找到了，释放之
                            if (false == pCClientCommandList->DelClientCommand(pClientCommandInfo->m_pClientCommand))
                            {
                                OUR_DEBUG((LM_INFO, "[CMessageManager::UnloadModuleCommand]DelClientCommand(%d) is fail.\n", pClientCommandInfo->m_u2CommandID));
                            }
                            else
                            {
                                OUR_DEBUG((LM_INFO, "[CMessageManager::UnloadModuleCommand]DelClientCommand(%d) is OK.\n", pClientCommandInfo->m_u2CommandID));
                            }
                            
                            //如果该指令下的命令已经不存在，则删除之
                            if(pCClientCommandList->GetCount() == 0)
                            {
                                SAFE_DELETE(pCClientCommandList);
                                char szCommandID[10] = {'\0'};
                                sprintf_safe(szCommandID, 10, "%d", u2CommandID);
                                m_objClientCommandList.Del_Hash_Data(szCommandID);
                                m_u4CurrCommandCount--;
                            }

                            break;
                        }
                    }
                }
            }
        }

        u4TmpUpdateIndex++;

        //卸载插件信息(不在这里卸载，在定时器检测所有工作线程都处理完了，再卸载指定的模块,先将需要卸载的模块放入需要卸载的队列)
        if (false == App_ModuleLoader::instance()->MoveUnloadList(pModuleName, u4TmpUpdateIndex, u4ThreadCount, u1LoadState, strModulePath, strModuleN, strModuleParam))
        {
            OUR_DEBUG((LM_ERROR, "[CMessageManager::UnloadModuleCommand]MoveUnloadList error(%s).\n", pModuleName));
        }
    }

    m_u4UpdateIndex = u4TmpUpdateIndex;
    return true;
}

int CMessageManager::GetCommandCount()
{
    return (int)m_u4CurrCommandCount;
}

void CMessageManager::Close()
{
    //类关闭的清理工作
    vector<CClientCommandList*> vecClientCommandList;
    m_objClientCommandList.Get_All_Used(vecClientCommandList);

    for(int i = 0; i < (int)vecClientCommandList.size(); i++)
    {
        CClientCommandList* pClientCommandList =vecClientCommandList[i];
        SAFE_DELETE(pClientCommandList);
    }

    m_objClientCommandList.Close();

    vector<_ModuleClient*> vecModuleClient;
    m_objModuleClientList.Get_All_Used(vecModuleClient);

    for(int i = 0; i < (int)vecModuleClient.size(); i++)
    {
        _ModuleClient* pModuleClient = vecModuleClient[i];
        SAFE_DELETE(pModuleClient);
    }

    m_objModuleClientList.Close();

    m_u2MaxModuleCount  = 0;
    m_u4MaxCommandCount = 0;

}

CHashTable<_ModuleClient>* CMessageManager::GetModuleClient()
{
    return &m_objModuleClientList;
}

uint32 CMessageManager::GetWorkThreadCount()
{
    return App_MessageServiceGroup::instance()->GetWorkThreadCount();
}

uint32 CMessageManager::GetWorkThreadByIndex(uint32 u4Index)
{
    return App_MessageServiceGroup::instance()->GetWorkThreadIDByIndex(u4Index);
}

NAMESPACE::uint16 CMessageManager::GetMaxCommandCount()
{
    return m_u4MaxCommandCount;
}

uint32 CMessageManager::GetUpdateIndex()
{
    return m_u4UpdateIndex;
}

CHashTable<CClientCommandList>* CMessageManager::GetHashCommandList()
{
    return &m_objClientCommandList;
}

