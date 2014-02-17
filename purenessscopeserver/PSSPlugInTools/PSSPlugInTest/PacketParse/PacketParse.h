#ifndef _PACKETPARSE_H
#define _PACKETPARSE_H

#include "PacketParseBase.h"

#ifdef WIN32
#if defined PACKETPARSE_BUILD_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif
#else
#define DLL_EXPORT
#endif 

#ifdef WIN32
class DLL_EXPORT CPacketParse : public CPacketParseBase
#else
class CPacketParse : public CPacketParseBase
#endif 
{
public:
	CPacketParse(void);
	virtual ~CPacketParse(void);

	bool SetPacketHead(uint32 u4ConnectID, ACE_Message_Block* pmbHead, IMessageBlockManager* pMessageBlockManager);
	bool SetPacketBody(uint32 u4ConnectID, ACE_Message_Block* pmbBody, IMessageBlockManager* pMessageBlockManager);

	//专门对应 Mode为0的不带包头的数据包,如果是带包头的模式，这里什么都不用做。
	//因为用到了内存池，所以pHead和pBody由框架提供，并且由框架回收，所以在这里，不可以用new出来的pHead和pBody，否则会造成内存泄露。
	//这里要注意一下啊。当然，如果你觉得前面的接口繁琐，你也可以用这个接口实现你的规则，前提是你的m_u1PacketMode必须是PACKET_WITHSTREAM
	uint8 GetPacketStream(uint32 u4ConnectID, ACE_Message_Block* pCurrMessage, IMessageBlockManager* pMessageBlockManager);   

	//拼接数据返回包
	bool MakePacket(uint32 u4ConnectID, const char* pData, uint32 u4Len, ACE_Message_Block* pMbData, uint16 u2CommandID = 0);
	uint32 MakePacketLength(uint32 u4ConnectID, uint32 u4DataLen, uint16 u2CommandID = 0);

	//当连接第一次建立的时候，返回的接口用于你自己的处理。
	bool Connect(uint32 u4ConnectID, _ClientIPInfo& objClientIPInfo);
	//当连接断开的时候，返回你自己的处理
	void DisConnect(uint32 u4ConnectID);

};
#endif

