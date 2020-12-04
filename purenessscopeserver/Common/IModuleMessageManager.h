#ifndef _IMODULEMESSAGEMANAGER_H
#define _IMODULEMESSAGEMANAGER_H

#include "IBuffPacket.h"

class IModuleMessageManager
{
public:
    virtual ~IModuleMessageManager() {}
    virtual int SendModuleMessage(const char* pModuleName, uint16 u2CommandID, shared_ptr<IBuffPacket> pBuffPacket, shared_ptr<IBuffPacket> pReturnBuffPacket) = 0; //����ģ���ĵ��ù�ϵ
    virtual int SendFrameMessage(uint16 u2CommandID, uint32 u4WorkThreadID, shared_ptr<IBuffPacket> pHeadPacket, shared_ptr<IBuffPacket> pBodyBuffPacket)         = 0;  //�������������Իص����������
};

#endif