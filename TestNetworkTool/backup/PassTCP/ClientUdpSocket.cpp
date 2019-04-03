#include "StdAfx.h"
#include "ClientUdpSocket.h"

CClientUdpSocket::CClientUdpSocket(void)
{
	m_pSocket_Info       = NULL;
	m_pSocket_State_Info = NULL;
	m_blRun              = false;
}

CClientUdpSocket::~CClientUdpSocket(void)
{
  Close();
}

void CClientUdpSocket::Close()
{
	if(NULL != m_pSocket_Info)
	{
		delete m_pSocket_Info;
		m_pSocket_Info = NULL;
	}

	if(NULL != m_pSocket_State_Info)
	{
		delete m_pSocket_State_Info;
		m_pSocket_State_Info = NULL;
	}
}

void CClientUdpSocket::Run()
{
	int nPacketCount = 1;
	m_blRun = true;
	SOCKET sckClient;
	SOCKET sckServer;

	//此部分为兼容Lua参数而设计
	//为了减少不必要的new和delete操作，所以参数在这里先声明好
	_ParamData* pSendParam1   = NULL;
	_ParamData* pSendParam2   = NULL;
	_ParamData* pSendParam3   = NULL;
	_ParamData* pSendParam4   = NULL;
	_ParamData* pSendParamOut = NULL;
	_ParamData* pRecvParam1   = NULL;
	_ParamData* pRecvParam2   = NULL;
	_ParamData* pRecvParam3   = NULL;
	_ParamData* pRecvParam4   = NULL;
	_ParamData* pRecvParamOut = NULL;

	int nLuaBufferMaxLength = m_pSocket_Info->m_pLogic->GetSendLength();

	if(m_pSocket_Info == NULL || m_pSocket_State_Info == NULL)
	{
		m_blRun = false;
		return;
	}

	//如果是高级模式
	if(m_pSocket_Info->m_blLuaAdvance == true)
	{
		m_objLuaFn.InitClass();

		bool blState = m_objLuaFn.LoadLuaFile(m_pSocket_Info->m_szLuaFileName);
		if(false == blState)
		{
			printf_s("[Main]Open Lua file error.\n");
			return;
		}

		//初始化所有要使用的Lua参数
		pSendParam1   = new _ParamData();
		pSendParam2   = new _ParamData();
		pSendParam3   = new _ParamData();
		pSendParam4   = new _ParamData();
		pSendParamOut = new _ParamData();
		pRecvParam1   = new _ParamData();
		pRecvParam2   = new _ParamData();
		pRecvParam3   = new _ParamData();
		pRecvParam4   = new _ParamData();
		pRecvParamOut = new _ParamData(); 

	}

	//看看是否是长连接，如果是长连接，则只处理一次。
	bool blIsConnect = false;

	//socket创建的准备工作
	struct sockaddr_in svrsockaddr;

	memset(&svrsockaddr, 0, sizeof(SOCKADDR_IN));
	svrsockaddr.sin_family = AF_INET;
	svrsockaddr.sin_port   = htons(m_pSocket_Info->m_nPort);
	svrsockaddr.sin_addr.S_un.S_addr = inet_addr(m_pSocket_Info->m_szSerevrIP);

	sckClient = socket(AF_INET, SOCK_DGRAM, 0);

	DWORD TimeOut = (DWORD)m_pSocket_Info->m_nRecvTimeout;
	::setsockopt(sckClient, SOL_SOCKET, SO_RCVTIMEO, (char *)&TimeOut, sizeof(TimeOut));

	//设置接收监听端口
	sckServer = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in clientsockaddr;
	memset(&clientsockaddr, 0, sizeof(SOCKADDR_IN));
	clientsockaddr.sin_family = AF_INET;
	clientsockaddr.sin_port   = htons(m_pSocket_Info->m_nUdpClientPort);
	clientsockaddr.sin_addr.S_un.S_addr = inet_addr(m_pSocket_Info->m_szSerevrIP);

	bind(sckServer, (SOCKADDR *) &clientsockaddr, sizeof(clientsockaddr));

	//发送次数
	int nSendIndex = 0;

	while(m_blRun)
	{
		unsigned int tBegin = (unsigned int)GetTickCount();
		if(m_pSocket_Info->m_nSendCount != 0 && m_pSocket_State_Info->m_nSuccessSend >= m_pSocket_Info->m_nSendCount)
		{
			//发送指定数目的数据包完成
			break;
		}

		//查看是否开启高级模式
		if(m_pSocket_Info->m_blLuaAdvance == true)
		{
			//重置缓冲最大长度
			m_pSocket_Info->m_pLogic->SetMaxSendLength(nLuaBufferMaxLength);

			//开始调用Lua脚本，去组织数据块
			CParamGroup objIn;
			CParamGroup objOut;

			objIn.NeedRetrieve(false);
			objOut.NeedRetrieve(false);

			int nLuaSendLen = m_pSocket_Info->m_pLogic->GetSendLength();
			pSendParam1->SetParam((char* )m_pSocket_Info->m_pLogic->GetSendData(), "void", sizeof(int));
			pSendParam2->SetParam((char* )&nLuaSendLen, "int", sizeof(int));
			pSendParam3->SetParam((char* )&m_nThreadID, "int", sizeof(int));
			pSendParam4->SetParam((char* )&nSendIndex, "int", sizeof(int));

			int nSendLength = 0;
			pSendParamOut->SetParam((char* )&nSendLength, "int", sizeof(int));

			objIn.Push(pSendParam1);
			objIn.Push(pSendParam2);
			objIn.Push(pSendParam3);
			objIn.Push(pSendParam4);
			objOut.Push(pSendParamOut);

			m_objLuaFn.CallFileFn("PassTcp_CreateSendData", objIn, objOut);

			int* pLength = (int* )pSendParamOut->GetParam();
			m_pSocket_Info->m_pLogic->SetMaxSendLength((int)(*pLength));
		}


		if(blIsConnect == false)
		{
			DWORD dwSleepTime = (DWORD)m_pSocket_Info->m_nDelaySecond;
			if(m_pSocket_Info->m_blIsRadomaDelay == true)
			{
				//如果是随机的，则从1-1000之间随机一个时间
				dwSleepTime = (DWORD)RandomValue(1, 1000);
			}

			//挂起指定的时间
			Sleep(dwSleepTime);

			m_pSocket_State_Info->m_nSuccessConnect++;
			m_pSocket_State_Info->m_nCurrectSocket = 1;
			blIsConnect = true;
		}

		if(blIsConnect == true)
		{
			//发送数据
			char szSendBuffData[MAX_BUFF_1024 * 100] = {'\0'};
			char szRecvBuffData[MAX_BUFF_1024 * 100] = {'\0'};

			char* pSendData   = NULL;
			int nSendLen      = 0;
			int nTotalRecvLen = 0;
			//如果数据为随机数据包
			if(m_pSocket_Info->m_blIsSendCount == true)
			{
				int nSendCount = RandomValue(1, 10);

				//这里追加一个逻辑，记录当前发包的总数是否匹配，随机数不能超过当前发包总数
				if(m_pSocket_Info->m_nSendCount != 0 && nSendCount + m_pSocket_State_Info->m_nSuccessSend > m_pSocket_Info->m_nSendCount)
				{
					nSendCount = m_pSocket_Info->m_nSendCount - m_pSocket_State_Info->m_nSuccessSend;
				}

				char* pData = m_pSocket_Info->m_pLogic->GetSendData(m_pSocket_Info->m_nThreadID, nSendIndex, nSendLen);
				for(int i = 0; i < nSendCount; i++)
				{
					MEMCOPY_SAFE(&szSendBuffData[i * nSendLen], 
						pData, 
						nSendLen);
				}
				nPacketCount = nSendCount;

				//发送数据
				pSendData     = (char* )szSendBuffData;
				nSendLen      = m_pSocket_Info->m_pLogic->GetSendLength() * nSendCount;
				nTotalRecvLen = m_pSocket_Info->m_pLogic->GetSendLength() * nSendCount;
			}
			else
			{
				//发送数据
				pSendData     = (char* )m_pSocket_Info->m_pLogic->GetSendData(m_pSocket_Info->m_nThreadID, nSendIndex, nSendLen);
				nTotalRecvLen = m_pSocket_Info->m_pLogic->GetRecvLength();
			}

			//记录应收字节总数
			int nRecvAllSize = nTotalRecvLen;

			//如果需要记录日志，则将数据计入日志
			if(m_pSocket_Info->m_blIsWriteFile == true)
			{
				WriteFile_SendBuff(pSendData, nSendLen);
			}

			int nTotalSendLen = nSendLen;
			int nBeginSend    = 0;
			int nCurrSendLen  = 0;
			bool blSendFlag   = false;
			int nBeginRecv    = 0;
			int nCurrRecvLen  = 0;
			bool blRecvFlag   = false;
			while(true)
			{
				//UDP发送
				int nssssSize = sizeof(SOCKADDR);
				nCurrSendLen = sendto(sckClient, pSendData + nBeginSend, nTotalSendLen, 0, (sockaddr* )&svrsockaddr, sizeof(SOCKADDR));
				if(nCurrSendLen <= 0)
				{
					DWORD dwError = GetLastError();
					WriteFile_Error("sendto error", (int)dwError);
					m_pSocket_State_Info->m_nFailSend += nPacketCount;
					m_pSocket_State_Info->m_nCurrectSocket = 0;
					blIsConnect = false;
					break;
				}
				else
				{
					nTotalSendLen -= nCurrSendLen;
					if(nTotalSendLen == 0)
					{
						//发送完成
						m_pSocket_State_Info->m_nSuccessSend += nPacketCount;
						blSendFlag = true;
						break;
					}
					else
					{
						nBeginSend += nCurrSendLen;
					}
				}
			}

			//接收数据
			if(blSendFlag == true && m_pSocket_Info->m_blIsRecv == true)
			{
				while(true)
				{
					//如果发送成功了，则处理接收数据
					int nClientAddrSize = sizeof(SOCKADDR_IN);

					//socket创建的准备工作
					struct sockaddr_in clientsockaddr1;
					/*
					struct sockaddr_in clientsockaddr;

					memset(&clientsockaddr, 0, sizeof(SOCKADDR_IN));
					clientsockaddr.sin_family = AF_INET;
					clientsockaddr.sin_port   = htons(20004);
					clientsockaddr.sin_addr.S_un.S_addr = inet_addr(m_pSocket_Info->m_szSerevrIP);
					*/

					nCurrRecvLen = recvfrom(sckServer, (char* )szRecvBuffData + nBeginRecv, nTotalRecvLen, 0, (sockaddr* )&clientsockaddr1, &nClientAddrSize);
					if(nCurrRecvLen <= 0)
					{
						DWORD dwError = GetLastError();
						WriteFile_Error("sendto error", (int)dwError);
						m_pSocket_State_Info->m_nFailRecv += nPacketCount;
						//closesocket(sckClient);
						m_pSocket_State_Info->m_nCurrectSocket = 0;
						blIsConnect = false;
						break;
					}
					else
					{
						//如果是高级模式，这里调用Lua接口方法
						if(m_pSocket_Info->m_blLuaAdvance == true)
						{
							m_pSocket_State_Info->m_nRecvByteCount += nCurrRecvLen;
							int nState = 0;

							CParamGroup objRecvIn;
							CParamGroup objRecvOut;

							objRecvIn.NeedRetrieve(false);
							objRecvOut.NeedRetrieve(false);

							pRecvParam1->SetParam((char* )szRecvBuffData, "void", sizeof(int));
							pRecvParam2->SetParam((char* )&nCurrRecvLen, "int", sizeof(int));
							pRecvParam3->SetParam((char* )&m_nThreadID, "int", sizeof(int));
							pRecvParam4->SetParam((char* )&nSendIndex, "int", sizeof(int));

							pRecvParamOut->SetParam((char* )&nState, "int", sizeof(int));

							objRecvIn.Push(pRecvParam1);
							objRecvIn.Push(pRecvParam2);
							objRecvIn.Push(pRecvParam3);
							objRecvIn.Push(pRecvParam4);
							objRecvOut.Push(pRecvParamOut);

							//调用接收函数
							m_objLuaFn.CallFileFn("PassTcp_GetRecvData", objRecvIn, objRecvOut);

							int* pReturn = (int* )pRecvParamOut->GetParam();
							nState = (int)(*pReturn);

							objRecvIn.Close(false);
							objRecvOut.Close(false);

							//判断脚本返回值
							if(nState == 0)
							{
								//接收验证成功
								m_pSocket_State_Info->m_nSuccessRecv += nPacketCount;
								blRecvFlag = true;

								//如果需要记录日志，则将数据计入日志
								if(m_pSocket_Info->m_blIsWriteFile == true)
								{
									WriteFile_RecvBuff(szRecvBuffData, nRecvAllSize);
								}

								//计算最小时间和最大时间
								int tTime = (int)((unsigned int)GetTickCount() - tBegin);
								if(tTime > 0 && m_pSocket_State_Info->m_nMinRecvTime == 0)
								{
									m_pSocket_State_Info->m_nMinRecvTime = tTime;
								}
								else if(tTime < m_pSocket_State_Info->m_nMinRecvTime)
								{
									m_pSocket_State_Info->m_nMinRecvTime = tTime;
								}

								if(tTime > 0 && m_pSocket_State_Info->m_nMaxRecvTime == 0)
								{
									m_pSocket_State_Info->m_nMaxRecvTime = tTime;
								}
								else if(tTime > m_pSocket_State_Info->m_nMaxRecvTime)
								{
									m_pSocket_State_Info->m_nMaxRecvTime = tTime;
								}

								break;
							}
							else if(nState == 1)
							{
								//继续接收
								nBeginRecv += nCurrRecvLen;
							}
							else
							{
								//接收包验证失败
								m_pSocket_State_Info->m_nFailRecv += nPacketCount;
								blRecvFlag = true;

								//如果需要记录日志，则将数据计入日志
								if(m_pSocket_Info->m_blIsWriteFile == true)
								{
									WriteFile_RecvBuff(szRecvBuffData, nRecvAllSize);
								}

								break;
							}
						}
						else
						{
							//如果不是高级模式，则采用配置的判定准则
							m_pSocket_State_Info->m_nRecvByteCount += nCurrRecvLen;
							nTotalRecvLen -= nCurrRecvLen;

							EM_DATA_RETURN_STATE emState = m_pSocket_Info->m_pLogic->GetRecvData(m_nThreadID, nSendLen, szRecvBuffData, nCurrRecvLen);
							if(emState == DATA_RETURN_STATE_SUCCESS)
							{
								//接收完成
								m_pSocket_State_Info->m_nSuccessRecv += nPacketCount;
								blRecvFlag = true;

								//如果需要记录日志，则将数据计入日志
								if(m_pSocket_Info->m_blIsWriteFile == true)
								{
									WriteFile_RecvBuff(szRecvBuffData, nRecvAllSize);
								}

								//计算最小时间和最大时间
								int tTime = (int)((unsigned int)GetTickCount() - tBegin);
								if(tTime > 0 && m_pSocket_State_Info->m_nMinRecvTime == 0)
								{
									m_pSocket_State_Info->m_nMinRecvTime = tTime;
								}
								else if(tTime < m_pSocket_State_Info->m_nMinRecvTime)
								{
									m_pSocket_State_Info->m_nMinRecvTime = tTime;
								}

								if(tTime > 0 && m_pSocket_State_Info->m_nMaxRecvTime == 0)
								{
									m_pSocket_State_Info->m_nMaxRecvTime = tTime;
								}
								else if(tTime > m_pSocket_State_Info->m_nMaxRecvTime)
								{
									m_pSocket_State_Info->m_nMaxRecvTime = tTime;
								}

								break;
							}
							else
							{
								nBeginRecv += nCurrRecvLen;
							}
						}
					}
				}
			}

			//如果有数据包间隔，则sleep指定的时间
			if(m_pSocket_Info->m_nPacketTimewait > 0)
			{
				DWORD dwSleepTime = (DWORD)m_pSocket_Info->m_nPacketTimewait;
				Sleep(dwSleepTime);
			}

			//如果是长连接，则不关闭连接
			if(m_pSocket_Info->m_blIsAlwayConnect == false)
			{
				//closesocket(sckClient);
				m_pSocket_State_Info->m_nCurrectSocket = 0;
				blIsConnect = false;
			}
		}

		//如果只发送一次，在这里退出
		if(m_pSocket_Info->m_blIsSendOne == true)
		{
			m_blRun = false;
		}
	}

	//如果连接没断，则断开
	closesocket(sckClient);
	closesocket(sckServer);
	m_pSocket_State_Info->m_nCurrectSocket = 0;
	blIsConnect = false;

	//回收所有的Lua申请的内存
	delete pSendParam1;
	delete pSendParam2;
	delete pSendParam3;
	delete pSendParam4;
	delete pSendParamOut;
	delete pRecvParam1;
	delete pRecvParam2;
	delete pRecvParam3;
	delete pRecvParam4;
	delete pRecvParamOut;
}

void CClientUdpSocket::Stop()
{
	m_blRun = false;
}

void CClientUdpSocket::SetSocketThread( _Socket_Info* pSocket_Info, _Socket_State_Info* pSocket_State_Info )
{
	Close();

	m_pSocket_Info       = pSocket_Info;
	m_pSocket_State_Info = pSocket_State_Info;
}

_Socket_State_Info* CClientUdpSocket::GetStateInfo()
{
	return m_pSocket_State_Info;
}

bool CClientUdpSocket::WriteFile_SendBuff( const char* pData, int nLen )
{
	FILE* pFile = NULL;
	char szFileName[20];
	sprintf_s(szFileName, "Thread%d.log", m_pSocket_Info->m_nThreadID);
	fopen_s(&pFile, szFileName, "a+");
	if(pFile == NULL)
	{
		return false;
	}

	string strLog;
	strLog = "[SendBuff]";

	for(int i = 0; i < nLen; i++)
	{
		char szChar[20];
		sprintf_s(szChar, 20, " 0x%02X", (unsigned char )pData[i]);
		strLog += szChar;
	}

	strLog += "\n";

	fwrite(strLog.c_str(), strLog.length(), sizeof(char), pFile);

	fclose(pFile);
	return true;
}

bool CClientUdpSocket::WriteFile_RecvBuff( const char* pData, int nLen )
{
	FILE* pFile = NULL;
	char szFileName[20];
	sprintf_s(szFileName, "Thread%d.log", m_pSocket_Info->m_nThreadID);
	fopen_s(&pFile, szFileName, "a+");
	if(pFile == NULL)
	{
		return false;
	}

	string strLog;
	strLog = "[RecvBuff]";

	for(int i = 0; i < nLen; i++)
	{
		char szChar[20];
		sprintf_s(szChar, 20, " 0x%02X", (unsigned char )pData[i]);
		strLog += szChar;
	}

	strLog += "\n";

	fwrite(strLog.c_str(), strLog.length(), sizeof(char), pFile);

	fclose(pFile);
	return true;
}

bool CClientUdpSocket::WriteFile_Error( const char* pError, int nErrorNumber )
{
	time_t ttNow    = time(NULL);
	struct tm tmNow;
	localtime_s(&tmNow, &ttNow);

	char szTimeNow[30] = {'\0'};
	sprintf_s(szTimeNow, 30, "[%04d-%02d-%02d %02d:%02d:%02d]", tmNow.tm_year + 1900, tmNow.tm_mon + 1, tmNow.tm_mday, tmNow.tm_hour, tmNow.tm_min, tmNow.tm_sec);

	//拼接出错日志输出
	char szError[1024] = {'\0'};
	sprintf_s(szError, 1024, "%s %s, errno=%d.\n", szTimeNow, pError, nErrorNumber);

	FILE* pFile = NULL;
	char szFileName[30];
	sprintf_s(szFileName, "StressTest_Error.log");
	fopen_s(&pFile, szFileName, "a+");
	if(pFile == NULL)
	{
		return false;
	}

	fwrite(szError, strlen(szError), sizeof(char), pFile);

	fclose(pFile);
	return true;
}

void CClientUdpSocket::SetThreadID( int nThreadID )
{
	m_nThreadID = nThreadID;
}