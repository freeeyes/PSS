#include "MessageDispose.h"

bool GetCommandParam(const char* pCommand, const char* pTag, string& strValue)
{
    strValue.clear();

    const char* pPosBegin = ACE_OS::strstr(pCommand, pTag);

    //�ж��Ƿ����ָ���Ĺؼ���
    if (nullptr == pPosBegin)
    {
        OUR_DEBUG((LM_INFO, "[GetCommandParam](%s)no find(%s) key.\n", pCommand, pTag));
        return false;
    }

    const char* pPosEnd = ACE_OS::strstr(pPosBegin + 3, " ");

    int nLen = 0;

    if (nullptr == pPosEnd)
    {
        //û���ҵ� " "��β,ֱ�Ӵ�������
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
    //���ն��Ų��
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
                //ģ��·��
                nEnd = i;
                FileInfo.m_strFilePath.append(&pFile[nBegin], (size_t)(nEnd - nBegin));
                nBegin = i + 1;
                nPosIndex++;
            }
            else if (nPosIndex == 1)
            {
                //ģ���ļ���
                nEnd = i;
                FileInfo.m_strFileName.append(&pFile[nBegin], (size_t)(nEnd - nBegin));
                nBegin = i + 1;
                //ģ�����
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

    //���IP��ַ
    GetCommandParam(pCommand, "-c ", strTemp);
    ForbiddenIP.m_strClientIP = strTemp;

    //���IP���ͣ�UDP or TCP
    GetCommandParam(pCommand, "-t ", strTemp);
    ForbiddenIP.m_u1Type = (uint8)ACE_OS::atoi(strTemp.c_str());

    //��÷��ʱ�䣬��λ��
    GetCommandParam(pCommand, "-s ", strTemp);
    ForbiddenIP.m_u4Second = (uint32)ACE_OS::atoi(strTemp.c_str());

    return true;
}

bool GetTrackIP(const char* pCommand, _ForbiddenIP& ForbiddenIP)
{
    string strTemp;

    //���IP��ַ
    GetCommandParam(pCommand, "-c ", strTemp);
    ForbiddenIP.m_strClientIP = strTemp;

    return true;
}

bool GetLogLevel(const char* pCommand, int& nLogLevel)
{
    string strTemp;

    //�����־�ȼ�
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

    //���һ������
    strTemp.append(&pCommand[nBegin], (uint32)(ACE_OS::strlen(pCommand) - nBegin));
    nStop = ACE_OS::atoi(strTemp.c_str());

    return true;
}

bool GetNickName(const char* pCommand, string& strName)
{
    //��ñ���
    GetCommandParam(pCommand, "-n ", strName);
    return true;
}

bool GetConnectID(const char* pCommand, uint32& u4ConnectID, bool& blFlag)
{
    string strTemp;
    int  nFlag = 0;

    //���ConnectID
    GetCommandParam(pCommand, "-n ", strTemp);
    u4ConnectID = (uint32)ACE_OS::atoi(strTemp.c_str());

    //��ñ��λ
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

    //���ConnectID
    GetCommandParam(pCommand, "-n ", strTemp);
    u2MaxConnectCount = (uint16)ACE_OS::atoi(strTemp.c_str());

    return true;
}

bool GetConnectServerID(const char* pCommand, int& nServerID)
{
    string strTemp;

    //���IP��ַ
    GetCommandParam(pCommand, "-s ", strTemp);
    nServerID = ACE_OS::atoi(strTemp.c_str());

    return true;
}

bool GetListenInfo(const char* pCommand, _ListenInfo& objListenInfo)
{
    string strTemp;

    //���IP��ַ
    GetCommandParam(pCommand, "-i ", strTemp);
    objListenInfo.m_strListenIP = strTemp;

    //���Port
    GetCommandParam(pCommand, "-p ", strTemp);
    objListenInfo.m_u2Port = (uint16)ACE_OS::atoi(strTemp.c_str());

    //���IP����
    GetCommandParam(pCommand, "-t ", strTemp);
    objListenInfo.m_u1IPType = (uint8)ACE_OS::atoi(strTemp.c_str());

    //���PacketParseID
    GetCommandParam(pCommand, "-n ", strTemp);
    objListenInfo.m_u4PacketParseID = ACE_OS::atoi(strTemp.c_str());
    return true;
}

bool GetTestFileName(const char* pCommand, string& strFileName)
{
    //�õ��ļ���
    return GetCommandParam(pCommand, "-f ", strFileName);
}

bool GetDyeingIP(const char* pCommand, _DyeIPInfo& objDyeIPInfo)
{
    string strTemp;

    //���IP��ַ
    GetCommandParam(pCommand, "-i ", strTemp);
    objDyeIPInfo.m_strClientIP = strTemp;

    //��õ�ǰ����
    GetCommandParam(pCommand, "-c ", strTemp);
    objDyeIPInfo.m_u2MaxCount = (uint16)ACE_OS::atoi(strTemp.c_str());

    return true;
}

bool GetDyeingCommand(const char* pCommand, _DyeCommandInfo& objDyeCommandInfo)
{
    string strTemp;
    string strCommandID;

    //���CommandID
    GetCommandParam(pCommand, "-i ", strTemp);

    if (strTemp.at(0) != '0' && strTemp.at(1) != 'x')
    {
        return false;
    }

    strCommandID.append(&strTemp.at(2), (uint32)(strTemp.length() - 2));
    objDyeCommandInfo.m_u2CommandID = (uint16)ACE_OS::strtol(strCommandID.c_str(), nullptr, 16);

    //��õ�ǰ����
    GetCommandParam(pCommand, "-c ", strTemp);
    objDyeCommandInfo.m_u2MaxCount = (uint16)ACE_OS::atoi(strTemp.c_str());

    return true;
}

bool GetPoolSet(const char* pCommand, _PoolName& objPoolName)
{
    string strTemp;

    //����ڴ�ش�������
    GetCommandParam(pCommand, "-n ", strTemp);
    objPoolName.m_strPoolName = strTemp;

    //��õ�ǰ����
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
