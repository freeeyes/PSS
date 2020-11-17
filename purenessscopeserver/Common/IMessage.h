#ifndef _IMESSAGE_H
#define _IMESSAGE_H

#include "IBuffPacket.h"
#include "ProfileTime.h"

//��¼��Ϣ��һЩ����
struct _MessageBase
{
    uint32             m_u4HeadSrcSize    = 0;              //��ͷԭʼ����
    uint32             m_u4BodySrcSize    = 0;              //����ԭʼ����
    uint32             m_u4ConnectID      = 0;              //��Ϣ����ConnectID�������UDP�����ֵ��Ч
    uint16             m_u2ClientPort     = 0;              //�ͻ��˶˿�
    uint16             m_u2ListenPort     = 0;              //�����˿�
    uint32             m_u4WorkThreadID   = 0;              //�����߳�ID
    uint32             m_u4PacketParseID  = 0;              //��ConnectID��Ӧ�Ľ�����ID
    uint16             m_u2Cmd            = 0;              //�����ֵ�ID
    EM_CONNECT_IO_TYPE m_emPacketType  = EM_CONNECT_IO_TYPE::CONNECT_IO_TCP;               //���ݰ���Դ����
    EM_PACKET_RESOURCE m_emResouceType = EM_PACKET_RESOURCE::PACKET_RESOURCE_FROM_CLIENT;  //������Դ�������������ݰ����߿ͻ������ݰ�
    string             m_strClientIP;                       //�ͻ���IP
    string             m_strListenIP;                       //����IP
    char               m_szTraceID[MAX_BUFF_50] = {'\0'};   //TraceID
    CProfileTime       m_ProfileTime;                       //��Ϣ����ʱ��
    ACE_Time_Value     m_tvRecvTime;                        //��Ϣ����ʱ��

    _MessageBase()
    {
        m_emPacketType    = EM_CONNECT_IO_TYPE::CONNECT_IO_TCP; //Ĭ��ΪTCP
        m_emResouceType   = EM_PACKET_RESOURCE::PACKET_RESOURCE_FROM_CLIENT; //Ĭ��Ϊ��Դ�ڿͻ��˵����ݰ�
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
		m_emPacketType    = EM_CONNECT_IO_TYPE::CONNECT_IO_TCP; //Ĭ��ΪTCP
		m_emResouceType   = EM_PACKET_RESOURCE::PACKET_RESOURCE_FROM_CLIENT; //Ĭ��Ϊ��Դ�ڿͻ��˵����ݰ�
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

//��Ϣ��Ϣ��Ľӿ�
class IMessage
{
public:
    virtual ~IMessage() {}

    virtual bool GetPacketHead(_PacketInfo& PacketInfo)    = 0;               //�õ���ͷ�ṹ�壬����ֵ��_PacketInfo����
    virtual bool GetPacketBody(_PacketInfo& PacketInfo)    = 0;               //�õ�����ṹ�壬����ֵ��_PacketInfo����
    virtual bool SetPacketHead(ACE_Message_Block* pmbHead) = 0;               //���ð�ͷ���ݿ�
    virtual bool SetPacketBody(ACE_Message_Block* pmbBody) = 0;               //���ð������ݿ�

    virtual _MessageBase* GetMessageBase()                 = 0;               //�õ������ӻ�����Ϣ
};

//�м��������Ϣ��ӿ�
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
