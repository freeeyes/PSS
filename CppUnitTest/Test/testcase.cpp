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
    setsockopt(sckClient, SOL_SOCKET, SO_RCVTIMEO, (char*)&tvTimeout, sizeof(tvTimeout));

    //连接远程服务器
    int nErr = connect(sckClient, (struct sockaddr*)&sockaddr, sizeof(sockaddr));

    if(0 != nErr)
    {
        gettimeofday(&ttEnd, NULL);
        sprintf(objResultInfo.m_szResult, "[e][%s:%d]connnect server fail.[%s]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, strerror(errno));
        objResultInfo.m_nRet          = 1;
        objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
        return false;
    }

    //拼装发送包体
    char szSendBuffer[MAX_BUFF_200] = {'\0'};

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
    setsockopt(sckClient, SOL_SOCKET, SO_RCVTIMEO, (char*)&tvTimeout, sizeof(tvTimeout));

    //连接远程服务器
    int nErr = connect(sckClient, (struct sockaddr*)&sockaddr, sizeof(sockaddr));

    if(0 != nErr)
    {
        gettimeofday(&ttEnd, NULL);
        sprintf(objResultInfo.m_szResult, "[e][%s:%d]connnect server fail.[%s]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, strerror(errno));
        objResultInfo.m_nRet          = 1;
        objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
        return false;
    }

    //拼装发送包体
    char szSendBuffer[MAX_BUFF_200] = {'\0'};

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
        setsockopt(sckClient, SOL_SOCKET, SO_RCVTIMEO, (char*)&tvTimeout, sizeof(tvTimeout));

        //连接远程服务器
        int nErr = connect(sckClient, (struct sockaddr*)&sockaddr, sizeof(sockaddr));

        if(0 != nErr)
        {
            gettimeofday(&ttEnd, NULL);
            sprintf(objResultInfo.m_szResult, "[e][%s:%d]connnect server fail.[%s]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, strerror(errno));
            objResultInfo.m_nRet          = 1;
            objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
            return false;
        }

        //拼装发送包体
        char szSendBuffer[MAX_BUFF_200] = {'\0'};

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
    setsockopt(sckClient, SOL_SOCKET, SO_RCVTIMEO, (char*)&tvTimeout, sizeof(tvTimeout));

    //连接远程服务器
    int nErr = connect(sckClient, (struct sockaddr*)&sockaddr, sizeof(sockaddr));

    if(0 != nErr)
    {
        gettimeofday(&ttEnd, NULL);
        sprintf(objResultInfo.m_szResult, "[e][%s:%d]connnect server fail.[%s]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, strerror(errno));
        objResultInfo.m_nRet          = 1;
        objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
        return false;
    }

    //拼装发送包体
    char szSendBuffer[MAX_BUFF_200] = {'\0'};

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

    sprintf(objResultInfo.m_szTestName, "helf packet test");
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
    setsockopt(sckClient, SOL_SOCKET, SO_RCVTIMEO, (char*)&tvTimeout, sizeof(tvTimeout));

    //连接远程服务器
    int nErr = connect(sckClient, (struct sockaddr*)&sockaddr, sizeof(sockaddr));

    if(0 != nErr)
    {
        gettimeofday(&ttEnd, NULL);
        sprintf(objResultInfo.m_szResult, "[e][%s:%d]connnect server fail.[%s]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, strerror(errno));
        objResultInfo.m_nRet          = 1;
        objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
        return false;
    }

    //拼装发送包体
    char szSendBuffer[MAX_BUFF_200] = {'\0'};

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

void* Thread_CheckTcpPacket(void* arg)
{
    _ThreadParam* pThreadParam = (_ThreadParam* )arg;

    if(NULL != pThreadParam)
    {
        CheckTcpPacket(*pThreadParam->m_pClientInfo, *pThreadParam->m_pResultInfo);
    }

    pthread_barrier_wait(pThreadParam->m_Barrier);
}


void* Thread_CheckRecvUdpPacket(void* arg)
{
    _ThreadParam* pThreadParam = (_ThreadParam* )arg;

    if(NULL != pThreadParam)
    {
        Thread_CheckUdpPacket_Recv(pThreadParam ->m_nClientSocket, *pThreadParam->m_pClientInfo, *pThreadParam->m_pResultInfo);
    }

    pthread_barrier_wait(pThreadParam->m_Barrier);
}

bool CheckTcpMulipleThreadPacket(int nCount, _ClientInfo& objClientInfo, _ResultInfo& objResultInfo)
{
    struct timeval ttStart, ttEnd;
    int sckClient;
    char szSession[32]      = {'\0'};
    int nSrcLen = 0;
    int nDecLen = 0;

    gettimeofday(&ttStart, NULL);

    sprintf(objResultInfo.m_szTestName, "Muliple connect packet test");

    _ResultInfo* pResultInfoList   = new _ResultInfo[nCount];
    _ThreadParam* pThreadParamList = new _ThreadParam[nCount];

    memset(pResultInfoList, 0, sizeof(pResultInfoList));
    memset(pThreadParamList, 0, sizeof(pThreadParamList));

    pthread_barrier_t barrier;

    //初始化栅栏
    pthread_barrier_init(&barrier, NULL, 1 + nCount);

    for(int i = 0; i < nCount; i++)
    {
        pthread_t pid;

        _ThreadParam* pThreadParam  = &pThreadParamList[i];
        pThreadParam->m_pClientInfo = &objClientInfo;
        pThreadParam->m_pResultInfo = &pResultInfoList[i];
        pThreadParam->m_Barrier     = &barrier;

        pthread_create(&pid, NULL, &Thread_CheckTcpPacket, (void* )pThreadParam);
    }

    pthread_barrier_wait(&barrier);
    pthread_barrier_destroy(&barrier);

    for(int i = 0; i < nCount; i++)
    {
        if(pResultInfoList[i].m_nRet == 1)
        {
            sprintf(objResultInfo.m_szResult, "[e][%s:%d]error(Connect ID %d).", objClientInfo.m_szServerIP, objClientInfo.m_nPort, i);
            objResultInfo.m_nRet = 1;
        }
    }

    sprintf(objResultInfo.m_szResult, "[s][%s:%d]success.", objClientInfo.m_szServerIP, objClientInfo.m_nPort);
    gettimeofday(&ttEnd, NULL);
    objResultInfo.m_nRet = 0;
    objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;

    delete [] pThreadParamList;
    delete [] pResultInfoList;
    return true;
}

bool Thread_CheckUdpPacket(_ClientInfo& objClientInfo, _ResultInfo& objResultInfo, _ResultInfo& objRecvResultInfo)
{
    //测试UDP发包
    struct timeval ttStart, ttEnd;
    int sckClient;
    char szSession[32]      = {'\0'};
    int nSrcLen = 0;
    int nDecLen = 0;

    gettimeofday(&ttStart, NULL);

    sprintf(objResultInfo.m_szTestName, "single UDP packet test");
    sprintf(szSession, "FREEEYES");

    //socket创建的准备工作
    struct sockaddr_in sockaddr_Server;

    memset(&sockaddr_Server, 0, sizeof(sockaddr_Server));
    sockaddr_Server.sin_family = AF_INET;
    sockaddr_Server.sin_port   = htons(10003);
    sockaddr_Server.sin_addr.s_addr = inet_addr(objClientInfo.m_szServerIP);
    printf("[Thread_CheckUdpPacket]objClientInfo.m_szServerIP=%s.\n", objClientInfo.m_szServerIP);

    sckClient = socket(AF_INET, SOCK_DGRAM, 0);

		//绑定服务器端口
		struct sockaddr_in sockaddr_Client;
		memset(&sockaddr_Client, 0, sizeof(sockaddr_Client));
		sockaddr_Client.sin_family = AF_INET;
		sockaddr_Client.sin_port = htons(20002);
		sockaddr_Client.sin_addr.s_addr = 0;
		if(-1 == bind(sckClient, (struct sockaddr*)&sockaddr_Client, sizeof(sockaddr_Client)))
		{
			printf("[Thread_CheckUdpPacket]Bind 20002 error.\n");
		}
		
		printf("[Thread_CheckUdpPacket]sckClient=%d.\n", sckClient);

    //拼装发送包体
    char szSendBuffer[MAX_BUFF_200] = {'\0'};

    short sVersion = 1;
    short sCommand = (short)COMMAND_AUTOTEST_HEAD;
    int nPacketLen = objClientInfo.m_nSendLength;

    memcpy(szSendBuffer, (char* )&sVersion, sizeof(short));
    memcpy((char* )&szSendBuffer[2], (char* )&sCommand, sizeof(short));
    memcpy((char* )&szSendBuffer[4], (char* )&nPacketLen, sizeof(int));
    memcpy((char* )&szSendBuffer[8], (char* )&szSession, sizeof(char)*32);
    memcpy((char* )&szSendBuffer[40], (char* )objClientInfo.m_pSendBuffer, sizeof(char) * objClientInfo.m_nSendLength);
    int nSendLen = nPacketLen + 40;

    //设置接收数据包线程
    pthread_t pid;
    _ThreadParam* pThreadParam = new _ThreadParam();

    pthread_barrier_t barrier;

    //初始化栅栏
    pthread_barrier_init(&barrier, NULL, 1 + 1);

    pThreadParam->m_pClientInfo   = &objClientInfo;
    pThreadParam->m_pResultInfo   = &objRecvResultInfo;
    pThreadParam->m_Barrier       = &barrier;
    pThreadParam->m_nClientSocket = sckClient;

    pthread_create(&pid, NULL, &Thread_CheckRecvUdpPacket, (void* )pThreadParam);

    //发送数据
    int nTotalSendLen = nSendLen;
    int nBeginSend    = 0;
    int nCurrSendLen  = 0;
    bool blSendFlag   = false;
    int nBeginRecv    = 0;
    int nCurrRecvLen  = 0;
    bool blRecvFlag   = false;

    printf("[Thread_CheckUdpPacket]nTotalSendLen=%d.\n", nTotalSendLen);

    if( sendto(sckClient, szSendBuffer, nTotalSendLen, 0, (struct sockaddr*)&sockaddr_Server, sizeof(sockaddr_Server)) == -1)
    {
        close(sckClient);
        gettimeofday(&ttEnd, NULL);
        sprintf(objResultInfo.m_szResult, "[e][%s:%d]client Udp Send data error.[%s]", objClientInfo.m_szServerIP, 10003, strerror(errno));
        objResultInfo.m_nRet = 1;
        objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
        return false;
    }

    sprintf(objResultInfo.m_szResult, "[s][%s:%d]success.", objClientInfo.m_szServerIP, 10003);
    gettimeofday(&ttEnd, NULL);
    objResultInfo.m_nRet = 0;
    objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;

    //等待接收线程返回信息
    pthread_barrier_wait(&barrier);
    pthread_barrier_destroy(&barrier);

		close(sckClient);
    delete pThreadParam;

    return true;
}

bool Thread_CheckUdpPacket_Recv(int nClientSocket, _ClientInfo& objClientInfo, _ResultInfo& objResultInfo)
{
    struct timeval ttStart, ttEnd;
    int sockListen = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in recvAddr;

    gettimeofday(&ttStart, NULL);
    sprintf(objResultInfo.m_szTestName, "single UDP packet recv test.");

		int set = 1;

    printf("[Thread_CheckUdpPacket_Recv]Begin Listen UDP(%d).\n", nClientSocket);
    int recvbytes;
    char recvbuf[128];
    int addrLen = sizeof(struct sockaddr_in);

    if((recvbytes = recvfrom(nClientSocket, recvbuf, 128, 0,
                             (struct sockaddr*)&recvAddr, (socklen_t*)&addrLen)) != -1)
    {
        recvbuf[recvbytes] = '\0';

        printf("receive a broadCast messgse:(%d)\n", recvbytes);
        if(recvbytes == 12)
        {
            sprintf(objResultInfo.m_szResult, "[s][%s:%d]success.", objClientInfo.m_szServerIP, 20002);
            gettimeofday(&ttEnd, NULL);
            objResultInfo.m_nRet = 0;
            objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
        }
        else
        {
            printf("[Thread_CheckUdpPacket_Recv]Recv Data Error(%d).\n", errno);
            sprintf(objResultInfo.m_szResult, "[s][%s:%d]Recv Data Error.\n", objClientInfo.m_szServerIP, 20002);
            gettimeofday(&ttEnd, NULL);
            objResultInfo.m_nRet = 0;
            objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
        }

        close(nClientSocket);
    }
    else
    {
        close(nClientSocket);
        gettimeofday(&ttEnd, NULL);
        sprintf(objResultInfo.m_szResult, "[e][%s:%d]client Udp bind error.[%s]", objClientInfo.m_szServerIP, 20002, strerror(errno));
        objResultInfo.m_nRet = 1;
        objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
        return false;
    }

    return true;
}

bool CheckConsolePacket(_ResultInfo& objResultInfo)
{
    struct timeval ttStart, ttEnd;
    int sckClient;
    char szSession[32]      = {'\0'};
    int nSrcLen = 0;
    int nDecLen = 0;

    gettimeofday(&ttStart, NULL);

    sprintf(objResultInfo.m_szTestName, "console command test");
    sprintf(szSession, "FREEEYES");

    //socket创建的准备工作
    struct sockaddr_in sockaddr;

    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port   = htons(10021);
    sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    sckClient = socket(AF_INET, SOCK_STREAM, 0);

    struct timeval tvTimeout;
    tvTimeout.tv_sec  = 5;
    tvTimeout.tv_usec = 0;
    setsockopt(sckClient, SOL_SOCKET, SO_RCVTIMEO, (char*)&tvTimeout, sizeof(tvTimeout));

    //连接远程服务器
    int nErr = connect(sckClient, (struct sockaddr*)&sockaddr, sizeof(sockaddr));

    if(0 != nErr)
    {
        gettimeofday(&ttEnd, NULL);
        sprintf(objResultInfo.m_szResult, "[e][127.0.0.1:10021]connnect server fail.[%s]。", strerror(errno));
        objResultInfo.m_nRet          = 1;
        objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
        return false;
    }

    //拼装发送包体
    char szSendBuffer[MAX_BUFF_200] = {'\0'};

    char szSendData[200] = { '\0' };
    sprintf(szSendData, "b freeeyes ShowModule -a&");
    int nSendDataLen = strlen(szSendData);

    //发送数据
    int nTotalSendLen = nSendDataLen;
    int nBeginSend    = 0;
    int nCurrSendLen  = 0;
    bool blSendFlag   = false;
    int nBeginRecv    = 0;
    int nCurrRecvLen  = 0;
    bool blRecvFlag   = false;

    while(true)
    {
        nCurrSendLen = send(sckClient, szSendData + nBeginSend, nTotalSendLen, 0);

        if(nCurrSendLen <= 0)
        {
            close(sckClient);
            gettimeofday(&ttEnd, NULL);
            sprintf(objResultInfo.m_szResult, "[e][127.0.0.1:10021]send server fail.[%s]。", strerror(errno));
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
        sprintf(objResultInfo.m_szResult, "[e][127.0.0.1:10021]send buff size not equal, buffer size[%d], send size[%d]", nSendDataLen, nTotalSendLen);
        objResultInfo.m_nRet          = 1;
        objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
        return false;
    }
    else
    {
        int nTotalRecvLen               = 200;
        char szRecvBuffData[1024 * 100] = {'\0'};

        while(true)
        {

            //如果发送成功了，则处理接收数据
            nCurrRecvLen = recv(sckClient, (char* )szRecvBuffData + nBeginRecv, nTotalRecvLen, 0);

            if(nCurrRecvLen <= 0)
            {
                close(sckClient);
                gettimeofday(&ttEnd, NULL);
                sprintf(objResultInfo.m_szResult, "[e][127.0.0.1:10021]client recv data error.[%s]", strerror(errno));
                objResultInfo.m_nRet = 1;
                objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
                return false;
            }
            else
            {
                nTotalRecvLen -= nCurrRecvLen;
                break;
            }
        }
    }

    sprintf(objResultInfo.m_szResult, "[s][127.0.0.1:10021]success.");
    gettimeofday(&ttEnd, NULL);
    objResultInfo.m_nRet = 0;
    objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
    close(sckClient);

    return true;


    return true;
}
