#include "ITimerManager.h"   //--
#include "SenceManager.h"

CSenceManager::CSenceManager(void)
{
	m_blRun        = false;
	m_nTimerID     = -1;
	m_u4ProCount   = 0;
	m_u4TimeCost   = 0;
	m_pActiveTimer = NULL;
	m_pSendCommand = NULL;

	App_SenceManagerPtr::instance()->SetSenceManagerPtr(this);
}

CSenceManager::~CSenceManager(void)
{
}

void CSenceManager::Init(ActiveTimer* pActiveTimer, ISendCommand* pSendCommand)
{
	//初始化对象池
	m_objMessagePool.Init(QUEUE_COUNT);

	//初始化Buff缓冲池
	//App_BufferInfoPool::instance()->Init(BUFFER_COUNT);

	//初始化lua引擎
	m_objLuaFn.InitClass();

	bool blState = m_objLuaFn.LoadLuaFile(LUA_ROOM_PATH);
	if(false == blState)
	{
		OUR_DEBUG((LM_ERROR, "[Main]Open lua file error(%s).\n", LUA_ROOM_PATH));
	}

	m_pActiveTimer = pActiveTimer;
	m_pSendCommand = pSendCommand;

}

int CSenceManager::open(void* args)
{
	if(args != NULL)
	{
		OUR_DEBUG((LM_INFO, "[CMessageService::open]args is not NULL.\n"));
	}

	m_blRun = true;
	msg_queue()->high_water_mark(WATER_MASK);
	msg_queue()->low_water_mark(WATER_MASK);

	if(activate(THR_NEW_LWP | THR_JOINABLE | THR_INHERIT_SCHED | THR_SUSPENDED, 1) == -1)
	{
		OUR_DEBUG((LM_ERROR, "[CMessageService::open] activate error ThreadCount = [%d].", 1));
		m_blRun = false;
		return -1;
	}

	resume();

	return 0;
}

int CSenceManager::svc(void)
{
	ACE_Message_Block* mb = NULL;
	ACE_Time_Value xtime;

	ACE_OS::sleep(1);

	while(IsRun())
	{
		mb = NULL;
		//xtime = ACE_OS::gettimeofday() + ACE_Time_Value(0, MAX_MSG_PUTTIMEOUT);
		if(getq(mb, 0) == -1)
		{
			OUR_DEBUG((LM_ERROR,"[CMessageService::svc] PutMessage error errno = [%d].\n", errno));
			m_blRun = false;
			break;
		}
		if (mb == NULL)
		{
			continue;
		}

		//处理消息
		_QueueMessage* pQueueMessage = *((_QueueMessage**)mb->base());
		if (! pQueueMessage)
		{
			OUR_DEBUG((LM_ERROR,"[CMessageService::svc] mb msg == NULL CurrthreadNo=[0]!\n"));
			m_objMessageBlockPool.Delete(mb);
			m_objMessagePool.Delete(pQueueMessage);
			continue;
		}

		ACE_hrtime_t tvBegin = ACE_OS::gethrtime();
		ProcessMessage(pQueueMessage);
		m_u4TimeCost += (uint32)(ACE_OS::gethrtime() - tvBegin);
		m_u4ProCount++;

		if(m_u4ProCount == 6000)
		{
			OUR_DEBUG((LM_ERROR,"[CMessageService::svc]m_u4ProCount = %d,m_u4TimeCost = %d!\n", m_u4ProCount, m_u4TimeCost));
		}

		m_objMessageBlockPool.Delete(mb);
		m_objMessagePool.Delete(pQueueMessage);
	}

	OUR_DEBUG((LM_INFO,"[CSenceManager::svc] svc finish!\n"));
	return 0;
}

bool CSenceManager::ProcessMessage(_QueueMessage* pQueueMessage)
{
	//这里处理实际到达事件，并根据命令ID转入相应的lua接口
	if(pQueueMessage->m_u4CommandID == COMMAND_ENTERROOM)
	{
		CParamGroup objIn;
		CParamGroup objOut;

		_ParamData* pParam1 = new _ParamData((char* )&pQueueMessage->m_objData.u4RoomID, "int", sizeof(uint32));
		objIn.Push(pParam1);

		_ParamData* pParam2 = new _ParamData((char* )&pQueueMessage->m_objData.u4PlayerID, "int", sizeof(uint32));
		objIn.Push(pParam2);

		m_objLuaFn.CallFileFn("EnterRoom", objIn, objOut);
	}
	else if(pQueueMessage->m_u4CommandID == COMMAND_SETROOM)
	{
		CParamGroup objIn;
		CParamGroup objOut;

		_ParamData* pParam1 = new _ParamData((char* )&pQueueMessage->m_objData.u4RoomID, "int", sizeof(uint32));
		objIn.Push(pParam1);

		_ParamData* pParam2 = new _ParamData((char* )&pQueueMessage->m_objData.u4PlayerID, "int", sizeof(uint32));
		objIn.Push(pParam2);

		_ParamData* pParam3 = new _ParamData((char* )&pQueueMessage->m_objData.u4Data, "int", sizeof(uint32));
		objIn.Push(pParam3);

		m_objLuaFn.CallFileFn("SitRoom", objIn, objOut);
	}
	else if(pQueueMessage->m_u4CommandID == COMMAND_EXITROOM)
	{
		CParamGroup objIn;
		CParamGroup objOut;

		_ParamData* pParam1 = new _ParamData((char* )&pQueueMessage->m_objData.u4RoomID, "int", sizeof(uint32));
		objIn.Push(pParam1);

		_ParamData* pParam2 = new _ParamData((char* )& pQueueMessage->m_objData.u4PlayerID, "int", sizeof(uint32));
		objIn.Push(pParam2);

		m_objLuaFn.CallFileFn("ExitRoom", objIn, objOut);
	}
	else if(pQueueMessage->m_u4CommandID == COMMAND_BEGINROOM)
	{
		CParamGroup objIn;
		CParamGroup objOut;

		_ParamData* pParam1 = new _ParamData((char* )&pQueueMessage->m_objData.u4RoomID, "int", sizeof(uint32));
		objIn.Push(pParam1);

		_ParamData* pParam2 = new _ParamData((char* )&pQueueMessage->m_objData.u4PlayerID, "int", sizeof(uint32));
		objIn.Push(pParam2);

		_ParamData* pParam3 = new _ParamData((char* )&pQueueMessage->m_objData.u4OpreationID, "int", sizeof(uint32));
		objIn.Push(pParam3);

		m_objLuaFn.CallFileFn("Update", objIn, objOut);
	}
	else if(pQueueMessage->m_u4CommandID == COMMAND_ENDROOM)
	{
		CParamGroup objIn;
		CParamGroup objOut;

		_ParamData* pParam1 = new _ParamData((char* )&pQueueMessage->m_objData.u4RoomID, "int", sizeof(uint32));
		objIn.Push(pParam1);

		_ParamData* pParam2 = new _ParamData((char* )&pQueueMessage->m_objData.u4PlayerID, "int", sizeof(uint32));
		objIn.Push(pParam2);

		_ParamData* pParam3 = new _ParamData((char* )&pQueueMessage->m_objData.u4OpreationID, "int", sizeof(uint32));
		objIn.Push(pParam3);

		_ParamData* pParam4 = new _ParamData((char* )&pQueueMessage->m_objData.u4Data, "int", sizeof(uint32));
		objIn.Push(pParam4);

		m_objLuaFn.CallFileFn("Update", objIn, objOut);
	}
	else if(pQueueMessage->m_u4CommandID == COMMAND_OUTOLAYER)
	{
		CParamGroup objIn;
		CParamGroup objOut;

		_ParamData* pParam1 = new _ParamData((char* )&pQueueMessage->m_objData.u4RoomID, "int", sizeof(uint32));
		objIn.Push(pParam1);

		_ParamData* pParam2 = new _ParamData((char* )&pQueueMessage->m_objData.u4PlayerID, "int", sizeof(uint32));
		objIn.Push(pParam2);

		_ParamData* pParam3 = new _ParamData((char* )&pQueueMessage->m_objData.u4OpreationID, "int", sizeof(uint32));
		objIn.Push(pParam3);

		m_objLuaFn.CallFileFn("OutRoom", objIn, objOut);
	}
	else if(pQueueMessage->m_u4CommandID == COMMAND_CHANGEROOM)
	{
		CParamGroup objIn;
		CParamGroup objOut;

		_ParamData* pParam1 = new _ParamData((char* )&pQueueMessage->m_objData.u4RoomID, "int", sizeof(uint32));
		objIn.Push(pParam1);

		_ParamData* pParam2 = new _ParamData((char* )&pQueueMessage->m_objData.u4PlayerID, "int", sizeof(uint32));
		objIn.Push(pParam2);

		_ParamData* pParam3 = new _ParamData((char* )&pQueueMessage->m_objData.u4Data, "int", sizeof(uint32));
		objIn.Push(pParam3);

		m_objLuaFn.CallFileFn("ChangeRoom", objIn, objOut);
	}
	else if(pQueueMessage->m_u4CommandID == COMMAND_CLEARROOM)
	{
		CParamGroup objIn;
		CParamGroup objOut;

		m_objLuaFn.CallFileFn("ClearRoom", objIn, objOut);
	}
	else if(pQueueMessage->m_u4CommandID == COMMAND_INITROOM)
	{
		CParamGroup objIn;
		CParamGroup objOut;

		m_objLuaFn.CallFileFn("InitRoom", objIn, objOut);
	}
	else if(pQueueMessage->m_u4CommandID == COMMAND_TIMERPLAYER)
	{
		CParamGroup objIn;
		CParamGroup objOut;

		_ParamData* pParam1 = new _ParamData((char* )&pQueueMessage->m_objData.u4RoomID, "int", sizeof(uint32));
		objIn.Push(pParam1);

		_ParamData* pParam2 = new _ParamData((char* )&pQueueMessage->m_objData.u4PlayerID, "int", sizeof(uint32));
		objIn.Push(pParam2);

		_ParamData* pParam3 = new _ParamData((char* )&pQueueMessage->m_objData.u4OpreationID, "int", sizeof(uint32));
		objIn.Push(pParam3);

		m_objLuaFn.CallFileFn("PlayerTimer", objIn, objOut);
	}
	else if(pQueueMessage->m_u4CommandID == COMMAND_TIMERROOM)
	{
		CParamGroup objIn;
		CParamGroup objOut;

		_ParamData* pParam1 = new _ParamData((char* )&pQueueMessage->m_objData.u4RoomID, "int", sizeof(uint32));
		objIn.Push(pParam1);

		_ParamData* pParam2 = new _ParamData((char* )&pQueueMessage->m_objData.u4PlayerID, "int", sizeof(uint32));
		objIn.Push(pParam2);

		_ParamData* pParam3 = new _ParamData((char* )&pQueueMessage->m_objData.u4OpreationID, "int", sizeof(uint32));
		objIn.Push(pParam3);

		m_objLuaFn.CallFileFn("RoomTimer", objIn, objOut);
	}

	return true;
}

ActiveTimer* CSenceManager::GetActiveTimer()
{
	return m_pActiveTimer;
}

bool CSenceManager::PutMessage(uint32 u4CommandID, _CommandInfo& objCommandInfo)
{
	_QueueMessage* pQueueMessage = m_objMessagePool.Create();
	if(NULL == pQueueMessage)
	{
		OUR_DEBUG((LM_INFO,"[CSenceManager::PutMessage] m_objMessagePool not enougth!\n"));
		return false;
	}

	pQueueMessage->m_u4CommandID = u4CommandID;
	pQueueMessage->m_objData    = objCommandInfo;

	ACE_Message_Block* pmb = m_objMessageBlockPool.Create(sizeof(_QueueMessage*));
	if(NULL == pmb)
	{
		OUR_DEBUG((LM_INFO,"[CSenceManager::PutMessage] m_objMessageBlockPool not enougth!\n"));
		m_objMessagePool.Delete(pQueueMessage);
		return false;
	}

	_QueueMessage** ppMessage = (_QueueMessage **)pmb->base();
	*ppMessage = pQueueMessage;

	//判断队列是否是已经最大
	int nQueueCount = (int)msg_queue()->message_count();
	if(nQueueCount >= (int)QUEUE_COUNT)
	{
		OUR_DEBUG((LM_ERROR,"[CSenceManager::PutMessage] Queue is Full nQueueCount = [%d].\n", nQueueCount));
		m_objMessageBlockPool.Delete(pmb);
		m_objMessagePool.Delete(pQueueMessage);
		return false;
	}

	ACE_Time_Value xtime = ACE_OS::gettimeofday();
	if(this->putq(pmb, &xtime) == -1)
	{
		OUR_DEBUG((LM_ERROR,"[CSenceManager::PutMessage] Queue putq  error nQueueCount = [%d] errno = [%d].\n", nQueueCount, errno));
		m_objMessageBlockPool.Delete(pmb);
		m_objMessagePool.Delete(pQueueMessage);
		return false;
	}

	return true;
}

int CSenceManager::handle_timeout(const ACE_Time_Value &tv, const void *arg)
{
	if(arg != NULL)
	{
		//OUR_DEBUG((LM_ERROR,"[CMessageService::handle_timeout] arg is not NULL, time is (%d).\n", tv.sec()));
		//思考再三，决定在Timer线程里面，添加到队列中去。这样减少多并发引起的麻烦。

		_TimerInfo* pTimerInfo = (_TimerInfo* )arg;

		_CommandInfo objCommandInfo;

		objCommandInfo.u4RoomID      = pTimerInfo->m_u4RoomID;
		objCommandInfo.u4PlayerID    = pTimerInfo->m_u4PlayerID;
		objCommandInfo.u4OpreationID = pTimerInfo->m_u4Event;

		//清理一下定时器的记录信息
		if(pTimerInfo->m_u4CommandID == COMMAND_TIMERPLAYER)
		{
			App_RoomManager::instance()->SetRoomPlayerTimerID(objCommandInfo.u4RoomID, objCommandInfo.u4PlayerID, 0, TIMER_ERROR_IN, NULL);
		}
		else if(pTimerInfo->m_u4CommandID == COMMAND_TIMERROOM)
		{
			App_RoomManager::instance()->SetRoomTimerID(objCommandInfo.u4RoomID, 0, TIMER_ERROR_IN, NULL);
		}
		

		PutMessage(pTimerInfo->m_u4CommandID, objCommandInfo);

		SAFE_DELETE(pTimerInfo);
	}

	return 0;
}

int CSenceManager::Close()
{
	KillTimer();
	m_blRun = false;
	msg_queue()->deactivate();
	msg_queue()->flush();
	return 0;
}

bool CSenceManager::StartTimer()
{
	OUR_DEBUG((LM_ERROR, "CMessageService::StartTimer()-->begin....\n"));

	//m_nTimerID = App_TimerManager::instance()->schedule(this, NULL, ACE_OS::gettimeofday() + ACE_Time_Value(TIMER_CHECK), ACE_Time_Value(TIMER_CHECK));
	//if(0 == m_nTimerID)
	//{
	//	OUR_DEBUG((LM_ERROR, "CMessageService::StartTimer()--> Start thread time error.\n"));
	//	return false;
	//}

	return true;
}

bool CSenceManager::KillTimer()
{
	if(m_nTimerID > 0 && m_pActiveTimer != NULL)
	{
		m_pActiveTimer->cancel(m_nTimerID);
	}
	return true;
}

bool CSenceManager::AddSencePlayer(uint32 u4PlayerID, uint32 u4Pos)
{
	mapOnSencePlayer::iterator f = m_mapOnSencePlayer.find(u4PlayerID);
	if(f == m_mapOnSencePlayer.end())
	{
		_UserPlayer* pUserPlayer = m_objSMDataManager.GetUserPlayer(u4Pos);
		m_mapOnSencePlayer.insert(mapOnSencePlayer::value_type(u4PlayerID, pUserPlayer));
	}

	return true;
}

bool CSenceManager::DelSencePlayer(uint32 u4PlayerID)
{
	mapOnSencePlayer::iterator f = m_mapOnSencePlayer.find(u4PlayerID);
	if(f != m_mapOnSencePlayer.end())
	{
		m_mapOnSencePlayer.erase(f);
	}

	return true;
}

bool CSenceManager::IsRun()
{
	return m_blRun;
}

bool CSenceManager::SendRoomState(uint32 u4RoomID, uint32 u4PlayerID)
{
	_Send_RoomInfo objSendRoomInfo;

	_Room* pRoom = App_RoomManager::instance()->GetRoom(u4RoomID);
	if(NULL == pRoom)
	{
		return false;
	}
	else
	{
		objSendRoomInfo.m_u4RoomID = u4RoomID;
		sprintf_safe(objSendRoomInfo.m_szRoomName, MAX_BUFF_20, "%s", pRoom->m_szRoomName);
		sprintf_safe(objSendRoomInfo.m_szRoomDesc, MAX_BUFF_200, "%s", pRoom->m_szRoomDesc);
		objSendRoomInfo.m_u4PlayerCount = pRoom->m_u4PlayerCount;

		int nIndex     = 0;
		int nCardIndex = 0;
		for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
		{
			if(pRoom->m_objPlayer[i].m_u4PlayerID != PLAYER_INVALID_ID)
			{
				objSendRoomInfo.m_objSendRoomPlayer[nIndex].m_u4PlayerID  = pRoom->m_objPlayer[i].m_u4PlayerID;
				sprintf_safe(objSendRoomInfo.m_objSendRoomPlayer[nIndex].m_szPlayerNick, MAX_BUFF_20, "%s", pRoom->m_objPlayer[i].m_szPlayerNick);
				objSendRoomInfo.m_objSendRoomPlayer[nIndex].m_u1Location  = (uint8)i;
				objSendRoomInfo.m_objSendRoomPlayer[nIndex].m_u4CardCount = pRoom->m_objPlayer[i].m_u4CardCount;
				if(pRoom->m_objPlayer[i].m_u4PlayerID == u4PlayerID)
				{
					for(int j = 0; j < PLAYER_CODE_SIZE; j++)
					{
						if(pRoom->m_objPlayer[i].m_objCard[j] != CARD_INVALID_ID)
						{
							objSendRoomInfo.m_objSendRoomPlayer[nIndex].m_objCard[nCardIndex] = pRoom->m_objPlayer[i].m_objCard[j];
							nCardIndex++;
						}
					}
				}

				nIndex++;
			}
		}

		if(NULL != m_pSendCommand)
		{
			m_pSendCommand->SendRoomState(COMMAND_RETURN_USERINROOM, u4PlayerID, objSendRoomInfo);
		}

		return true;
	}
}

bool CSenceManager::SendRoomSitdown(uint32 u4RoomID, uint32 u4PlayerID)
{
	_Send_RoomPlayerInfo objSendRoomPlayerInfo;

	_Room* pRoom = App_RoomManager::instance()->GetRoom(u4RoomID);
	if(NULL == pRoom)
	{
		return false;
	}
	else
	{
		objSendRoomPlayerInfo.m_u4RoomID   = u4RoomID;
		objSendRoomPlayerInfo.m_u4PlayerID = PLAYER_INVALID_ID;
		for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
		{
			if(pRoom->m_objPlayer[i].m_u4PlayerID == u4PlayerID)
			{
				objSendRoomPlayerInfo.m_u4PlayerID = u4PlayerID;
				sprintf_safe(objSendRoomPlayerInfo.m_szPlayerNick, MAX_BUFF_20, "%s", pRoom->m_objPlayer[i].m_szPlayerNick);
				objSendRoomPlayerInfo.m_u1Location = (uint8)i;
				objSendRoomPlayerInfo.m_u4CardCount = 0;
				break;
			}
		}
	}

	if(NULL != m_pSendCommand)
	{
		//群发发送给所有在座玩家
		for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
		{
			if(pRoom->m_objPlayer[i].m_u4PlayerID != PLAYER_INVALID_ID)
			{
				m_pSendCommand->SendRoomSitdown(COMMAND_RETURN_USERSIT, pRoom->m_objPlayer[i].m_u4PlayerID, objSendRoomPlayerInfo);
			}
		}
	}

	return true;
}

bool CSenceManager::SendRoomTimer(uint32 u4RoomID, uint32 u4PlayerID)
{
	_Send_RoomTimer objSendRoomTimer;

	_Room* pRoom = App_RoomManager::instance()->GetRoom(u4RoomID);
	if(NULL == pRoom)
	{
		return false;
	}
	else
	{
		objSendRoomTimer.m_u4RoomID = u4RoomID;
		objSendRoomTimer.m_u2RoomSeconds = pRoom->GetTimerInterval();

		if(NULL != m_pSendCommand)
		{
			//群发发送给所有在座玩家
			for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
			{
				if(pRoom->m_objPlayer[i].m_u4PlayerID != PLAYER_INVALID_ID)
				{
					m_pSendCommand->SendRoomTimer(COMMAND_RETURN_ROOMTIMER, pRoom->m_objPlayer[i].m_u4PlayerID, objSendRoomTimer);
				}
			}
		}
	}

	return true;
}

bool CSenceManager::SendRoomPlayerTimer(uint32 u4RoomID, uint32 u4PlayerID)
{
	_Send_RoomPlayerTimer objSendRoomPlayerTimer;

	_Room* pRoom = App_RoomManager::instance()->GetRoom(u4RoomID);
	if(NULL == pRoom)
	{
		return false;
	}
	else
	{
		objSendRoomPlayerTimer.m_u4RoomID      = u4RoomID;
		objSendRoomPlayerTimer.m_u4PlayerID    = u4PlayerID;
		objSendRoomPlayerTimer.m_u2RoomSeconds = 0;
		for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
		{
			if(pRoom->m_objPlayer[i].m_u4PlayerID == u4PlayerID)
			{
				objSendRoomPlayerTimer.m_u2RoomSeconds = pRoom->m_objPlayer[i].GetTimerInterval();
				break;
			}
		}

		if(NULL != m_pSendCommand)
		{
			//群发发送给所有在座玩家
			for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
			{
				if(pRoom->m_objPlayer[i].m_u4PlayerID != PLAYER_INVALID_ID)
				{
					m_pSendCommand->SendRoomPlayerTimer(COMMAND_RETURN_ROOMPLAYERTIMER, pRoom->m_objPlayer[i].m_u4PlayerID, objSendRoomPlayerTimer);
				}
			}
		}
	}

	return true;
}

bool CSenceManager::SendRoomMoney(uint32 u4RoomID)
{
	_Send_RoomMoney objSendRoomMoney;
	_Room* pRoom = App_RoomManager::instance()->GetRoom(u4RoomID);
	if(NULL == pRoom)
	{
		return false;
	}
	else
	{
		objSendRoomMoney.m_u4RoomID      = pRoom->m_u4RoomID;
		objSendRoomMoney.m_u4RoomMoney   = pRoom->m_objGameInfo.m_u4BottomMoney;
		objSendRoomMoney.m_u4PlayerCount = pRoom->m_u4PlayerCount;

		for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
		{
			if(pRoom->m_objPlayer[i].m_u4PlayerID != PLAYER_INVALID_ID)
			{
				objSendRoomMoney.m_objSendRoomPlayerMoneyInfo[i].m_u4PlayerID    = pRoom->m_objPlayer[i].m_u4PlayerID;
				objSendRoomMoney.m_objSendRoomPlayerMoneyInfo[i].m_u4PlayerMoney = pRoom->m_objPlayer[i].m_u4Money;
				break;
			}
		}

		if(NULL != m_pSendCommand)
		{
			//群发发送给所有在座玩家
			for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
			{
				if(pRoom->m_objPlayer[i].m_u4PlayerID != PLAYER_INVALID_ID)
				{
					m_pSendCommand->SendRoomPlayerMoney(COMMAND_RETURN_ROOMMONEY, pRoom->m_objPlayer[i].m_u4PlayerID, objSendRoomMoney);
				}
			}
		}
	}

	return true;
}

bool CSenceManager::SendRoomTableCode(uint32 u4RoomID, uint8 u1CodseCount)
{
	_Send_RoomCodeInfo objSendRoomCodeInfo;
	_Room* pRoom = App_RoomManager::instance()->GetRoom(u4RoomID);
	if(NULL == pRoom)
	{
		return false;
	}
	else
	{
		objSendRoomCodeInfo.m_u4RoomID         = pRoom->m_u4RoomID;
		objSendRoomCodeInfo.m_u1TableCardCount = u1CodseCount;

		for(int i = 0; i < ROOM_TABLE_CARD_SIZE; i++)
		{
			objSendRoomCodeInfo.m_objCard[i] = pRoom->m_objGameInfo.m_objTableCard[i];
		}

		if(NULL != m_pSendCommand)
		{
			//群发发送给所有在座玩家
			for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
			{
				if(pRoom->m_objPlayer[i].m_u4PlayerID != PLAYER_INVALID_ID)
				{
					m_pSendCommand->SendRoomTableCard(COMMAND_RETURN_ROOMTABLECARD, pRoom->m_objPlayer[i].m_u4PlayerID, objSendRoomCodeInfo);
				}
			}
		}
	}

	return true;
}

