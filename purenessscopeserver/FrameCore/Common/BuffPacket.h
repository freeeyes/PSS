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
    CBuffPacket();
    ~CBuffPacket(void);

    virtual uint32 GetPacketSize();    //�õ����ݰ��ĸ�ʽ������
    virtual uint32 GetPacketLen();     //�õ����ݰ���ʵ�ʳ���
    virtual uint32 GetReadLen();       //�õ�����ȡ�ĳ���
    virtual uint32 GetWriteLen();      //�õ���д��ĳ���
    virtual uint32 GetHeadLen();       //�õ����ݰ�ͷ�ĳ���
    virtual uint32 GetPacketCount();   //�õ��������ݰ��ĸ���
    virtual const char*  GetData() const;  //�õ���ǰ����ָ��

    bool Init(int32 nSize = DEFINE_PACKET_SIZE, int32 nMaxBuffSize = MAX_PACKET_SIZE);
    virtual bool Close();              //ɾ���Ѿ�ʹ�õ��ڴ�
    virtual bool Clear();              //������еı�־λ������ɾ���ڴ档

    virtual bool WriteStream(const char* pData, uint32 u4Len);
    virtual bool ReadStream(char* pData, uint32& u4Len);
    virtual char* ReadPtr();                            //��ö�ָ��
    virtual char* WritePtr();                           //���дָ��
    virtual void SetReadPtr(uint32 u4Pos);              //���ö�ָ���λ��
    virtual void SetPacketCount(uint32 u4PacketCount);  //���û������ݰ��ĸ���
    virtual bool RollBack(uint32 u4Len);                //��ȡ��������ɾ��������������ݼ���
    virtual void ReadBuffPtr(uint32 u4Size);
    virtual void WriteBuffPtr(uint32 u4Size);

    bool AddBuff(uint32 u4Size);                //����Packet�İ���С


    void SetNetSort(bool blState);              //�����ֽ��������أ�falseΪ��������trueΪ��������

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
    virtual CBuffPacket& operator >> (uint8& u1Data);
    virtual CBuffPacket& operator >> (uint16& u2Data);
    virtual CBuffPacket& operator >> (uint32& u4Data);
    virtual CBuffPacket& operator >> (uint64& u8Data);
    virtual CBuffPacket& operator >> (int8& n1Data);
    virtual CBuffPacket& operator >> (int16& n2Data);
    virtual CBuffPacket& operator >> (int32& n8Data);

    virtual CBuffPacket& operator >> (float32& f4Data);
    virtual CBuffPacket& operator >> (float64& f8Data);

    virtual CBuffPacket& operator >> (VCHARS_STR& str);
    virtual CBuffPacket& operator >> (VCHARM_STR& str);
    virtual CBuffPacket& operator >> (VCHARB_STR& str);
    virtual CBuffPacket& operator >> (string& str);

    //д��
    virtual CBuffPacket& operator << (uint8 u1Data);
    virtual CBuffPacket& operator << (uint16 u2Data);
    virtual CBuffPacket& operator << (uint32 u4Data);
    virtual CBuffPacket& operator << (uint64 u8Data);
    virtual CBuffPacket& operator << (int8 u1Data);
    virtual CBuffPacket& operator << (int16 u2Data);
    virtual CBuffPacket& operator << (int32 u4Data);

    virtual CBuffPacket& operator << (float32 f4Data);
    virtual CBuffPacket& operator << (float64 f8Data);

    virtual CBuffPacket& operator << (VCHARS_STR& str);
    virtual CBuffPacket& operator << (VCHARM_STR& str);
    virtual CBuffPacket& operator << (VCHARB_STR& str);
    virtual CBuffPacket& operator << (string& str);

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
