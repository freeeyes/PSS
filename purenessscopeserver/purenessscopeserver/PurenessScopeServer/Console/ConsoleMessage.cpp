#include "ConsoleMessage.h"

CConsoleMessage::CConsoleMessage()
{
	m_pvecConsoleKey = NULL;
}

CConsoleMessage::~CConsoleMessage()
{
}

int CConsoleMessage::Dispose(ACE_Message_Block* pmb, IBuffPacket* pBuffPacket)
{
	//处理命令
	if(NULL == pmb)
	{
		OUR_DEBUG((LM_ERROR, "[CConsoleMessage::Dispose]pmb is NULL.\n"));
		return CONSOLE_MESSAGE_FAIL;
	}

	char* pCommand = new char[(uint32)pmb->length() + 1];
	if(NULL == pCommand)
	{
		OUR_DEBUG((LM_ERROR, "[CConsoleMessage::Dispose]pCommand is NULL.\n"));
		return CONSOLE_MESSAGE_FAIL;
	}

	pCommand[(uint32)pmb->length()] = '\0';

	ACE_OS::memcpy(pCommand, pmb->rd_ptr(), (uint32)pmb->length());

	//解析命令，把数据切割出来
	if(CONSOLE_MESSAGE_SUCCESS != ParseCommand(pCommand, pBuffPacket))
	{
		SAFE_DELETE(pCommand);
		return CONSOLE_MESSAGE_FAIL;
	}
	else
	{
		SAFE_DELETE(pCommand);
		return CONSOLE_MESSAGE_SUCCESS;
	}
}

bool CConsoleMessage::GetCommandInfo(const char* pCommand, _CommandInfo& CommandInfo)
{
	int i = 0;
	int nLen = (int)ACE_OS::strlen(pCommand);
	char szKey[MAX_BUFF_100] = {'\0'};

	AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONSOLEDATA, "<Command>%s.", pCommand);

	if(nLen > MAX_BUFF_100*2 + 1)
	{
		OUR_DEBUG((LM_ERROR, "[CConsoleMessage::GetCommandInfo]pCommand is too long.\n"));
		return false;
	}

	//获得key值
	int nKeyEnd = 0;
	bool blFind = false;
	for(nKeyEnd = 0; nKeyEnd < nLen; nKeyEnd++)
	{
		if(nKeyEnd >= MAX_BUFF_100 - 1)
		{
			OUR_DEBUG((LM_ERROR, "[CConsoleMessage::GetCommandInfo]pCommand key is too long.\n"));
			return false;
		}

		if(pCommand[nKeyEnd] == ' ')
		{
			blFind = true;
			break;
		}
	}

	if(blFind == false)
	{
		OUR_DEBUG((LM_ERROR, "[CConsoleMessage::GetCommandInfo]No find command.\n"));
		return false;
	}

	ACE_OS::memcpy(&szKey, pCommand, nKeyEnd);
	szKey[nKeyEnd] = '\0';

	if(false == CheckConsoleKey(szKey))
	{
		OUR_DEBUG((LM_ERROR, "[CConsoleMessage::GetCommandInfo]szKey is invalid.\n"));
		return false;
	}


	//获得命令头
	for(i = nKeyEnd + 1; i < nLen; i++)
	{
		if(i >= MAX_BUFF_100 - 1)
		{
			OUR_DEBUG((LM_ERROR, "[CConsoleMessage::GetCommandInfo]pCommand m_szCommandTitle is too long.\n"));
			return false;
		}

		if(pCommand[i] == ' ')
		{
			blFind = true;
			break;
		}
	}

	if(blFind == false)
	{
		OUR_DEBUG((LM_ERROR, "[CConsoleMessage::GetCommandInfo]No find command.\n"));
		return false;
	}

	ACE_OS::memcpy(&CommandInfo.m_szCommandTitle, pCommand + nKeyEnd + 1, i - nKeyEnd - 1);
	CommandInfo.m_szCommandTitle[i - nKeyEnd - 1] = '\0';

	//获得扩展参数
	ACE_OS::memcpy(&CommandInfo.m_szCommandExp, pCommand + i + 1, (nLen - i + 1));
	CommandInfo.m_szCommandExp[nLen - i + 1] = '\0';

	return true;
}

int CConsoleMessage::ParseCommand(const char* pCommand, IBuffPacket* pBuffPacket)
{
	_CommandInfo CommandInfo;

	if(false == GetCommandInfo(pCommand, CommandInfo))
	{
		OUR_DEBUG((LM_ERROR, "[CConsoleMessage::ParseCommand]pCommand format is error.\n"));
		return CONSOLE_MESSAGE_FAIL;
	}

	if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAHE_LOADMOUDLE) == 0)
	{
		//处理添加模块的命令（支持重载）
		DoMessage_LoadModule(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAHE_UNLOADMOUDLE) == 0)
	{
		//处理卸载模块的命令
		DoMessage_UnLoadModule(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAHE_RELOADMOUDLE) == 0)
	{
		//处理卸载模块的命令
		DoMessage_ReLoadModule(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAHE_CLIENTCOUNT) == 0)
	{
		//处理获得当前连接数的命令
		DoMessage_ClientMessageCount(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAHE_SHOWMOUDLE) == 0)
	{
		//处理显示所有当前已经加载的模块名称和文件名
		DoMessage_ShowModule(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAHE_COMMANDINFO) == 0)
	{
		DoMessage_CommandInfo(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAHE_THREADINFO) == 0)
	{
		DoMessage_WorkThreadState(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAHE_CLIENTINFO) == 0)
	{
		DoMessage_ClientInfo(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAHE_FORBIDDENIP) == 0)
	{
		DoMessage_ForbiddenIP(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAHE_FORBIDDENIPSHOW) == 0)
	{
		DoMessage_ShowForbiddenList(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAHE_LIFTED) == 0)
	{
		DoMessage_LifedIP(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAHE_COLSECLIENT) == 0)
	{
		DoMessgae_CloseClient(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAHE_UDPCONNECTINFO) == 0)
	{
		DoMessage_UDPClientInfo(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAHE_SERVERCONNECT_TCP) == 0)
	{
		DoMessage_ServerConnectTCP(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAHE_SERVERCONNECT_UDP) == 0)
	{
		DoMessage_ServerConnectUDP(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAGE_PROCESSINFO) == 0)
	{
		DoMessage_ShowProcessInfo(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAGE_CLIENTHISTORY) == 0)
	{
		DoMessage_ShowClientHisTory(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAGE_ALLCOMMANDINFO) == 0)
	{
		DoMessage_ShowAllCommandInfo(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAGE_SERVERINFO) == 0)
	{
		DoMessage_ShowServerInfo(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAGE_SERVERRECONNECT) == 0)
	{
		DoMessage_ReConnectServer(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAHE_COMMANDTIMEOUT) == 0)
	{
		DoMessage_CommandTimeout(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAHE_COMMANDTIMEOUTCLR) == 0)
	{
		DoMessage_CommandTimeoutclr(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAGE_COMMANDDATALOG) == 0)
	{
		DoMessage_CommandDataLog(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAGE_SETDEBUG) == 0)
	{
		DoMessage_SetDebug(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAGE_SHOWDEBUG) == 0)
	{
		DoMessage_ShowDebug(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAGE_SETTRACKIP) == 0)
	{
		DoMessage_SetTrackIP(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAGE_DELTRACKIP) == 0)
	{
		DoMessage_DelTrackIP(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAGE_GETTRACKIPINFO) == 0)
	{
		DoMessage_GetTrackIPInfo(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAGE_GETCONNECTIPINFO) == 0)
	{
		DoMessage_GetConnectIPInfo(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAGE_GETLOGINF) == 0)
	{
		DoMessage_GetLogLevelInfo(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAGE_SETLOGLEVEL) == 0)
	{
		DoMessage_SetLogLevelInfo(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAGE_GETWTAI) == 0)
	{
		DoMessage_GetThreadAI(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAGE_GETWTTIMEOUT) == 0)
	{
		DoMessage_GetWorkThreadTO(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAGE_GETWTTIMEOUT) == 0)
	{
		DoMessage_GetWorkThreadTO(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAGE_GETNICKNAMEINFO) == 0)
	{
		DoMessage_GetNickNameInfo(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAGE_SETCONNECTLOG) == 0)
	{
		DoMessage_SetConnectLog(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandTitle, CONSOLEMESSAGE_SETMAXCONNECTCOUNT) == 0)
	{
		DoMessage_SetMaxConnectCount(CommandInfo, pBuffPacket);
		return CONSOLE_MESSAGE_SUCCESS;
	}
	else
	{
		return CONSOLE_MESSAGE_FAIL;
	}
}

bool CConsoleMessage::GetFileInfo(const char* pFile, _FileInfo& FileInfo)
{
	int i = 0;
	int nLen = (int)ACE_OS::strlen(pFile);

	bool blFind = false;
	for(i = nLen - 1; i >= 0; i--)
	{
		if(pFile[i] == '\\' || pFile[i] == '/')
		{
			blFind = true;
			break;
		}
	}

	if(false == blFind)
	{
		OUR_DEBUG((LM_ERROR, "[CConsoleMessage::GetFileInfo]No find file path.\n"));
		return false;
	}

	ACE_OS::memcpy(&FileInfo.m_szFileName, pFile + i + 1, nLen - i - 1);
	FileInfo.m_szFileName[nLen - i - 1] = '\0';

	ACE_OS::memcpy(&FileInfo.m_szFilePath, pFile, i + 1);
	FileInfo.m_szFilePath[i + 1] = '\0';

	return true;
}

bool CConsoleMessage::GetForbiddenIP(const char* pCommand, _ForbiddenIP& ForbiddenIP)
{
	char szTempData[MAX_BUFF_100] = {'\0'};

	//获得IP地址
	char* pPosBegin = (char* )ACE_OS::strstr(pCommand, "-c ");
	char* pPosEnd   = (char* )ACE_OS::strstr(pPosBegin + 3, " ");
	int nLen = (int)(pPosEnd - pPosBegin - 3);
	if(nLen >= MAX_BUFF_100 || nLen < 0)
	{
		return false;
	}
	ACE_OS::memcpy(szTempData, pPosBegin + 3, nLen);
	szTempData[nLen] = '\0';
	sprintf_safe(ForbiddenIP.m_szClientIP, MAX_IP_SIZE, szTempData);

	pPosBegin = (char* )ACE_OS::strstr(pCommand, "-t ");
	pPosEnd   = (char* )ACE_OS::strstr(pPosBegin + 3, " ");
	nLen = (int)(pPosEnd - pPosBegin - 3);
	if(nLen >= MAX_BUFF_100 || nLen < 0)
	{
		return false;
	}
	ACE_OS::memcpy(szTempData, pPosBegin + 3, nLen);
	szTempData[nLen] = '\0';
	ForbiddenIP.m_u1Type = (uint8)ACE_OS::atoi(szTempData);

	pPosBegin = (char* )ACE_OS::strstr(pCommand, "-s ");
	pPosEnd   = (char* )ACE_OS::strstr(pPosBegin + 3, " ");
	nLen = (int)(pPosEnd - pPosBegin - 3);
	if(nLen >= MAX_BUFF_100 || nLen < 0)
	{
		return false;
	}
	ACE_OS::memcpy(szTempData, pPosBegin + 3, nLen);
	szTempData[nLen] = '\0';
	ForbiddenIP.m_u4Second = (uint32)ACE_OS::atoi(szTempData);

	return true;
}

bool CConsoleMessage::GetTrackIP(const char* pCommand, _ForbiddenIP& ForbiddenIP)
{
	char szTempData[MAX_BUFF_100] = {'\0'};

	//获得IP地址
	char* pPosBegin = (char* )ACE_OS::strstr(pCommand, "-c ");
	char* pPosEnd   = (char* )ACE_OS::strstr(pPosBegin + 3, " ");
	int nLen = (int)(pPosEnd - pPosBegin - 3);
	if(nLen >= MAX_BUFF_100 || nLen < 0)
	{
		return false;
	}
	ACE_OS::memcpy(szTempData, pPosBegin + 3, nLen);
	szTempData[nLen] = '\0';
	sprintf_safe(ForbiddenIP.m_szClientIP, MAX_IP_SIZE, szTempData);

	return true;
}

bool CConsoleMessage::GetLogLevel(const char* pCommand, int& nLogLevel)
{
	char szTempData[MAX_BUFF_100] = {'\0'};

	//获得日志等级
	char* pPosBegin = (char* )ACE_OS::strstr(pCommand, "-l ");
	if(NULL != pPosBegin)
	{
		int nLen = ACE_OS::strlen(pCommand) - (int)(pPosBegin - pCommand) - 3;
		ACE_OS::memcpy(szTempData, pPosBegin + 3, nLen);
		nLogLevel = ACE_OS::atoi(szTempData);
	}

	return true;
}

bool CConsoleMessage::GetAIInfo(const char* pCommand, int& nAI, int& nDispose, int& nCheck, int& nStop)
{
	int nIndex               = 0;
	int nBegin               = 0;
	int nEnd                 = 0;
	char szTemp[MAX_BUFF_20] = {'\0'};

	nBegin = 3;
	ACE_OS::memset(szTemp, 0, MAX_BUFF_20);

	for(int i = 3; i < (int)ACE_OS::strlen(pCommand); i++)
	{
		if(pCommand[i] == ',')
		{
			nEnd = i;
			ACE_OS::memcpy(szTemp, (char* )&pCommand[nBegin], nEnd - nBegin);
			if(nIndex == 0)
			{
				nAI = ACE_OS::atoi(szTemp);
			}
			else if(nIndex == 1)
			{
				nDispose = ACE_OS::atoi(szTemp);
			}
			else if(nIndex == 2)
			{
				nCheck = ACE_OS::atoi(szTemp);
			}

			ACE_OS::memset(szTemp, 0, MAX_BUFF_20);
			nBegin = i + 1;
			nIndex++;
		}
	}

	//最后一个参数
	ACE_OS::memcpy(szTemp, (char* )&pCommand[nBegin], (int)ACE_OS::strlen(pCommand) - nBegin);
	nStop = ACE_OS::atoi(szTemp);

	return true;
}

bool CConsoleMessage::GetNickName(const char* pCommand, char* pName)
{
	//获得别名
	char* pPosBegin = (char* )ACE_OS::strstr(pCommand, "-n ");
	if(NULL != pPosBegin)
	{
		int nLen = ACE_OS::strlen(pCommand) - (int)(pPosBegin - pCommand) - 3;
		ACE_OS::memcpy(pName, pPosBegin + 3, nLen);
		pName[nLen] = '\0';
	}

	return true;
}

bool CConsoleMessage::GetConnectID(const char* pCommand, uint32& u4ConnectID, bool& blFlag)
{
	char szTempData[MAX_BUFF_100] = {'\0'};
	int  nFlag                    = 0;

	//获得ConnectID
	char* pPosBegin = (char* )ACE_OS::strstr(pCommand, "-n ");
	char* pPosEnd   = (char* )ACE_OS::strstr(pPosBegin + 3, " ");
	int nLen = (int)(pPosEnd - pPosBegin - 3);
	if(nLen >= MAX_BUFF_100 || nLen < 0)
	{
		return false;
	}
	ACE_OS::memcpy(szTempData, pPosBegin + 3, nLen);
	szTempData[nLen] = '\0';
	u4ConnectID = (uint32)ACE_OS::atoi(szTempData);

	pPosBegin = (char* )ACE_OS::strstr(pCommand, "-f ");
	pPosEnd   = (char* )ACE_OS::strstr(pPosBegin + 3, " ");
	nLen = (int)(pPosEnd - pPosBegin - 3);
	if(nLen >= MAX_BUFF_100 || nLen < 0)
	{
		return false;
	}
	ACE_OS::memcpy(szTempData, pPosBegin + 3, nLen);
	szTempData[nLen] = '\0';
	nFlag = (int)ACE_OS::atoi(szTempData);

	if(nFlag == 0)
	{
		blFlag = false;
	}
	else
	{
		blFlag = true;
	}

	return true;
}

bool CConsoleMessage::GetMaxConnectCount(const char* pCommand, uint16& u2MaxConnectCount)
{
	char szTempData[MAX_BUFF_100] = {'\0'};

	//获得ConnectID
	char* pPosBegin = (char* )ACE_OS::strstr(pCommand, "-n ");
	char* pPosEnd   = (char* )ACE_OS::strstr(pPosBegin + 3, " ");
	int nLen = (int)(pPosEnd - pPosBegin - 3);
	if(nLen >= MAX_BUFF_100 || nLen < 0)
	{
		return false;
	}
	ACE_OS::memcpy(szTempData, pPosBegin + 3, nLen);
	szTempData[nLen] = '\0';
	u2MaxConnectCount = (uint16)ACE_OS::atoi(szTempData);

	return true;
}

bool CConsoleMessage::GetConnectServerID(const char* pCommand, int& nServerID)
{
	char szTempData[MAX_BUFF_100] = {'\0'};

	int nCommandLen = (int)ACE_OS::strlen(pCommand);
	//获得IP地址
	char* pPosBegin = (char* )ACE_OS::strstr(pCommand, "-s ");
	int nLen = (int)(nCommandLen - (pPosBegin - pCommand) - 3);
	if(nLen >= MAX_BUFF_100 || nLen < 0)
	{
		return false;
	}
	ACE_OS::memcpy(szTempData, pPosBegin + 3, nLen);

	nServerID = ACE_OS::atoi(szTempData);

	return true;
}

bool CConsoleMessage::DoMessage_LoadModule(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket)
{
	_FileInfo FileInfo;
	if(false == GetFileInfo(CommandInfo.m_szCommandExp, FileInfo))
	{
		if(NULL != pBuffPacket)
		{
			(*pBuffPacket) << (uint8)1;
		}
		return false;
	}

	if(true == App_ModuleLoader::instance()->LoadModule(FileInfo.m_szFilePath, FileInfo.m_szFileName))
	{
		if(NULL != pBuffPacket)
		{
			(*pBuffPacket) << (uint8)0;
		}
	}
	else
	{
		if(NULL != pBuffPacket)
		{
			(*pBuffPacket) << (uint8)1;
		}
	}

	return true;
}

bool CConsoleMessage::DoMessage_UnLoadModule(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket)
{
	if(true == App_MessageManager::instance()->UnloadModuleCommand(CommandInfo.m_szCommandExp, (uint8)1))
	{
		if(NULL != pBuffPacket)
		{
			(*pBuffPacket) << (uint8)0;
		}
	}
	else
	{
		if(NULL != pBuffPacket)
		{
			(*pBuffPacket) << (uint8)1;
		}
	}

	return true;
}

bool CConsoleMessage::DoMessage_ReLoadModule(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket)
{
	if(true == App_MessageManager::instance()->UnloadModuleCommand(CommandInfo.m_szCommandExp, (uint8)2))
	{
		if(NULL != pBuffPacket)
		{
			(*pBuffPacket) << (uint8)0;
		}
	}
	else
	{
		if(NULL != pBuffPacket)
		{
			(*pBuffPacket) << (uint8)1;
		}
	}

	return true;
}

bool CConsoleMessage::DoMessage_ClientMessageCount(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket)
{
	if(ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-c") == 0)
	{
		//-c 只返回当前激活的链接数
#if WIN32
		int nActiveClient = App_ProConnectManager::instance()->GetCount();
		(*pBuffPacket) << (uint32)nActiveClient;
#else
		int nActiveClient = App_ConnectManager::instance()->GetCount();
		(*pBuffPacket) << (uint32)nActiveClient;
#endif
	}
	else if(ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-cp") == 0)
	{
		//-cp 返回当前激活连接数和池中剩余可分配数
#if WIN32
		int nActiveClient = App_ProConnectManager::instance()->GetCount();
		int nPoolClient   = App_ProConnectHandlerPool::instance()->GetFreeCount();
		(*pBuffPacket) << (uint32)nActiveClient;
		(*pBuffPacket) << (uint32)nPoolClient;
		(*pBuffPacket) << App_MainConfig::instance()->GetMaxHandlerCount();
#else
		int nActiveClient = App_ConnectManager::instance()->GetCount();
		int nPoolClient   = App_ConnectHandlerPool::instance()->GetFreeCount();
		(*pBuffPacket) << (uint32)nActiveClient;
		(*pBuffPacket) << (uint32)nPoolClient;
		(*pBuffPacket) << App_MainConfig::instance()->GetMaxHandlerCount();
#endif
	}

	return true;
}

bool CConsoleMessage::DoMessage_ShowModule(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket)
{
	if(ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
	{
		if(App_ModuleLoader::instance()->GetCurrModuleCount() != 0)
		{
			(*pBuffPacket) << (uint32)App_ModuleLoader::instance()->GetCurrModuleCount();
		}
		else
		{
			if(NULL != pBuffPacket)
			{
				(*pBuffPacket) << (uint32)0;
			}
		}

		for(int i = 0; i < App_ModuleLoader::instance()->GetCurrModuleCount(); i++)
		{
			_ModuleInfo* pModuleInfo = App_ModuleLoader::instance()->GetModuleIndex(i);
			if(NULL != pModuleInfo)
			{
				VCHARS_STR strSName;
				strSName.u1Len = (uint8)ACE_OS::strlen(pModuleInfo->GetName());
				strSName.text  = (char* )pModuleInfo->GetName();
				(*pBuffPacket) << strSName;
				VCHARS_STR strSModileFile;
				strSModileFile.u1Len = (uint8)ACE_OS::strlen(pModuleInfo->strModuleName.c_str());
				strSModileFile.text  = (char* )pModuleInfo->strModuleName.c_str();
				(*pBuffPacket) << strSModileFile;
				VCHARS_STR strSModileDesc;
				strSModileDesc.u1Len = (uint8)ACE_OS::strlen(pModuleInfo->GetDesc());
				strSModileDesc.text  = (char* )pModuleInfo->GetDesc();
				(*pBuffPacket) << strSModileDesc;

				char szTime[MAX_BUFF_100] = {'\0'};
				sprintf_safe(szTime, MAX_BUFF_100, "%04d-%02d-%02d %02d:%02d:%02d", pModuleInfo->dtCreateTime.year(), pModuleInfo->dtCreateTime.month(), pModuleInfo->dtCreateTime.day(), pModuleInfo->dtCreateTime.hour(), pModuleInfo->dtCreateTime.minute(), pModuleInfo->dtCreateTime.second());
				strSName.text  = (char* )szTime;
				strSName.u1Len = (uint8)ACE_OS::strlen(szTime);
				(*pBuffPacket) << strSName;
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool CConsoleMessage::DoMessage_CommandInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket)
{
	uint16 u2CommandID = (uint16)ACE_OS::strtol(CommandInfo.m_szCommandExp, NULL, 16);
	if(u2CommandID != 0)
	{
		(*pBuffPacket) << (uint32)App_MessageManager::instance()->GetCommandCount();
		CClientCommandList* pClientCommandList = App_MessageManager::instance()->GetClientCommandList(u2CommandID);
		if(pClientCommandList != NULL)
		{
			int nCount = pClientCommandList->GetCount();
			(*pBuffPacket) << (uint16)nCount;
			for(int i = 0; i < nCount; i++)
			{
				_ClientCommandInfo* pClientCommandInfo = pClientCommandList->GetClientCommandIndex(i);
				if(NULL != pClientCommandInfo)
				{
					VCHARS_STR strSName;
					strSName.text  = pClientCommandInfo->m_szModuleName;
					strSName.u1Len = (uint8)ACE_OS::strlen(pClientCommandInfo->m_szModuleName);
					(*pBuffPacket) << strSName;
					(*pBuffPacket) << u2CommandID;
					(*pBuffPacket) << pClientCommandInfo->m_u4Count;
					(*pBuffPacket) << pClientCommandInfo->m_u4TimeCost;
				}
			}
		}
		return true;
	}
	else
	{
		return true;
	}
}

bool CConsoleMessage::DoMessage_WorkThreadState(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket)
{
	if(ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-s") == 0)
	{
		//获得当前加解密包线程状态
		CThreadInfo* pThreadInfo = NULL;

		//获得当前工作线程状态
		pThreadInfo = App_MessageServiceGroup::instance()->GetThreadInfo();
		if(NULL != pThreadInfo)
		{
			int nThreadCount = pThreadInfo->GetThreadCount();
			(*pBuffPacket) << (uint8)nThreadCount;

			for(int i = 0; i < nThreadCount; i++)
			{
				_ThreadInfo* pCurrThreadInfo = pThreadInfo->GetThreadInfo(i);
				(*pBuffPacket) << (uint8)i;
				(*pBuffPacket) << (uint32)pCurrThreadInfo->m_tvUpdateTime.sec();
				(*pBuffPacket) << (uint32)pCurrThreadInfo->m_tvCreateTime.sec();
				(*pBuffPacket) << (uint8)pCurrThreadInfo->m_u4State;
				(*pBuffPacket) << (uint32)pCurrThreadInfo->m_u4RecvPacketCount;
				(*pBuffPacket) << (uint16)pCurrThreadInfo->m_u2CommandID;
				(*pBuffPacket) << (uint16)pCurrThreadInfo->m_u2PacketTime;
				(*pBuffPacket) << (uint32)pCurrThreadInfo->m_u4CurrPacketCount;
			}

			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool CConsoleMessage::DoMessage_ClientInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket)
{
	if(ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
	{
		char szIP[MAX_BUFF_100] = {'\0'};
#ifdef WIN32
		vecClientConnectInfo VecClientConnectInfo;
		App_ProConnectManager::instance()->GetConnectInfo(VecClientConnectInfo);

		(*pBuffPacket) << (uint32)VecClientConnectInfo.size();
		for(int i = 0; i < (int)VecClientConnectInfo.size(); i++)
		{
			_ClientConnectInfo ClientConnectInfo = VecClientConnectInfo[i];
			if(true == ClientConnectInfo.m_blValid)
			{
				sprintf_safe(szIP, MAX_BUFF_100, "%s:%d", ClientConnectInfo.m_addrRemote.get_host_addr(), ClientConnectInfo.m_addrRemote.get_port_number());

				VCHARS_STR strSName;
				strSName.text  = szIP;
				strSName.u1Len = (uint8)ACE_OS::strlen(szIP);

				(*pBuffPacket) << strSName;
				(*pBuffPacket) << ClientConnectInfo.m_u4ConnectID;
				(*pBuffPacket) << ClientConnectInfo.m_u4RecvCount;
				(*pBuffPacket) << ClientConnectInfo.m_u4SendCount;
				(*pBuffPacket) << ClientConnectInfo.m_u4AllRecvSize;
				(*pBuffPacket) << ClientConnectInfo.m_u4AllSendSize;
				(*pBuffPacket) << ClientConnectInfo.m_u4BeginTime;
				(*pBuffPacket) << ClientConnectInfo.m_u4AliveTime;
				(*pBuffPacket) << ClientConnectInfo.m_u4RecvQueueCount;
				(*pBuffPacket) << ClientConnectInfo.m_u8RecvQueueTimeCost;
				(*pBuffPacket) << ClientConnectInfo.m_u8SendQueueTimeCost;
			}
		}
#else
		vecClientConnectInfo VecClientConnectInfo;
		App_ConnectManager::instance()->GetConnectInfo(VecClientConnectInfo);

		(*pBuffPacket) << (uint32)VecClientConnectInfo.size();
		for(int i = 0; i < (int)VecClientConnectInfo.size(); i++)
		{
			_ClientConnectInfo ClientConnectInfo = VecClientConnectInfo[i];
			if(true == ClientConnectInfo.m_blValid)
			{
				sprintf_safe(szIP, MAX_BUFF_100, "%s:%d", ClientConnectInfo.m_addrRemote.get_host_addr(), ClientConnectInfo.m_addrRemote.get_port_number());

				VCHARS_STR strSName;
				strSName.text  = szIP;
				strSName.u1Len = (uint8)ACE_OS::strlen(szIP);

				(*pBuffPacket) << strSName;
				(*pBuffPacket) << ClientConnectInfo.m_u4ConnectID;
				(*pBuffPacket) << ClientConnectInfo.m_u4RecvCount;
				(*pBuffPacket) << ClientConnectInfo.m_u4SendCount;
				(*pBuffPacket) << ClientConnectInfo.m_u4AllRecvSize;
				(*pBuffPacket) << ClientConnectInfo.m_u4AllSendSize;
				(*pBuffPacket) << ClientConnectInfo.m_u4BeginTime;
				(*pBuffPacket) << ClientConnectInfo.m_u4AliveTime;
				(*pBuffPacket) << ClientConnectInfo.m_u4RecvQueueCount;
				(*pBuffPacket) << ClientConnectInfo.m_u8RecvQueueTimeCost;
				(*pBuffPacket) << ClientConnectInfo.m_u8SendQueueTimeCost;
			}
		}
#endif
	}

	return true;
}

bool CConsoleMessage::DoMessgae_CloseClient(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket)
{
	uint32 u4ConnectID = (uint32)ACE_OS::atoi(CommandInfo.m_szCommandExp);
#ifdef WIN32
	App_ProConnectManager::instance()->CloseConnect(u4ConnectID);
	(*pBuffPacket) << (uint8)0;
#else
	App_ConnectManager::instance()->CloseConnect(u4ConnectID);
	(*pBuffPacket) << (uint8)0;
#endif

	return true;
}

bool CConsoleMessage::DoMessage_ForbiddenIP(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket)
{
	_ForbiddenIP ForbiddenIP;
	if(GetForbiddenIP(CommandInfo.m_szCommandExp, ForbiddenIP) == true)
	{
		if(ForbiddenIP.m_u1Type == 0)
		{
			//永久封禁IP
			App_ForbiddenIP::instance()->AddForeverIP(ForbiddenIP.m_szClientIP);
		}
		else
		{
			//封禁时段IP
			App_ForbiddenIP::instance()->AddTempIP(ForbiddenIP.m_szClientIP, ForbiddenIP.m_u4Second);
		}

		(*pBuffPacket) << (uint8)0;   //添加成功
	}
	else
	{
		(*pBuffPacket) << (uint8)1;   //添加失败
	}
	return true;
}

bool CConsoleMessage::DoMessage_ShowForbiddenList(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket)
{
	if(ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
	{
		VecForbiddenIP* pForeverForbiddenIP = App_ForbiddenIP::instance()->ShowForeverIP();
		VecForbiddenIP* pTempForbiddenIP = App_ForbiddenIP::instance()->ShowTemoIP();

		if(pForeverForbiddenIP == NULL || pTempForbiddenIP == NULL)
		{
			return false;
		}

		uint32 u4Count = (uint32)pForeverForbiddenIP->size() + (uint32)pTempForbiddenIP->size();
		(*pBuffPacket) << u4Count;

		for(int i = 0; i < (int)pForeverForbiddenIP->size(); i++)
		{
			VCHARS_STR strSName;
			strSName.text  = pForeverForbiddenIP->at(i).m_szClientIP;
			strSName.u1Len = (uint8)ACE_OS::strlen(pForeverForbiddenIP->at(i).m_szClientIP);

			(*pBuffPacket) << strSName;
			(*pBuffPacket) << pForeverForbiddenIP->at(i).m_u1Type;
			(*pBuffPacket) << (uint32)pForeverForbiddenIP->at(i).m_tvBegin.sec();
			(*pBuffPacket) << pForeverForbiddenIP->at(i).m_u4Second;
		}

		for(int i = 0; i < (int)pTempForbiddenIP->size(); i++)
		{
			VCHARS_STR strSName;
			strSName.text  = pTempForbiddenIP->at(i).m_szClientIP;
			strSName.u1Len = (uint8)ACE_OS::strlen(pTempForbiddenIP->at(i).m_szClientIP);

			(*pBuffPacket) << strSName;
			(*pBuffPacket) << pTempForbiddenIP->at(i).m_u1Type;
			(*pBuffPacket) << (uint32)pTempForbiddenIP->at(i).m_tvBegin.sec();
			(*pBuffPacket) << pTempForbiddenIP->at(i).m_u4Second;
		}
	}

	return true;
}

bool CConsoleMessage::DoMessage_LifedIP(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket)
{
	App_ForbiddenIP::instance()->DelForeverIP(CommandInfo.m_szCommandExp);
	App_ForbiddenIP::instance()->DelTempIP(CommandInfo.m_szCommandExp);

	(*pBuffPacket) << (uint8)0;
	return true;
}

bool CConsoleMessage::DoMessage_UDPClientInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket)
{
	if(ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
	{
		char szIP[MAX_BUFF_100] = {'\0'};
#ifdef WIN32
		vecClientConnectInfo VecClientConnectInfo;
		App_ProUDPManager::instance()->GetClientConnectInfo(VecClientConnectInfo);
		(*pBuffPacket) << (uint32)VecClientConnectInfo.size();
		for(int i = 0; i < (int)VecClientConnectInfo.size(); i++)
		{
			_ClientConnectInfo ClientConnectInfo = VecClientConnectInfo[i];
			if(true == ClientConnectInfo.m_blValid)
			{
				sprintf_safe(szIP, MAX_BUFF_100, "0.0.0.0:0");

				VCHARS_STR strSName;
				strSName.text  = szIP;
				strSName.u1Len = (uint8)ACE_OS::strlen(szIP);

				(*pBuffPacket) << strSName;
				(*pBuffPacket) << ClientConnectInfo.m_u4ConnectID;
				(*pBuffPacket) << ClientConnectInfo.m_u4RecvCount;
				(*pBuffPacket) << ClientConnectInfo.m_u4SendCount;
				(*pBuffPacket) << ClientConnectInfo.m_u4AllRecvSize;
				(*pBuffPacket) << ClientConnectInfo.m_u4AllSendSize;
				(*pBuffPacket) << ClientConnectInfo.m_u4BeginTime;
				(*pBuffPacket) << ClientConnectInfo.m_u4AliveTime;
				(*pBuffPacket) << ClientConnectInfo.m_u4RecvQueueCount;
				(*pBuffPacket) << ClientConnectInfo.m_u8RecvQueueTimeCost;
				(*pBuffPacket) << ClientConnectInfo.m_u8SendQueueTimeCost;
			}
		}
#else
		vecClientConnectInfo VecClientConnectInfo;
		App_ReUDPManager::instance()->GetClientConnectInfo(VecClientConnectInfo);
		(*pBuffPacket) << (uint8)VecClientConnectInfo.size();
		for(int i = 0; i < (int)VecClientConnectInfo.size(); i++)
		{
			_ClientConnectInfo ClientConnectInfo = VecClientConnectInfo[i];
			if(true == ClientConnectInfo.m_blValid)
			{
				sprintf_safe(szIP, MAX_BUFF_100, "0.0.0.0:0");

				VCHARS_STR strSName;
				strSName.text  = szIP;
				strSName.u1Len = (uint8)ACE_OS::strlen(szIP);

				(*pBuffPacket) << strSName;
				(*pBuffPacket) << ClientConnectInfo.m_u4ConnectID;
				(*pBuffPacket) << ClientConnectInfo.m_u4RecvCount;
				(*pBuffPacket) << ClientConnectInfo.m_u4SendCount;
				(*pBuffPacket) << ClientConnectInfo.m_u4AllRecvSize;
				(*pBuffPacket) << ClientConnectInfo.m_u4AllSendSize;
				(*pBuffPacket) << ClientConnectInfo.m_u4BeginTime;
				(*pBuffPacket) << ClientConnectInfo.m_u4AliveTime;
				(*pBuffPacket) << ClientConnectInfo.m_u4RecvQueueCount;
				(*pBuffPacket) << ClientConnectInfo.m_u8RecvQueueTimeCost;
				(*pBuffPacket) << ClientConnectInfo.m_u8SendQueueTimeCost;
			}
		}
#endif
	}
	return true;
}

bool CConsoleMessage::DoMessage_ServerConnectTCP(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket)
{
	if(ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
	{
		char szIP[MAX_BUFF_100] = {'\0'};
#ifdef WIN32
		vecClientConnectInfo VecClientConnectInfo;
		App_ClientProConnectManager::instance()->GetConnectInfo(VecClientConnectInfo);
		(*pBuffPacket) << (uint32)VecClientConnectInfo.size();
		for(int i = 0; i < (int)VecClientConnectInfo.size(); i++)
		{
			_ClientConnectInfo ClientConnectInfo = VecClientConnectInfo[i];

			sprintf_safe(szIP, MAX_BUFF_100, "%s:%d", ClientConnectInfo.m_addrRemote.get_host_addr(), ClientConnectInfo.m_addrRemote.get_port_number());

			VCHARS_STR strSName;
			strSName.text  = szIP;
			strSName.u1Len = (uint8)ACE_OS::strlen(szIP);

			if(ClientConnectInfo.m_blValid == true)
			{
				(*pBuffPacket) << (uint8)0;   //链接已存在
			}
			else
			{
				(*pBuffPacket) << (uint8)1;   //连接不存在
			}

			(*pBuffPacket) << strSName;
			(*pBuffPacket) << ClientConnectInfo.m_u4ConnectID;
			(*pBuffPacket) << ClientConnectInfo.m_u4RecvCount;
			(*pBuffPacket) << ClientConnectInfo.m_u4SendCount;
			(*pBuffPacket) << ClientConnectInfo.m_u4AllRecvSize;
			(*pBuffPacket) << ClientConnectInfo.m_u4AllSendSize;
			(*pBuffPacket) << ClientConnectInfo.m_u4BeginTime;
			(*pBuffPacket) << ClientConnectInfo.m_u4AliveTime;
			(*pBuffPacket) << ClientConnectInfo.m_u4RecvQueueCount;
			(*pBuffPacket) << ClientConnectInfo.m_u8RecvQueueTimeCost;
			(*pBuffPacket) << ClientConnectInfo.m_u8SendQueueTimeCost;
		}
#else
		vecClientConnectInfo VecClientConnectInfo;
		App_ClientReConnectManager::instance()->GetConnectInfo(VecClientConnectInfo);
		(*pBuffPacket) << (uint8)VecClientConnectInfo.size();
		for(int i = 0; i < (int)VecClientConnectInfo.size(); i++)
		{
			_ClientConnectInfo ClientConnectInfo = VecClientConnectInfo[i];

			sprintf_safe(szIP, MAX_BUFF_100, "%s:%d", ClientConnectInfo.m_addrRemote.get_host_addr(), ClientConnectInfo.m_addrRemote.get_port_number());

			VCHARS_STR strSName;
			strSName.text  = szIP;
			strSName.u1Len = (uint8)ACE_OS::strlen(szIP);

			if(ClientConnectInfo.m_blValid == true)
			{
				(*pBuffPacket) << (uint8)0;   //链接已存在
			}
			else
			{
				(*pBuffPacket) << (uint8)1;   //连接不存在
			}

			(*pBuffPacket) << strSName;
			(*pBuffPacket) << ClientConnectInfo.m_u4ConnectID;
			(*pBuffPacket) << ClientConnectInfo.m_u4RecvCount;
			(*pBuffPacket) << ClientConnectInfo.m_u4SendCount;
			(*pBuffPacket) << ClientConnectInfo.m_u4AllRecvSize;
			(*pBuffPacket) << ClientConnectInfo.m_u4AllSendSize;
			(*pBuffPacket) << ClientConnectInfo.m_u4BeginTime;
			(*pBuffPacket) << ClientConnectInfo.m_u4AliveTime;
			(*pBuffPacket) << ClientConnectInfo.m_u4RecvQueueCount;
			(*pBuffPacket) << ClientConnectInfo.m_u8RecvQueueTimeCost;
			(*pBuffPacket) << ClientConnectInfo.m_u8SendQueueTimeCost;
		}
#endif
	}

	return true;
}

bool CConsoleMessage::DoMessage_ServerConnectUDP(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket)
{
	if(ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
	{
		char szIP[MAX_BUFF_100] = {'\0'};
#ifdef WIN32
		vecClientConnectInfo VecClientConnectInfo;
		App_ClientProConnectManager::instance()->GetUDPConnectInfo(VecClientConnectInfo);
		(*pBuffPacket) << (uint32)VecClientConnectInfo.size();
		for(int i = 0; i < (int)VecClientConnectInfo.size(); i++)
		{
			_ClientConnectInfo ClientConnectInfo = VecClientConnectInfo[i];
			if(true == ClientConnectInfo.m_blValid)
			{
				sprintf_safe(szIP, MAX_BUFF_100, "0.0.0.0:0");

				VCHARS_STR strSName;
				strSName.text  = szIP;
				strSName.u1Len = (uint8)ACE_OS::strlen(szIP);

				(*pBuffPacket) << strSName;
				(*pBuffPacket) << ClientConnectInfo.m_u4ConnectID;
				(*pBuffPacket) << ClientConnectInfo.m_u4RecvCount;
				(*pBuffPacket) << ClientConnectInfo.m_u4SendCount;
				(*pBuffPacket) << ClientConnectInfo.m_u4AllRecvSize;
				(*pBuffPacket) << ClientConnectInfo.m_u4AllSendSize;
				(*pBuffPacket) << ClientConnectInfo.m_u4BeginTime;
				(*pBuffPacket) << ClientConnectInfo.m_u4AliveTime;
				(*pBuffPacket) << ClientConnectInfo.m_u4RecvQueueCount;
				(*pBuffPacket) << ClientConnectInfo.m_u8RecvQueueTimeCost;
				(*pBuffPacket) << ClientConnectInfo.m_u8SendQueueTimeCost;
			}
		}
#else
		vecClientConnectInfo VecClientConnectInfo;
		App_ClientReConnectManager::instance()->GetUDPConnectInfo(VecClientConnectInfo);
		(*pBuffPacket) << (uint8)VecClientConnectInfo.size();
		for(int i = 0; i < (int)VecClientConnectInfo.size(); i++)
		{
			_ClientConnectInfo ClientConnectInfo = VecClientConnectInfo[i];
			if(true == ClientConnectInfo.m_blValid)
			{
				sprintf_safe(szIP, MAX_BUFF_100, "0.0.0.0:0");

				VCHARS_STR strSName;
				strSName.text  = szIP;
				strSName.u1Len = (uint8)ACE_OS::strlen(szIP);

				(*pBuffPacket) << strSName;
				(*pBuffPacket) << ClientConnectInfo.m_u4ConnectID;
				(*pBuffPacket) << ClientConnectInfo.m_u4RecvCount;
				(*pBuffPacket) << ClientConnectInfo.m_u4SendCount;
				(*pBuffPacket) << ClientConnectInfo.m_u4AllRecvSize;
				(*pBuffPacket) << ClientConnectInfo.m_u4AllSendSize;
				(*pBuffPacket) << ClientConnectInfo.m_u4BeginTime;
				(*pBuffPacket) << ClientConnectInfo.m_u4AliveTime;
				(*pBuffPacket) << ClientConnectInfo.m_u4RecvQueueCount;
				(*pBuffPacket) << ClientConnectInfo.m_u8RecvQueueTimeCost;
				(*pBuffPacket) << ClientConnectInfo.m_u8SendQueueTimeCost;
			}
		}
#endif
	}

	return true;
}

bool CConsoleMessage::DoMessage_ShowProcessInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket)
{
	if(ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
	{


#ifdef WIN32  //如果是windows
		int nCPU = GetProcessCPU_Idel();
		(*pBuffPacket) << (uint32)nCPU;

		int nMemorySize = GetProcessMemorySize();
		(*pBuffPacket) << (uint32)nMemorySize;

		(*pBuffPacket) << (uint8)App_CommandAccount::instance()->GetFLow();
		(*pBuffPacket) << (uint32)App_CommandAccount::instance()->GetFlowIn();
		(*pBuffPacket) << (uint32)App_CommandAccount::instance()->GetFlowOut();


#else   //如果是linux
		int nCPU = GetProcessCPU_Idel_Linux();
		(*pBuffPacket) << (uint32)nCPU;

		int nMemorySize = GetProcessMemorySize_Linux();
		(*pBuffPacket) << (uint32)nMemorySize;

		(*pBuffPacket) << (uint8)App_CommandAccount::instance()->GetFLow();
		(*pBuffPacket) << (uint32)App_CommandAccount::instance()->GetFlowIn();
		(*pBuffPacket) << (uint32)App_CommandAccount::instance()->GetFlowOut();
#endif
	}

	return true;
}

bool CConsoleMessage::DoMessage_ShowClientHisTory(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket)
{
	if(ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
	{
		char szTime[MAX_BUFF_100] = {'\0'};

		vecIPAccount VecIPAccount;
		App_IPAccount::instance()->GetInfo(VecIPAccount);

		(*pBuffPacket) << (uint32)VecIPAccount.size();
		for(int i = 0; i < (int)VecIPAccount.size(); i++)
		{
			VCHARS_STR strSName;
			strSName.text  = (char* )VecIPAccount[i].m_strIP.c_str();
			strSName.u1Len = (uint8)VecIPAccount[i].m_strIP.length();

			(*pBuffPacket) << strSName;
			(*pBuffPacket) << (uint32)VecIPAccount[i].m_nCount;
			(*pBuffPacket) << (uint32)VecIPAccount[i].m_nAllCount;

			sprintf_safe(szTime, MAX_BUFF_100, "%04d-%02d-%02d %02d:%02d:%02d", VecIPAccount[i].m_dtLastTime.year(), VecIPAccount[i].m_dtLastTime.month(), VecIPAccount[i].m_dtLastTime.day(), VecIPAccount[i].m_dtLastTime.hour(), VecIPAccount[i].m_dtLastTime.minute(), VecIPAccount[i].m_dtLastTime.second());
			strSName.text  = (char* )szTime;
			strSName.u1Len = (uint8)ACE_OS::strlen(szTime);

			(*pBuffPacket) << strSName;
		}
	}

	return true;
}

bool CConsoleMessage::DoMessage_ShowAllCommandInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket)
{
	if(ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
	{
		mapModuleClient* pmapModuleClient = App_MessageManager::instance()->GetModuleClient();
		if(pmapModuleClient != NULL)
		{
			(*pBuffPacket) << (uint32)pmapModuleClient->size();
			for(mapModuleClient::iterator b = pmapModuleClient->begin(); b != pmapModuleClient->end(); b++)
			{
				_ModuleClient* pModuleClient = (_ModuleClient* )b->second;
				if(NULL != pModuleClient)
				{
					(*pBuffPacket) << (uint32)pModuleClient->m_vecClientCommandInfo.size();
					for(int i = 0; i < (int)pModuleClient->m_vecClientCommandInfo.size(); i++)
					{
						_ClientCommandInfo* pClientCommandInfo = (_ClientCommandInfo* )pModuleClient->m_vecClientCommandInfo[i];
						if(NULL != pClientCommandInfo)
						{
							VCHARS_STR strSName;
							strSName.text  = pClientCommandInfo->m_szModuleName;
							strSName.u1Len = (uint8)ACE_OS::strlen(pClientCommandInfo->m_szModuleName);							
							(*pBuffPacket) << strSName;
							(*pBuffPacket) << pClientCommandInfo->m_u2CommandID;
							(*pBuffPacket) << pClientCommandInfo->m_u4Count;
							(*pBuffPacket) << pClientCommandInfo->m_u4TimeCost;
						}
					}
				}
			}
		}
	}

	return true;	
}

bool CConsoleMessage::DoMessage_ShowServerInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket)
{
	if(ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
	{
		VCHARS_STR strSTemp;

		//返回服务器ID
		uint16 u2SerevrID = (uint32)App_MainConfig::instance()->GetServerID();
		(*pBuffPacket) << u2SerevrID;

		//返回服务器名称
		strSTemp.text  = (char* )App_MainConfig::instance()->GetServerName();
		strSTemp.u1Len = (uint8)ACE_OS::strlen(App_MainConfig::instance()->GetServerName());
		(*pBuffPacket) << strSTemp;

		//返回服务器版本
		strSTemp.text  = (char* )App_MainConfig::instance()->GetServerVersion();
		strSTemp.u1Len = (uint8)ACE_OS::strlen(App_MainConfig::instance()->GetServerName());
		(*pBuffPacket) << strSTemp;

		//返回加载模块个数
		(*pBuffPacket) << (uint16)App_ModuleLoader::instance()->GetCurrModuleCount();

		//返回工作线程个数
		(*pBuffPacket) << (uint16)App_MessageServiceGroup::instance()->GetThreadInfo()->GetThreadCount();

		//返回当前协议包的版本号
		strSTemp.text  = (char* )App_MainConfig::instance()->GetServerVersion();
		strSTemp.u1Len = (uint8)ACE_OS::strlen(App_MainConfig::instance()->GetPacketVersion());
		(*pBuffPacket) << strSTemp;

		//返回当前服务器是大端还是小端
		if(App_MainConfig::instance()->GetCharOrder() == SYSTEM_LITTLE_ORDER)
		{
			(*pBuffPacket) << (uint8)0;     //小端
		}
		else
		{
			(*pBuffPacket) << (uint8)1;     //大端
		}

		//返回当前网络包字序规则
		if(App_MainConfig::instance()->GetByteOrder() == false)
		{
			(*pBuffPacket) << (uint8)0;   //主机字序
		}
		else
		{
			(*pBuffPacket) << (uint8)1;   //网络字序
		}

	}

	return true;
}

bool CConsoleMessage::DoMessage_ReConnectServer( _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket )
{
	int nSerevrID = 0;
	if(GetConnectServerID(CommandInfo.m_szCommandExp, nSerevrID) == true)
	{
		char szIP[MAX_BUFF_100] = {'\0'};
#ifdef WIN32  //如果是windows
		App_ClientProConnectManager::instance()->ReConnect(nSerevrID);

		//获得当前连接状态
		vecClientConnectInfo VecClientConnectInfo;
		App_ClientProConnectManager::instance()->GetConnectInfo(VecClientConnectInfo);
		(*pBuffPacket) << (uint32)VecClientConnectInfo.size();
		for(int i = 0; i < (int)VecClientConnectInfo.size(); i++)
		{
			_ClientConnectInfo ClientConnectInfo = VecClientConnectInfo[i];

			sprintf_safe(szIP, MAX_BUFF_100, "%s:%d", ClientConnectInfo.m_addrRemote.get_host_addr(), ClientConnectInfo.m_addrRemote.get_port_number());

			VCHARS_STR strSName;
			strSName.text  = szIP;
			strSName.u1Len = (uint8)ACE_OS::strlen(szIP);

			if(ClientConnectInfo.m_blValid == true)
			{
				(*pBuffPacket) << (uint8)0;   //链接已存在
			}
			else
			{
				(*pBuffPacket) << (uint8)1;   //连接不存在
			}

			(*pBuffPacket) << strSName;
			(*pBuffPacket) << ClientConnectInfo.m_u4ConnectID;
			(*pBuffPacket) << ClientConnectInfo.m_u4RecvCount;
			(*pBuffPacket) << ClientConnectInfo.m_u4SendCount;
			(*pBuffPacket) << ClientConnectInfo.m_u4AllRecvSize;
			(*pBuffPacket) << ClientConnectInfo.m_u4AllSendSize;
			(*pBuffPacket) << ClientConnectInfo.m_u4BeginTime;
			(*pBuffPacket) << ClientConnectInfo.m_u4AliveTime;
			(*pBuffPacket) << ClientConnectInfo.m_u4RecvQueueCount;
			(*pBuffPacket) << ClientConnectInfo.m_u8RecvQueueTimeCost;
			(*pBuffPacket) << ClientConnectInfo.m_u8SendQueueTimeCost;
		}
#else   //如果是linux
		App_ClientReConnectManager::instance()->ReConnect(nSerevrID);

		//获得当前连接状态
		vecClientConnectInfo VecClientConnectInfo;
		App_ClientReConnectManager::instance()->GetConnectInfo(VecClientConnectInfo);
		(*pBuffPacket) << (uint8)VecClientConnectInfo.size();
		for(int i = 0; i < (int)VecClientConnectInfo.size(); i++)
		{
			_ClientConnectInfo ClientConnectInfo = VecClientConnectInfo[i];

			sprintf_safe(szIP, MAX_BUFF_100, "%s:%d", ClientConnectInfo.m_addrRemote.get_host_addr(), ClientConnectInfo.m_addrRemote.get_port_number());

			VCHARS_STR strSName;
			strSName.text  = szIP;
			strSName.u1Len = (uint8)ACE_OS::strlen(szIP);

			if(ClientConnectInfo.m_blValid == true)
			{
				(*pBuffPacket) << (uint8)0;   //链接已存在
			}
			else
			{
				(*pBuffPacket) << (uint8)1;   //连接不存在
			}

			(*pBuffPacket) << strSName;
			(*pBuffPacket) << ClientConnectInfo.m_u4ConnectID;
			(*pBuffPacket) << ClientConnectInfo.m_u4RecvCount;
			(*pBuffPacket) << ClientConnectInfo.m_u4SendCount;
			(*pBuffPacket) << ClientConnectInfo.m_u4AllRecvSize;
			(*pBuffPacket) << ClientConnectInfo.m_u4AllSendSize;
			(*pBuffPacket) << ClientConnectInfo.m_u4BeginTime;
			(*pBuffPacket) << ClientConnectInfo.m_u4AliveTime;
			(*pBuffPacket) << ClientConnectInfo.m_u4RecvQueueCount;
			(*pBuffPacket) << ClientConnectInfo.m_u8RecvQueueTimeCost;
			(*pBuffPacket) << ClientConnectInfo.m_u8SendQueueTimeCost;
		}
#endif
	}

	return true;
}

bool CConsoleMessage::DoMessage_CommandTimeout(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket)
{
	if(ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
	{
		uint32 u4Count = App_CommandAccount::instance()->GetTimeoutCount();
		(*pBuffPacket) << u4Count;

		for(uint32 i = 0; i < u4Count; i++)
		{
			_CommandTimeOut* pCommandTimeOut = App_CommandAccount::instance()->GetTimeoutInfo(i);
			if(NULL != pCommandTimeOut)
			{
				(*pBuffPacket) << pCommandTimeOut->m_u2CommandID;
				(*pBuffPacket) << (uint32)pCommandTimeOut->m_tvTime.sec();
				(*pBuffPacket) << (uint32)pCommandTimeOut->m_u4TimeOutTime;
			}
		}
	}

	return true;
}

bool CConsoleMessage::DoMessage_CommandTimeoutclr(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket)
{
	if(ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
	{
		App_CommandAccount::instance()->ClearTimeOut();
		(*pBuffPacket) << (uint8)0;	
	}

	return true;
}

bool CConsoleMessage::DoMessage_CommandDataLog(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket)
{
	if(ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
	{
		bool blState = App_CommandAccount::instance()->SaveCommandDataLog();
		if(blState == false)
		{
			(*pBuffPacket) << (uint8)1;	
		}
		else
		{
			(*pBuffPacket) << (uint8)0;	
		}
	}

	return true;
}

bool CConsoleMessage::DoMessage_SetDebug( _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket )
{
	uint8 u1Debug = 0;
	if(GetDebug(CommandInfo.m_szCommandExp, u1Debug) == true)
	{
		if(u1Debug == DEBUG_OFF)
		{
			App_MainConfig::instance()->SetDebug(DEBUG_OFF);
		}
		else
		{
			App_MainConfig::instance()->SetDebug(DEBUG_ON);
		}

		(*pBuffPacket) << (uint8)0;	
	}

	return true;
}

bool CConsoleMessage::DoMessage_ShowDebug( _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket )
{
	if(ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
	{
		(*pBuffPacket) << (uint8)App_MainConfig::instance()->GetDebug();

		(*pBuffPacket) << (uint8)0;	
	}

	return true;
}

bool CConsoleMessage::SetConsoleKey(vecConsoleKey* pvecConsoleKey)
{
	m_pvecConsoleKey = pvecConsoleKey;
	return true;
}

bool CConsoleMessage::CheckConsoleKey( const char* pKey )
{
	if(NULL == m_pvecConsoleKey)
	{
		return false;
	}

	for(int i = 0; i < (int)m_pvecConsoleKey->size(); i++)
	{
		if(ACE_OS::strcmp((*m_pvecConsoleKey)[i].m_szKey, pKey) == 0)
		{
			//key值对上了
			return true;
		}
	}

	return false;
}

bool CConsoleMessage::GetDebug(const char* pCommand, uint8& u1Debug)
{
	char szTempData[MAX_BUFF_100] = {'\0'};

	int nCommandLen = ACE_OS::strlen(pCommand);
	//获得IP地址
	char* pPosBegin = (char* )ACE_OS::strstr(pCommand, "-s ");
	int nLen = (int)(nCommandLen - (pPosBegin - pCommand) - 3);
	if(nLen >= MAX_BUFF_100 || nLen < 0)
	{
		return false;
	}
	ACE_OS::memcpy(szTempData, pPosBegin + 3, nLen);

	u1Debug = (uint8)ACE_OS::atoi(szTempData);

	return true;
}

bool CConsoleMessage::DoMessage_SetTrackIP(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket)
{
	_ForbiddenIP ForbiddenIP;
	if(GetTrackIP(CommandInfo.m_szCommandExp, ForbiddenIP) == true)
	{
		App_IPAccount::instance()->SetTrackIP(ForbiddenIP.m_szClientIP);

		(*pBuffPacket) << (uint8)0;   //追踪成功
	}
	else
	{
		(*pBuffPacket) << (uint8)1;   //追踪失败
	}

	return true;
}

bool CConsoleMessage::DoMessage_DelTrackIP(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket)
{
	if(ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
	{
		//清除追踪
		App_IPAccount::instance()->ClearTrackIP();

		(*pBuffPacket) << (uint8)0;	
	}

	return true;
}

bool CConsoleMessage::DoMessage_GetTrackIPInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket)
{
	char szTimeBegin[MAX_BUFF_100] = {'\0'};
	char szTimeEnd[MAX_BUFF_100]   = {'\0'};

	if(ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
	{
		//清除追踪
		int nCount = App_IPAccount::instance()->GetTrackIPInfoCount();
		_IPTrackInfo* pIPTrackInfo = App_IPAccount::instance()->GetBase();

		//寻找所有合法的历史记录
		uint16 u2HistoryCount = 0;
		for(int i = 0; i < nCount; i++)
		{
			_IPTrackInfo* pCurrInfo = pIPTrackInfo + i;
			if(ACE_OS::strlen(pCurrInfo->m_szClientIP) != 0)
			{
				u2HistoryCount++;
			}
		}

		//记录总个数
		(*pBuffPacket) << (uint16)u2HistoryCount;	

		//传入历史记录
		for(int i = 0; i < nCount; i++)
		{
			_IPTrackInfo* pCurrInfo = pIPTrackInfo + i;
			if(ACE_OS::strlen(pCurrInfo->m_szClientIP) != 0)
			{
				VCHARS_STR strSName;
				strSName.text  = pCurrInfo->m_szClientIP;
				strSName.u1Len = (uint8)ACE_OS::strlen(pCurrInfo->m_szClientIP);

				(*pBuffPacket) << strSName;                    //IP
				(*pBuffPacket) << (uint32)pCurrInfo->m_nPort;  //端口

				sprintf_safe(szTimeBegin, MAX_BUFF_100, "%04d-%02d-%02d %02d:%02d:%02d", pCurrInfo->m_dtConnectStart.year(), pCurrInfo->m_dtConnectStart.month(), pCurrInfo->m_dtConnectStart.day(), pCurrInfo->m_dtConnectStart.hour(), pCurrInfo->m_dtConnectStart.minute(), pCurrInfo->m_dtConnectStart.second());

				strSName.text  = szTimeBegin;
				strSName.u1Len = (uint8)ACE_OS::strlen(szTimeBegin);
				(*pBuffPacket) << strSName;

				sprintf_safe(szTimeEnd, MAX_BUFF_100, "%04d-%02d-%02d %02d:%02d:%02d", pCurrInfo->m_dtConnectEnd.year(), pCurrInfo->m_dtConnectEnd.month(), pCurrInfo->m_dtConnectEnd.day(), pCurrInfo->m_dtConnectEnd.hour(), pCurrInfo->m_dtConnectEnd.minute(), pCurrInfo->m_dtConnectEnd.second());

				strSName.text  = szTimeEnd;
				strSName.u1Len = (uint8)ACE_OS::strlen(szTimeBegin);
				(*pBuffPacket) << strSName;

				(*pBuffPacket) << (uint32)pCurrInfo->m_u4RecvByteSize;  //接收字节数
				(*pBuffPacket) << (uint32)pCurrInfo->m_u4SendByteSize;  //发送字节数
				(*pBuffPacket) << (uint8)pCurrInfo->m_u1State;          //状态位 

			}
		}
	}

	return true;
}

bool CConsoleMessage::DoMessage_GetConnectIPInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket)
{
	int nConnectID = 0;
	if(GetConnectServerID(CommandInfo.m_szCommandExp, nConnectID) == true)
	{
#ifdef WIN32  //如果是windows
		_ClientIPInfo objClientIPInfo = App_ProConnectManager::instance()->GetClientIPInfo((uint32)nConnectID);
#else
		_ClientIPInfo objClientIPInfo = App_ConnectManager::instance()->GetClientIPInfo((uint32)nConnectID);
#endif


		if(ACE_OS::strlen(objClientIPInfo.m_szClientIP) == 0)
		{
			//没有找到对应的IP信息
			(*pBuffPacket) << (uint16)1;
		}
		else
		{
			//找到了对应的IP信息
			(*pBuffPacket) << (uint16)0;

			VCHARS_STR strSName;
			strSName.text  = objClientIPInfo.m_szClientIP;
			strSName.u1Len = (uint8)ACE_OS::strlen(objClientIPInfo.m_szClientIP);

			(*pBuffPacket) << strSName;                         //IP
			(*pBuffPacket) << (uint32)objClientIPInfo.m_nPort;  //端口
		}
	}

	return true;
}

bool CConsoleMessage::DoMessage_GetLogLevelInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket)
{
	if(ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
	{
		(*pBuffPacket) << AppLogManager::instance()->GetLogCount();
		(*pBuffPacket) << AppLogManager::instance()->GetCurrLevel();

		for(uint16 i = 0; i < (uint16)AppLogManager::instance()->GetLogCount(); i++)
		{
			uint16 u2LogID = AppLogManager::instance()->GetLogID(i);

			(*pBuffPacket) << u2LogID;

			char* pServerName = AppLogManager::instance()->GetLogInfoByServerName(u2LogID);
			if(NULL == pServerName)
			{
				//如果服务器名称为空则直接返回
				return true;
			}

			VCHARS_STR strSName;
			strSName.text  = pServerName;
			strSName.u1Len = (uint8)ACE_OS::strlen(pServerName);

			(*pBuffPacket) << strSName;

			char* pLogName = AppLogManager::instance()->GetLogInfoByLogName(u2LogID);
			if(NULL == pLogName)
			{
				//如果服务器名称为空则直接返回
				return true;
			}

			strSName.text  = pLogName;
			strSName.u1Len = (uint8)ACE_OS::strlen(pLogName);

			(*pBuffPacket) << strSName;

			uint8 u1LogType = (uint8)AppLogManager::instance()->GetLogInfoByLogDisplay(u2LogID);

			(*pBuffPacket) << u1LogType;
		}
	}

	return true;
}

bool CConsoleMessage::DoMessage_SetLogLevelInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket)
{
	int nLogLevel = 1;
	if(GetLogLevel(CommandInfo.m_szCommandExp, nLogLevel) == true)
	{
		AppLogManager::instance()->ResetLogData(nLogLevel);

		(*pBuffPacket) << (uint32)0;
	}

	return true;
}

bool CConsoleMessage::DoMessage_GetThreadAI(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket)
{
	if(ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
	{
		vecWorkThreadAIInfo objvecWorkThreadAIInfo;
		App_MessageServiceGroup::instance()->GetWorkThreadAIInfo(objvecWorkThreadAIInfo);

		uint16 u2ThreadCount = (uint16)objvecWorkThreadAIInfo.size();
		(*pBuffPacket) << (uint16)u2ThreadCount;

		for(uint16 i = 0; i < u2ThreadCount; i++)
		{
			(*pBuffPacket) << objvecWorkThreadAIInfo[i].m_u4ThreadID;
			(*pBuffPacket) << objvecWorkThreadAIInfo[i].m_u1WTAI;
			(*pBuffPacket) << objvecWorkThreadAIInfo[i].m_u4DisposeTime;
			(*pBuffPacket) << objvecWorkThreadAIInfo[i].m_u4WTCheckTime;
			(*pBuffPacket) << objvecWorkThreadAIInfo[i].m_u4WTTimeoutCount;
			(*pBuffPacket) << objvecWorkThreadAIInfo[i].m_u4WTStopTime;
		}
	}

	return true;
}

bool CConsoleMessage::DoMessage_GetWorkThreadTO(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket)
{
	if(ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
	{
		vecCommandTimeout objTimeout;
		vecCommandTimeout objTimeoutF;
		App_MessageServiceGroup::instance()->GetAITO(objTimeout);

		uint16 u2ThreadCount = (uint16)objTimeout.size();
		(*pBuffPacket) << (uint16)u2ThreadCount;

		for(uint16 i = 0; i < u2ThreadCount; i++)
		{
			(*pBuffPacket) << objTimeout[i].m_u4ThreadID;
			(*pBuffPacket) << objTimeout[i].m_u2CommandID;
			(*pBuffPacket) << objTimeout[i].m_u4Second;
			(*pBuffPacket) << objTimeout[i].m_u4Timeout;
		}

		App_MessageServiceGroup::instance()->GetAITF(objTimeoutF);

		u2ThreadCount = (uint16)objTimeoutF.size();
		(*pBuffPacket) << (uint16)u2ThreadCount;

		for(uint16 i = 0; i < u2ThreadCount; i++)
		{
			(*pBuffPacket) << objTimeoutF[i].m_u4ThreadID;
			(*pBuffPacket) << objTimeoutF[i].m_u2CommandID;
			(*pBuffPacket) << objTimeoutF[i].m_u4Second;
		}
	}

	return true;
}

bool CConsoleMessage::DoMessage_SetWorkThreadAI(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket)
{
	int nAI      = 0;
	int nDispose = 0;
	int nCheck   = 0;
	int nStop    = 0;
	if(GetAIInfo(CommandInfo.m_szCommandExp, nAI, nDispose, nCheck, nStop) == true)
	{
		//规范化数据
		if(nDispose > 0 && nCheck > 0 && nStop > 0)
		{
			if(nAI > 0)
			{
				nAI = 1;
			}
			else
			{
				nAI = 0;
			}

			App_MessageServiceGroup::instance()->SetAI((uint8)nAI, (uint32)nDispose, (uint32)nCheck, (uint32)nStop);
		}
	}

	(*pBuffPacket) << (uint32)0;
	return true;
}

bool CConsoleMessage::DoMessage_GetNickNameInfo( _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket )
{
	char szNickName[MAX_BUFF_100] = {'\0'};
	vecClientNameInfo objClientNameInfo;

	if(GetNickName(CommandInfo.m_szCommandExp, szNickName) == true)
	{
#ifdef WIN32
		App_ProConnectManager::instance()->GetClientNameInfo(szNickName, objClientNameInfo);
#else
		App_ConnectManager::instance()->GetClientNameInfo(szNickName, objClientNameInfo);
#endif

		//返回信息列表
		(*pBuffPacket) << (uint32)objClientNameInfo.size();

		for(uint32 i = 0; i < objClientNameInfo.size(); i++)
		{
			VCHARS_STR strIP;
			strIP.text  = objClientNameInfo[i].m_szClientIP;
			strIP.u1Len = (uint8)ACE_OS::strlen(objClientNameInfo[i].m_szClientIP);

			VCHARS_STR strName;
			strName.text  = objClientNameInfo[i].m_szName;
			strName.u1Len = (uint8)ACE_OS::strlen(objClientNameInfo[i].m_szName);

			(*pBuffPacket) << (uint32)objClientNameInfo[i].m_nConnectID;
			(*pBuffPacket) << strIP;
			(*pBuffPacket) << (uint32)objClientNameInfo[i].m_nPort;
			(*pBuffPacket) << strName;
			(*pBuffPacket) << (uint8)objClientNameInfo[i].m_nLog;
		}
	}

	return true;
}

bool CConsoleMessage::DoMessage_SetConnectLog( _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket )
{
	uint32 u4ConnectID = 0;
	bool blIsLog       = false;
	if(GetConnectID(CommandInfo.m_szCommandExp, u4ConnectID, blIsLog) == true)
	{
#ifdef WIN32
		App_ProConnectManager::instance()->SetIsLog(u4ConnectID, blIsLog);
#else
		App_ConnectManager::instance()->SetIsLog(u4ConnectID, blIsLog);
#endif
	}

	(*pBuffPacket) << (uint32)0;

	return true;
}

bool CConsoleMessage::DoMessage_SetMaxConnectCount( _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket )
{
	uint16 u2MaxConnectHandler = 0;
	if(GetMaxConnectCount(CommandInfo.m_szCommandExp, u2MaxConnectHandler) == true)
	{
		if(u2MaxConnectHandler > 0)
		{
			App_MainConfig::instance()->SetMaxHandlerCount(u2MaxConnectHandler);
		}
	}

	(*pBuffPacket) << (uint32)0;

	return true;
}
