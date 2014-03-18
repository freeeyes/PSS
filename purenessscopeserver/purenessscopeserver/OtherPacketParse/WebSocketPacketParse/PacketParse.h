#ifndef _PACKETPARSE_H
#define _PACKETPARSE_H

//这个类是实现一整套websocket的数据解析以及映射
//首先这里先处理一个握手包，然后在处理数据连接信息
//这里感谢?..!的支持，他提交了PSS对websocket支持的第一版，并提交给我
//我在这个版本上，进行了一些略微的整理
//add by freeeyes

#include "PacketParseBase.h"
#include "WebSocketInfo.h"

#ifdef WIN32
#if defined PACKETPARSE_BUILD_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif
#else
#define DLL_EXPORT
#endif 

//最大解密包的长度，如果长度比这个大，请修改这里
#define MAX_DECRYPTLENGTH 5*MAX_BUFF_1024

#ifdef WIN32
class DLL_EXPORT CPacketParse : public CPacketParseBase
#else
class CPacketParse : public CPacketParseBase
#endif 
{
public:
	CPacketParse(void);
	virtual ~CPacketParse(void);

	//得到符合条件的数据包头数据块，u4ConnectID是连接ID，pmbHead是数据块，pMessageBlockManager是数据块池，如果不用解密这个参数对你无效
	bool SetPacketHead(uint32 u4ConnectID, ACE_Message_Block* pmbHead, IMessageBlockManager* pMessageBlockManager);
	//得到符合条件的数据包体数据块，u4ConnectID是连接ID，pmbHead是数据块，pMessageBlockManager是数据块池，如果不用解密这个参数对你无效
	bool SetPacketBody(uint32 u4ConnectID, ACE_Message_Block* pmbBody, IMessageBlockManager* pMessageBlockManager);

	//专门对应 Mode为0的不带包头的数据包,如果是带包头的模式，这里什么都不用做。
	//因为用到了内存池，所以pHead和pBody由框架提供，并且由框架回收，所以在这里，不可以用new出来的pHead和pBody，否则会造成内存泄露。
	//这里要注意一下啊。当然，如果你觉得前面的接口繁琐，你也可以用这个接口实现你的规则，前提是你的m_u1PacketMode必须是PACKET_WITHSTREAM
	uint8 GetPacketStream(uint32 u4ConnectID, ACE_Message_Block* pCurrMessage, IMessageBlockManager* pMessageBlockManager);   

	//拼接数据返回包，所有的返回数据包都会调用这个
	bool MakePacket(uint32 u4ConnectID, const char* pData, uint32 u4Len, ACE_Message_Block* pMbData, uint16 u2CommandID = 0);
	//得到返回数据包的长度
	uint32 MakePacketLength(uint32 u4ConnectID, uint32 u4DataLen, uint16 u2CommandID = 0);
	//当连接第一次建立的时候，返回的接口用于你自己的处理。
	bool Connect(uint32 u4ConnectID, _ClientIPInfo& objClientIPInfo);
	//当连接断开的时候，返回你自己的处理
	void DisConnect(uint32 u4ConnectID);

private:
	//以下函数WebSocket PacketParse独有
	//处理握手包
	uint8 WebSocketDisposeHandIn(_WebSocketInfo* pWebSocketInfo, ACE_Message_Block* pCurrMessage, IMessageBlockManager* pMessageBlockManager);
	//处理数据包
	uint8 WebSocketDisposeDataIn(_WebSocketInfo* pWebSocketInfo, ACE_Message_Block* pCurrMessage, IMessageBlockManager* pMessageBlockManager);
	//获得当前包长以及包ID
	uint8 ReadDataPacketInfo(const char* pData, uint32 u4DataLen, uint16& u2CommandID, uint32& u4PacketLen);
	//解密数据包
	uint8 Decrypt(char* pOriData, uint32 u4Len, char* pEncryData, uint32& u4EncryLen);
};

#endif
