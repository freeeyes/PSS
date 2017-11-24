#ifndef _IMESSAGE_H
#define _IMESSAGE_H

#include "IBuffPacket.h"
#include "ProfileTime.h"

//记录消息的一些参数
struct _MessageBase
{
    uint32         m_u4HeadSrcSize;             //包头原始长度
    uint32         m_u4BodySrcSize;             //包体原始长度
    uint32         m_u4ConnectID;               //消息链接ConnectID，如果是UDP则这个值无效
    uint32         m_u4PacketID;                //数据包的ID
    uint32         m_u4Port;                    //客户端端口
    uint32         m_u4ListenPort;              //监听端口
    uint32         m_u4WorkThreadID;            //工作线程ID
    uint32         m_u4PacketParseID;           //与ConnectID对应的解析器ID
    uint16         m_u2Cmd;                     //命令字的ID
    uint8          m_u1PacketType;              //数据包来源类型
    char           m_szIP[MAX_BUFF_20];         //客户端IP
    char           m_szListenIP[MAX_BUFF_20];   //监听IP
    char           m_szTraceID[MAX_BUFF_50];    //TraceID
    CProfileTime   m_ProfileTime;               //消息到达时间
    ACE_Time_Value m_tvRecvTime;                //消息接收时间

    _MessageBase()
    {
        m_u1PacketType    = PACKET_TCP;   //默认为TCP
        m_u4ConnectID     = 0;
        m_u4PacketID      = 0;
        m_u2Cmd           = 0;
        m_u4HeadSrcSize   = 0;
        m_u4BodySrcSize   = 0;
        m_u4WorkThreadID  = 0;
        m_u4PacketParseID = 0;
        m_u4ListenPort    = 0;
        m_u4Port          = 0;
        m_szIP[0]         = '\0';
        m_szListenIP[0]   = '\0';
        m_szTraceID[0]    = '\0';
    }

    void Clear()
    {
        m_u1PacketType    = PACKET_TCP;   //默认为TCP
        m_u4ConnectID     = 0;
        m_u4PacketID      = 0;
        m_u2Cmd           = 0;
        m_u4HeadSrcSize   = 0;
        m_u4BodySrcSize   = 0;
        m_u4WorkThreadID  = 0;
        m_u4PacketParseID = 0;
        m_u4ListenPort    = 0;
        m_u4Port          = 0;
        m_szIP[0]         = '\0';
        m_szListenIP[0]   = '\0';
        m_szTraceID[0]    = '\0';
    }
};

//消息信息类的接口
class IMessage
{
public:
    virtual ~IMessage() {}

    virtual void Close() = 0;
    virtual void Clear() = 0;

    virtual void SetMessageBase(_MessageBase* pMessageBase)              = 0; //设置连接基本信息

    virtual bool GetPacketHead(_PacketInfo& PacketInfo)    = 0;               //得到包头结构体，并赋值给_PacketInfo对象
    virtual bool GetPacketBody(_PacketInfo& PacketInfo)    = 0;               //得到包体结构体，并赋值给_PacketInfo对象
    virtual bool SetPacketHead(ACE_Message_Block* pmbHead) = 0;               //设置包头数据块
    virtual bool SetPacketBody(ACE_Message_Block* pmbBody) = 0;               //设置包体数据块

    virtual _MessageBase* GetMessageBase()                 = 0;               //得到包连接基本信息

    virtual const char* GetError()                         = 0;
};

//中间服务器消息类接口
class IPostMessage
{
public:
    IPostMessage()
    {
        m_pRecvPacket = NULL;
        m_pSendPacket = NULL;
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
        if(NULL != m_pRecvPacket)
        {
            delete m_pRecvPacket;
            m_pRecvPacket = NULL;
        }

        if(NULL != m_pSendPacket)
        {
            delete m_pSendPacket;
            m_pSendPacket = NULL;
        }

        m_u4ServerID  = 0;
        m_u2CommandID = 0;
        m_blDelete    = true;
    }

    virtual bool SetRecvPacket(IBuffPacket* pRecvPacket)
    {
        if(NULL == pRecvPacket)
        {
            return false;
        }

        m_pRecvPacket = pRecvPacket;
        return true;
    }

    virtual bool SetSendPacket(IBuffPacket* pSendPacket)
    {
        if(NULL == pSendPacket)
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
