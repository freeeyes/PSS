#ifndef _IMODULEMESSAGEMANAGER_H
#define _IMODULEMESSAGEMANAGER_H

#include "IBuffPacket.h"

class IModuleMessageManager
{
public:
    virtual ~IModuleMessageManager() {}
    virtual int SendModuleMessage(const char* pModuleName, uint16 u2CommandID, shared_ptr<IBuffPacket> pBuffPacket, shared_ptr<IBuffPacket> pReturnBuffPacket) = 0; //处理模块间的调用关系
    virtual int SendFrameMessage(uint16 u2CommandID, uint32 u4WorkThreadID, shared_ptr<IBuffPacket> pHeadPacket, shared_ptr<IBuffPacket> pBodyBuffPacket)         = 0;  //向插件添加命令，可以回调到其他插件
};

#endif