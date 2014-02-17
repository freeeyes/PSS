#ifndef _CLIENTPARSE_H
#define _CLIENTPARSE_H

//客户端数据包解析类
//中间服务器协议架构为 (<包长 unit32>):包头 (<命令ID uint16> + <包自增量ID uint32> + 数据体 XXX):包体
//add by freeeyes 2010-12-24
#include "define.h"

#define MAX_PACKET_HEAD sizeof(uint32)

class CClientParse
{
public:
	CClientParse(void);
	~CClientParse(void);

	void Init();

	uint32 GetPacketHeadLen();
	uint32 GetPacketDataLen();
	uint16 GetPacketCommandID();
	uint32 GetPacketSerialNo();

	bool GetIsHead();

	ACE_Message_Block* GetMessageHead();
	ACE_Message_Block* GetMessageBody();
	bool SetMessageHead(ACE_Message_Block* pmbHead);
	bool SetMessageBody(ACE_Message_Block* pmbBody);

	bool SetPacketHead(char* pData, uint32 u4Len);
	bool SetPacketData(char* pData, uint32 u4Len);

	//拼接数据返回包
	bool MakePacket(const char* pData, uint32 u4Len, ACE_Message_Block* pMbData);
	uint32 MakePacketLength(uint32 u4DataLen);

	void Close();

private:
	uint32 m_u4PacketHead;
	uint32 m_u4PacketData;
	uint16 m_u2PacketCommandID;
	bool   m_blIsHead;

	ACE_Message_Block* m_pmbHead;   //包头部分
	ACE_Message_Block* m_pmbBody;   //包体部分
};
#endif
