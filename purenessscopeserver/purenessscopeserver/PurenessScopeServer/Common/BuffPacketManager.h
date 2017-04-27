#ifndef _BUFFPACKETMAGER_H
#define _BUFFPACKETMAGER_H

//发现在这里CreateBuffPacket会非常慢，这里需要提升能力。
//使用BuffPacket池，在服务器启动的时候，一次性创建好多个Buffpacket，利用内存池的想法。
//这里感谢freebird的测试客户端，正是因为它，我才能发现了这个问题。不要让关注你的人失望，这是必须的。
//2010-06-03 freeeyes

#include "HashTable.h"
#include "IPacketManager.h"
#include "BuffPacket.h"

using namespace std;

class CBuffPacketManager : public IPacketManager
{
public:
    CBuffPacketManager(void);
    ~CBuffPacketManager(void);

    void Init(uint32 u4PacketCount, uint32 u4MaxBuffSize, bool blByteOrder);
    void Close();

    uint32 GetBuffPacketUsedCount();
    uint32 GetBuffPacketFreeCount();

    IBuffPacket* Create();
    bool Delete(IBuffPacket* pBuffPacket);

private:
    CHashTable<CBuffPacket>    m_objHashBuffPacketList;               //存储空闲BuffPacket指针的hash列表
    bool                       m_blSortType;                          //字序规则，true为网序，false为主机序
    ACE_Recursive_Thread_Mutex m_ThreadWriteLock;
};

typedef ACE_Singleton<CBuffPacketManager, ACE_Null_Mutex> App_BuffPacketManager;
#endif
