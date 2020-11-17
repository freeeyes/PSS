#ifndef _PACKETPARSEBASE_H
#define _PACKETPARSEBASE_H

//������ǰѲ������仯�ĺ������������ֻ�ÿ�����ȥʵ�ֱ�Ҫ��5���ӿ�
//ʣ�µģ������߲���Ҫ��ϵ�����ǿ���Լ�ʵ�ֵ����顣
//���԰������������࣬����������ࡣ
//���ÿһ��εĽ���������������ȥ����������롣
//add by freeeyes

#include "BuffPacket.h"
#include "IMessageBlockManager.h"

class CPacketParseBase
{
public:
    CPacketParseBase(void);

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
    uint32 m_u4PacketHead                 = 0;         //��ͷ�ĳ���
    uint32 m_u4PacketBody                 = 0;         //����ĳ���
    uint32 m_u4HeadSrcSize                = 0;         //��ͷ��ԭʼ����
    uint32 m_u4BodySrcSize                = 0;         //�����ԭʼ����
    uint16 m_u2PacketCommandID            = 0;         //������
    bool   m_blIsHandleHead               = true;
    char   m_szPacketVersion[MAX_BUFF_20] = {'\0'};    //���������汾
    uint8  m_u1Sort                       = 0;         //�ֽ������0Ϊ�����ֽ���1Ϊ�����ֽ���

    ACE_Message_Block* m_pmbHead          = nullptr;      //��ͷ����
    ACE_Message_Block* m_pmbBody          = nullptr;      //���岿��
};

#endif
