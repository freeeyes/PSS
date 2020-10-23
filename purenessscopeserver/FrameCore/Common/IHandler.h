#ifndef PSS_IHANDLER_H
#define PSS_IHANDLER_H

//�ڹ����߳��еĶ�����󷽷���
//���ڼ�¼��WorkThread��

#include "define.h"
#include "SendMessage.h"

class IHandler
{
public:
	virtual void Close() = 0;

	virtual bool SendMessage(CSendMessageInfo objSendMessageInfo, uint32& u4PacketSize) = 0;
};

#endif
