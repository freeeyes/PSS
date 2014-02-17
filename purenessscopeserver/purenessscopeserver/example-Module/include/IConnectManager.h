#ifndef _ICONNECTMANAGER_H
#define _ICONNECTMANAGER_H

#include "IBuffPacket.h"
#include <vector>

class IConnectManager
{
public:
	virtual ~IConnectManager() {};
	//异步发送指定的数据包(经过PacketParse整理发送包) IBuffPacket 会在内部用完自行销毁
	virtual bool PostMessage(uint32 u4ConnectID, IBuffPacket* pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true, bool blDlete = true)                    = 0;  
	//异步发送指定的数据包(不经过PacketParse整理发送包) pData在里面不负责销毁，需要在逻辑模块自行处理
	virtual bool PostMessage(uint32 u4ConnectID, const char* pData, uint32 nDataLen, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true, bool blDlete = true)          = 0; 
	//异步群发指定的数据包(经过PacketParse整理发送包) IBuffPacket 会在内部用完自行销毁
	virtual bool PostMessage(vector<uint32> vecConnectID, IBuffPacket* pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true, bool blDlete = true)           = 0;  
	//异步群发指定的数据包(不经过PacketParse整理发送包) pData在里面不负责销毁，需要在逻辑模块自行处理
	virtual bool PostMessage(vector<uint32> vecConnectID, const char* pData, uint32 nDataLen, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true, bool blDlete = true) = 0;  
	//异步全发指定的数据包(经过PacketParse整理发送包) IBuffPacket 会在内部用完自行销毁
	virtual bool PostMessageAll(IBuffPacket* pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true, bool blDlete = true)                                     = 0;  
	//异步全发指定的数据包(不经过PacketParse整理发送包) pData在里面不负责销毁，需要在逻辑模块自行处理
	virtual bool PostMessageAll(const char* pData, uint32 nDataLen, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, bool blSendState = true, bool blDlete = true)                           = 0;  
	virtual bool CloseConnect(uint32 u4ConnectID)                                                                = 0;
	virtual _ClientIPInfo GetClientIPInfo(uint32 u4ConnectID)                                                    = 0;
	virtual int  GetCount()                                                                                      = 0;
};

#endif
 