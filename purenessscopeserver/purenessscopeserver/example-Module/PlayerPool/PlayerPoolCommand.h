#pragma once

#include "IBuffPacket.h"
#include "ClientCommand.h"
#include "IObject.h"
#include "SMPool.h"
#include "IOData.h"
#include "MapTemplate.h"

#include <string>
#include <map>

//登陆池，指的是用户数据需要修改，DS配合的数据集合。这部分数据当满员后，不会启动调度算法。
//读取池，指的是用户数据不需要修改，只是临时使用的数据。这部分数据满员后，会启动调度算法。

using namespace std;

#define COMMAND_PLAYINSERT 0x1010   //用户数据创建
#define COMMAND_PLAYUPDATE 0x1011   //用户数据更新
#define COMMAND_PLAYDELETE 0x1012   //用户数据删除
#define COMMAND_PLAYSEACH  0x1013   //用户查询
#define COMMAND_PLAYLOGIN  0x1014   //用户登陆
#define COMMAND_PLAYLOGOFF 0x1015   //用户离开

#define MAX_READ_OBJECT    1000     //共享内存池允许用户的最大数量
#define MAX_ONLINE_OBJECT  500      //共享内存池允许用户在线的最大数量

class CPlayerPoolCommand : public CClientCommand
{
public:
	CPlayerPoolCommand(void);
	~CPlayerPoolCommand(void);

	int DoMessage(IMessage* pMessage, bool& bDeleteFlag);
	void SetServerObject(CServerObject* pServerObject);

private:
	void Init();
	CPlayerData* Do_PlayerInsert(const char* pPlayerNick);
	bool         Do_PlayerUpdate(CPlayerData* pPlayerData);
	bool         Do_PlayerDelete(const char* pPlayerNick);
	CPlayerData* Do_PlayerSearch(const char* pPlayerNick);
	CPlayerData* Do_PlayerLogin(const char* pPlayerNick);
	bool         Do_PlayerLogOff(const char* pPlayerNick);


private:
	CServerObject*       m_pServerObject;
	int                  m_nCount;
	CSMPool<CPlayerData> m_UserPool;
	typedef CMapTemplate<string, CPlayerData> MapMRU;
	CIOData m_IOData;                    //IO数据接口
	MapMRU  m_MapReadData;               //当前允许的读取用户池
	MapMRU  m_MapOnlineData;             //当前允许的在线用户池
};
