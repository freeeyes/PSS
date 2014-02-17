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



typedef map<uint32, _UserPlayer*> mapOnlineUserPlayer;
typedef list<_UserPlayer*> listDeleteUserPlayer;

class CSMDataManager
{
public:
	CSMDataManager(void);
	~CSMDataManager(void);

	bool         Init();                                    //打开共享内存
	_UserPlayer* GetUserPlayer(uint32 u4Pos);               //根据偏移获得数据指针


private:
	CSMPool<_UserPlayer> m_objUserPlayerPool;     //玩家用户池
	SMKey                m_objKey;                //共享内存key

};
#endif

