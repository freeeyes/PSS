#include "SMDataManager.h"

CSMDataManager::CSMDataManager(void)
{
}

CSMDataManager::~CSMDataManager(void)
{
	
}

bool CSMDataManager::Init()
{
#ifdef _WIN32	
	m_objKey = 100020;
#else
	m_objKey = ftok("/home/freeeyes/SMData/", 0);
#endif

	bool blState = m_objUserPlayerPool.Init(m_objKey, PLAYER_SMPOOL_COUNT);
	if(blState == true)
	{
		//恢复在线信息列表
		ReadSMPool();
	}

	return blState;
}

void CSMDataManager::ReadSMPool()
{
	int nCount = m_objUserPlayerPool.GetUsedObjectCount();
	for(int i = 0; i < nCount; i++)
	{
		_UserPlayer* pUserPlayer = m_objUserPlayerPool.GetUsedObject(i);
		if(pUserPlayer != NULL)
		{
			if(pUserPlayer->m_u1DataState == 0)
			{
				//添加到正在使用列表中
				mapOnlineUserPlayer::iterator f = m_mapOnlineUserPlayer.find((string)pUserPlayer->m_szPlayerNick);
				if(f == m_mapOnlineUserPlayer.end())
				{
					m_mapOnlineUserPlayer.insert(mapOnlineUserPlayer::value_type((string)pUserPlayer->m_szPlayerNick, pUserPlayer));
				}
			}
			else if(pUserPlayer->m_u1DataState == 1)
			{
				//添加到已经离开列表中
				mapOfflineUserPlayer::iterator f = m_mapLeaveUserPlayer.find((string)pUserPlayer->m_szPlayerNick);
				if(f == m_mapLeaveUserPlayer.end())
				{
					m_mapLeaveUserPlayer.insert(mapOfflineUserPlayer::value_type(pUserPlayer->m_szPlayerNick, pUserPlayer));
				}
			}
			else if(pUserPlayer->m_u1DataState == 2)
			{
				//添加到可以删除列表中
				m_listDeleteUserPlayer.push_back(pUserPlayer);

				//添加到已经离开列表中
				mapOfflineUserPlayer::iterator f = m_mapDeleteUserPlayer.find((string)pUserPlayer->m_szPlayerNick);
				if(f == m_mapDeleteUserPlayer.end())
				{
					m_mapDeleteUserPlayer.insert(mapOfflineUserPlayer::value_type(pUserPlayer->m_szPlayerNick, pUserPlayer));
				}
			}

		}
	}
}

int32 CSMDataManager::ReadPlayerSMPos(const char* pPlayerNick)
{
	//现在以后的缓冲中查找，在线表中是否存在
	mapOnlineUserPlayer::iterator f = m_mapOnlineUserPlayer.find((string)pPlayerNick);
	if(f != m_mapOnlineUserPlayer.end())
	{
		_UserPlayer* pUserPlayer = (_UserPlayer* )f->second;
		return (int32)m_objUserPlayerPool.GetDataPos(pUserPlayer);
	}

	//查找缓存数据是否已经包含了此用户
	_UserPlayer* pLeavePlayer = FindPlayerLeave(pPlayerNick);
	if(pLeavePlayer != NULL)
	{
		return (int32)m_objUserPlayerPool.GetDataPos(pLeavePlayer);
	}

	//查找可删除列表中是否包含了此用户
	_UserPlayer*  pDeletePlayer = FindPlayerDelete(pPlayerNick);
	if(pDeletePlayer != NULL)
	{
		return (int32)m_objUserPlayerPool.GetDataPos(pDeletePlayer);
	}

	//如果都没有找到缓冲数据，则去数据源去取得
	//判断共享内存中是否还有空闲
	if(m_objUserPlayerPool.GetUsedObjectCount() >= PLAYER_SMPOOL_COUNT)
	{
		//已经没有空闲了，需要清除一些数据
		bool blState = DeleteUserPlayer();
		if(blState == false)
		{
			return PLAYER_INVALID_ID;
		}
	}

	//获得PlayerID，这里做一个简单的算法，以后有数据源，请替换之
	_UserPlayer* pUserPlayer = m_objUserPlayerPool.NewObject();
	if(NULL != pUserPlayer)
	{
		pUserPlayer->m_u4PlayerID    =  GetRandNumber();
		pUserPlayer->m_u1PlayerState = 0;
		sprintf_safe(pUserPlayer->m_szPlayerNick, MAX_BUFF_20, "%s", pPlayerNick);
		pUserPlayer->m_u4Money       = 10000;
		pUserPlayer->m_u4RoomID      = 0;

		//添加到列表中
		m_mapOnlineUserPlayer.insert(mapOnlineUserPlayer::value_type((string)pUserPlayer->m_szPlayerNick, pUserPlayer));
		return (int32)m_objUserPlayerPool.GetDataPos(pUserPlayer);
	}
	else
	{
		return PLAYER_INVALID_ID;
	}
}

bool CSMDataManager::ReadDBData()
{
	//这里会预读所有的缓冲数据库中的活跃数据，在这里暂不实现
	return true;
}

int32 CSMDataManager::ReadDBData(const char* pPlayerNick)
{
	//现在以后的缓冲中查找，在线表中是否存在
	mapOnlineUserPlayer::iterator f = m_mapOnlineUserPlayer.find((string)pPlayerNick);
	if(f != m_mapOnlineUserPlayer.end())
	{
		_UserPlayer* pUserPlayer = (_UserPlayer* )f->second;
		return pUserPlayer->m_u4PlayerID;
	}


	//查找缓存数据是否已经包含了此用户
	_UserPlayer* pUserPlayer = FindPlayerLeave(pPlayerNick);
	if(pUserPlayer != NULL)
	{
		return pUserPlayer->m_u4PlayerID;
	}

	//查找可删除列表中是否包含了此用户
	_UserPlayer*  pDeletePlayer = FindPlayerDelete(pPlayerNick);
	if(pDeletePlayer != NULL)
	{
		return pDeletePlayer->m_u4PlayerID;
	}

	//如果都没有找到缓冲数据，则去数据源去取得
	//判断共享内存中是否还有空闲
	if(m_objUserPlayerPool.GetUsedObjectCount() >= PLAYER_SMPOOL_COUNT)
	{
		//已经没有空闲了，需要清除一些数据
		bool blState = DeleteUserPlayer();
		if(blState == false)
		{
			return PLAYER_INVALID_ID;
		}
	}

	//获得PlayerID，这里做一个简单的算法，以后有数据源，请替换之
	pUserPlayer = m_objUserPlayerPool.NewObject();
	if(NULL != pUserPlayer)
	{
		pUserPlayer->m_u4PlayerID    =  GetRandNumber();
		pUserPlayer->m_u1PlayerState = 0;
		sprintf_safe(pUserPlayer->m_szPlayerNick, MAX_BUFF_20, "%s", pPlayerNick);
		pUserPlayer->m_u4Money       = 10000;
		pUserPlayer->m_u4RoomID      = 0;

		//添加到列表中
		m_mapOnlineUserPlayer.insert(mapOnlineUserPlayer::value_type((string)pUserPlayer->m_szPlayerNick, pUserPlayer));
		return pUserPlayer->m_u4PlayerID;
	}
	else
	{
		return PLAYER_INVALID_ID;
	}
}

bool CSMDataManager::LeaveDBData(const char* pPlayerNick)
{
	//此数据可以标记为离开状态
	mapOnlineUserPlayer::iterator f = m_mapOnlineUserPlayer.find((string)pPlayerNick);
	if(f != m_mapOnlineUserPlayer.end())
	{
		_UserPlayer* pUserPlayer = (_UserPlayer* )f->second;
		if(NULL != pUserPlayer)
		{
			pUserPlayer->m_u1DataState = 1;
		}
		m_mapOnlineUserPlayer.erase(f);

		//插入离开表
		mapOfflineUserPlayer::iterator fl = m_mapLeaveUserPlayer.find((string)pUserPlayer->m_szPlayerNick);
		if(fl == m_mapLeaveUserPlayer.end())
		{
			m_mapLeaveUserPlayer.insert(mapOfflineUserPlayer::value_type(pUserPlayer->m_szPlayerNick, pUserPlayer));
		}
	}

	return true;
}

bool CSMDataManager::WriteDBData(const char* pPlayerNick)
{
	//写入DB数据源，并将共享内存清除
	mapOfflineUserPlayer::iterator f = m_mapLeaveUserPlayer.find(pPlayerNick);
	if(f != m_mapLeaveUserPlayer.end())
	{
		//这里执行写入数据源动作，并标记可以删除
		_UserPlayer* pUserPlayer = (_UserPlayer* )f->second;
		if(NULL != pUserPlayer)
		{
			//这里进行数据源的存储动作，目前暂时是空，由具体实现的开发者自己添加
			pUserPlayer->m_u1DataState = 2;
			//m_objUserPlayerPool.DeleteObject(pUserPlayer);
		}
		m_mapLeaveUserPlayer.erase(f);

		//添加到可以删除的map链表中去
		mapOfflineUserPlayer::iterator fDelete = m_mapDeleteUserPlayer.find((string)pUserPlayer->m_szPlayerNick);
		if(fDelete == m_mapDeleteUserPlayer.end())
		{
			m_mapDeleteUserPlayer.insert(mapOfflineUserPlayer::value_type((string)pUserPlayer->m_szPlayerNick, pUserPlayer));
		}

		m_listDeleteUserPlayer.push_back(pUserPlayer);
	}

	return true;
}

_UserPlayer* CSMDataManager::GetUserPlayer(uint32 u4Pos)
{
	return m_objUserPlayerPool.GetPosToData(u4Pos);
}

uint32 CSMDataManager::GetRandNumber()
{
	uint32 u4RandID = 0;

	ACE_OS::srand((unsigned)time(NULL));
	u4RandID = (uint32)((ACE_OS::rand() % (RANAD_MAX_NUMBER - RANAD_MIN_NUMBER)) + RANAD_MAX_NUMBER);

	return u4RandID;
}

bool CSMDataManager::DeleteUserPlayer()
{
	if(m_listDeleteUserPlayer.size() > 0)
	{
		listDeleteUserPlayer::iterator b = m_listDeleteUserPlayer.begin();
		_UserPlayer* pUserPlayer = (_UserPlayer* )*b;
		m_listDeleteUserPlayer.erase(b);

		//同时删除map中的玩家
		mapOfflineUserPlayer::iterator f = m_mapDeleteUserPlayer.find((string)pUserPlayer->m_szPlayerNick);
		if(f != m_mapDeleteUserPlayer.end())
		{
			m_mapDeleteUserPlayer.erase(f);
		}

		if(NULL != pUserPlayer)
		{
			m_objUserPlayerPool.DeleteObject(pUserPlayer);
		}

		return true;
	}
	else
	{
		return false;
	}
}

_UserPlayer* CSMDataManager::FindPlayerLeave(const char* pPlayerNick)
{
	//查找已离开的数据中是否存在当前玩家，有则加载
	mapOfflineUserPlayer::iterator f = m_mapLeaveUserPlayer.find((string)pPlayerNick);
	if(f != m_mapLeaveUserPlayer.end())
	{
		return (_UserPlayer* )f->second;
	}
	else
	{
		return NULL;
	}
}

_UserPlayer* CSMDataManager::FindPlayerDelete(const char* pPlayerNick)
{
	mapOfflineUserPlayer::iterator f = m_mapDeleteUserPlayer.find((string)pPlayerNick);
	if(f != m_mapDeleteUserPlayer.end())
	{
		return (_UserPlayer*)f->second;
	}
	else
	{
		return NULL;
	}
}

