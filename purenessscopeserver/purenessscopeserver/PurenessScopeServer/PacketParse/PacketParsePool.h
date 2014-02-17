#ifndef _PACKETPARSEPOOL_H
#define _PACKETPARSEPOOL_H

#include "ace/Thread_Mutex.h"
#include "ace/Singleton.h"

#include "PacketParse.h"

#include <map>

using namespace std;

//CPacketParse对象池
class CPacketParsePool
{
public:
	CPacketParsePool();
	~CPacketParsePool();

	void Init(uint32 u4PacketParsePoolCount);
	void Close();

	CPacketParse* Create();
	bool Delete(CPacketParse* pPacketParse);

	int GetUsedCount();
	int GetFreeCount();

private:
	typedef map<CPacketParse*, CPacketParse*> mapPacketParse;
	mapPacketParse                  m_mapPacketUsed;                       //已使用的
	mapPacketParse                  m_mapPacketFree;                       //没有使用的
	ACE_Recursive_Thread_Mutex      m_ThreadWriteLock;                     //控制多线程锁
};

typedef ACE_Singleton<CPacketParsePool, ACE_Null_Mutex> App_PacketParsePool; 

#endif
