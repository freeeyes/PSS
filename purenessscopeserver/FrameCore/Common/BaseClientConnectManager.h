#ifndef _BASECLIENTCONNECTMANAGER_H
#define _BASECLIENTCONNECTMANAGER_H

//此类实现Client*ConnectManager的公共方法
//add by freeeyes
#include "IClientManager.h"
#include "MessageBlockManager.h"

//向插件接口发送连接远程服务器失败信息
void Common_Send_ConnectError(ACE_Message_Block* pmblk, const ACE_INET_Addr& objAddrServer, IClientMessage* pClientMessage);

bool Common_Send_Data(ACE_Message_Block* pmblk, IClientMessage* pClientMessage, ACE_Message_Block*& pSend);

#endif
