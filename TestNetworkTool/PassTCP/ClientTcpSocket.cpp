//#include "StdAfx.h"
#include "ClientTcpSocket.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>

#include <time.h>

#include <thread>

#include "sock_wrap.h"



CClientTcpSocket::CClientTcpSocket(void)
{
	m_pSocket_Info       = NULL;
	m_pSocket_State_Info = NULL;
	m_blRun              = false;
	m_nThreadID          = 0;
}

CClientTcpSocket::~CClientTcpSocket(void)
{
	Close();
}

void CClientTcpSocket::Close()
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

void CClientTcpSocket::SetSocketThread( _Socket_Info* pSocket_Info, _Socket_State_Info* pSocket_State_Info )
{
	Close();

	m_pSocket_Info       = pSocket_Info;
	m_pSocket_State_Info = pSocket_State_Info;
}

_Socket_State_Info* CClientTcpSocket::GetStateInfo()
{
	return m_pSocket_State_Info;
}

void CClientTcpSocket::Stop()
{
	m_blRun = false;
}

void CClientTcpSocket::Run()
{
	int nPacketCount = 1;
	m_blRun = true;
	HSocket sckClient;

	//�˲���Ϊ����Lua���������
	//Ϊ�˼��ٲ���Ҫ��new��delete���������Բ�����������������
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

	//����Ǹ߼�ģʽ
	if(m_pSocket_Info->m_blLuaAdvance == true)
	{
		m_objLuaFn.InitClass();

		bool blState = m_objLuaFn.LoadLuaFile(m_pSocket_Info->m_szLuaFileName);
		if(false == blState)
		{
			printf("[Main]Open Lua file error.\n");
			return;
		}

		//��ʼ������Ҫʹ�õ�Lua����
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

	//�����Ƿ��ǳ����ӣ�����ǳ����ӣ���ֻ����һ�Ρ�
	bool blIsConnect = false;

	//socket������׼������
	struct sockaddr_in sockaddr;

//	memset(&sockaddr, 0, sizeof(sockaddr));
//	sockaddr.sin_family = AF_INET;
//	sockaddr.sin_port   = htons(m_pSocket_Info->m_nPort);
//	sockaddr.sin_addr.S_un.S_addr = inet_addr(m_pSocket_Info->m_szSerevrIP);

	GetAddressFrom(&sockaddr,   m_pSocket_Info->m_szSerevrIP, m_pSocket_Info->m_nPort);

	//���ʹ���
	int nSendIndex = 0;

	while(m_blRun)
	{
		unsigned int tBegin = (unsigned int)GetTickCount();
		if(m_pSocket_Info->m_nSendCount != 0 && m_pSocket_State_Info->m_nSuccessSend >= m_pSocket_Info->m_nSendCount)
		{
			//����ָ����Ŀ�����ݰ����
			break;
		}

		//�鿴�Ƿ����߼�ģʽ
		if(m_pSocket_Info->m_blLuaAdvance == true)
		{
			//���û�����󳤶�
			m_pSocket_Info->m_pLogic->SetMaxSendLength(nLuaBufferMaxLength);

			//��ʼ����Lua�ű���ȥ��֯���ݿ�
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
			sckClient = socket(AF_INET, SOCK_STREAM, 0);

			int TimeOut = (int)m_pSocket_Info->m_nRecvTimeout;
			::setsockopt(sckClient, SOL_SOCKET, SO_RCVTIMEO, (char *)&TimeOut, sizeof(TimeOut));

			int dwSleepTime = (int)m_pSocket_Info->m_nDelaySecond;
			if(m_pSocket_Info->m_blIsRadomaDelay == true)
			{
				//���������ģ����1-1000֮�����һ��ʱ��
				dwSleepTime = (int)RandomValue(1, 1000);
			}

			if(dwSleepTime > 0)
			{
				//����ָ����ʱ
				std::this_thread::sleep_for(std::chrono::milliseconds(dwSleepTime*1000));
			}

			//����Զ�̷�����
			int nErr = SocketConnect(sckClient, &sockaddr);

			if(0 != nErr)
			{
				//�ر�socket
				SocketClose(sckClient);

				int dwError = GetLastSocketError();
				WriteFile_Error("Connect error", (int)dwError);
				m_pSocket_State_Info->m_nFailConnect++;
				m_pSocket_State_Info->m_nCurrectSocket = 0;
			}
			else
			{
				//�ɹ�����
				m_pSocket_State_Info->m_nSuccessConnect++;
				m_pSocket_State_Info->m_nCurrectSocket = 1;
				blIsConnect = true;
				nSendIndex  = 0;
			}
		}

		if(blIsConnect == true)
		{
			//��������
			char szSendBuffData[MAX_BUFF_1024 * 100] = {'\0'};
			char szRecvBuffData[MAX_BUFF_1024 * 100] = {'\0'};

			char* pSendData   = NULL;
			int nSendLen      = 0;
			int nTotalRecvLen = 0;
			//�������Ϊ������ݰ�
			if(m_pSocket_Info->m_blIsSendCount == true)
			{
				int nSendCount = RandomValue(1, 10);

				//����׷��һ���߼�����¼��ǰ�����������Ƿ�ƥ�䣬��������ܳ�����ǰ��������
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

				//��������
				pSendData     = (char* )szSendBuffData;
				nSendLen      = nSendLen * nSendCount;
				nTotalRecvLen = m_pSocket_Info->m_pLogic->GetRecvLength() * nSendCount;
			}
			else
			{
				//��������
				pSendData     = (char* )m_pSocket_Info->m_pLogic->GetSendData(m_pSocket_Info->m_nThreadID, nSendIndex, nSendLen);
				nTotalRecvLen = m_pSocket_Info->m_pLogic->GetRecvLength();

				nPacketCount  = 1;
			}

			//��¼Ӧ���ֽ�����
			int nRecvAllSize = nTotalRecvLen;

			//�����Ҫ��¼��־�������ݼ�����־
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
				nCurrSendLen = send(sckClient, pSendData + nBeginSend, nTotalSendLen, 0);
				if(nCurrSendLen <= 0)
				{
					int dwError = GetLastSocketError();
					WriteFile_Error("send error", (int)dwError);

					m_pSocket_State_Info->m_nFailSend += nPacketCount;
					SocketClose(sckClient);
					m_pSocket_State_Info->m_nCurrectSocket = 0;
					blIsConnect = false;

					//�ж��Ƿ����Զ�����
					if(m_pSocket_Info->m_blIsBroken == false)
					{
						return ;
					}

					break;
				}
				else
				{
					nTotalSendLen -= nCurrSendLen;
					if(nTotalSendLen == 0)
					{
						//�������
						m_pSocket_State_Info->m_nSuccessSend += nPacketCount;

						//��¼�����ֽ���
						m_pSocket_State_Info->m_nSendByteCount += nCurrSendLen;

						blSendFlag = true;
						break;
					}
					else
					{
						nBeginSend += nCurrSendLen;

						//��¼�����ֽ���
						m_pSocket_State_Info->m_nSendByteCount += nCurrSendLen;
					}
				}
			}
			nSendIndex++;

			//��������
			if(blSendFlag == true && m_pSocket_Info->m_blIsRecv == true)
			{
				while(true)
				{
					//������ͳɹ��ˣ������������
					nCurrRecvLen = recv(sckClient, (char* )szRecvBuffData + nBeginRecv, nTotalRecvLen, 0);
					if(nCurrRecvLen <= 0)
					{
						int dwError = GetLastSocketError();
						WriteFile_Error("recv error", (int)dwError);

						//�õ����ص�IP�Ͷ˿�
						struct sockaddr_in sockClient;

						memset(&sockClient, 0, sizeof(sockClient));
						int nClientSocketSize = sizeof(sockClient);

						GetSocketName(sckClient, (struct sockaddr_in *)&sockClient);

						char szWData[MAX_BUFF_1024] = {'\0'};
						sprintf(szWData,  "[%s:%d]SendCount=%d.", inet_ntoa(sockClient.sin_addr), ntohs(sockClient.sin_port), nSendIndex);
						WriteFile_Data(szWData);

						m_pSocket_State_Info->m_nFailRecv += nPacketCount;
						SocketClose(sckClient);
						m_pSocket_State_Info->m_nCurrectSocket = 0;
						blIsConnect = false;

						//�ж��Ƿ����Զ�����
						if(m_pSocket_Info->m_blIsBroken == false)
						{
							return ;
						}

						break;
					}
					else
					{
						//����Ǹ߼�ģʽ���������Lua�ӿڷ���
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

							//���ý��պ���
							m_objLuaFn.CallFileFn("PassTcp_GetRecvData", objRecvIn, objRecvOut);

							int* pReturn = (int* )pRecvParamOut->GetParam();
							nState = (int)(*pReturn);

							objRecvIn.Close(false);
							objRecvOut.Close(false);

							//�жϽű�����ֵ
							if(nState == 0)
							{
								//������֤�ɹ�
								m_pSocket_State_Info->m_nSuccessRecv += nPacketCount;
								blRecvFlag = true;

								//�����Ҫ��¼��־�������ݼ�����־
								if(m_pSocket_Info->m_blIsWriteFile == true)
								{
									WriteFile_RecvBuff(szRecvBuffData, nRecvAllSize);
								}

								//������Сʱ������ʱ��
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
								//��������
								nBeginRecv += nCurrRecvLen;
							}
							else
							{
								//���հ���֤ʧ��
								m_pSocket_State_Info->m_nFailRecv += nPacketCount;
								blRecvFlag = true;

								//�����Ҫ��¼��־�������ݼ�����־
								if(m_pSocket_Info->m_blIsWriteFile == true)
								{
									WriteFile_RecvBuff(szRecvBuffData, nRecvAllSize);
								}

								break;
							}
						}
						else
						{
							//������Ǹ߼�ģʽ����������õ��ж�׼��
							m_pSocket_State_Info->m_nRecvByteCount += nCurrRecvLen;
							nTotalRecvLen -= nCurrRecvLen;

							EM_DATA_RETURN_STATE emState = m_pSocket_Info->m_pLogic->GetRecvData(m_nThreadID, nSendIndex, szRecvBuffData, nCurrRecvLen);

							if(nTotalRecvLen == 0 || emState == DATA_RETURN_STATE_SUCCESS)
							{
								//�������
								m_pSocket_State_Info->m_nSuccessRecv += nPacketCount;
								blRecvFlag = true;

								//�����Ҫ��¼��־�������ݼ�����־
								if(m_pSocket_Info->m_blIsWriteFile == true)
								{
									WriteFile_RecvBuff(szRecvBuffData, nRecvAllSize);
								}

								//������Сʱ������ʱ��
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

			//��������ݰ��������sleepָ����ʱ��
			if(m_pSocket_Info->m_nPacketTimewait > 0)
			{
				int dwSleepTime = (int)m_pSocket_Info->m_nPacketTimewait;
				std::this_thread::sleep_for(std::chrono::milliseconds(dwSleepTime*1000));
			}

			//����ǳ����ӣ��򲻹ر�����
			if(m_pSocket_Info->m_blIsAlwayConnect == false)
			{
				SocketClose(sckClient);
				m_pSocket_State_Info->m_nCurrectSocket = 0;
				blIsConnect = false;
			}
		}

		//���ֻ����һ�Σ��������˳�
		if(m_pSocket_Info->m_blIsSendOne == true)
		{
			m_blRun = false;
		}
	}

	//�������û�ϣ���Ͽ�
	if(blIsConnect == true)
	{
		SocketClose(sckClient);
		m_pSocket_State_Info->m_nCurrectSocket = 0;
		blIsConnect = false;
	}

	//�������е�Lua������ڴ�
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

bool CClientTcpSocket::WriteFile_SendBuff( const char* pData, int nLen )
{
	FILE* pFile = NULL;
	char szFileName[20];
	sprintf(szFileName, "Thread%d.log", m_pSocket_Info->m_nThreadID);
	pFile   =fopen( szFileName, "a+");
	if(pFile == NULL)
	{
		return false;
	}

	string strLog;
	strLog = "[SendBuff]";

	for(int i = 0; i < nLen; i++)
	{
		char szChar[20];
		sprintf(szChar,  " 0x%02X", (unsigned char )pData[i]);
		strLog += szChar;
	}

	strLog += "\n";

	fwrite(strLog.c_str(), strLog.length(), sizeof(char), pFile);

	fclose(pFile);
	return true;
}

bool CClientTcpSocket::WriteFile_RecvBuff( const char* pData, int nLen )
{
	FILE* pFile = NULL;
	char szFileName[20];
	sprintf(szFileName, "Thread%d.log", m_pSocket_Info->m_nThreadID);
	pFile   =fopen( szFileName, "a+");
	if(pFile == NULL)
	{
		return false;
	}

	string strLog;
	strLog = "[RecvBuff]";

	for(int i = 0; i < nLen; i++)
	{
		char szChar[20];
		sprintf(szChar,  " 0x%02X", (unsigned char )pData[i]);
		strLog += szChar;
	}

	strLog += "\n";

	fwrite(strLog.c_str(), strLog.length(), sizeof(char), pFile);

	fclose(pFile);
	return true;
}

bool CClientTcpSocket::WriteFile_Error( const char* pError, int nErrorNumber )
{
	time_t ttNow = time(NULL);
	struct tm tmNow;
	tmNow   =*(localtime( &ttNow));

	char szTimeNow[30] = {'\0'};
	sprintf(szTimeNow,  "[%04d-%02d-%02d %02d:%02d:%02d]", tmNow.tm_year + 1900, tmNow.tm_mon + 1, tmNow.tm_mday, tmNow.tm_hour, tmNow.tm_min, tmNow.tm_sec);

	//ƴ�ӳ�����־���
	char szError[1024] = {'\0'};
	sprintf(szError,  "%s %s, errno=%d.\n", szTimeNow, pError, nErrorNumber);

	FILE* pFile = NULL;
	char szFileName[30];
	sprintf(szFileName, "StressTest_Error.log");
	pFile   =fopen( szFileName, "a+");
	if(pFile == NULL)
	{
		return false;
	}

	fwrite(szError, strlen(szError), sizeof(char), pFile);

	fclose(pFile);
	return true;
}

bool CClientTcpSocket::WriteFile_Data( const char* pError)
{
	time_t ttNow = time(NULL);
	struct tm tmNow;
	//localtime(&tmNow, &ttNow);
	tmNow   =*(localtime( &ttNow));

	char szTimeNow[30] = {'\0'};
	sprintf(szTimeNow,  "[%04d-%02d-%02d %02d:%02d:%02d]", tmNow.tm_year + 1900, tmNow.tm_mon + 1, tmNow.tm_mday, tmNow.tm_hour, tmNow.tm_min, tmNow.tm_sec);

	//ƴ�ӳ�����־���
	char szError[1024] = {'\0'};
	sprintf(szError,  "%s %s.\n", szTimeNow, pError);

	FILE* pFile = NULL;
	char szFileName[30];
	sprintf(szFileName, "StressTest_Error.log");
	pFile   =fopen( szFileName, "a+");
	if(pFile == NULL)
	{
		return false;
	}

	fwrite(szError, strlen(szError), sizeof(char), pFile);

	fclose(pFile);
	return true;
}

void CClientTcpSocket::SetThreadID(int nThreadID)
{
	m_nThreadID = nThreadID;
}
