#ifndef _PACKETPARSEPOOL_H
#define _PACKETPARSEPOOL_H

#include "ace/Thread_Mutex.h"
#include "ace/Singleton.h"

#include "PacketParse.h"
#include "MainConfig.h"
#include "MessageBlockManager.h"
#include "HashTable.h"

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
    bool Delete(CPacketParse* pPacketParse, bool blDelete = false);

    int GetUsedCount();
    int GetFreeCount();

private:
    CHashTable<CPacketParse>        m_objPacketParseList;                  //Hash内存池
    ACE_Recursive_Thread_Mutex      m_ThreadWriteLock;                     //控制多线程锁
};

typedef ACE_Singleton<CPacketParsePool, ACE_Null_Mutex> App_PacketParsePool;

#endif
