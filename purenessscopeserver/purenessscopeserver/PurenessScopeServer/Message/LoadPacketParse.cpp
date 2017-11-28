#include "LoadPacketParse.h"

CLoadPacketParse::CLoadPacketParse()
{
}

CLoadPacketParse::~CLoadPacketParse()
{
    OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::~CLoadPacketParse]\n"));
    //Close();
}

void CLoadPacketParse::Init(int nCount)
{
    Close();
    //初始化HashTable
    m_objPacketParseList.Init((int)nCount);
}

bool CLoadPacketParse::LoadPacketInfo(uint32 u4PacketParseID, uint8 u1Type, uint32 u4HeadLen, const char* pPacketParsePath, const char* szPacketParseName)
{
    int nRet = 0;
    //隐式加载PacketParse接口
    _Packet_Parse_Info* pPacketParseInfo = new _Packet_Parse_Info();

    if (NULL == pPacketParseInfo)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] pPacketParseInfo is NULL!\n"));
        return false;
    }

    pPacketParseInfo->m_u4PacketParseID   = u4PacketParseID;
    pPacketParseInfo->m_u1PacketParseType = u1Type;
    pPacketParseInfo->m_u4OrgLength       = u4HeadLen;
    char szFilePath[MAX_BUFF_500]         = { '\0' };
    sprintf_safe(szFilePath, MAX_BUFF_500, "%s%s", pPacketParsePath, szPacketParseName);
    pPacketParseInfo->m_hModule           = ACE_OS::dlopen((ACE_TCHAR*)szFilePath, RTLD_NOW);

    if(NULL == pPacketParseInfo->m_hModule)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, pModuleInfo->hModule is NULL(%s)!\n", szPacketParseName, ACE_OS::dlerror()));
        SAFE_DELETE(pPacketParseInfo);
        return false;
    }

    pPacketParseInfo->Parse_Packet_Head_Info = (bool(*)(uint32, ACE_Message_Block*, IMessageBlockManager*, _Head_Info*))ACE_OS::dlsym(pPacketParseInfo->m_hModule, "Parse_Packet_Head_Info");

    if(NULL == pPacketParseInfo->m_hModule || !pPacketParseInfo->Parse_Packet_Head_Info)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, Function Parse_Packet_Head_Info is error!\n", szPacketParseName));
        nRet = ACE_OS::dlclose(pPacketParseInfo->m_hModule);
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo]PacketID=%d, ret=%d.\n", pPacketParseInfo->m_u4PacketParseID, nRet));
        SAFE_DELETE(pPacketParseInfo);
        return false;
    }

    pPacketParseInfo->Parse_Packet_Body_Info = (bool(*)(uint32, ACE_Message_Block*, IMessageBlockManager*, _Body_Info*))ACE_OS::dlsym(pPacketParseInfo->m_hModule, "Parse_Packet_Body_Info");

    if(NULL == pPacketParseInfo->m_hModule || !pPacketParseInfo->Parse_Packet_Body_Info)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, Function Parse_Packet_Body_Info is error!\n", szPacketParseName));
        nRet = ACE_OS::dlclose(pPacketParseInfo->m_hModule);
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo]PacketID=%d, ret=%d.\n", pPacketParseInfo->m_u4PacketParseID, nRet));
        SAFE_DELETE(pPacketParseInfo);
        return false;
    }

    pPacketParseInfo->Parse_Packet_Stream = (uint8(*)(uint32, ACE_Message_Block*, IMessageBlockManager*, _Packet_Info*))ACE_OS::dlsym(pPacketParseInfo->m_hModule, "Parse_Packet_Stream");

    if(NULL == pPacketParseInfo->m_hModule || !pPacketParseInfo->Parse_Packet_Stream)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, Function Parse_Packet_Stream is error!\n", szPacketParseName));
        nRet = ACE_OS::dlclose(pPacketParseInfo->m_hModule);
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo]PacketID=%d, ret=%d.\n", pPacketParseInfo->m_u4PacketParseID, nRet));
        SAFE_DELETE(pPacketParseInfo);
        return false;
    }

    pPacketParseInfo->Make_Send_Packet = (bool(*)(uint32, const char*, uint32, ACE_Message_Block*, uint16))ACE_OS::dlsym(pPacketParseInfo->m_hModule, "Make_Send_Packet");

    if(NULL == pPacketParseInfo->m_hModule || !pPacketParseInfo->Make_Send_Packet)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, Function Make_Send_Packet is error!\n", szPacketParseName));
        nRet = ACE_OS::dlclose(pPacketParseInfo->m_hModule);
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo]PacketID=%d, ret=%d.\n", pPacketParseInfo->m_u4PacketParseID, nRet));
        SAFE_DELETE(pPacketParseInfo);
        return false;
    }

    pPacketParseInfo->Make_Send_Packet_Length = (uint32(*)(uint32, uint32, uint16))ACE_OS::dlsym(pPacketParseInfo->m_hModule, "Make_Send_Packet_Length");

    if(NULL == pPacketParseInfo->m_hModule || !pPacketParseInfo->Make_Send_Packet_Length)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, Function Make_Send_Packet_Length is error!\n", szPacketParseName));
        nRet = ACE_OS::dlclose(pPacketParseInfo->m_hModule);
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo]PacketID=%d, ret=%d.\n", pPacketParseInfo->m_u4PacketParseID, nRet));
        SAFE_DELETE(pPacketParseInfo);
        return false;
    }

    pPacketParseInfo->Connect = (bool(*)(uint32, _ClientIPInfo, _ClientIPInfo))ACE_OS::dlsym(pPacketParseInfo->m_hModule, "Connect");

    if(NULL == pPacketParseInfo->m_hModule || !pPacketParseInfo->Connect)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, Function Connect is error!\n", szPacketParseName));
        nRet = ACE_OS::dlclose(pPacketParseInfo->m_hModule);
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo]PacketID=%d, ret=%d.\n", pPacketParseInfo->m_u4PacketParseID, nRet));
        SAFE_DELETE(pPacketParseInfo);
        return false;
    }

    pPacketParseInfo->DisConnect = (void(*)(uint32))ACE_OS::dlsym(pPacketParseInfo->m_hModule, "DisConnect");

    if(NULL == pPacketParseInfo->m_hModule || !pPacketParseInfo->DisConnect)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, Function DisConnect is error!\n", szPacketParseName));
        nRet = ACE_OS::dlclose(pPacketParseInfo->m_hModule);
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo]PacketID=%d, ret=%d.\n", pPacketParseInfo->m_u4PacketParseID, nRet));
        SAFE_DELETE(pPacketParseInfo);
        return false;
    }

    pPacketParseInfo->Close = (void(*)())ACE_OS::dlsym(pPacketParseInfo->m_hModule, "Close");

    if (NULL == pPacketParseInfo->m_hModule || !pPacketParseInfo->Close)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, Function Close is error!\n", szPacketParseName));
        nRet = ACE_OS::dlclose(pPacketParseInfo->m_hModule);
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo]PacketID=%d, ret=%d.\n", pPacketParseInfo->m_u4PacketParseID, nRet));
        SAFE_DELETE(pPacketParseInfo);
        return false;
    }

    //添加到HashPool里面
    char szPacketID[10] = { '\0' };
    sprintf_safe(szPacketID, 10, "%d", pPacketParseInfo->m_u4PacketParseID);
    int nHashPos = m_objPacketParseList.Add_Hash_Data(szPacketID, pPacketParseInfo);

    if (nHashPos < 0)
    {
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

    if(NULL == pPacketParseInfo)
    {
        //这里打印输出错误
        OUR_DEBUG((LM_INFO, "[CLoadPacketParse::GetPacketParseInfo]can't find u4PacketParseID(%d).\n", u4PacketParseID));
    }

    return pPacketParseInfo;
}

void CLoadPacketParse::Close()
{
    //清理所有已存在的指针
    vector<_Packet_Parse_Info*> vecPacketParseList;
    m_objPacketParseList.Get_All_Used(vecPacketParseList);

    for (int i = 0; i < (int)vecPacketParseList.size(); i++)
    {
        if (NULL != vecPacketParseList[i] && NULL != vecPacketParseList[i]->m_hModule)
        {
            vecPacketParseList[i]->Close();
            int nRet = ACE_OS::dlclose(vecPacketParseList[i]->m_hModule);
            OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::Close]PacketID=%d, ret=%d.\n", vecPacketParseList[i]->m_u4PacketParseID, nRet));
        }

        SAFE_DELETE(vecPacketParseList[i]);
    }

    m_objPacketParseList.Close();
}
