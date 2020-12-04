#ifndef _BUFFPACKETMAGER_H
#define _BUFFPACKETMAGER_H

//����������CreateBuffPacket��ǳ�����������Ҫ����������
//ʹ��BuffPacket�أ��ڷ�����������ʱ��һ���Դ����ö��Buffpacket�������ڴ�ص��뷨��
//�����лfreebird�Ĳ��Կͻ��ˣ�������Ϊ�����Ҳ��ܷ�����������⡣��Ҫ�ù�ע�����ʧ�������Ǳ���ġ�
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

using App_BuffPacketManager = ACE_Singleton<CBuffPacketManager, ACE_Null_Mutex>;
#endif
