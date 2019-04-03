//#include "StdAfx.h"
#include "TpClientManager.h"

CTpClientManager* CTpClientManager::m_pTpClientManager    = NULL;


//¶¨Ê±Æ÷µ½´ï»Øµ÷ÊÂ¼þ

int  Recv_thread( )
{
	//±éÀúÑ­»·ËùÓÐµÄ¼¤»îÁ¬½Ó£¬Èç¹ûÓÐÊý¾Ý£¬ÔòÊÕµ½
	while(true)
	{
		CTpClientManager::Instance().Recv_All_Data();

		int dwMilliseconds = TCP_RECV_SLEEP;
		std::this_thread::sleep_for(std::chrono::milliseconds(dwMilliseconds));
	}
}

int  Timer_thread( )
{
	while(true)
	{
        CTpClientManager::Instance().TPClient_HandleEvents((int)TIMER_EVENT_ID);

		int dwMilliseconds = TIMER_EVENT_INTERVAL;
		std::this_thread::sleep_for(std::chrono::milliseconds(dwMilliseconds));
		printf_s("[Timer Thread]Timer is End.\n");

	}

	return 0;
}



CTpClientManager::CTpClientManager(void)
{
}

CTpClientManager::~CTpClientManager(void)
{
	TPClient_DisConnect_All();
	FreeSocketEnvironment();
}

int CTpClientManager::TPClient_Connect( int nServerID, const char* pIP, int nPort, CRecvData* pRecvData /*= NULL*/ )
{
	//¼ì²éµ±Ç°Á¬½ÓÊÇ·ñÑ°ÔÚ£¬´æÔÚÔò·µ»Ø´íÎó
	if(IsFindTpClient(nServerID) == true)
	{
		return FUNC_FAIL_EXIST;
	}

	//Èç¹û²»´æÔÚ£¬ÔòÌí¼Ó½øÁÐ±í
	_TpClientInfo objTpClientInfo;
	objTpClientInfo.m_nServerID  = nServerID;
	sprintf_s(objTpClientInfo.m_szTpIP, MAX_BUFF_50, "%s", pIP);
	objTpClientInfo.m_nPort      = nPort;
	objTpClientInfo.m_pRecvData  = pRecvData;

	ConnectToServer(objTpClientInfo);

	m_vecTpClientInfo.push_back(objTpClientInfo);

	return FUNC_SUCCESS;
}

int CTpClientManager::TPClient_DisConnect( int nServerID )
{
	if(DelTcpClient(nServerID) == true)
	{
		return FUNC_SUCCESS;
	}
	else
	{
		return FUNC_FAIL_NO_EXIST;
	}
}


int CTpClientManager::TPClient_Send( int nServerID, const char* pBuff, int nLen )
{
	bool blSendFlag   = false;
	_TpClientInfo* pTpClientInfo = IsGetTpClient(nServerID);
	if(NULL != pTpClientInfo)
	{
		//¿´¿´Á¬½ÓÊÇ·ñ´æÔÚ£¬Èç¹û²»´æÔÚÔò×Ô¶¯ÖØÁ·
		if(pTpClientInfo->m_nState == 0)
		{
			ConnectToServer((*pTpClientInfo));
		}

		int nTotalSendLen = nLen;
		int nBeginSend    = 0;
		int nCurrSendLen  = 0;

		//¿´¿´Á¬½ÓÊÇ·ñ½¨Á¢³É¹¦
		if(pTpClientInfo->m_nState == 1 && pTpClientInfo->m_sckClient != INVALID_SOCKET)
		{
			while(true)
			{
				//nCurrSendLen = send(pTpClientInfo->m_sckClient, pBuff + nBeginSend, nTotalSendLen, 0);
		        transresult_t   rt;
                SocketSend(pTpClientInfo->m_sckClient, pBuff + nBeginSend, nTotalSendLen, rt);
				nCurrSendLen    =rt.nbytes;

				if(nCurrSendLen <= 0)
				{
					//closesocket(pTpClientInfo->m_sckClient);
					SocketClose(pTpClientInfo->m_sckClient);

					pTpClientInfo->m_nState    = 0;
					pTpClientInfo->m_sckClient = INVALID_SOCKET;
					break;
				}
				else
				{
					nTotalSendLen -= nCurrSendLen;
					if(nTotalSendLen == 0)
					{
						//·¢ËÍÍê³É
						blSendFlag = true;
						break;
					}
					else
					{
						nBeginSend += nCurrSendLen;
					}
				}
			}
		}
	}

	if(blSendFlag == true)
	{
		return FUNC_SUCCESS;
	}
	else
	{
		return FUNC_SEND_ERROR;
	}
}


int CTpClientManager::TPClient_ConnectState( int nServerID )
{
	_TpClientInfo* pTpClientInfo = IsGetTpClient(nServerID);
	if(pTpClientInfo == NULL)
	{
		return FUNC_FAIL_NO_EXIST;
	}
	else
	{
		return pTpClientInfo->m_nState;
	}
}

int CTpClientManager::TPClient_ReConnect( int nServerID )
{
	return FUNC_SUCCESS;
}

int CTpClientManager::TPClient_DisConnect_All()
{
	DelTcpClient_All();
	return FUNC_SUCCESS;
}

bool CTpClientManager::IsFindTpClient( int nServerID )
{
	for(int i = 0; i < (int)m_vecTpClientInfo.size(); i++)
	{
		if(m_vecTpClientInfo[i].m_nServerID == nServerID)
		{
			return true;
		}
	}

	return false;
}

_TpClientInfo* CTpClientManager::IsGetTpClient( int nServerID )
{
	for(int i = 0; i < (int)m_vecTpClientInfo.size(); i++)
	{
		if(m_vecTpClientInfo[i].m_nServerID == nServerID)
		{
			return (_TpClientInfo* )&m_vecTpClientInfo[i];
		}
	}

	return NULL;
}

bool CTpClientManager::DelTcpClient( int nServerID )
{
	for(vecTpClientInfo::iterator b = m_vecTpClientInfo.begin(); b != m_vecTpClientInfo.end(); b++)
	{
		if(((_TpClientInfo)(*b)).m_nServerID == nServerID)
		{
			//ÅÐ¶ÏÊÇ·ñÐèÒª¶Ï¿ªÁ¬½Ó
			if(((_TpClientInfo)(*b)).m_nState == 1 && ((_TpClientInfo)(*b)).m_sckClient != INVALID_SOCKET)
			{
				//closesocket(((_TpClientInfo)(*b)).m_sckClient);
                HSocket sckclent    =((_TpClientInfo)(*b)).m_sckClient;
                SocketClose(sckclent);

				if(m_blDebug)
				{
					printf_s("[CTpClientManager::DelTcpClient]close ServerID=%d socket.\n", ((_TpClientInfo)(*b)).m_nServerID);
				}
			}

			m_vecTpClientInfo.erase(b);
			return true;
		}
	}

	return false;
}

bool CTpClientManager::DelTcpClient_All()
{
	//Ñ­»·ÅÐ¶¨ÊÇ·ñÒª¹Ø±ÕÒÑÓÐÁ¬½Ó
	for(vecTpClientInfo::iterator b = m_vecTpClientInfo.begin(); b != m_vecTpClientInfo.end(); b++)
	{
		//ÅÐ¶ÏÊÇ·ñÐèÒª¶Ï¿ªÁ¬½Ó
		if(((_TpClientInfo)(*b)).m_nState == 1 && ((_TpClientInfo)(*b)).m_sckClient != INVALID_SOCKET)
		{
            HSocket sckclent    =((_TpClientInfo)(*b)).m_sckClient;
			//closesocket(((_TpClientInfo)(*b)).m_sckClient);
			SocketClose(sckclent);

			if(m_blDebug)
			{
				printf_s("[CTpClientManager::DelTcpClient_All]close ServerID=%d socket\n.", ((_TpClientInfo)(*b)).m_nServerID);
			}
		}
	}

	m_vecTpClientInfo.clear();
	return true;
}

int CTpClientManager::TPClient_HandleEvents( int nEventsID )
{
	if(m_blDebug)
	{
		printf_s("[CTpClientManager::TPClient_HandleEvents]nEventsID=%d is happen.\n", nEventsID);
	}

	Connect_All_Server();
	return 0;
}

int CTpClientManager::TPClient_Run(bool blIsDebug)
{

	//´´½¨Ò»¸öÏß³Ì¶¨Ê±ÔËÐÐ
    //std::thread hThreadTimer(&Timer_thread);
    //hThreadTimer.detach();

	//´´½¨Ò»¸öÏß³Ì½ÓÊÕËùÓÐÊý¾Ý
    //std::thread hThreadRecv(&Recv_thread);
    //hThreadRecv.detach();

    auto hThreadTimer = std::make_shared<std::thread>(&Timer_thread);
    hThreadTimer->detach();    
    
    auto hThreadRecv = std::make_shared<std::thread>(&Recv_thread);
    hThreadRecv->detach();

	return 0;
}

void CTpClientManager::SetDebug( bool blDebug )
{
	m_blDebug = blDebug;
}

int CTpClientManager::ConnectToServer( _TpClientInfo& objTpClientInfo )
{
	//socket´´½¨µÄ×¼±¸¹¤×÷
	//ÉèÖÃ½ÓÊÕ³¬Ê±Ê±¼ä
	int TimeOut = TCP_RECV_TIMEOUT;

    struct sockaddr_in sockaddr;
	HSocket sckClient = SocketOpen(SOCK_STREAM);
	SocketTimeOut(sckClient, TimeOut, -1, -1);
	GetAddressFrom(&sockaddr,objTpClientInfo.m_szTpIP, objTpClientInfo.m_nPort);

	int nErr = SocketConnect(sckClient, &sockaddr);
	if(0 != nErr)
	{
		if(m_blDebug)
		{
			printf_s("[CTpClientManager::ConnectToServer]connect error(%d).\n", GetLastSocketError());
			objTpClientInfo.m_nState    = 0;
			objTpClientInfo.m_sckClient = INVALID_SOCKET;
		}
	}
	else
	{
		bool ul = true;//falseÎª·Ç×èÈû,trueÎª×èÈû
		//ioctlsocket(sckClient, FIONBIO, (unsigned long*)&ul);
		SocketBlock(sckClient, ul);

		if(m_blDebug)
		{
			printf_s("[CTpClientManager::ConnectToServer][%d]connect OK(%s:%d).\n", objTpClientInfo.m_nServerID, objTpClientInfo.m_szTpIP, objTpClientInfo.m_nPort);
		}
		objTpClientInfo.m_sckClient = sckClient;
		objTpClientInfo.m_nState    = 1;
	}

	return 0;
}

void CTpClientManager::Recv_All_Data()
{
	if(m_blDebug)
	{
		printf_s("[CTpClientManager::Recv_All_Data]is Begin.\n");
	}

	for(int i = 0; i <(int)m_vecTpClientInfo.size(); i++)
	{
		if(m_vecTpClientInfo[i].m_nState == 1 && m_vecTpClientInfo[i].m_sckClient != INVALID_SOCKET)
		{
			char szBuff[MAX_BUFF_1024] = {'\0'};

			transresult_t   rt;
            SocketRecv(m_vecTpClientInfo[i].m_sckClient, (char* )szBuff, MAX_BUFF_1024, rt);
            int nCurrRecvLen    =rt.nbytes;

			if(nCurrRecvLen <= 0)
			{
				int nError = GetLastSocketError();

				//ÒòÎªÊÇ·Ç×èÈû£¬ËùÒÔÈç¹ûÊÇ10035ÔòºöÂÔ
				if((nError != 0)  ||(nError != 10035)  )
				{
					if(m_blDebug)
					{
						printf_s("[CTpClientManager::Recv_All_Data][%d](%s:%d) is close£¨%d£©.\n", m_vecTpClientInfo[i].m_nServerID, m_vecTpClientInfo[i].m_szTpIP, m_vecTpClientInfo[i].m_nPort, nError);
					}

					//closesocket(m_vecTpClientInfo[i].m_sckClient);
					SocketClose(m_vecTpClientInfo[i].m_sckClient);

					m_vecTpClientInfo[i].m_sckClient = INVALID_SOCKET;
					m_vecTpClientInfo[i].m_nState    = 0;

				}
				continue;
			}

			if(m_vecTpClientInfo[i].m_pRecvData != NULL)
			{
				//´¦Àí½ÓÊÕµ½µÄÊý¾Ý
				m_vecTpClientInfo[i].m_pRecvData->RecvData(szBuff, nCurrRecvLen);
			}
		}

	}
}

void CTpClientManager::Connect_All_Server()
{
	for(int i = 0; i < (int)m_vecTpClientInfo.size(); i++)
	{
		//¿´¿´Á¬½ÓÊÇ·ñ´æÔÚ£¬Èç¹û²»´æÔÚÔò×Ô¶¯ÖØÁ·
		if(m_vecTpClientInfo[i].m_nState == 0)
		{
			if(m_blDebug)
			{
				printf_s("[CTpClientManager::Connect_All_Server](%d)[%s:%d] is reconnect.\n", m_vecTpClientInfo[i].m_nServerID, m_vecTpClientInfo[i].m_szTpIP, m_vecTpClientInfo[i].m_nPort);
			}

			ConnectToServer(m_vecTpClientInfo[i]);
		}
	}

}
