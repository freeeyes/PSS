#include "MessageDispose.h"

bool GetCommandParam(const string& strCommand, const char* pTag, string& strValue)
{
    strValue.clear();

    const char* pPosBegin = ACE_OS::strstr(strCommand.c_str(), pTag);

    //�ж��Ƿ����ָ���Ĺؼ���
    if (nullptr == pPosBegin)
    {
        PSS_LOGGER_DEBUG("[GetCommandParam]({0})no find({1}) key.", strCommand, pTag);
        return false;
    }

    const char* pPosEnd = ACE_OS::strstr(pPosBegin + 3, " ");

    int nLen = 0;

    if (nullptr == pPosEnd)
    {
        //û���ҵ� " "��β,ֱ�Ӵ�������
        nLen = (int)(strCommand.length() - 3);
    }
    else
    {
        nLen = (int)(pPosEnd - pPosBegin - 3);
    }

    strValue.append(pPosBegin + 3, (size_t)nLen);
    return true;
}

bool GetFileInfo(const string& strFile, _FileInfo& FileInfo)
{
    //���ն��Ų��
    int nBegin = 0;
    int nEnd = 0;
    int nPosIndex = 0;

    auto nLen = (int)strFile.length();

    for (int i = 0; i < nLen; i++)
    {
        if (strFile[i] == ',')
        {
            if (nPosIndex == 0)
            {
                //ģ��·��
                nEnd = i;
                FileInfo.m_strFilePath.append(&strFile[nBegin], (size_t)(nEnd - nBegin));
                nBegin = i + 1;
                nPosIndex++;
            }
            else if (nPosIndex == 1)
            {
                //ģ���ļ���
                nEnd = i;
                FileInfo.m_strFileName.append(&strFile[nBegin], (size_t)(nEnd - nBegin));
                nBegin = i + 1;
                //ģ�����
                nEnd = nLen;
                FileInfo.m_strFileParam.append(&strFile[nBegin], (size_t)(nEnd - nBegin));
                break;
            }
        }
    }

    return true;
}

bool GetForbiddenIP(const string& strCommand, _ForbiddenIP& ForbiddenIP)
{
    string strTemp;

    //���IP��ַ
    GetCommandParam(strCommand, "-c ", strTemp);
    ForbiddenIP.m_strClientIP = strTemp;

    //���IP���ͣ�UDP or TCP
    GetCommandParam(strCommand, "-t ", strTemp);
    ForbiddenIP.m_u1Type = (uint8)ACE_OS::atoi(strTemp.c_str());

    //��÷��ʱ�䣬��λ��
    GetCommandParam(strCommand, "-s ", strTemp);
    ForbiddenIP.m_u4Second = (uint32)ACE_OS::atoi(strTemp.c_str());

    return true;
}

bool GetTrackIP(const string& strCommand, _ForbiddenIP& ForbiddenIP)
{
    string strTemp;

    //���IP��ַ
    GetCommandParam(strCommand, "-c ", strTemp);
    ForbiddenIP.m_strClientIP = strTemp;

    return true;
}

bool GetLogLevel(const string& strCommand, int& nLogLevel)
{
    string strTemp;

    //�����־�ȼ�
    GetCommandParam(strCommand, "-l ", strTemp);
    nLogLevel = ACE_OS::atoi(strTemp.c_str());
    return true;
}

bool GetAIInfo(const string& strCommand, int& nAI, int& nDispose, int& nCheck, int& nStop)
{
    int nIndex = 0;
    int nBegin = 0;
    int nEnd = 0;
    string strTemp;

    nBegin = 3;

    for (int i = 3; i < (int)strCommand.length(); i++)
    {
        if (strCommand[i] == ',')
        {
            nEnd = i;
            strTemp.append(&strCommand[nBegin], (uint32)(nEnd - nBegin));

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
    strTemp.append(&strCommand[nBegin], (uint32)(strCommand.length() - nBegin));
    nStop = ACE_OS::atoi(strTemp.c_str());

    return true;
}

bool GetNickName(const string& strCommand, string& strName)
{
    //��ñ���
    GetCommandParam(strCommand, "-n ", strName);
    return true;
}

bool GetConnectID(const string& strCommand, uint32& u4ConnectID, bool& blFlag)
{
    string strTemp;
    int  nFlag = 0;

    //���ConnectID
    GetCommandParam(strCommand, "-n ", strTemp);
    u4ConnectID = (uint32)ACE_OS::atoi(strTemp.c_str());

    //��ñ��λ
    GetCommandParam(strCommand, "-f ", strTemp);
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

bool GetMaxConnectCount(const string& strCommand, uint16& u2MaxConnectCount)
{
    string strTemp;

    //���ConnectID
    GetCommandParam(strCommand, "-n ", strTemp);
    u2MaxConnectCount = (uint16)ACE_OS::atoi(strTemp.c_str());

    return true;
}

bool GetConnectServerID(const string& strCommand, int& nServerID)
{
    string strTemp;

    //���IP��ַ
    GetCommandParam(strCommand, "-s ", strTemp);
    nServerID = ACE_OS::atoi(strTemp.c_str());

    return true;
}

bool GetListenInfo(const string& strCommand, _ListenInfo& objListenInfo)
{
    string strTemp;

    //���IP��ַ
    GetCommandParam(strCommand, "-i ", strTemp);
    objListenInfo.m_strListenIP = strTemp;

    //���Port
    GetCommandParam(strCommand, "-p ", strTemp);
    objListenInfo.m_u2Port = (uint16)ACE_OS::atoi(strTemp.c_str());

    //���IP����
    GetCommandParam(strCommand, "-t ", strTemp);
    objListenInfo.m_u1IPType = (uint8)ACE_OS::atoi(strTemp.c_str());

    //���PacketParseID
    GetCommandParam(strCommand, "-n ", strTemp);
    objListenInfo.m_u4PacketParseID = ACE_OS::atoi(strTemp.c_str());
    return true;
}

bool GetTestFileName(const string& strCommand, string& strFileName)
{
    //�õ��ļ���
    return GetCommandParam(strCommand, "-f ", strFileName);
}

bool GetDyeingIP(const string& strCommand, _DyeIPInfo& objDyeIPInfo)
{
    string strTemp;

    //���IP��ַ
    GetCommandParam(strCommand, "-i ", strTemp);
    objDyeIPInfo.m_strClientIP = strTemp;

    //��õ�ǰ����
    GetCommandParam(strCommand, "-c ", strTemp);
    objDyeIPInfo.m_u2MaxCount = (uint16)ACE_OS::atoi(strTemp.c_str());

    return true;
}

bool GetDyeingCommand(const string& strCommand, _DyeCommandInfo& objDyeCommandInfo)
{
    string strTemp;
    string strCommandID;

    //���CommandID
    GetCommandParam(strCommand, "-i ", strTemp);

    if (strTemp.at(0) != '0' && strTemp.at(1) != 'x')
    {
        return false;
    }

    strCommandID.append(&strTemp.at(2), (uint32)(strTemp.length() - 2));
    objDyeCommandInfo.m_u2CommandID = (uint16)ACE_OS::strtol(strCommandID.c_str(), nullptr, 16);

    //��õ�ǰ����
    GetCommandParam(strCommand, "-c ", strTemp);
    objDyeCommandInfo.m_u2MaxCount = (uint16)ACE_OS::atoi(strTemp.c_str());

    return true;
}

bool GetPoolSet(const string& strCommand, _PoolName& objPoolName)
{
    string strTemp;

    //����ڴ�ش�������
    GetCommandParam(strCommand, "-n ", strTemp);
    objPoolName.m_strPoolName = strTemp;

    //��õ�ǰ����
    GetCommandParam(strCommand, "-b ", strTemp);

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

bool GetDebug(const string& strCommand, uint8& u1Debug)
{
    string strTemp;

    GetCommandParam(strCommand, "-s ", strTemp);
    u1Debug = (uint8)ACE_OS::atoi(strTemp.c_str());

    return true;
}
