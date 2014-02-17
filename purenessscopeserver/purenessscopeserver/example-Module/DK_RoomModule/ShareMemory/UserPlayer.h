#ifndef _USERPLAYER_H
#define _USERPLAYER_H

//这里负责记录用户结构体，这部分结构体将会放入共享内存中
//add by freeeyes

#include "define.h"

#define PLAYER_INVALID_ID         -1     //无效的PlayerID
#define ROOM_INVALID_ID           -1     //无效的RoomID

#define PLAYER_SMPOOL_COUNT       5000   //共享内存池的Player的个数 

struct _UserPlayer
{
	uint32 m_u4PlayerID;                 //玩家的ID 
	char   m_szPlayerNick[MAX_BUFF_20];  //玩家的名称
	uint32 m_u4Money;                    //玩家身上的金钱
	uint8  m_u1PlayerState;              //玩家目前状态，0是在大厅，1是在房间内
	uint8  m_u1DataState;                //当前数据状态，0是正在使用，1是已经离开，2是可以删除 ，3是空闲
	uint32 m_u4RoomID;                   //当前玩家所在的RoomID

	_UserPlayer()
	{
		Clear();
	}

	void Clear()
	{
		m_u4PlayerID      = PLAYER_INVALID_ID;
		m_szPlayerNick[0] = '\0';
		m_u4Money         = 0;
		m_u1PlayerState   = 0;
		m_u1DataState     = 3;
		m_u4RoomID        = ROOM_INVALID_ID;
	}
};

#endif
