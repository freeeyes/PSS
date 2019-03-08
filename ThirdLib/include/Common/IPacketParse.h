#ifndef _PACKETPARSE_H
#define _PACKETPARSE_H

// IPacketParse.h : ����Э�飬���û��̳�����࣬��ʵ�ֶ�Ӧ�ӿڣ�����Э������Ϳ�ܵ�������
// add by freeeyes
// 2010-05-26

#include "define.h"

class IPacketParse
{
public:
    virtual ~IPacketParse() {}

    virtual uint32 GetPacketHeadLen()    = 0;
    virtual uint32 GetPacketBodyLen()    = 0;
    virtual uint16 GetPacketCommandID()  = 0;

    virtual bool GetIsHandleHead()       = 0;

    virtual bool SetPacketHead(char* pData, uint32 u4Len) = 0;
    virtual bool SetPacketBody(char* pData, uint32 u4Len) = 0;

    virtual bool MakePacket(const char* pData, uint32 u4Len, ACE_Message_Block* pMbData) = 0;
    virtual uint32 MakePacketLength(uint32 u4DataLen) = 0;
};
#endif
