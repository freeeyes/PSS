#ifndef _ICONNECTMANAGER_H
#define _ICONNECTMANAGER_H

#include "IBuffPacket.h"
#include <vector>

enum EM_Client_Connect_status
{
	CLIENT_CONNECT_EXIST = 0,
	CLIENT_CONNECT_NO_EXIST,
};

enum EM_Client_Close_status
{
	CLIENT_CLOSE_NOTHING = 0,  //什么都不做
	CLIENT_CLOSE_IMMEDIATLY,   //立即关闭连接
	CLIENT_CLOSE_SENDOK,       //发送完成关闭连接  
};

class IConnectManager
{
public:
    virtual ~IConnectManager() {}
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
	//服务器关闭连接
	virtual bool CloseConnect(uint32 u4ConnectID, EM_Client_Close_status emStatus = CLIENT_CLOSE_IMMEDIATLY)     = 0;
	//得到当前连接信息IP和端口
	virtual _ClientIPInfo GetClientIPInfo(uint32 u4ConnectID)                                                    = 0;
	//得到当前连接监听IP和端口
	virtual _ClientIPInfo GetLocalIPInfo(uint32 u4ConnectID)                                                     = 0;
	//得到当前连接数量
	virtual int  GetCount()                                                                                      = 0;
	//设置相关ConnectID和Name的映射关系
	virtual bool SetConnectName(uint32 u4ConnectID, const char* pName)                                           = 0;
	//设置相关ConnectID是否打开日志
	virtual bool SetIsLog(uint32 u4ConnectID, bool blIsLog)                                                      = 0;
	//得到指定Name的相关ConnectID映射信息
	virtual void GetClientNameInfo(const char* pName, vecClientNameInfo& objClientNameInfo)                      = 0;
	//得到指定的客户端连接状态,false为链接不存在，true为链接存在
	virtual EM_Client_Connect_status GetConnectState(uint32 u4ConnectID)                                         = 0;
};

#endif
 