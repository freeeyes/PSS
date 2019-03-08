#include "BaseCommand.h"
#include "base64.h"
#include "sha1.h"

CBaseCommand::CBaseCommand(void)
{
	m_pServerObject = NULL;
	m_nCount        = 0;
}

CBaseCommand::~CBaseCommand(void)
{
}

void CBaseCommand::SetServerObject(CServerObject* pServerObject)
{
	m_pServerObject = pServerObject;
}

int CBaseCommand::DoMessage(IMessage* pMessage, bool& bDeleteFlag)
{
	//__ENTER_FUNCTION���Զ�����ĺ���׷��try�ĺ꣬���ڲ�׽���������ʹ��__THROW_FUNCTION()�׳���Ĵ���
	//����__THROW_FUNCTION("hello"); �����Զ���Log�ļ����µ�assert.log��¼�ļ�������������������������
	//�Ƽ���ʹ������꣬��סһ��Ҫ�ں����Ľ���ʹ��__LEAVE_FUNCTION����__LEAVE_FUNCTION_WITHRETURN��֮��ԡ�
	//__LEAVE_FUNCTION_WITHRETURN��__LEAVE_FUNCTION��ͬ��ǰ�߿���֧�ֺ����˳���ʱ�򷵻�һ������
	//������������Ƿ���һ��int
	//��ôĩβ����__LEAVE_FUNCTION_WITHRETURN(0); 0���Ƿ��ص�int����Ȼ��Ҳ֧�ֱ�����ͱ��������忴���Լ��ĺ�����
	//��������������κα����������ʹ��__LEAVE_FUNCTION���ɡ�
	__ENTER_FUNCTION();

	if(m_pServerObject == NULL)
	{
		OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] m_pServerObject is NULL(%d).\n", bDeleteFlag));
		return -1;
	}

	if(pMessage == NULL)
	{
		OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pMessage is NULL.\n"));
		return -1;
	}

	//�������ӽ�����Ϣ
	if(pMessage->GetMessageBase()->m_u2Cmd == CLIENT_LINK_CONNECT)
	{
		OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] CLIENT_LINK_CONNECT OK.\n"));
		return 0;
	}

	//�������ӶϿ���Ϣ
	if(pMessage->GetMessageBase()->m_u2Cmd == CLIENT_LINK_CDISCONNET)
	{
		OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] CLIENT_LINK_CDISCONNET OK.\n"));
		return 0;
	}

	//����������Ϣ
	if(pMessage->GetMessageBase()->m_u2Cmd == COMMAND_HANDIN)
	{
		DoMessage_HandIn(pMessage, bDeleteFlag);
	}
	else if(pMessage->GetMessageBase()->m_u2Cmd == COMMAND_DATAIN)
	{
		DoMessage_DataIn(pMessage, bDeleteFlag);
	}

	return 0;

	__LEAVE_FUNCTION_WITHRETURN(0);
}

int CBaseCommand::DoMessage_HandIn(IMessage* pMessage, bool& bDeleteFlag)
{
	//������յ����������ݣ�������������
	char szReturnData[MAX_BUFF_500] = {'\0'};
	char szClientKey[MAX_BUFF_100]  = {'\0'};
	char szOrigenKey[MAX_BUFF_100]  = {'\0'};
	char szMagicKey[MAX_BUFF_100]   = {'\0'};

	//����ħ���ַ�
	sprintf_safe(szMagicKey, MAX_BUFF_100, "258EAFA5-E914-47DA-95CA-C5AB0DC85B11");

	_PacketInfo BodyPacket;
	pMessage->GetPacketBody(BodyPacket);

	//��������յ������ݰ�����������֤key
	//����Ѱ�����ְ��е�keyֵ
	char* pClientkeyEnd = NULL;

	char* pClientKeyBegin = ACE_OS::strstr(BodyPacket.m_pData, CLIENT_KEY_TAG);		
	if(NULL != pClientKeyBegin)
	{
		pClientkeyEnd = strstr(pClientKeyBegin, "\r\n");
	}
	else
	{
		OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage_HandIn] pClientKeyBegin = NULL(%d).\n", bDeleteFlag));
		return 0;
	}
	
	if(NULL == pClientkeyEnd)
	{
		OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage_HandIn] pClientkeyEnd = NULL.\n"));
		return 0;
	}

	//��ÿͻ���key
	uint32 u4TagLength = ACE_OS::strlen(CLIENT_KEY_TAG);
	uint32 u4ClientKeyLength = (uint32)(pClientkeyEnd - pClientKeyBegin) - u4TagLength;
	if(u4ClientKeyLength >= MAX_BUFF_100)
	{
		OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage_HandIn] key is too length(%d).\n", u4ClientKeyLength));
		return 0;
	}
	ACE_OS::memcpy(szOrigenKey, pClientKeyBegin + u4TagLength, u4ClientKeyLength);

	//ȥ�ո�
	TrimString(szOrigenKey, szClientKey);

	OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage_HandIn]ClientKey=%s.\n", szClientKey));

	//����ħ���ַ��ͻ��˵�key
	ACE_OS::strcat(szClientKey, szMagicKey);

	//����sha1�㷨
	SHA1   objSha;
	uint32 u4digest[5];

	objSha.Reset();
	objSha << szClientKey;

	objSha.Result(u4digest);

	//������ļ��ܴ�ת������
	for (int i = 0; i < 5; i++) 
	{
		u4digest[i] = htonl(u4digest[i]);
	}

	//�ٽ������ַ���ת��Base64����
	std::string strServerKey = "";
	strServerKey = base64_encode(
		reinterpret_cast<const unsigned char*>
		(u4digest), sizeof(u4digest)
		);

	//���÷����ַ���
	sprintf_safe(szReturnData, MAX_BUFF_200, HANDIN_RETURN_DATA, strServerKey.c_str());

	OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage_HandIn]ServerKey=%s.\n", strServerKey.c_str()));

	uint16 u2PostCommandID = COMMAND_RETURN_HANDIN;

	//������֤��
	if(NULL != m_pServerObject->GetConnectManager())
	{
		//����ȫ������
		 char* ptrReturnData = reinterpret_cast< char*>(szReturnData);
		m_pServerObject->GetConnectManager()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID, ptrReturnData, 
			(uint32)ACE_OS::strlen(szReturnData), SENDMESSAGE_JAMPNOMAL, u2PostCommandID, PACKET_SEND_IMMEDIATLY, PACKET_IS_SELF_RECYC);
	}
	else
	{
		OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage_HandIn] m_pConnectManager = NULL.\n"));
	}

	return 0;
}

int CBaseCommand::DoMessage_DataIn(IMessage* pMessage, bool& bDeleteFlag)
{
	_PacketInfo BodyPacket;
	pMessage->GetPacketBody(BodyPacket);

	char* pData = new char[BodyPacket.m_nDataLen + 1];
	pData[BodyPacket.m_nDataLen] = '\0';

	ACE_OS::memcpy(pData, BodyPacket.m_pData, BodyPacket.m_nDataLen);

	//��ӡ���պ��������
	OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage_DataIn](%d)%s\n", bDeleteFlag, pData));

	//����ԭ������
	char szReturnBuff[MAX_BUFF_1024] = {'\0'};
	uint32 u4ReturnLen = MAX_BUFF_1024;

	uint16 u2PostCommandID = COMMAND_RETURN_DATAIN;

	//ƴ�ӷ��ذ�
	char szTemp[MAX_BUFF_1024] = {'\0'};
	ACE_OS::memset(szTemp, 0, MAX_BUFF_1024);
	sprintf_safe(szTemp, MAX_BUFF_1024, "%04x,%04d", u2PostCommandID, BodyPacket.m_nDataLen + 9);
	ACE_OS::memcpy(&szTemp[ACE_OS::strlen(szTemp)], BodyPacket.m_pData, BodyPacket.m_nDataLen);

	WebSocketEncrypt(szTemp, (uint32)ACE_OS::strlen(szTemp), szReturnBuff, u4ReturnLen);

	SAFE_DELETE_ARRAY(pData);

	if(NULL != m_pServerObject->GetConnectManager())
	{
		//����ȫ������
		char* ptrReturnData = szReturnBuff;
		m_pServerObject->GetConnectManager()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID, ptrReturnData, 
			u4ReturnLen, SENDMESSAGE_JAMPNOMAL, u2PostCommandID, PACKET_SEND_IMMEDIATLY, PACKET_IS_SELF_RECYC);
	}
	else
	{
		OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage_HandIn] m_pConnectManager = NULL.\n"));
	}


	return 0;
}

bool CBaseCommand::WebSocketEncrypt(char* pOriData, uint32 u4OriLen, char* pEncryData, uint32& u4EnCryLen)
{				 
	uint64 length =  (uint64)u4OriLen;
	uint64 expectedSize = length + 1; //flags byte.
	if(length <= 125)
		expectedSize += 1;
	else if(length > 125  && length <= 65535)
		expectedSize += 3;
	else
		expectedSize += 9;

	int iPyl = 0;
	unsigned char payloadFlags = 129;
	ACE_OS::memcpy(pEncryData, &payloadFlags, 1);
	//length+=1;
	iPyl+=1;

	//create the length byte
	if (length <= 125)
	{				    
		char basicSize = u4OriLen;
		ACE_OS::memcpy(pEncryData+iPyl, &basicSize, 1);
		//length+=1;
		iPyl+=1;		
	}
	else if ((length > 125) && (length <= 65535))
	{
		char basicSize = 126;					
		ACE_OS::memcpy(pEncryData+iPyl, &basicSize, 1);
		//length+=1;
		iPyl+=1;


		char len[2];
		len[0] = ( length >> 8 ) & 255;
		len[1] = ( length ) & 255;
		ACE_OS::memcpy(pEncryData+iPyl,&len,2);
		//length+=2;
		iPyl+=2;		
	}
	else
	{
		char basicSize = 127;					
		ACE_OS::memcpy(pEncryData+iPyl,&basicSize,1);
		//length+=1;
		iPyl+=1;

		char len[8];
		len[0] = ( length >> 56 ) & 255;
		len[1] = ( length >> 48  ) & 255;
		len[2] = ( length >> 40 ) & 255;
		len[3] = ( length >> 32  ) & 255;
		len[4] = ( length >> 24 ) & 255;
		len[5] = ( length >> 16  ) & 255;
		len[6] = ( length >> 8 ) & 255;
		len[7] = ( length ) & 255;
		ACE_OS::memcpy(pEncryData+iPyl, len, 8);
		//length+=8;
		iPyl+=8;		
	}

	ACE_OS::memcpy(pEncryData+iPyl, pOriData, u4OriLen);
	u4EnCryLen = (uint32)expectedSize;

	return true;
}
