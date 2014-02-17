#ifndef _ISECNCEMANAGER_H
#define _ISECNCEMANAGER_H

#include "ITimerManager.h"

#include "ace/Task.h"
#include "ace/Thread_Mutex.h"
#include "ace/OS_main.h"
#include "ace/Synch.h"

#include "ace/Singleton.h"
#include "ace/Null_Mutex.h"



//记录ISenceManager的指针
//add by freeeyes

class ISenceManager : public ACE_Task<ACE_MT_SYNCH>
{
public:
	ISenceManager() {};
	virtual ~ISenceManager() {};

	virtual ActiveTimer* GetActiveTimer() = 0;

	virtual bool SendRoomState(uint32 u4RoomID, uint32 u4PlayerID)        = 0;
	virtual bool SendRoomSitdown(uint32 u4RoomID, uint32 u4PlayerID)      = 0;
	virtual bool SendRoomTimer(uint32 u4RoomID, uint32 u4PlayerID)        = 0;
	virtual bool SendRoomPlayerTimer(uint32 u4RoomID, uint32 u4PlayerID)  = 0;
	virtual bool SendRoomMoney(uint32 u4RoomID)                           = 0;
	virtual bool SendRoomTableCode(uint32 u4RoomID, uint8 u1CodseCount)   = 0;
};

//管理CSenceManager的远程指针，这里很恶心，为了解决循环引用，必须拆出来。
//循环引用有时候真的在面向对象的时候会有麻烦，先这么写，以后再找更好的方法。
//add by freeeyes
class CSenceManagerPtr
{
public: 
	CSenceManagerPtr() { m_pSenceManager = NULL; };
	~CSenceManagerPtr() {};

	void SetSenceManagerPtr(ISenceManager* pSenceManager)
	{
		m_pSenceManager = pSenceManager;
	}

	ISenceManager* GetSenceManagerPtr()
	{
		return m_pSenceManager;
	}

	ActiveTimer* GetActiveTimer() { return m_pSenceManager->GetActiveTimer(); };       //得到当前定时器对象

private:
	ISenceManager* m_pSenceManager;
};

typedef ACE_Singleton<CSenceManagerPtr, ACE_Null_Mutex> App_SenceManagerPtr;

#endif
