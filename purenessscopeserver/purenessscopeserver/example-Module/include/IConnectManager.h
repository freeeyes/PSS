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
	//关闭远程连接
	virtual bool CloseConnect(uint32 u4ConnectID)                                                                = 0;
	//得到某一个ConnectID的IP和端口信息
	virtual _ClientIPInfo GetClientIPInfo(uint32 u4ConnectID)                                                    = 0;
	//得到当前活跃连接的总数
	virtual int  GetCount()                                                                                      = 0;
	//设置当前连接的别名，默认为这个链接的IP
	virtual bool SetConnectName(uint32 u4ConnectID, const char* pName)                                           = 0;
	//设置是否打开这个日志的别名            
  virtual bool SetIsLog(uint32 u4ConnectID, bool blIsLog)                                                      = 0;                                                         
};

#endif
 