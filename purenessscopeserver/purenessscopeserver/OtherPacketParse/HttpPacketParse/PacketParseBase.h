#ifndef _PACKETPARSEBASE_H
#define _PACKETPARSEBASE_H

//这个类是把不经常变化的函数，放在这里，只让开发者去实现必要的5个接口
//剩下的，开发者不需要关系，这是框架自己实现的事情。
//所以剥离出来了这个类，想让他更简洁。
//坚持每一点滴的进步，持续的走下去，这就是梦想。
//add by freeeyes

#include "BuffPacket.h"
#include "IMessageBlockManager.h"

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
class DLL_EXPORT CPacketParseBase
#else
class CPacketParseBase
#endif 
{
public:
	CPacketParseBase(void);

	virtual ~CPacketParseBase(void);

	void Init();

	void Clear();

	void Close();

	const char* GetPacketVersion();
	uint8 GetPacketMode();
	uint32 GetPacketHeadLen();
	uint32 GetPacketBodyLen();

	uint16 GetPacketCommandID();
	bool GetIsHead();
	uint32 GetPacketHeadSrcLen();
	uint32 GetPacketBodySrcLen();

	ACE_Message_Block* GetMessageHead();
	ACE_Message_Block* GetMessageBody();

	virtual bool SetPacketHead(uint32 u4ConnectID, ACE_Message_Block* pmbHead, IMessageBlockManager* pMessageBlockManager)           = 0;
	virtual bool SetPacketBody(uint32 u4ConnectID, ACE_Message_Block* pmbBody, IMessageBlockManager* pMessageBlockManager)           = 0;
	virtual uint8 GetPacketStream(uint32 u4ConnectID, ACE_Message_Block* pCurrMessage, IMessageBlockManager* pMessageBlockManager)   = 0;
	virtual bool MakePacket(uint32 u4ConnectID, const char* pData, uint32 u4Len, ACE_Message_Block* pMbData, uint16 u2CommandID = 0) = 0;
	virtual uint32 MakePacketLength(uint32 u4ConnectID, uint32 u4DataLen, uint16 u2CommandID = 0)                                    = 0;
	virtual bool Connect(uint32 u4ConnectID, _ClientIPInfo& objClientIPInfo)                                                         = 0;
	virtual void DisConnect(uint32 u4ConnectID)                                                                                      = 0;

protected:
	uint32 m_u4PacketHead;               //包头的长度
	uint32 m_u4PacketData;               //包体的长度
	uint32 m_u4HeadSrcSize;              //包头的原始长度 
	uint32 m_u4BodySrcSize;              //包体的原始长度
	uint16 m_u2PacketCommandID;          //包命令
	bool   m_blIsHead;
	char   m_szPacketVersion[MAX_BUFF_20];   //包解析器版本
	uint8  m_u1PacketMode;                   //包解析模式    

	ACE_Message_Block* m_pmbHead;   //包头部分
	ACE_Message_Block* m_pmbBody;   //包体部分

	CBuffPacket m_objCurrBody;      //记录尚未完整的包体
};

#endif
