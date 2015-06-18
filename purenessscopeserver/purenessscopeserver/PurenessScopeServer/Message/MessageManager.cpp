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
}

CMessageManager::~CMessageManager(void)
{
	OUR_DEBUG((LM_INFO, "[CMessageManager::~CMessageManager].\n"));
	//Close();
}

bool CMessageManager::DoMessage(ACE_Time_Value& tvBegin, IMessage* pMessage, uint16& u2CommandID, uint32& u4TimeCost, uint16& u2Count, bool& bDeleteFlag)
{
	if(NULL == pMessage)
	{
		OUR_DEBUG((LM_ERROR, "[CMessageManager::DoMessage] pMessage is NULL.\n"));
		return false;
	}

	//放给需要继承的ClientCommand类去处理
	//OUR_DEBUG((LM_ERROR, "[CMessageManager::DoMessage]u2CommandID = %d.\n", u2CommandID));

	CClientCommandList* pClientCommandList = GetClientCommandList(u2CommandID);
	if(pClientCommandList != NULL)
	{
		int nCount = pClientCommandList->GetCount();
		for(int i = 0; i < nCount; i++)
		{
			_ClientCommandInfo* pClientCommandInfo = pClientCommandList->GetClientCommandIndex(i);
			if(NULL != pClientCommandInfo && pClientCommandInfo->m_u1State == 0)
			{
				//判断当前消息是否有指定的监听端口
				if(pClientCommandInfo->m_objListenIPInfo.m_nPort > 0)
				{
					if(ACE_OS::strcmp(pClientCommandInfo->m_objListenIPInfo.m_szClientIP, pMessage->GetMessageBase()->m_szListenIP) != 0 ||
						pClientCommandInfo->m_objListenIPInfo.m_nPort != pMessage->GetMessageBase()->m_u4ListenPort)
					{
						continue;
					}
				}

				//OUR_DEBUG((LM_ERROR, "[CMessageManager::DoMessage]u2CommandID = %d Begin.\n", u2CommandID));
				//这里指记录处理毫秒数
				pClientCommandInfo->m_pClientCommand->DoMessage(pMessage, bDeleteFlag);
				ACE_Time_Value tvCost =  ACE_OS::gettimeofday() - tvBegin;
				u4TimeCost =  (uint32)tvCost.msec();

				//记录命令被调用次数
				u2Count++;
				//OUR_DEBUG((LM_ERROR, "[CMessageManager::DoMessage]u2CommandID = %d End.\n", u2CommandID));
				
			}
		}		
	}

	return true;
}

CClientCommandList* CMessageManager::GetClientCommandList(uint16 u2CommandID)
{
	mapClientCommand::iterator f = m_mapClientCommand.find(u2CommandID);
	if(f != m_mapClientCommand.end())
	{
		CClientCommandList* pClientCommandList = (CClientCommandList* )f->second;
		return pClientCommandList;
	}
	else
	{
		//OUR_DEBUG((LM_ERROR, "[CMessageManager::GetClientCommand] u2CommandID = %d 0x%08x Add OK.\n", u2CommandID, &m_mapClientCommand));
		return NULL;
	}
}

bool CMessageManager::AddClientCommand(uint16 u2CommandID, CClientCommand* pClientCommand, const char* pModuleName)
{
	if(NULL == pClientCommand)
	{
		OUR_DEBUG((LM_ERROR, "[CMessageManager::AddClientCommand] u2CommandID = %d pClientCommand is NULL.\n", u2CommandID));
		return false;
	}

	mapClientCommand::iterator f = m_mapClientCommand.find(u2CommandID);
	if(f != m_mapClientCommand.end())
	{
		CClientCommandList* pClientCommandList = (CClientCommandList* )f->second;
		if(NULL != pClientCommandList)
		{
			_ClientCommandInfo* pClientCommandInfo = pClientCommandList->AddClientCommand(pClientCommand, pModuleName);
			if(NULL != pClientCommandInfo) 
			{
				//设置命令绑定ID
				pClientCommandInfo->m_u2CommandID = u2CommandID;
				
				//添加到模块里面
				string strModule = pModuleName;
				mapModuleClient::iterator f = m_mapModuleClient.find(strModule);
				if(f == m_mapModuleClient.end())
				{
					//找不到，创建新的模块信息
					_ModuleClient* pModuleClient = new _ModuleClient();
					if(NULL != pModuleClient)
					{
						pModuleClient->m_vecClientCommandInfo.push_back(pClientCommandInfo);
						m_mapModuleClient.insert(mapModuleClient::value_type(strModule, pModuleClient));
					}
				}
				else
				{
					//找到了，添加进去
					_ModuleClient* pModuleClient = (_ModuleClient* )f->second;
					if(NULL != pModuleClient)
					{
						pModuleClient->m_vecClientCommandInfo.push_back(pClientCommandInfo);
					}
				}
				OUR_DEBUG((LM_ERROR, "[CMessageManager::AddClientCommand] u2CommandID = %d Add OK***.\n", u2CommandID));
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}

	}

	//没有找到已有的命令，新建一个列表
	CClientCommandList* pClientCommandList = new CClientCommandList();
	if(NULL == pClientCommandList)
	{
		return false;
	}
	else
	{
		_ClientCommandInfo* pClientCommandInfo = pClientCommandList->AddClientCommand(pClientCommand, pModuleName);
		if(NULL != pClientCommandInfo)
		{
			//设置命令绑定ID
			pClientCommandInfo->m_u2CommandID = u2CommandID;			
			
			//添加到模块里面
			string strModule = pModuleName;
			mapModuleClient::iterator f = m_mapModuleClient.find(strModule);
			if(f == m_mapModuleClient.end())
			{
				//找不到，创建新的模块信息
				_ModuleClient* pModuleClient = new _ModuleClient();
				if(NULL != pModuleClient)
				{
					pModuleClient->m_vecClientCommandInfo.push_back(pClientCommandInfo);
					m_mapModuleClient.insert(mapModuleClient::value_type(strModule, pModuleClient));
				}
			}
			else
			{
				//找到了，添加进去
				_ModuleClient* pModuleClient = (_ModuleClient* )f->second;
				if(NULL != pModuleClient)
				{
					pModuleClient->m_vecClientCommandInfo.push_back(pClientCommandInfo);
				}
			}
		}

		m_mapClientCommand.insert(mapClientCommand::value_type(u2CommandID, pClientCommandList));
		OUR_DEBUG((LM_ERROR, "[CMessageManager::AddClientCommand] u2CommandID = %d Add OK***.\n", u2CommandID));
		return true;
	}
}

bool CMessageManager::AddClientCommand(uint16 u2CommandID, CClientCommand* pClientCommand, const char* pModuleName, _ClientIPInfo objListenInfo)
{
	if(NULL == pClientCommand)
	{
		OUR_DEBUG((LM_ERROR, "[CMessageManager::AddClientCommand] u2CommandID = %d pClientCommand is NULL.\n", u2CommandID));
		return false;
	}

	mapClientCommand::iterator f = m_mapClientCommand.find(u2CommandID);
	if(f != m_mapClientCommand.end())
	{
		CClientCommandList* pClientCommandList = (CClientCommandList* )f->second;
		if(NULL != pClientCommandList)
		{
			_ClientCommandInfo* pClientCommandInfo = pClientCommandList->AddClientCommand(pClientCommand, pModuleName, objListenInfo);
			if(NULL != pClientCommandInfo) 
			{
				//设置命令绑定ID
				pClientCommandInfo->m_u2CommandID = u2CommandID;

				//添加到模块里面
				string strModule = pModuleName;
				mapModuleClient::iterator f = m_mapModuleClient.find(strModule);
				if(f == m_mapModuleClient.end())
				{
					//找不到，创建新的模块信息
					_ModuleClient* pModuleClient = new _ModuleClient();
					if(NULL != pModuleClient)
					{
						pModuleClient->m_vecClientCommandInfo.push_back(pClientCommandInfo);
						m_mapModuleClient.insert(mapModuleClient::value_type(strModule, pModuleClient));
					}
				}
				else
				{
					//找到了，添加进去
					_ModuleClient* pModuleClient = (_ModuleClient* )f->second;
					if(NULL != pModuleClient)
					{
						pModuleClient->m_vecClientCommandInfo.push_back(pClientCommandInfo);
					}
				}
				OUR_DEBUG((LM_ERROR, "[CMessageManager::AddClientCommand] u2CommandID = %d Add OK***.\n", u2CommandID));
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}

	}

	//没有找到已有的命令，新建一个列表
	CClientCommandList* pClientCommandList = new CClientCommandList();
	if(NULL == pClientCommandList)
	{
		return false;
	}
	else
	{
		_ClientCommandInfo* pClientCommandInfo = pClientCommandList->AddClientCommand(pClientCommand, pModuleName, objListenInfo);
		if(NULL != pClientCommandInfo)
		{
			//设置命令绑定ID
			pClientCommandInfo->m_u2CommandID = u2CommandID;			

			//添加到模块里面
			string strModule = pModuleName;
			mapModuleClient::iterator f = m_mapModuleClient.find(strModule);
			if(f == m_mapModuleClient.end())
			{
				//找不到，创建新的模块信息
				_ModuleClient* pModuleClient = new _ModuleClient();
				if(NULL != pModuleClient)
				{
					pModuleClient->m_vecClientCommandInfo.push_back(pClientCommandInfo);
					m_mapModuleClient.insert(mapModuleClient::value_type(strModule, pModuleClient));
				}
			}
			else
			{
				//找到了，添加进去
				_ModuleClient* pModuleClient = (_ModuleClient* )f->second;
				if(NULL != pModuleClient)
				{
					pModuleClient->m_vecClientCommandInfo.push_back(pClientCommandInfo);
				}
			}
		}

		m_mapClientCommand.insert(mapClientCommand::value_type(u2CommandID, pClientCommandList));
		OUR_DEBUG((LM_ERROR, "[CMessageManager::AddClientCommand] u2CommandID = %d Add OK***.\n", u2CommandID));
		return true;
	}
}

bool CMessageManager::DelClientCommand(uint16 u2CommandID, CClientCommand* pClientCommand)
{
	mapClientCommand::iterator f = m_mapClientCommand.find(u2CommandID);
	if(f != m_mapClientCommand.end())
	{
		CClientCommandList* pClientCommandList = (CClientCommandList* )f->second;
		if(NULL != pClientCommandList)
		{
			if(true == pClientCommandList->DelClientCommand(pClientCommand))
			{
				SAFE_DELETE(pClientCommandList);
				m_mapClientCommand.erase(f);
			}
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

bool CMessageManager::UnloadModuleCommand(const char* pModuleName, uint8 u1State)
{
	string strModuleName = pModuleName;

	mapModuleClient::iterator f = m_mapModuleClient.find(strModuleName);
	if(f != m_mapModuleClient.end())
	{
		//首先删除所有插件对应的Command映射
		_ModuleClient* pModuleClient = (_ModuleClient* )f->second;
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
								pCClientCommandList->DelClientCommand(pClientCommandInfo->m_pClientCommand);
								break;
							}
						}
					}
				}
			}

			//最后删除实际模块和CommandInfo的关系
			SAFE_DELETE(pModuleClient);
			m_mapModuleClient.erase(f);

			//卸载插件信息
			App_ModuleLoader::instance()->UnLoadModule(pModuleName);
		}

		//看看是否要重新加载
		if(u1State == 2)
		{
			_ModuleInfo* pModuleInfo = App_ModuleLoader::instance()->GetModuleInfo(pModuleName);
			if(NULL != pModuleInfo)
			{
				//获取对象信息
				string strModuleN    = pModuleInfo->strModuleName;
				string strModulePath = pModuleInfo->strModulePath;

				//重新加载
				App_ModuleLoader::instance()->LoadModule(strModulePath.c_str(), strModuleN.c_str());
			}
		}

		return true;
	}
	else
	{
		return false;
	}
}

int CMessageManager::GetCommandCount()
{
	return (int)m_mapClientCommand.size();
}

void CMessageManager::Close()
{
	//类关闭的清理工作
	for(mapClientCommand::iterator b = m_mapClientCommand.begin(); b != m_mapClientCommand.end(); b++)
	{
		 CClientCommandList* pClientCommandList = (CClientCommandList* )b->second;
		 SAFE_DELETE(pClientCommandList);
	}

	for(mapModuleClient::iterator Mb = m_mapModuleClient.begin(); Mb != m_mapModuleClient.end(); Mb++)
	{
		_ModuleClient* pModuleClient = (_ModuleClient* )Mb->second;
		SAFE_DELETE(pModuleClient);
	}

	m_mapClientCommand.clear();
}

mapModuleClient* CMessageManager::GetModuleClient()
{
	return &m_mapModuleClient;
}

uint32 CMessageManager::GetWorkThreadCount()
{
	return App_MessageServiceGroup::instance()->GetWorkThreadCount();
}

uint32 CMessageManager::GetWorkThreadByIndex(uint32 u4Index)
{
	return App_MessageServiceGroup::instance()->GetWorkThreadIDByIndex(u4Index);
}

