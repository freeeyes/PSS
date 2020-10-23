#ifndef PSS_IHANDLER_H
#define PSS_IHANDLER_H

//在工作线程中的对象抽象方法类
//用于记录在WorkThread中

#include "define.h"
#include "SendMessage.h"

class IHandler
{
public:
	virtual void Close() = 0;

	virtual bool SendMessage(CSendMessageInfo objSendMessageInfo, uint32& u4PacketSize) = 0;
};

#endif
