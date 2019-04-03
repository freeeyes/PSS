#include "BaseCommand.h"
CRecvTTyMessage::CRecvTTyMessage()
	:ITTyMessage(),
	m_pServerObject(nullptr)
{

}

CRecvTTyMessage::~CRecvTTyMessage()
{
}

bool CRecvTTyMessage::RecvData(uint16 u2ConnectID, const char * pData, uint32 u4len)
{
	char* pSendData = (char*)pData;
	OUR_DEBUG((LM_ERROR, "[CRecvTTyMessage::RecvData] u2ConnectID is %d, data is %s.\n", u2ConnectID, pData));
	if (u2ConnectID == TTY_CONNECT_ID_COM2)
	{
		m_pServerObject->GetTTyClientManager()->SendMessage(TTY_CONNECT_ID_COM2, pSendData, u4len);
	}

	return true;
}

bool CRecvTTyMessage::ReportMessage(uint16 u2ConnectID, uint32 u4ErrorNo, EM_TTY_EVENT_TYPE em_type)
{
	OUR_DEBUG((LM_ERROR, "[CRecvTTyMessage::ReportMessage] u4ErrorNo is %d, em_type is %d.\n", u4ErrorNo, em_type));
	return true;
}

void CRecvTTyMessage::SetServerObject(CServerObject * pServerObject)
{
	m_pServerObject = pServerObject;
}


CBaseCommand::CBaseCommand(void)
	:m_pServerObject(nullptr)
{
	m_pRecvTTyMsg = new CRecvTTyMessage();
}

CBaseCommand::~CBaseCommand(void)
{
}

void CBaseCommand::SetServerObject(CServerObject* pServerObject)
{
	m_pServerObject = pServerObject;
	m_pRecvTTyMsg->SetServerObject(pServerObject);
}

int CBaseCommand::OpenTTy()
{
	_TTyDevParam objCom2;
	_TTyDevParam objCom3;
	::memset(&objCom2, 0, sizeof(_TTyDevParam));
	::memset(&objCom3, 0, sizeof(_TTyDevParam));

	objCom2.m_nBaudRate = 9600;
	objCom2.m_uDatabits = 8;
	objCom2.m_uStopbits = 1;
	objCom2.m_pParitymode = "none";
	objCom2.m_blCTSenb = false;
	objCom2.m_blDSRenb = true;
	objCom2.m_blDTRdisable = false;
	objCom2.m_nReadtimeoutmsec = 50;
	::strcpy_safe(objCom2.m_szDevName, "COM2", 4);

	//objCom3.m_nBaudRate = 9600;
	//objCom3.m_uDatabits = 5;
	//objCom3.m_uStopbits = 1;
	//objCom3.m_pParitymode = "none";
	//objCom3.m_blCTSenb = false;
	//objCom3.m_blDSRenb = false;
	//objCom3.m_blDTRdisable = false;
	//objCom3.m_nReadtimeoutmsec = 500;
	//::strcpy_safe(objCom3.m_szDevName, "COM3", 4);

	m_pServerObject->GetTTyClientManager()->Connect(TTY_CONNECT_ID_COM2, "COM2", objCom2, (ITTyMessage*)m_pRecvTTyMsg);
	//m_pServerObject->GetTTyClientManager()->Connect(TTY_CONNECT_ID_COM1, "COM3", objCom3, (ITTyMessage*)m_pRecvTTyMsg);

	return 0;
}

void CBaseCommand::CloseAll()
{
	m_pServerObject->GetTTyClientManager()->Close(TTY_CONNECT_ID_COM2);
	//m_pServerObject->GetTTyClientManager()->Close(TTY_CONNECT_ID_COM2);
}

bool CBaseCommand::Pause(uint16 u2ConnectId)
{
	return m_pServerObject->GetTTyClientManager()->Pause(u2ConnectId);
}

bool CBaseCommand::Resume(uint16 u2ConnectId)
{
	return m_pServerObject->GetTTyClientManager()->Resume(u2ConnectId);
}

int CBaseCommand::DoMessage(IMessage* pMessage, bool& bDeleteFlag)
{	
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

	//处理链接建立信息
	if(pMessage->GetMessageBase()->m_u2Cmd == CLIENT_LINK_CONNECT)
	{
		OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] CLIENT_LINK_CONNECT OK.\n"));
	}

	//处理链接断开信息
	if(pMessage->GetMessageBase()->m_u2Cmd == CLIENT_LINK_CDISCONNET)
	{
		OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] CLIENT_LINK_CDISCONNET OK.\n"));
	}

	//测试透传数据
	if(pMessage->GetMessageBase()->m_u2Cmd == COMMAND_BASE)
	{
		uint16     u2CommandID = 0;
		uint64     u8ClientTime = 0;

		IBuffPacket* pBodyPacket = m_pServerObject->GetPacketManager()->Create();
		if(NULL == pBodyPacket)
		{
			OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pBodyPacket is NULL.\n"));
			return -1;
		}
		
		_PacketInfo BodyPacket;
		pMessage->GetPacketBody(BodyPacket);

		pBodyPacket->WriteStream(BodyPacket.m_pData, BodyPacket.m_nDataLen);

		(*pBodyPacket) >> u2CommandID;
		(*pBodyPacket) >> u8ClientTime;

		

		//ACE_Time_Value tvSleep(10);
		//ACE_OS::sleep(tvSleep);

		if(NULL != m_pServerObject->GetLogManager())
		{
			m_pServerObject->GetLogManager()->WriteLog(LOG_SYSTEM,  "[CBaseCommand::DoMessage] Get CommandID = %d", u2CommandID);
		}
		else
		{
			OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pLogManager = NULL"));
		}

		//测试返回数据(UDP)
		if(NULL != m_pServerObject->GetTTyClientManager())
		{
			char* pData = (char*)pBodyPacket->GetData();
			m_pServerObject->GetTTyClientManager()->SendMessage(TTY_CONNECT_ID_COM2, pData, pBodyPacket->GetPacketLen());
		}
		m_pServerObject->GetPacketManager()->Delete(pBodyPacket);
	}


	return 0;
}

