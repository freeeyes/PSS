#include "PacketParse.h"


CPacketParse::CPacketParse(void)
{
	//如果是包头模式，这里需要设置包头的长度
	m_u4PacketHead      = PACKET_HEAD;

	//这里修改属于你的包解析版本号
	sprintf_safe(m_szPacketVersion, MAX_BUFF_20, "0.90");

	//这里设置你的包模式
	m_u1PacketMode      = PACKET_WITHSTREAM;
}

CPacketParse::~CPacketParse(void)
{
}

void CPacketParse::Init()
{
	m_u4PacketHead      = PACKET_HEAD;
	m_u2PacketCommandID = 0;
	m_u4PacketData      = 0;
	m_u4HeadSrcSize     = 0;
	m_u4BodySrcSize     = 0;

	m_blIsHandleHead    = true;

	m_pmbHead           = NULL;
	m_pmbBody           = NULL;
}


bool CPacketParse::SetPacketHead(uint32 u4ConnectID, ACE_Message_Block* pmbHead, IMessageBlockManager* pMessageBlockManager)
{
	//这里添加自己对包头的分析，主要分析出包长度。
	char* pData  = (char* )pmbHead->rd_ptr();
	uint32 u4Len = pmbHead->length();

	m_u4HeadSrcSize = u4Len;
	if(u4Len == sizeof(uint32))
	{
		ACE_OS::memcpy(&m_u4PacketData, pData, sizeof(uint32));
		
		m_pmbHead = pmbHead;
		m_blIsHandleHead    = false;
		return true;
	}
	else
	{
		return false;
	}
}

bool CPacketParse::SetPacketBody(uint32 u4ConnectID, ACE_Message_Block* pmbBody, IMessageBlockManager* pMessageBlockManager)
{
	//这里分析出包体内的一些数据，如果包头包含了CommandID，那么包体就不必做解析。
	char* pData  = (char* )pmbBody->rd_ptr();
	uint32 u4Len = pmbBody->length();

	m_u4BodySrcSize = u4Len;
	if(u4Len >= sizeof(uint16))
	{
		ACE_OS::memcpy(&m_u2PacketCommandID, pData, sizeof(uint16));
		m_blIsHandleHead    = true;
		m_pmbBody = pmbBody;
		return true;
	}
	else
	{
		m_blIsHandleHead    = true;
		return false;
	}
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
	//专门处理为数据流的数据包
	if(NULL == pCurrMessage || NULL == pMessageBlockManager)
	{
		return PACKET_GET_ERROR;
	}

	_HttpInfo* pHttpInfo = App_HttpInfoManager::instance()->GetWebSocketInfo(u4ConnectID);
	if(NULL == pHttpInfo)
	{
		//没有找到连接相关信息，说明有错误，断开连接
		return PACKET_GET_ERROR;
	}

	//判断http数据头是否完整
	return HttpDispose(pHttpInfo, pCurrMessage, pMessageBlockManager);

}

bool CPacketParse::Connect(uint32 u4ConnectID, _ClientIPInfo objClientIPInfo, _ClientIPInfo objLocalIPInfo)
{
	//这里添加你对连接建立的逻辑处理，如果没有则不用在这里写任何代码
	//返回false，则连接会断开

	//查找当前连接记录里面有无当前记录
	return App_HttpInfoManager::instance()->Insert(u4ConnectID);
}

void CPacketParse::DisConnect(uint32 u4ConnectID)
{
	//这里添加你对连接断开的逻辑处理
	App_HttpInfoManager::instance()->Delete(u4ConnectID);
}

uint8 CPacketParse::HttpDispose(_HttpInfo* pHttpInfo, ACE_Message_Block* pCurrMessage, IMessageBlockManager* pMessageBlockManager)
{
	char* pData   = pCurrMessage->rd_ptr();     //得到这个数据块的首字节
	uint32 u4Data = pCurrMessage->length();     //得到这个数据块的长度

	//将收到的数据粘入缓冲等待做切包处理
	if(pHttpInfo->m_u4DataLength + u4Data > MAX_DECRYPTLENGTH)
	{
		OUR_DEBUG((LM_ERROR, "[CPacketParse::HttpDispose]pWebSocketInfo is full.\n"));
		return PACKET_GET_ERROR;
	}

	//OUR_DEBUG((LM_ERROR, "[CPacketParse::HttpDispose]sizeof()=%d.\n", sizeof(pHttpInfo->m_szData)));
	//OUR_DEBUG((LM_ERROR, "[CPacketParse::HttpDispose]m_u4DataLength=%d.\n", pHttpInfo->m_u4DataLength));
	//OUR_DEBUG((LM_ERROR, "[CPacketParse::HttpDispose]u4Data=%d.\n", u4Data));
	
	ACE_OS::memcpy(&pHttpInfo->m_szData[pHttpInfo->m_u4DataLength], pData, u4Data);
	pHttpInfo->m_u4DataLength += u4Data;
	pHttpInfo->m_szData[pHttpInfo->m_u4DataLength] = '\0';

	//寻找Http包头
	char* pHttpHead = ACE_OS::strstr(pHttpInfo->m_szData, HTTP_HEAD_END);
	if(NULL == pHttpHead)
	{
		return PACKET_GET_NO_ENOUGTH;
	}

	uint32 u4HttpHeadLen = pHttpHead - pHttpInfo->m_szData - 4;
	//OUR_DEBUG((LM_ERROR, "[CPacketParse::HttpDispose]u4HttpHeadLen=%d.\n", u4HttpHeadLen));

	//找到了完整的包头
	m_pmbHead = pMessageBlockManager->Create(u4HttpHeadLen);
	if(NULL == m_pmbHead)
	{
		OUR_DEBUG((LM_ERROR, "[CPacketParse::HttpDispose]m_pmbHead is NULL.\n"));
		return PACKET_GET_ERROR;
	}

	memcpy(m_pmbHead->wr_ptr(), (char*)pHttpInfo->m_szData, u4HttpHeadLen);
	m_pmbHead->wr_ptr(u4HttpHeadLen);

	//设置命令字
	m_u2PacketCommandID = 0xea01;

	//查看有没有包体
	uint32 u4HttpBodyLength = 0;
	uint8 u1Ret = GetHttpBodyLen(pHttpInfo->m_szData, pHttpInfo->m_u4DataLength, u4HttpHeadLen, u4HttpBodyLength);
	if(u1Ret != PACKET_GET_ENOUGTH)
	{
		return u1Ret;
	}

	//OUR_DEBUG((LM_ERROR, "[CPacketParse::HttpDispose]u4HttpBodyLength=%d.\n", u4HttpBodyLength));
	if(u4HttpBodyLength == 0)
	{
		//获得包体
		m_pmbBody = pMessageBlockManager->Create(sizeof(uint32));
		if(NULL == m_pmbBody)
		{
			OUR_DEBUG((LM_ERROR, "[CPacketParse::WebSocketDisposeHandIn]m_pmbBody is NULL.\n"));
			return PACKET_GET_ERROR;
		}

		memcpy(m_pmbBody->wr_ptr(), (char*)&u4HttpHeadLen, sizeof(uint32));
		m_pmbBody->wr_ptr(sizeof(uint32));
	}
	else
	{
		//有包体，创建包体
		m_pmbBody = pMessageBlockManager->Create(u4HttpBodyLength);
		if(NULL == m_pmbBody)
		{
			OUR_DEBUG((LM_ERROR, "[CPacketParse::HttpDispose]m_pmbBody is NULL.\n"));
			return PACKET_GET_ERROR;
		}

		memcpy(m_pmbBody->wr_ptr(), (char*)pHttpHead, u4HttpBodyLength);
		m_pmbBody->wr_ptr(u4HttpBodyLength);
	}

	//处理完的数据从池中移除
	pCurrMessage->rd_ptr(u4Data);
	
	/*
	OUR_DEBUG((LM_ERROR, "[CPacketParse::HttpDispose]1.\n"));
	
	//测试代码
	m_pmbHead = pMessageBlockManager->Create(1);
	if(NULL == m_pmbHead)
	{
		OUR_DEBUG((LM_ERROR, "[CPacketParse::HttpDispose]m_pmbHead is NULL.\n"));
		return PACKET_GET_ERROR;
	}
	m_u4PacketHead = 1;
	
	char cData = '1';
	memcpy(m_pmbHead->wr_ptr(), (char*)&cData, 1);
	m_pmbHead->wr_ptr(1);	
	
	m_pmbBody = pMessageBlockManager->Create(1);
	if(NULL == m_pmbBody)
	{
		OUR_DEBUG((LM_ERROR, "[CPacketParse::HttpDispose]m_pmbHead is NULL.\n"));
		return PACKET_GET_ERROR;
	}
	m_u4PacketData = 1;
	
	OUR_DEBUG((LM_ERROR, "[CPacketParse::HttpDispose]2.\n"));
	
	memcpy(m_pmbBody->wr_ptr(), (char*)&cData, 1);
	m_pmbBody->wr_ptr(1);	
	
	OUR_DEBUG((LM_ERROR, "[CPacketParse::HttpDispose]u4Data=%d.\n", u4Data));
	pCurrMessage->rd_ptr(u4Data);
	OUR_DEBUG((LM_ERROR, "[CPacketParse::HttpDispose]Length=%d.\n", pCurrMessage->length()));
	*/
	
	pHttpInfo->m_u4DataLength = 0;
	return (uint8)PACKET_GET_ENOUGTH;
}

uint8 CPacketParse::GetHttpBodyLen(char* pData, uint32 u4Len, uint32 u4HeadLen, uint32& u4BodyLen)
{
	char szBodyLen[10] = {'\0'};
	int nNameLen = ACE_OS::strlen(HTTP_BODY_LENGTH);
		
	//OUR_DEBUG((LM_ERROR, "[CPacketParse::GetHttpBodyLen]nNameLen=%d.\n", nNameLen));
	//OUR_DEBUG((LM_ERROR, "[CPacketParse::GetHttpBodyLen]pData=%s.\n", pData));

	//解析出整个Http包长
	char* pLength = ACE_OS::strstr(pData, HTTP_BODY_LENGTH);
	if(NULL != pLength)
	{
		uint8 u1LengthLen = 0;
		//包含了包长字段,解析出包长字段
		for(int i = nNameLen; i < nNameLen + 9; i++)
		{
			if(pLength[i] == '\r')
			{
				break;
			}
			u1LengthLen++;
		}

		//OUR_DEBUG((LM_ERROR, "[CPacketParse::GetHttpBodyLen]u1LengthLen=%d.\n", u1LengthLen));
		ACE_OS::memcpy(szBodyLen, &pLength[nNameLen], u1LengthLen);

		u4BodyLen = ACE_OS::atoi(szBodyLen);
		//OUR_DEBUG((LM_ERROR, "[CPacketParse::GetHttpBodyLen]u4BodyLen=%d.\n", u4BodyLen));
		if(u4BodyLen == 0)
		{
			return PACKET_GET_ERROR;
		}

		//如果接受的字节比http里面的数据长度短，说明没接受完
		if(u4BodyLen > u4Len - u4HeadLen)
		{
			return PACKET_GET_NO_ENOUGTH;
		}
	}
	else
	{
		//找不到包长，就把所有数据算作包长
		u4BodyLen = u4Len - u4HeadLen;
	}

	return PACKET_GET_ENOUGTH;
}
