#ifndef _BUFFPACKETMAGER_H
#define _BUFFPACKETMAGER_H

//发现在这里CreateBuffPacket会非常慢，这里需要提升能力。
//使用BuffPacket池，在服务器启动的时候，一次性创建好多个Buffpacket，利用内存池的想法。
//这里感谢freebird的测试客户端，正是因为它，我才能发现了这个问题。不要让关注你的人失望，这是必须的。
//2010-06-03 freeeyes

#include "IPacketManager.h"
#include "BuffPacket.h"
#include "ObjectPoolManager.h"
#include "XmlConfig.h"

class CBuffPacketManager : public CObjectPoolManager<CBuffPacket, ACE_Recursive_Thread_Mutex>, public IPacketManager
{
public:
    CBuffPacketManager() = default;
    virtual ~CBuffPacketManager() = default;

    static void Init_Callback(int nIndex, CBuffPacket* pBuffPacket);
    static void Close_Callback(int nIndex, CBuffPacket* pBuffPacket);

    uint32 GetBuffPacketUsedCount() final;
    uint32 GetBuffPacketFreeCount() final;
    void OutputCreateInfo() final;
    void SetCreateFlag(bool blTagCreateInfo) final;

    IBuffPacket* Create(const char* pFileName, uint32 u4Line) final;
    bool Delete(IBuffPacket* pBuffPacket) final;
    uint32 GetCreateInfoCount() final;
    bool GetCreateInfoList(uint32 u4Index, _Object_Create_Info& objCreateInfo) final;
private:
    vector<_Object_Create_Info> m_objCreateList;
};

typedef ACE_Singleton<CBuffPacketManager, ACE_Null_Mutex> App_BuffPacketManager;
#endif
