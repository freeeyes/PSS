// MessageService.h
// ������Ϣ������Ϣ���ɸ�������߼�������ȥִ��
// ���쵽�˹���ͼ��ݣ��о��¹���ĺ����ɣ�������д���������˼��
// ��ʱ���������ǵ�֧���£�PSS�Ż��ߵĸ�Զ��
// ���ϵ��кý���������ɼ���Ҷ�������ʹ���С�
// ��ʱ��ƽ������������
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

    //������ض�����ز���
    auto pModuleInfo = App_ModuleLoader::instance()->GetModuleInfo(pModuleName);

    if(nullptr != pModuleInfo)
    {
        //��ȡ������Ϣ
        strModuleN     = pModuleInfo->strModuleName;
        strModulePath  = pModuleInfo->strModulePath;
        strModuleParam = pModuleInfo->strModuleParam;
    }

    auto f = m_objModuleClientList.find(strModuleName.c_str());

    if(m_objModuleClientList.end() != f)
    {
        //�Ӳ��Ŀǰע������������ҵ����иò������Ϣ��һ�����ͷ�
        for(auto pClientCommandInfo : f->second->m_vecClientCommandInfo)
        {
            //�������б���ɾ��ָ�������ӳ���ϵ
            DeleteCommandByModule(pClientCommandInfo);
        }

        u4TmpUpdateIndex++;

        //ж�ز����Ϣ(��������ж�أ��ڶ�ʱ��������й����̶߳��������ˣ���ж��ָ����ģ��,�Ƚ���Ҫж�ص�ģ�������Ҫж�صĶ���)
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
    //��رյ�������
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

    //�������ļ���ȡTimeout����
   const xmlCommandsTimeout::_CommandsTimeout* pCommandTimeout = GetXmlConfigAttribute(xmlCommandsTimeout)->GetCommandAlert(u2CommandID);

    shared_ptr<CClientCommandList> pClientCommandList = GetClientCommandExist(u2CommandID);

    if (nullptr != pClientCommandList)
    {
        Add_ClientCommandList(pCommandTimeout, pClientCommandList, u2CommandID, pClientCommand, pModuleName, pListenInfo);

        PSS_LOGGER_DEBUG("[CMessageManager::AddClientCommand_Ex] u2CommandID = {0} Add OK***.", u2CommandID);
    }
    else
    {
        //��������δ���
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
        //�ҵ��Ǹ�Ψһ
        if (pClientCommandList->GetClientCommandIndex(i) == pClientCommandInfo)
        {
            //�ҵ��ˣ��ͷ�֮
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
	//�����ʱʱ�䲻Ϊ�գ�����Ϊ��ʱʱ��
	if (nullptr != pCommandTimeout)
	{
		pClientCommandList->SetCommandTimeout(pCommandTimeout->Timeout);
	}

	//�������Ѵ���
	shared_ptr<_ClientCommandInfo> pClientCommandInfo = pClientCommandList->AddClientCommand(pClientCommand, pModuleName, pListenInfo);
	//���������ID
	pClientCommandInfo->m_u2CommandID = u2CommandID;

	//��ӵ�ģ������
	string strModule = pModuleName;
	auto f = m_objModuleClientList.find(strModule.c_str());

	if (m_objModuleClientList.end() == f)
	{
		//�Ҳ����������µ�ģ����Ϣ
		auto pModuleClient = std::make_shared<_ModuleClient>();

		pModuleClient->m_vecClientCommandInfo.push_back(pClientCommandInfo);
		m_objModuleClientList[strModule.c_str()] = pModuleClient;
	}
	else
	{
		//�ҵ��ˣ���ӽ�ȥ
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

