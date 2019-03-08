#ifndef _IBUFFPACKET_H
#define _IBUFFPACKET_H

#include "define.h"

//BuffPacket�Ľӿ���
class IBuffPacket
{
public:
    virtual ~IBuffPacket() {}

    virtual uint32 GetPacketSize()      = 0;    //�õ����ݰ��ĸ�ʽ������
    virtual uint32 GetPacketLen()       = 0;    //�õ����ݰ���ʵ�ʳ���
    virtual uint32 GetReadLen()         = 0;    //�õ�����ȡ�ĳ���
    virtual uint32 GetWriteLen()        = 0;    //�õ���д��ĳ���
    virtual uint32 GetHeadLen()         = 0;    //�õ����ݰ�ͷ�ĳ���
    virtual uint32 GetPacketCount()     = 0;    //�õ��������ݰ��ĸ���

    virtual bool Close()                = 0;
    virtual bool Clear()                = 0;

    virtual bool AddBuff(uint32 u4Size) = 0;
    virtual const char* GetData()       = 0;

    virtual void SetReadPtr(uint32 u4Pos)                                = 0;    //���ö�ָ���λ��
    virtual void SetPacketCount(uint32 u4PacketCount)                    = 0;    //���û������ݰ��ĸ���
    virtual bool WriteStream(const char* pData, uint32 u4Len)            = 0;    //ֱ��д��һ�����������ݿ�
    virtual bool ReadStream(char* pData, uint32& u4Len)                  = 0;    //ֱ�Ӷ�ȡ��һ�����������ݿ�
    virtual char* ReadPtr()                                              = 0;    //��ö�ָ��
    virtual char* WritePtr()                                             = 0;    //���дָ��
    virtual bool RollBack(uint32 u4Len)                                  = 0;    //��ȡ��������ɾ��������������ݼ���

    virtual void ReadBuffPtr(uint32 u4Size)                              = 0;    //�ƶ�ָ�����ȵĶ�ָ��
    virtual void WriteBuffPtr(uint32 u4Size)                             = 0;    //�ƶ�ָ�����ȵ�дָ��

    virtual void SetNetSort(bool blState)                                = 0;    //�����Ƿ��������ֽ���falseΪ��������trueΪ����

    //��ȡ
    virtual IBuffPacket& operator >> (uint8& u1Data)   = 0;        //����һ��uint8��1�ֽ��޷�������
    virtual IBuffPacket& operator >> (uint16& u2Data)  = 0;        //����һ��uint16��2�ֽ��޷�������
    virtual IBuffPacket& operator >> (uint32& u4Data)  = 0;        //����һ��uint32��4�ֽ��޷�������
    virtual IBuffPacket& operator >> (uint64& u8Data)  = 0;        //����һ��uint64��8�ֽ��޷�������
    virtual IBuffPacket& operator >> (int8& n1Data)    = 0;        //����һ��int8��1�ֽ��޷�������
    virtual IBuffPacket& operator >> (int16& n2Data)   = 0;        //����һ��int16��2�ֽ��޷�������
    virtual IBuffPacket& operator >> (int32& n8Data)   = 0;        //����һ��int32��4�ֽ��޷�������
    virtual IBuffPacket& operator >> (float32& f4Data) = 0;        //����һ��float32��4�ֽ�С��
    virtual IBuffPacket& operator >> (float64& f8Data) = 0;        //����һ��float64��8�ֽ�С��

    virtual IBuffPacket& operator >> (VCHARS_STR& str) = 0;        //����һ��һ���ֽڳ��ȵ����ݿ飨����1��uint8�����ݳ��Ⱥͺ�������ݿ�ʵ�壩
    virtual IBuffPacket& operator >> (VCHARM_STR& str) = 0;        //����һ��һ���ֽڳ��ȵ����ݿ飨����1��uint16�����ݳ��Ⱥͺ�������ݿ�ʵ�壩
    virtual IBuffPacket& operator >> (VCHARB_STR& str) = 0;        //����һ��һ���ֽڳ��ȵ����ݿ飨����1��uint32�����ݳ��Ⱥͺ�������ݿ�ʵ�壩
    virtual IBuffPacket& operator >> (string& str)     = 0;        //����һ��string

    //д��
    virtual IBuffPacket& operator << (uint8 u1Data)    = 0;        //д��һ��uint8��1�ֽ��޷�������
    virtual IBuffPacket& operator << (uint16 u2Data)   = 0;        //д��һ��uint16��2�ֽ��޷�������
    virtual IBuffPacket& operator << (uint32 u4Data)   = 0;        //д��һ��uint32��4�ֽ��޷�������
    virtual IBuffPacket& operator << (uint64 u8Data)   = 0;        //д��һ��uint64��8�ֽ��޷�������
    virtual IBuffPacket& operator << (int8 u1Data)     = 0;        //д��һ��int8��1�ֽ��޷�������
    virtual IBuffPacket& operator << (int16 u2Data)    = 0;        //д��һ��int16��2�ֽ��޷�������
    virtual IBuffPacket& operator << (int32 u4Data)    = 0;        //д��һ��int32��4�ֽ��޷�������

    virtual IBuffPacket& operator << (float32 f4Data)  = 0;        //д��һ��float32��4�ֽ�С��
    virtual IBuffPacket& operator << (float64 f8Data)  = 0;        //д��һ��float64��8�ֽ�С��

    virtual IBuffPacket& operator << (VCHARS_STR& str) = 0;        //д��һ��һ���ֽڳ��ȵ����ݿ飨����1��uint8�����ݳ��Ⱥͺ�������ݿ�ʵ�壩
    virtual IBuffPacket& operator << (VCHARM_STR& str) = 0;        //д��һ��һ���ֽڳ��ȵ����ݿ飨����1��uint16�����ݳ��Ⱥͺ�������ݿ�ʵ�壩
    virtual IBuffPacket& operator << (VCHARB_STR& str) = 0;        //д��һ��һ���ֽڳ��ȵ����ݿ飨����1��uint32�����ݳ��Ⱥͺ�������ݿ�ʵ�壩
    virtual IBuffPacket& operator << (string& str)     = 0;        //д��һ��string
};

#endif
