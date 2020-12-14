#ifndef MAKEPACKET_COMMON_H
#define MAKEPACKET_COMMON_H

#include "define.h"
#include "PacketParse.h"
#include "IHandler.h"

class _MakePacket
{
public:
	uint32                   m_u4ConnectID     = 0;                                   //链接ID
	uint32                   m_u4PacketParseID = 0;                                   //对应ConnectID的PacketParseID
	int32                    m_nHashID         = 0;                                   //对应记录hash的ID
	shared_ptr<CPacketParse> m_pPacketParse    = nullptr;                             //数据包数据指针
	EM_CONNECT_IO_TYPE       m_emPacketType    = EM_CONNECT_IO_TYPE::CONNECT_IO_TCP;  //数据包类型
	uint8                    m_u1Option        = 0;                                   //操作类型
	ACE_INET_Addr            m_AddrRemote;                                            //数据包的来源IP信息
	ACE_INET_Addr            m_AddrListen;                                            //数据包来源监听IP信息
	IHandler*                m_pHandler        = nullptr;                             //当前Handler句柄名称
	PSS_Time_Point           m_tvRecv;                                                //接收数据的时间 

	_MakePacket() = default;

	void Clear()
	{
		m_emPacketType    = EM_CONNECT_IO_TYPE::CONNECT_IO_TCP;  //0为TCP,1是UDP 默认是TCP
		m_u4ConnectID     = 0;
		m_u1Option        = 0;
		m_pPacketParse    = nullptr;
		m_u4PacketParseID = 0;
		m_pHandler        = nullptr;
	}

	void SetHashID(int32 nHashID)
	{
		m_nHashID = nHashID;
	}

	int32 GetHashID() const
	{
		return m_nHashID;
	}
};

#endif