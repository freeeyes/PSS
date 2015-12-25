#ifndef _CLIENTCOMMAND_H
#define _CLIENTCOMMAND_H

#include "IMessage.h"

//基础抽象类，处理命令的类继承它。

class CClientCommand
{
public:
    virtual ~CClientCommand() {}
	virtual int DoMessage(IMessage* pMessage, bool& bDeleteFlag) = 0; 
};
#endif
