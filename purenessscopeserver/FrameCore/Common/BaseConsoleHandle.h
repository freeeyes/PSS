#ifndef _BASECONSOLEHANDLE_H
#define _BASECONSOLEHANDLE_H

#include "BuffPacketManager.h"
#include "PacketConsoleParse.h"
#include "MessageBlockManager.h"
#include "ConsoleMessage.h"
#include "XmlConfig.h"

//���Ҫ���͵�Console����
bool Console_Common_SendMessage_Data_Check(uint32 u4ConnectID, IBuffPacket* pBuffPacket, uint8 u1OutputType, ACE_Message_Block*& pMbData);

//�����յ�Console����
bool Console_Common_CheckMessage_Data(uint32& u4AllRecvSize, uint32& u4AllRecvCount, shared_ptr<CConsolePacketParse> pPacketParse, uint8& u1Output, IBuffPacket*& pBuffPacket);

//���Console���ӵ�IP�ǲ����������Ip�б���
bool check_console_ip(const char* pConsoleIP);

#endif
