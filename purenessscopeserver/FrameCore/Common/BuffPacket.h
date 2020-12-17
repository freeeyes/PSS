#ifndef _BUFFPACKET_H
#define _BUFFPACKET_H

#include "ace/Thread_Mutex.h"

#include "IBuffPacket.h"
#include <stdexcept>
#include <string>
#include <sstream>

const uint32 DEFINE_PACKET_SIZE      = 1024;
const uint32 DEFINE_PACKET_ADD       = 1024;
const uint8  USER_PACKET_MEMORY_POOL = 1; //定义使用ACE内存池分配

class CBuffPacket : public IBuffPacket
{
public:
    CBuffPacket() = default;
    CBuffPacket(int32 nSize, int32 nMaxBuffSize);
    ~CBuffPacket() final;

    uint32 GetPacketSize() final;       //得到数据包的格式化长度
    uint32 GetPacketLen() final;        //得到数据包的实际长度
    uint32 GetReadLen() final;          //得到包读取的长度
    uint32 GetWriteLen() final;         //得到包写入的长度
    uint32 GetHeadLen() final;          //得到数据包头的长度
    uint32 GetPacketCount() final;      //得到缓存数据包的个数
    const char*  GetData() const final; //得到当前数据指针

    bool Init(int32 nSize = DEFINE_PACKET_SIZE, int32 nMaxBuffSize = MAX_PACKET_SIZE);
    bool Close() final;              //删除已经使用的内存
    bool Clear() final;              //清除所有的标志位，并不删除内存。

    bool WriteStream(const char* pData, uint32 u4Len) final;
    bool ReadStream(char* pData, uint32& u4Len) final;
    char* ReadPtr() final;                            //获得读指针
    char* WritePtr() final;                           //获得写指针
    void SetReadPtr(uint32 u4Pos) final;              //设置读指针的位置
    void SetPacketCount(uint32 u4PacketCount) final;  //设置缓存数据包的个数
    bool RollBack(uint32 u4Len) final;                //将取出的数据删除，将后面的数据加上
    void ReadBuffPtr(uint32 u4Size) final;
    void WriteBuffPtr(uint32 u4Size) final;

    bool AddBuff(uint32 u4Size) final;                //增加Packet的包大小


    void SetNetSort(bool blState) final;              //设置字节序开启开关，false为主机字序，true为网络字序

    void SetBuffID(uint32 u4BuffID);            //设置BuffID
    uint32 GetBuffID() const;                   //得到BuffID

    int32 GetHashID() const;                    //设置HashID
    void SetHashID(int32 nHashID);              //得到HashID

    const char* GetError() const;               //返回错误信息

private:
    void ReadPtr(uint32 u4Size);
    void WritePtr(uint32 u4Size);


public:
    //读取
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

    //写入
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
    char*                      m_szData          = nullptr;                //数据指针
    uint32                     m_u4ReadPtr       = 0;                   //读包的位置
    uint32                     m_u4WritePtr      = 0;                   //写包的位置
    uint32                     m_u4PacketLen     = 0 ;                  //包总长度
    uint32                     m_u4PacketCount   = 0;                   //当前数据包的个数
    uint32                     m_u4MaxPacketSize = 0;                   //最大当前BuffPacket的容量
    uint32                     m_u4BuffID        = 0;                   //Buffer对象名字，用于核对内存池数据使用，寻找内存泄露。
    int32                      m_nHashID         = 0;                   //记录当前对象在Hash数组中的位置
    bool                       m_blNetSort       = false;               //字节序开启开关，false为不转换为主机字节序，true为转换为主机字节序
    string                     m_strError;                              //错误信息
};
#endif
