#ifndef _ICONNECTMANAGER_H
#define _ICONNECTMANAGER_H

#include "IBuffPacket.h"
#include <vector>

enum class EM_Client_Connect_status
{
    CLIENT_CONNECT_EXIST = 0,
    CLIENT_CONNECT_NO_EXIST,
};

enum class EM_Client_Send_Status
{
    CLIENT_SEND_IMMEDIATLY = 0,   //立刻发送
    CLIENT_SEND_CACHE,            //发送到缓存中
    CLIENT_CLOSE_SENDOK,          //发送完成关闭连接
};

//发送参数配置项
class CSend_Param
{
public:
    EM_SEND_PACKET_PARSE   m_emSendType  = EM_SEND_PACKET_PARSE::EM_SENDMESSAGE_JAMPNOMAL;
    EM_Client_Send_Status m_emSendState = EM_Client_Send_Status::CLIENT_SEND_IMMEDIATLY;
    int                    m_nMessageID  = 0;
    bool                   m_blDlete     = true;
};

class IConnectManager
{
public:
    virtual ~IConnectManager() {}
    /*
    参数说明
    u4ConnectID 是要下发对应的ConnectID
    pBuffPacket 一般是数据内容，pData同理
    u2CommandID 是要发送包对应的信令ID，用于框架对数据包的统计。
    CSend_Param 发送参数列表
    */
    //异步发送指定的数据包(经过PacketParse整理发送包) IBuffPacket 会在内部用完自行销毁
    virtual bool PostMessage(uint32 u4ConnectID, uint16 u2CommandID, shared_ptr<IBuffPacket> pBuffPacket, CSend_Param objSendParam) = 0;

    //服务器关闭连接
    virtual bool CloseConnect(uint32 u4ConnectID)                                                                = 0;
    //得到当前连接信息IP和端口
    virtual _ClientIPInfo GetClientIPInfo(uint32 u4ConnectID)                                                    = 0;
    //得到当前连接监听IP和端口
    virtual _ClientIPInfo GetLocalIPInfo(uint32 u4ConnectID)                                                     = 0;
    //得到当前连接数量
    virtual uint32 GetCount()                                                                                    = 0;
    //设置相关ConnectID是否打开日志
    virtual bool SetIsLog(uint32 u4ConnectID, bool blIsLog)                                                      = 0;
    //得到指定的客户端连接状态,false为链接不存在，true为链接存在
    virtual EM_Client_Connect_status GetConnectState(uint32 u4ConnectID)                                         = 0;
};

#endif
