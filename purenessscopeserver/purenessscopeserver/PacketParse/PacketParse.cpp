#include "PacketParse.h"


CPacketParse::CPacketParse(void)
{
	//如果是包头模式，这里需要设置包头的长度
	m_u4PacketHead      = PACKET_HEAD_LENGTH;

	//这里修改属于你的包解析版本号
	sprintf_safe(m_szPacketVersion, MAX_BUFF_20, "0.94");

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
	m_u4PacketData      = 0;
	m_u4HeadSrcSize     = 0;
	m_u4BodySrcSize     = 0;

	m_blIsHead          = false;

	m_pmbHead           = NULL;
	m_pmbBody           = NULL;
}

bool CPacketParse::SetPacketHead(uint32 u4ConnectID, ACE_Message_Block* pmbHead, IMessageBlockManager* pMessageBlockManager)
{
	//这里添加自己对包头的分析，主要分析出包长度。
	//获得包头30个字节的相关信息，还原成数据包信息结构
	char* pData  = (char* )pmbHead->rd_ptr();
	uint32 u4Len = pmbHead->length();
	uint32 u4Pos = 0;

	ACE_OS::memcpy((char* )&m_objPacketHeadInfo.m_u2Version, (char* )&pData[u4Pos], sizeof(uint16));
	u4Pos += sizeof(uint16);
	ACE_OS::memcpy((char* )&m_objPacketHeadInfo.m_u2CmdID, (char* )&pData[u4Pos], sizeof(uint16));
	u4Pos += sizeof(uint16);
	ACE_OS::memcpy((char* )&m_objPacketHeadInfo.m_u4BodyLen, (char* )&pData[u4Pos], sizeof(uint32));
	u4Pos += sizeof(uint32);
	ACE_OS::memcpy(m_objPacketHeadInfo.m_szSession, (char* )&pData[u4Pos], sizeof(char)*32);
	u4Pos += sizeof(char)*32;

	m_u4PacketData      = m_objPacketHeadInfo.m_u4BodyLen;
	m_u2PacketCommandID = m_objPacketHeadInfo.m_u2CmdID;

	m_pmbHead = pmbHead;

	return true;
}

bool CPacketParse::SetPacketBody(uint32 u4ConnectID, ACE_Message_Block* pmbBody, IMessageBlockManager* pMessageBlockManager)
{
	//这里分析出包体内的一些数据，如果包头包含了CommandID，那么包体就不必做解析。
	m_pmbBody = pmbBody;
	return true;

}



uint32 CPacketParse::MakePacketLength(uint32 u4ConnectID, uint32 u4DataLen, uint16 u2CommandID)
{
	if(u2CommandID == 0)
	{
		return 0;
	}
	else
	{
		return u4DataLen + sizeof(uint32);
	}
}

bool CPacketParse::MakePacket(uint32 u4ConnectID, const char* pData, uint32 u4Len, ACE_Message_Block* pMbData, uint16 u2CommandID)
{
	if(pMbData == NULL && u2CommandID == 0)
	{
		return false;
	}

	//拼装数据包
	ACE_OS::memcpy(pMbData->wr_ptr(), (const void*)&u4Len, sizeof(uint32));
	ACE_OS::memcpy(pMbData->wr_ptr() + sizeof(uint32), (const void*)pData, u4Len);
	pMbData->wr_ptr(u4Len + sizeof(uint32));

	return true;
}



uint8 CPacketParse::GetPacketStream(uint32 u4ConnectID, ACE_Message_Block* pCurrMessage, IMessageBlockManager* pMessageBlockManager)
{
	//这里是测试代码，专门处理为数据流的数据包
	if(NULL == pCurrMessage || NULL == pMessageBlockManager)
	{
		return PACKET_GET_ERROR;
	}

	CBuffPacket* pBuffPacket = App_PacketBufferManager::instance()->GetBuffPacket(u4ConnectID);
	if(NULL == pBuffPacket)
	{
		return PACKET_GET_ERROR;
	}

	//这里添加了我的规则，比如，我的数据包第一个字节是7E开头，以7E为结尾，那么我会这么做
	//首先，判断数据里面第一个字节是不是7E，如果不是，则返回错误的数据。
	//如果是错误数据，为了保证数据安全，框架会关闭这个链接。
	char* pData   = pCurrMessage->rd_ptr();     //得到这个数据块的首字节
	uint32 u4Data = pCurrMessage->length();     //得到这个数据块的长度

	//判断数据长度是否为0
	if(u4Data <= 0)
	{
		return PACKET_GET_ERROR;
	}

	//如果收首字节不是7E，则证明数据要不不全需要粘包，要不就是错数据包。
	if(pData[0] == 0x7E)
	{
		//包头正确，开始寻找包尾
		uint32 u4Pos  = 1;
		bool   blFind = false;
		for(u4Pos = 1; u4Pos < u4Data; u4Pos++)
		{
			//找到了包末尾
			if(pData[u4Pos] == 0x7E)
			{
				blFind = true;
				break;
			}
		}

		if(blFind == false)
		{
			if(pCurrMessage->length() > 2)
			{
				//将不完整的数据放入缓冲，等待完整后放入包体
				pBuffPacket->WriteStream(pCurrMessage->rd_ptr(), pCurrMessage->length());
			}

			m_blIsHead = true;

			//没有找到包尾，需要继续接受数据
			return PACKET_GET_NO_ENOUGTH;
		}
		else
		{
			//接收到了完整数据包
			uint32 u4PacketLen = u4Pos;

			m_u4PacketHead  = sizeof(uint32);
			m_u4HeadSrcSize = sizeof(uint32);
			m_u4PacketData  = u4PacketLen;
			m_u4BodySrcSize = u4PacketLen;

			//从内存池中申请一个包头
			m_pmbHead = pMessageBlockManager->Create(sizeof(uint32));
			if(NULL == m_pmbHead)
			{
				return PACKET_GET_ERROR;
			}

			//记录包长，并转换为网络字节序，放入包头
			//uint32 u4NetPacketLen = ACE_HTONL(u4PacketLen);
			uint32 u4NetPacketLen = u4PacketLen;
			memcpy(m_pmbHead->wr_ptr(), (char*)&u4NetPacketLen, sizeof(uint32));
			m_pmbHead->wr_ptr(sizeof(uint32));
			
			//从内存池申请一个包体
			m_pmbBody = pMessageBlockManager->Create(u4PacketLen);
			if(NULL == m_pmbBody)
			{
				return PACKET_GET_ERROR;
			}

			//获得包命令ID
			ACE_OS::memcpy(&m_u2PacketCommandID, (char*)&pData[1], sizeof(uint16));

			//将包内容放入包体
			memcpy(m_pmbBody->wr_ptr(), (char*)&pData[1], u4PacketLen);
			m_pmbBody->wr_ptr(u4PacketLen);

			m_blIsHead = false;

			//处理完的数据从池中移除
			pCurrMessage->rd_ptr(u4Pos);
			return PACKET_GET_ENOUGTH;
		}
	}
	else
	{
		//如果缓冲不存在，则说明缓冲中的包都解析完毕了
		if(m_blIsHead == false)
		{
			return PACKET_GET_ERROR;
		}

		pBuffPacket->WriteStream(pData, u4Data);

		uint32 u4Pos  = 0;
		bool   blFind = false;
		if(pBuffPacket->GetPacketLen() > 0)
		{
			//如果有缓冲，则看看是否有结束标记
			for(u4Pos = 1; u4Pos < u4Data; u4Pos++)
			{
				//找到了包末尾
				if(pData[u4Pos] == 0x7E)
				{
					blFind = true;
					break;
				}
			}
		}
		else
		{
			//没有缓冲，则从首字节判断
			for(u4Pos = 1; u4Pos < u4Data; u4Pos++)
			{
				//找到了包末尾
				if(pData[u4Pos] == 0x7E)
				{
					blFind = true;
					break;
				}
			}
		}

		if(blFind == false)
		{
			m_blIsHead = true;

			//没有找到包尾，需要继续接受数据
			return PACKET_GET_NO_ENOUGTH;
		}
		else
		{
			//接收到了完整数据包
			m_u4PacketHead  = sizeof(uint32);
			m_u4HeadSrcSize = sizeof(uint32);
			m_u4PacketData  = u4Pos + 1;
			m_u4BodySrcSize = u4Pos + 1;

			//从内存池中申请一个包头
			m_pmbHead = pMessageBlockManager->Create(sizeof(uint32));
			if(NULL == m_pmbHead)
			{
				return PACKET_GET_ERROR;
			}

			//记录包长，并转换为网络字节序，放入包头
			//uint32 u4NetPacketLen = ACE_HTONL(m_objCurrBody.GetPacketLen());
			uint32 u4NetPacketLen = m_u4PacketData;
			memcpy(m_pmbHead->wr_ptr(), (char*)&u4NetPacketLen, sizeof(uint32));
			m_pmbHead->wr_ptr(sizeof(uint32));

			//从内存池申请一个包体
			m_pmbBody = pMessageBlockManager->Create(m_u4PacketData);
			if(NULL == m_pmbBody)
			{
				return PACKET_GET_ERROR;
			}

			//获得包命令ID
			ACE_OS::memcpy(&m_u2PacketCommandID, pData, sizeof(uint16));


			//将包内容放入包体
			memcpy(m_pmbBody->wr_ptr(), (char*)pBuffPacket->GetData(), m_u4PacketData);
			m_pmbBody->wr_ptr(m_u4PacketData);

			//删除缓冲中的数据
			pBuffPacket->RollBack(u4Pos);

			m_blIsHead = false;

			//处理完的数据从池中移除
			pCurrMessage->rd_ptr(pCurrMessage->length());
			return PACKET_GET_ENOUGTH;
		}

	}
}

bool CPacketParse::Connect(uint32 u4ConnectID, _ClientIPInfo& objClientIPInfo)
{
	//这里添加你对连接建立的逻辑处理，如果没有则不用在这里写任何代码
	//返回false，则连接会断开
	App_PacketBufferManager::instance()->AddBuffer(u4ConnectID);

	return true;
}

void CPacketParse::DisConnect(uint32 u4ConnectID)
{
	//这里添加你对连接断开的逻辑处理
	App_PacketBufferManager::instance()->DelBuffer(u4ConnectID);
}

void CPacketParse::GetPacketHeadInfo(_PacketHeadInfo& objPacketHeadInfo)
{
	objPacketHeadInfo.m_u2Version = m_objPacketHeadInfo.m_u2Version;
	objPacketHeadInfo.m_u2CmdID   = m_objPacketHeadInfo.m_u2CmdID;
	objPacketHeadInfo.m_u4BodyLen = m_objPacketHeadInfo.m_u4BodyLen;
	sprintf_safe(objPacketHeadInfo.m_szSession, SESSION_LEN, "%s", m_objPacketHeadInfo.m_szSession);
}

