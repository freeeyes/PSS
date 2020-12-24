// MessageService.h
// 处理消息，将消息分派给具体的逻辑处理类去执行
// 今天到了国家图书馆，感觉新馆真的很气派，在这里写代码很有意思。
// 有时候，在朋友们的支持下，PSS才会走的更远。
// 不断的有好建议提出，可见大家都在认真使用中。
// 有时候平凡就是这样，
// add by freeeyes
// 2009-01-29

#include "MessageManager.h"
#if PSS_PLATFORM == PLATFORM_WIN
#include "ProConnectHandler.h"
#else
#include "ConnectHandler.h"
#endif

void CMessageManager::Init(uint16 u2MaxModuleCount, uint32 u4MaxCommandCount)
{
    m_u2MaxModuleCount  = u2MaxModuleCount;
    m_u4MaxCommandCount = u4MaxCommandCount;
    m_u4UpdateIndex     = 0;
}

bool CMessageManager::AddClientCommand(uint16 u2CommandID, shared_ptr<CClientCommand> pClientCommand, const char* pModuleName)
{
    return AddClientCommand_Ex(u2CommandID, pClientCommand, pModuleName, nullptr);
}

bool CMessageManager::AddClientCommand(uint16 u2CommandID, shared_ptr<CClientCommand> pClientCommand, const char* pModuleName, _ClientIPInfo* pListenInfo)
{
    return AddClientCommand_Ex(u2CommandID, pClientCommand, pModuleName, pListenInfo);
}

bool CMessageManager::UnloadModuleCommand(const char* pModuleName, uint8 u1LoadState, uint32 u4ThreadCount)
{
    string strModuleName  = pModuleName;
    string strModuleN     = "";
    string strModulePath  = "";
    string strModuleParam = "";
    uint32 u4TmpUpdateIndex = 0;

    //获得重载对象相关参数
    auto pModuleInfo = App_ModuleLoader::instance()->GetModuleInfo(pModuleName);

    if(nullptr != pModuleInfo)
    {
        //获取对象信息
        strModuleN     = pModuleInfo->strModuleName;
        strModulePath  = pModuleInfo->strModulePath;
        strModuleParam = pModuleInfo->strModuleParam;
    }

    auto f = m_objModuleClientList.find(strModuleName.c_str());

    if(m_objModuleClientList.end() != f)
    {
        //从插件目前注册的命令里面找到所有该插件的信息，一个个释放
        for(auto pClientCommandInfo : f->second->m_vecClientCommandInfo)
        {
            //在命令列表中删除指定命令的映射关系
            DeleteCommandByModule(pClientCommandInfo);
        }

        u4TmpUpdateIndex++;

        //卸载插件信息(不在这里卸载，在定时器检测所有工作线程都处理完了，再卸载指定的模块,先将需要卸载的模块放入需要卸载的队列)
        if (false == App_ModuleLoader::instance()->MoveUnloadList(pModuleName, u4TmpUpdateIndex, u4ThreadCount, u1LoadState, strModulePath, strModuleN, strModuleParam))
        {
            PSS_LOGGER_DEBUG("[CMessageManager::UnloadModuleCommand]MoveUnloadList error({0}).", pModuleName);
        }
    }

    m_u4UpdateIndex = u4TmpUpdateIndex;
    return true;
}

int CMessageManager::GetCommandCount() const
{
    return (int)m_u4CurrCommandCount;
}

void CMessageManager::Close()
{
    //类关闭的清理工作
    m_objClientCommandList.clear();

    m_objModuleClientList.clear();

    m_u2MaxModuleCount  = 0;
    m_u4MaxCommandCount = 0;

}

hashmapModuleClientList CMessageManager::GetModuleClient() const
{
    return m_objModuleClientList;
}

uint32 CMessageManager::GetWorkThreadCount()
{
    return App_MessageServiceGroup::instance()->GetWorkThreadCount();
}

uint32 CMessageManager::GetWorkThreadByIndex(uint32 u4Index)
{
    return App_MessageServiceGroup::instance()->GetWorkThreadIDByIndex(u4Index);
}

uint16 CMessageManager::GetMaxCommandCount() const
{
    return (uint16)m_u4MaxCommandCount;
}

uint32 CMessageManager::GetUpdateIndex() const
{
    return m_u4UpdateIndex;
}

hashmapClientCommandList CMessageManager::GetHashCommandList() const
{
    return m_objClientCommandList;
}

bool CMessageManager::AddClientCommand_Ex(uint16 u2CommandID, shared_ptr<CClientCommand> pClientCommand, const char* pModuleName, const _ClientIPInfo* pListenInfo)
{
    if (nullptr == pClientCommand)
    {
        PSS_LOGGER_DEBUG("[CMessageManager::AddClientCommand_Ex] u2CommandID = {0} pClientCommand is nullptr.", u2CommandID);
        return false;
    }

    //从配置文件获取Timeout设置
   const xmlCommandsTimeout::_CommandsTimeout* pCommandTimeout = GetXmlConfigAttribute(xmlCommandsTimeout)->GetCommandAlert(u2CommandID);

    shared_ptr<CClientCommandList> pClientCommandList = GetClientCommandExist(u2CommandID);

    if (nullptr != pClientCommandList)
    {
        Add_ClientCommandList(pCommandTimeout, pClientCommandList, u2CommandID, pClientCommand, pModuleName, pListenInfo);

        PSS_LOGGER_DEBUG("[CMessageManager::AddClientCommand_Ex] u2CommandID = {0} Add OK***.", u2CommandID);
    }
    else
    {
        //该命令尚未添加
        pClientCommandList = std::make_shared<CClientCommandList>(u2CommandID);

        Add_ClientCommandList(pCommandTimeout, pClientCommandList, u2CommandID, pClientCommand, pModuleName, pListenInfo);

        m_objClientCommandList[u2CommandID] = pClientCommandList;
        m_u4CurrCommandCount++;
        PSS_LOGGER_DEBUG("[CMessageManager::AddClientCommand_Ex]AddClientCommand u2CommandID = {0} Add OK***.", u2CommandID);
    }

    return true;
}

void CMessageManager::DeleteCommandByModule(shared_ptr<_ClientCommandInfo> pClientCommandInfo)
{
    uint16 u2CommandID = pClientCommandInfo->m_u2CommandID;
    shared_ptr<CClientCommandList> pClientCommandList = GetClientCommandExist(u2CommandID);

    if (nullptr == pClientCommandList)
    {
        return;
    }

    for (int i = 0; i < pClientCommandList->GetCount(); i++)
    {
        //找到那个唯一
        if (pClientCommandList->GetClientCommandIndex(i) == pClientCommandInfo)
        {
            //找到了，释放之
            if (false == pClientCommandList->DelClientCommand(pClientCommandInfo->m_pClientCommand))
            {
                PSS_LOGGER_DEBUG("[CMessageManager::UnloadModuleCommand]DelClientCommand({0}) is fail.", pClientCommandInfo->m_u2CommandID);
            }
            else
            {
                PSS_LOGGER_DEBUG("[CMessageManager::UnloadModuleCommand]DelClientCommand({0}) is OK.", pClientCommandInfo->m_u2CommandID);
            }

            break;
        }
    }
}

void CMessageManager::Add_ClientCommandList(const xmlCommandsTimeout::_CommandsTimeout* pCommandTimeout, shared_ptr<CClientCommandList> pClientCommandList, uint16 u2CommandID, shared_ptr<CClientCommand> pClientCommand, const char* pModuleName, const _ClientIPInfo* pListenInfo)
{
	//如果超时时间不为空，设置为超时时间
	if (nullptr != pCommandTimeout)
	{
		pClientCommandList->SetCommandTimeout(pCommandTimeout->Timeout);
	}

	//该命令已存在
	shared_ptr<_ClientCommandInfo> pClientCommandInfo = pClientCommandList->AddClientCommand(pClientCommand, pModuleName, pListenInfo);
	//设置命令绑定ID
	pClientCommandInfo->m_u2CommandID = u2CommandID;

	//添加到模块里面
	string strModule = pModuleName;
	auto f = m_objModuleClientList.find(strModule.c_str());

	if (m_objModuleClientList.end() == f)
	{
		//找不到，创建新的模块信息
		auto pModuleClient = std::make_shared<_ModuleClient>();

		pModuleClient->m_vecClientCommandInfo.push_back(pClientCommandInfo);
		m_objModuleClientList[strModule.c_str()] = pModuleClient;
	}
	else
	{
		//找到了，添加进去
		f->second->m_vecClientCommandInfo.push_back(pClientCommandInfo);
	}
}

bool CMessageManager::DelClientCommand(uint16 u2CommandID, shared_ptr<CClientCommand> pClientCommand)
{
    auto f = m_objClientCommandList.find(u2CommandID);

    if (m_objClientCommandList.end() == f)
    {
        PSS_LOGGER_DEBUG("[CMessageManager::DelClientCommand] u2CommandID = {0} is not exist.", u2CommandID);
        return false;
    }

    auto pClientCommandList = f->second;

    if (true == pClientCommandList->DelClientCommand(pClientCommand))
    {
        m_objClientCommandList.erase(f);
        PSS_LOGGER_DEBUG("[CMessageManager::DelClientCommand] u2CommandID = {0} List Del OK.", u2CommandID);
    }
    else
    {
        PSS_LOGGER_DEBUG("[CMessageManager::DelClientCommand] u2CommandID = {0} Del Command OK.", u2CommandID);
    }

    return true;
}

shared_ptr<CClientCommandList> CMessageManager::GetClientCommandExist(uint16 u2CommandID)
{
    auto f = m_objClientCommandList.find(u2CommandID);

    if (m_objClientCommandList.end() == f)
    {
        return nullptr;
    }

    return f->second;
}

