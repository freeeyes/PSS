#include "LoadPacketParse.h"

CLoadPacketParse::CLoadPacketParse()
{
}

void CLoadPacketParse::Init(int nCount)
{
    Close();
    //��ʼ��HashTable
    m_objPacketParseList.Init(nCount);
}

bool CLoadPacketParse::LoadPacketInfo(uint32 u4PacketParseID, uint8 u1Type, uint32 u4HeadLen, const char* pPacketParsePath, const char* szPacketParseName)
{
    int nRet = 0;
    //��ʽ����PacketParse�ӿ�
    _Packet_Parse_Info* pPacketParseInfo = new _Packet_Parse_Info();

    if (nullptr == pPacketParseInfo)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] pPacketParseInfo is nullptr!\n"));
        return false;
    }

    pPacketParseInfo->m_u4PacketParseID   = u4PacketParseID;
    pPacketParseInfo->m_u1PacketParseType = u1Type;
    pPacketParseInfo->m_u4OrgLength       = u4HeadLen;
    char szFilePath[MAX_BUFF_500]         = { '\0' };
    sprintf_safe(szFilePath, MAX_BUFF_500, "%s%s", pPacketParsePath, szPacketParseName);
    pPacketParseInfo->m_hModule           = ACE_OS::dlopen((ACE_TCHAR*)szFilePath, RTLD_NOW);

    if(nullptr == pPacketParseInfo->m_hModule)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, pModuleInfo->hModule is nullptr(%s)!\n", szPacketParseName, ACE_OS::dlerror()));
        SAFE_DELETE(pPacketParseInfo);
        return false;
    }

    pPacketParseInfo->Parse_Packet_Head_Info = (bool(*)(uint32, ACE_Message_Block*, IMessageBlockManager*, _Head_Info*, EM_CONNECT_IO_TYPE))ACE_OS::dlsym(pPacketParseInfo->m_hModule, "Parse_Packet_Head_Info");

    if(nullptr == pPacketParseInfo->m_hModule || !pPacketParseInfo->Parse_Packet_Head_Info)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, Function Parse_Packet_Head_Info is error!\n", szPacketParseName));
        nRet = ACE_OS::dlclose(pPacketParseInfo->m_hModule);
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo]PacketID=%d, ret=%d.\n", pPacketParseInfo->m_u4PacketParseID, nRet));
        SAFE_DELETE(pPacketParseInfo);
        return false;
    }

    pPacketParseInfo->Parse_Packet_Body_Info = (bool(*)(uint32, ACE_Message_Block*, IMessageBlockManager*, _Body_Info*, EM_CONNECT_IO_TYPE))ACE_OS::dlsym(pPacketParseInfo->m_hModule, "Parse_Packet_Body_Info");

    if(nullptr == pPacketParseInfo->m_hModule || !pPacketParseInfo->Parse_Packet_Body_Info)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, Function Parse_Packet_Body_Info is error!\n", szPacketParseName));
        nRet = ACE_OS::dlclose(pPacketParseInfo->m_hModule);
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo]PacketID=%d, ret=%d.\n", pPacketParseInfo->m_u4PacketParseID, nRet));
        SAFE_DELETE(pPacketParseInfo);
        return false;
    }

    pPacketParseInfo->Parse_Packet_Stream = (uint8(*)(uint32, ACE_Message_Block*, IMessageBlockManager*, _Packet_Info*, EM_CONNECT_IO_TYPE))ACE_OS::dlsym(pPacketParseInfo->m_hModule, "Parse_Packet_Stream");

    if(nullptr == pPacketParseInfo->m_hModule || !pPacketParseInfo->Parse_Packet_Stream)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, Function Parse_Packet_Stream is error!\n", szPacketParseName));
        nRet = ACE_OS::dlclose(pPacketParseInfo->m_hModule);
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo]PacketID=%d, ret=%d.\n", pPacketParseInfo->m_u4PacketParseID, nRet));
        SAFE_DELETE(pPacketParseInfo);
        return false;
    }

    pPacketParseInfo->Make_Send_Packet = (bool(*)(uint32, const char*, uint32, ACE_Message_Block*, uint16))ACE_OS::dlsym(pPacketParseInfo->m_hModule, "Make_Send_Packet");

    if(nullptr == pPacketParseInfo->m_hModule || !pPacketParseInfo->Make_Send_Packet)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, Function Make_Send_Packet is error!\n", szPacketParseName));
        nRet = ACE_OS::dlclose(pPacketParseInfo->m_hModule);
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo]PacketID=%d, ret=%d.\n", pPacketParseInfo->m_u4PacketParseID, nRet));
        SAFE_DELETE(pPacketParseInfo);
        return false;
    }

    pPacketParseInfo->Make_Send_Packet_Length = (uint32(*)(uint32, uint32, uint16))ACE_OS::dlsym(pPacketParseInfo->m_hModule, "Make_Send_Packet_Length");

    if(nullptr == pPacketParseInfo->m_hModule || !pPacketParseInfo->Make_Send_Packet_Length)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, Function Make_Send_Packet_Length is error!\n", szPacketParseName));
        nRet = ACE_OS::dlclose(pPacketParseInfo->m_hModule);
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo]PacketID=%d, ret=%d.\n", pPacketParseInfo->m_u4PacketParseID, nRet));
        SAFE_DELETE(pPacketParseInfo);
        return false;
    }

    pPacketParseInfo->Connect = (bool(*)(uint32, _ClientIPInfo, _ClientIPInfo const&))ACE_OS::dlsym(pPacketParseInfo->m_hModule, "Connect");

    if(nullptr == pPacketParseInfo->m_hModule || !pPacketParseInfo->Connect)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, Function Connect is error!\n", szPacketParseName));
        nRet = ACE_OS::dlclose(pPacketParseInfo->m_hModule);
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo]PacketID=%d, ret=%d.\n", pPacketParseInfo->m_u4PacketParseID, nRet));
        SAFE_DELETE(pPacketParseInfo);
        return false;
    }

    pPacketParseInfo->DisConnect = (void(*)(uint32))ACE_OS::dlsym(pPacketParseInfo->m_hModule, "DisConnect");

    if(nullptr == pPacketParseInfo->m_hModule || !pPacketParseInfo->DisConnect)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, Function DisConnect is error!\n", szPacketParseName));
        nRet = ACE_OS::dlclose(pPacketParseInfo->m_hModule);
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo]PacketID=%d, ret=%d.\n", pPacketParseInfo->m_u4PacketParseID, nRet));
        SAFE_DELETE(pPacketParseInfo);
        return false;
    }

    pPacketParseInfo->Close = (void(*)())ACE_OS::dlsym(pPacketParseInfo->m_hModule, "Close");

    if (nullptr == pPacketParseInfo->m_hModule || !pPacketParseInfo->Close)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, Function Close is error!\n", szPacketParseName));
        nRet = ACE_OS::dlclose(pPacketParseInfo->m_hModule);
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo]PacketID=%d, ret=%d.\n", pPacketParseInfo->m_u4PacketParseID, nRet));
        SAFE_DELETE(pPacketParseInfo);
        return false;
    }

    //��ӵ�HashPool����
    char szPacketID[10] = { '\0' };
    sprintf_safe(szPacketID, 10, "%d", pPacketParseInfo->m_u4PacketParseID);
    int32 nHashPos = m_objPacketParseList.Add_Hash_Data(szPacketID, pPacketParseInfo);

    if (nHashPos < 0)
    {
        SAFE_DELETE(pPacketParseInfo);
        return false;
    }
    else
    {
        return true;
    }
}

_Packet_Parse_Info* CLoadPacketParse::GetPacketParseInfo(uint32 u4PacketParseID)
{
    char szPacketID[10] = { '\0' };
    sprintf_safe(szPacketID, 10, "%d", u4PacketParseID);

    _Packet_Parse_Info* pPacketParseInfo = m_objPacketParseList.Get_Hash_Box_Data(szPacketID);

    if(nullptr == pPacketParseInfo)
    {
        //�����ӡ�������
        OUR_DEBUG((LM_INFO, "[CLoadPacketParse::GetPacketParseInfo]can't find u4PacketParseID(%d).\n", u4PacketParseID));
    }

    return pPacketParseInfo;
}

void CLoadPacketParse::Close()
{
    //���������Ѵ��ڵ�ָ��
    vector<_Packet_Parse_Info*> vecPacketParseList;
    m_objPacketParseList.Get_All_Used(vecPacketParseList);

    uint32 u4Size = (uint32)vecPacketParseList.size();

    OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::Close]u4Size=%d.\n", u4Size));

    for (uint32 i = 0; i < u4Size; i++)
    {
        if (nullptr != vecPacketParseList[i] && nullptr != vecPacketParseList[i]->m_hModule)
        {
            vecPacketParseList[i]->Close();
            int nRet = ACE_OS::dlclose(vecPacketParseList[i]->m_hModule);
            OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::Close]PacketID=%d, ret=%d.\n", vecPacketParseList[i]->m_u4PacketParseID, nRet));
        }

        SAFE_DELETE(vecPacketParseList[i]);
    }

    vecPacketParseList.clear();

    m_objPacketParseList.Close();
}
