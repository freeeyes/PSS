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

bool CMessageManager::DoMessage(IMessage* pMessage, uint16& u2CommandID)
{
	if(NULL == pMessage)
	{
		OUR_DEBUG((LM_ERROR, "[CMessageManager::DoMessage] pMessage is NULL.\n"));
		return false;
	}

	//放给需要继承的ClientCommand类去处理
	bool bDeleteFlag = true;         //数据包是否用完后删除
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
				m_ThreadWriteLock.acquire();
				pClientCommandInfo->m_u4CurrUsedCount++;
				pClientCommandInfo->m_u4Count++;
				m_ThreadWriteLock.release();

				//获得包长
				_PacketInfo PacketInfoHead;
				_PacketInfo PacketInfoBody;

				pMessage->GetPacketHead(PacketInfoHead);
				pMessage->GetPacketBody(PacketInfoBody);

				//pMessage->GetMessageBase()->m_ProfileTime.Start();
				//OUR_DEBUG((LM_ERROR, "[CMessageManager::DoMessage]u2CommandID = %d Begin.\n", u2CommandID));
				//这里指记录处理毫秒数
				ACE_Time_Value tvBegin = ACE_OS::gettimeofday();
				pClientCommandInfo->m_pClientCommand->DoMessage(pMessage, bDeleteFlag);
				ACE_Time_Value tvCost =  ACE_OS::gettimeofday() - tvBegin;
				//OUR_DEBUG((LM_ERROR, "[CMessageManager::DoMessage]u2CommandID = %d End.\n", u2CommandID));
				m_ThreadWriteLock.acquire();

				//添加统计信息
				App_CommandAccount::instance()->SaveCommandData(u2CommandID, (uint64)tvCost.msec(), 
					                                           pMessage->GetMessageBase()->m_u1PacketType, 
															   pMessage->GetMessageBase()->m_u4HeadSrcSize + pMessage->GetMessageBase()->m_u4BodySrcSize, 
															   (uint32)(PacketInfoHead.m_nDataLen + PacketInfoBody.m_nDataLen), 
															   COMMAND_TYPE_IN);

				if(pClientCommandInfo->m_u4CurrUsedCount > 0)
				{
					pClientCommandInfo->m_u4CurrUsedCount--;

					//这里加一个判断，判断是否需要关闭
					if(pClientCommandInfo->m_u1State == 1)
					{
						CloseCommandInfo(pClientCommandInfo->m_szModuleName);
					}
				}
				m_ThreadWriteLock.release();
			}
		}		
	}

	if(true == bDeleteFlag)
	{
		App_MessagePool::instance()->Delete((CMessage* )pMessage);
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
		_ModuleClient* pModuleClient = (_ModuleClient* )f->second;
		if(NULL != pModuleClient)
		{
			pModuleClient->m_u1ModuleState = u1State;
			for(int i = 0; i < (int)pModuleClient->m_vecClientCommandInfo.size(); i++)
			{
				_ClientCommandInfo* pClientCommandInfo = (_ClientCommandInfo* )pModuleClient->m_vecClientCommandInfo[i];
				if(NULL != pClientCommandInfo)
				{
					pClientCommandInfo->m_u1State = 1;
				}
			}

			//查找已经等于0的，删除之
			vector<_ClientCommandInfo*>::iterator fd = std::remove_if(pModuleClient->m_vecClientCommandInfo.begin(), pModuleClient->m_vecClientCommandInfo.end(), Delete_CommandInfo);
			pModuleClient->m_vecClientCommandInfo.erase (fd, pModuleClient->m_vecClientCommandInfo.end());

			if((int)pModuleClient->m_vecClientCommandInfo.size() == 0)
			{
				//如果没有这个模块要处理的消息了，则关闭之。
				if(pModuleClient->m_u1ModuleState == (uint8)1)
				{
					App_ModuleLoader::instance()->UnLoadModule(pModuleName);
					m_mapModuleClient.erase(f);
					SAFE_DELETE(pModuleClient);
          return true;
				}

				//如果是2，则重新加载这个模块
				if(pModuleClient->m_u1ModuleState == (uint8)2)
				{
					_ModuleInfo* pModuleInfo = App_ModuleLoader::instance()->GetModuleInfo(pModuleName);
					if(NULL != pModuleInfo)
					{
						//获取对象信息
						string strModuleName = pModuleInfo->strModuleName;
						string strModulePath = pModuleInfo->strModulePath;

						//关闭模块对象
						App_ModuleLoader::instance()->UnLoadModule(pModuleName);
						m_mapModuleClient.erase(f);
						SAFE_DELETE(pModuleClient);

						//重新加载
						App_ModuleLoader::instance()->LoadModule(strModulePath.c_str(), strModuleName.c_str());
					}
				}
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

bool CMessageManager::CloseCommandInfo(const char* pModuleName)
{
	string strModuleName = pModuleName;
	mapModuleClient::iterator f = m_mapModuleClient.find(strModuleName);
	if(f != m_mapModuleClient.end())
	{
		_ModuleClient* pModuleClient = (_ModuleClient* )f->second;
		if(NULL != pModuleClient)
		{
			for(int i = 0; i < (int)pModuleClient->m_vecClientCommandInfo.size(); i++)
			{
				_ClientCommandInfo* pClientCommandInfo = (_ClientCommandInfo* )pModuleClient->m_vecClientCommandInfo[i];
				if(NULL != pClientCommandInfo)
				{
					if(pClientCommandInfo->m_u4CurrUsedCount == 0)
					{
						pModuleClient->m_vecClientCommandInfo.erase((pModuleClient->m_vecClientCommandInfo.begin() + i));
						break;
					}
				}
			}

			if((int)pModuleClient->m_vecClientCommandInfo.size() == 0)
			{
				//如果没有这个模块要处理的消息了，则关闭之。
				if(pModuleClient->m_u1ModuleState == (uint8)1)
				{
					App_ModuleLoader::instance()->UnLoadModule(pModuleName);
					m_mapModuleClient.erase(f);
					SAFE_DELETE(pModuleClient);
				}

				//如果是2，则重新加载这个模块
				if(pModuleClient->m_u1ModuleState == (uint8)2)
				{
					_ModuleInfo* pModuleInfo = App_ModuleLoader::instance()->GetModuleInfo(pModuleName);
					if(NULL != pModuleInfo)
					{
						//获取对象信息
						string strModuleName = pModuleInfo->strModuleName;
						string strModulePath = pModuleInfo->strModulePath;

						//关闭模块对象
						App_ModuleLoader::instance()->UnLoadModule(pModuleName);
						m_mapModuleClient.erase(f);
						SAFE_DELETE(pModuleClient);

						//重新加载
						App_ModuleLoader::instance()->LoadModule(strModulePath.c_str(), strModuleName.c_str());
					}
				}
			}
		}
		return true;
	}
	else
	{
		return false;
	}
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

