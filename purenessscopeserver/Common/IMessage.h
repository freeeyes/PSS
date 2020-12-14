#ifndef _IMESSAGE_H
#define _IMESSAGE_H

#include "IBuffPacket.h"
#include "ProfileTime.h"
#include "TimeStamp.hpp"

//记录消息的一些参数
struct _MessageBase
{
    uint32             m_u4HeadSrcSize    = 0;              //包头原始长度
    uint32             m_u4BodySrcSize    = 0;              //包体原始长度
    uint32             m_u4ConnectID      = 0;              //消息链接ConnectID，如果是UDP则这个值无效
    uint16             m_u2ClientPort     = 0;              //客户端端口
    uint16             m_u2ListenPort     = 0;              //监听端口
    uint32             m_u4WorkThreadID   = 0;              //工作线程ID
    uint32             m_u4PacketParseID  = 0;              //与ConnectID对应的解析器ID
    uint16             m_u2Cmd            = 0;              //命令字的ID
    EM_CONNECT_IO_TYPE m_emPacketType  = EM_CONNECT_IO_TYPE::CONNECT_IO_TCP;               //数据包来源类型
    EM_PACKET_RESOURCE m_emResouceType = EM_PACKET_RESOURCE::PACKET_RESOURCE_FROM_CLIENT;  //数据来源，服务器间数据包或者客户端数据包
    string             m_strClientIP;                       //客户端IP
    string             m_strListenIP;                       //监听IP
    char               m_szTraceID[MAX_BUFF_50] = {'\0'};   //TraceID
    CProfileTime       m_ProfileTime;                       //消息到达时间
    PSS_Time_Point     m_tvRecvTime;                        //消息接收时间

    _MessageBase()
    {
        m_emPacketType    = EM_CONNECT_IO_TYPE::CONNECT_IO_TCP; //默认为TCP
        m_emResouceType   = EM_PACKET_RESOURCE::PACKET_RESOURCE_FROM_CLIENT; //默认为来源于客户端的数据包
        m_u4ConnectID     = 0;
        m_u2Cmd           = 0;
        m_u4HeadSrcSize   = 0;
        m_u4BodySrcSize   = 0;
        m_u4WorkThreadID  = 0;
        m_u4PacketParseID = 0;
        m_u2ListenPort    = 0;
        m_u2ClientPort     = 0;
        m_szTraceID[0]    = '\0';
    }

    void Clear()
    {
		m_emPacketType    = EM_CONNECT_IO_TYPE::CONNECT_IO_TCP; //默认为TCP
		m_emResouceType   = EM_PACKET_RESOURCE::PACKET_RESOURCE_FROM_CLIENT; //默认为来源于客户端的数据包
        m_u4ConnectID     = 0;
        m_u2Cmd           = 0;
        m_u4HeadSrcSize   = 0;
        m_u4BodySrcSize   = 0;
        m_u4WorkThreadID  = 0;
        m_u4PacketParseID = 0;
        m_u2ListenPort    = 0;
        m_u2ClientPort    = 0;
        m_szTraceID[0]    = '\0';
    }
};

//消息信息类的接口
class IMessage
{
public:
    virtual ~IMessage() {}

    virtual bool GetPacketHead(_PacketInfo& PacketInfo)    = 0;               //得到包头结构体，并赋值给_PacketInfo对象
    virtual bool GetPacketBody(_PacketInfo& PacketInfo)    = 0;               //得到包体结构体，并赋值给_PacketInfo对象
    virtual bool SetPacketHead(ACE_Message_Block* pmbHead) = 0;               //设置包头数据块
    virtual bool SetPacketBody(ACE_Message_Block* pmbBody) = 0;               //设置包体数据块

    virtual _MessageBase* GetMessageBase()                 = 0;               //得到包连接基本信息
};

//中间服务器消息类接口
class IPostMessage
{
public:
    IPostMessage()
    {
        m_pRecvPacket = nullptr;
        m_pSendPacket = nullptr;
        m_u4ServerID  = 0;
        m_u2CommandID = 0;
        m_blDelete    = true;
    }

    virtual ~IPostMessage()
    {
        Close();
    }

    virtual void Close()
    {
        if(nullptr != m_pRecvPacket)
        {
            delete m_pRecvPacket;
            m_pRecvPacket = nullptr;
        }

        if(nullptr != m_pSendPacket)
        {
            delete m_pSendPacket;
            m_pSendPacket = nullptr;
        }

        m_u4ServerID  = 0;
        m_u2CommandID = 0;
        m_blDelete    = true;
    }

    virtual bool SetRecvPacket(IBuffPacket* pRecvPacket)
    {
        if(nullptr == pRecvPacket)
        {
            return false;
        }

        m_pRecvPacket = pRecvPacket;
        return true;
    }

    virtual bool SetSendPacket(IBuffPacket* pSendPacket)
    {
        if(nullptr == pSendPacket)
        {
            return false;
        }

        m_pSendPacket = pSendPacket;
        return true;
    }

    virtual IBuffPacket*  GetRecvPacket()
    {
        return m_pRecvPacket;
    }

    virtual IBuffPacket*  GetSendPacket()
    {
        return m_pSendPacket;
    }

    virtual void CallBefore()              = 0;
    virtual void CallBack()                = 0;

    virtual void   SetServerID(uint32 u4ServerID)
    {
        m_u4ServerID = u4ServerID;
    }

    virtual uint32 GetServerID()
    {
        return m_u4ServerID;
    }

    virtual void   SetCommandID(uint16 u2CommandID)
    {
        m_u2CommandID = u2CommandID;
    }

    virtual uint16 GetCommandID()
    {
        return m_u2CommandID;
    }

private:
    IBuffPacket* m_pRecvPacket;
    IBuffPacket* m_pSendPacket;
    uint32       m_u4ServerID;
    uint16       m_u2CommandID;
    bool         m_blDelete;
};

#endif
