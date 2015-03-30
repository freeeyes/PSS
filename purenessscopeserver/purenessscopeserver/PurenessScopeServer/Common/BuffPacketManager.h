#ifndef _BUFFPACKETMAGER_H
#define _BUFFPACKETMAGER_H

//发现在这里CreateBuffPacket会非常慢，这里需要提升能力。
//使用BuffPacket池，在服务器启动的时候，一次性创建好多个Buffpacket，利用内存池的想法。
//这里感谢freebird的测试客户端，正是因为它，我才能发现了这个问题。不要让关注你的人失望，这是必须的。
//2010-06-03 freeeyes

#include <map>
#include "IPacketManager.h"
#include "BuffPacket.h"

using namespace std;

class CBuffPacketManager : public IPacketManager
{
public:
	CBuffPacketManager(void);
	~CBuffPacketManager(void);

	void Init(uint32 u4PacketCount, bool blByteOrder);
	void Close();
	
	uint32 GetBuffPacketUsedCount();
	uint32 GetBuffPacketFreeCount();

	IBuffPacket* Create(uint32 u4BuffID = 0);
	bool Delete(IBuffPacket* pBuffPacket);

private:
	typedef map<CBuffPacket*, CBuffPacket*> mapPacket;
	mapPacket                  m_mapPacketUsed;                       //已使用的
	mapPacket                  m_mapPacketFree;                       //没有使用的
	ACE_Recursive_Thread_Mutex m_ThreadWriteLock;
};

typedef ACE_Singleton<CBuffPacketManager, ACE_Null_Mutex> App_BuffPacketManager; 
#endif
