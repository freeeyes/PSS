#include "MessageDispose.h"

bool GetFileInfo(const char* pFile, _FileInfo& FileInfo)
{
    //按照逗号拆分
    int nBegin = 0;
    int nEnd = 0;
    int nPosIndex = 0;

    int nLen = (int)ACE_OS::strlen(pFile);

    for (int i = 0; i < nLen; i++)
    {
        if (pFile[i] == ',')
        {
            if (nPosIndex == 0)
            {
                //模块路径
                nEnd = i;
                memcpy_safe((char*)&pFile[nBegin], nEnd - nBegin, FileInfo.m_szFilePath, MAX_BUFF_100);
                FileInfo.m_szFilePath[nEnd - nBegin] = '\0';
                nBegin = i + 1;
                nPosIndex++;
            }
            else if (nPosIndex == 1)
            {
                //模块文件名
                nEnd = i;
                memcpy_safe((char*)&pFile[nBegin], nEnd - nBegin, FileInfo.m_szFileName, MAX_BUFF_100);
                FileInfo.m_szFileName[nEnd - nBegin] = '\0';
                nBegin = i + 1;
                //模块参数
                nEnd = nLen;
                memcpy_safe((char*)&pFile[nBegin], nEnd - nBegin, FileInfo.m_szFileParam, MAX_BUFF_200);
                FileInfo.m_szFileParam[nEnd - nBegin] = '\0';
                break;
            }
        }
    }

    return true;
}

bool GetForbiddenIP(const char* pCommand, _ForbiddenIP& ForbiddenIP)
{
    char szTempData[MAX_BUFF_100] = { '\0' };

    //获得IP地址
    char* pPosBegin = (char*)ACE_OS::strstr(pCommand, "-c ");
    char* pPosEnd = (char*)ACE_OS::strstr(pPosBegin + 3, " ");
    int nLen = (int)(pPosEnd - pPosBegin - 3);

    if (nLen >= MAX_BUFF_100 || nLen < 0)
    {
        return false;
    }

    memcpy_safe(pPosBegin + 3, (uint32)nLen, szTempData, (uint32)MAX_BUFF_100);
    szTempData[nLen] = '\0';
    sprintf_safe(ForbiddenIP.m_szClientIP, MAX_IP_SIZE, szTempData);

    pPosBegin = (char*)ACE_OS::strstr(pCommand, "-t ");
    pPosEnd = (char*)ACE_OS::strstr(pPosBegin + 3, " ");
    nLen = (int)(pPosEnd - pPosBegin - 3);

    if (nLen >= MAX_BUFF_100 || nLen < 0)
    {
        return false;
    }

    memcpy_safe(pPosBegin + 3, (uint32)nLen, szTempData, (uint32)MAX_BUFF_100);
    szTempData[nLen] = '\0';
    ForbiddenIP.m_u1Type = (uint8)ACE_OS::atoi(szTempData);

    pPosBegin = (char*)ACE_OS::strstr(pCommand, "-s ");
    pPosEnd = (char*)ACE_OS::strstr(pPosBegin + 3, " ");
    nLen = (int)(pPosEnd - pPosBegin - 3);

    if (nLen >= MAX_BUFF_100 || nLen < 0)
    {
        return false;
    }

    memcpy_safe(pPosBegin + 3, (uint32)nLen, szTempData, (uint32)MAX_BUFF_100);
    szTempData[nLen] = '\0';
    ForbiddenIP.m_u4Second = (uint32)ACE_OS::atoi(szTempData);

    return true;
}

bool GetTrackIP(const char* pCommand, _ForbiddenIP& ForbiddenIP)
{
    char szTempData[MAX_BUFF_100] = { '\0' };

    //获得IP地址
    char* pPosBegin = (char*)ACE_OS::strstr(pCommand, "-c ");
    char* pPosEnd = (char*)ACE_OS::strstr(pPosBegin + 3, " ");
    int nLen = (int)(pPosEnd - pPosBegin - 3);

    if (nLen >= MAX_BUFF_100 || nLen < 0)
    {
        return false;
    }

    memcpy_safe(pPosBegin + 3, (uint32)nLen, szTempData, (uint32)MAX_BUFF_100);
    szTempData[nLen] = '\0';
    sprintf_safe(ForbiddenIP.m_szClientIP, MAX_IP_SIZE, szTempData);

    return true;
}

bool GetLogLevel(const char* pCommand, int& nLogLevel)
{
    //获得日志等级
    char* pPosBegin = (char*)ACE_OS::strstr(pCommand, "-l ");

    if (NULL != pPosBegin)
    {
        char szTempData[MAX_BUFF_100] = { '\0' };
        int nLen = (int)ACE_OS::strlen(pCommand) - (int)(pPosBegin - pCommand) - 3;
        memcpy_safe(pPosBegin + 3, (uint32)nLen, szTempData, (uint32)MAX_BUFF_100);
        nLogLevel = ACE_OS::atoi(szTempData);
    }

    return true;
}

bool GetAIInfo(const char* pCommand, int& nAI, int& nDispose, int& nCheck, int& nStop)
{
    int nIndex = 0;
    int nBegin = 0;
    int nEnd = 0;
    char szTemp[MAX_BUFF_20] = { '\0' };

    nBegin = 3;
    ACE_OS::memset(szTemp, 0, MAX_BUFF_20);

    for (int i = 3; i < (int)ACE_OS::strlen(pCommand); i++)
    {
        if (pCommand[i] == ',')
        {
            nEnd = i;
            memcpy_safe((char*)&pCommand[nBegin], (uint32)(nEnd - nBegin), szTemp, (uint32)MAX_BUFF_20);

            if (nIndex == 0)
            {
                nAI = ACE_OS::atoi(szTemp);
            }
            else if (nIndex == 1)
            {
                nDispose = ACE_OS::atoi(szTemp);
            }
            else if (nIndex == 2)
            {
                nCheck = ACE_OS::atoi(szTemp);
            }

            ACE_OS::memset(szTemp, 0, MAX_BUFF_20);
            nBegin = i + 1;
            nIndex++;
        }
    }

    //最后一个参数
    memcpy_safe((char*)&pCommand[nBegin], (uint32)(ACE_OS::strlen(pCommand) - nBegin), szTemp, (uint32)MAX_BUFF_20);
    nStop = ACE_OS::atoi(szTemp);

    return true;
}

bool GetNickName(const char* pCommand, char* pName)
{
    //获得别名
    char* pPosBegin = (char*)ACE_OS::strstr(pCommand, "-n ");

    if (NULL != pPosBegin)
    {
        int nLen = (int)ACE_OS::strlen(pCommand) - (int)(pPosBegin - pCommand) - 3;
        memcpy_safe((char*)(pPosBegin + 3), (uint32)nLen, pName, (uint32)MAX_BUFF_100);
        pName[nLen] = '\0';
    }

    return true;
}

bool GetConnectID(const char* pCommand, uint32& u4ConnectID, bool& blFlag)
{
    char szTempData[MAX_BUFF_100] = { '\0' };
    int  nFlag = 0;

    //获得ConnectID
    char* pPosBegin = (char*)ACE_OS::strstr(pCommand, "-n ");
    char* pPosEnd = (char*)ACE_OS::strstr(pPosBegin + 3, " ");
    int nLen = (int)(pPosEnd - pPosBegin - 3);

    if (nLen >= MAX_BUFF_100 || nLen < 0)
    {
        return false;
    }

    memcpy_safe((char*)(pPosBegin + 3), (uint32)nLen, szTempData, (uint32)MAX_BUFF_100);
    szTempData[nLen] = '\0';
    u4ConnectID = (uint32)ACE_OS::atoi(szTempData);

    pPosBegin = (char*)ACE_OS::strstr(pCommand, "-f ");
    pPosEnd = (char*)ACE_OS::strstr(pPosBegin + 3, " ");
    nLen = (int)(pPosEnd - pPosBegin - 3);

    if (nLen >= MAX_BUFF_100 || nLen < 0)
    {
        return false;
    }

    memcpy_safe((char*)(pPosBegin + 3), (uint32)nLen, szTempData, (uint32)MAX_BUFF_100);
    szTempData[nLen] = '\0';
    nFlag = (int)ACE_OS::atoi(szTempData);

    if (nFlag == 0)
    {
        blFlag = false;
    }
    else
    {
        blFlag = true;
    }

    return true;
}

bool GetMaxConnectCount(const char* pCommand, uint16& u2MaxConnectCount)
{
    char szTempData[MAX_BUFF_100] = { '\0' };

    //获得ConnectID
    char* pPosBegin = (char*)ACE_OS::strstr(pCommand, "-n ");
    char* pPosEnd = (char*)ACE_OS::strstr(pPosBegin + 3, " ");
    int nLen = (int)(pPosEnd - pPosBegin - 3);

    if (nLen >= MAX_BUFF_100 || nLen < 0)
    {
        return false;
    }

    memcpy_safe((char*)(pPosBegin + 3), (uint32)nLen, szTempData, (uint32)MAX_BUFF_100);
    szTempData[nLen] = '\0';
    u2MaxConnectCount = (uint16)ACE_OS::atoi(szTempData);

    return true;
}

bool GetConnectServerID(const char* pCommand, int& nServerID)
{
    char szTempData[MAX_BUFF_100] = { '\0' };

    int nCommandLen = (int)ACE_OS::strlen(pCommand);
    //获得IP地址
    char* pPosBegin = (char*)ACE_OS::strstr(pCommand, "-s ");
    int nLen = (int)(nCommandLen - (pPosBegin - pCommand) - 3);

    if (nLen >= MAX_BUFF_100 || nLen < 0)
    {
        return false;
    }

    memcpy_safe((char*)(pPosBegin + 3), (uint32)nLen, szTempData, (uint32)MAX_BUFF_100);

    nServerID = ACE_OS::atoi(szTempData);

    return true;
}

bool GetListenInfo(const char* pCommand, _ListenInfo& objListenInfo)
{
    char szTempData[MAX_BUFF_100] = { '\0' };

    //获得IP地址
    char* pPosBegin = (char*)ACE_OS::strstr(pCommand, "-i ");
    char* pPosEnd = (char*)ACE_OS::strstr(pPosBegin + 3, " ");
    int nLen = (int)(pPosEnd - pPosBegin - 3);

    if (nLen >= MAX_BUFF_100 || nLen < 0)
    {
        return false;
    }

    memcpy_safe((char*)(pPosBegin + 3), (uint32)nLen, szTempData, (uint32)MAX_BUFF_100);
    szTempData[nLen] = '\0';
    sprintf_safe(objListenInfo.m_szListenIP, MAX_BUFF_20, szTempData);

    //获得Port
    pPosBegin = (char*)ACE_OS::strstr(pCommand, "-p ");
    pPosEnd = (char*)ACE_OS::strstr(pPosBegin + 3, " ");
    nLen = (int)(pPosEnd - pPosBegin - 3);

    if (nLen >= MAX_BUFF_100 || nLen < 0)
    {
        return false;
    }

    memcpy_safe((char*)(pPosBegin + 3), (uint32)nLen, szTempData, (uint32)MAX_BUFF_100);
    szTempData[nLen] = '\0';
    objListenInfo.m_u4Port = ACE_OS::atoi(szTempData);

    //获得IP类型
    pPosBegin = (char*)ACE_OS::strstr(pCommand, "-t ");
    pPosEnd = (char*)ACE_OS::strstr(pPosBegin + 3, " ");
    nLen = (int)(pPosEnd - pPosBegin - 3);

    if (nLen >= MAX_BUFF_100 || nLen < 0)
    {
        return false;
    }

    memcpy_safe((char*)(pPosBegin + 3), (uint32)nLen, szTempData, (uint32)MAX_BUFF_100);
    szTempData[nLen] = '\0';
    objListenInfo.m_u1IPType = ACE_OS::atoi(szTempData);

    //获得PacketParseID
    pPosBegin = (char*)ACE_OS::strstr(pCommand, "-n ");
    pPosEnd = (char*)ACE_OS::strstr(pPosBegin + 3, " ");
    nLen = (int)(pPosEnd - pPosBegin - 3);

    if (nLen >= MAX_BUFF_100 || nLen < 0)
    {
        return false;
    }

    memcpy_safe((char*)(pPosBegin + 3), (uint32)nLen, szTempData, (uint32)MAX_BUFF_100);
    szTempData[nLen] = '\0';
    objListenInfo.m_u4PacketParseID = ACE_OS::atoi(szTempData);
    return true;
}

bool GetTestFileName(const char* pCommand, char* pFileName)
{
    int nCommandSize = (int)ACE_OS::strlen(pCommand);
    char* pPosBegin = (char*)ACE_OS::strstr(pCommand, "-f ");
    uint16 u2Len = (uint16)(nCommandSize - (int(pPosBegin - pCommand) + 3));

    if (u2Len < MAX_BUFF_200 && u2Len > 0)
    {
        memcpy_safe((char*)(pPosBegin + 3), (uint32)u2Len, pFileName, (uint32)MAX_BUFF_200);
        pFileName[u2Len] = '\0';
        return true;
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[GetTestFileName]nCommandSize=%d is more than MAX_BUFF_200 or is zero.\n", u2Len));
        return false;
    }
}

bool GetDyeingIP(const char* pCommand, _DyeIPInfo& objDyeIPInfo)
{
    char szTempData[MAX_BUFF_100] = { '\0' };

    //获得IP地址
    char* pPosBegin = (char*)ACE_OS::strstr(pCommand, "-i ");
    char* pPosEnd = (char*)ACE_OS::strstr(pPosBegin + 3, " ");
    int nLen = (int)(pPosEnd - pPosBegin - 3);

    if (nLen >= MAX_BUFF_100 || nLen < 0)
    {
        return false;
    }

    memcpy_safe((char*)(pPosBegin + 3), (uint32)nLen, szTempData, (uint32)MAX_BUFF_100);
    szTempData[nLen] = '\0';
    sprintf_safe(objDyeIPInfo.m_szClientIP, MAX_BUFF_20, szTempData);

    //获得当前个数
    pPosBegin = (char*)ACE_OS::strstr(pCommand, "-c ");
    pPosEnd = (char*)ACE_OS::strstr(pPosBegin + 3, " ");
    nLen = (int)(pPosEnd - pPosBegin - 3);

    if (nLen >= MAX_BUFF_100 || nLen < 0)
    {
        return false;
    }

    memcpy_safe((char*)(pPosBegin + 3), (uint32)nLen, szTempData, (uint32)MAX_BUFF_100);
    szTempData[nLen] = '\0';
    objDyeIPInfo.m_u2MaxCount = ACE_OS::atoi(szTempData);

    return true;
}

bool GetDyeingCommand(const char* pCommand, _DyeCommandInfo& objDyeCommandInfo)
{
    char szCommandID[MAX_BUFF_20] = { '\0' };
    char szTempData[MAX_BUFF_100] = { '\0' };

    //获得CommandID
    char* pPosBegin = (char*)ACE_OS::strstr(pCommand, "-i ");
    char* pPosEnd = (char*)ACE_OS::strstr(pPosBegin + 3, " ");
    int nLen = (int)(pPosEnd - pPosBegin - 3);

    if (nLen >= MAX_BUFF_100 || nLen < 0)
    {
        return false;
    }

    memcpy_safe((char*)(pPosBegin + 3), (uint32)nLen, szTempData, (uint32)MAX_BUFF_100);
    szTempData[nLen] = '\0';

    if (szTempData[0] != '0' && szTempData[1] != 'x')
    {
        return false;
    }

    if (false == memcpy_safe(&szTempData[2], (uint32)(ACE_OS::strlen(szTempData) - 2), szCommandID, MAX_BUFF_20))
    {
        return false;
    }

    objDyeCommandInfo.m_u2CommandID = (uint16)ACE_OS::strtol(szCommandID, NULL, 16);

    //获得当前个数
    pPosBegin = (char*)ACE_OS::strstr(pCommand, "-c ");
    pPosEnd = (char*)ACE_OS::strstr(pPosBegin + 3, " ");
    nLen = (int)(pPosEnd - pPosBegin - 3);

    if (nLen >= MAX_BUFF_100 || nLen < 0)
    {
        return false;
    }

    memcpy_safe((char*)(pPosBegin + 3), (uint32)nLen, szTempData, (uint32)MAX_BUFF_100);
    szTempData[nLen] = '\0';
    objDyeCommandInfo.m_u2MaxCount = ACE_OS::atoi(szTempData);

    return true;
}

bool GetPoolSet(const char* pCommand, _PoolName& objPoolName)
{
    char szTempData[MAX_BUFF_100] = { '\0' };

    //获得内存池创建设置
    char* pPosBegin = (char*)ACE_OS::strstr(pCommand, "-n ");
    char* pPosEnd = (char*)ACE_OS::strstr(pPosBegin + 3, " ");
    int nLen = (int)(pPosEnd - pPosBegin - 3);

    if (nLen >= MAX_BUFF_50 || nLen < 0)
    {
        return false;
    }

    memcpy_safe((char*)(pPosBegin + 3), (uint32)nLen, objPoolName.m_szPoolName, (uint32)MAX_BUFF_50);
    objPoolName.m_szPoolName[nLen] = '\0';

    //获得当前个数
    pPosBegin = (char*)ACE_OS::strstr(pCommand, "-b ");
    pPosEnd = (char*)ACE_OS::strstr(pPosBegin + 3, " ");
    nLen = (int)(pPosEnd - pPosBegin - 3);

    if (nLen >= MAX_BUFF_50 || nLen < 0)
    {
        return false;
    }

    memcpy_safe((char*)(pPosBegin + 3), (uint32)nLen, szTempData, (uint32)MAX_BUFF_100);
    szTempData[nLen] = '\0';

    if (ACE_OS::strcmp(szTempData, "true") == 0)
    {
        objPoolName.m_blState = true;
    }
    else
    {
        objPoolName.m_blState = false;
    }

    return true;
}

bool GetDebug(const char* pCommand, uint8& u1Debug)
{
    char szTempData[MAX_BUFF_100] = { '\0' };

    int nCommandLen = (int)ACE_OS::strlen(pCommand);
    //获得IP地址
    char* pPosBegin = (char*)ACE_OS::strstr(pCommand, "-s ");
    int nLen = (int)(nCommandLen - (pPosBegin - pCommand) - 3);

    if (nLen >= MAX_BUFF_100 || nLen < 0)
    {
        return false;
    }

    memcpy_safe((char*)(pPosBegin + 3), (uint32)nLen, szTempData, (uint32)MAX_BUFF_100);

    u1Debug = (uint8)ACE_OS::atoi(szTempData);

    return true;
}
