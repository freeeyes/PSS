#ifndef _PACKETPARSEBASE_H
#define _PACKETPARSEBASE_H

//这个类是把不经常变化的函数，放在这里，只让开发者去实现必要的5个接口
//剩下的，开发者不需要关系，这是框架自己实现的事情。
//所以剥离出来了这个类，想让他更简洁。
//坚持每一点滴的进步，持续的走下去，这就是梦想。
//add by freeeyes

#include "BuffPacket.h"
#include "IMessageBlockManager.h"

class CPacketParseBase
{
public:
	CPacketParseBase(void);

	virtual ~CPacketParseBase(void);

	void Clear();

	void Close();

	const char* GetPacketVersion();
	uint8 GetPacketMode();
	uint32 GetPacketHeadLen();
	uint32 GetPacketBodyLen();

	uint16 GetPacketCommandID();
	bool GetIsHandleHead();
	uint32 GetPacketHeadSrcLen();
	uint32 GetPacketBodySrcLen();

	void SetSort(uint8 u1Sort);

	void Check_Recv_Unit16(uint16& u2Data);
	void Check_Recv_Unit32(uint32& u4Data);
	void Check_Recv_Unit64(uint64& u2Data);

	void Check_Send_Unit16(uint16& u2Data);
	void Check_Send_Unit32(uint32& u4Data);
	void Check_Send_Unit64(uint64& u8Data);

	ACE_Message_Block* GetMessageHead();
	ACE_Message_Block* GetMessageBody();

	virtual void Init() = 0;
	IPacketHeadInfo* GetPacketHeadInfo();
	void SetPacketHeadInfo(IPacketHeadInfo* pPacketHeadInfo);

public:
	uint32 m_u4PacketHead;               //包头的长度
	uint32 m_u4PacketBody;               //包体的长度
	uint32 m_u4HeadSrcSize;              //包头的原始长度 
	uint32 m_u4BodySrcSize;              //包体的原始长度
	uint16 m_u2PacketCommandID;          //包命令
	bool   m_blIsHandleHead;
	char   m_szPacketVersion[MAX_BUFF_20];   //包解析器版本
	uint8  m_u1PacketMode;                   //包解析模式 
	uint8  m_u1Sort;                         //字节序规则，0为主机字节序，1为网络字节序   

	IPacketHeadInfo*    m_pPacketHeadInfo;  //数据包头信息 

	ACE_Message_Block* m_pmbHead;   //包头部分
	ACE_Message_Block* m_pmbBody;   //包体部分
};

#endif
