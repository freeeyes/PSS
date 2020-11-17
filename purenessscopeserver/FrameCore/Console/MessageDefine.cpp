#include "MessageDispose.h"

bool GetCommandParam(const char* pCommand, const char* pTag, string& strValue)
{
    strValue.clear();

    const char* pPosBegin = ACE_OS::strstr(pCommand, pTag);

    //判断是否包含指定的关键字
    if (nullptr == pPosBegin)
    {
        OUR_DEBUG((LM_INFO, "[GetCommandParam](%s)no find(%s) key.\n", pCommand, pTag));
        return false;
    }

    const char* pPosEnd = ACE_OS::strstr(pPosBegin + 3, " ");

    int nLen = 0;

    if (nullptr == pPosEnd)
    {
        //没有找到 " "结尾,直接从最后计算
        nLen = (int)(pCommand - pPosBegin - 3);
    }
    else
    {
        nLen = (int)(pPosEnd - pPosBegin - 3);
    }

    strValue.append(pPosBegin + 3, (size_t)nLen);
    return true;
}

bool GetFileInfo(const char* pFile, _FileInfo& FileInfo)
{
    //按照逗号拆分
    int nBegin = 0;
    int nEnd = 0;
    int nPosIndex = 0;

    auto nLen = (int)ACE_OS::strlen(pFile);

    for (int i = 0; i < nLen; i++)
    {
        if (pFile[i] == ',')
        {
            if (nPosIndex == 0)
            {
                //模块路径
                nEnd = i;
                FileInfo.m_strFilePath.append(&pFile[nBegin], (size_t)(nEnd - nBegin));
                nBegin = i + 1;
                nPosIndex++;
            }
            else if (nPosIndex == 1)
            {
                //模块文件名
                nEnd = i;
                FileInfo.m_strFileName.append(&pFile[nBegin], (size_t)(nEnd - nBegin));
                nBegin = i + 1;
                //模块参数
                nEnd = nLen;
                FileInfo.m_strFileParam.append(&pFile[nBegin], (size_t)(nEnd - nBegin));
                break;
            }
        }
    }

    return true;
}

bool GetForbiddenIP(const char* pCommand, _ForbiddenIP& ForbiddenIP)
{
    string strTemp;

    //获得IP地址
    GetCommandParam(pCommand, "-c ", strTemp);
    ForbiddenIP.m_strClientIP = strTemp;

    //获得IP类型，UDP or TCP
    GetCommandParam(pCommand, "-t ", strTemp);
    ForbiddenIP.m_u1Type = (uint8)ACE_OS::atoi(strTemp.c_str());

    //获得封禁时间，单位秒
    GetCommandParam(pCommand, "-s ", strTemp);
    ForbiddenIP.m_u4Second = (uint32)ACE_OS::atoi(strTemp.c_str());

    return true;
}

bool GetTrackIP(const char* pCommand, _ForbiddenIP& ForbiddenIP)
{
    string strTemp;

    //获得IP地址
    GetCommandParam(pCommand, "-c ", strTemp);
    ForbiddenIP.m_strClientIP = strTemp;

    return true;
}

bool GetLogLevel(const char* pCommand, int& nLogLevel)
{
    string strTemp;

    //获得日志等级
    GetCommandParam(pCommand, "-l ", strTemp);
    nLogLevel = ACE_OS::atoi(strTemp.c_str());
    return true;
}

bool GetAIInfo(const char* pCommand, int& nAI, int& nDispose, int& nCheck, int& nStop)
{
    int nIndex = 0;
    int nBegin = 0;
    int nEnd = 0;
    string strTemp;

    nBegin = 3;

    for (int i = 3; i < (int)ACE_OS::strlen(pCommand); i++)
    {
        if (pCommand[i] == ',')
        {
            nEnd = i;
            strTemp.append(&pCommand[nBegin], (uint32)(nEnd - nBegin));

            if (nIndex == 0)
            {
                nAI = ACE_OS::atoi(strTemp.c_str());
            }
            else if (nIndex == 1)
            {
                nDispose = ACE_OS::atoi(strTemp.c_str());
            }
            else if (nIndex == 2)
            {
                nCheck = ACE_OS::atoi(strTemp.c_str());
            }

            strTemp.clear();
            nBegin = i + 1;
            nIndex++;
        }
    }

    //最后一个参数
    strTemp.append(&pCommand[nBegin], (uint32)(ACE_OS::strlen(pCommand) - nBegin));
    nStop = ACE_OS::atoi(strTemp.c_str());

    return true;
}

bool GetNickName(const char* pCommand, string& strName)
{
    //获得别名
    GetCommandParam(pCommand, "-n ", strName);
    return true;
}

bool GetConnectID(const char* pCommand, uint32& u4ConnectID, bool& blFlag)
{
    string strTemp;
    int  nFlag = 0;

    //获得ConnectID
    GetCommandParam(pCommand, "-n ", strTemp);
    u4ConnectID = (uint32)ACE_OS::atoi(strTemp.c_str());

    //获得标记位
    GetCommandParam(pCommand, "-f ", strTemp);
    nFlag = ACE_OS::atoi(strTemp.c_str());

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
    string strTemp;

    //获得ConnectID
    GetCommandParam(pCommand, "-n ", strTemp);
    u2MaxConnectCount = (uint16)ACE_OS::atoi(strTemp.c_str());

    return true;
}

bool GetConnectServerID(const char* pCommand, int& nServerID)
{
    string strTemp;

    //获得IP地址
    GetCommandParam(pCommand, "-s ", strTemp);
    nServerID = ACE_OS::atoi(strTemp.c_str());

    return true;
}

bool GetListenInfo(const char* pCommand, _ListenInfo& objListenInfo)
{
    string strTemp;

    //获得IP地址
    GetCommandParam(pCommand, "-i ", strTemp);
    objListenInfo.m_strListenIP = strTemp;

    //获得Port
    GetCommandParam(pCommand, "-p ", strTemp);
    objListenInfo.m_u2Port = (uint16)ACE_OS::atoi(strTemp.c_str());

    //获得IP类型
    GetCommandParam(pCommand, "-t ", strTemp);
    objListenInfo.m_u1IPType = (uint8)ACE_OS::atoi(strTemp.c_str());

    //获得PacketParseID
    GetCommandParam(pCommand, "-n ", strTemp);
    objListenInfo.m_u4PacketParseID = ACE_OS::atoi(strTemp.c_str());
    return true;
}

bool GetTestFileName(const char* pCommand, string& strFileName)
{
    //得到文件名
    return GetCommandParam(pCommand, "-f ", strFileName);
}

bool GetDyeingIP(const char* pCommand, _DyeIPInfo& objDyeIPInfo)
{
    string strTemp;

    //获得IP地址
    GetCommandParam(pCommand, "-i ", strTemp);
    objDyeIPInfo.m_strClientIP = strTemp;

    //获得当前个数
    GetCommandParam(pCommand, "-c ", strTemp);
    objDyeIPInfo.m_u2MaxCount = (uint16)ACE_OS::atoi(strTemp.c_str());

    return true;
}

bool GetDyeingCommand(const char* pCommand, _DyeCommandInfo& objDyeCommandInfo)
{
    string strTemp;
    string strCommandID;

    //获得CommandID
    GetCommandParam(pCommand, "-i ", strTemp);

    if (strTemp.at(0) != '0' && strTemp.at(1) != 'x')
    {
        return false;
    }

    strCommandID.append(&strTemp.at(2), (uint32)(strTemp.length() - 2));
    objDyeCommandInfo.m_u2CommandID = (uint16)ACE_OS::strtol(strCommandID.c_str(), nullptr, 16);

    //获得当前个数
    GetCommandParam(pCommand, "-c ", strTemp);
    objDyeCommandInfo.m_u2MaxCount = (uint16)ACE_OS::atoi(strTemp.c_str());

    return true;
}

bool GetPoolSet(const char* pCommand, _PoolName& objPoolName)
{
    string strTemp;

    //获得内存池创建设置
    GetCommandParam(pCommand, "-n ", strTemp);
    objPoolName.m_strPoolName = strTemp;

    //获得当前个数
    GetCommandParam(pCommand, "-b ", strTemp);

    if (strTemp == "true")
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
    string strTemp;

    GetCommandParam(pCommand, "-s ", strTemp);
    u1Debug = (uint8)ACE_OS::atoi(strTemp.c_str());

    return true;
}
