#include "PlayerPoolCommand.h"

CPlayerPoolCommand::CPlayerPoolCommand(void)
:m_MapReadData(MAX_READ_OBJECT),m_MapOnlineData(MAX_ONLINE_OBJECT)
{
	Init();
	m_pServerObject = NULL;
}

CPlayerPoolCommand::~CPlayerPoolCommand(void)
{
}

void CPlayerPoolCommand::Init()
{
	SMKey key;
#ifdef WIN32	
	key = 1111;
#else
	key = ftok("/home/freeeyes/SMData/", 0);
#endif

	printf("[Test]%d.\n", key);
	m_UserPool.Init(key, MAX_READ_OBJECT + MAX_ONLINE_OBJECT);

	//把已有数据存入map指针
	for(int i = 0; i < m_UserPool.GetUsedObjectCount(); i++)
	{
		CPlayerData* pData = m_UserPool.GetUsedObject(i);
		if(pData->GetOnline() == true)
		{
			//如果是在线用户，则添加到登陆池
			m_MapOnlineData.AddMapData((string)pData->GetKey(), pData, false);
		}
		else
		{
			//添加到读取池
			m_MapReadData.AddMapData((string)pData->GetKey(), pData, false);
		}
	}
}

int CPlayerPoolCommand::DoMessage(IMessage* pMessage, bool& bDeleteFlag)
{
	uint16 u2CommandID = 0;
	IBuffPacket* pBodyPacket = m_pServerObject->GetPacketManager()->Create();
	if(NULL == pBodyPacket)
	{
		OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pBodyPacket is NULL.\n"));
		return -1;
	}

	_PacketInfo BodyPacket;
	pMessage->GetPacketBody(BodyPacket);

	pBodyPacket->WriteStream(BodyPacket.m_pData, BodyPacket.m_nDataLen);

	(*pBodyPacket) >> u2CommandID;

	//根据ID进行数据处理(玩家数据添加)
	if(u2CommandID == COMMAND_PLAYINSERT)
	{
		VCHARS_STR strsData;
		char szNick[20] = {'\0'};
		(*pBodyPacket) >> strsData;
		sprintf_safe(szNick, 20, "%s", strsData.text);
		szNick[strsData.u1Len] = '\0';
		CPlayerData* pData = Do_PlayerInsert(szNick);

		m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

		if(NULL != pData)
		{
			//返回数据
			IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
			VCHARM_STR strMData;
			strMData.u2Len = sizeof(CPlayerData);
			strMData.text  = (const char* )pData;

			(*pResponsesPacket) << (uint16)COMMAND_PLAYINSERT;
			(*pResponsesPacket) << (uint8)1;         //1表示有数据
			(*pResponsesPacket) << strMData;

			if(NULL != m_pServerObject->GetConnectManager())
			{
				m_pServerObject->GetConnectManager()->SendMessage(pMessage->GetMessageBase()->m_u4ConnectID, pResponsesPacket);
			}
			else
			{
				OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
			}
		}
		else
		{
			//返回数据
			IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();

			(*pResponsesPacket) << (uint16)COMMAND_PLAYINSERT;
			(*pResponsesPacket) << (uint8)0;         //0表示没有数据

			if(NULL != m_pServerObject->GetConnectManager())
			{
				m_pServerObject->GetConnectManager()->SendMessage(pMessage->GetMessageBase()->m_u4ConnectID, pResponsesPacket);
			}
			else
			{
				OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
			}
		}
	}

	//(玩家数据更新)
	if(u2CommandID == COMMAND_PLAYUPDATE)
	{
		VCHARM_STR strMName;
		(*pBodyPacket) >> strMName;
		CPlayerData* pData = (CPlayerData* )strMName.text;
		Do_PlayerUpdate(pData);

		m_pServerObject->GetPacketManager()->Delete(pBodyPacket);
	}

	//(玩家数据删除)
	if(u2CommandID == COMMAND_PLAYDELETE)
	{
		VCHARS_STR strsData;
		char szNick[20] = {'\0'};
		(*pBodyPacket) >> strsData;
		sprintf_safe(szNick, 20, "%s", strsData.text);
		szNick[strsData.u1Len] = '\0';
		Do_PlayerDelete(szNick);

		m_pServerObject->GetPacketManager()->Delete(pBodyPacket);
	}

	//(玩家数据查询)
	if(u2CommandID == COMMAND_PLAYSEACH)
	{
		VCHARS_STR strsData;
		char szNick[20] = {'\0'};
		(*pBodyPacket) >> strsData;
		sprintf_safe(szNick, 20, "%s", strsData.text);
		szNick[strsData.u1Len] = '\0';
		CPlayerData* pData = Do_PlayerSearch(szNick);

		m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

		if(pData != NULL)
		{
			//返回数据
			IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
			VCHARM_STR strMData;
			strMData.u2Len = sizeof(CPlayerData);
			strMData.text  = (const char* )pData;

			(*pResponsesPacket) << (uint16)COMMAND_PLAYINSERT;
			(*pResponsesPacket) << (uint8)1;             //1表示有数据
			(*pResponsesPacket) << strMData;

			if(NULL != m_pServerObject->GetConnectManager())
			{
				m_pServerObject->GetConnectManager()->SendMessage(pMessage->GetMessageBase()->m_u4ConnectID, pResponsesPacket);
			}
			else
			{
				OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
			}
		}
		else
		{
			//返回数据
			IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();

			(*pResponsesPacket) << (uint16)COMMAND_PLAYINSERT;
			(*pResponsesPacket) << (uint8)0;             //0表示没有数据

			if(NULL != m_pServerObject->GetConnectManager())
			{
				m_pServerObject->GetConnectManager()->SendMessage(pMessage->GetMessageBase()->m_u4ConnectID, pResponsesPacket);
			}
			else
			{
				OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
			}
		}

	}

	//(玩家数据登陆)
	if(u2CommandID == COMMAND_PLAYLOGIN)
	{
		VCHARS_STR strsData;
		char szNick[20] = {'\0'};
		(*pBodyPacket) >> strsData;
		sprintf_safe(szNick, 20, "%s", strsData.text);
		szNick[strsData.u1Len] = '\0';
		CPlayerData* pData = Do_PlayerLogin(szNick);

		m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

		if(pData != NULL)
		{
			//返回数据
			IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
			VCHARM_STR strMData;
			strMData.u2Len = sizeof(CPlayerData);
			strMData.text  = (const char* )pData;

			(*pResponsesPacket) << (uint16)COMMAND_PLAYLOGIN;
			(*pResponsesPacket) << (uint8)1;             //1表示有数据
			(*pResponsesPacket) << strMData;

			if(NULL != m_pServerObject->GetConnectManager())
			{
				m_pServerObject->GetConnectManager()->SendMessage(pMessage->GetMessageBase()->m_u4ConnectID, pResponsesPacket);
			}
			else
			{
				OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
			}
		}
		else
		{
			//返回数据
			IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();

			(*pResponsesPacket) << (uint16)COMMAND_PLAYLOGIN;
			(*pResponsesPacket) << (uint8)0;             //0表示没有数据

			if(NULL != m_pServerObject->GetConnectManager())
			{
				m_pServerObject->GetConnectManager()->SendMessage(pMessage->GetMessageBase()->m_u4ConnectID, pResponsesPacket);
			}
			else
			{
				OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
			}
		}
	}


	//(玩家数据离开)
	if(u2CommandID == COMMAND_PLAYLOGOFF)
	{
		VCHARS_STR strsData;
		char szNick[20] = {'\0'};
		(*pBodyPacket) >> strsData;
		sprintf_safe(szNick, 20, "%s", strsData.text);
		szNick[strsData.u1Len] = '\0';
		bool blState = Do_PlayerLogOff(szNick);

		m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

		//返回数据
		IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();

		(*pResponsesPacket) << (uint16)COMMAND_PLAYLOGOFF;
		if(true == blState)
		{
			(*pResponsesPacket) << (uint8)1;             //1表示离开成功
		}
		else
		{
			(*pResponsesPacket) << (uint8)0;             //1表示离开失败
		}

		if(NULL != m_pServerObject->GetConnectManager())
		{
			m_pServerObject->GetConnectManager()->SendMessage(pMessage->GetMessageBase()->m_u4ConnectID, pResponsesPacket);
		}
		else
		{
			OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
		}
	}

	return 0;
}

CPlayerData* CPlayerPoolCommand::Do_PlayerInsert(const char* pPlayerNick)
{
	string strPlayerNick = pPlayerNick;
	
	//在已有的列表中查找（可读的和在线的map中查找）
	CPlayerData* pPlayerDataRead = m_MapReadData.SearchMapData(strPlayerNick);
	if(NULL != pPlayerDataRead)
	{
		return NULL;
	}

	CPlayerData* pPlayerDataOnline = m_MapOnlineData.SearchMapData(strPlayerNick);
	if(NULL != pPlayerDataOnline)
	{
		return NULL;
	}

	//在IO里查找
	CPlayerData* pPlayerIO = m_IOData.GetPlayer(pPlayerNick);
	if(NULL != pPlayerIO)
	{
		//如果数据存在，则返回创建失败，pPlayerIO需要自己回收
		SAFE_DELETE(pPlayerIO);
		return NULL;
	}

	//检查登陆池是否有足够的空间。
	if((int)m_MapOnlineData.GetSize() >= MAX_ONLINE_OBJECT)
	{
		//登陆池满了，不允许登陆了
		return NULL;
	}

	//查看缓冲池是否已经全满，如果是，需要清理一个最不常用的读取池用户
	if((int)m_MapOnlineData.GetSize() + (int)m_MapOnlineData.GetSize() >= MAX_ONLINE_OBJECT + MAX_READ_OBJECT)
	{
		//从读取池中删除一个最不常用的数据
		CPlayerData* pPlayerDataRead = m_MapReadData.GetDeleteKey();
		if(pPlayerDataRead != NULL)
		{
			//需要处理读取数据，是否需要IO存储
			const time_t ttHeadTimeStamp = m_UserPool.GetObjectHeadTimeStamp(pPlayerDataRead);
			if(ttHeadTimeStamp < pPlayerDataRead->GetTimeStamp())
			{
				//DS尚未写入此数据，此数据是需要写入的，在这里做一次IO写入。
				m_IOData.SavePlayer(pPlayerDataRead);
			}

			//删除共享内存单元，并执行登陆池->读取池，增加命中
			m_UserPool.DeleteObject(pPlayerDataRead);
			m_MapReadData.DelMapData(pPlayerDataRead->GetKey(), false);
		}
	}

	//创建新的角色
	CPlayerData* pData = m_UserPool.NewObject();
	if(NULL == pData)
	{
		return NULL;
	}

	pData->Create(pPlayerNick);
	pData->SetOnline(true);
	m_MapOnlineData.AddMapData(pPlayerNick, pData, false);
	return pData;
}

bool CPlayerPoolCommand::Do_PlayerUpdate(CPlayerData* pPlayerData)
{
	if(NULL == pPlayerData)
	{
		return false;
	}

	string strPlayerNick = pPlayerData->GetKey();
	CPlayerData* pPlayerDataOnline = m_MapOnlineData.SearchMapData(strPlayerNick);
	if(NULL == pPlayerDataOnline)
	{
		return false;
	}
	else
	{
		memcpy(pPlayerDataOnline, pPlayerData, sizeof(CPlayerData));

		//更新时间戳
		pPlayerDataOnline->SetTimeStamp();
		return true;
	}
}

bool CPlayerPoolCommand::Do_PlayerDelete(const char* pPlayerNick)
{
	//用户必须Online在线的时候才能删除
	string strPlayerNick = pPlayerNick;
	CPlayerData* pPlayerDataOnline = m_MapOnlineData.SearchMapData(strPlayerNick);
	if(NULL == pPlayerDataOnline)
	{
		return false;
	}

	//删除IO操作
	m_IOData.DeletePlayer(pPlayerNick);

	//清除共享内存中的数据
	m_UserPool.DeleteObject(pPlayerDataOnline);
	m_MapOnlineData.DelMapData(pPlayerNick, false);

	return true;
}

CPlayerData* CPlayerPoolCommand::Do_PlayerSearch(const char* pPlayerNick)
{
	string strPlayerNick = pPlayerNick;
	//现在在线用户中查找，如果没有，则在读取池里面去命中
	CPlayerData* pPlayerDataOnline = m_MapOnlineData.SearchMapData(strPlayerNick);
	if(NULL == pPlayerDataOnline)
	{
		CPlayerData* pPlayerDataRead = m_MapReadData.SearchMapData(strPlayerNick);
		if(NULL == pPlayerDataRead)
		{
			//如果在线用户池中没有，读取池中也没有，则去IO去找
			//IO查找动作，如果找到了，添加到读取池中
			CPlayerData* pPlayerDataIO = m_IOData.GetPlayer(pPlayerNick);
			if(NULL == pPlayerDataIO)
			{
				return NULL;
			}
			else
			{
				//找到了，添加到读取池
				CPlayerData* pPlayerDataDelete = m_MapReadData.GetDeleteKey();
				if(NULL != pPlayerDataDelete)
				{
					m_UserPool.DeleteObject(pPlayerDataDelete);
					m_MapReadData.DelMapData(pPlayerNick, false);
				}

				//获得新的共享内存节点
				CPlayerData* pPlayerDataNew = m_UserPool.NewObject();
				if(NULL != pPlayerDataNew)
				{
					SAFE_DELETE(pPlayerDataDelete);
					return NULL;
				}

				ACE_OS::memcpy(pPlayerDataNew, pPlayerDataIO, sizeof(CPlayerData));
				m_MapReadData.AddMapData(pPlayerDataNew->GetKey(), pPlayerDataNew, false);
				SAFE_DELETE(pPlayerDataDelete);
				return pPlayerDataNew;
			}
		}
		else
		{
			return pPlayerDataRead;
		}
	}
	else
	{
		return pPlayerDataOnline;
	}
}

CPlayerData* CPlayerPoolCommand::Do_PlayerLogin(const char* pPlayerNick)
{
	string strPlayerNick = pPlayerNick;
	CPlayerData* pPlayerDataOnline = m_MapOnlineData.SearchMapData(strPlayerNick);
	if(NULL == pPlayerDataOnline)
	{
		//如果不在登陆池中，去读取池查找
		CPlayerData* pPlayerDataRead = m_MapReadData.SearchMapData(pPlayerNick);
		if(NULL == pPlayerDataRead)
		{
			//到IO里面去查找
			CPlayerData* pPlayerDataIO = m_IOData.GetPlayer(pPlayerNick);
			if(NULL == pPlayerDataIO)
			{
				return NULL;
			}
			else
			{
				//找到了，添加到登陆池中去
				if(m_MapOnlineData.GetSize() >= MAX_ONLINE_OBJECT)
				{
					//登陆池满了，不能再添加了。
					SAFE_DELETE(pPlayerDataIO);
					return NULL;
				}
				else
				{
					//获得新的共享内存节点
					CPlayerData* pPlayerDataNew = m_UserPool.NewObject();
					if(NULL != pPlayerDataNew)
					{
						SAFE_DELETE(pPlayerDataIO);
						return NULL;
					}

					ACE_OS::memcpy(pPlayerDataNew, pPlayerDataIO, sizeof(CPlayerData));
					pPlayerDataNew->SetOnline(true);
					m_MapOnlineData.AddMapData(pPlayerDataNew->GetKey(), pPlayerDataNew, false);
					SAFE_DELETE(pPlayerDataIO);
					return pPlayerDataNew;
				}
			}
		}
		else
		{
			//将读取池中的数据迁移到登陆池中
			if((int)m_MapOnlineData.GetSize() >= MAX_ONLINE_OBJECT)
			{
				//登陆池已经满了，不能在登陆了
				return NULL;
			}
			else
			{
				//读取池 -> 登陆池
				pPlayerDataRead->SetOnline(true);
				m_MapReadData.DelMapData(pPlayerNick, false);
				m_MapOnlineData.AddMapData(strPlayerNick, pPlayerDataRead, false);
				return pPlayerDataRead;
			}
		}
	}
	else
	{
		return pPlayerDataOnline;
	}
}

bool CPlayerPoolCommand::Do_PlayerLogOff(const char* pPlayerNick) 
{
	string strPlayerNick = pPlayerNick;
	CPlayerData* pPlayerDataOnline = m_MapOnlineData.SearchMapData(strPlayerNick);
	if(NULL == pPlayerDataOnline)
	{
		return false;
	}
	else
	{
		//这里有一个小技巧，先从读取库中获得一个可替换数据的指针
		//判断这个指针是不是需要写入IO，如果需要就写入，不需要就直接删除
		CPlayerData* pPlayerDataRead = m_MapReadData.GetDeleteKey();
		if(pPlayerDataRead == NULL)
		{
			//没有需要移出的数据，就直接放入读取池
			pPlayerDataOnline->SetOnline(false);
			m_MapOnlineData.DelMapData(pPlayerNick, false);
			m_MapReadData.AddMapData(pPlayerNick, pPlayerDataOnline, false);
		}
		else
		{
			//需要处理读取数据，是否需要IO存储
			const time_t ttHeadTimeStamp = m_UserPool.GetObjectHeadTimeStamp(pPlayerDataRead);
			if(ttHeadTimeStamp < pPlayerDataRead->GetTimeStamp())
			{
				//DS尚未写入此数据，此数据是需要写入的，在这里做一次IO写入。
				m_IOData.SavePlayer(pPlayerDataRead);
			}

			//删除共享内存单元，并执行登陆池->读取池，增加命中
			m_UserPool.DeleteObject(pPlayerDataRead);
			m_MapReadData.DelMapData(pPlayerDataRead->GetKey(), false);
			m_MapOnlineData.DelMapData(pPlayerNick, false);
			m_MapReadData.AddMapData(pPlayerNick, pPlayerDataOnline, false);
		}

		return true;
	}
}

void CPlayerPoolCommand::SetServerObject(CServerObject* pServerObject)
{
	m_pServerObject = pServerObject;
}
