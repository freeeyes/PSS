#include "MessageDispose.h"

bool GetCommandParam(const char* pCommand, const char* pTag, char* pData, int nMaxSize)
{
    const char* pPosBegin = ACE_OS::strstr(pCommand, pTag);

    //判断是否包含指定的关键字
    if (NULL == pPosBegin)
    {
        OUR_DEBUG((LM_INFO, "[GetCommandParam](%s)no find(%s) key.\n", pCommand, pTag));
        return false;
    }

    const char* pPosEnd = ACE_OS::strstr(pPosBegin + 3, " ");

    int nLen = 0;

    if (NULL == pPosEnd)
    {
        //没有找到 " "结尾,直接从最后计算
        nLen = (int)(pCommand - pPosBegin - 3);
    }
    else
    {
        nLen = (int)(pPosEnd - pPosBegin - 3);
    }

    if (nLen >= nMaxSize || nLen < 0)
    {
        OUR_DEBUG((LM_INFO, "[GetCommandParam](%s)[%s]nLen is more key.\n", pCommand, pTag));
        return false;
    }

    memcpy_safe(pPosBegin + 3, (uint32)nLen, pData, (uint32)nMaxSize);
    pData[nLen] = '\0';
    return true;
}

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
                memcpy_safe(&pFile[nBegin], nEnd - nBegin, FileInfo.m_szFilePath, MAX_BUFF_100, true);
                nBegin = i + 1;
                nPosIndex++;
            }
            else if (nPosIndex == 1)
            {
                //模块文件名
                nEnd = i;
                memcpy_safe(&pFile[nBegin], nEnd - nBegin, FileInfo.m_szFileName, MAX_BUFF_100, true);
                nBegin = i + 1;
                //模块参数
                nEnd = nLen;
                memcpy_safe(&pFile[nBegin], nEnd - nBegin, FileInfo.m_szFileParam, MAX_BUFF_200, true);
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
    GetCommandParam(pCommand, "-c ", szTempData, MAX_BUFF_100);
    sprintf_safe(ForbiddenIP.m_szClientIP, MAX_IP_SIZE, szTempData);

    //获得IP类型，UDP or TCP
    GetCommandParam(pCommand, "-t ", szTempData, MAX_BUFF_100);
    ForbiddenIP.m_u1Type = (uint8)ACE_OS::atoi(szTempData);

    //获得封禁时间，单位秒
    GetCommandParam(pCommand, "-s ", szTempData, MAX_BUFF_100);
    ForbiddenIP.m_u4Second = (uint32)ACE_OS::atoi(szTempData);

    return true;
}

bool GetTrackIP(const char* pCommand, _ForbiddenIP& ForbiddenIP)
{
    char szTempData[MAX_BUFF_100] = { '\0' };

    //获得IP地址
    GetCommandParam(pCommand, "-c ", szTempData, MAX_BUFF_100);
    sprintf_safe(ForbiddenIP.m_szClientIP, MAX_IP_SIZE, szTempData);

    return true;
}

bool GetLogLevel(const char* pCommand, int& nLogLevel)
{
    char szTempData[MAX_BUFF_100] = { '\0' };

    //获得日志等级
    GetCommandParam(pCommand, "-l ", szTempData, MAX_BUFF_100);
    nLogLevel = ACE_OS::atoi(szTempData);
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
            memcpy_safe(&pCommand[nBegin], (uint32)(nEnd - nBegin), szTemp, (uint32)MAX_BUFF_20);

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
    memcpy_safe(&pCommand[nBegin], (uint32)(ACE_OS::strlen(pCommand) - nBegin), szTemp, (uint32)MAX_BUFF_20);
    nStop = ACE_OS::atoi(szTemp);

    return true;
}

bool GetNickName(const char* pCommand, char* pName)
{
    //获得别名
    GetCommandParam(pCommand, "-n ", pName, MAX_BUFF_100);
    return true;
}

bool GetConnectID(const char* pCommand, uint32& u4ConnectID, bool& blFlag)
{
    char szTempData[MAX_BUFF_100] = { '\0' };
    int  nFlag = 0;

    //获得ConnectID
    GetCommandParam(pCommand, "-n ", szTempData, MAX_BUFF_100);
    u4ConnectID = (uint32)ACE_OS::atoi(szTempData);

    //获得标记位
    GetCommandParam(pCommand, "-f ", szTempData, MAX_BUFF_100);
    nFlag = ACE_OS::atoi(szTempData);

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
    GetCommandParam(pCommand, "-n ", szTempData, MAX_BUFF_100);
    u2MaxConnectCount = (uint16)ACE_OS::atoi(szTempData);

    return true;
}

bool GetConnectServerID(const char* pCommand, int& nServerID)
{
    char szTempData[MAX_BUFF_100] = { '\0' };

    //获得IP地址
    GetCommandParam(pCommand, "-s ", szTempData, MAX_BUFF_100);
    nServerID = ACE_OS::atoi(szTempData);

    return true;
}

bool GetListenInfo(const char* pCommand, _ListenInfo& objListenInfo)
{
    char szTempData[MAX_BUFF_100] = { '\0' };

    //获得IP地址
    GetCommandParam(pCommand, "-i ", szTempData, MAX_BUFF_100);
    sprintf_safe(objListenInfo.m_szListenIP, MAX_BUFF_20, szTempData);

    //获得Port
    GetCommandParam(pCommand, "-p ", szTempData, MAX_BUFF_100);
    objListenInfo.m_u4Port = ACE_OS::atoi(szTempData);

    //获得IP类型
    GetCommandParam(pCommand, "-t ", szTempData, MAX_BUFF_100);
    objListenInfo.m_u1IPType = (uint8)ACE_OS::atoi(szTempData);

    //获得PacketParseID
    GetCommandParam(pCommand, "-n ", szTempData, MAX_BUFF_100);
    objListenInfo.m_u4PacketParseID = ACE_OS::atoi(szTempData);
    return true;
}

bool GetTestFileName(const char* pCommand, char* pFileName)
{
    //得到文件名
    return GetCommandParam(pCommand, "-f ", pFileName, MAX_BUFF_200);
}

bool GetDyeingIP(const char* pCommand, _DyeIPInfo& objDyeIPInfo)
{
    char szTempData[MAX_BUFF_100] = { '\0' };

    //获得IP地址
    GetCommandParam(pCommand, "-i ", szTempData, MAX_BUFF_100);
    sprintf_safe(objDyeIPInfo.m_szClientIP, MAX_BUFF_20, szTempData);

    //获得当前个数
    GetCommandParam(pCommand, "-c ", szTempData, MAX_BUFF_100);
    objDyeIPInfo.m_u2MaxCount = (uint16)ACE_OS::atoi(szTempData);

    return true;
}

bool GetDyeingCommand(const char* pCommand, _DyeCommandInfo& objDyeCommandInfo)
{
    char szCommandID[MAX_BUFF_20] = { '\0' };
    char szTempData[MAX_BUFF_100] = { '\0' };

    //获得CommandID
    GetCommandParam(pCommand, "-i ", szTempData, MAX_BUFF_100);

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
    GetCommandParam(pCommand, "-c ", szTempData, MAX_BUFF_100);
    objDyeCommandInfo.m_u2MaxCount = (uint16)ACE_OS::atoi(szTempData);

    return true;
}

bool GetPoolSet(const char* pCommand, _PoolName& objPoolName)
{
    char szTempData[MAX_BUFF_100] = { '\0' };

    //获得内存池创建设置
    GetCommandParam(pCommand, "-n ", szTempData, MAX_BUFF_100);
    uint32 u4SrcSize = (uint32)ACE_OS::strlen(szTempData);
    memcpy_safe(szTempData, u4SrcSize, objPoolName.m_szPoolName, (uint32)MAX_BUFF_50, true);

    //获得当前个数
    GetCommandParam(pCommand, "-b ", szTempData, MAX_BUFF_100);

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

    GetCommandParam(pCommand, "-s ", szTempData, MAX_BUFF_100);
    u1Debug = (uint8)ACE_OS::atoi(szTempData);

    return true;
}
