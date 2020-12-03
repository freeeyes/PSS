#ifndef _ICONTROLLISTEN_H
#define _ICONTROLLISTEN_H

#include "define.h"

struct _ControlInfo
{
    string m_strListenIP;
    uint32 m_u4Port = 0;

    _ControlInfo() = default;
};

typedef vector<_ControlInfo> vecControlInfo;

class IControlListen
{
public:
    virtual ~IControlListen() {}
    virtual bool   AddListen(const char* pListenIP, uint16 u2Port, uint8 u1IPType, int nPacketParseID) = 0;  //打开一个新的监听端口
    virtual bool   DelListen(const char* pListenIP, uint16 u2Port)                                     = 0;  //关闭一个已知的连接
    virtual uint32 GetListenCount()                                                                    = 0;  //得到当前已打开的监听个数
    virtual bool   ShowListen(uint32 u4Index, _ControlInfo& objControlInfo)                                = 0;  //根据索引获得已打开的监听端口信息

    virtual uint32 GetServerID()                                                                       = 0;  //得到当前服务器的ID
};

#endif
