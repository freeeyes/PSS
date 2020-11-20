#ifndef POSTSERVER_MESSAGE_H
#define POSTSERVER_MESSAGE_H

#include "IBuffPacket.h"
#include "ClientCommand.h"
#include "ClientReConnectManager.h"

//此类处理所有透传数据相关逻辑

#define RECV_BUFF_SIZE  MAX_BUFF_1024      //接收缓冲大小

class CPostUdpServerData : public IClientUDPMessage
{
public:
    CPostUdpServerData() = default;

    ~CPostUdpServerData() final = default;

    //这里提供接受数据拼包算法，组成完整数据包后会调用RecvData方法
    bool RecvData(const char* pData, int nLen, _ClientIPInfo const& objServerIPInfo) final
    {
        //数据包已经收全，在这里处理数据
        ACE_UNUSED_ARG(pData);
        ACE_UNUSED_ARG(nLen);
        ACE_UNUSED_ARG(objServerIPInfo);

        return true;
    }
};

#endif
