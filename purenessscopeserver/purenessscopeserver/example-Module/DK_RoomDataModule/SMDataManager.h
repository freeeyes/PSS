#ifndef _SMDATAMANAGER_H
#define _SMDATAMANAGER_H

//这里负责管理所有的SM共享内存数据
//目前包括所有的Player和Room的两种内存数据
//add by freeeyes

#include <list>

using namespace std;

#include "UserPlayer.h"
#include "SMPool.h"

#define RANAD_MIN_NUMBER  1
#define RANAD_MAX_NUMBER  1000000

#include <string>

typedef map<string, _UserPlayer*> mapOnlineUserPlayer;
typedef map<string, _UserPlayer*> mapOfflineUserPlayer;
typedef list<_UserPlayer*> listDeleteUserPlayer;

class CSMDataManager
{
public:
	CSMDataManager(void);
	~CSMDataManager(void);

	bool         Init();                                    //打开共享内存
	bool         ReadDBData();                              //从数据库中填充玩家信息
	int32        ReadDBData(const char* pPlayerNick);       //从玩家昵称获得玩家信息
	int32        ReadPlayerSMPos(const char* pPlayerNick);  //获得玩家当前内存地址的偏移位置
	bool         LeaveDBData(const char* pPlayerNick);      //玩家离开
	bool         WriteDBData(const char* pPlayerNick);      //写入数据源玩家的数据
	_UserPlayer* GetUserPlayer(uint32 u4Pos);               //根据偏移获得数据指针
	void         ReadSMPool();                              //还原数据在线，离开和可以删除的数据列表

private:
	uint32       GetRandNumber();                           //获得一个随机数
	bool         DeleteUserPlayer();                        //删除最后一个应该删除链表的指针
	_UserPlayer* FindPlayerLeave(const char* pPlayerNick);  //在离线列表中寻找是否已有数据
	_UserPlayer* FindPlayerDelete(const char* pPlayerNick); //在可删除列表中寻找是否已有数据

private:
	CSMPool<_UserPlayer> m_objUserPlayerPool;     //玩家用户池
	SMKey                m_objKey;                //共享内存key
	mapOnlineUserPlayer  m_mapOnlineUserPlayer;   //在线玩家表
	mapOfflineUserPlayer m_mapLeaveUserPlayer;    //已经离开玩家表
	mapOfflineUserPlayer m_mapDeleteUserPlayer;   //可以删除的玩家表(用于高效查找) 
	listDeleteUserPlayer m_listDeleteUserPlayer;  //可以删除的玩家表(用于淘汰算法)
};
#endif

