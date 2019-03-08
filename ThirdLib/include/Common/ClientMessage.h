#ifndef _ICLIENTMESSAGE_H
#define _ICLIENTMESSAGE_H

//����һ���ӿ��࣬�������ݷ��ͳ�ȥ����������ݷ��أ��ṩ���Ӵ������
//��������RecvMessage()�������ݺ�ConnectError()���ʹ������ַ�����
//add by freeeyes 2010-12-31

#include "IMessageBlockManager.h"

//�յ������ݰ����ݻ�·
enum EM_PACKET_ROUTE
{
    PACKET_ROUTE_SELF = 0,   //���ݰ���·����Ҫm_pClientMessage�Լ�ȥ����
    PACKET_ROUTE_CLIENT_MESSAHE,   //���ݰ���·����Ӧ��CMessageServiceȥ����
};

class IClientMessage
{
public:
    virtual ~IClientMessage() {}

    //�Ƿ���Ҫ��ܵ��÷����������
    virtual bool Need_Send_Format()                                                                                                                              = 0;
    //�����������
    virtual bool Send_Format_data(char* pData, uint32 u4Len, IMessageBlockManager* pMessageBlockManager, ACE_Message_Block*& mbSend)                             = 0;    //�������ݰ����ͽӿ�
    //�����������
    virtual bool Recv_Format_data(ACE_Message_Block* mbRecv, IMessageBlockManager* pMessageBlockManager, uint16& u2CommandID,
                                  ACE_Message_Block*& mbFinishRecv, EM_PACKET_ROUTE& emPacketRoute)  = 0;    //�������ݰ����սӿ�

    //���������ɹ��ص�����
    virtual void ReConnect(int nServerID)                                                               = 0;    //���������ɹ��ӿ�
    //�����������
    virtual bool RecvData(uint16 u2CommandID, ACE_Message_Block* mbRecv, _ClientIPInfo objServerIPInfo) = 0;    //�������ݵĺ���
    //�����쳣�ص�����
    virtual bool ConnectError(int nError, _ClientIPInfo objServerIPInfo)                                = 0;    //�������ʱ�򣬵��ô˽ӿڷ��ش�����Ϣ
};

#endif
