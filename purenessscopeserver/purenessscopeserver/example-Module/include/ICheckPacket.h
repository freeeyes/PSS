#ifndef _ICHECKPACKET_H
#define _ICHECKPACKET_H

//想设计一个支持Http和Tcp协议的通用类。
//设计的时候遇到了一个问题，就是如果在出现更多的数据包类型的判定如何处理？
//于是构想了一个这样的接口，它负责将检测数据包完整性的功能剥离出来，每种解析和判定格式可以自己定义。但必须实现这个接口的属性。
//2009-09-30

#include "IBuffPacket.h"

class ICheckPacket
{
public:
	virtual ~ICheckPacket() {};
	virtual  IBuffPacket* CheckData(IBuffPacket* pRecvPacket, uint32 u4ConnectID, uint32& u4PacketLen) = 0;
};

#endif
