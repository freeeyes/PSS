#ifndef _BUFFPACKET_H
#define _BUFFPACKET_H

#include "ace/Thread_Mutex.h"

#include "ACEMemory.hpp"
#include "IBuffPacket.h"
#include <stdexcept>
#include <string>
#include <sstream>

const uint32 DEFINE_PACKET_SIZE      = 1024;
const uint32 DEFINE_PACKET_ADD       = 1024;
const uint8  USER_PACKET_MEMORY_POOL = 1; //����ʹ��ACE�ڴ�ط���

class CBuffPacket : public IBuffPacket
{
public:
    CBuffPacket() = default;
    ~CBuffPacket(void) final = default;

    uint32 GetPacketSize() final;       //�õ����ݰ��ĸ�ʽ������
    uint32 GetPacketLen() final;        //�õ����ݰ���ʵ�ʳ���
    uint32 GetReadLen() final;          //�õ�����ȡ�ĳ���
    uint32 GetWriteLen() final;         //�õ���д��ĳ���
    uint32 GetHeadLen() final;          //�õ����ݰ�ͷ�ĳ���
    uint32 GetPacketCount() final;      //�õ��������ݰ��ĸ���
    const char*  GetData() const final; //�õ���ǰ����ָ��

    bool Init(int32 nSize = DEFINE_PACKET_SIZE, int32 nMaxBuffSize = MAX_PACKET_SIZE);
    bool Close() final;              //ɾ���Ѿ�ʹ�õ��ڴ�
    bool Clear() final;              //������еı�־λ������ɾ���ڴ档

    bool WriteStream(const char* pData, uint32 u4Len) final;
    bool ReadStream(char* pData, uint32& u4Len) final;
    char* ReadPtr() final;                            //��ö�ָ��
    char* WritePtr() final;                           //���дָ��
    void SetReadPtr(uint32 u4Pos) final;              //���ö�ָ���λ��
    void SetPacketCount(uint32 u4PacketCount) final;  //���û������ݰ��ĸ���
    bool RollBack(uint32 u4Len) final;                //��ȡ��������ɾ��������������ݼ���
    void ReadBuffPtr(uint32 u4Size) final;
    void WriteBuffPtr(uint32 u4Size) final;

    bool AddBuff(uint32 u4Size) final;                //����Packet�İ���С


    void SetNetSort(bool blState) final;              //�����ֽ��������أ�falseΪ��������trueΪ��������

    void SetBuffID(uint32 u4BuffID);            //����BuffID
    uint32 GetBuffID() const;                   //�õ�BuffID

    int32 GetHashID() const;                    //����HashID
    void SetHashID(int32 nHashID);              //�õ�HashID

    const char* GetError() const;               //���ش�����Ϣ

private:
    void ReadPtr(uint32 u4Size);
    void WritePtr(uint32 u4Size);


public:
    //��ȡ
    CBuffPacket& operator >> (uint8& u1Data) final;
    CBuffPacket& operator >> (uint16& u2Data) final;
    CBuffPacket& operator >> (uint32& u4Data) final;
    CBuffPacket& operator >> (uint64& u8Data) final;
    CBuffPacket& operator >> (int8& n1Data) final;
    CBuffPacket& operator >> (int16& n2Data) final;
    CBuffPacket& operator >> (int32& n8Data) final;

    CBuffPacket& operator >> (float32& f4Data) final;
    CBuffPacket& operator >> (float64& f8Data) final;

    CBuffPacket& operator >> (VCHARS_STR& str) final;
    CBuffPacket& operator >> (VCHARM_STR& str) final;
    CBuffPacket& operator >> (VCHARB_STR& str) final;
    CBuffPacket& operator >> (string& str) final;

    //д��
    CBuffPacket& operator << (uint8 u1Data) final;
    CBuffPacket& operator << (uint16 u2Data) final;
    CBuffPacket& operator << (uint32 u4Data) final;
    CBuffPacket& operator << (uint64 u8Data) final;
    CBuffPacket& operator << (int8 u1Data) final;
    CBuffPacket& operator << (int16 u2Data) final;
    CBuffPacket& operator << (int32 u4Data) final;

    CBuffPacket& operator << (float32 f4Data) final;
    CBuffPacket& operator << (float64 f8Data) final;

    CBuffPacket& operator << (VCHARS_STR& str) final;
    CBuffPacket& operator << (VCHARM_STR& str) final;
    CBuffPacket& operator << (VCHARB_STR& str) final;
    CBuffPacket& operator << (string& str) final;

private:
    char*                      m_szData          = nullptr;                //����ָ��
    uint32                     m_u4ReadPtr       = 0;                   //������λ��
    uint32                     m_u4WritePtr      = 0;                   //д����λ��
    uint32                     m_u4PacketLen     = 0 ;                  //���ܳ���
    uint32                     m_u4PacketCount   = 0;                   //��ǰ���ݰ��ĸ���
    uint32                     m_u4MaxPacketSize = 0;                   //���ǰBuffPacket������
    uint32                     m_u4BuffID        = 0;                   //Buffer�������֣����ں˶��ڴ������ʹ�ã�Ѱ���ڴ�й¶��
    int32                      m_nHashID         = 0;                   //��¼��ǰ������Hash�����е�λ��
    bool                       m_blNetSort       = false;               //�ֽ��������أ�falseΪ��ת��Ϊ�����ֽ���trueΪת��Ϊ�����ֽ���
    string                     m_strError;                              //������Ϣ

public:
    void* operator new(size_t stSize)
    {
        return App_ACEMemory::instance()->malloc(stSize);
    }

    void operator delete(void* p)
    {
        App_ACEMemory::instance()->free(p);
    }
};
#endif
