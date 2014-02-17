#ifndef _SERVERPARSE_H
#define _SERVERPARSE_H

//处理中间服务器数据包解析的类
//用户可以在这里修改中间服务器处理规则
//如果一般不管的话，类的规则就是<包头长度>（4字节）+<包体>
//Create by freeeyes
//2010-12-21

#include "define.h"

class CServerParse
{
public:
	CServerParse(void);
	~CServerParse(void);

	void Init();

	uint32 GetPacketHeadLen();
	uint32 GetPacketBodyLen();
	uint16 GetPacketCommandID();

	bool GetIsHead();

	ACE_Message_Block* GetMessageHead();
	ACE_Message_Block* GetMessageBody();
	bool SetMessageHead(ACE_Message_Block* pmbHead);
	bool SetMessageBody(ACE_Message_Block* pmbBody);

	bool SetPacketHead(char* pData, uint32 u4Len);
	bool SetPacketBody(char* pData, uint32 u4Len);

	//拼接数据返回包
	bool MakePacket(const char* pData, uint32 u4Len, ACE_Message_Block* pMbData);
	uint32 MakePacketLength(uint32 u4DataLen);

	void Close();

private:
	uint32 m_u4PacketHead;
	uint32 m_u4PacketData;
	uint16 m_u4PacketCommandID;
	bool   m_blIsHead;

	ACE_Message_Block* m_pmbHead;   //包头部分
	ACE_Message_Block* m_pmbBody;   //包体部分
};

#endif
