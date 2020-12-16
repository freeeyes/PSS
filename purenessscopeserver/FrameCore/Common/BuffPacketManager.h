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

class CBuffPacketManager : public IPacketManager
{
public:
    CBuffPacketManager() = default;
    ~CBuffPacketManager() final = default;

    shared_ptr<IBuffPacket> Create(const char* pFileName, uint32 u4Line) final;
};

using App_BuffPacketManager = PSS_singleton<CBuffPacketManager>;
#endif
