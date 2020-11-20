#ifndef POSTSERVER_MESSAGE_H
#define POSTSERVER_MESSAGE_H

#include "IBuffPacket.h"
#include "ClientCommand.h"
#include "ClientReConnectManager.h"

//���ദ������͸����������߼�

#define RECV_BUFF_SIZE  MAX_BUFF_1024      //���ջ����С

class CPostUdpServerData : public IClientUDPMessage
{
public:
    CPostUdpServerData() = default;

    ~CPostUdpServerData() final = default;

    //�����ṩ��������ƴ���㷨������������ݰ�������RecvData����
    bool RecvData(const char* pData, int nLen, _ClientIPInfo const& objServerIPInfo) final
    {
        //���ݰ��Ѿ���ȫ�������ﴦ������
        ACE_UNUSED_ARG(pData);
        ACE_UNUSED_ARG(nLen);
        ACE_UNUSED_ARG(objServerIPInfo);

        return true;
    }
};

#endif
