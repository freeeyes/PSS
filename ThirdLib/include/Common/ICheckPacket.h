#ifndef _ICHECKPACKET_H
#define _ICHECKPACKET_H

//�����һ��֧��Http��TcpЭ���ͨ���ࡣ
//��Ƶ�ʱ��������һ�����⣬��������ڳ��ָ�������ݰ����͵��ж���δ���
//���ǹ�����һ�������Ľӿڣ������𽫼�����ݰ������ԵĹ��ܰ��������ÿ�ֽ������ж���ʽ�����Լ����塣������ʵ������ӿڵ����ԡ�
//2009-09-30

#include "IBuffPacket.h"

class ICheckPacket
{
public:
    virtual ~ICheckPacket() {}
    virtual  IBuffPacket* CheckData(IBuffPacket* pRecvPacket, uint32 u4ConnectID, uint32& u4PacketLen) = 0;
};

#endif
