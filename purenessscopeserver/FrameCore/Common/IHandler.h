#ifndef PSS_IHANDLER_H
#define PSS_IHANDLER_H

//�ڹ����߳��еĶ�����󷽷���
//���ڼ�¼��WorkThread��

#include "define.h"
#include "SendMessage.h"

class IHandler
{
public:
	virtual void Close(uint32 u4ConnectID) = 0;

	virtual bool SendMessage(CSendMessageInfo objSendMessageInfo, uint32& u4PacketSize) = 0;

	virtual bool PutSendPacket(uint32 u4ConnectID, ACE_Message_Block* pMbData, uint32 u4Size, const ACE_Time_Value tvSend) = 0;
};

#endif
