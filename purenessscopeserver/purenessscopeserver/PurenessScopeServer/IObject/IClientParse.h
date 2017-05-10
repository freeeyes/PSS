#ifndef _ICLIENTPARSE_H
#define _ICLIENTPARSE_H

#include "define.h"

class IClientParse
{
public:
    virtual ~IClientParse() {}

    virtual void Init()                         = 0;
    virtual uint32 GetPacketHeadLen()           = 0;
    virtual uint32 GetPacketBodyLen()           = 0;
    virtual uint16 GetPacketCommandID()         = 0;
    virtual bool GetIsHandleHead()              = 0;

    virtual ACE_Message_Block* GetMessageHead()             = 0;
    virtual ACE_Message_Block* GetMessageBody()             = 0;
    virtual bool SetMessageHead(ACE_Message_Block* pmbHead) = 0;
    virtual bool SetMessageBody(ACE_Message_Block* pmbBody) = 0;

    virtual bool SetPacketHead(char* pData, uint32 u4Len)   = 0;
    virtual bool SetPacketBody(char* pData, uint32 u4Len)   = 0;

    //拼接数据返回包
    virtual bool MakePacket(const char* pData, uint32 u4Len, ACE_Message_Block* pMbData) = 0;
    virtual uint32 MakePacketLength(uint32 u4DataLen)                                    = 0;

    virtual void Close()                                                                 = 0;
};

#endif
