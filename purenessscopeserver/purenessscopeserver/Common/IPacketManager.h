#ifndef _IPACKETMANAGER_H
#define _IPACKETMANAGER_H

#include "IBuffPacket.h"

//对象创建信息
struct _Packet_Create_Info
{
    char   m_szCreateFileName[MAX_BUFF_100];   //声明文件
    uint32 m_u4Line;                           //声明行数位置
    uint32 m_u4Count;                          //对象个数

    _Packet_Create_Info()
    {
        m_szCreateFileName[0] = '\0';
        m_u4Line              = 0;
        m_u4Count             = 0;
    }
};

class IPacketManager
{
public:
    virtual ~IPacketManager() {}

    virtual uint32 GetBuffPacketUsedCount() = 0;
    virtual uint32 GetBuffPacketFreeCount() = 0;

    virtual IBuffPacket* Create(const char* pFileName = __FILE__, uint32 u4Line = __LINE__) = 0;
    virtual bool Delete(IBuffPacket* pBuffPacket)     = 0;
    virtual void GetCreateInfoList(vector<_Packet_Create_Info>& objCreateList) = 0;
};

#endif
