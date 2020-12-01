#ifndef _PACKETPARSEPOOL_H
#define _PACKETPARSEPOOL_H

#include "ace/Thread_Mutex.h"
#include "ace/Singleton.h"

#include "PacketParse.h"
#include "MessageBlockManager.h"
#include "ObjectPoolManager.h"

//CPacketParse�����
class CPacketParsePool : public CObjectPoolManager<CPacketParse, ACE_Recursive_Thread_Mutex>
{
public:
    CPacketParsePool() = default;

    static void Init_Callback(int nIndex, CPacketParse* pPacketParse);
    void Close();

    CPacketParse* Create(const char* pFileName, uint32 u4Line);
    bool Delete(CPacketParse* pPacketParse, bool blDelete = false);

    int GetUsedCount();
    int GetFreeCount();

private:
    ACE_Recursive_Thread_Mutex m_ThreadWriteLock;                     //���ƶ��߳���
};

using App_PacketParsePool = ACE_Singleton<CPacketParsePool, ACE_Null_Mutex>;

#endif
