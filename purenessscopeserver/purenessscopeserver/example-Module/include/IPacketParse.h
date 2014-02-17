#ifndef _PACKETPARSE_H
#define _PACKETPARSE_H

// IPacketParse.h : 定义协议，由用户继承这个类，并实现对应接口，剥离协议解析和框架的依赖。
// add by freeeyes
// 2010-05-26

#include "define.h"

class IPacketParse
{
public:
	virtual ~IPacketParse() {};

	virtual uint32 GetPacketHeadLen()    = 0;
	virtual uint32 GetPacketDataLen()    = 0;
	virtual uint16 GetPacketCommandID()  = 0;

	virtual bool GetIsHead()             = 0;

	virtual bool SetPacketHead(char* pData, uint32 u4Len) = 0;
	virtual bool SetPacketData(char* pData, uint32 u4Len) = 0;

	virtual bool MakePacket(const char* pData, uint32 u4Len, ACE_Message_Block* pMbData) = 0;
	virtual uint32 MakePacketLength(uint32 u4DataLen) = 0;
};
#endif
