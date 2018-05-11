#ifndef _BASEREACTORHANDLER_H
#define _BASEREACTORHANDLER_H

#include "define.h"
#include "MessageBlockManager.h"
#include "LoadPacketParse.h"
#include "PacketParsePool.h"

//所有的handler用到的公共函数
//add by freeeyes

//回收内存
void Recovery_Message(bool blDelete, char*& pMessage);

//udp函数发送数据包合成函数
bool Udp_Common_Send_Message(uint32 u4PacketParseInfoID, ACE_INET_Addr& AddrRemote, char*& pMessage, uint32 u4Len, const char* szIP, int nPort, bool blHead, uint16 u2CommandID, bool blDlete, ACE_Message_Block*& pMbData);

#endif
