#include "TcpClientConnect.h"

CTcpClientConnect::CTcpClientConnect(void)
{
}

CTcpClientConnect::~CTcpClientConnect(void)
{
}

void CTcpClientConnect::Init(const char* pIp, int nPort, const char* pKey)
{
    sprintf(m_szServerIP, "%s", pIp);
    sprintf(m_szConsoleKey, "%s", pKey);
    m_nServerPort = nPort;
}

bool CTcpClientConnect::SendConsoleMessage(const char* pMessage, int nMessageLen, char* pRevBuff, int& nRecvLen)
{

    char szBodyBuff[10 * 1024] = { '\0' };
    int nPacketLen = 0;

    //socket创建的准备工作

    char szSendData[500] = { '\0' };
    sprintf(szSendData, "b %s", pMessage);
    int nSendDataLen = strlen(szSendData);

    struct sockaddr_in sockaddr;
	HSocket sckClient = SocketOpen(SOCK_STREAM);
	GetAddressFrom(&sockaddr,m_szServerIP, m_nServerPort);




    //连接远程服务器
    int nErr = SocketConnect(sckClient, &sockaddr);

    if (0 != nErr)
    {
        SocketClose(sckClient);
        return false;
    }

    int nTotalRecvLen = nRecvLen;
    int nTotalSendLen = nSendDataLen;
    int nBeginSend = 0;
    int nCurrSendLen = 0;
    int nFinishRecv = 0;
    bool blSendFlag = false;

    while (true)
    {
        transresult_t   rt;
        SocketSend(sckClient, szSendData + nBeginSend, nSendDataLen - nBeginSend, rt);
        nCurrSendLen    =rt.nbytes;

        if (nCurrSendLen <= 0)
        {
            SocketClose(sckClient);
            return false;
        }
        else
        {
            nTotalSendLen -= nCurrSendLen;

            if (nTotalSendLen == 0)
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

    int nCurrRecvLen = 0;
    int nBodyLen = 0;

    //接收数据
    if (blSendFlag == true)
    {
        //首先接收六字节数据头
        char szHeadData[6] = { '\0' };
        int nHeadLen = sizeof(int) + sizeof(short);

        while (true)
        {
			transresult_t   rt;
            SocketRecv(sckClient, (char*)&szHeadData[nCurrRecvLen], nHeadLen - nCurrRecvLen, rt);
            int nLen    =rt.nbytes;

            if (nLen <= 0)
            {
                SocketClose(sckClient);
                return false;
            }
            else
            {
                if (nLen + nCurrRecvLen >= nHeadLen)
                {
                    break;
                }
                else
                {
                    //接收未完成，继续接收
                    nCurrRecvLen += nLen;
                }
            }
        }

        //获得数据包长度
        memcpy_s(&nBodyLen, sizeof(int), szHeadData, sizeof(int));

        if (nBodyLen <= 0 || nBodyLen >= 1024 * 1024)
        {
            SocketClose(sckClient);
            return false;
        }

        //去掉命令头2字节的 CommandID
        nBodyLen = nBodyLen - 2;

        //开始接收数据体
        nCurrRecvLen = 0;

        while (true)
        {
			transresult_t   rt;
            SocketRecv(sckClient, (char*)&szHeadData[nCurrRecvLen], nBodyLen - nCurrRecvLen, rt);
            int nLen    =rt.nbytes;

            if (nLen <= 0)
            {
                SocketClose(sckClient);
                return false;
            }
            else
            {
                if (nLen + nCurrRecvLen >= nBodyLen)
                {
                    break;
                }
                else
                {
                    //接收未完成，继续接收
                    nCurrRecvLen += nLen;
                }
            }
        }

    }

    memcpy_s(pRevBuff, nBodyLen, szBodyBuff, nBodyLen);
    nRecvLen = nBodyLen;

    SocketClose(sckClient);
    return true;
}

char* CTcpClientConnect::GetKey()
{
    return m_szConsoleKey;
}

char* CTcpClientConnect::GetServerIP()
{
    return m_szServerIP;
}

int CTcpClientConnect::GetServerPort()
{
    return m_nServerPort;
}

char* CTcpClientConnect::GetServerKey()
{
    return m_szConsoleKey;
}
