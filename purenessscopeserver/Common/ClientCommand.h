#ifndef _CLIENTCOMMAND_H
#define _CLIENTCOMMAND_H

#include "IMessage.h"
#include "IBuffPacket.h"

//���������࣬�����������̳�����

class CClientCommand
{
public:
    virtual ~CClientCommand() {}
    virtual int DoMessage(IMessage* pMessage, bool& bDeleteFlag, IBuffPacket* pSendBuffPacket) = 0;                                                        //����ӿͻ��˷���������Ϣ
};
#endif
