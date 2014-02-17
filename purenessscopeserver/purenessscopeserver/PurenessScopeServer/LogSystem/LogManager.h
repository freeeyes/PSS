#ifndef _LOGMANAGER_H
#define _LOGMANAGER_H

#include "ace/Task.h"
#include <stdio.h>
#include <stdarg.h>
#include "define.h"
#include "MapTemplate.h"
#include "ILogObject.h"
#include "ILogManager.h"

//管理日志块的池
//add by freeeyes
class CLogBlockPool
{
public:
	CLogBlockPool();
	~CLogBlockPool();

	void Init(uint32 u4BlockSize, uint32 u4PoolCount);
	void Close();

	_LogBlockInfo* GetLogBlockInfo();                       //得到一个空余的日志块
	void ReturnBlockInfo(_LogBlockInfo* pLogBlockInfo);     //归还一个用完的日志块

	uint32 GetBlockSize();

private:
	_LogBlockInfo* m_pLogBlockInfo;
	uint32         m_u4MaxBlockSize;
	uint32         m_u4PoolCount;
	uint32         m_u4CurrIndex;
};


class CLogManager : public ACE_Task<ACE_MT_SYNCH>, public ILogManager
{
public:
	CLogManager(void);
	~CLogManager(void);

	virtual int open (void *args = 0);
	virtual int svc(void);
	int Close();

	void Init(int nThreadCount = 1, int nQueueMax = MAX_MSG_THREADQUEUE);
	int Start();
	int Stop();
	bool IsRun();

	
	int PutLog(int nLogType, _LogBlockInfo* pLogBlockInfo);
	int RegisterLog(CServerLogger * pServerLogger);
	int UnRegisterLog(CServerLogger * pServerLogger);

	//对外写日志的接口
	int WriteLog(int nLogType, const char* fmt, ...);

	int WriteLogBinary(int nLogType, const char* pData, int nLen);

private:
	int ProcessLog(int nLogType, _LogBlockInfo* pLogBlockInfo);

private:
	bool                              m_blRun;
	int                               m_nThreadCount;
	int                               m_nQueueMax;
	CMapTemplate<int, CServerLogger>  m_mapServerLogger;
	CLogBlockPool                     m_objLogBlockPool;          //日志块池
	ACE_Recursive_Thread_Mutex        m_Logger_Mutex;
};


typedef ACE_Singleton<CLogManager, ACE_Recursive_Thread_Mutex> AppLogManager; 


#endif
