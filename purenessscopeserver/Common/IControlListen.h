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
    virtual bool   AddListen(const char* pListenIP, uint16 u2Port, uint8 u1IPType, int nPacketParseID) = 0;  //��һ���µļ����˿�
    virtual bool   DelListen(const char* pListenIP, uint16 u2Port)                                     = 0;  //�ر�һ����֪������
    virtual uint32 GetListenCount()                                                                    = 0;  //�õ���ǰ�Ѵ򿪵ļ�������
    virtual bool   ShowListen(uint32 u4Index, _ControlInfo& objControlInfo)                                = 0;  //������������Ѵ򿪵ļ����˿���Ϣ

    virtual uint32 GetServerID()                                                                       = 0;  //�õ���ǰ��������ID
};

#endif
