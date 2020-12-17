#ifndef _BASECONNECTCLIENT_H
#define _BASECONNECTCLIENT_H

//实现*ConnectClient公共的代码部分。
//add by freeeyes

#include "MessageBlockManager.h"
#include "ClientMessage.h"
#include "MessageService.h"
#include "XmlConfig.h"

//接收来自服务器间通讯的消息，并处理
bool Recv_Common_Dispose_Client_Message(uint16 u2CommandID, ACE_Message_Block* pRecvFinish, const _ClientIPInfo& objServerIPInfo, IClientMessage* pClientMessage);

//将收到的消息组织到工作线程去处理
int Make_Common_Dispose_Client_WorkTread_Message(uint16 u2CommandID, uint32 u4ServerID, ACE_Message_Block* pmblk, const ACE_INET_Addr& AddrRemote);

#endif
