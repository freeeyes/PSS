#ifndef _LOADPACKETPARSE_H
#define _LOADPACKETPARSE_H

#include "ace/Date_Time.h"
#include "ace/Thread_Mutex.h"
#include "ace/Singleton.h"
#include "ace/OS_NS_dlfcn.h"

#include "define.h"
#include "IMessageBlockManager.h"

struct _Packet_Parse_Info
{
	ACE_Date_Time    dtCreateTime;          //模块创建时间
	ACE_SHLIB_HANDLE hModule;
	bool (*Parse_Packet_Head_Info)(uint32 u4ConnectID, ACE_Message_Block* pmbHead, IMessageBlockManager* pMessageBlockManager, _Head_Info* pHeadInfo);
	bool (*Parse_Packet_Body_Info)(uint32 u4ConnectID, ACE_Message_Block* pmbbody, IMessageBlockManager* pMessageBlockManager, _Body_Info* pBodyInfo);
	uint8 (*Parse_Packet_Stream)(uint32 u4ConnectID, ACE_Message_Block* pCurrMessage, IMessageBlockManager* pMessageBlockManager, _Packet_Info* pPacketInfo);
	bool (*Make_Send_Packet)(uint32 u4ConnectID, const char* pData, uint32 u4Len, ACE_Message_Block* pMbData, uint16 u2CommandID);
	uint32 (*Make_Send_Packet_Length)(uint32 u4ConnectID, uint32 u4DataLen, uint16 u2CommandID);
	bool (*Connect)(uint32 u4ConnectID, _ClientIPInfo objClientIPInfo, _ClientIPInfo objLocalIPInfo);
	void (*DisConnect)(uint32 u4ConnectID);
};

class CLoadPacketParse
{
public:
	CLoadPacketParse();
	~CLoadPacketParse();

	bool LoadPacketInfo(char* szPacketParseName);

	void Close();

	_Packet_Parse_Info* GetPacketParseInfo();

private:
	_Packet_Parse_Info m_Packet_Parse_Info;
};

typedef ACE_Singleton<CLoadPacketParse, ACE_Null_Mutex> App_PacketParseLoader; 

#endif
