#ifndef _BASECONSOLEHANDLE_H
#define _BASECONSOLEHANDLE_H

#include "BuffPacketManager.h"
#include "PacketConsoleParse.h"
#include "MessageBlockManager.h"
#include "ConsoleMessage.h"
#include "XmlConfig.h"

//检查要发送的Console数据
bool Console_Common_SendMessage_Data_Check(uint32 u4ConnectID, shared_ptr<IBuffPacket> pBuffPacket, uint8 u1OutputType, ACE_Message_Block*& pMbData);

//检查接收的Console数据
bool Console_Common_CheckMessage_Data(uint32& u4AllRecvSize, uint32& u4AllRecvCount, shared_ptr<CConsolePacketParse> pPacketParse, uint8& u1Output, shared_ptr<IBuffPacket>& pBuffPacket);

//检查Console连接的IP是不是在允许的Ip列表中
bool check_console_ip(const char* pConsoleIP);

#endif
