#ifndef _BASECONNECTCLIENT_H
#define _BASECONNECTCLIENT_H

//ʵ��*ConnectClient�����Ĵ��벿�֡�
//add by freeeyes

#include "MessageBlockManager.h"
#include "ClientMessage.h"
#include "MessageService.h"
#include "XmlConfig.h"

//�������Է�������ͨѶ����Ϣ��������
bool Recv_Common_Dispose_Client_Message(uint16 u2CommandID, ACE_Message_Block* pRecvFinish, const _ClientIPInfo& objServerIPInfo, IClientMessage* pClientMessage);

//���յ�����Ϣ��֯�������߳�ȥ����
int Make_Common_Dispose_Client_WorkTread_Message(uint16 u2CommandID, uint32 u4ServerID, ACE_Message_Block* pmblk, const ACE_INET_Addr& AddrRemote);

#endif
