#ifndef _ISENDCOMMAND_H
#define _ISENDCOMMAND_H

//发送命令的虚接口，对应BaseCommand的继承关系。
//为lua内部静态端口提供发送数据的支持

#include "define.h"
#include "IBuffPacket.h"
#include "Room.h"

//发送房间内玩家结构体
struct _Send_RoomPlayer
{
	uint32 m_u4PlayerID;                  //玩家ID
	uint8  m_u1Location;                  //玩家的座位ID
	char   m_szPlayerNick[MAX_BUFF_20];   //玩家昵称
	uint32 m_u4CardCount;                 //当前拥有牌的个数
	uint32 m_objCard[PLAYER_CODE_SIZE];   //玩家手上的牌

	_Send_RoomPlayer()
	{
		m_u4CardCount = 0;
		for(int i = 0; i < PLAYER_CODE_SIZE; i++)
		{
			m_objCard[i] = CARD_INVALID_ID;
		}
	}
};

//玩家房间信息
struct _Send_RoomInfo
{
	uint32            m_u4RoomID;                                    //房间ID
	char              m_szRoomName[MAX_BUFF_20];                     //房间名称
	char              m_szRoomDesc[MAX_BUFF_200];                    //房间信息描述
	uint32            m_u4PlayerCount;                               //当前房间坐下的人数
	_Send_RoomPlayer  m_objSendRoomPlayer[ROOM_PLAYER_SIZE];         //当前房间玩家信息

	_Send_RoomInfo()
	{
		m_u4PlayerCount = 0;
	}

	bool SetStream(uint16& u2CommandID, IBuffPacket* pBuffPacket)    //将当前类数据压缩成二进制流
	{
		if(NULL == pBuffPacket)
		{
			return false;
		}
		else
		{
			(*pBuffPacket) << u2CommandID;
			(*pBuffPacket) << m_u4RoomID;
			VCHARS_STR ss_Data;
			ss_Data.text  = (char* )m_szRoomName;
			ss_Data.u1Len = (uint8)ACE_OS::strlen(m_szRoomName);
			(*pBuffPacket) << ss_Data;

			ss_Data.text = (char* )m_szRoomDesc;
			ss_Data.u1Len = (uint8)ACE_OS::strlen(m_szRoomName);
			(*pBuffPacket) << ss_Data;

			(*pBuffPacket) << m_u4PlayerCount;

			for(int i = 0; i < (int)m_u4PlayerCount; i++)
			{
				(*pBuffPacket) << m_objSendRoomPlayer[i].m_u4PlayerID;
				(*pBuffPacket) << m_objSendRoomPlayer[i].m_u1Location;
				ss_Data.text  = (char* )m_objSendRoomPlayer[i].m_szPlayerNick;
				ss_Data.u1Len =(uint8) ACE_OS::strlen(m_objSendRoomPlayer[i].m_szPlayerNick);
				(*pBuffPacket) << ss_Data;
				(*pBuffPacket) << m_objSendRoomPlayer[i].m_u4CardCount;

				for(int j = 0; j < (int)m_objSendRoomPlayer[i].m_u4CardCount; j++)
				{
					(*pBuffPacket) << m_objSendRoomPlayer[i].m_objCard[j];
				}
			}

			return true;
		}
	}

	bool GetStream(uint16& u2CommandID, IBuffPacket* pBuffPacket)   //将当前二进制流转换为类结构
	{
		if(NULL == pBuffPacket)
		{
			return false;
		}
		else
		{
			(*pBuffPacket) >> u2CommandID;
			(*pBuffPacket) >> m_u4RoomID;
			VCHARS_STR ss_Data;
			(*pBuffPacket) >> ss_Data;
			ACE_OS::memcpy(m_szRoomName, ss_Data.text, ss_Data.u1Len);

			(*pBuffPacket) >> ss_Data;
			ACE_OS::memcpy(m_szRoomDesc, ss_Data.text, ss_Data.u1Len);

			(*pBuffPacket) >> m_u4PlayerCount;

			for(int i = 0; i < (int)m_u4PlayerCount; i++)
			{
				(*pBuffPacket) >> m_objSendRoomPlayer[i].m_u4PlayerID;
				(*pBuffPacket) >> m_objSendRoomPlayer[i].m_u1Location;
				(*pBuffPacket) >> ss_Data;
				ACE_OS::memcpy(m_objSendRoomPlayer[i].m_szPlayerNick, ss_Data.text, ss_Data.u1Len);
				(*pBuffPacket) >> m_objSendRoomPlayer[i].m_u4CardCount;

				for(int j = 0; j < (int)m_objSendRoomPlayer[i].m_u4CardCount; j++)
				{
					(*pBuffPacket) >> m_objSendRoomPlayer[i].m_objCard[j];
				}
			}

			return true;
		}
	}
};

//发送房间定时信息
struct _Send_RoomTimer
{
	uint32 m_u4RoomID;
	uint16 m_u2RoomSeconds;

	bool SetStream(uint16& u2CommandID, IBuffPacket* pBuffPacket)    //将当前类数据压缩成二进制流
	{
		if(NULL == pBuffPacket)
		{
			return false;
		}
		else
		{
			(*pBuffPacket) << u2CommandID;
			(*pBuffPacket) << m_u4RoomID;
			(*pBuffPacket) << m_u2RoomSeconds;
		}

		return true;
	}

	bool GetStream(uint16& u2CommandID, IBuffPacket* pBuffPacket)   //将当前二进制流转换为类结构
	{
		if(NULL == pBuffPacket)
		{
			return false;
		}
		else
		{
			(*pBuffPacket) >> u2CommandID;
			(*pBuffPacket) >> m_u4RoomID;
			(*pBuffPacket) >> m_u2RoomSeconds;
		}

		return true;
	}
};

//发送玩家定时器消息
struct _Send_RoomPlayerTimer
{
	uint32 m_u4RoomID;
	uint32 m_u4PlayerID;
	uint16 m_u2RoomSeconds;

	bool SetStream(uint16& u2CommandID, IBuffPacket* pBuffPacket)    //将当前类数据压缩成二进制流
	{
		if(NULL == pBuffPacket)
		{
			return false;
		}
		else
		{
			(*pBuffPacket) << u2CommandID;
			(*pBuffPacket) << m_u4RoomID;
			(*pBuffPacket) << m_u4PlayerID;
			(*pBuffPacket) << m_u2RoomSeconds;
		}

		return true;
	}

	bool GetStream(uint16& u2CommandID, IBuffPacket* pBuffPacket)   //将当前二进制流转换为类结构
	{
		if(NULL == pBuffPacket)
		{
			return false;
		}
		else
		{
			(*pBuffPacket) >> u2CommandID;
			(*pBuffPacket) >> m_u4RoomID;
			(*pBuffPacket) >> m_u4PlayerID;
			(*pBuffPacket) >> m_u2RoomSeconds;
		}

		return true;
	}
};

//玩家坐下信息
struct _Send_RoomPlayerInfo
{
	uint32 m_u4RoomID;                    //玩家所在房间ID
	uint32 m_u4PlayerID;                  //玩家ID
	uint8  m_u1Location;                  //玩家的座位ID
	char   m_szPlayerNick[MAX_BUFF_20];   //玩家昵称
	uint32 m_u4CardCount;                 //当前拥有牌的个数
	uint32 m_objCard[PLAYER_CODE_SIZE];   //玩家手上的牌

	bool SetStream(uint16 u2CommandID, IBuffPacket* pBuffPacket)    //将当前类数据压缩成二进制流
	{
		if(NULL == pBuffPacket)
		{
			return false;
		}
		else
		{
			(*pBuffPacket) << u2CommandID;

			(*pBuffPacket) << m_u4RoomID;
			(*pBuffPacket) << m_u4PlayerID;
			(*pBuffPacket) << m_u1Location;

			VCHARS_STR ss_Data;
			ss_Data.text  = (char* )m_szPlayerNick;
			ss_Data.u1Len = (uint8)ACE_OS::strlen(m_szPlayerNick);
			(*pBuffPacket) << ss_Data;

			(*pBuffPacket) << m_u4CardCount;

			for(int i = 0; i < (int)m_u4CardCount; i++)
			{
				(*pBuffPacket) << m_objCard[i];
			}

			return true;
		}
	}

	bool GetStream(uint16 u2CommandID, IBuffPacket* pBuffPacket)   //将当前二进制流转换为类结构
	{
		if(NULL == pBuffPacket)
		{
			return false;
		}
		else
		{
			(*pBuffPacket) >> u2CommandID;
			(*pBuffPacket) >> m_u4RoomID;
			(*pBuffPacket) >> m_u4PlayerID;
			(*pBuffPacket) >> m_u1Location;

			VCHARS_STR ss_Data;
			(*pBuffPacket) >> ss_Data;
			ACE_OS::memcpy(m_szPlayerNick, ss_Data.text, ss_Data.u1Len);

			(*pBuffPacket) >> m_u4CardCount;

			for(int i = 0; i < (int)m_u4CardCount; i++)
			{
				(*pBuffPacket) >> m_objCard[i];
			}

			return true;
		}
	}
};

//当前玩家下注消息
struct _Send_RoomPlayerMoneyInfo
{
	uint32 m_u4PlayerID;    //玩家的ID
	uint32 m_u4PlayerMoney; //玩家的金钱
};

//玩家下注消息
struct _Send_RoomMoney
{
	uint32                    m_u4RoomID;                      //房间ID
	uint32                    m_u4RoomMoney;                   //当前房间内钱数
	uint32                    m_u4PlayerCount;                 //当前房间内玩家个数
	_Send_RoomPlayerMoneyInfo m_objSendRoomPlayerMoneyInfo[ROOM_PLAYER_SIZE];

	bool SetStream(uint16 u2CommandID, IBuffPacket* pBuffPacket)    //将当前类数据压缩成二进制流
	{
		if(NULL == pBuffPacket)
		{
			return false;
		}
		else
		{
			(*pBuffPacket) << u2CommandID;

			(*pBuffPacket) << m_u4RoomID;
			(*pBuffPacket) << m_u4RoomMoney;
			(*pBuffPacket) << m_u4PlayerCount;

			for(int i = 0; i < (int)m_u4PlayerCount; i++)
			{
				(*pBuffPacket) << m_objSendRoomPlayerMoneyInfo[i].m_u4PlayerID;
				(*pBuffPacket) << m_objSendRoomPlayerMoneyInfo[i].m_u4PlayerMoney;
			}

			return true;
		}
	}

	bool GetStream(uint16 u2CommandID, IBuffPacket* pBuffPacket)   //将当前二进制流转换为类结构
	{
		if(NULL == pBuffPacket)
		{
			return false;
		}
		else
		{
			(*pBuffPacket) >> u2CommandID;

			(*pBuffPacket) >> m_u4RoomID;
			(*pBuffPacket) >> m_u4RoomMoney;
			(*pBuffPacket) >> m_u4PlayerCount;

			for(int i = 0; i < (int)m_u4PlayerCount; i++)
			{
				(*pBuffPacket) >> m_objSendRoomPlayerMoneyInfo[i].m_u4PlayerID;
				(*pBuffPacket) >> m_objSendRoomPlayerMoneyInfo[i].m_u4PlayerMoney;
			}

			return true;
		}
	}
};

//牌局房间卡牌信息
struct _Send_RoomCodeInfo
{
	uint32 m_u4RoomID;          //房间ID
	uint8  m_u1TableCardCount;  //当前桌面上要显示的卡牌数量
	uint32 m_objCard[ROOM_TABLE_CARD_SIZE];

	bool SetStream(uint16 u2CommandID, IBuffPacket* pBuffPacket)    //将当前类数据压缩成二进制流
	{
		if(NULL == pBuffPacket)
		{
			return false;
		}
		else
		{
			(*pBuffPacket) << u2CommandID;

			(*pBuffPacket) << m_u4RoomID;
			(*pBuffPacket) << m_u1TableCardCount;

			for(int i = 0; i < m_u1TableCardCount; i++)
			{
				(*pBuffPacket) << m_objCard[i];
			}

			return true;
		}
	}

	bool GetStream(uint16 u2CommandID, IBuffPacket* pBuffPacket)   //将当前二进制流转换为类结构
	{
		if(NULL == pBuffPacket)
		{
			return false;
		}
		else
		{
			(*pBuffPacket) >> u2CommandID;

			(*pBuffPacket) >> m_u4RoomID;
			(*pBuffPacket) >> m_u1TableCardCount;

			for(int i = 0; i < m_u1TableCardCount; i++)
			{
				(*pBuffPacket) >> m_objCard[i];
			}

			return true;
		}
	}

};

class ISendCommand
{
public:
	virtual bool SendRoomState(uint16 u2CommandID, uint32 u4PlayerID, _Send_RoomInfo& objSendRoomInfo)                     = 0;         //发送房间内的玩家信息
	virtual bool SendRoomSitdown(uint16 u2CommandID, uint32 u4PlayerID, _Send_RoomPlayerInfo& objSendRoomPlayerInfo)       = 0;         //玩家坐下信息
	virtual bool SendRoomTimer(uint16 u2CommandID, uint32 u4PlayerID, _Send_RoomTimer& objSendRoomTimer)                   = 0;         //发送房间定时器开启消息
	virtual bool SendRoomPlayerTimer(uint16 u2CommandID, uint32 u4PlayerID, _Send_RoomPlayerTimer objSendRoomPlayerTimer)  = 0;         //发送玩家定时器开启消息
	virtual bool SendRoomPlayerMoney(uint16 u2CommandID, uint32 u4PlayerID, _Send_RoomMoney objSendRoomMoney)              = 0;         //发送房间内玩家金钱信息
	virtual bool SendRoomTableCard(uint16 u2CommandID, uint32 u4PlayerID, _Send_RoomCodeInfo objSendRoomCodeInfo)          = 0;         //发送房间内的卡牌信息
};

#endif
