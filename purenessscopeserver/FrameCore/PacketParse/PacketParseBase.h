#ifndef _PACKETPARSEBASE_H
#define _PACKETPARSEBASE_H

//这个类是把不经常变化的函数，放在这里，只让开发者去实现必要的5个接口
//剩下的，开发者不需要关系，这是框架自己实现的事情。
//所以剥离出来了这个类，想让他更简洁。
//坚持每一点滴的进步，持续的走下去，这就是梦想。
//add by freeeyes

#include "BuffPacket.h"
#include "IMessageBlockManager.h"

class CPacketParseBase
{
public:
    CPacketParseBase(void) = default;

    virtual ~CPacketParseBase() = default;

    void Clear();

    void Close();

    const char* GetPacketVersion() const;
    uint32 GetPacketHeadLen() const;
    uint32 GetPacketBodyLen() const;

    uint16 GetPacketCommandID() const;
    bool GetIsHandleHead() const;
    uint32 GetPacketHeadSrcLen() const;
    uint32 GetPacketBodySrcLen() const;

    void SetSort(uint8 u1Sort);

    void Check_Recv_Unit16(uint16& u2Data) const;
    void Check_Recv_Unit32(uint32& u4Data) const;
    void Check_Recv_Unit64(uint64& u2Data) const;

    void Check_Send_Unit16(uint16& u2Data) const;
    void Check_Send_Unit32(uint32& u4Data) const;
    void Check_Send_Unit64(uint64& u8Data) const;

    ACE_Message_Block* GetMessageHead();
    ACE_Message_Block* GetMessageBody();

    virtual void Init() = 0;

    void SetPacket_Head_Curr_Length(uint32 u4CurrLength);
    void SetPacket_Body_Curr_Length(uint32 u4CurrLength);
    void SetPacket_Head_Src_Length(uint32 u4SrcLength);
    void SetPacket_Body_Src_Length(uint32 u4SrcLength);
    void SetPacket_CommandID(uint16 u2PacketCommandID);
    void SetPacket_IsHandleHead(bool blState);
    void SetPacket_Head_Message(ACE_Message_Block* pmbHead);
    void SetPacket_Body_Message(ACE_Message_Block* pmbBody);

private:
    uint32 m_u4PacketHead                 = 0;         //包头的长度
    uint32 m_u4PacketBody                 = 0;         //包体的长度
    uint32 m_u4HeadSrcSize                = 0;         //包头的原始长度
    uint32 m_u4BodySrcSize                = 0;         //包体的原始长度
    uint16 m_u2PacketCommandID            = 0;         //包命令
    bool   m_blIsHandleHead               = true;
    string m_strPacketVersion             = "2.0.0";   //包解析器版本
    uint8  m_u1Sort                       = 0;         //字节序规则，0为主机字节序，1为网络字节序

    ACE_Message_Block* m_pmbHead          = nullptr;      //包头部分
    ACE_Message_Block* m_pmbBody          = nullptr;      //包体部分
};

#endif
