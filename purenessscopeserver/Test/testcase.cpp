#include "testcase.h"

bool CheckTcpPacket(_ClientInfo& objClientInfo, _ResultInfo& objResultInfo)
{
	struct timeval ttStart, ttEnd;
	int sckClient;
	char szSession[32]      = {'\0'}; 
	int nSrcLen = 0;
	int nDecLen = 0;
	
	gettimeofday(&ttStart, NULL);
	
	sprintf(objResultInfo.m_szTestName, "single packet test");
	sprintf(szSession, "FREEEYES");

	//socket创建的准备工作
	struct sockaddr_in sockaddr;

	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port   = htons(objClientInfo.m_nPort);
	sockaddr.sin_addr.s_addr = inet_addr(objClientInfo.m_szServerIP);

	sckClient = socket(AF_INET, SOCK_STREAM, 0);

	struct timeval tvTimeout;  
	tvTimeout.tv_sec  = 5;
	tvTimeout.tv_usec = 0;
	setsockopt(sckClient, SOL_SOCKET, SO_RCVTIMEO, (char *)&tvTimeout, sizeof(tvTimeout));

	//连接远程服务器
	int nErr = connect(sckClient, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
	if(0 != nErr)
	{
		gettimeofday(&ttEnd, NULL);
		sprintf(objResultInfo.m_szResult, "[e][%s:%d]connnect server fail.[%s]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, strerror(errno));
		objResultInfo.m_nRet          = 1;
		objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
		return false;
	}

	//拼装发送包体
	char szSendBuffer[MAX_BUFF_200] ={'\0'};

	short sVersion = 1;
	short sCommand = (short)COMMAND_AUTOTEST_HEAD;
	int nPacketLen = objClientInfo.m_nSendLength;

	memcpy(szSendBuffer, (char* )&sVersion, sizeof(short));
	memcpy((char* )&szSendBuffer[2], (char* )&sCommand, sizeof(short));
	memcpy((char* )&szSendBuffer[4], (char* )&nPacketLen, sizeof(int));
	memcpy((char* )&szSendBuffer[8], (char* )&szSession, sizeof(char)*32);
	memcpy((char* )&szSendBuffer[40], (char* )objClientInfo.m_pSendBuffer, sizeof(char) * objClientInfo.m_nSendLength);
	int nSendLen = nPacketLen + 40;

	//发送数据
	int nTotalSendLen = nSendLen;
	int nBeginSend    = 0;
	int nCurrSendLen  = 0;
	bool blSendFlag   = false;
	int nBeginRecv    = 0;
	int nCurrRecvLen  = 0;
	bool blRecvFlag   = false;
	while(true)
	{
		nCurrSendLen = send(sckClient, szSendBuffer + nBeginSend, nTotalSendLen, 0);
		if(nCurrSendLen <= 0)
		{
			close(sckClient);
			gettimeofday(&ttEnd, NULL);
			sprintf(objResultInfo.m_szResult, "[e][%s:%d]send server fail.[%s]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, strerror(errno));
			objResultInfo.m_nRet          = 1;
			objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
			return false;
		}
		else
		{
			nTotalSendLen -= nCurrSendLen;
			if(nTotalSendLen == 0)
			{
				//发送完成
				blSendFlag = true;
				break;
			}
			else
			{
				nBeginSend += nCurrSendLen;
			}
		}
	}

	if(blSendFlag == false)
	{
		close(sckClient);
	  gettimeofday(&ttEnd, NULL);
		sprintf(objResultInfo.m_szResult, "[e][%s:%d]send buff size not equal, buffer size[%d], send size[%d]", objClientInfo.m_szServerIP, objClientInfo.m_nPort, objClientInfo.m_nSendLength, nTotalSendLen);
		objResultInfo.m_nRet          = 1;
		objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
		return false;
	}
	else
	{
		int nTotalRecvLen               = nPacketLen + sizeof(int);
		char szRecvBuffData[1024 * 100] = {'\0'};

		while(true)
		{
			
			//如果发送成功了，则处理接收数据
			nCurrRecvLen = recv(sckClient, (char* )szRecvBuffData + nBeginRecv, nTotalRecvLen, 0);
			if(nCurrRecvLen <= 0)
			{
				close(sckClient);
				gettimeofday(&ttEnd, NULL);
				sprintf(objResultInfo.m_szResult, "[e][%s:%d]client recv data error.[%s]", objClientInfo.m_szServerIP, objClientInfo.m_nPort, strerror(errno));
				objResultInfo.m_nRet = 1;
				objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
				return false;
			}
			else
			{
				nTotalRecvLen -= nCurrRecvLen;
				if(nTotalRecvLen == 0)
				{
					//接收完成
					break;
				}
				else
				{
					nBeginRecv += nCurrRecvLen;
				}
			}
		}
	}

	sprintf(objResultInfo.m_szResult, "[s][%s:%d]success.", objClientInfo.m_szServerIP, objClientInfo.m_nPort);
	gettimeofday(&ttEnd, NULL);
	objResultInfo.m_nRet = 0;
	objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
	close(sckClient);

	return true;
}

bool CheckMultipleTcpPacket(int nCount, _ClientInfo& objClientInfo, _ResultInfo& objResultInfo)
{
	struct timeval ttStart, ttEnd;
	int sckClient;
	char szSession[32]      = {'\0'}; 
	int nSrcLen = 0;
	int nDecLen = 0;
	
	gettimeofday(&ttStart, NULL);
	
	sprintf(objResultInfo.m_szTestName, "multiple packet test(%d)", nCount);
	sprintf(szSession, "FREEEYES");

	//socket创建的准备工作
	struct sockaddr_in sockaddr;

	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port   = htons(objClientInfo.m_nPort);
	sockaddr.sin_addr.s_addr = inet_addr(objClientInfo.m_szServerIP);

	sckClient = socket(AF_INET, SOCK_STREAM, 0);

	struct timeval tvTimeout;  
	tvTimeout.tv_sec  = 5;
	tvTimeout.tv_usec = 0;
	setsockopt(sckClient, SOL_SOCKET, SO_RCVTIMEO, (char *)&tvTimeout, sizeof(tvTimeout));

	//连接远程服务器
	int nErr = connect(sckClient, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
	if(0 != nErr)
	{
		gettimeofday(&ttEnd, NULL);
		sprintf(objResultInfo.m_szResult, "[e][%s:%d]connnect server fail.[%s]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, strerror(errno));
		objResultInfo.m_nRet          = 1;
		objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
		return false;
	}

	//拼装发送包体
	char szSendBuffer[MAX_BUFF_200] ={'\0'};

	short sVersion = 1;
	short sCommand = (short)COMMAND_AUTOTEST_HEAD;
	int nPacketLen = objClientInfo.m_nSendLength;

	memcpy(szSendBuffer, (char* )&sVersion, sizeof(short));
	memcpy((char* )&szSendBuffer[2], (char* )&sCommand, sizeof(short));
	memcpy((char* )&szSendBuffer[4], (char* )&nPacketLen, sizeof(int));
	memcpy((char* )&szSendBuffer[8], (char* )&szSession, sizeof(char)*32);
	memcpy((char* )&szSendBuffer[40], (char* )objClientInfo.m_pSendBuffer, sizeof(char) * objClientInfo.m_nSendLength);
	int nSendLen = nPacketLen + 40;
	
	//拼装测试发送数据包
	char* pData = new char[nCount * nSendLen];
	memset(pData, 0, nCount * nSendLen);
	for(int i = 0; i < nCount; i++)
	{
		memcpy((char* )&pData[i * nSendLen], szSendBuffer, nSendLen);
	}

	//发送数据
	int nTotalSendLen = nCount * nSendLen;
	int nBeginSend    = 0;
	int nCurrSendLen  = 0;
	bool blSendFlag   = false;
	int nBeginRecv    = 0;
	int nCurrRecvLen  = 0;
	bool blRecvFlag   = false;
	while(true)
	{
		nCurrSendLen = send(sckClient, pData + nBeginSend, nTotalSendLen, 0);
		if(nCurrSendLen <= 0)
		{
			delete [] pData;
			close(sckClient);
			gettimeofday(&ttEnd, NULL);
			sprintf(objResultInfo.m_szResult, "[e][%s:%d]send server fail.[%s]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, strerror(errno));
			objResultInfo.m_nRet          = 1;
			objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
			return false;
		}
		else
		{
			nTotalSendLen -= nCurrSendLen;
			if(nTotalSendLen == 0)
			{
				//发送完成
				delete [] pData;
				blSendFlag = true;
				break;
			}
			else
			{
				nBeginSend += nCurrSendLen;
			}
		}
	}

	if(blSendFlag == false)
	{
		close(sckClient);
	  gettimeofday(&ttEnd, NULL);
		sprintf(objResultInfo.m_szResult, "[e][%s:%d]send buff size not equal, buffer size[%d], send size[%d]", objClientInfo.m_szServerIP, objClientInfo.m_nPort, objClientInfo.m_nSendLength, nTotalSendLen);
		objResultInfo.m_nRet          = 1;
		objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
		return false;
	}
	else
	{
		int nTotalRecvLen               = nCount * (nPacketLen + sizeof(int));
		char szRecvBuffData[1024 * 100] = {'\0'};

		while(true)
		{
			
			//如果发送成功了，则处理接收数据
			nCurrRecvLen = recv(sckClient, (char* )szRecvBuffData + nBeginRecv, nTotalRecvLen, 0);
			if(nCurrRecvLen <= 0)
			{
				close(sckClient);
				gettimeofday(&ttEnd, NULL);
				sprintf(objResultInfo.m_szResult, "[e][%s:%d]client recv data error.[%s]", objClientInfo.m_szServerIP, objClientInfo.m_nPort, strerror(errno));
				objResultInfo.m_nRet = 1;
				objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
				return false;
			}
			else
			{
				nTotalRecvLen -= nCurrRecvLen;
				if(nTotalRecvLen == 0)
				{
					//接收完成
					break;
				}
				else
				{
					nBeginRecv += nCurrRecvLen;
				}
			}
		}
	}

	sprintf(objResultInfo.m_szResult, "[s][%s:%d]success.", objClientInfo.m_szServerIP, objClientInfo.m_nPort);
	gettimeofday(&ttEnd, NULL);
	objResultInfo.m_nRet = 0;
	objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
	close(sckClient);

	return true;	
}

bool CheckMultipleTcpConnect(int nCount, _ClientInfo& objClientInfo, _ResultInfo& objResultInfo)
{
	struct timeval ttStart, ttEnd;
	int sckClient;
	char szSession[32]      = {'\0'}; 
	int nSrcLen = 0;
	int nDecLen = 0;
	
	gettimeofday(&ttStart, NULL);
	
	sprintf(objResultInfo.m_szTestName, "multiple connect test(%d)", nCount);
	sprintf(szSession, "FREEEYES");

	//socket创建的准备工作
	struct sockaddr_in sockaddr;

	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port   = htons(objClientInfo.m_nPort);
	sockaddr.sin_addr.s_addr = inet_addr(objClientInfo.m_szServerIP);

	for(int i = 0; i < nCount; i++)
	{
		sckClient = socket(AF_INET, SOCK_STREAM, 0);

		struct timeval tvTimeout;  
		tvTimeout.tv_sec  = 5;
		tvTimeout.tv_usec = 0;
		setsockopt(sckClient, SOL_SOCKET, SO_RCVTIMEO, (char *)&tvTimeout, sizeof(tvTimeout));

		//连接远程服务器
		int nErr = connect(sckClient, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
		if(0 != nErr)
		{
			gettimeofday(&ttEnd, NULL);
			sprintf(objResultInfo.m_szResult, "[e][%s:%d]connnect server fail.[%s]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, strerror(errno));
			objResultInfo.m_nRet          = 1;
			objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
			return false;
		}

		//拼装发送包体
		char szSendBuffer[MAX_BUFF_200] ={'\0'};

		short sVersion = 1;
		short sCommand = (short)COMMAND_AUTOTEST_HEAD;
		int nPacketLen = objClientInfo.m_nSendLength;

		memcpy(szSendBuffer, (char* )&sVersion, sizeof(short));
		memcpy((char* )&szSendBuffer[2], (char* )&sCommand, sizeof(short));
		memcpy((char* )&szSendBuffer[4], (char* )&nPacketLen, sizeof(int));
		memcpy((char* )&szSendBuffer[8], (char* )&szSession, sizeof(char)*32);
		memcpy((char* )&szSendBuffer[40], (char* )objClientInfo.m_pSendBuffer, sizeof(char) * objClientInfo.m_nSendLength);
		int nSendLen = nPacketLen + 40;

		//发送数据
		int nTotalSendLen = nSendLen;
		int nBeginSend    = 0;
		int nCurrSendLen  = 0;
		bool blSendFlag   = false;
		int nBeginRecv    = 0;
		int nCurrRecvLen  = 0;
		bool blRecvFlag   = false;
		while(true)
		{
			nCurrSendLen = send(sckClient, szSendBuffer + nBeginSend, nTotalSendLen, 0);
			if(nCurrSendLen <= 0)
			{
				close(sckClient);
				gettimeofday(&ttEnd, NULL);
				sprintf(objResultInfo.m_szResult, "[e][%s:%d]send server fail.[%s]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, strerror(errno));
				objResultInfo.m_nRet          = 1;
				objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
				return false;
			}
			else
			{
				nTotalSendLen -= nCurrSendLen;
				if(nTotalSendLen == 0)
				{
					//发送完成
					blSendFlag = true;
					break;
				}
				else
				{
					nBeginSend += nCurrSendLen;
				}
			}
		}

		if(blSendFlag == false)
		{
			close(sckClient);
		  gettimeofday(&ttEnd, NULL);
			sprintf(objResultInfo.m_szResult, "[e][%s:%d]send buff size not equal, buffer size[%d], send size[%d]", objClientInfo.m_szServerIP, objClientInfo.m_nPort, objClientInfo.m_nSendLength, nTotalSendLen);
			objResultInfo.m_nRet          = 1;
			objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
			return false;
		}
		else
		{
			int nTotalRecvLen               = nPacketLen + sizeof(int);
			char szRecvBuffData[1024 * 100] = {'\0'};

			while(true)
			{
				
				//如果发送成功了，则处理接收数据
				nCurrRecvLen = recv(sckClient, (char* )szRecvBuffData + nBeginRecv, nTotalRecvLen, 0);
				if(nCurrRecvLen <= 0)
				{
					close(sckClient);
					gettimeofday(&ttEnd, NULL);
					sprintf(objResultInfo.m_szResult, "[e][%s:%d]client recv data error.[%s]", objClientInfo.m_szServerIP, objClientInfo.m_nPort, strerror(errno));
					objResultInfo.m_nRet = 1;
					objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
					return false;
				}
				else
				{
					nTotalRecvLen -= nCurrRecvLen;
					if(nTotalRecvLen == 0)
					{
						//接收完成
						break;
					}
					else
					{
						nBeginRecv += nCurrRecvLen;
					}
				}
			}
		}

		sprintf(objResultInfo.m_szResult, "[s][%s:%d]success.", objClientInfo.m_szServerIP, objClientInfo.m_nPort);
		gettimeofday(&ttEnd, NULL);
		objResultInfo.m_nRet = 0;
		objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
		close(sckClient);
	}

	return true;	
}

bool CheckTcpErrorPacketHead(_ClientInfo& objClientInfo, _ResultInfo& objResultInfo)
{
	struct timeval ttStart, ttEnd;
	int sckClient;
	char szSession[32]      = {'\0'}; 
	int nSrcLen = 0;
	int nDecLen = 0;
	
	gettimeofday(&ttStart, NULL);
	
	sprintf(objResultInfo.m_szTestName, "unvaild packet test");
	sprintf(szSession, "FREEEYES");

	//socket创建的准备工作
	struct sockaddr_in sockaddr;

	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port   = htons(objClientInfo.m_nPort);
	sockaddr.sin_addr.s_addr = inet_addr(objClientInfo.m_szServerIP);

	sckClient = socket(AF_INET, SOCK_STREAM, 0);

	struct timeval tvTimeout;  
	tvTimeout.tv_sec  = 5;
	tvTimeout.tv_usec = 0;
	setsockopt(sckClient, SOL_SOCKET, SO_RCVTIMEO, (char *)&tvTimeout, sizeof(tvTimeout));

	//连接远程服务器
	int nErr = connect(sckClient, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
	if(0 != nErr)
	{
		gettimeofday(&ttEnd, NULL);
		sprintf(objResultInfo.m_szResult, "[e][%s:%d]connnect server fail.[%s]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, strerror(errno));
		objResultInfo.m_nRet          = 1;
		objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
		return false;
	}

	//拼装发送包体
	char szSendBuffer[MAX_BUFF_200] ={'\0'};

	short sVersion = 1;
	short sCommand = (short)COMMAND_AUTOTEST_HEAD;
	int nPacketLen = objClientInfo.m_nSendLength;

	//模拟一个无效包头
	memset(szSendBuffer, 0, 40);
	memcpy((char* )&szSendBuffer[40], (char* )objClientInfo.m_pSendBuffer, sizeof(char) * objClientInfo.m_nSendLength);
	int nSendLen = nPacketLen + 40;

	//发送数据
	int nTotalSendLen = nSendLen;
	int nBeginSend    = 0;
	int nCurrSendLen  = 0;
	bool blSendFlag   = false;
	int nBeginRecv    = 0;
	int nCurrRecvLen  = 0;
	bool blRecvFlag   = false;
	while(true)
	{
		nCurrSendLen = send(sckClient, szSendBuffer + nBeginSend, nTotalSendLen, 0);
		if(nCurrSendLen <= 0)
		{
			close(sckClient);
			gettimeofday(&ttEnd, NULL);
			sprintf(objResultInfo.m_szResult, "[e][%s:%d]send server fail.[%s]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, strerror(errno));
			objResultInfo.m_nRet          = 1;
			objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
			return false;
		}
		else
		{
			nTotalSendLen -= nCurrSendLen;
			if(nTotalSendLen == 0)
			{
				//发送完成
				blSendFlag = true;
				break;
			}
			else
			{
				nBeginSend += nCurrSendLen;
			}
		}
	}

	if(blSendFlag == false)
	{
		close(sckClient);
	  gettimeofday(&ttEnd, NULL);
		sprintf(objResultInfo.m_szResult, "[e][%s:%d]send buff size not equal, buffer size[%d], send size[%d]", objClientInfo.m_szServerIP, objClientInfo.m_nPort, objClientInfo.m_nSendLength, nTotalSendLen);
		objResultInfo.m_nRet          = 1;
		objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
		return false;
	}
	else
	{
		int nTotalRecvLen               = nPacketLen + sizeof(int);
		char szRecvBuffData[1024 * 100] = {'\0'};

		while(true)
		{
			
			//如果发送成功了，则处理接收数据
			nCurrRecvLen = recv(sckClient, (char* )szRecvBuffData + nBeginRecv, nTotalRecvLen, 0);
			if(nCurrRecvLen <= 0)
			{
				close(sckClient);
				gettimeofday(&ttEnd, NULL);
				sprintf(objResultInfo.m_szResult, "[s][%s:%d]success.[%s]", objClientInfo.m_szServerIP, objClientInfo.m_nPort, strerror(errno));
				objResultInfo.m_nRet = 0;
				objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
				return false;
			}
			else
			{
				nTotalRecvLen -= nCurrRecvLen;
				if(nTotalRecvLen == 0)
				{
					//接收完成
					break;
				}
				else
				{
					nBeginRecv += nCurrRecvLen;
				}
			}
		}
	}

	sprintf(objResultInfo.m_szResult, "[e][%s:%d]error recv data.", objClientInfo.m_szServerIP, objClientInfo.m_nPort);
	gettimeofday(&ttEnd, NULL);
	objResultInfo.m_nRet = 1;
	objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
	close(sckClient);

	return true;	
}

bool CheckTcpHalfPacket(_ClientInfo& objClientInfo, _ResultInfo& objResultInfo)
{
	struct timeval ttStart, ttEnd;
	int sckClient;
	char szSession[32]      = {'\0'}; 
	int nSrcLen = 0;
	int nDecLen = 0;
	
	gettimeofday(&ttStart, NULL);
	
	sprintf(objResultInfo.m_szTestName, "single packet test");
	sprintf(szSession, "FREEEYES");

	//socket创建的准备工作
	struct sockaddr_in sockaddr;

	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port   = htons(objClientInfo.m_nPort);
	sockaddr.sin_addr.s_addr = inet_addr(objClientInfo.m_szServerIP);

	sckClient = socket(AF_INET, SOCK_STREAM, 0);

	struct timeval tvTimeout;  
	tvTimeout.tv_sec  = 5;
	tvTimeout.tv_usec = 0;
	setsockopt(sckClient, SOL_SOCKET, SO_RCVTIMEO, (char *)&tvTimeout, sizeof(tvTimeout));

	//连接远程服务器
	int nErr = connect(sckClient, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
	if(0 != nErr)
	{
		gettimeofday(&ttEnd, NULL);
		sprintf(objResultInfo.m_szResult, "[e][%s:%d]connnect server fail.[%s]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, strerror(errno));
		objResultInfo.m_nRet          = 1;
		objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
		return false;
	}

	//拼装发送包体
	char szSendBuffer[MAX_BUFF_200] ={'\0'};

	short sVersion = 1;
	short sCommand = (short)COMMAND_AUTOTEST_HEAD;
	int nPacketLen = objClientInfo.m_nSendLength;

	memcpy(szSendBuffer, (char* )&sVersion, sizeof(short));
	memcpy((char* )&szSendBuffer[2], (char* )&sCommand, sizeof(short));
	memcpy((char* )&szSendBuffer[4], (char* )&nPacketLen, sizeof(int));
	memcpy((char* )&szSendBuffer[8], (char* )&szSession, sizeof(char)*32);
	memcpy((char* )&szSendBuffer[40], (char* )objClientInfo.m_pSendBuffer, sizeof(char) * objClientInfo.m_nSendLength);
	int nSendLen = nPacketLen + 40;

	//发送数据
	int nTotalSendLen = 2;
	int nBeginSend    = 0;
	int nCurrSendLen  = 0;
	bool blSendFlag   = false;
	int nBeginRecv    = 0;
	int nCurrRecvLen  = 0;
	bool blRecvFlag   = false;
	while(true)
	{
		nCurrSendLen = send(sckClient, szSendBuffer + nBeginSend, nTotalSendLen, 0);
		if(nCurrSendLen <= 0)
		{
			close(sckClient);
			gettimeofday(&ttEnd, NULL);
			sprintf(objResultInfo.m_szResult, "[e][%s:%d]send server fail.[%s]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, strerror(errno));
			objResultInfo.m_nRet          = 1;
			objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
			return false;
		}
		else
		{
			nTotalSendLen -= nCurrSendLen;
			if(nTotalSendLen == 0)
			{
				//发送完成
				blSendFlag = true;
				break;
			}
			else
			{
				nBeginSend += nCurrSendLen;
			}
		}
	}
	
	usleep(1);
	nTotalSendLen = nSendLen - 2;
	nBeginSend    = 2;
	nCurrSendLen  = 0;
	blSendFlag   = false;
	nBeginRecv    = 0;
	nCurrRecvLen  = 0;
	while(true)
	{
		nCurrSendLen = send(sckClient, szSendBuffer + nBeginSend, nTotalSendLen, 0);
		if(nCurrSendLen <= 0)
		{
			close(sckClient);
			gettimeofday(&ttEnd, NULL);
			sprintf(objResultInfo.m_szResult, "[e][%s:%d]send server fail.[%s]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, strerror(errno));
			objResultInfo.m_nRet          = 1;
			objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
			return false;
		}
		else
		{
			nTotalSendLen -= nCurrSendLen;
			if(nTotalSendLen == 0)
			{
				//发送完成
				blSendFlag = true;
				break;
			}
			else
			{
				nBeginSend += nCurrSendLen;
			}
		}
	}	

	if(blSendFlag == false)
	{
		close(sckClient);
	  gettimeofday(&ttEnd, NULL);
		sprintf(objResultInfo.m_szResult, "[e][%s:%d]send buff size not equal, buffer size[%d], send size[%d]", objClientInfo.m_szServerIP, objClientInfo.m_nPort, objClientInfo.m_nSendLength, nTotalSendLen);
		objResultInfo.m_nRet          = 1;
		objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
		return false;
	}
	else
	{
		int nTotalRecvLen               = nPacketLen + sizeof(int);
		char szRecvBuffData[1024 * 100] = {'\0'};

		while(true)
		{
			
			//如果发送成功了，则处理接收数据
			nCurrRecvLen = recv(sckClient, (char* )szRecvBuffData + nBeginRecv, nTotalRecvLen, 0);
			if(nCurrRecvLen <= 0)
			{
				close(sckClient);
				gettimeofday(&ttEnd, NULL);
				sprintf(objResultInfo.m_szResult, "[e][%s:%d]client recv data error.[%s]", objClientInfo.m_szServerIP, objClientInfo.m_nPort, strerror(errno));
				objResultInfo.m_nRet = 1;
				objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
				return false;
			}
			else
			{
				nTotalRecvLen -= nCurrRecvLen;
				if(nTotalRecvLen == 0)
				{
					//接收完成
					break;
				}
				else
				{
					nBeginRecv += nCurrRecvLen;
				}
			}
		}
	}

	sprintf(objResultInfo.m_szResult, "[s][%s:%d]success.", objClientInfo.m_szServerIP, objClientInfo.m_nPort);
	gettimeofday(&ttEnd, NULL);
	objResultInfo.m_nRet = 0;
	objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
	close(sckClient);

	return true;	
}
