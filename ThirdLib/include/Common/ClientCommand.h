#ifndef _CLIENTCOMMAND_H
#define _CLIENTCOMMAND_H

#include "IMessage.h"

//���������࣬�����������̳�����

class CClientCommand
{
public:
    virtual ~CClientCommand() {}
    virtual int DoMessage(IMessage* pMessage, bool& bDeleteFlag) = 0;                                                        //����ӿͻ��˷���������Ϣ
};
#endif
