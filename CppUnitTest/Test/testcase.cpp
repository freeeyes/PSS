#include "testcase.h"


bool CheckTcpPacket(_ClientInfo& objClientInfo, _ResultInfo& objResultInfo)
{
    struct timeval ttStart, ttEnd;

    char szSession[32]      = {'\0'};
    int nSrcLen = 0;
    int nDecLen = 0;

    gettimeofday(&ttStart, NULL);

    sprintf(objResultInfo.m_szTestName, "single packet test");
    sprintf(szSession, "FREEEYES");


	InitializeSocketEnvironment();
	HSocket obj_ODSocket    =SocketOpen(SOCK_STREAM);
	sockaddr_in addr;
	GetAddressFrom(&addr, objClientInfo.m_szServerIP, objClientInfo.m_nPort);
    SocketTimeOut(obj_ODSocket,5,-1,-1);
    int nErr    =SocketConnect(obj_ODSocket, &addr);


    if(0 != nErr)
    {
        gettimeofday(&ttEnd, NULL);
        sprintf(objResultInfo.m_szResult, "[e][%s:%d]connnect server fail.[%s]¡£", objClientInfo.m_szServerIP, objClientInfo.m_nPort, strerror(errno));
        objResultInfo.m_nRet          = 1;
        objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
        return false;
    }

    //Æ´×°·¢ËÍ°üÌå
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

    //·¢ËÍÊý¾Ý
    int nTotalSendLen = nSendLen;
    int nBeginSend    = 0;
    int nCurrSendLen  = 0;
    bool blSendFlag   = false;
    int nBeginRecv    = 0;
    int nCurrRecvLen  = 0;
    bool blRecvFlag   = false;

    while(true)
    {
        transresult_t rt;
        SocketSend(obj_ODSocket, szSendBuffer + nBeginSend, nTotalSendLen, rt);
        nCurrSendLen    =rt.nbytes;

        if(nCurrSendLen <= 0)
        {
            SocketClose(obj_ODSocket);
            gettimeofday(&ttEnd, NULL);
            sprintf(objResultInfo.m_szResult, "[e][%s:%d]send server fail.[%s]¡£", objClientInfo.m_szServerIP, objClientInfo.m_nPort, strerror(errno));
            objResultInfo.m_nRet          = 1;
            objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
            return false;
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

    if(blSendFlag == false)
    {
        SocketClose(obj_ODSocket);
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

            //Èç¹û·¢ËÍ³É¹¦ÁË£¬Ôò´¦Àí½ÓÊÕÊý¾Ý
            transresult_t rt;
            SocketRecv(obj_ODSocket, szRecvBuffData + nBeginRecv, nTotalRecvLen, rt);
            nCurrRecvLen    =rt.nbytes;
                
            if(nCurrRecvLen <= 0)
            {
                SocketClose(obj_ODSocket);
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
                    //½ÓÊÕÍê³É
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
    SocketClose(obj_ODSocket);

    return true;
}

bool CheckMultipleTcpPacket(int nCount, _ClientInfo& objClientInfo, _ResultInfo& objResultInfo)
{
    struct timeval ttStart, ttEnd;

    char szSession[32]      = {'\0'};
    int nSrcLen = 0;
    int nDecLen = 0;

    gettimeofday(&ttStart, NULL);

    sprintf(objResultInfo.m_szTestName, "multiple packet test(%d)", nCount);
    sprintf(szSession, "FREEEYES");

	InitializeSocketEnvironment();
	HSocket obj_ODSocket    =SocketOpen(SOCK_STREAM);
	sockaddr_in addr;
	GetAddressFrom(&addr, objClientInfo.m_szServerIP, objClientInfo.m_nPort);
    SocketTimeOut(obj_ODSocket,5,-1,-1);
    int nErr    =SocketConnect(obj_ODSocket, &addr);

    if(0 != nErr)
    {
        gettimeofday(&ttEnd, NULL);
        sprintf(objResultInfo.m_szResult, "[e][%s:%d]connnect server fail.[%s]¡£", objClientInfo.m_szServerIP, objClientInfo.m_nPort, strerror(errno));
        objResultInfo.m_nRet          = 1;
        objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
        return false;
    }

    //Æ´×°·¢ËÍ°üÌå
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

    //Æ´×°²âÊÔ·¢ËÍÊý¾Ý°ü
    char* pData = new char[nCount * nSendLen];
    memset(pData, 0, nCount * nSendLen);

    for(int i = 0; i < nCount; i++)
    {
        memcpy((char* )&pData[i * nSendLen], szSendBuffer, nSendLen);
    }

    //·¢ËÍÊý¾Ý
    int nTotalSendLen = nCount * nSendLen;
    int nBeginSend    = 0;
    int nCurrSendLen  = 0;
    bool blSendFlag   = false;
    int nBeginRecv    = 0;
    int nCurrRecvLen  = 0;
    bool blRecvFlag   = false;

    while(true)
    {
        transresult_t rt;
        SocketSend(obj_ODSocket, pData + nBeginSend, nTotalSendLen, rt);
        nCurrSendLen    =rt.nbytes;

        if(nCurrSendLen <= 0)
        {
            delete [] pData;
            SocketClose(obj_ODSocket);
            gettimeofday(&ttEnd, NULL);
            sprintf(objResultInfo.m_szResult, "[e][%s:%d]send server fail.[%s]¡£", objClientInfo.m_szServerIP, objClientInfo.m_nPort, strerror(errno));
            objResultInfo.m_nRet          = 1;
            objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
            return false;
        }
        else
        {
            nTotalSendLen -= nCurrSendLen;

            if(nTotalSendLen == 0)
            {
                //·¢ËÍÍê³É
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
        SocketClose(obj_ODSocket);
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

            //Èç¹û·¢ËÍ³É¹¦ÁË£¬Ôò´¦Àí½ÓÊÕÊý¾Ý
            transresult_t rt;
            SocketRecv(obj_ODSocket, szRecvBuffData + nBeginRecv, nTotalRecvLen, rt);
            nCurrRecvLen    =rt.nbytes;
                
            if(nCurrRecvLen <= 0)
            {
                SocketClose(obj_ODSocket);
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
                    //½ÓÊÕÍê³É
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
    SocketClose(obj_ODSocket);

    return true;
}

bool CheckMultipleTcpConnect(int nCount, _ClientInfo& objClientInfo, _ResultInfo& objResultInfo)
{
    struct timeval ttStart, ttEnd;
    HSocket obj_ODSocket;
    char szSession[32]      = {'\0'};
    int nSrcLen = 0;
    int nDecLen = 0;

    gettimeofday(&ttStart, NULL);

    sprintf(objResultInfo.m_szTestName, "multiple connect test(%d)", nCount);
    sprintf(szSession, "FREEEYES");

	InitializeSocketEnvironment();	
	sockaddr_in addr;
	GetAddressFrom(&addr, objClientInfo.m_szServerIP, objClientInfo.m_nPort);
    
    
    for(int i = 0; i < nCount; i++)
    {
        obj_ODSocket    =SocketOpen(SOCK_STREAM);

        SocketTimeOut(obj_ODSocket,5,-1,-1);
        int nErr    =SocketConnect(obj_ODSocket, &addr);
        if(0 != nErr)
        {
            gettimeofday(&ttEnd, NULL);
            sprintf(objResultInfo.m_szResult, "[e][%s:%d]connnect server fail.[%s]¡£", objClientInfo.m_szServerIP, objClientInfo.m_nPort, strerror(errno));
            objResultInfo.m_nRet          = 1;
            objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
            return false;
        }

        //Æ´×°·¢ËÍ°üÌå
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

        //·¢ËÍÊý¾Ý
        int nTotalSendLen = nSendLen;
        int nBeginSend    = 0;
        int nCurrSendLen  = 0;
        bool blSendFlag   = false;
        int nBeginRecv    = 0;
        int nCurrRecvLen  = 0;
        bool blRecvFlag   = false;

        while(true)
        {
            transresult_t rt;
            SocketSend(obj_ODSocket, szSendBuffer + nBeginSend, nTotalSendLen, rt);
            nCurrSendLen    =rt.nbytes;
            if(nCurrSendLen <= 0)
            {
                SocketClose(obj_ODSocket);
                gettimeofday(&ttEnd, NULL);
                sprintf(objResultInfo.m_szResult, "[e][%s:%d]send server fail.[%s]¡£", objClientInfo.m_szServerIP, objClientInfo.m_nPort, strerror(errno));
                objResultInfo.m_nRet          = 1;
                objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
                return false;
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

        if(blSendFlag == false)
        {
            SocketClose(obj_ODSocket);
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

                //Èç¹û·¢ËÍ³É¹¦ÁË£¬Ôò´¦Àí½ÓÊÕÊý¾Ý
                transresult_t rt;
                SocketRecv(obj_ODSocket, szRecvBuffData +nBeginRecv , nTotalRecvLen, rt);                
                nCurrRecvLen    =rt.nbytes;
                if(nCurrRecvLen <= 0)
                {
                    SocketClose(obj_ODSocket);
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
                        //½ÓÊÕÍê³É
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
        SocketClose(obj_ODSocket);
    }

    return true;
}

bool CheckTcpErrorPacketHead(_ClientInfo& objClientInfo, _ResultInfo& objResultInfo)
{
    struct timeval ttStart, ttEnd;

    char szSession[32]      = {'\0'};
    int nSrcLen = 0;
    int nDecLen = 0;

    gettimeofday(&ttStart, NULL);

    sprintf(objResultInfo.m_szTestName, "unvaild packet test");
    sprintf(szSession, "FREEEYES");

	InitializeSocketEnvironment();
	HSocket obj_ODSocket    =SocketOpen(SOCK_STREAM);
	sockaddr_in addr;
	GetAddressFrom(&addr, objClientInfo.m_szServerIP, objClientInfo.m_nPort);
    SocketTimeOut(obj_ODSocket,5,-1,-1);
    int nErr    =SocketConnect(obj_ODSocket, &addr);
    if(0 != nErr)
    {
        gettimeofday(&ttEnd, NULL);
        sprintf(objResultInfo.m_szResult, "[e][%s:%d]connnect server fail.[%s]¡£", objClientInfo.m_szServerIP, objClientInfo.m_nPort, strerror(errno));
        objResultInfo.m_nRet          = 1;
        objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
        return false;
    }

    //Æ´×°·¢ËÍ°üÌå
    char szSendBuffer[MAX_BUFF_200] = {'\0'};

    short sVersion = 1;
    short sCommand = (short)COMMAND_AUTOTEST_HEAD;
    int nPacketLen = objClientInfo.m_nSendLength;

    //Ä£ÄâÒ»¸öÎÞÐ§°üÍ·
    memset(szSendBuffer, 0, 40);
    memcpy((char* )&szSendBuffer[40], (char* )objClientInfo.m_pSendBuffer, sizeof(char) * objClientInfo.m_nSendLength);
    int nSendLen = nPacketLen + 40;

    //·¢ËÍÊý¾Ý
    int nTotalSendLen = nSendLen;
    int nBeginSend    = 0;
    int nCurrSendLen  = 0;
    bool blSendFlag   = false;
    int nBeginRecv    = 0;
    int nCurrRecvLen  = 0;
    bool blRecvFlag   = false;

    while(true)
    {
        transresult_t rt;
        SocketSend(obj_ODSocket, szSendBuffer + nBeginSend, nTotalSendLen, rt);
        nCurrSendLen    =rt.nbytes;
        if(nCurrSendLen <= 0)
        {
            SocketClose(obj_ODSocket);
            gettimeofday(&ttEnd, NULL);
            sprintf(objResultInfo.m_szResult, "[e][%s:%d]send server fail.[%s]¡£", objClientInfo.m_szServerIP, objClientInfo.m_nPort, strerror(errno));
            objResultInfo.m_nRet          = 1;
            objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
            return false;
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

    if(blSendFlag == false)
    {
        SocketClose(obj_ODSocket);;
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

            //Èç¹û·¢ËÍ³É¹¦ÁË£¬Ôò´¦Àí½ÓÊÕÊý¾Ý
            transresult_t rt;
            SocketRecv(obj_ODSocket, szRecvBuffData + nBeginRecv, nTotalRecvLen, rt);
            nCurrRecvLen    =rt.nbytes;
                
            if(nCurrRecvLen <= 0)
            {
                SocketClose(obj_ODSocket);
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
                    //½ÓÊÕÍê³É
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
    SocketClose(obj_ODSocket);
    
    return true;
}

bool CheckTcpHalfPacket(_ClientInfo& objClientInfo, _ResultInfo& objResultInfo)
{
    struct timeval ttStart, ttEnd;
    char szSession[32]      = {'\0'};
    int nSrcLen = 0;
    int nDecLen = 0;

    gettimeofday(&ttStart, NULL);

    sprintf(objResultInfo.m_szTestName, "helf packet test");
    sprintf(szSession, "FREEEYES");


	InitializeSocketEnvironment();
	HSocket obj_ODSocket    =SocketOpen(SOCK_STREAM);
	sockaddr_in addr;
	GetAddressFrom(&addr, objClientInfo.m_szServerIP, objClientInfo.m_nPort);
    SocketTimeOut(obj_ODSocket,5,-1,-1);
    int nErr    =SocketConnect(obj_ODSocket, &addr);

    if(0 != nErr)
    {
        gettimeofday(&ttEnd, NULL);
        sprintf(objResultInfo.m_szResult, "[e][%s:%d]connnect server fail.[%s]¡£", objClientInfo.m_szServerIP, objClientInfo.m_nPort, strerror(errno));
        objResultInfo.m_nRet          = 1;
        objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
        return false;
    }

    //Æ´×°·¢ËÍ°üÌå
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

    //·¢ËÍÊý¾Ý
    int nTotalSendLen = 2;
    int nBeginSend    = 0;
    int nCurrSendLen  = 0;
    bool blSendFlag   = false;
    int nBeginRecv    = 0;
    int nCurrRecvLen  = 0;
    bool blRecvFlag   = false;

    while(true)
    {
        transresult_t rt;
        SocketSend(obj_ODSocket, szSendBuffer + nBeginSend, nTotalSendLen, rt);
        nCurrSendLen    =rt.nbytes;
        if(nCurrSendLen <= 0)
        {
            SocketClose(obj_ODSocket);
            gettimeofday(&ttEnd, NULL);
            sprintf(objResultInfo.m_szResult, "[e][%s:%d]send server fail.[%s]¡£", objClientInfo.m_szServerIP, objClientInfo.m_nPort, strerror(errno));
            objResultInfo.m_nRet          = 1;
            objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
            return false;
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

    //usleep(1);
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
    nTotalSendLen = nSendLen - 2;
    nBeginSend    = 2;
    nCurrSendLen  = 0;
    blSendFlag   = false;
    nBeginRecv    = 0;
    nCurrRecvLen  = 0;

    while(true)
    {
        transresult_t rt;
        SocketSend(obj_ODSocket, szSendBuffer + nBeginSend, nTotalSendLen, rt);
        nCurrSendLen    =rt.nbytes;
        if(nCurrSendLen <= 0)
        {
            SocketClose(obj_ODSocket);
            gettimeofday(&ttEnd, NULL);
            sprintf(objResultInfo.m_szResult, "[e][%s:%d]send server fail.[%s]¡£", objClientInfo.m_szServerIP, objClientInfo.m_nPort, strerror(errno));
            objResultInfo.m_nRet          = 1;
            objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
            return false;
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

    if(blSendFlag == false)
    {
        SocketClose(obj_ODSocket);
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

            //Èç¹û·¢ËÍ³É¹¦ÁË£¬Ôò´¦Àí½ÓÊÕÊý¾Ý
            transresult_t rt;
            SocketRecv(obj_ODSocket, szRecvBuffData + nBeginRecv, nTotalRecvLen, rt);
            nCurrRecvLen    =rt.nbytes;
                
            if(nCurrRecvLen <= 0)
            {
                SocketClose(obj_ODSocket);
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
                    //½ÓÊÕÍê³É
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
    SocketClose(obj_ODSocket);

    return true;
}

void Thread_CheckTcpPacket(void* arg)
{
    _ThreadParam* pThreadParam = (_ThreadParam* )arg;

    if(NULL != pThreadParam)
    {
        CheckTcpPacket(*pThreadParam->m_pClientInfo, *pThreadParam->m_pResultInfo);
    }

    //pthread_barrier_wait(pThreadParam->m_Barrier);
}


void Thread_CheckRecvUdpPacket(void* arg)
{
    _ThreadParam* pThreadParam = (_ThreadParam* )arg;

    if(NULL != pThreadParam)
    {
        Thread_CheckUdpPacket_Recv(*pThreadParam->m_pClientInfo, *pThreadParam->m_pResultInfo);
    }

    //pthread_barrier_wait(pThreadParam->m_Barrier);
}

bool CheckTcpMulipleThreadPacket(int nCount, _ClientInfo& objClientInfo, _ResultInfo& objResultInfo)
{
    struct timeval ttStart, ttEnd;
    char szSession[32]      = {'\0'};
    int nSrcLen = 0;
    int nDecLen = 0;

    gettimeofday(&ttStart, NULL);

    sprintf(objResultInfo.m_szTestName, "Muliple connect packet test");

    _ResultInfo* pResultInfoList   = new _ResultInfo[nCount];
    _ThreadParam* pThreadParamList = new _ThreadParam[nCount];

    memset(pResultInfoList, 0, sizeof(pResultInfoList));
    memset(pThreadParamList, 0, sizeof(pThreadParamList));

    std::vector<std::thread> Thread_CheckTcpPacket_list;       
    for(int i = 0; i < nCount; i++)
    {
        _ThreadParam* pThreadParam  = &pThreadParamList[i];
        pThreadParam->m_pClientInfo = &objClientInfo;
        pThreadParam->m_pResultInfo = &pResultInfoList[i];
        
        //std::thread  thread_test(Thread_CheckTcpPacket,(void*)(pThreadParam)); 
        //std::thread thread_test(std::bind(Thread_CheckTcpPacket, this, (void*)(pThreadParam))); 
        std::thread thread_test([&] { Thread_CheckTcpPacket((void*)(pThreadParam));        });
               
        Thread_CheckTcpPacket_list.push_back(std::move(thread_test));
    }

    for(int i = 0; i < nCount; i++)
    {
        Thread_CheckTcpPacket_list[i].join();
    }
    
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
    //²âÊÔUDP·¢°ü
    struct timeval ttStart, ttEnd;
    char szSession[32]      = {'\0'};
    int nSrcLen = 0;
    int nDecLen = 0;

    gettimeofday(&ttStart, NULL);

    sprintf(objResultInfo.m_szTestName, "single UDP packet test");
    sprintf(szSession, "FREEEYES");

    //socket´´½¨µÄ×¼±¸¹¤×÷

	InitializeSocketEnvironment();
	HSocket obj_ODSocket    =SocketOpen(SOCK_DGRAM);
	sockaddr_in addr;
	GetAddressFrom(&addr, objClientInfo.m_szServerIP, objClientInfo.m_nPort);


    //Æ´×°·¢ËÍ°üÌå
    char szSendBuffer[MAX_BUFF_200] = {'\0'};

    short sVersion = 1;
    short sCommand = (short)COMMAND_AUTOTEST_UDP_HEAD;
    int nPacketLen = objClientInfo.m_nSendLength;

    memcpy(szSendBuffer, (char* )&sVersion, sizeof(short));
    memcpy((char* )&szSendBuffer[2], (char* )&sCommand, sizeof(short));
    memcpy((char* )&szSendBuffer[4], (char* )&nPacketLen, sizeof(int));
    memcpy((char* )&szSendBuffer[8], (char* )&szSession, sizeof(char)*32);
    memcpy((char* )&szSendBuffer[40], (char* )objClientInfo.m_pSendBuffer, sizeof(char) * objClientInfo.m_nSendLength);
    int nSendLen = nPacketLen + 40;

    //ÉèÖÃ½ÓÊÕÊý¾Ý°üÏß³Ì
    _ThreadParam* pThreadParam = new _ThreadParam();

    pThreadParam->m_pClientInfo = &objClientInfo;
    pThreadParam->m_pResultInfo = &objRecvResultInfo;
    
    std::thread thread_test(&Thread_CheckRecvUdpPacket,pThreadParam);
    thread_test.join(); // 等待线程结束

    //·¢ËÍÊý¾Ý
    int nTotalSendLen = nSendLen;
    int nBeginSend    = 0;
    int nCurrSendLen  = 0;
    bool blSendFlag   = false;
    int nBeginRecv    = 0;
    int nCurrRecvLen  = 0;
    bool blRecvFlag   = false;

    printf("[Thread_CheckUdpPacket]nTotalSendLen=%d.\n", nTotalSendLen);
    if( SockSendTo(obj_ODSocket, szSendBuffer, nTotalSendLen,  &addr) == -1)
    {
        SocketClose(obj_ODSocket);
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
    SocketClose(obj_ODSocket);

    //µÈ´ý½ÓÊÕÏß³Ì·µ»ØÐÅÏ¢
    //pthread_barrier_wait(&barrier);
    //pthread_barrier_destroy(&barrier);
    //pthread_barrier_wait(&barrier);
    //pthread_barrier_destroy(&barrier);

    delete pThreadParam;

    return true;
}

bool Thread_CheckUdpPacket_Recv(_ClientInfo& objClientInfo, _ResultInfo& objResultInfo)
{
    struct timeval ttStart, ttEnd;

    gettimeofday(&ttStart, NULL);
    sprintf(objResultInfo.m_szTestName, "single UDP packet recv test");

    //int set = 1;
    //setsockopt(sockListen, SOL_SOCKET, SO_REUSEADDR, &set, sizeof(int));
    
	InitializeSocketEnvironment();
	HSocket obj_ODSocket    =SocketOpen(SOCK_DGRAM);
	sockaddr_in addr;
	GetAddressFrom(&addr, objClientInfo.m_szServerIP, objClientInfo.m_nPort);
    SocketTimeOut(obj_ODSocket,5,-1,-1);

    // ±ØÐë°ó¶¨£¬·ñÔòÎÞ·¨¼àÌý
    if(SocketBind(obj_ODSocket, &addr) == -1)
    {
        SocketClose(obj_ODSocket);
        gettimeofday(&ttEnd, NULL);
        sprintf(objResultInfo.m_szResult, "[e][%s:%d]client Udp bind error.[%s]", objClientInfo.m_szServerIP, 20002, strerror(errno));
        objResultInfo.m_nRet = 1;
        objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
        return false;
    }

    printf("[Thread_CheckUdpPacket_Recv]Begin Listen UDP.\n");
    int recvbytes;
    char recvbuf[128];
    int addrLen = sizeof(struct sockaddr_in);

    if((recvbytes = SockRecvFrom(obj_ODSocket, recvbuf, 128, &addr)) != -1)
    {
        recvbuf[recvbytes] = '\0';

        //printf("receive a broadCast messgse:%s\n", recvbuf);
        if(recvbytes == 14 && strcmp(recvbuf, "Hello  friend.") == 0)
        {
            sprintf(objResultInfo.m_szResult, "[s][%s:%d]success.", objClientInfo.m_szServerIP, 20002);
            gettimeofday(&ttEnd, NULL);
            objResultInfo.m_nRet = 0;
            objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
        }
        else
        {
            printf("[Thread_CheckUdpPacket_Recv]Recv Data Error.\n");
            sprintf(objResultInfo.m_szResult, "[s][%s:%d]Recv Data Error.", objClientInfo.m_szServerIP, 20002);
            gettimeofday(&ttEnd, NULL);
            objResultInfo.m_nRet = 0;
            objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
        }

        SocketClose(obj_ODSocket);
    }
    else
    {
        SocketClose(obj_ODSocket);
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

    char szSession[32]      = {'\0'};
    int nSrcLen = 0;
    int nDecLen = 0;

    gettimeofday(&ttStart, NULL);

    sprintf(objResultInfo.m_szTestName, "console command test");
    sprintf(szSession, "FREEEYES");

	InitializeSocketEnvironment();
	HSocket obj_ODSocket    =SocketOpen(SOCK_STREAM);
	sockaddr_in addr;
	GetAddressFrom(&addr, "127.0.0.1", 10010);
    SocketTimeOut(obj_ODSocket,5,-1,-1);
    int nErr    =SocketConnect(obj_ODSocket, &addr);

    if(0 != nErr)
    {
        gettimeofday(&ttEnd, NULL);
        sprintf(objResultInfo.m_szResult, "[e][127.0.0.1:10010]connnect server fail.[%s]¡£", strerror(errno));
        objResultInfo.m_nRet          = 1;
        objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
        return false;
    }

    //Æ´×°·¢ËÍ°üÌå
    char szSendBuffer[MAX_BUFF_200] = {'\0'};

    char szSendData[200] = { '\0' };
    sprintf(szSendData, "b freeeyes ShowModule -a&");
    int nSendDataLen = strlen(szSendData);

    //·¢ËÍÊý¾Ý
    int nTotalSendLen = nSendDataLen;
    int nBeginSend    = 0;
    int nCurrSendLen  = 0;
    bool blSendFlag   = false;
    int nBeginRecv    = 0;
    int nCurrRecvLen  = 0;
    bool blRecvFlag   = false;

    while(true)
    {
        transresult_t rt;
        SocketSend(obj_ODSocket,  szSendData+ nBeginSend, nTotalSendLen, rt);
        nCurrSendLen    =rt.nbytes;
        if(nCurrSendLen <= 0)
        {
            SocketClose(obj_ODSocket);
            gettimeofday(&ttEnd, NULL);
            sprintf(objResultInfo.m_szResult, "[e][127.0.0.1:10010]send server fail.[%s]¡£", strerror(errno));
            objResultInfo.m_nRet          = 1;
            objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
            return false;
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

    if(blSendFlag == false)
    {
        SocketClose(obj_ODSocket);
        gettimeofday(&ttEnd, NULL);
        sprintf(objResultInfo.m_szResult, "[e][127.0.0.1:10010]send buff size not equal, buffer size[%d], send size[%d]", nTotalSendLen);
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

            //Èç¹û·¢ËÍ³É¹¦ÁË£¬Ôò´¦Àí½ÓÊÕÊý¾Ý
            transresult_t rt;
            SocketRecv(obj_ODSocket, szRecvBuffData + nBeginRecv, nTotalRecvLen, rt);
            nCurrRecvLen    =rt.nbytes;
                
            if(nCurrRecvLen <= 0)
            {
                SocketClose(obj_ODSocket);
                gettimeofday(&ttEnd, NULL);
                sprintf(objResultInfo.m_szResult, "[e][127.0.0.1:10010]client recv data error.[%s]", strerror(errno));
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

    sprintf(objResultInfo.m_szResult, "[s][127.0.0.1:10010]success.");
    gettimeofday(&ttEnd, NULL);
    objResultInfo.m_nRet = 0;
    objResultInfo.m_fMilliseconds = (float)(1000000*(ttEnd.tv_sec - ttStart.tv_sec) + (ttEnd.tv_usec - ttStart.tv_usec))/1000.0f;
    SocketClose(obj_ODSocket);

    return true;


    return true;
}
