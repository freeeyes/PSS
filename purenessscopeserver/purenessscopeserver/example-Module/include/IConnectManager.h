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
    /*
    参数说明
    u4ConnectID 是要下发对应的ConnectID
    pBuffPacket 一般是数据内容，pData同理
    u1SendType  是发送数据包的类型，目前包括SENDMESSAGE_NOMAL(数据包通过packetParse发送)和SENDMESSAGE_JAMPNOMAL(直接发送该数据包)
    u2CommandID 是要发送包对应的信令ID，用于框架对数据包的统计。
    u1SendState 包括以下几种状态PACKET_SEND_IMMEDIATLY(立刻发送),PACKET_SEND_CACHE(数据包发送机制)和PACKET_SEND_FIN_CLOSE(立刻发送，发送完成后关闭连接)
    blDlete     是否由框架删除数据包内存块，true是框架删除，false由插件自己管理内存块的释放。
    nMessageID  这个参数用于数据回执，如果ID > 0，则如果发送成功，会回调CLINET_LINK_SENDOK，插件可以关注这个事件获得结果，发送失败会调用CLINET_LINK_SENDERROR
    */
    //异步发送指定的数据包(经过PacketParse整理发送包) IBuffPacket 会在内部用完自行销毁
    virtual bool PostMessage(uint32 u4ConnectID, IBuffPacket*& pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL,
                             uint16 u2CommandID = 0, uint8 u1SendState = 0, bool blDlete = true, int nMessageID = 0)   = 0;
    //异步发送指定的数据包(不经过PacketParse整理发送包) pData在里面不负责销毁，需要在逻辑模块自行处理
    virtual bool PostMessage(uint32 u4ConnectID, const char*& pData, uint32 nDataLen, uint8 u1SendType = SENDMESSAGE_NOMAL,
                             uint16 u2CommandID = 0, uint8 u1SendState = 0, bool blDlete = true, int nMessageID = 0)   = 0;
    //异步群发指定的数据包(经过PacketParse整理发送包) IBuffPacket 会在内部用完自行销毁
    virtual bool PostMessage(vector<uint32> vecConnectID, IBuffPacket*& pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL,
                             uint16 u2CommandID = 0, uint8 u1SendState = 0, bool blDlete = true, int nMessageID = 0)   = 0;
    //异步群发指定的数据包(不经过PacketParse整理发送包) pData在里面不负责销毁，需要在逻辑模块自行处理
    virtual bool PostMessage(vector<uint32> vecConnectID, const char*& pData, uint32 nDataLen, uint8 u1SendType = SENDMESSAGE_NOMAL,
                             uint16 u2CommandID = 0, uint8 u1SendState = 0, bool blDlete = true, int nMessageID = 0)   = 0;
    //异步全发指定的数据包(经过PacketParse整理发送包) IBuffPacket 会在内部用完自行销毁
    virtual bool PostMessageAll(IBuffPacket*& pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL,
                                uint16 u2CommandID = 0,uint8 u1SendState = 0, bool blDlete = true, int nMessageID = 0)    = 0;
    //异步全发指定的数据包(不经过PacketParse整理发送包) pData在里面不负责销毁，需要在逻辑模块自行处理
    virtual bool PostMessageAll(const char*& pData, uint32 nDataLen, uint8 u1SendType = SENDMESSAGE_NOMAL,
                                uint16 u2CommandID = 0, uint8 u1SendState = 0, bool blDlete = true, int nMessageID = 0)   = 0;
    //服务器关闭连接
    virtual bool CloseConnect(uint32 u4ConnectID)                                                                = 0;
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
