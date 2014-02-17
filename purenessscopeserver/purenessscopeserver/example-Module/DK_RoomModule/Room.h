#ifndef _ROOM_H
#define _ROOM_H

#include "ace/OS_NS_string.h"
#include "ace/Singleton.h"
#include "ace/Null_Mutex.h"

#include "define.h"

//房间相关信息，包括房间内的玩家信息
//add by freeeyes

//目前支持的命令
#define COMMAND_ENTERROOM    0x2003    //进入房间
#define COMMAND_EXITROOM     0x2004    //离开房间
#define COMMAND_BEGINROOM    0x2005    //开始房间
#define COMMAND_ENDROOM      0x2006    //结束房间
#define COMMAND_SETCARDROOM  0x2007    //出卡牌
#define COMMAND_OUTOLAYER    0x2008    //踢人
#define COMMAND_CHANGEROOM   0x2009    //改变房间
#define COMMAND_CLEARROOM    0x2010    //清理房间
#define COMMAND_INITROOM     0x2011    //初始化房间
#define COMMAND_TIMERPLAYER  0x2012    //玩家定时器到时
#define COMMAND_TIMERROOM    0x2013    //房间定时器到时
#define COMMAND_SETROOM      0x2014    //坐下

#define COMMAND_RETURN_USERINROOM       0xf003    //玩家进入房间
#define COMMAND_RETURN_USEROUTROOM      0xf004    //玩家离开房间
#define COMMAND_RETURN_USERSIT          0xf005    //玩家坐下
#define COMMAND_RETURN_USERINBET        0xf034    //玩家下注
#define COMMAND_RETURN_USEROVERGAME     0xf035    //玩家弃牌
#define COMMAND_RETURN_GAMEFINISH       0xf036    //玩家当前游戏结束
#define COMMAND_RETURN_ROOMTIMER        0xf037    //房间定时器开启
#define COMMAND_RETURN_ROOMPLAYERTIMER  0xf038    //房间玩家定时器开启
#define COMMAND_RETURN_ROOMMONEY        0xf039    //当前房间下注状态
#define COMMAND_RETURN_ROOMTABLECARD    0xf040    //当前房间内卡牌信息

#define PLAYER_CODE_SIZE          5    //玩家卡牌个数
#define ROOM_PLAYER_SIZE          5    //房间内玩家参加的最大人数
#define ROOM_PLAYER_VISITOR_SIZE  5    //观察者玩家的最大个数
#define ROOM_TABLE_CARD_SIZE      5    //房间内桌面上的牌
#define ROOM_CARD_SIZE            52   //房间内备牌的数量
#define ROOM_COUNT                50   //最大房间数

#define ROOM_ERROR_NULL           0    //操作成功
#define ROOM_ERROR_NOEXIST        -1   //房间ID不存在
#define ROOM_ERROR_NOBEGIN        -2   //房间已经开始，不允许进入
#define ROOM_ERROR_PLAYERFULL     -3   //房间人已经满了
#define ROOM_ERROR_PLAYERNOEXIST  -4   //指定ID的玩家不在房间
#define ROOM_ERROR_ISBEGIN        -5   //房间已经开始，不能重复开始
#define ROOM_ERROR_ISNOBEGIN      -6   //房间没有开始，不能结束
#define ROOM_ERROR_PLAYERCARDFULL -7   //用户身上的卡片已满不能再添加
#define ROOM_ERROR_PLAYERNOCARD   -8   //找不到玩家身上指定位置的卡片
#define ROOM_ERROR_PLAYEREXIST    -9   //玩家已经存在在指定位置上
#define ROOM_ERROR_PLAYERLOCATION -10  //超出座位玩家范围
#define ROOM_ERROR_PLAYERSITEXIST -11  //玩家已经入座
#define ROOM_ERROR_PLAYERVISEXIST -12  //玩家已经在观察者中
#define ROOM_ERROR_PLAYERNOBET    -13  //不是应该这个玩家下注
#define ROOM_ERROR_PLAYERMONONT   -14  //玩家身上金钱不够

#define GAMEINFO_BOTTOMONEY       1000 //默认底池
#define GAMEINFO_SMALLBLIND       1000 //默认小盲注

#define ROOM_BUFFPOLL_INVALID     -1   //无效的BuffID，BuffPool里面已经没有空余的对象
#define TIMER_ERROR_IN            -1   //无效的定时器

//房间状态
enum
{
	ROOMSTATE_REDAY = 0,    //房间装备好
	ROOMSTATE_ERROR,        //房间有异常
	ROOMSTATE_XIAZHU,       //房间有人需要下注
	ROOMSTATE_FINISH        //房间牌局结束
};

#define PLAYER_INVALID_ID         -1   //无效的PlayerID
#define ROOM_INVALID_ID           -1   //无效的RoomID
#define CARD_INVALID_ID           -1   //无效的卡片

//定时器参数结构体
struct _TimerInfo
{
	uint32 m_u4CommandID;
	uint32 m_u4RoomID;
	uint32 m_u4PlayerID;
	uint32 m_u4Event;

	_TimerInfo()
	{
		m_u4CommandID = 0;
		m_u4RoomID    = 0;
		m_u4PlayerID  = 0;
		m_u4Event     = 0;
	}
};

//游戏相关参数记录信息
struct _GameInfo
{
	uint16 m_u2Postion;                                   //出牌的当前位置
	uint16 m_u2GameState;                                 //房间牌局状态
	uint16 m_u2CurrentSmallBlind;                         //当前小盲注
	uint32 m_u4BottomMoney;                               //当前底池 
	uint16 m_u2RoundCount;                                //牌局当前轮数
	uint32 m_u4TempBet;                                   //牌局最大注数
	uint32 m_u4Money;                                     //牌桌上的钱
	uint32 m_u4CurrPlayerID;                              //当前玩家ID
	uint32 m_u4BeginPlayerID;                             //开始的玩家ID
	uint32 m_objTableCard[ROOM_TABLE_CARD_SIZE];          //桌上牌桌信息
	uint32 m_objBankCard[ROOM_CARD_SIZE];                 //牌库

	_GameInfo()
	{
		Clear();
	}

	void Clear()
	{
		m_u2Postion           = 0;
		m_u2GameState         = 0;
		m_u2CurrentSmallBlind = GAMEINFO_SMALLBLIND;
		m_u2RoundCount        = 0;
		m_u4TempBet           = 0;
		m_u4Money             = 0;
		m_u4CurrPlayerID      = PLAYER_INVALID_ID;
		m_u4BeginPlayerID     = PLAYER_INVALID_ID;
		m_u4BottomMoney       = GAMEINFO_BOTTOMONEY;

		for(int i = 0; i < ROOM_CARD_SIZE; i++)
		{
			m_objTableCard[i] = CARD_INVALID_ID;
		}

		for(int i = 0; i < ROOM_CARD_SIZE; i++)
		{

			m_objBankCard[i] = CARD_INVALID_ID;
		}
	}

};

//玩家信息
struct _Player
{
	uint32 m_u4PlayerID;                  //玩家ID
	char   m_szPlayerNick[MAX_BUFF_20];   //玩家昵称 
	uint32 m_u4Money;                     //玩家金钱
	uint32 m_objCard[PLAYER_CODE_SIZE];   //玩家手上的牌
	uint32 m_u4CardCount;                 //当前拥有牌的个数
	uint32 m_u4TimerID;                   //当前玩家身上的定时器
	_TimerInfo* m_pTimerInfo;             //当前的玩家定时器附加信息
	ACE_Time_Value m_tvUpdate;            //当前玩家更新最后时间
	ACE_Time_Value m_tvTimerArrived;      //定时器到达执行任务的时间

	//获得用户定时器到达时间
	ACE_Time_Value GetTimerArrived()
	{
		return m_tvTimerArrived;
	}

	//设置用户定时器到达时间
	void SetTimerArrived(uint16 u2Seconds)
	{
		m_tvTimerArrived = ACE_OS::gettimeofday() + ACE_Time_Value(u2Seconds);
	}

	//获得用户定时器时间间隔，0为没有间隔
	uint16 GetTimerInterval()
	{
		uint16 n2Second = 0;
		if(m_tvTimerArrived > ACE_OS::gettimeofday())
		{
			ACE_Time_Value tvInterval = (m_tvTimerArrived - ACE_OS::gettimeofday());
			n2Second = (uint16)tvInterval.sec();
		}
		return n2Second;
	}

	_Player()
	{
		m_u4TimerID       = TIMER_ERROR_IN;
		m_pTimerInfo      = NULL;
		Clear();
	}

	void Clear()
	{
		m_u4PlayerID      = PLAYER_INVALID_ID;
		m_u4Money         = 0;
		m_u4CardCount     = 0;
		m_szPlayerNick[0] = '\0';
		m_tvUpdate        = ACE_OS::gettimeofday();

		for(int i = 0; i < PLAYER_CODE_SIZE; i++)
		{
			m_objCard[i] = CARD_INVALID_ID;
		}
		m_u4CardCount = 0;

		if(m_pTimerInfo != NULL)
		{
			//如果Timer附加的指针没有得到释放，则在这里提出警告
			OUR_DEBUG((LM_ERROR, "[_Player::Clear]m_pTimerInfo is not NULL.\n"));
		}

		m_u4TimerID       = TIMER_ERROR_IN;
		m_pTimerInfo      = NULL;
	}
};

//房间结构体
struct _Room
{
	uint32      m_u4RoomID;                                    //房间ID
	char        m_szRoomName[MAX_BUFF_20];                     //房间名称
	char        m_szRoomDesc[MAX_BUFF_200];                    //房间信息描述 
	_Player     m_objPlayer[ROOM_PLAYER_SIZE];                 //最大房间内的玩家
	_Player     m_objVisitPlayer[ROOM_PLAYER_VISITOR_SIZE];    //最大房间参观者玩家
	bool        m_blState;                                     //是否开放，true开放，false不开放
	uint32      m_u4State;                                     //房间状态,0为准备开始，1为已经开始
	uint32      m_u4PlayerCount;                               //玩家个数
	uint32      m_u4VisitorCount;                              //参观玩家个数
	uint32      m_u4TimerID;                                   //房间定时器ID
	_TimerInfo* m_pTimerInfo;                                  //房间定时器的附加参数
	ACE_Time_Value m_tvUpdate;                                 //当前房间更新最后时间
	ACE_Time_Value m_tvTimerArrived;                           //定时器到达执行任务的时间

	_GameInfo   m_objGameInfo;                                 //房间游戏相关信息

	_Room()
	{
		m_u4RoomID = ROOM_ERROR_NOEXIST;
		m_u4TimerID  = TIMER_ERROR_IN;
		m_pTimerInfo = NULL;
		Clear();
	}

	//清理房间
	void Clear()
	{
		for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
		{
			m_objPlayer[i].m_u4PlayerID = PLAYER_INVALID_ID;
		}

		for(int i = 0; i < ROOM_PLAYER_VISITOR_SIZE; i++)
		{
			m_objVisitPlayer[i].m_u4PlayerID = PLAYER_INVALID_ID;
		}

		if(m_pTimerInfo != NULL)
		{
			//如果Timer附加的指针没有得到释放，则在这里提出警告
			OUR_DEBUG((LM_ERROR, "[_Room::Clear]m_pTimerInfo is not NULL.\n"));
		}

		m_u4TimerID  = TIMER_ERROR_IN;
		m_pTimerInfo = NULL;

		m_u4PlayerCount  = 0;
		m_u4VisitorCount = 0;
		m_blState        = true;
		m_u4State        = 0;
		m_tvUpdate       = ACE_OS::gettimeofday();
	}

	//获得房间定时器到达时间
	ACE_Time_Value GetTimerArrived()
	{
		return m_tvTimerArrived;
	}

	//设置房间定时器到达时间
	void SetTimerArrived(uint16 u2Seconds)
	{
		m_tvTimerArrived = ACE_OS::gettimeofday() + ACE_Time_Value(u2Seconds);
	}

	//获得房间定时器时间间隔，0为没有间隔
	uint16 GetTimerInterval()
	{
		uint16 n2Second = 0;
		if(m_tvTimerArrived > ACE_OS::gettimeofday())
		{
			ACE_Time_Value tvInterval = (m_tvTimerArrived - ACE_OS::gettimeofday());
			n2Second = (uint16)tvInterval.sec();
		}
		return n2Second;
	}
};

//房间管理类，管理所有的房间
struct _RoomManager
{
	_Room m_objRoom[ROOM_COUNT];

	_RoomManager()
	{
		for(int i = 0; i < ROOM_COUNT; i++)
		{
			m_objRoom[i].m_u4RoomID = i;
		}
	}

	_Room* GetRoom(uint32 u4RoomID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return NULL;
		}
		else
		{
			return &m_objRoom[u4RoomID];
		}
	}

	//设置开始的玩家ID
	uint32 SetRoomGame_BenginePlayerID(uint32 u4RoomID, uint32 u4PlayerID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			m_objRoom[u4RoomID].m_objGameInfo.m_u4BeginPlayerID = u4PlayerID;
			return ROOM_ERROR_NULL;
		}
	}

	//获得开始的玩家ID
	uint32 GetRoomGame_BenginePlayerID(uint32 u4RoomID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			return m_objRoom[u4RoomID].m_objGameInfo.m_u4BeginPlayerID;
		}
	}

	//得到当前需要操作的玩家ID
	uint32 GetCurrPlayerID(uint32 u4RoomID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			return m_objRoom[u4RoomID].m_objPlayer[m_objRoom[u4RoomID].m_objGameInfo.m_u2Postion].m_u4PlayerID;
		}
	}

	//下注
	uint32 SetBet(uint32 u4RoomID, uint32 u4PlayerID, uint32 u4Money)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			//如果不是应该下注的人，则什么都不做
			if(m_objRoom[u4RoomID].m_objPlayer[m_objRoom[u4RoomID].m_objGameInfo.m_u2Postion].m_u4PlayerID != u4PlayerID)
			{
				return ROOM_ERROR_PLAYERNOBET;
			}
			else
			{
				//开始下注
				uint32 u4PlayerMoney = GetPlayerMoney(u4RoomID, u4PlayerID);
				if(u4PlayerMoney < u4Money)
				{
					return ROOM_ERROR_PLAYERMONONT;
				}
				else
				{
					//从玩家身上扣除金钱，加入牌局金钱池中去
					SetPlayerMoney(u4RoomID, u4PlayerID, u4PlayerMoney - u4Money);
					m_objRoom[u4RoomID].m_objGameInfo.m_u4Money += u4Money;
					return ROOM_ERROR_NULL;
				}	
			}
		}
	}

	//发牌动作
	uint32 DealCode(uint32 u4RoomID, uint32 u4BottomMoney, int32 n4PlayerID, int32 n4SmallBlind)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			//初始化底池，小盲注
			m_objRoom[u4RoomID].m_objGameInfo.m_u4BottomMoney = u4BottomMoney;
			if(n4SmallBlind != -1)
			{
				m_objRoom[u4RoomID].m_objGameInfo.m_u2CurrentSmallBlind = n4SmallBlind;
			}

			int i = 0;
			//洗牌
			vector<uint32> vecCodeBank;
			for (i = 0 ;i < ROOM_TABLE_CARD_SIZE; i++)
			{
				vecCodeBank.push_back(i);
			}
			random_shuffle(vecCodeBank.begin(), vecCodeBank.end());

			//插入牌库
			for (i = 0 ;i < ROOM_TABLE_CARD_SIZE; i++)
			{
				m_objRoom[u4RoomID].m_objGameInfo.m_objBankCard[i] = (uint32)vecCodeBank[i];
			}

			int nCodeBankIndex = 0;
			//开始发牌(给在座的玩家)
			for(i = 0; i < ROOM_PLAYER_SIZE; i++)
			{
				if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID != PLAYER_INVALID_ID)
				{
					m_objRoom[u4RoomID].m_objPlayer[i].m_objCard[0] = m_objRoom[u4RoomID].m_objGameInfo.m_objBankCard[nCodeBankIndex];
					nCodeBankIndex++;
					m_objRoom[u4RoomID].m_objPlayer[i].m_objCard[1] = m_objRoom[u4RoomID].m_objGameInfo.m_objBankCard[nCodeBankIndex];
					nCodeBankIndex++;
					m_objRoom[u4RoomID].m_objPlayer[i].m_u4CardCount = 2;
				}
			}

			//开始发牌，发牌桌上的牌
			for(i = 0; i < ROOM_TABLE_CARD_SIZE; i++)
			{
				m_objRoom[u4RoomID].m_objGameInfo.m_objTableCard[i] =  m_objRoom[u4RoomID].m_objGameInfo.m_objBankCard[nCodeBankIndex];
				nCodeBankIndex++;
			}

			//设置第一个出牌者
			if(n4PlayerID != -1)
			{
				int16 n2Location = GetRoomPlayerLocation(u4RoomID, n4PlayerID);
				if(n2Location > 0)
				{
					//设置开始位置
					m_objRoom[u4RoomID].m_objGameInfo.m_u2Postion = n2Location;
				}
				else
				{
					return ROOM_ERROR_PLAYERNOEXIST;
				}
			}

			//开始扣除底池
			for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
			{
				if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID != PLAYER_INVALID_ID)
				{
					if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4Money >= m_objRoom[u4RoomID].m_objGameInfo.m_u4BottomMoney)
					{
						m_objRoom[u4RoomID].m_objPlayer[i].m_u4Money -= m_objRoom[u4RoomID].m_objGameInfo.m_u4BottomMoney;
						m_objRoom[u4RoomID].m_objGameInfo.m_u4Money += m_objRoom[u4RoomID].m_objGameInfo.m_u4BottomMoney;
					}
					else
					{
						m_objRoom[u4RoomID].m_objPlayer[i].m_u4Money = 0;
						m_objRoom[u4RoomID].m_objGameInfo.m_u4Money += m_objRoom[u4RoomID].m_objPlayer[i].m_u4Money;
					}
				}
			}

			//开始按顺序扣减小盲注，座位向下移动一个
			uint32 u4BeginPlayerID = GetRoomNextPlayer(u4RoomID);
			if(u4BeginPlayerID >= 0)
			{
				//扣除小盲注
				uint32 u4CurrPlayerMoney = GetPlayerMoney(u4RoomID, u4BeginPlayerID);
				if(u4CurrPlayerMoney > 0)
				{
					if(u4CurrPlayerMoney < m_objRoom[u4RoomID].m_objGameInfo.m_u2CurrentSmallBlind)
					{
						SetPlayerMoney(u4RoomID, u4BeginPlayerID, 0);
						m_objRoom[u4RoomID].m_objGameInfo.m_u4Money += u4CurrPlayerMoney;
					}
					else
					{
						SetPlayerMoney(u4RoomID, u4BeginPlayerID, u4CurrPlayerMoney - m_objRoom[u4RoomID].m_objGameInfo.m_u2CurrentSmallBlind);
						m_objRoom[u4RoomID].m_objGameInfo.m_u4Money += m_objRoom[u4RoomID].m_objGameInfo.m_u2CurrentSmallBlind;
					}
				}
			}

			//开始扣减大盲注，座位向下移动一个
			u4BeginPlayerID = GetRoomNextPlayer(u4RoomID);
			if(u4BeginPlayerID >= 0)
			{
				//扣除大盲注
				uint32 u4CurrPlayerMoney = GetPlayerMoney(u4RoomID, u4BeginPlayerID);
				if(u4CurrPlayerMoney > 0)
				{
					if(u4CurrPlayerMoney < (uint32)(m_objRoom[u4RoomID].m_objGameInfo.m_u2CurrentSmallBlind * 2))
					{
						SetPlayerMoney(u4RoomID, u4BeginPlayerID, 0);
						m_objRoom[u4RoomID].m_objGameInfo.m_u4Money += u4CurrPlayerMoney;
					}
					else
					{
						SetPlayerMoney(u4RoomID, u4BeginPlayerID, u4CurrPlayerMoney - m_objRoom[u4RoomID].m_objGameInfo.m_u2CurrentSmallBlind * 2);
						m_objRoom[u4RoomID].m_objGameInfo.m_u4Money += m_objRoom[u4RoomID].m_objGameInfo.m_u2CurrentSmallBlind * 2;
					}
				}
			}

			//设置房间为下注状态
			m_objRoom[u4RoomID].m_u4State = ROOMSTATE_XIAZHU;

			return ROOM_ERROR_NULL;
		}
	}

	//得到下一个在座的玩家，如果到了末尾，则变成第一个
	uint32 GetRoomNextPlayer(uint32 u4RoomID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			//只有一个玩家的时候，返回-1
			if(m_objRoom[u4RoomID].m_u4PlayerCount <= 1)
			{
				return PLAYER_INVALID_ID;
			}

			uint32 u4PlayerID = PLAYER_INVALID_ID;
			if(m_objRoom[u4RoomID].m_objGameInfo.m_u2Postion == 0)
			{
				//判断第0个是否有玩家
				if(m_objRoom[u4RoomID].m_objPlayer[m_objRoom[u4RoomID].m_objGameInfo.m_u2Postion].m_u4PlayerID == PLAYER_INVALID_ID)
				{
					//寻找下一个用户下标
					bool blState = false;
					for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
					{
						if( m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID != PLAYER_INVALID_ID)
						{
							m_objRoom[u4RoomID].m_objGameInfo.m_u2Postion = i;
							blState                                       = true;
							u4PlayerID                                    = m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID;
						}
					}

					//当前房间内没有玩家
					if(blState == false)
					{
						return PLAYER_INVALID_ID;
					}
				}
				else
				{
					u4PlayerID = m_objRoom[u4RoomID].m_objPlayer[0].m_u4PlayerID;
				}
			}
			else
			{
				u4PlayerID = m_objRoom[u4RoomID].m_objPlayer[m_objRoom[u4RoomID].m_objGameInfo.m_u2Postion].m_u4PlayerID;
			}

			//寻找下一个用户下标
			bool blState = false;
			for(int i = m_objRoom[u4RoomID].m_objGameInfo.m_u2Postion + 1; i < ROOM_PLAYER_SIZE; i++)
			{
				if( m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID != PLAYER_INVALID_ID)
				{
					m_objRoom[u4RoomID].m_objGameInfo.m_u2Postion = i;
					blState                                       = true;
					break;
				}
			}

			if(blState == false)
			{
				//寻找下一个用户下标
				bool blState = false;
				for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
				{
					if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID != PLAYER_INVALID_ID)
					{
						m_objRoom[u4RoomID].m_objGameInfo.m_u2Postion = i;
						blState                                       = true;
						break;
					}
				}

				//当前房间内没有玩家
				if(blState == false)
				{
					return PLAYER_INVALID_ID;
				}
			}

			//如果当前ID和BeginID相同，则轮次+1
			if(m_objRoom[u4RoomID].m_objGameInfo.m_u4BeginPlayerID == u4PlayerID)
			{
				m_objRoom[u4RoomID].m_objGameInfo.m_u2RoundCount++;
			}

			return u4PlayerID;
		}
	}

	uint32 SetRoomUpdateTime(uint32 u4RoomID)    //设置当前房间最后更新时间
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			m_objRoom[u4RoomID].m_tvUpdate = ACE_OS::gettimeofday();
			return ROOM_ERROR_NULL;
		}
	}

	bool CheckRoomUpdateTime(uint32 u4RoomID, uint32 u4MillSeconds)    //设置当前房间最后更新时间,u4MillSeconds单位是毫秒
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return false;
		}
		else
		{
			ACE_Time_Value tvDispose = ACE_OS::gettimeofday() - m_objRoom[u4RoomID].m_tvUpdate;
			if(u4MillSeconds >= (uint32)tvDispose.msec())
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}

	uint32 SetRoomPlayerUpdateTime(uint32 u4RoomID, uint32 u4PlayerID)    //设置当前房间玩家最后更新时间
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
			{
				if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID == u4PlayerID)
				{
					m_objRoom[u4RoomID].m_objPlayer[i].m_tvUpdate = ACE_OS::gettimeofday();
					return ROOM_ERROR_NULL;
				}
			}
			
			return ROOM_ERROR_PLAYERNOEXIST;
		}
	}

	bool CheckRoomPlayerUpdateTime(uint32 u4RoomID, uint32 u4PlayerID, uint32 u4MillSeconds)    //设置当前房间最后更新时间,u4MillSeconds单位是毫秒
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return false;
		}
		else
		{
			for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
			{
				if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID == u4PlayerID)
				{
					ACE_Time_Value tvDispose = ACE_OS::gettimeofday() - m_objRoom[u4RoomID].m_objPlayer[i].m_tvUpdate;
					if(u4MillSeconds >= (uint32)tvDispose.msec())
					{
						return false;
					}
					else
					{
						return true;
					}
				}
			}

			return false;
		}
	}

	uint32 GetRoomGame_Postion(uint32 u4RoomID)     //得到当前房间游戏信息
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			return m_objRoom[u4RoomID].m_objGameInfo.m_u2Postion;
		}
	}

	uint32 SetRoomGame_Postion(uint32 u4RoomID, uint16 u2Postion)     //设置当前房间游戏信息
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			m_objRoom[u4RoomID].m_objGameInfo.m_u2Postion = u2Postion;
			return ROOM_ERROR_NULL;
		}
	}

	uint32 GetRoomGame_GameState(uint32 u4RoomID)     //得到当前房间游戏信息
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			return m_objRoom[u4RoomID].m_objGameInfo.m_u2GameState;
		}
	}

	uint32 SetRoomGame_GameState(uint32 u4RoomID, uint16 u2GameState)     //设置当前房间游戏信息
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			m_objRoom[u4RoomID].m_objGameInfo.m_u2GameState = u2GameState;
			return ROOM_ERROR_NULL;
		}
	}

	uint32 GetRoomGame_CurrentSmallBlind(uint32 u4RoomID)     //得到当前房间游戏信息
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			return m_objRoom[u4RoomID].m_objGameInfo.m_u2CurrentSmallBlind;
		}
	}

	uint32 SetRoomGame_CurrentSmallBlind(uint32 u4RoomID, uint16 u2CurrentSmallBlind)     //设置当前房间游戏信息
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			m_objRoom[u4RoomID].m_objGameInfo.m_u2CurrentSmallBlind = u2CurrentSmallBlind;
			return ROOM_ERROR_NULL;
		}
	}

	uint32 GetRoomGame_RoundCount(uint32 u4RoomID)     //得到当前房间游戏信息
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			return m_objRoom[u4RoomID].m_objGameInfo.m_u2RoundCount;
		}
	}

	uint32 SetRoomGame_RoundCount(uint32 u4RoomID, uint16 u2RoundCount)     //设置当前房间游戏信息
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			m_objRoom[u4RoomID].m_objGameInfo.m_u2RoundCount = u2RoundCount;
			return ROOM_ERROR_NULL;
		}
	}

	uint32 GetRoomGame_TempBet(uint32 u4RoomID)     //得到当前房间游戏信息
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			return m_objRoom[u4RoomID].m_objGameInfo.m_u4TempBet;
		}
	}

	uint32 SetRoomGame_TempBet(uint32 u4RoomID, uint32 u4TempBat)     //设置当前房间游戏信息
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			m_objRoom[u4RoomID].m_objGameInfo.m_u4TempBet = u4TempBat;
			return ROOM_ERROR_NULL;
		}
	}

	uint32 GetRoomGame_Money(uint32 u4RoomID)     //得到当前房间游戏信息
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			return m_objRoom[u4RoomID].m_objGameInfo.m_u4Money;
		}
	}

	uint32 SetRoomGame_Money(uint32 u4RoomID, uint32 u4Money)     //设置当前房间游戏信息
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			m_objRoom[u4RoomID].m_objGameInfo.m_u4Money = u4Money;
			return ROOM_ERROR_NULL;
		}
	}

	uint32 GetRoomGame_CurrPlayerID(uint32 u4RoomID)     //得到当前房间游戏信息
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			return m_objRoom[u4RoomID].m_objGameInfo.m_u4CurrPlayerID;
		}
	}

	uint32 SetRoomGame_CurrPlayerID(uint32 u4RoomID, uint32 u4CurrPlayerID)     //设置当前房间游戏信息
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			m_objRoom[u4RoomID].m_objGameInfo.m_u4CurrPlayerID = u4CurrPlayerID;
			return ROOM_ERROR_NULL;
		}
	}

	uint32 GetRoomPlayerCount(uint32 u4RoomID)     //得到当前玩家的数量
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			return m_objRoom[u4RoomID].m_u4PlayerCount;
		}
	}

	uint32 GetRoomVisitorCount(uint32 u4RoomID)     //得到参观玩家数量
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			return m_objRoom[u4RoomID].m_u4VisitorCount;
		}
	}

	//得到指定的玩家信息(这里的u4Index是格子的下标)
	char* GetRoomPlayerName(uint32 u4RoomID, uint32 u4Index)
	{
		if(u4Index < 0 || u4Index >= ROOM_PLAYER_SIZE)
		{
			return NULL;
		}

		uint32 u4CurrIndex = 0;
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return NULL;
		}
		else
		{
			if(m_objRoom[u4RoomID].m_objPlayer[u4Index].m_u4PlayerID != PLAYER_INVALID_ID)
			{
				return m_objRoom[u4RoomID].m_objPlayer[u4Index].m_szPlayerNick;
			}
			else
			{
				return NULL;
			}
		}
	}

	//得到房间名称
	const char* GetRoomName(uint32 u4RoomID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return NULL;
		}
		else
		{
			return m_objRoom[u4RoomID].m_szRoomName;
		}
	}

	//设置房间名称
	uint32 SetRoomName(uint32 u4RoomID, const char* pRoomName)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			sprintf_safe(m_objRoom[u4RoomID].m_szRoomName, MAX_BUFF_20, "%s", pRoomName);
			return ROOM_ERROR_NULL;
		}
	}

	//得到房间描述
	const char* GetRoomDesc(uint32 u4RoomID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return NULL;
		}
		else
		{
			return m_objRoom[u4RoomID].m_szRoomDesc;
		}
	}

	//得到房间状态
	uint32 GetRoomState(uint32 u4RoomID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			return m_objRoom[u4RoomID].m_u4State;
		}
	}

	//设置房间状态
	uint32 SetRoomState(uint32 u4RoomID, uint32 u4State)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			m_objRoom[u4RoomID].m_u4State = u4State;
			return ROOM_ERROR_NULL;
		}
	}

	//设置房间描述信息
	uint32 SetRoomDesc(uint32 u4RoomID, const char* pRoomDesc)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			sprintf_safe(m_objRoom[u4RoomID].m_szRoomDesc, MAX_BUFF_200, "%s", pRoomDesc);
			return ROOM_ERROR_NULL;
		}
	}

	//得到Room的定时器ID
	uint32 GetRoomTimerID(uint32 u4RoomID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			return m_objRoom[u4RoomID].m_u4TimerID;
		}
	}

	//从观察的玩家指定到固定位置游戏的玩家,如果n2Location是-1,则是按顺序坐下，否则是指定的位置。
	uint32 SetRoomPlayerSitDown(uint32 u4RoomID, uint32 u4PlayerID, int16 n2Location)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(n2Location < -1 || n2Location >= ROOM_PLAYER_SIZE)
			{
				return ROOM_ERROR_PLAYERLOCATION;
			}
			else
			{
				for(int i = 0; i < ROOM_PLAYER_VISITOR_SIZE; i++)
				{
					if(m_objRoom[u4RoomID].m_objVisitPlayer[i].m_u4PlayerID == u4PlayerID)
					{
						if(n2Location == -1)
						{
							uint32 uRet = JoinRoom(u4RoomID, u4PlayerID, m_objRoom[u4RoomID].m_objVisitPlayer[i].m_szPlayerNick, m_objRoom[u4RoomID].m_objVisitPlayer[i].m_u4Money);
							if(uRet == ROOM_ERROR_NULL)
							{
								m_objRoom[u4RoomID].m_objVisitPlayer[i].Clear();
								m_objRoom[u4RoomID].m_u4VisitorCount--;
								return ROOM_ERROR_NULL;
							}
							else
							{
								return uRet;
							}
						}
						else
						{
							uint32 uRet = JoinRoom(u4RoomID, u4PlayerID, m_objRoom[u4RoomID].m_objVisitPlayer[i].m_szPlayerNick, m_objRoom[u4RoomID].m_objVisitPlayer[i].m_u4Money, (uint16)n2Location);
							if(uRet == ROOM_ERROR_NULL)
							{
								m_objRoom[u4RoomID].m_objVisitPlayer[i].Clear();
								m_objRoom[u4RoomID].m_u4VisitorCount--;
								return ROOM_ERROR_NULL;
							}
							else
							{
								return uRet;
							}
						}
					}
				}

				return ROOM_ERROR_PLAYERNOEXIST;
			}
		}
	}

	//设置Room的定时器ID
	uint32 SetRoomTimerID(uint32 u4RoomID, uint32 u4TimerID, uint16 u2Seconds, _TimerInfo* pTimerInfo)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			m_objRoom[u4RoomID].m_u4TimerID  = u4TimerID;
			m_objRoom[u4RoomID].m_pTimerInfo = pTimerInfo;
			if(u2Seconds > 0)
			{
				m_objRoom[u4RoomID].SetTimerArrived(u2Seconds);
			}
			return ROOM_ERROR_NULL;
		}
	}

	//得到Room的定时器附加信息
	_TimerInfo*  GetRoomTimerInfo(uint32 u4RoomID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return NULL;
		}
		else
		{
			return m_objRoom[u4RoomID].m_pTimerInfo;
		}
	}

	//得到玩家的定时器ID
	uint32 GetRoomPlayerTimerID(uint32 u4RoomID, uint32 u4PlayerID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(m_objRoom[u4RoomID].m_u4State == 0)
			{
				if(m_objRoom[u4RoomID].m_u4PlayerCount <= 0)
				{
					return ROOM_ERROR_PLAYERNOEXIST;
				}
				else
				{
					for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
					{
						if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID == u4PlayerID)
						{
							return m_objRoom[u4RoomID].m_objPlayer[i].m_u4TimerID;
						}
					}

					return ROOM_ERROR_PLAYERNOEXIST;
				}
			}
			else
			{
				return ROOM_ERROR_NOBEGIN;
			}
		}
	}

	//得到玩家身上的TimerInfo;
	_TimerInfo*  GetRoomPlayerTimerInfo(uint32 u4RoomID, uint32 u4PlayerID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return NULL;
		}
		else
		{
			if(m_objRoom[u4RoomID].m_u4State == 0)
			{
				if(m_objRoom[u4RoomID].m_u4PlayerCount <= 0)
				{
					return NULL;
				}
				else
				{
					for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
					{
						if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID == u4PlayerID)
						{
							return m_objRoom[u4RoomID].m_objPlayer[i].m_pTimerInfo;
						}
					}

					return NULL;
				}
			}
			else
			{
				return NULL;
			}
		}
	}

	//设置玩家身上的定时器ID
	uint32  SetRoomPlayerTimerID(uint32 u4RoomID, uint32 u4PlayerID, uint16 u2Seconds, uint32 u4TimerID, _TimerInfo* pTimerInfo)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(m_objRoom[u4RoomID].m_u4State == 0)
			{
				if(m_objRoom[u4RoomID].m_u4PlayerCount <= 0)
				{
					return ROOM_ERROR_PLAYERNOEXIST;
				}
				else
				{
					for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
					{
						if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID == u4PlayerID)
						{
							m_objRoom[u4RoomID].m_objPlayer[i].m_u4TimerID  = u4TimerID;
							m_objRoom[u4RoomID].m_objPlayer[i].m_pTimerInfo = pTimerInfo;
							if(u2Seconds > 0)
							{
								m_objRoom[u4RoomID].m_objPlayer[i].SetTimerArrived(u2Seconds);
							}
							return ROOM_ERROR_NULL;
						}
					}

					return ROOM_ERROR_PLAYERNOEXIST;
				}
			}
			else
			{
				return ROOM_ERROR_NOBEGIN;
			}
		}
	}

	//根据RoomID和PlayerID获得当前房间的位置
	uint32 GetRoomPlayerLocation(uint32 u4RoomID, uint32 u4PlayerID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			int nIndex = 0;
			for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
			{
				if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID == u4PlayerID)
				{
					return nIndex;
				}
				nIndex++;
			}

			return ROOM_ERROR_PLAYERNOEXIST;
		}
	}

	//进入房间(固定位置)
	uint32 JoinRoom(uint32 u4RoomID, uint32 u4PlayerID, const char* pPlayerName, uint32 u4Money, uint16 nIndex)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(m_objRoom[u4RoomID].m_objPlayer[nIndex].m_u4PlayerID == PLAYER_INVALID_ID)
			{
				m_objRoom[u4RoomID].m_objPlayer[nIndex].m_u4PlayerID = u4PlayerID;
				m_objRoom[u4RoomID].m_objPlayer[nIndex].m_u4Money    = u4Money;
				m_objRoom[u4RoomID].m_u4PlayerCount++;
				sprintf_safe(m_objRoom[u4RoomID].m_objPlayer[nIndex].m_szPlayerNick, MAX_BUFF_20, "%s", pPlayerName);
				return ROOM_ERROR_NULL;
			}
			else
			{
				return ROOM_ERROR_PLAYEREXIST;
			}
		}
	}

	//进入房间观察者
	uint32 EnterRoom(uint32 u4RoomID, uint32 u4PlayerID, const char* pPlayerName, uint32 u4Money)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(m_objRoom[u4RoomID].m_u4State == 0)
			{
				if(m_objRoom[u4RoomID].m_u4VisitorCount >= ROOM_PLAYER_VISITOR_SIZE)
				{
					return ROOM_ERROR_PLAYERFULL;
				}
				else
				{
					//查找当前有无重复的PlayerID存在(观察者)
					for(int i = 0; i < ROOM_PLAYER_VISITOR_SIZE; i++)
					{
						if(m_objRoom[u4RoomID].m_objVisitPlayer[i].m_u4PlayerID == u4PlayerID)
						{
							return ROOM_ERROR_PLAYERVISEXIST;
						}
					}

					//查找当前玩家是否已经就坐
					for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
					{
						if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID == u4PlayerID)
						{
							return ROOM_ERROR_PLAYERSITEXIST;
						}
					}

					//寻找空位
					for(int i = 0; i < ROOM_PLAYER_VISITOR_SIZE; i++)
					{
						if(m_objRoom[u4RoomID].m_objVisitPlayer[i].m_u4PlayerID == PLAYER_INVALID_ID)
						{
							m_objRoom[u4RoomID].m_objVisitPlayer[i].m_u4PlayerID = u4PlayerID;
							m_objRoom[u4RoomID].m_objVisitPlayer[i].m_u4Money    = u4Money;
							m_objRoom[u4RoomID].m_u4VisitorCount++;
							sprintf_safe(m_objRoom[u4RoomID].m_objVisitPlayer[i].m_szPlayerNick, MAX_BUFF_20, "%s", pPlayerName);
							return ROOM_ERROR_NULL;
						}
					}

					return ROOM_ERROR_PLAYERFULL;
				}
			}
			else
			{
				return ROOM_ERROR_NOBEGIN;
			}
		}
	}

	//进入房间(随机位置)
	uint32 JoinRoom(uint32 u4RoomID, uint32 u4PlayerID, const char* pPlayerName, uint32 u4Money)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(m_objRoom[u4RoomID].m_u4State == 0)
			{
				if(m_objRoom[u4RoomID].m_u4PlayerCount >= ROOM_PLAYER_SIZE)
				{
					return ROOM_ERROR_PLAYERFULL;
				}
				else
				{
					for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
					{
						if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID == PLAYER_INVALID_ID)
						{
							m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID = u4PlayerID;
							m_objRoom[u4RoomID].m_objPlayer[i].m_u4Money    = u4Money;
							m_objRoom[u4RoomID].m_u4PlayerCount++;
							sprintf_safe(m_objRoom[u4RoomID].m_objPlayer[i].m_szPlayerNick, MAX_BUFF_20, "%s", pPlayerName);
							return ROOM_ERROR_NULL;
						}
					}

					return ROOM_ERROR_PLAYERFULL;
				}
			}
			else
			{
				return ROOM_ERROR_NOBEGIN;
			}
		}
	}

	//离开房间
	uint32 ExitRoom(uint32 u4RoomID, uint32 u4PlayerID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(m_objRoom[u4RoomID].m_u4State == 0)
			{
				if(m_objRoom[u4RoomID].m_u4PlayerCount <= 0)
				{
					return ROOM_ERROR_PLAYERNOEXIST;
				}
				else
				{
					//先检查一遍是否已经坐下
					for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
					{
						if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID == u4PlayerID)
						{
							m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID = PLAYER_INVALID_ID;
							m_objRoom[u4RoomID].m_objPlayer[i].m_u4Money    = 0;
							m_objRoom[u4RoomID].m_objPlayer[i].Clear();
							m_objRoom[u4RoomID].m_u4PlayerCount--;
							return ROOM_ERROR_NULL;
						}
					}

					//如果没有坐下，看看是否是观察者
					for(int i = 0; i < ROOM_PLAYER_VISITOR_SIZE; i++)
					{
						if(m_objRoom[u4RoomID].m_objVisitPlayer[i].m_u4PlayerID == u4PlayerID)
						{
							m_objRoom[u4RoomID].m_objVisitPlayer[i].m_u4PlayerID = PLAYER_INVALID_ID;
							m_objRoom[u4RoomID].m_objVisitPlayer[i].m_u4Money    = 0;
							m_objRoom[u4RoomID].m_objVisitPlayer[i].Clear();
							m_objRoom[u4RoomID].m_u4VisitorCount--;
							return ROOM_ERROR_NULL;
						}
					}

					return ROOM_ERROR_PLAYERNOEXIST;
				}
			}
			else
			{
				return ROOM_ERROR_NOBEGIN;
			}
		}
	}

	//开始房间
	uint32 BeginRoom(uint32 u4RoomID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(m_objRoom[u4RoomID].m_u4State == 0)
			{
				m_objRoom[u4RoomID].m_u4State = 1;
				return ROOM_ERROR_NULL;
			}
			else
			{
				return ROOM_ERROR_ISBEGIN;
			}
		}
	}

	//结束房间
	uint32 EndRoom(uint32 u4RoomID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(m_objRoom[u4RoomID].m_u4State == 1)
			{
				m_objRoom[u4RoomID].m_u4State = 0;
				return ROOM_ERROR_NULL;
			}
			else
			{
				return ROOM_ERROR_ISNOBEGIN;
			}
		}
	}

	//设置房间内玩家的金钱
	uint32 SetPlayerMoney(uint32 u4RoomID, uint32 u4PlayerID, uint32 u4Money)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(m_objRoom[u4RoomID].m_u4State == 0)
			{
				if(m_objRoom[u4RoomID].m_u4PlayerCount <= 0)
				{
					return ROOM_ERROR_PLAYERNOEXIST;
				}
				else
				{
					for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
					{
						if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID == u4PlayerID)
						{
							m_objRoom[u4RoomID].m_objPlayer[i].m_u4Money    = u4Money;
							return ROOM_ERROR_NULL;
						}
					}

					return ROOM_ERROR_PLAYERNOEXIST;
				}
			}
			else
			{
				return ROOM_ERROR_NOBEGIN;
			}
		}
	}

	//得到玩家金钱
	uint32 GetPlayerMoney(uint32 u4RoomID, uint32 u4PlayerID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(m_objRoom[u4RoomID].m_u4State == 0)
			{
				if(m_objRoom[u4RoomID].m_u4PlayerCount <= 0)
				{
					return ROOM_ERROR_PLAYERNOEXIST;
				}
				else
				{
					for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
					{
						if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID == u4PlayerID)
						{
							return m_objRoom[u4RoomID].m_objPlayer[i].m_u4Money;
						}
					}

					return ROOM_ERROR_PLAYERNOEXIST;
				}
			}
			else
			{
				return ROOM_ERROR_NOBEGIN;
			}
		}
	}

	//设置玩家的牌
	uint32 SetPlayerCard(uint32 u4RoomID, uint32 u4PlayerID, uint32 u4CardID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(m_objRoom[u4RoomID].m_u4State == 0)
			{
				if(m_objRoom[u4RoomID].m_u4PlayerCount <= 0)
				{
					return ROOM_ERROR_PLAYERNOEXIST;
				}
				else
				{
					for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
					{
						if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID == u4PlayerID)
						{
							for(int j = 0; j < PLAYER_CODE_SIZE; j++)
							{
								//找到一个空格，放牌
								if(m_objRoom[u4RoomID].m_objPlayer[i].m_objCard[j] == CARD_INVALID_ID)
								{
									m_objRoom[u4RoomID].m_objPlayer[i].m_objCard[j] = u4CardID;
									m_objRoom[u4RoomID].m_objPlayer[i].m_u4CardCount++;
									return ROOM_ERROR_NULL;
								}
							}

							return ROOM_ERROR_PLAYERCARDFULL;
						}
					}

					return ROOM_ERROR_PLAYERNOEXIST;
				}
			}
			else
			{
				return ROOM_ERROR_NOBEGIN;
			}
		}
	}

	//得到玩家指定位置的牌
	uint32 GetPlayerCard(uint32 u4RoomID, uint32 u4PlayerID, uint32 u4Index)
	{
		uint32 u4CurrIndex = 0;
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(m_objRoom[u4RoomID].m_u4State == 0)
			{
				if(m_objRoom[u4RoomID].m_u4PlayerCount <= 0)
				{
					return ROOM_ERROR_PLAYERNOEXIST;
				}
				else
				{
					for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
					{
						if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID == u4PlayerID)
						{
							for(int j = 0; j < PLAYER_CODE_SIZE; j++)
							{
								//找到一个空格，放牌
								if(m_objRoom[u4RoomID].m_objPlayer[i].m_objCard[j] != CARD_INVALID_ID)
								{
									if(u4CurrIndex == u4Index)
									{
										return m_objRoom[u4RoomID].m_objPlayer[i].m_objCard[j];
									}
									else
									{
										u4CurrIndex++;
									}
									return ROOM_ERROR_NULL;
								}
							}

							return ROOM_ERROR_PLAYERNOCARD;
						}
					}

					return ROOM_ERROR_PLAYERNOEXIST;
				}
			}
			else
			{
				return ROOM_ERROR_NOBEGIN;
			}
		}
	}

	//删除玩家身上的一张卡牌
	uint32 DelPalyerCard(uint32 u4RoomID, uint32 u4PlayerID, uint32 u4Index)
	{
		uint32 u4CurrIndex = 0;
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(m_objRoom[u4RoomID].m_u4State == 0)
			{
				if(m_objRoom[u4RoomID].m_u4PlayerCount <= 0)
				{
					return ROOM_ERROR_PLAYERNOEXIST;
				}
				else
				{
					for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
					{
						if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID == u4PlayerID)
						{
							for(int j = 0; j < PLAYER_CODE_SIZE; j++)
							{
								//找到一个空格，放牌
								if(m_objRoom[u4RoomID].m_objPlayer[i].m_objCard[j] != CARD_INVALID_ID)
								{
									if(u4CurrIndex == u4Index)
									{
										m_objRoom[u4RoomID].m_objPlayer[i].m_objCard[j] = CARD_INVALID_ID;
										m_objRoom[u4RoomID].m_objPlayer[i].m_u4CardCount--;
										return ROOM_ERROR_NULL;
									}
									else
									{
										u4CurrIndex++;
									}
									return ROOM_ERROR_NULL;
								}
							}

							return ROOM_ERROR_PLAYERNOCARD;
						}
					}

					return ROOM_ERROR_PLAYERNOEXIST;
				}
			}
			else
			{
				return ROOM_ERROR_NOBEGIN;
			}
		}
	}

	//得到玩家的卡牌数量
	uint32 GetPlayerCardCount(uint32 u4RoomID, uint32 u4PlayerID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(m_objRoom[u4RoomID].m_u4State == 0)
			{
				if(m_objRoom[u4RoomID].m_u4PlayerCount <= 0)
				{
					return ROOM_ERROR_PLAYERNOEXIST;
				}
				else
				{
					for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
					{
						if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID == u4PlayerID)
						{
							return m_objRoom[u4RoomID].m_objPlayer[i].m_u4CardCount;
						}
					}

					return ROOM_ERROR_PLAYERNOEXIST;
				}
			}
			else
			{
				return ROOM_ERROR_NOBEGIN;
			}
		}
	}

	//设置房间牌桌上的一张卡牌
	uint32 SetRoomTableCard(uint32 u4RoomID, uint32 u4CardID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			for(int i = 0; i < ROOM_TABLE_CARD_SIZE; i++)
			{
				if(m_objRoom[u4RoomID].m_objGameInfo.m_objTableCard[i] == CARD_INVALID_ID)
				{
					m_objRoom[u4RoomID].m_objGameInfo.m_objTableCard[i] = u4CardID;
				}
			}

			return ROOM_ERROR_PLAYERCARDFULL;
		}
	}

	//得到房间牌桌上指定位置的一张牌
	uint32 GetRoomTableCard(uint32 u4RoomID, uint32 u4Index)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(u4Index < 0 || u4Index >= ROOM_TABLE_CARD_SIZE)
			{
				return ROOM_ERROR_PLAYERNOCARD;
			}
			else
			{
				return m_objRoom[u4RoomID].m_objGameInfo.m_objTableCard[u4Index];
			}
		}
	}

	//删除牌桌指定位置的一张卡牌
	uint32 DelRoomTableCard(uint32 u4RoomID, uint32 u4Index)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(u4Index < 0 || u4Index >= ROOM_TABLE_CARD_SIZE)
			{
				return ROOM_ERROR_PLAYERNOCARD;
			}
			else
			{
				m_objRoom[u4RoomID].m_objGameInfo.m_objTableCard[u4Index] = CARD_INVALID_ID;
				return ROOM_ERROR_NULL;
			}
		}
	}

	//设置牌库中的一张牌
	uint32 SetRoomBankCard(uint32 u4RoomID, uint32 u4CardID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			for(int i = 0; i < ROOM_TABLE_CARD_SIZE; i++)
			{
				if(m_objRoom[u4RoomID].m_objGameInfo.m_objBankCard[i] == CARD_INVALID_ID)
				{
					m_objRoom[u4RoomID].m_objGameInfo.m_objBankCard[i] = u4CardID;
				}
			}

			return ROOM_ERROR_PLAYERCARDFULL;
		}
	}

	//得到房间牌库中的一张牌
	uint32 GetRoomBankCard(uint32 u4RoomID, uint32 u4Index)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(u4Index < 0 || u4Index >= ROOM_CARD_SIZE)
			{
				return ROOM_ERROR_PLAYERNOCARD;
			}
			else
			{
				return m_objRoom[u4RoomID].m_objGameInfo.m_objBankCard[u4Index];
			}
		}
	}

	//删除房间牌库中的一张卡牌
	uint32 DelRoomBankCard(uint32 u4RoomID, uint32 u4Index)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(u4Index < 0 || u4Index >= ROOM_CARD_SIZE)
			{
				return ROOM_ERROR_PLAYERNOCARD;
			}
			else
			{
				m_objRoom[u4RoomID].m_objGameInfo.m_objBankCard[u4Index] = CARD_INVALID_ID;
				return ROOM_ERROR_NULL;
			}
		}
	}
};

typedef ACE_Singleton<_RoomManager, ACE_Null_Mutex> App_RoomManager;

#endif
