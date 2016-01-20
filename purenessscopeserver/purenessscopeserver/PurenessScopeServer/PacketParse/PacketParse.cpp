#include "PacketParse.h"


CPacketParse::CPacketParse(void)
{
	//如果是包头模式，这里需要设置包头的长度
	m_u4HeadSrcSize     = PACKET_HEAD_LENGTH;

	//这里修改属于你的包解析版本号
	sprintf_safe(m_szPacketVersion, MAX_BUFF_20, "0.96");

	//这里设置你的包模式
	m_u1PacketMode      = PACKET_WITHHEAD;
}

CPacketParse::~CPacketParse(void)
{

}

void CPacketParse::Init()
{
	m_u4PacketHead      = PACKET_HEAD_LENGTH;
	m_u2PacketCommandID = 0;
	m_u4PacketBody      = 0;
	m_u4HeadSrcSize     = PACKET_HEAD_LENGTH;
	m_u4BodySrcSize     = 0;

	m_blIsHandleHead    = true;

	m_pmbHead           = NULL;
	m_pmbBody           = NULL;
}

